#include "acquisitionpage.h"
#include "uicommon/extendstackwidget.h"
#include "uicommon/commonutils.h"
#include "filevariable/ifilevariable.h"
#include "g12common.h"
#include "g12utils.h"
#include "stringcommon.h"
#include "callsdk.h"
#include "g12globalenv.h"
#include "vscameratools/cameramanager/icamerafactorymanage.h"
#include "./vscameratools/cameramanager/icamerabuffermanager.h"
#include "illuminationcontroldlg_new.h"
#include "sendmessage.h"
#include "areacampage.h"
#define  ENABLE_DV_MODE_R  1// 屏蔽DV 
AcquisitionPage::AcquisitionPage(QWidget *parent):AbstractPageWidget(parent)
  ,m_pExtendAcquisition(NULL)
{
    m_pExtendAcquisition = new ExtendStackWidget(this);
    m_pExtendAcquisition->setTitleBtnWidth(60);
	int icnt  = getImageStationCnt();
	for (int i = 0; i < icnt; i++)
	{
		ExtendStackWidget* m_pExtendfirst = new ExtendStackWidget();
		StackWidgetVec.push_back(m_pExtendfirst);
		if (getStationType(i) == 0)
		{
			ImageConfigPage* pImageConfigPagefirst = new ImageConfigPage(i, NULL);
			connect(pImageConfigPagefirst, SIGNAL(sig_channelCntUpdate(int ,int )), this, SLOT(slot_channelCntUpdate(int, int )));
			//spinBox_light_cnt
			m_pExtendfirst->addWidget(QString::fromLocal8Bit("图像设置"), pImageConfigPagefirst);
		}
		else if (getStationType(i) == 1)
		{
			CAreaCamPage *pAreaCamPage = new CAreaCamPage(QString::number(i), NULL);
			connect(pAreaCamPage, SIGNAL(sig_channelCntUpdate(int, int)), this, SLOT(slot_channelCntUpdate(int, int)));
			m_pExtendfirst->addWidget(QString::fromLocal8Bit("图像设置"), pAreaCamPage);
		}
		
		InitLightWdgt_new* m_pinitlihgWdtNew = new InitLightWdgt_new(i,NULL);//inilight

		m_pExtendfirst->addWidget(QString::fromLocal8Bit("光源配方"), m_pinitlihgWdtNew);

		string channels = getCurrentProgramRecipesList();
		vector<string> channellist = SString::split(channels, ",");
		vector<IllumChannelWdgt_new* > ChannelWdgtVec;
		//map<int, vector<IllumChannelWdgt_new* > > ChannelWdgtVecMap;
		int channelCnt, maxChannelCnt;
		IFileVariable* pFileVariable = getAcquisitionFileVariable(m_strProductName.toStdString(), SString::IntToStr(i));
		if (pFileVariable != NULL)
		{
			channelCnt = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CHANNEL_CNT).toInt();
		}
		//maxChannelCnt = (maxChannelCnt > channellist.size()) ? channellist.size() : maxChannelCnt;
		char key[100] = { 0 };
		sprintf(key, "ImageStation_%d", i);
		IFileVariable* pFile = getStationCfgVariable();
		if (pFile != NULL)
		{
			maxChannelCnt = pFile->getValue(key, ACQUISITION_MAX_CHANNEL_CNT).toInt();
		}
		if (maxChannelCnt > channellist.size())
		{
			maxChannelCnt = channellist.size();
		}
        maxChannelCnt =Channel_Index_Size;
		for (int index = 0; index < maxChannelCnt; index++)
		{
			QString key = QString::fromStdString(channellist.at(index));
			IllumChannelWdgt_new* pIllumChannelWdgt = new IllumChannelWdgt_new(QString::number(i), key);
			ChannelWdgtVec.push_back(pIllumChannelWdgt);
			m_pExtendfirst->addWidget(key, pIllumChannelWdgt);
			connect(m_pinitlihgWdtNew, SIGNAL(updateSenceName(string)), pIllumChannelWdgt, SLOT(slotUpdateSenceName(string)));
		}
		ChannelWdgtVecMap[i] = ChannelWdgtVec;
		//for (int index = 0; index <maxChannelCnt; index++)
		//{
		//	if (index < channelCnt)
		//	{
		//		ChannelWdgtVec.at(index)->setEnabled(true);
		//	}
		//	else
		//	{
		//		ChannelWdgtVec.at(index)->setEnabled(false);
		//	}
		//}
		//工站名字获取
		QString stationName = QString::fromLocal8Bit(getStationName(i).c_str());
		m_pExtendAcquisition->addWidget(stationName, m_pExtendfirst);
		QObject::connect(m_pExtendfirst, SIGNAL(currentChanged(int)), this, SLOT(on_PagetChanged(int)));
		m_pExtendfirst->setCurrentWidget(0);
		m_stationWidgets.push_back(m_pExtendfirst);
	}
    m_pExtendAcquisition->setCurrentWidget(0);
    QObject::connect(m_pExtendAcquisition,SIGNAL(currentChanged(int)),this,SLOT(on_currentChanged(int)));
    QObject::connect(SENDMSG_INSTANCE, SIGNAL(signalLoginModelChange(int)), this, SLOT(slotsLoginModelChange(int)));


    // 第一次加载读当前登录等级
    QSettings setting(QString("config/loginmode.ini"), QSettings::IniFormat);
    setting.beginGroup("mode");
    int LoginModel = setting.value("login","0").toInt();
    setting.endGroup();
    slotsLoginModelChange(LoginModel);
    #if ENABLE_DV_MODE_R
     //m_pExtendAcquisition->hide();
    #else
    //
    #endif
    //
}

void AcquisitionPage::slot_channelCntUpdate(int stationIndex, int channelCnt)
{
	if (StackWidgetVec.size() == 0)
	{
		return;
	}
	string channels = getCurrentProgramRecipesList();
	vector<string> channellist = SString::split(channels, ",");
	if (channellist.size() < channelCnt)
	{
		return;
	}
	vector<IllumChannelWdgt_new* > wdgVec = ChannelWdgtVecMap[stationIndex];
	char key[100] = { 0 };
	sprintf(key, "ImageStation_%d", stationIndex);
	int maxChannelCnt;
	IFileVariable* pFile = getStationCfgVariable();
	if (pFile != NULL)
	{
		maxChannelCnt = pFile->getValue(key, ACQUISITION_MAX_CHANNEL_CNT).toInt();
	}
	for (int index = 0; index < maxChannelCnt; index++)
	{
		if (index < channelCnt)
		{
			wdgVec[index]->setEnabled(true);
		}
		else
		{
		//	wdgVec[index]->setEnabled(false);
		}
	}
}

void AcquisitionPage::slotsLoginModelChange(int iModel)
{
	if (iModel < 3)
	{
		m_pExtendAcquisition->hide();
	}
#if ENABLE_DV_MODE_R
	else if (3 <= iModel)
	{
		m_pExtendAcquisition->show();
	}
#else
	//
#endif
}
void AcquisitionPage::onEnable(bool enable)
{
	int index = m_pExtendAcquisition->currentIndex();
	ExtendStackWidget * pWidget = qobject_cast<ExtendStackWidget *>(m_pExtendAcquisition->widgetOf(index));
	if (pWidget == NULL)
	{
		return;
	}
	AbstractPageWidget* pPageWidget = qobject_cast<AbstractPageWidget*> (pWidget->currentWidget());;
	if (pPageWidget == NULL)
	{
		return;
	}
	pPageWidget->onEnable(enable);
	pPageWidget->setEnabled(enable);
}

void AcquisitionPage::onSave()
{
	int index = m_pExtendAcquisition->currentIndex();
	ExtendStackWidget * pWidget = qobject_cast<ExtendStackWidget *>(m_pExtendAcquisition->widgetOf(index));
	if (pWidget == NULL)
	{
		return;
	}
	AbstractPageWidget* pPageWidget = qobject_cast<AbstractPageWidget*> (pWidget->currentWidget());
	if (pPageWidget == NULL)
	{
		return;
	}

	
	pPageWidget->updateProductName(m_strProductName);
	pPageWidget->onSave();
}

void AcquisitionPage::onCancel()
{
	int index = m_pExtendAcquisition->currentIndex();
	ExtendStackWidget * pWidget = qobject_cast<ExtendStackWidget *>(m_pExtendAcquisition->widgetOf(index));
	if (pWidget == NULL)
	{
		return;
	}
	AbstractPageWidget* pPageWidget = qobject_cast<AbstractPageWidget*> (pWidget->currentWidget());
	if (pPageWidget == NULL)
	{
		return;
	}
	pPageWidget->onCancel();
}

void AcquisitionPage::onUpdate()
{
	int index = m_pExtendAcquisition->currentIndex();
	ExtendStackWidget * pWidget = qobject_cast<ExtendStackWidget *>(m_pExtendAcquisition->widgetOf(index));
	if (pWidget == NULL)
	{
		return;
	}
	AbstractPageWidget* pPageWidget = qobject_cast<AbstractPageWidget*> (pWidget->currentWidget());
	if (pPageWidget == NULL)
	{
		return;
	}

	pPageWidget->updateProductName(m_strProductName);
	pPageWidget->onUpdate();
}

void AcquisitionPage::on_PagetChanged(int pageindex)
{
	int index = m_pExtendAcquisition->currentIndex();
	ExtendStackWidget * pWidget = qobject_cast<ExtendStackWidget *>(m_pExtendAcquisition->currentWidget());
	if (pWidget == NULL)
	{
		return;
	}
	AbstractPageWidget* pPageWidget = qobject_cast<AbstractPageWidget*> (pWidget->widgetOf(pageindex));
	if (pPageWidget == NULL)
	{
		return;
	}

	pPageWidget->updateProductName(m_strProductName);
	pPageWidget->onUpdate();
}


void AcquisitionPage::on_currentChanged(int index)
{
	ExtendStackWidget * pWidget =  qobject_cast<ExtendStackWidget *>(m_pExtendAcquisition->widgetOf(index));
    if (pWidget != NULL)
    {
		AbstractPageWidget* pPageWidget = qobject_cast<AbstractPageWidget*>(pWidget->currentWidget());
		if (pPageWidget != NULL)
		{
			pPageWidget->updateProductName(m_strProductName);
			pPageWidget->onUpdate();
		}
    }
}

AcquisitionConfigPage::AcquisitionConfigPage(QString strKey, QWidget *parent):
    AbstractPageWidget(parent),
    m_strChannelName(strKey),
    ui(new Ui::acquisitionForm)
{
    ui->setupUi(this);
    QObject::connect(ui->radioButton_auto,SIGNAL(clicked()),this,SLOT(on_autoCutImage()));
    QObject::connect(ui->radioButton_manual,SIGNAL(clicked()),this,SLOT(on_manualCutImage()));
}

AcquisitionConfigPage::~AcquisitionConfigPage()
{
    delete ui;
}

void AcquisitionConfigPage::onEnable( bool enable)
{

}

void AcquisitionConfigPage::onSave()
{
    IFileVariable* pFileVariable = getAcquisitionFileVariable(m_strProductName.toStdString());
    if (pFileVariable != NULL)
    {
        int itype = 0;
        if(getUItemValue("radioButton_auto",0).toInt())  //1 auto, 0 manual
        {
             itype = 1;
        }
        else {
            itype = 0;
        }

        double  dminthre = getUItemValue("doubleSpinBox_minthre",0).toDouble();
        double  dmaxthre = getUItemValue("doubleSpinBox_maxthre",0).toDouble();
        int itop= getUItemValue("cutimg_top",0).toInt();;
        int ibottom= getUItemValue("cutimg_bottom",0).toInt();;
        int ileft= getUItemValue("cutimg_left",0).toInt();;
        int iright= getUItemValue("cutimg_right",0).toInt();



        pFileVariable->setValue(m_strChannelName.toStdString(),ACQUISITION_CUTIMG_TYPE,itype,true);
        pFileVariable->setValue(m_strChannelName.toStdString(),ACQUISITION_CUTIMG_MINTHRE,dminthre,true);
        pFileVariable->setValue(m_strChannelName.toStdString(),ACQUISITION_CUTIMG_MAXTHRE,dmaxthre,true);
        pFileVariable->setValue(m_strChannelName.toStdString(),ACQUISITION_CUTIMG_TOP,itop,true);
        pFileVariable->setValue(m_strChannelName.toStdString(),ACQUISITION_CUTIMG_BOTTOM,ibottom,true);
        pFileVariable->setValue(m_strChannelName.toStdString(),ACQUISITION_CUTIMG_LEFT,ileft,true);
        pFileVariable->setValue(m_strChannelName.toStdString(),ACQUISITION_CUTIMG_RIGHT,iright,true);
    }
}

void AcquisitionConfigPage::onCancel()
{
    onUpdate();
}

void AcquisitionConfigPage::onUpdate()
{
    IFileVariable* pFileVariable = getAcquisitionFileVariable(m_strProductName.toStdString());
    int itype = 0;
    double  dminthre = 0.0;
    double  dmaxthre = 0.0;
    int itop  = 0;
    int ibottom = 0;
    int ileft = 0;
    int iright = 0;


    if (pFileVariable != NULL)
    {
         itype = pFileVariable->getValue(m_strChannelName.toStdString(), ACQUISITION_CUTIMG_TYPE).toInt();
         dminthre = pFileVariable->getValue(m_strChannelName.toStdString(), ACQUISITION_CUTIMG_MINTHRE).toDouble();
         dmaxthre = pFileVariable->getValue(m_strChannelName.toStdString(), ACQUISITION_CUTIMG_MAXTHRE).toDouble();
         itop= pFileVariable->getValue(m_strChannelName.toStdString(), ACQUISITION_CUTIMG_TOP).toInt();
         ibottom= pFileVariable->getValue(m_strChannelName.toStdString(), ACQUISITION_CUTIMG_BOTTOM).toInt();
         ileft= pFileVariable->getValue(m_strChannelName.toStdString(), ACQUISITION_CUTIMG_LEFT).toInt();
         iright= pFileVariable->getValue(m_strChannelName.toStdString(), ACQUISITION_CUTIMG_RIGHT).toInt();


    }

    if(itype == 1)//1 auto, 0 manual
    {
        setUIItemValue("radioButton_auto",1);
        setUIItemValue("radioButton_manual",0);
        on_autoCutImage();
    }
    else
    {
        setUIItemValue("radioButton_auto",0);
        setUIItemValue("radioButton_manual",1);
        on_manualCutImage();
    }

    setUIItemValue("doubleSpinBox_minthre",dminthre);
    setUIItemValue("doubleSpinBox_maxthre",dmaxthre);
    setUIItemValue("cutimg_top",itop);
    setUIItemValue("cutimg_bottom",ibottom);
    setUIItemValue("cutimg_left",ileft);
    setUIItemValue("cutimg_right",iright);

}

void AcquisitionConfigPage::on_autoCutImage()
{
    enableUIItem("doubleSpinBox_minthre",true);
    enableUIItem("doubleSpinBox_maxthre",true);
    enableUIItem("cutimg_top",false);
    enableUIItem("cutimg_bottom",false);
    enableUIItem("cutimg_left",false);
    enableUIItem("cutimg_right",false);
}

void AcquisitionConfigPage::on_manualCutImage()
{
    enableUIItem("doubleSpinBox_minthre",false);
    enableUIItem("doubleSpinBox_maxthre",false);
    enableUIItem("cutimg_top",true);
    enableUIItem("cutimg_bottom",true);
    enableUIItem("cutimg_left",true);
    enableUIItem("cutimg_right",true);
}


ImageConfigPage::ImageConfigPage(int index, QWidget *parent):
    AbstractPageWidget(parent),
	m_index(index),
    ui(new Ui::ImageConfigForm)
{
    ui->setupUi(this);
	connect(ui->checkBox_BlockEnable, SIGNAL(clicked()), this, SLOT(on_enableBlock()));
	connect(ui->spinBox_BlockOverlapLines, SIGNAL(valueChanged(int)), this, SLOT(on_BlockvalueChanged(int)));
	connect(ui->spinBox_BlockCnt, SIGNAL(valueChanged(int)), this, SLOT(on_BlockvalueChanged(int)));
	connect(ui->spinBox_BlockLines, SIGNAL(valueChanged(int)), this, SLOT(on_BlockvalueChanged(int)));;
	connect(ui->spinBox_ImageHeight, SIGNAL(valueChanged(int)), this, SLOT(on_BlockvalueChanged(int)));

	connect(ui->spinBox_resolutionX, SIGNAL(valueChanged(int)), this, SLOT(on_CamvalueChanged(int)));
	connect(ui->spinBox_resolutionY, SIGNAL(valueChanged(int)), this, SLOT(on_CamvalueChanged(int)));
	connect(ui->spinBox_camCnt, SIGNAL(valueChanged(int)), this, SLOT(on_CamvalueChanged(int)));
	connect(ui->spinBox_overlapX, SIGNAL(valueChanged(int)), this, SLOT(on_CamvalueChanged(int)));
	connect(ui->spinBox_overlapY, SIGNAL(valueChanged(int)), this, SLOT(on_CamvalueChanged(int)));
	//connect(ui->spinBox_light_cnt, SIGNAL(valueChanged(int)), this, SLOT(on_lightCnt_Changed(int)));


	QObject::connect(ui->checkBox_upcam1, SIGNAL(clicked()), this, SLOT(on_enableUpCam()));
	QObject::connect(ui->checkBox_upcam2, SIGNAL(clicked()), this, SLOT(on_enableUpCam()));
	QObject::connect(ui->checkBox_upcam3, SIGNAL(clicked()), this, SLOT(on_enableUpCam()));
	QObject::connect(ui->checkBox_upcam4, SIGNAL(clicked()), this, SLOT(on_enableUpCam()));
	connect(ui->comboBox_image, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ImageConfigChanged(int)));
	
	ILineCameraFactoryManage* pCameraManager = (ILineCameraFactoryManage*)GetSDKInstance("cameramanager", ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE);
	if (pCameraManager != NULL)
	{
		vector<string> camList;
		pCameraManager->GetCameraNameList(camList);
		for (int i = 0; i < camList.size(); i++)
		{
			ui->comboBox_upcam1->addItem(QString::fromStdString(camList.at(i)));
			ui->comboBox_upcam2->addItem(QString::fromStdString(camList.at(i)));
			ui->comboBox_upcam3->addItem(QString::fromStdString(camList.at(i)));
			ui->comboBox_upcam4->addItem(QString::fromStdString(camList.at(i)));
		}
	}
	ui->comboBox_image->setCurrentIndex(0);
	on_ImageConfigChanged(0);
}


ImageConfigPage::~ImageConfigPage()
{
    delete ui;
}

void ImageConfigPage::onEnable(bool enable)
{

}

void ImageConfigPage::on_spinBox_channel_cnt_valueChanged(int value)
{
	
}

void ImageConfigPage::on_comboBox_imge_channel_currentIndexChanged(int index)
{
	IFileVariable* pFileVariable = getAcquisitionFileVariable(m_strProductName.toStdString(), SString::IntToStr(m_index));
	bool bm_grab_Enable = false;
	bool bm_jz_Enable = false;
	int igrab_val = 0;
	int ijz_val = 0;
	if (pFileVariable != NULL)
	{
		int imge_channelindex = ui->comboBox_imge_channel->currentIndex();
		QString KeyNameCfg = QString("Config_%1").arg(imge_channelindex);
		bm_grab_Enable = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_GRAB_IMAGE_ENABLE).toBool();
		bm_jz_Enable = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_AI_JZ_ENABLE).toBool();
		igrab_val = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_GRAB_IMAGE_VAL).toInt();
		ijz_val = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_AI_JZ_VAL).toInt();
	}

	setUIItemValue("checkBox_grab_Enable", bm_grab_Enable);
	setUIItemValue("checkBox_jz_Enable", bm_jz_Enable);

	setUIItemValue("spinBox_grab_val", igrab_val);
	setUIItemValue("spinBox_jz_val", ijz_val);
}

void ImageConfigPage::onSave()
{
    IFileVariable* pFileVariable = getAcquisitionFileVariable(m_strProductName.toStdString(),SString::IntToStr(m_index));
    if (pFileVariable != NULL)
    {
		bool bEnableGrap = getUItemValue("checkBox_enable_grap", true).toBool();
		bool bSaveRawImage = getUItemValue("checkBox_enable_save_raw", true).toBool();

		bool bAllEnalbe = getUItemValue("checkBox_image_enable", true).toBool();
		QString strDes = getUItemValue("lineEdit_image_des","").toString();
		int index = ui->comboBox_image->currentIndex();
		
		int iresolutionX = getUItemValue("spinBox_resolutionX",8196).toInt();
		int iresolutionY = getUItemValue("spinBox_resolutionY", 0).toInt();
		int iCamCnt = getUItemValue("spinBox_camCnt", 1).toInt();
		int ioverlapX = getUItemValue("spinBox_overlapX", 0).toInt();
		int ioverlapY = getUItemValue("spinBox_overlapY", 0).toInt();
		int iImageWidth = getUItemValue("spinBox_ImageWidth", 0).toInt();
		int iImageHeight = getUItemValue("spinBox_ImageHeight", 0).toInt();;

		bool bEnableBlock = getUItemValue("checkBox_BlockEnable", false).toBool();
		int iBlockLines = getUItemValue("spinBox_BlockLines", 0).toInt();
		int iBlockOverlapLines = getUItemValue("spinBox_BlockOverlapLines", 0).toInt();
		int iBlockCnt = getUItemValue("spinBox_BlockCnt", 0).toInt();
		if (iBlockLines > 0)
		{
			iBlockCnt = iImageHeight / iBlockLines;
		}
		
		bool bEnablerestore =  getUItemValue("checkBox_enable_restore", false).toBool();

		bool bCam1Enable = getUItemValue("checkBox_upcam1", 0).toBool();
		bool bCam2Enable = getUItemValue("checkBox_upcam2", 0).toBool();
		bool bCam3Enable = getUItemValue("checkBox_upcam3", 0).toBool();
		bool bCam4Enable = getUItemValue("checkBox_upcam4", 0).toBool();

		//----
		
		bool bm_imge_Enable = getUItemValue("checkBox_m_imge_Enable", 0).toBool();
		int  iimgemove = getUItemValue("spinBox_imge_move", 0).toInt();

		int  imge_channel_index = ui->comboBox_imge_channel->currentIndex();

		int igrab_val = getUItemValue("spinBox_grab_val", 0).toInt();
		bool bm_grab_Enable = getUItemValue("checkBox_grab_Enable", 0).toBool();

		int ijz_val = getUItemValue("spinBox_jz_val", 0).toInt();
		bool bm_jz_Enable = getUItemValue("checkBox_jz_Enable", 0).toBool();
		
		//---

		string strCam1Sn = getUItemValue("comboBox_upcam1", "").toString().toStdString();
		string strCam2Sn = getUItemValue("comboBox_upcam2", "").toString().toStdString();
		string strCam3Sn = getUItemValue("comboBox_upcam3", "").toString().toStdString();
		string strCam4Sn = getUItemValue("comboBox_upcam4", "").toString().toStdString();

		bool bShowImageA = getUItemValue("checkBox_ShowImageA", 0).toBool();
		bool bShowImageB = getUItemValue("checkBox_ShowImageB", 0).toBool();
		bool bShowImageC = getUItemValue("checkBox_ShowImageC", 0).toBool();
		bool bShowImageD = getUItemValue("checkBox_ShowImageD", 0).toBool();
		bool bShowImageE = getUItemValue("checkBox_ShowImageE", 0).toBool();
		bool bShowImageF = getUItemValue("checkBox_ShowImageF", 0).toBool();
		bool bShowImageG = getUItemValue("checkBox_ShowImageG", 0).toBool();
		bool bShowImageH = getUItemValue("checkBox_ShowImageH", 0).toBool();
		int itrigSource = ui->comboBox_trig_source->currentIndex();
		int itrigIndex = ui->comboBox_trig_index->currentIndex();
		int itrigNetid = getUItemValue("spinBox_trig_netid", 0).toInt();

		int channelCnt = getUItemValue("spinBox_channel_cnt", 1).toInt();
		string snap_sig_name = getUItemValue("lineEdit_triggle_IO", "").toString().toStdString();
		int acq_time_out = getUItemValue("spinBox_AcqTimeOut", 6500).toInt();

		emit sig_channelCntUpdate(m_index, channelCnt);
		QString KeyNameCfg = QString("Config_%1").arg(imge_channel_index);
		pFileVariable->setValue(KeyNameCfg.toStdString(), ACQUISITION_GRAB_IMAGE_ENABLE, bm_grab_Enable, true);
		pFileVariable->setValue(KeyNameCfg.toStdString(), ACQUISITION_AI_JZ_ENABLE, bm_jz_Enable, true);
		pFileVariable->setValue(KeyNameCfg.toStdString(), ACQUISITION_GRAB_IMAGE_VAL, igrab_val, true);
		pFileVariable->setValue(KeyNameCfg.toStdString(), ACQUISITION_AI_JZ_VAL, ijz_val, true);

        QString KeyName = QString("ImageConfig_%1").arg(index);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_IMAGE_ENABLE, bAllEnalbe, true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_IMAGE_DES, strDes.toLocal8Bit().toStdString().c_str(), true);

        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_CAM_RESOLUTIONX, iresolutionX,true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_CAM_RESOLUTIONY, iresolutionY,true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_CAM_CNT, iCamCnt,true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_OVERLAPX, ioverlapX,true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_OVERLAPY, ioverlapY,true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_IMAGEWIDTH, iImageWidth,true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_IMAGEHEIGHT, iImageHeight,true);

        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_UPCAM1_ENABLE, bCam1Enable, true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_UPCAM1_SN, strCam1Sn, true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_UPCAM2_ENABLE, bCam2Enable, true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_UPCAM2_SN, strCam2Sn, true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_UPCAM3_ENABLE, bCam3Enable, true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_UPCAM3_SN, strCam3Sn, true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_UPCAM4_ENABLE, bCam4Enable, true);
        pFileVariable->setValue(KeyName.toStdString(), ACQUISITION_UPCAM4_SN, strCam4Sn, true);

		pFileVariable->setValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKENABLE, bEnableBlock, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKLINES, iBlockLines, true);;
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKOVERLAPLINES, iBlockOverlapLines, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG,ACQUISITION_ENABLE_RESTORE_IMAGE, bEnablerestore, true);

		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SHOWIMAGEA_ENABLE, bShowImageA, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SHOWIMAGEB_ENABLE, bShowImageB, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SHOWIMAGEC_ENABLE, bShowImageC, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SHOWIMAGED_ENABLE, bShowImageD, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SHOWIMAGEE_ENABLE, bShowImageE, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SHOWIMAGEF_ENABLE, bShowImageF, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SHOWIMAGEG_ENABLE, bShowImageG, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SHOWIMAGEH_ENABLE, bShowImageH, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_GRAP_ENABLE, bEnableGrap, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SAVE_RAW_IMAGE_ENABLE, bSaveRawImage, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_SOURCE, itrigSource, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_INDEX, itrigIndex, true);

		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_NETID, itrigNetid, true);

		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CHANNEL_CNT, channelCnt, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SNAP_SIG_NAME, snap_sig_name, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_ACQ_TIME_OUT, acq_time_out, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_AI_IMAGE_ENABLE, bm_imge_Enable, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_AI_IMAGE_MOVE, iimgemove, true);

    }

	IBufferManagerFactory* pIBufferManagerFactory = (IBufferManagerFactory*)GetSDKInstance("cameramanager", IBufferManagerFactory::IID_CAMERA_BUFFER_MANAGER_FACTORY);
	if (pIBufferManagerFactory != NULL)
	{
		IBufferManager *pCameraBufferManager = pIBufferManagerFactory->createBufferManager(SString::IntToStr(m_index));
		if (NULL != pCameraBufferManager)
		{
			pCameraBufferManager->init();
		}
	}
}

void ImageConfigPage::onCancel()
{
    onUpdate();
}

void ImageConfigPage::onUpdate()
{
    IFileVariable* pFileVariable = getAcquisitionFileVariable(m_strProductName.toStdString(), SString::IntToStr(m_index));
	bool bEnableGrap = true;
	bool bSaveRawImage = true;
	bool bEnableBlock = false;
	string strDes = "";
    int iresolutionX = 0;
	int iresolutionY = 0;
	int iCamCnt = 1;
	int ioverlapX = 0;
	int ioverlapY = 0;
	int iImageWidth = 0;
	int iImageHeight = 0;

	int iBlockLines = 0;
	int iBlockOverlapLines = 0;
	int iBlockCnt = 0;
	bool bEnablerestore = false;

	bool bAllEnable = false;
	bool bCam1Enable = false;
	bool bCam2Enable = false;
	bool bCam3Enable = false;
	bool bCam4Enable = false;

	string strCam1Sn = "";
	string strCam2Sn = "";
	string strCam3Sn = "";
	string strCam4Sn = "";

	bool bShowImageA = true;
	bool bShowImageB = true;
	bool bShowImageC = true;
	bool bShowImageD = true;
	bool bShowImageE = true;
	bool bShowImageF = true;
	bool bShowImageG = true;
	bool bShowImageH = true;
	int itrigSource = 0;
	int itrigIndex = 0;
	int itrigNetid = 0;
	int channelCnt = 1;
	string snap_sig_name = "";
	int acq_time_out = 6500;
	int max_channel_cnt = 1;

	bool bm_grab_Enable = false;
	bool bm_jz_Enable   = false;
	bool bm_imge_Enable = false;
	int iimagemove = 0;
	int igrab_val = 0;
	int ijz_val = 0;

	char key[100] = { 0 };
	sprintf(key, "ImageStation_%d", m_index);
	IFileVariable* file = getStationCfgVariable();
	if (file != NULL)
	{
		max_channel_cnt = file->getValue(key, ACQUISITION_MAX_CHANNEL_CNT).toInt();
	}
    if (pFileVariable != NULL)
    {
        int index = ui->comboBox_image->currentIndex();
		int imge_channelindex = ui->comboBox_imge_channel->currentIndex();
		//
		QString KeyNameCfg = QString("Config_%1").arg(imge_channelindex);
		
		bm_grab_Enable = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_GRAB_IMAGE_ENABLE).toBool();
		bm_jz_Enable   = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_AI_JZ_ENABLE).toBool();
		igrab_val      = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_GRAB_IMAGE_VAL).toInt();
		ijz_val        = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_AI_JZ_VAL).toInt();
		
		//
        QString KeyName = QString("ImageConfig_%1").arg(index);

        bAllEnable = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_IMAGE_ENABLE).toBool();
        strDes = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_IMAGE_DES).toCString();
        iresolutionX = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_CAM_RESOLUTIONX).toInt();
        iresolutionY = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_CAM_RESOLUTIONY).toInt();
        iCamCnt = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_CAM_CNT).toInt();
        ioverlapX = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_OVERLAPX).toInt();
        ioverlapY = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_OVERLAPY).toInt();
        iImageWidth = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_IMAGEWIDTH).toInt();
        iImageHeight = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_IMAGEHEIGHT).toInt();

        bCam1Enable = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_UPCAM1_ENABLE).toBool();
        bCam2Enable = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_UPCAM2_ENABLE).toBool();
        bCam3Enable = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_UPCAM3_ENABLE).toBool();
        bCam4Enable = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_UPCAM4_ENABLE).toBool();
        strCam1Sn = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_UPCAM1_SN).toCString("");
        strCam2Sn = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_UPCAM2_SN).toCString("");
        strCam3Sn = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_UPCAM3_SN).toCString("");
        strCam4Sn = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_UPCAM4_SN).toCString("");

		bEnableBlock = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKENABLE).toBool();
		iBlockLines = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKLINES).toInt();;
		iBlockOverlapLines = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKOVERLAPLINES).toInt();
		iBlockCnt = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKCNT).toInt();
		bEnablerestore = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_ENABLE_RESTORE_IMAGE).toBool();
			
        bShowImageA = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_SHOWIMAGEA_ENABLE).toBool(true);
        bShowImageB = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_SHOWIMAGEB_ENABLE).toBool(true);
        bShowImageC = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_SHOWIMAGEC_ENABLE).toBool(true);
        bShowImageD = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_SHOWIMAGED_ENABLE).toBool(true);
        bShowImageE = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_SHOWIMAGEE_ENABLE).toBool(true);
        bShowImageF = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_SHOWIMAGEF_ENABLE).toBool(true);
        bShowImageG = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_SHOWIMAGEG_ENABLE).toBool(true);
        bShowImageH = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_SHOWIMAGEH_ENABLE).toBool(true);

		bEnableGrap = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_GRAP_ENABLE).toBool(true);
		bSaveRawImage = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_SAVE_RAW_IMAGE_ENABLE).toBool(true);
		itrigSource = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_SOURCE).toInt();
		itrigIndex = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_INDEX).toInt();

		itrigNetid = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_NETID).toInt();

		channelCnt = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CHANNEL_CNT).toInt();
		snap_sig_name = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SNAP_SIG_NAME).toCString();
		acq_time_out = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_ACQ_TIME_OUT).toInt();

		bm_imge_Enable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_AI_IMAGE_ENABLE).toBool();
		iimagemove = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_AI_IMAGE_MOVE).toInt();

    }
	setUIItemValue("checkBox_image_enable", bAllEnable);
	setUIItemValue("lineEdit_image_des", QString::fromLocal8Bit(strDes.c_str()));
    setUIItemValue("spinBox_resolutionX", iresolutionX);
    setUIItemValue("spinBox_resolutionY", iresolutionY);
	setUIItemValue("spinBox_camCnt", iCamCnt);
	setUIItemValue("spinBox_overlapX", ioverlapX);
	setUIItemValue("spinBox_overlapY", ioverlapY);
	setUIItemValue("spinBox_ImageWidth", iImageWidth);
	setUIItemValue("spinBox_ImageHeight", iImageHeight);

	setUIItemValue("checkBox_BlockEnable", bEnableBlock);
	setUIItemValue("spinBox_BlockLines", iBlockLines);
	setUIItemValue("spinBox_BlockOverlapLines", iBlockOverlapLines);
	setUIItemValue("spinBox_BlockCnt", iBlockCnt);
	setUIItemValue("checkBox_enable_restore", bEnablerestore);

	setUIItemValue("checkBox_upcam1", bCam1Enable);
	setUIItemValue("checkBox_upcam2", bCam2Enable);
	setUIItemValue("checkBox_upcam3", bCam3Enable);
	setUIItemValue("checkBox_upcam4", bCam4Enable);

	setUIItemValue("comboBox_upcam1", QString::fromStdString(strCam1Sn));
	setUIItemValue("comboBox_upcam2", QString::fromStdString(strCam2Sn));
	setUIItemValue("comboBox_upcam3", QString::fromStdString(strCam3Sn));
	setUIItemValue("comboBox_upcam4", QString::fromStdString(strCam4Sn));

	setUIItemValue("checkBox_ShowImageA", bShowImageA);
	setUIItemValue("checkBox_ShowImageB", bShowImageB);
	setUIItemValue("checkBox_ShowImageC", bShowImageC);
	setUIItemValue("checkBox_ShowImageD", bShowImageD);
	setUIItemValue("checkBox_ShowImageE", bShowImageE);
	setUIItemValue("checkBox_ShowImageF", bShowImageF);
	setUIItemValue("checkBox_ShowImageG", bShowImageG);
	setUIItemValue("checkBox_ShowImageH", bShowImageH);
	setUIItemValue("checkBox_enable_grap", bEnableGrap);
	setUIItemValue("checkBox_enable_save_raw", bSaveRawImage);
	//setUIItemValue("comboBox_trig_source", itrigSource);
	ui->comboBox_trig_source->setCurrentIndex(itrigSource);

	setUIItemValue("comboBox_trig_source", itrigSource);
	setUIItemValue("comboBox_trig_index", itrigIndex);
	setUIItemValue("spinBox_trig_netid", itrigNetid);

	setUIItemValue("lineEdit_triggle_IO", QString::fromStdString(snap_sig_name));
	setUIItemValue("spinBox_channel_cnt", channelCnt);
	setUIItemValue("spinBox_AcqTimeOut", acq_time_out);

	//
	setUIItemValue("checkBox_m_imge_Enable", bm_imge_Enable);
	setUIItemValue("checkBox_grab_Enable", bm_grab_Enable);
	setUIItemValue("checkBox_jz_Enable", bm_jz_Enable);

	setUIItemValue("spinBox_imge_move", iimagemove);
	setUIItemValue("spinBox_grab_val", igrab_val);
	setUIItemValue("spinBox_jz_val", ijz_val);

	//
	ui->spinBox_channel_cnt->setMaximum(max_channel_cnt);

	on_enableBlock();
	on_BlockvalueChanged(0);
	on_CamvalueChanged(0);
}

void ImageConfigPage::on_enableBlock()
{
	ui->spinBox_BlockLines->setEnabled(ui->checkBox_BlockEnable->checkState() == Qt::Checked);
	ui->spinBox_BlockOverlapLines->setEnabled(ui->checkBox_BlockEnable->checkState() == Qt::Checked);
	ui->spinBox_BlockCnt->setEnabled(ui->checkBox_BlockEnable->checkState() == Qt::Checked);
}

void ImageConfigPage::on_BlockvalueChanged(int iValue)
{
	bool bEnableBlock = getUItemValue("checkBox_BlockEnable", false).toBool();
	int iBlockLines = getUItemValue("spinBox_BlockLines", 0).toInt();
	int iBlockOverlapLines = getUItemValue("spinBox_BlockOverlapLines", 0).toInt();
	int iBlockCnt = getUItemValue("spinBox_BlockCnt", 0).toInt();
	int iImageHeight = getUItemValue("spinBox_resolutionY", 0).toInt();

	if (!bEnableBlock)
	{
		return;
	}
	if (iBlockOverlapLines >= iBlockLines )
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("错误重叠行数大于块行数。"));
		return;
	}
	if (iBlockLines - iBlockOverlapLines > 0)
	{
		iBlockCnt = (iImageHeight - iBlockOverlapLines) / (iBlockLines - iBlockOverlapLines);
	}
	
	iBlockCnt += (iImageHeight - iBlockOverlapLines) % (iBlockLines - iBlockOverlapLines) > 0 ? 1 : 0;   // 块数

	setUIItemValue("spinBox_BlockCnt", iBlockCnt);
}

void ImageConfigPage::on_CamvalueChanged(int iValue)
{
	int iresolutionX = getUItemValue("spinBox_resolutionX", 8196).toInt();
	int iresolutionY = getUItemValue("spinBox_resolutionY", 0).toInt();
	int iCamCnt = getUItemValue("spinBox_camCnt", 1).toInt();
	int ioverlapX = getUItemValue("spinBox_overlapX", 0).toInt();
	int ioverlapY = getUItemValue("spinBox_overlapY", 0).toInt();
	int iImageWidth = getUItemValue("spinBox_ImageWidth", 0).toInt();
	int iImageHeight = getUItemValue("spinBox_ImageHeight", 0).toInt();

	iImageHeight = iresolutionY;
	iImageWidth = iresolutionX*iCamCnt - ioverlapX*(iCamCnt - 1);
	setUIItemValue("spinBox_ImageWidth", iImageWidth);
	setUIItemValue("spinBox_ImageHeight", iImageHeight);

	enableUIItem("checkBox_upcam1", iCamCnt > 0);
	enableUIItem("checkBox_upcam2", iCamCnt > 1);
	enableUIItem("checkBox_upcam3", iCamCnt > 2);
	enableUIItem("checkBox_upcam4", iCamCnt > 3);

	on_enableUpCam();
}


void ImageConfigPage::on_enableUpCam()
{
	ui->comboBox_upcam1->setEnabled(ui->checkBox_upcam1->checkState() == Qt::Checked && ui->checkBox_upcam1->isEnabled());
	ui->comboBox_upcam2->setEnabled(ui->checkBox_upcam2->checkState() == Qt::Checked && ui->checkBox_upcam2->isEnabled());
	ui->comboBox_upcam3->setEnabled(ui->checkBox_upcam3->checkState() == Qt::Checked && ui->checkBox_upcam3->isEnabled());
	ui->comboBox_upcam4->setEnabled(ui->checkBox_upcam4->checkState() == Qt::Checked && ui->checkBox_upcam4->isEnabled());
}

void ImageConfigPage::on_ImageConfigChanged(int index)
{
	onUpdate();
}


