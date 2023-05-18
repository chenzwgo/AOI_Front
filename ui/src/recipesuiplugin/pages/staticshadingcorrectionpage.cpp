#include "staticshadingcorrectionpage.h"
#include "uicommon/extendstackwidget.h"
#include "uicommon/commonutils.h"
#include "filevariable/ifilevariable.h"
#include "g12common.h"
#include "g12utils.h"
#include "stringcommon.h"
#include "uicommon/curvegraphwidget.h"
#include "cimageprocess.h"
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <string>
#include <QImage>
#include "g12arithmetic/iarithmeticStatic.h"
#include "callsdk.h"
#include "g12globalenv.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "vscameratools/cameramanager/icamerabuffermanager.h"
#include <QPlainTextEdit>
using namespace std;

#define CHART_VALUE_RAW           0
#define CHART_VALUE_CORRECTION    1
#define CHART_VALUE_BASE          2

StaticShadingCorrectionpage::StaticShadingCorrectionpage(QWidget *parent):AbstractPageWidget(parent)
	,m_pExtendStatic(NULL)
{
    m_pExtendStatic = new ExtendStackWidget(this);
    m_pExtendStatic->setTitleBtnWidth(60);

    string channels = getCurrentProgramRecipesList();
    vector<string> channellist = SString::split(channels,",");
    for (int index = 0;index <channellist.size();index++)
    {
        StaticPage* pStaticPage = new StaticPage(QString::fromStdString(channellist.at(index)));
        //pStaticPage->setEnabled(false);
        m_pExtendStatic->addWidget(QString::fromStdString(channellist.at(index)),pStaticPage);
       // pStaticPage->onEnable(false);
       // pStaticPage->setEnabled(false);
    }
    m_pExtendStatic->setCurrentWidget(0);
    QObject::connect(m_pExtendStatic,SIGNAL(currentChanged(int)),this,SLOT(on_currentChanged(int)));

}

void StaticShadingCorrectionpage::onEnable(bool enable)
{
    AbstractPageWidget* pWidget = qobject_cast<AbstractPageWidget*>( m_pExtendStatic->currentWidget());
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->onEnable(enable);
    pWidget->setEnabled(enable);

}

void StaticShadingCorrectionpage::onSave()
{
    AbstractPageWidget* pWidget = qobject_cast<AbstractPageWidget*>( m_pExtendStatic->currentWidget());
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->updateProductName(m_strProductName);
    pWidget->onSave();
}

void StaticShadingCorrectionpage::onCancel()
{
    AbstractPageWidget* pWidget = qobject_cast<AbstractPageWidget*>( m_pExtendStatic->currentWidget());
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->onCancel();
}

void StaticShadingCorrectionpage::onUpdate()
{
    AbstractPageWidget* pWidget = qobject_cast<AbstractPageWidget*>( m_pExtendStatic->currentWidget());
    if (pWidget == NULL)
    {
        return;
    }
    pWidget->updateProductName(m_strProductName);
    pWidget->onUpdate();
}



void StaticShadingCorrectionpage::on_currentChanged( int index )
{
    AbstractPageWidget* pWidget =  qobject_cast<AbstractPageWidget*>(m_pExtendStatic->widgetOf(index));
    if (pWidget != NULL)
    {
        pWidget->updateProductName(m_strProductName);
        pWidget->onUpdate();
    }
}

StaticPage::StaticPage( QString strKey,QWidget *parent /*= 0*/ ):
    AbstractPageWidget(parent),
	m_strChannelName(strKey),
    m_pIArithmeticStatic(NULL),
	ui(new Ui::StaticCorrectionForm)
{
    ui->setupUi(this);
    m_pStaticCurveGraph = new CurveGraph();
    m_pStaticCurveGraph->setXAxisLower(0);
    m_pStaticCurveGraph->setXAxisUpper(8196);
    m_pStaticCurveGraph->setYAxisLower(0);
    m_pStaticCurveGraph->setYAxisUpper(255);
    ui->scrollArea_chart->setWidget(m_pStaticCurveGraph);

    QObject::connect(ui->Btn_clearTextEdit,SIGNAL(clicked()),this,SLOT(on_clearTextEdit()));
    QObject::connect(ui->Btn_saveFactors,SIGNAL(clicked()),this,SLOT(on_saveFactors()));
    QObject::connect(ui->Btn_run,SIGNAL(clicked()),this,SLOT(on_runTeach()));
    QObject::connect(ui->Btn_loadPic2,SIGNAL(clicked()),this,SLOT(on_loadPic()));
	QObject::connect(ui->Btn_loadPic2, SIGNAL(clicked()), this, SLOT(on_loadPicSetFlag()));

    QObject::connect(ui->Btn_LoadFactors,SIGNAL(clicked()),this,SLOT(on_loadFactors()));
    //QObject::connect(ui->Btn_showChart,SIGNAL(clicked()),this,SLOT(on_showChart()));
    QObject::connect(ui->Btn_loadPic,SIGNAL(clicked()),this,SLOT(on_loadPic()));
    QObject::connect(ui->Btn_run2,SIGNAL(clicked()),this,SLOT(on_runCorrection()));
    QObject::connect(ui->Btn_saveCorImage,SIGNAL(clicked()),this,SLOT(on_saveCorImage()));
    IArithmeticStaticMgr* pIArithmeticStaticMgr = (IArithmeticStaticMgr*)GetSDKInstance("g12arithmetic",IArithmeticStaticMgr::IDD_ARITHMETICSTATICMGR);
    if (pIArithmeticStaticMgr != NULL)
    {
        m_pIArithmeticStatic = pIArithmeticStaticMgr->getArithmeticStatic(strKey.toStdString());
    }
    

    m_imageWidth = -1;
    m_imageHeight = -1;
    m_standardGray = -1;
    m_iStartOffset = 0;
    m_iEndOffset = 0;
    m_rawImage = NULL;
    m_correctImage = NULL;
	m_pFfactor = NULL;
    m_ifactorLength = -1;
    m_qImg = NULL;
    m_qcorrectImage = NULL;
    m_imageInfoBuffer = NULL;
    m_bflag = false;
}

StaticPage::~StaticPage()
{
    delete ui;   
}

void StaticPage::onEnable( bool enable)
{

}

void StaticPage::onSave()
{
    IFileVariable* pFileVariable =  getCurrentStaticCorrectVariable(m_strProductName.toStdString());
    if (pFileVariable != NULL)
    {
        int iDisEnbale= getUItemValue("checkBox_staticdisable",0).toInt();
        pFileVariable->setValue(m_strChannelName.toStdString(),STATICCORRECT_DISABLE,iDisEnbale,true);
        int istandardGray = getUItemValue("spinBox_grey",0).toInt();
        pFileVariable->setValue(m_strChannelName.toStdString(), STATICCORRECT_GREY, istandardGray, true);
        int iStart = getUItemValue("spinBox_start_offset", 0).toInt();
        pFileVariable->setValue(m_strChannelName.toStdString(), STATICCORRECT_START_OFFSET, iStart, true);
        int iEnd = getUItemValue("spinBox_end_offset", 0).toInt();
        pFileVariable->setValue(m_strChannelName.toStdString(), STATICCORRECT_END_OFFSET, iEnd, true);
		int iStartCol = getUItemValue("spinBox_start_offset_col", 0).toInt();
		pFileVariable->setValue(m_strChannelName.toStdString(), STATICCORRECT_START_OFFSET_COL, iStartCol, true);
		int iEndCol = getUItemValue("spinBox_end_offset_col", 0).toInt();
		pFileVariable->setValue(m_strChannelName.toStdString(), STATICCORRECT_END_OFFSET_COL, iEndCol, true);
    }
	reInitSDKFlag();
    /*
    int iacqCnt = getUItemValue("spinBox_acqCnt",0).toInt();
    int irowCnt = getUItemValue("spinBox_rowCnt",0).toInt();
    int startcol = getUItemValue("spinBox_startcol",0).toInt();
    int startcol2 = getUItemValue("spinBox_startcol2",0).toInt();
    pFileVariable->setValue(m_strChannelName.toStdString(),"acqCnt",iacqCnt,true);
    pFileVariable->setValue(m_strChannelName.toStdString(),"rowCnt",irowCnt,true);
    pFileVariable->setValue(m_strChannelName.toStdString(),"startcol",iacqCnt,true);
    pFileVariable->setValue(m_strChannelName.toStdString(),"startcol2",irowCnt,true);*/
}

void StaticPage::onCancel()
{
    onUpdate();
}

void StaticPage::onUpdate()
{
    IFileVariable* pFileVariable =  getCurrentStaticCorrectVariable(m_strProductName.toStdString());
    int iDisenble = 0;
    int iGray = 0;
    int iStart = 0;
    int iEnd = 0;
	int iStartCol = 0;
	int iEndCol = 0;
    if (pFileVariable != NULL)
    {
        iDisenble = pFileVariable->getValue(m_strChannelName.toStdString(),STATICCORRECT_DISABLE).toInt();
        iGray = pFileVariable->getValue(m_strChannelName.toStdString(), STATICCORRECT_GREY).toInt();
        iStart = pFileVariable->getValue(m_strChannelName.toStdString(), STATICCORRECT_START_OFFSET).toInt();
        iEnd = pFileVariable->getValue(m_strChannelName.toStdString(), STATICCORRECT_END_OFFSET).toInt();       
		iStartCol = pFileVariable->getValue(m_strChannelName.toStdString(), STATICCORRECT_START_OFFSET_COL).toInt();
		iEndCol = pFileVariable->getValue(m_strChannelName.toStdString(), STATICCORRECT_END_OFFSET_COL).toInt();
    }   
    setUIItemValue("checkBox_staticdisable",iDisenble);
    setUIItemValue("spinBox_grey", iGray);
    setUIItemValue("spinBox_start_offset", iStart);
    setUIItemValue("spinBox_end_offset", iEnd);
	setUIItemValue("spinBox_start_offset_col", iStartCol);
	setUIItemValue("spinBox_end_offset_col", iEndCol);
}


void StaticPage::on_clearTextEdit()
{
    ui->plainTextEdit->clear();
}

void StaticPage::on_saveFactors()
{    
    QString productDir =  QString::fromStdString(getCurrentProgramPath());
    productDir += "/" + m_strChannelName + ".fct" ;
    QFile file(productDir);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"warning",QString::fromLocal8Bit("Factor 保存失败"));
        return;
    }     

    QString str = ui->plainTextEdit->toPlainText();
    file.write(str.toLatin1(),str.length());
    file.close();
    QMessageBox::warning(this,"warning",QString::fromLocal8Bit("Factor 保存"));
	reInitSDKFlag();
    return;
}

void StaticPage::on_runTeach()
{
    if (m_imageWidth < 0 || m_imageHeight < 0)
    {
        QMessageBox::information(this, QString::fromLocal8Bit("异常"), QString::fromLocal8Bit("无图像数据"));
        return;
    }
    int istandardGray = getUItemValue("spinBox_grey",0).toInt();
    int iStartOffset = getUItemValue("spinBox_start_offset", 0).toInt();
    int iEndOffset = getUItemValue("spinBox_end_offset", 0).toInt();
	int iStartOffsetCol = getUItemValue("spinBox_start_offset_col", 0).toInt();
	int iEndOffsetCol = getUItemValue("spinBox_end_offset_col", 0).toInt();

	if (m_pFfactor == NULL)
	{
		m_pFfactor = new float[m_imageWidth];
		for (int i = 0; i < m_imageWidth; i++)
			m_pFfactor[i] = 1;
	}
    if (m_iEndOffset>m_imageHeight)
    {
        iEndOffset = m_imageHeight;
    }
    if(m_iStartOffset>iEndOffset)
    {
		m_iStartOffset = 0;
    }
	if (iEndOffsetCol == -1 || iEndOffsetCol > m_imageWidth)
	{
		iEndOffsetCol = m_imageWidth;
	}
	if (iStartOffsetCol > iEndOffsetCol)
	{
		iStartOffsetCol = 0;
	}
    if (m_pIArithmeticStatic != NULL)
    {
        int iret = m_pIArithmeticStatic->Teaching(m_rawImage,m_imageWidth,m_imageHeight, istandardGray, iStartOffset, iEndOffset, iStartOffsetCol,iEndOffsetCol, m_pFfactor);
        if (iret == 0)
        {
            m_bflag =true;
        }      
    }
    
	QStringList strFactor;
    for (int i = 0;i <m_imageWidth;i++)
    {
		strFactor.append(QString::number(m_pFfactor[i],'f',3));
    }
	QString data  = strFactor.join(",");

    ui->plainTextEdit->setPlainText(data);
    QMessageBox::warning(this,"warning",QString::fromLocal8Bit("Teach完成"));
    return;
}

void StaticPage::on_loadFactors()
{
    QString productDir =  "./programs/" + m_strProductName;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Factors File"), 
        productDir,
        "*.fct", 
        0);
    if (!fileName.isNull())
    {
        //fileName是文件名
        // fileName是文件名
        QByteArray iamgedata;
        QFile* file=new QFile(fileName); //file为二进制数据文件名
        file->open(QIODevice::ReadOnly);
        iamgedata = file->readAll();
        file->close();
        ui->plainTextEdit->setPlainText(iamgedata);
        QStringList strFactors = QString(iamgedata).split(',');
        //if (m_pfactor != NULL)
        //{
        //    delete m_pfactor;
        //    m_pfactor = NULL;
        //}
        //m_pfactor =  new int[m_factors.size()];
        //for (int i = 0;i < m_factors.size();i++)
        //{
        //    m_pfactor[i] = m_factors.at(i).toFloat();
        //}

		if (m_pFfactor != NULL)
		{
			delete m_pFfactor;
			m_pFfactor = NULL;
		}
		m_pFfactor = new float[strFactors.size()];
		for (int i = 0; i < strFactors.size(); i++)
		{
			m_pFfactor[i] = strFactors.at(i).toFloat();
		}
        //Load完成后更新因子;
    } 
    else
    {
        //点的是取消

    }
}


void StaticPage::on_showChart()
{
    //if (m_pStaticCurveGraph != NULL)
    //{
    //    m_pStaticCurveGraph->show();
    //}
}

void StaticPage::on_loadPic()
{
    QString productDir =  "./programs/" + m_strProductName;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Factors File"), 
        productDir,
        "Image Files(*.jpg *.png *.bmp)", 
        0);
    if (!fileName.isNull())
    {
        unsigned int iWidth;
        unsigned int iHeight;

        //if(!getImageSize(fileName.toStdString(),iWidth,iHeight))
        //{
        //    return ;
        //}

        if (m_qcorrectImage != NULL)
        {
            delete m_qcorrectImage;
            m_qcorrectImage = NULL;
        }

        if (m_qImg != NULL)
        {
            delete m_qImg;
            m_qImg = NULL;
        }

        m_qImg = new QImage(fileName);
        int ibyteCount = m_qImg->byteCount();
        int ibytesPerLine = m_qImg->bytesPerLine();
         m_imageHeight = m_qImg->height();
         m_imageWidth = m_qImg->width();
        m_rawImage = m_qImg->bits();
        QImage::Format fmt =m_qImg->format();

        updateVideo(CHART_VALUE_BASE);
    }
    else
    {
        // 点的是取消
    }
}

void StaticPage::on_runCorrection()
{
    unsigned char minGray,maxGray;
    double mean,devi;
    QVector<double> x;
    QVector<double> y;
    if (m_imageWidth < 0 || m_imageHeight < 0)
    {
        QMessageBox::information(this, QString::fromLocal8Bit("异常"), QString::fromLocal8Bit("无图像数据"));
        return;
    }
    if (m_correctImage != NULL)
    {
        delete m_correctImage;
        m_correctImage = NULL;
    }
    m_correctImage = new unsigned char[m_imageWidth*m_imageHeight];

    m_pIArithmeticStatic->iniStatCorr(m_imageWidth,m_imageHeight);
	//
	int iret = m_pIArithmeticStatic->StatCorr(m_rawImage, m_correctImage);
	if (iret < 0)
	{
		QMessageBox::information(this, "warning", "StatCorr StatCorr fail");
		return;
	}
	//
    //m_pIArithmeticStatic->StatCorr(m_rawImage,m_correctImage);
    m_pIArithmeticStatic->GetStatParam(m_correctImage,m_imageWidth,m_imageHeight,maxGray,minGray,mean,devi);

    setUIItemValue("label_mean",QString::number(mean,'f',4).toDouble());
    setUIItemValue("label_variance",QString::number(devi,'f',4).toDouble());
    setUIItemValue("label_max",QString::number(maxGray,'f',4).toInt());
    setUIItemValue("label_min",QString::number(minGray,'f',4).toInt());

    QMessageBox::warning(this,"warning",QString::fromLocal8Bit("Correction完成"));
    int line = getUItemValue("spinBox_current_line",0).toInt();
    if (line > m_imageHeight-1)
    {
        line = m_imageHeight-1;
    }
    int index = line * m_imageWidth;
    for (int i = 0; i < m_imageWidth; i++)
    {
        x << i;
        y << m_correctImage[index + i];
    }
    if (ui->checkBox_correction->isChecked())
    {
        m_pStaticCurveGraph->setValues(x,y, CHART_VALUE_CORRECTION);
    }
    update();
}


void StaticPage::on_saveCorImage()
{
    QString productDir =  "./programs/" + m_strProductName;
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Open Factors File"),
        productDir,
        tr("PNG File(*.png);;BMP File(*.bmp);;JPG File(*.jpg)"));

    if (!fileName.isNull())
    {
        //m_qImg->save(fileName);
        CImageProcess::SaveImageToFile(m_correctImage, m_imageWidth, m_imageHeight, fileName);
    }
    else
    {
        //点的是取消
    }
}


void StaticPage::on_spinBox_current_line_valueChanged()
{
    QVector<double> key;
    QVector<double> value1;
    QVector<double> value2;
    int line = getUItemValue("spinBox_current_line",0).toInt();
    if (line > m_imageHeight-1)
    {
        line = m_imageHeight-1;
    }
    int index = line * m_imageWidth;
    for (int i = 0; i < m_imageWidth; i++)
    {
        key << i;
        if (NULL != m_rawImage)
        {
            value1 << m_rawImage[index+i];
        }
        if (NULL != m_correctImage)
        {
            value2 << m_correctImage[index+i];
        }
    }
    if (ui->checkBox_source->isChecked() && NULL != m_rawImage)
    {
        m_pStaticCurveGraph->setValues(key,value1);
    }
    if (ui->checkBox_source->isChecked() && NULL != m_rawImage)
    {
        m_pStaticCurveGraph->setValues(key,value1);
    }
    if (ui->checkBox_correction->isChecked() && NULL != m_correctImage)
    {
        m_pStaticCurveGraph->setValues(key,value2, CHART_VALUE_CORRECTION);
    }
}

void StaticPage::on_spinBox_grey_valueChanged()
{
    QVector<double> key;
    QVector<double> value;
    int grey = getUItemValue("spinBox_grey", 0).toInt();
    for (int i = 0; i < m_imageWidth; i++)
    {
        key << i;
        value << grey;
    }
    m_pStaticCurveGraph->setValues(key,value,CHART_VALUE_BASE);
}

void StaticPage::on_spinBox_min_col_valueChanged()
{
    double min = ui->spinBox_min_col->value();
    double max = ui->spinBox_max_col->value();
    if (min < max)
    {
        m_pStaticCurveGraph->setXAxisLower(min);
    }
}

void StaticPage::on_spinBox_max_col_valueChanged()
{
    double min = ui->spinBox_min_col->value();
    double max = ui->spinBox_max_col->value();
    if (min < max)
    {
        m_pStaticCurveGraph->setXAxisUpper(max);
    }
}

void StaticPage::on_spinBox_min_grey_valueChanged()
{
    double min = ui->spinBox_min_grey->value();
    double max = ui->spinBox_max_grey->value();
    if (min < max)
    {
        m_pStaticCurveGraph->setYAxisLower(min);
    }
}

void StaticPage::on_spinBox_max_grey_valueChanged()
{
    double min = ui->spinBox_min_grey->value();
    double max = ui->spinBox_max_grey->value();
    if (min < max)
    {
        m_pStaticCurveGraph->setYAxisUpper(max);
    }
}

void StaticPage::on_checkBox_source_clicked( bool checked )
{
    m_pStaticCurveGraph->clearValue();
    on_spinBox_current_line_valueChanged();
}

void StaticPage::on_checkBox_correction_clicked( bool checked )
{
    m_pStaticCurveGraph->clearValue();
    on_spinBox_current_line_valueChanged();
}

void StaticPage::on_loadPicSetFlag()
{
	m_bflag = false;
}

void StaticPage::on_pushButton_get_image_clicked(bool checked)
{
    if (NULL == m_imageInfoBuffer)
    {
        m_imageInfoBuffer = new stImageInfo;
    }
    IBufferManager *pCameraBufferManager = (IBufferManager*)GetSDKInstance("cameramanager", IBufferManager::IID_CAMERA_BUFFER_MANAGER);
    if (NULL == pCameraBufferManager)
    {
        return;
    }

	if (!pCameraBufferManager->isInitFinished())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("执行失败"), QString::fromLocal8Bit("相机未初始化成功， 请检测硬件或配置是否正常"));
		return;
	}

    int width = 0;
    int height = 0;
    pCameraBufferManager->getBufferSize(width, height);
    if (width != m_imageInfoBuffer->width || height != m_imageInfoBuffer->height)
    {
        m_imageInfoBuffer->width = width;
        m_imageInfoBuffer->height = height;
        if (NULL != m_imageInfoBuffer->pixelR)
        {
            delete[]m_imageInfoBuffer->pixelR;
            m_imageInfoBuffer->pixelR = NULL;
        }
    }
    if (NULL == m_imageInfoBuffer->pixelR && width != 0 && height != 0)
    {
        m_imageInfoBuffer->pixelR = new unsigned char[width * height];
    }
    const unsigned char *pBuffer = pCameraBufferManager->getBuffer(getSeqIndexWithName(m_strChannelName.toLocal8Bit().data()));
    if (NULL != pBuffer)
    {
        memcpy(m_imageInfoBuffer->pixelR, pBuffer, sizeof(unsigned char)*width * height);

        m_imageHeight = m_imageInfoBuffer->height;
        m_imageWidth = m_imageInfoBuffer->width;
        m_rawImage = m_imageInfoBuffer->pixelR;

        updateVideo(CHART_VALUE_BASE);
    }
}

void StaticPage::on_pushButton_get_check_image_clicked(bool checked)
{
    on_pushButton_get_image_clicked(checked);
}

bool StaticPage::saveImage(unsigned char *buff, int width, int height, QString strPath)
{
    QImage* img= new QImage(width,height, QImage::Format_Indexed8);
    if (NULL != img)
    {
        QVector<QRgb> colors;
        for (int i=0; i<256; ++i)
        {
            colors.push_back(qRgb(i,i,i));
        }
        img->setColorTable(colors);

        uchar* pPixData = img->bits();
        memcpy(pPixData, buff, width*height);
        QString strSavePath ;
        strSavePath+=strPath;//+".bmp";

        img->save(strSavePath);
        delete img;
        img = NULL;
        return true;
    }
    return false;
}

bool StaticPage::updateVideo(int valueIndex)
{
    if (m_rawImage == NULL)
    {
        return false;
    }
    QVector<double> x;
    QVector<double> y;
    QVector<double> b;
    int standardGray = getUItemValue("spinBox_grey", 0).toInt();
    int line = getUItemValue("spinBox_current_line", 0).toInt();
    if (line > m_imageHeight - 1)
    {
        line = m_imageHeight - 1;
    }
    int index = line * m_imageWidth;
    for (int i = 0; i < m_imageWidth; i++)
    {
        x << i;
        y << m_rawImage[index + i];
        b << standardGray;
    }
    if (ui->checkBox_source->isChecked());
    {
        m_pStaticCurveGraph->setValues(x, y);
    }
    m_pStaticCurveGraph->setValues(x, b, valueIndex);
    update();
    return true;
}

void StaticPage::reInitSDKFlag()
{
	IArithmeticStaticMgr* m_pIArithmeticStaticMgr = (IArithmeticStaticMgr*)GetSDKInstance("g12arithmetic", IArithmeticStaticMgr::IDD_ARITHMETICSTATICMGR);
	if (m_pIArithmeticStaticMgr != NULL)
	{
		IArithmeticStatic*  pIArithmeticStatic = m_pIArithmeticStaticMgr->getArithmeticStatic(m_strChannelName.toStdString());
		if (pIArithmeticStatic != NULL)
		{
			pIArithmeticStatic->uninit();
		}
	}
}

