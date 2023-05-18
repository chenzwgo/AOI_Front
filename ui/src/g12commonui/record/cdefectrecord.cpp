#include "cdefectrecord.h"
#include "ui_cdefectrecord.h"
#include "cimageprocess.h"
#include "expressionparser/Features.h"
#include "g12common.h"
#include "maindefine.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "g12globalenv.h"

CDefectRecord *CDefectRecord::m_pInstance = NULL;

CDefectRecord *CDefectRecord::getInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CDefectRecord();
    }
    return m_pInstance;
}

void CDefectRecord::releaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

CDefectRecord::CDefectRecord(QWidget *parent) :
    ui(new Ui::CDefectRecord)
{
    ui->setupUi(this);
    init();
}

CDefectRecord::~CDefectRecord()
{
    delete ui;
}

void CDefectRecord::setDefectParam(stDefectFeature *param)
{
    if (NULL != param)
    {
        m_param = *param;
        m_param.iRealDefectLevel = param->iDefectLevel;
        QPoint point;
        point.setX(m_param.posx);
        point.setY(m_param.posy);
        if (NULL != m_pImage)
        {
            //获取缺陷对应图片所在位置
            int width = m_pImage[0].width;
            int height = m_pImage[0].height;
            int defectWidth = m_defectImageWidth;
            int defectHeight = m_defectImageHeight;
            if (defectWidth < m_param.dWide)
            {
                defectWidth = m_param.dWide;
            }
            if (defectHeight < m_param.dWide)
            {
                defectHeight = m_param.dHigh;
            }
            QRect rect = getImageRoi(m_pImage[0].width, m_pImage[0].height, point, defectWidth, defectHeight);
            //TBM
            if (m_param.iCameraIndex == 0)
            {
                m_rect[0] = rect;
                m_rect[1] = getMatrixRect(rect);
                m_rect[1] = getValidRect(m_rect[1], width, height);
            }
            else
            {
                m_rect[1] = rect;
                m_rect[0] = getMatrixRect(rect, true);
                m_rect[0] = getValidRect(m_rect[0], width, height);
            }
            for (int i = 0; i < SEQUE_NUMBER; ++i)
            {
                if (NULL != m_pImage[i].pixelR)
                {
                    int index = 0;
                    int iType = 0;
                    //获取通道名称列表， 并通过当前通道名称获取相机索引
                    QString strChannelNameList = getCurrentProgramRecipesList().c_str();
                    if (strChannelNameList.isEmpty())
                    {
                        strChannelNameList = "SEQUENCE_A,SEQUENCE_B,SEQUENCE_C,SEQUENCE_D,SEQUENCE_E,SEQUENCE_F,SEQUENCE_G,SEQUENCE_H";
                    }
                    QStringList ChannelNameList = strChannelNameList.split(",");
                    getSqueLightTypeAndCameraIndex(ChannelNameList[i].toStdString(), iType, index);
                    if (index > CAMERA_NUMBER) 
                    {
                        continue;
                    }

                    //如果内存大小有变化则重新申请
                    if (m_defectImageBuffer[i].width != defectWidth || m_defectImageBuffer[i].height != defectHeight)
                    {
                        if (NULL != m_defectImageBuffer[i].pixelR)
                        {
                            delete []m_defectImageBuffer[i].pixelR;
                            m_defectImageBuffer[i].pixelR = NULL;
                        }
                    }

                    if (NULL == m_defectImageBuffer[i].pixelR)
                    {
                        m_defectImageBuffer[i].pixelR = new unsigned char[defectWidth*defectHeight];
                    }
                    m_defectImageBuffer[i].width = defectWidth;
                    m_defectImageBuffer[i].height = defectHeight;
                    const unsigned char *pTmpBuffer = NULL;
                    for (int iH = 0; iH < defectHeight; ++iH)
                    {
                        unsigned long long iOffset = (m_rect[index].y() + iH)*m_pImage[i].width + m_rect[index].x();
                        pTmpBuffer = m_pImage[i].pixelR + iOffset;
                        memcpy(m_defectImageBuffer[i].pixelR + iH*defectWidth, pTmpBuffer, defectWidth);
                    }
                }
            }
        }
    }
}

void CDefectRecord::clearImagePtr()
{
    for (int i = 0; i < SEQUE_NUMBER; ++i)
    {
        m_pImage[i].pixelR = NULL; //防止多重释放
    }
}

void CDefectRecord::setImageAddr(ImageSrcStrut **pImage)
{
    for (int i = 0; i < SEQUE_NUMBER; ++i)
    {
        if (NULL != pImage[i])
        {
            m_pImage[i].pixelR = pImage[i]->imageAddr;
            m_pImage[i].width = pImage[i]->iImageWide;
            m_pImage[i].height = pImage[i]->iImageHight;
        }
    }
}

void CDefectRecord::setImageAddr(stImageInfo *pImage, int index)
{
    if (index < SEQUE_NUMBER && NULL != pImage)
    {
        m_pImage[index].pixelR = pImage->pixelR;
        m_pImage[index].width = pImage->width;
        m_pImage[index].height = pImage->height;
    }
}

void CDefectRecord::saveDefectParam()
{
    //保存;
    QString strFile = "";
    QString strDir = "./record";
    QDir dir;
    if (!dir.exists(strDir))
    {
        dir.mkdir(strDir);
    }
    JTime currentTime = JTime::CurrentLocalDateTime();
    QString strFileName = QString("%1/%2-%3-%4").arg(strDir).arg(currentTime.year()).arg(currentTime.month()).arg(currentTime.day());
    if (!dir.exists(strDir))
    {
        dir.mkdir(strDir);
    }
    strDir = QString("%1/%2").arg(strDir).arg(m_param.sn.c_str());
    if (!dir.exists(strDir))
    {
        dir.mkdir(strDir);
    }
    strDir = QString("%1/%2").arg(strDir).arg(m_param.iMarkIndex);
    if (!dir.exists(strDir))
    {
        dir.mkdir(strDir);
    }
    for (int i = 0; i < SEQUE_NUMBER; ++i)
    {
        if (NULL != m_defectImageBuffer[i].pixelR)
        {
            strFile = QString("%1/%2_%3.bmp").arg(strDir).arg(m_param.iMarkIndex).arg('A' + i);
            CImageProcess::SaveImageToFile(m_defectImageBuffer[i].pixelR, m_defectImageBuffer[i].width, m_defectImageBuffer[i].height, strFile);
        }
    }
    //保存参数
    strFile = QString("%1/%2_%3.bmp").arg(strDir).arg(m_param.iMarkIndex).arg('A' + m_param.iChannelType);
    saveDefectInfo(strFile);
}

void CDefectRecord::saveDefectInfo(QString strImagePath)
{
    IFileVariable *pUserSettingFile = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
    int x = 0;
    int y = 0;
    int iGray = 0;
    if (NULL != pUserSettingFile)
    {
        int iPointSize = pUserSettingFile->getValue(LOG_SAVE_GRAY_COUNT, 0).toInt();
        if (!m_csvRecordFile.isOpen())
        {
            QString strDir = "./record";
            QDir dir;
            if (!dir.exists(strDir))
            {
                dir.mkdir(strDir);
            }
            JTime currentTime = JTime::CurrentLocalDateTime();
            QString strFileName = QString("%1/%2-%3-%4.csv").arg(strDir).arg(currentTime.year()).arg(currentTime.month()).arg(currentTime.day())/*.arg(currentTime.hour()).arg(currentTime.minute()).arg(currentTime.second())*/;
            m_csvRecordFile.setFileName(strFileName);
            bool bIsExsit = m_csvRecordFile.exists();
            m_csvRecordFile.open(QIODevice::Append | QIODevice::Text);
            if (!bIsExsit)
            {
                QString strTitle1 = QString::fromLocal8Bit("通道名称, 产品序号, 缺陷检出级别, 缺陷实际等级,检出缺陷类别,实际缺陷类别,面积,对比度,分类分数,检测分数,宽度,高度,X,Y,图像路径\r");
                m_csvRecordFile.write(strTitle1.toLocal8Bit());
                m_csvRecordFile.flush();
            }
        }
        static int iDataCount = 0;
        QString strData = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15\r") \
            .arg(m_param.strChannelName.c_str())
            .arg(m_param.sn.c_str())
            .arg(m_param.iDefectLevel)
            .arg(m_param.iRealDefectLevel)
            .arg(m_param.iDefectType)
            .arg(m_param.iRealType)
            .arg(QString("%1\\%2").arg(m_param.iSize).arg(m_param.dSizeArea))
            .arg(m_param.contrast)
            .arg(m_param.fscore)
            .arg(m_param.fdetect_score)
            .arg(m_param.dWide)
            .arg(m_param.dHigh)
            .arg(m_param.posx)
            .arg(m_param.posy)
            .arg(strImagePath);
        m_csvRecordFile.write(strData.toLocal8Bit());
        m_csvRecordFile.flush();
    }
}
void CDefectRecord::on_pushButton_save_clicked(bool clicked)
{
    m_param.iRealType = ui->comboBox_defect_list->currentIndex();
    m_param.iRealDefectLevel = ui->spinBox_defect_level->value();
    saveDefectParam();
}

void CDefectRecord::showEvent(QShowEvent *event)
{
    updateUI();
    QWidget::showEvent(event);
}

void CDefectRecord::init()
{
    //TBM 修改为从配置读取
    m_defectImageWidth = 512;
    m_defectImageHeight = 512;
    QWidget *pImageWidgetList[SEQUE_NUMBER];
    int index = 0;
    m_view[index++] = ui->widget_A;
    m_view[index++] = ui->widget_B;
    m_view[index++] = ui->widget_C;
    m_view[index++] = ui->widget_D;
    m_view[index++] = ui->widget_E;
    m_view[index++] = ui->widget_F;
    m_view[index++] = ui->widget_G;
    m_view[index] = ui->widget_H;
    //for (int i = 0; i < SEQUE_NUMBER; i++)
    //{
    //    m_scene[i] = new DiagramScene();
    //    m_view[i] = new GraphWidget(m_scene[i]);
    //    QHBoxLayout *pLayout = new QHBoxLayout();
    //    pLayout->addWidget(m_view[i]);
    //    pLayout->setContentsMargins(0, 0, 0, 0);
    //    pImageWidgetList[i]->setLayout(pLayout);
    //}
    //m_pCFeatures = CFeaturesMgr::getCFeaturesMgr()->getCFeatures(getCurrentProgramName());
    //string CategorysString = m_pCFeatures->getCategorysString();
    ui->comboBox_defect_list->clear();
    for (int i = 0; i < DEFECT_TYPE_SIZE; i++)
    {
        ui->comboBox_defect_list->addItem(QString::fromLocal8Bit(szdefectTypeCn[i]));
    }
    //QStringList CategorysList = QString::fromStdString(CategorysString).split(",");
    //ui->comboBox_defect_list->addItems(CategorysList);
}

void CDefectRecord::deinit()
{
    if (m_csvRecordFile.isOpen())
    {
        m_csvRecordFile.close();
    }
    clearImagePtr();
}

void CDefectRecord::updateUI()
{
    int indexR = 0;
    int indexC = 0;
    ui->spinBox_defect_level->setValue(m_param.iRealDefectLevel);
    QTableWidgetItem *pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setText(m_param.strChannelName.c_str());
    }
    pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setData(Qt::EditRole, m_param.iDefectLevel);
    }
    pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setText(QString::fromLocal8Bit(szdefectTypeCn[m_param.iDefectType]));
    }
    pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setText(QString("%1/%2").arg(m_param.iSize).arg(m_param.dSizeArea, 0, 'f', 3));
    }
    pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setData(Qt::EditRole, m_param.contrast);
    }
    pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setData(Qt::EditRole, m_param.fdetect_score);
    }
    pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setData(Qt::EditRole, m_param.fscore);
    }
    pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setData(Qt::EditRole, m_param.dWide);
    }
    pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setData(Qt::EditRole, m_param.dHigh);
    }
    pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setData(Qt::EditRole, m_param.posx);
    }
    pItem = ui->tableWidget_defect_param->item(indexR, indexC++);
    if (NULL != pItem)
    {
        pItem->setData(Qt::EditRole, m_param.posy);
    }

    for (int i = 0; i < SEQUE_NUMBER; ++i)
    {
        if (NULL != m_defectImageBuffer[i].pixelR)
        {
            m_view[i]->show();
            m_view[i]->showPictureInLabel(m_defectImageBuffer[i].pixelR, NULL, NULL, m_defectImageBuffer[i].width, m_defectImageBuffer[i].height);
        }
        else
        {
            m_view[i]->hide();
        }
    }
    update();
}

//void CDefectRecord::saveImage(unsigned char *pImage, QString strFile, const QRect &rect)
//{
//    unsigned char *szSaveImageBuffer = new unsigned char[rect.width()*rect.height()];
//
//    for (int i = 0; i < rect.height(); ++i)
//    {
//        memcpy(szSaveImageBuffer, pImage, rect.width());
//    }
//
//    CImageProcess::SaveImageToFile(szSaveImageBuffer, rect.width(), rect.height(), strFile);
//}

QRect CDefectRecord::getImageRoi(int imageWidth, int imageHeight, QPoint center, int resultWidth, int resultHeigth)
{
    QRect roiRect;
    int startX = 0;
    int startY = 0;
    startX = center.x() - resultWidth / 2;
    if (startX < 0)
    {
        startX = 0;
    }
    if (startX >= imageWidth - resultWidth)
    {
        startX = imageWidth - resultWidth;
    }

    startY = center.y() - resultHeigth / 2;
    if (startY < 0)
    {
        startY = 0;
    }
    if (startY >= imageHeight - resultHeigth)
    {
        startY = imageHeight - resultHeigth;
    }
    roiRect.setX(startX);
    roiRect.setY(startY);
    roiRect.setWidth(resultWidth);
    roiRect.setHeight(resultHeigth);
    return roiRect;
}

QRect CDefectRecord::getMatrixRect(QRect source, bool bIsReversal/* = false*/, int indexGroup /*= 0*/, int indexItem /*= 0*/)
{
    IFileVariable *pFileVariable = getCameraSetCfgFileVariable();
    QRect rect;
    if (NULL != pFileVariable)
    {
        double xscale = pFileVariable->getValue(CORRECTION_X_SCALE).toDouble();
        double yscale = pFileVariable->getValue(CORRECTION_Y_SCALE).toDouble();
        double xoffset = pFileVariable->getValue(CORRECTION_X_OFFSET).toDouble();
        double yoffset = pFileVariable->getValue(CORRECTION_Y_OFFSET).toDouble();
        bool bIsMirror = pFileVariable->getValue(CORRECTION_IS_MIRROR).toBool();
        QMatrix4x4 matix;
        matix.scale(xscale, yscale);
        matix.translate(xoffset, yoffset);
        if (bIsMirror)
        {
            matix.rotate(180, 0, 1, 0); //绕Y轴旋转180
        }
        if (!bIsReversal)
        {
            matix = matix.inverted();
        }
        rect = matix.mapRect(source);
    }
    return rect;
}

QRect CDefectRecord::getValidRect(QRect source, int width, int height)
{
    QRect rect;
    rect = source;
    if (source.x() < 0)
    {
        rect.setX(0);
    }
    if (source.y() < 0)
    {
        rect.setY(0);
    }
    if (source.x() + source.width() > width)
    {
        rect.setX(width - rect.width());
    }
    if (source.y() + source.height() > height)
    {
        rect.setY(height - rect.height());
    }
    return rect;
}
