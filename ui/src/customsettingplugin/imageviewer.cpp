#include "imageviewer.h"
#include "ui_imageviewer.h"
#include "common_data.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "./vscameratools/cameramanager/icamerabuffermanager.h"
#include "vscameratools/camera/ilinecamera.h"
#include "uicommon/commonutils.h"
#include "callsdk.h"
#include "rtdefine.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "g12arithmetic/iarithmeticStatic.h"
#include "filevariable/ifilevariable.h"
#include "g12commonui/idefectrecord.h"
#include "uicommon/commonutils.h"
#include "iuiplugin.h"
#include "motion/imotion.h"
#include "card/icardcontinterp.h"
#include "station/istation.h"
#include "g12utils.h"
#include "common_roi.h"
#include "stringcommon.h"
#include "cimageprocess.h"
#include <QFileDialog>
#include  "lightcontrol_new.h"

#define  TARGET_STATION     "target"//

ImageViewer::ImageViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageViewer),
    //m_pCurrentCamera(NULL),
    m_iCurrentSelect(0),
    m_iCurrentProType(0),
    m_pIArithmeticStaticMgr(NULL),
    m_pIArithmeticStatic(NULL),
    m_pCfgFile(NULL),
    m_imageFormat(QImage::Format_Invalid),
    m_pDefectRecord(NULL),
    m_iUnionWidth(8192),
    m_iUnionHeight(1),
    m_pCameraBufferManager(NULL)
{
    ui->setupUi(this);
    init();
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::init()
{

	IBufferManagerFactory* pIBufferManagerFactory = (IBufferManagerFactory*)GetSDKInstance("cameramanager", IBufferManagerFactory::IID_CAMERA_BUFFER_MANAGER_FACTORY);
	if (pIBufferManagerFactory != NULL)
	{
		m_pCameraBufferManager = pIBufferManagerFactory->createBufferManager("0");
	}

    if (NULL == m_pCameraBufferManager)
    {
        return;
    }
    m_strImageName = "E:\\image";
    m_pIArithmeticStaticMgr = (IArithmeticStaticMgr*)GetSDKInstance("g12arithmetic", IArithmeticStaticMgr::IDD_ARITHMETICSTATICMGR);
    m_pCfgFile = getCameraSetCfgFileVariable();
    for (int i = 0; i < SEQUE_NUMBER; i++)
    {
        m_imageInfoBuffer[i] = new stImageInfo;
    }

    ui->comboBox_channel->clear();
    QString strSeqName = getCurrentProgramRecipesList().c_str();
    QStringList strSeqList = strSeqName.split(",");
    for (int i = 0; i < strSeqList.size(); ++i)
    {
        ui->comboBox_channel->addItem(strSeqList[i]);
    }
    ui->comboBox_channel->setCurrentIndex(0);
    ui->widget_show_pic->showStatusBar(XTV_IMAGE_SHOW::VS_STATUS_SHOW_ALL);
    ui->widget_show_pic->showToolBar(XTV_IMAGE_SHOW::VS_TOOLBAR_IMAGEZOOM);
    ui->widget_show_pic->showActionItem(XTV_IMAGE_SHOW::VS_ACTIONRECTANGLE | XTV_IMAGE_SHOW::VS_ACTIONERASE);
    IUIPlugin *pUiPligin = CommonUtils::getIUIPlugin("g12commonui");
    if (NULL != pUiPligin)
    {
        pUiPligin->queryInterface(IDefectRecordInterface::IID_DEFECT_RECORD_INTERFACE, (void **)&m_pDefectRecord);
    }
    IFileVariable* pFileVariable = getAcquisitionFileVariable();
    if (pFileVariable != NULL && NULL != m_imageInfoBuffer)
    {
        m_iUnionWidth = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH, 8192).toInt();
        m_iUnionHeight = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT, 1).toInt();
        m_iImageCount = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_CNT).toInt();
        pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT, &m_iUnionHeight);
        pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH, &m_iUnionWidth);
        pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_CNT, &m_iImageCount);
    }
    m_pTriggerThread = new JThreadTP<ImageViewer, void *, void>;

    m_waitDlg.setWindowTitle(QString::fromLocal8Bit("图像采集"));
    m_waitDlg.addButton(QMessageBox::Cancel);

    connect(this, SIGNAL(signalCancelWait()), this, SLOT(slotWaitCancel()));
    connect(this, SIGNAL(signalFinished()), this, SLOT(slotWaitFinished()));
    connect(m_waitDlg.button(QMessageBox::Cancel), SIGNAL(clicked()), this, SLOT(slotWaitCancel()));
    connect(&m_waitDlg, SIGNAL(finished(int)), this, SLOT(slotWaitCancel(int)));
}

void ImageViewer::on_pushButton_load_pic_clicked(bool checked)
{
    m_strImageName = QFileDialog::getOpenFileName(this,
        tr("Open Factors File"),
        m_strImageName,
        "Image Files(*.jpg *.png *.bmp)",
        0);
    QImage* img = new QImage;
    if (!img->load(m_strImageName)) //加载图像
    {
        //如果加载失败，则提示并返回
        //QMessageBox::information(this,tr("打开图像失败"),tr("打开图像失败!"));
        delete img;
        return;
    }
    QFileInfo fileInfo(m_strImageName);
    ui->widget_show_pic->setSaveImageFileName(fileInfo.fileName());

    ui->label_path->setText(m_strImageName);
    QImage::Format format = img->format();

    //根据文件名称自动切通道索引
    string strList = getCurrentProgramRecipesList();
    vector<string> strNameList = SString::split(strList, ",");
    int index = ui->comboBox_channel->currentIndex();
    for (int i = 0; i < strNameList.size(); i++)
    {
        if (m_strImageName.contains(strNameList[i].c_str(), Qt::CaseSensitive))
        {
            index = i;
            ui->comboBox_channel->setCurrentIndex(index);
            break;
        }
    }

    UpdateImageBuffer(img->width(), img->height(), format, index);
    stImageInfo *pImageBuffer = m_imageInfoBuffer[index];

    for (int i = 0; i < pImageBuffer->height; i++)
    {
        if (QImage::Format_Mono == format || QImage::Format_Indexed8 == format)
        {
            memcpy(pImageBuffer->pixelR + pImageBuffer->width*i, img->bits() + img->bytesPerLine() * i, pImageBuffer->width);
        }
        else
        {
            for (int j = 0; j < pImageBuffer->width; j++)
            {
                int p = j * 4;
                pImageBuffer->pixelB[i*pImageBuffer->width + j] = img->bits()[img->bytesPerLine() * i + p];
                pImageBuffer->pixelG[i*pImageBuffer->width + j] = img->bits()[img->bytesPerLine() * i + p + 1];
                pImageBuffer->pixelR[i*pImageBuffer->width + j] = img->bits()[img->bytesPerLine() * i + p + 2];
                pImageBuffer->pixelA[i*pImageBuffer->width + j] = img->bits()[img->bytesPerLine() * i + p + 3];
            }
        }
    }

    delete img;
    ImageProcess();
	ui->widget_show_pic->showPictureInLabel(pImageBuffer->pixelR, pImageBuffer->pixelG, pImageBuffer->pixelB, pImageBuffer->width, pImageBuffer->height);
}
void ImageViewer::GetCurrentChannel()
{
	int index = 0;
	if (ui->radioButton_A->isChecked()){
		index = 0;
	}
	else if(ui->radioButton_B->isChecked()){
		index = 1;
	}
	else if (ui->radioButton_C->isChecked()) {
		index = 2;
	}
	else if (ui->radioButton_D->isChecked()) {
		index = 3;
	}
	else if (ui->radioButton_E->isChecked()) {
		index = 4;
	}
	else if (ui->radioButton_F->isChecked()) {
		index = 5;
	}
	else if (ui->radioButton_G->isChecked()) {
		index = 6;
	}
	else if (ui->radioButton_H->isChecked()) {
		index = 7;
	}
	string strChannelNames = getCurrentProgramRecipesList().c_str();
	std::vector<std::string> ChannelNameList = SString::split(strChannelNames, ",");
	string Channe = ChannelNameList.at(index);

	//
	bool bIsChange = false;
	if (m_iCurrentSelect != index) 
	{
		if (index ==3 || m_iCurrentSelect == 3) //D //
		{
			bIsChange = true;
		}
	}
	m_iCurrentSelect = index;
	m_currentChName  = Channe;


	//清除
	if (bIsChange)
	{
		QList<QVariant> dataList = ui->widget_show_pic->queryShapeItem();
		ui->widget_show_pic->removeShapeItem(0, true);
		QList<QVariant>::iterator itL = dataList.begin();
		for (; itL != dataList.end(); itL++)
		{
			QList<QVariant> dataArray = itL->toList();
			if (dataArray.size() > 5)
			{
				QPoint localP1 = dataArray.at(5).toPoint();
				QPoint localP2 = dataArray.at(6).toPoint();
				QRect rect;
				rect.setTopLeft(localP1);
				rect.setBottomRight(localP2);
				//rect.setWidth();
				//转化后
				rect = getMatrixRect(rect);
				QList<QVariant> dataList; dataList << EnShape_Rect1 << rect.center() << rect.width() << rect.height() << 0 << rect.topLeft() << rect.bottomRight();
				ui->widget_show_pic->addShapeItem(dataList);
			}
		}
	}
	//
}
void ImageViewer::on_pushButton_get_online_pic_clicked(bool checked)
{
    if (!m_pCameraBufferManager->isInitFinished())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("执行失败"), QString::fromLocal8Bit("相机未初始化成功， 请检测硬件或配置是否正常"));
		return;
    }
    int sn = 0;
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if (NULL != pFile)
    {
        sn = pFile->getValue(WORK_PRODUCT_SN).toInt();
    }
	GetCurrentChannel();//add 20190917
    QString strFileName = QString("%1-%2").arg(sn).arg(QString::fromStdString(m_currentChName));
    ui->widget_show_pic->setSaveImageFileName(strFileName);

    int index = m_iCurrentSelect;

    stImageInfo *pImageBuffer = NULL;
    m_pCameraBufferManager->getBufferSize(m_iUnionWidth, m_iUnionHeight);
    UpdateImageBuffer(m_iUnionWidth, m_iUnionHeight);
    //for (index = 0; index < SEQUE_NUMBER; index++)
    //{
    //    const unsigned char *pSourceBuf = m_pCameraBufferManager->getBuffer(index);
    //    if (NULL == pSourceBuf)
    //    {
    //        continue;
    //    }
    //    if (width != m_imageInfoBuffer[index]->width || height != m_imageInfoBuffer[index]->height)
    //    {
    //        if (NULL != m_imageInfoBuffer[index]->pixelR);
    //        {
    //            delete []m_imageInfoBuffer[index]->pixelR;
    //            m_imageInfoBuffer[index]->pixelR = NULL;
    //        }
    //        m_imageInfoBuffer[index]->width = width;
    //        m_imageInfoBuffer[index]->height = height;
    //        if (NULL == m_imageInfoBuffer[index]->pixelR);
    //        {
    //            m_imageInfoBuffer[index]->pixelR = new unsigned char[width * height];
    //        }
    //    }
    //    pImageBuffer = m_imageInfoBuffer[index];
    //    memcpy(pImageBuffer->pixelR , pSourceBuf, m_iUnionWidth * m_iUnionHeight);
    //}
	const unsigned char *pSourceBuf = m_pCameraBufferManager->getBuffer(index);
	if (NULL == pSourceBuf)
	{ 
		return;
	}
	if (NULL == m_imageInfoBuffer[index]->pixelR);
	{
		m_imageInfoBuffer[index]->pixelR = new unsigned char[m_iUnionWidth * m_iUnionHeight];
	}
	pImageBuffer = m_imageInfoBuffer[index];
	memcpy(pImageBuffer->pixelR, pSourceBuf, m_iUnionWidth * m_iUnionHeight);
	m_iCurrentProType = ui->comboBox_pro_type->currentIndex();
	ImageProcess();
	ui->widget_show_pic->showPictureInLabel(pImageBuffer->pixelR, pImageBuffer->pixelG, pImageBuffer->pixelB, pImageBuffer->width, pImageBuffer->height);
}

void ImageViewer::on_comboBox_channel_currentIndexChanged(int index)
{
    bool bIsReversal = false;
    bool bIsChange = false;
    if (m_iCurrentSelect == 3)
    {
        if (index != 3)
        {
            bIsReversal = true;
            bIsChange = true;
        }
    }
    else
    {
        if (index == 3)
        {
            bIsChange = true;
        }
    }
    m_iCurrentSelect = index;
    m_currentChName = ui->comboBox_channel->currentText().toLocal8Bit().data();
    if (m_pIArithmeticStaticMgr != NULL)
    {
        m_pIArithmeticStatic = m_pIArithmeticStaticMgr->getArithmeticStatic(m_currentChName);
    }
    //清除
    if (bIsChange)
    {
        QList<QVariant> dataList = ui->widget_show_pic->queryShapeItem();
        ui->widget_show_pic->removeShapeItem(0, true);
        QList<QVariant>::iterator itL = dataList.begin();
        for (; itL != dataList.end(); itL++)
        {
            QList<QVariant> dataArray = itL->toList();
            if (dataArray.size() > 5)
            {
                QPoint localP1 = dataArray.at(5).toPoint();
                QPoint localP2 = dataArray.at(6).toPoint();
                QRect rect;
                rect.setTopLeft(localP1);
                rect.setBottomRight(localP2);
                //rect.setWidth();
                //转化后
                rect = getMatrixRect(rect);
                QList<QVariant> dataList; dataList << EnShape_Rect1 << rect.center() << rect.width() << rect.height() << 0 << rect.topLeft() << rect.bottomRight();
                ui->widget_show_pic->addShapeItem(dataList);
            }
        }
    }
    //QList<QVariant>dataList;
    //dataList << type() << mapToScene(m_rect.center()) << m_rect.width() << m_rect.height() << 0 << mapToScene(m_rect.topLeft()) << mapToScene(m_rect.bottomRight());
}

void ImageViewer::on_comboBox_pro_type_currentIndexChanged(int index)
{
    m_iCurrentProType = index;
    ImageProcess();
}

void ImageViewer::on_pushButton_static_clicked(bool checked)
{
    if (!m_pCameraBufferManager->isInitFinished())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("执行失败"), QString::fromLocal8Bit("相机未初始化成功， 请检测硬件或配置是否正常"));
    }
    //清计数
//    if (!lightcontrol::GetInstance()->ClearNumber())
//    {
//        QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("清计数失败失败！"));
//    }
    //uint8_t  ls_config_id = 0;
    //if (0 != lightcontrol_new::GetInstance()->clearCounter(ls_config_id))
    //{
    //     QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit(" network通道清除计数失败!"));

    //}

    //触发采集
    bool bRet = m_pCameraBufferManager->startProcess();

    m_waitDlg.setText(QString::fromLocal8Bit("静态采图中..."));
    //m_waitDlg.setStandardButtons(0);
    m_waitDlg.show();

    SetUIEnable(false);
    //起线程触发;
    m_bRunning = true;
    m_pTriggerThread->StartThread(this, &ImageViewer::ThreadTrigger, this);
}

void ImageViewer::ThreadTrigger(void* param)
{
	//iMotion *pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
	//RETURN_IF(NULL == pMotion);
	//ICard *pCard = (ICard *)pMotion->GetCardInstance(CONTROLCARD_NAME);
	//RETURN_IF(NULL == pCard);
	//ICardContInterp *pCardConInterp = dynamic_cast<ICardContInterp*>(pCard);
	//iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion", iMotionCfg::IID_MOTION_CONFIG);
	//RETURN_IF(NULL == pMotionCfg);
	//StCardInfo cardInfo;
	//pMotionCfg->getCardInfo("card", cardInfo);
	//bool bIsGGCard = false;
	//if (0 == strcmp(cardInfo.cardType, "cardgg"))
	//{
	//	bIsGGCard = true;
	//}
	//
	////设等间距触发
	//IFileVariable *pFile = getCameraSetCfgFileVariable();
	//double dSpeed = 0;
	//double dMoveUnit = 0;
	//double dPluseWidgh = 0;
	//
	//if (pFile)
	//{
	//	dSpeed = pFile->getValue(CAMERAPARAM_SPEED).toDouble();
	//	dMoveUnit = pFile->getValue(CAMERAPARAM_MOVEUNIT).toDouble();
	//	dPluseWidgh = pFile->getValue(CAMERAPARAM_PLUSEWIDGH).toDouble();
	//}
	//int iSequeNumber = getSequenceNumber();
	//double dunit = dMoveUnit / double(iSequeNumber);//间距 = 精度/时序
	//int CamLines = m_iUnionHeight;
	//int iNumber = iSequeNumber*CamLines;
	//
	//if (bIsGGCard)
	//{
	//	IStation* pTargetStation = pMotion->GetStationInstance(TARGET_STATION);
	//
	//	int Encoder = pTargetStation->GetAxisInfo(0).axisIndex + 1;
	//
	//	do
	//	{
	//		pCardConInterp->SetComparePluse(2, 0, dPluseWidgh * 1000);
	//		usSleep(80);
	//		iNumber--;
	//	} while (iNumber > 0 && m_bRunning);
	//	m_bRunning = false;
	//}
	//else
	//{
	//	pCardConInterp->SetComparePluse(0, 1, iNumber);
	//	short istu = 0;//触发是否完成 值1为完成 值 0为正在进行
	//	long  ldata[2] = { 0 };
	//	bool res = false;
	//	Sleep(150);
	//	do
	//	{
	//		pCardConInterp->GetCompareStatus(istu, ldata);
	//		if (istu == 1)
	//			break;
	//	} while (m_bRunning);
	//	pCardConInterp->stopComparePos(0);
	//}
    //
    //SaveStaticImage();
    ////触发完成后通知取图
    //emit ui->pushButton_get_online_pic->clicked(true);
    //emit signalFinished();
}

void ImageViewer::ImageProcess()
{
    bool bStaticShading = false;
    bool bDynamicShading = false;
    bool bSignalFilter = false;
    if (m_pCfgFile != NULL)
    {
        bStaticShading = m_pCfgFile->getValue(IMAGE_PARAM_STATIC_ENABLE).toBool();
        bDynamicShading = m_pCfgFile->getValue(IMAGE_PARAM_DYNAMIC_ENABLE).toBool();
        bSignalFilter = m_pCfgFile->getValue(IMAGE_PARAM_FILTER_ENABLE).toBool();
    }
    int index = ui->comboBox_channel->currentIndex();
    RETURN_IF(index > SEQUE_NUMBER || NULL == m_imageInfoBuffer[index]);

    stImageInfo *pImageBuffer = m_imageInfoBuffer[index];
    int width = pImageBuffer->width;
    int height = pImageBuffer->height;
    stImageInfo imageLocal;
    imageLocal.width = width;
    imageLocal.height = height;
    unsigned char *pSrcBuffer = new unsigned char[width * height];
    unsigned char *pDstBuffer = new unsigned char[width * height];
    memcpy(pSrcBuffer, pImageBuffer->pixelR, width*height);
    imageLocal.pixelR = pSrcBuffer;

    //静态校准
    if (NULL != m_pIArithmeticStatic && m_iCurrentProType > 0 && bStaticShading)
    {
        RETURN_IF(0 > m_pIArithmeticStatic->iniStatCorr(width, height));
		RETURN_IF(0 > m_pIArithmeticStatic->StatCorr(pSrcBuffer, pDstBuffer));

        imageLocal.pixelR = pDstBuffer;
        pDstBuffer = pSrcBuffer;
        pSrcBuffer = imageLocal.pixelR;
    }
}

void ImageViewer::clearImageBuffer(stImageInfo *pImageBuffer)
{
    if (NULL != pImageBuffer->pixelR)
    {
        delete[]pImageBuffer->pixelR;
        pImageBuffer->pixelR = NULL;
    }
    if (NULL != pImageBuffer->pixelG)
    {
        delete[]pImageBuffer->pixelR;
        pImageBuffer->pixelR = NULL;
    }
    if (NULL != pImageBuffer->pixelB)
    {
        delete[]pImageBuffer->pixelR;
        pImageBuffer->pixelR = NULL;
    }
    if (NULL != pImageBuffer->pixelA)
    {
        delete[]pImageBuffer->pixelR;
        pImageBuffer->pixelR = NULL;
    }
    pImageBuffer->width = 0;
    pImageBuffer->height = 0;
}

void ImageViewer::UpdateImageBuffer(int width, int height, QImage::Format format/* = QImage::Format_Mono*/, int index/* = -1*/)
{
    for (int i = 0; i < SEQUE_NUMBER; i++)
    {
        if (index > 0 && i != index)
        {
            continue;
        }
        stImageInfo *pImageBuffer = m_imageInfoBuffer[i];
        if (pImageBuffer->width != width
            || pImageBuffer->height != height
            || m_imageFormat != format)
        {
            if (NULL != pImageBuffer->pixelR)
            {
                delete[]pImageBuffer->pixelR;
                pImageBuffer->pixelR = NULL;
            }
            if (NULL != pImageBuffer->pixelG)
            {
                delete[]pImageBuffer->pixelG;
                pImageBuffer->pixelG = NULL;
            }
            if (NULL != pImageBuffer->pixelB)
            {
                delete[]pImageBuffer->pixelB;
                pImageBuffer->pixelB = NULL;
            }
            if (NULL != pImageBuffer->pixelA)
            {
                delete[]pImageBuffer->pixelA;
                pImageBuffer->pixelA = NULL;
            }
        }
        pImageBuffer->width = width;
        pImageBuffer->height = height;
        m_imageFormat = format;
        if (NULL == pImageBuffer->pixelR)
        {
            pImageBuffer->pixelR = new unsigned char[pImageBuffer->width*pImageBuffer->height];
        }
        if (QImage::Format_RGB32 == format)
        {
            if (NULL == pImageBuffer->pixelG)
            {
                pImageBuffer->pixelG = new unsigned char[pImageBuffer->width*pImageBuffer->height];
            }
            if (NULL == pImageBuffer->pixelB)
            {
                pImageBuffer->pixelB = new unsigned char[pImageBuffer->width*pImageBuffer->height];
            }
            if (NULL == pImageBuffer->pixelA)
            {
                pImageBuffer->pixelA = new unsigned char[pImageBuffer->width*pImageBuffer->height];
            }
        }
    }
}

void ImageViewer::SetUIEnable(bool bEnable)
{
    ui->pushButton_load_pic->setEnabled(bEnable);
    ui->pushButton_get_online_pic->setEnabled(bEnable);
    ui->comboBox_channel->setEnabled(bEnable);
    ui->comboBox_pro_type->setEnabled(bEnable);
    ui->pushButton_static->setEnabled(bEnable);
}

void ImageViewer::slotWaitCancel()
{
    if (NULL != m_pCameraBufferManager)
    {
        m_pCameraBufferManager->abort();
    }
    m_bRunning = false;
    SetUIEnable(true);
}
void ImageViewer::slotWaitCancel(int iR)
{
    slotWaitCancel();
}

void ImageViewer::slotWaitFinished()
{
    m_waitDlg.hide();
    SetUIEnable(true);
}

QRect ImageViewer::getMatrixRect(QRect source, bool bIsReversal/* = false*/, int indexGroup /*= 0*/, int indexItem /*= 0*/)
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

void ImageViewer::SaveStaticImage()
{
    int width = 0;
    int height = 0;
    QString strSeqName = getCurrentProgramRecipesList().c_str();
    QStringList strSeqList = strSeqName.split(",");
    m_pCameraBufferManager->getBufferSize(width, height);
    unsigned char *szBuffer = new unsigned char[width * height];
    for (int index = 0; index < SEQUE_NUMBER; index++)
    {
        const unsigned char *pSourceBuf = m_pCameraBufferManager->getBuffer(index);
        if (NULL == pSourceBuf)
        {
            continue;
        }

        memcpy(szBuffer, pSourceBuf, width * height);

        string  strPath = "D:\\IMAGE";
        IFileVariable* pfileVariable = getCameraSetCfgFileVariable();
        if (pfileVariable != NULL)
        {
            strPath = pfileVariable->getValue("image", "savepath", strPath.c_str()).toCString();
        }
        JTime curTime = JTime::CurrentLocalDateTime();
	
        QString strSavePath = QString("%1\\%2-%3-%4").arg(strPath.c_str()).arg(curTime.year()).arg(curTime.month(), 2, 10, QChar('0')).arg(curTime.day(), 2, 10, QChar('0'));
        QDir dir(strSavePath);
        if (!dir.exists(strSavePath))
        {
            dir.mkdir(strSavePath);
        }
        strSavePath = strSavePath + "\\" + "static";
        QDir dir2(strSavePath);
        if (!dir2.exists(strSavePath))
        {
            dir2.mkdir(strSavePath);
        }
        strSavePath = strSavePath + "\\" + strSeqList[index];
        strSavePath += ".bmp";
        CImageProcess::SaveImageToFile(szBuffer, width, height, strSavePath);
    }
    delete[]szBuffer;
    szBuffer = NULL;
}
