#include "areacampage.h"
#include "ui_areacampage.h"
#include "g12common.h"
#include "g12globalenv.h"
#include "filevariable/ifilevariable.h"
#include "vscameratools/cameramanager/icamerafactorymanage.h"
#include "vscameratools/camera/icamerafactory.h"
#include "vscameratools/camera/icamera.h"
#include "vscameratools/cameramanager/icamerabuffermanager.h"
#include "callsdk.h"
#include "lightcontrol_new.h"
#include "maindefine.h"
#include <QMessageBox>
#include "stringcommon.h"

CAreaCamPage::CAreaCamPage(QString strKey,QWidget *parent) :
    m_strKeyName(strKey),
    AbstractPageWidget(parent),
	m_pTriggerThread(NULL),
    m_plightcontrol_new(NULL),
    m_pmotioncontrol(NULL),
    ui(new Ui::CAreaCamPage)
{
    ui->setupUi(this);

	connect(ui->spinBox_resolutionX, SIGNAL(valueChanged(int)), this, SLOT(on_CamvalueChanged(int)));
	connect(ui->spinBox_resolutionY, SIGNAL(valueChanged(int)), this, SLOT(on_CamvalueChanged(int)));
	connect(ui->spinBox_camCnt, SIGNAL(valueChanged(int)), this, SLOT(on_CamvalueChanged(int)));
	connect(ui->spinBox_overlapX, SIGNAL(valueChanged(int)), this, SLOT(on_CamvalueChanged(int)));
	connect(ui->spinBox_overlapY, SIGNAL(valueChanged(int)), this, SLOT(on_CamvalueChanged(int)));
    m_plightcontrol_new =  lightcontrol_new_manager::GetInstance()->getLightcontrol(strKey.toInt());

    IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), m_strKeyName.toStdString());
    if (pFileVariable != NULL)
    {
        int iMotionCtl = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_INDEX, 0).toInt();
        m_pmotioncontrol = motioncontrol_new_manager::GetInstance()->getMotioncontrol(iMotionCtl);
		m_itrigNetid = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_NETID, 1).toInt();
    }
	m_pTriggerThread = new JThreadTP<CAreaCamPage, void*, void>;
	connect(this, SIGNAL(sigSendImage(void*)), this, SLOT(on_image_display(void*)),Qt::BlockingQueuedConnection);
	onUpdate();
}

CAreaCamPage::~CAreaCamPage()
{
	m_bRunRealtime = false;
	if (m_pTriggerThread != NULL)
	{
		while (m_pTriggerThread->IsRunning())
		{
			Sleep(50);
		}
		delete m_pTriggerThread;
		m_pTriggerThread = NULL;
	}

    delete ui;
}

void CAreaCamPage::onEnable(bool enable)
{

}

void CAreaCamPage::onSave()
{
    IFileVariable* pFileVariable = getAcquisitionFileVariable(m_strProductName.toStdString(),m_strKeyName.toStdString());

	if (pFileVariable != NULL)
	{
		bool bEnableGrap = getUItemValue("checkBox_enable_grap", true).toBool();
		bool bSaveRawImage = getUItemValue("checkBox_enable_save_raw", true).toBool();

		int index = ui->comboBox_cam_name->currentIndex();
		QString CamName = QString("ImageConfig_%1").arg(index);

		int iresolutionX = getUItemValue("spinBox_resolutionX", 8196).toInt();
		int iresolutionY = getUItemValue("spinBox_resolutionY", 0).toInt();
		int iCamCnt = getUItemValue("spinBox_camCnt", 1).toInt();
		int ioverlapX = getUItemValue("spinBox_overlapX", 0).toInt();
		int ioverlapY = getUItemValue("spinBox_overlapY", 0).toInt();
		int iImageWidth = getUItemValue("spinBox_ImageWidth", 0).toInt();
		int iImageHeight = getUItemValue("spinBox_ImageHeight", 0).toInt();;

		QString camSN = getUItemValue("lineEdit_cam_sn","").toString();
		int iExposeTime = getUItemValue("spinBox_cam_expose", "").toInt();
		int iGain = getUItemValue("spinBox_cam_gain", "").toInt();
		double dalay_trig = getUItemValue("doubleSpinBox_delay_trig", "").toDouble();

		int channelCnt = getUItemValue("spinBox_channel_cnt", 1).toInt();
		string snap_sig_name = getUItemValue("lineEdit_triggle_IO", "").toString().toStdString();
		int acq_time_out = getUItemValue("spinBox_AcqTimeOut", 6500).toInt();
		int itrigIndex = getUItemValue("comboBox_trig_index", 0).toInt();

		emit sig_channelCntUpdate(m_strKeyName.toInt(), channelCnt);

		int itrig_mode = ui->comboBox_trig_mode->currentIndex(); // getUItemValue("comboBox_trig_mode", "").toInt();
		int itrig_edge = ui->comboBox_trig_edge->currentIndex(); //getUItemValue("comboBox_trig_edge", "").toInt();

		pFileVariable->setValue(CamName.toStdString(), ACQUISITION_GRAP_ENABLE, bEnableGrap, true);
		pFileVariable->setValue(CamName.toStdString(), ACQUISITION_SAVE_RAW_IMAGE_ENABLE, bSaveRawImage, true);
		pFileVariable->setValue(CamName.toStdString(), ACQUISITION_CAM_RESOLUTIONX, iresolutionX, true);
		pFileVariable->setValue(CamName.toStdString(), ACQUISITION_CAM_RESOLUTIONY, iresolutionY, true);
		pFileVariable->setValue(CamName.toStdString(), ACQUISITION_CAM_CNT, iCamCnt, true);
		pFileVariable->setValue(CamName.toStdString(), ACQUISITION_OVERLAPX, ioverlapX, true);
		pFileVariable->setValue(CamName.toStdString(), ACQUISITION_OVERLAPY, ioverlapY, true);
		pFileVariable->setValue(CamName.toStdString(), ACQUISITION_IMAGEWIDTH, iImageWidth, true);
		pFileVariable->setValue(CamName.toStdString(), ACQUISITION_IMAGEHEIGHT, iImageHeight, true);

		pFileVariable->setValue(CamName.toStdString(), HKCAM_ATTRIBUTE_NAME, CamName.toStdString(), true);
		pFileVariable->setValue(CamName.toStdString(), HKCAM_ATTRIBUTE_SN, camSN.toStdString(), true);
		pFileVariable->setValue(CamName.toStdString(), HKCAM_ATTRIBUTE_EXPOSURE, iExposeTime, true);
		pFileVariable->setValue(CamName.toStdString(), HKCAM_ATTRIBUTE_GAIN, iGain, true);
		pFileVariable->setValue(CamName.toStdString(), HKCAM_ATTRIBUTE_TRIGMODE, itrig_mode, true);
		pFileVariable->setValue(CamName.toStdString(), HKCAM_ATTRIBUTE_TRIGEDGE, itrig_edge, true);
		pFileVariable->setValue(CamName.toStdString(), HKCAM_ATTRIBUTE_DELAY_TRIG, dalay_trig, true);
		m_strCamSn = camSN.toStdString();

		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CHANNEL_CNT, channelCnt, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SNAP_SIG_NAME, snap_sig_name, true);
		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_ACQ_TIME_OUT, acq_time_out, true);


		pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_INDEX, itrigIndex, true);

		//test测试是相机参数写入//
		ICameraFactory* m_pCameraFactory = (ICameraFactory*)GetSDKInstance("hiksourcecamera", ICameraFactory::IID_CAMERAFACTORY);
		if (NULL == m_pCameraFactory)
		{
			return;
		}
		string m_strSn = m_strCamSn;
		ICamera* pCamera = m_pCameraFactory->CreateCamera(m_strSn.c_str(), 0);
		if (pCamera == NULL)
		{
			return;
		}
		VS_CAMERA_INFO param;
		param.exposure = iExposeTime;
		param.gain = iGain;
		param.Activation = (CameraTriggerActivation)itrig_edge;
		param.TriggerMode = (CameraTriggerMode)itrig_mode;
		param.SerialNumber = m_strCamSn;
		pCamera->SetCameraInParam(param);
		//
	}
	
}

void CAreaCamPage::onCancel()
{
    onUpdate();
}

void CAreaCamPage::onUpdate()
{
    IFileVariable* pFileVariable = getAcquisitionFileVariable(m_strProductName.toStdString(),m_strKeyName.toStdString());

	bool bEnableGrap = true;
	bool bSaveRawImage = true;
	QString CamName = "";
	int iresolutionX = 0;
	int iresolutionY = 0;
	int iCamCnt = 1;
	int ioverlapX = 0;
	int ioverlapY = 0;
	int iImageWidth = 0;
	int iImageHeight = 0;
	string camSN = "";
	int iExposeTime = 0;
	int iGain =  0;
	int itrig_mode = 0;
	int itrig_edge = 0;
	double delay_trig = 0.0;
	int channelCnt = 1;
	string snap_sig_name = "";
	int acq_time_out = 6500;
	int max_channel_cnt = 1;
	int itrigIndex = 0;

	char key[100] = { 0 };
	sprintf(key, "ImageStation_%d", SString::strtoInt(m_strKeyName.toStdString()));
	IFileVariable* file = getStationCfgVariable();
	if (file != NULL)
	{
		max_channel_cnt = file->getValue(key, ACQUISITION_MAX_CHANNEL_CNT).toInt();
	}

	if (pFileVariable != NULL)
	{
		int index = ui->comboBox_cam_name->currentIndex();
		CamName = QString("ImageConfig_%1").arg(index);

		bEnableGrap = pFileVariable->getValue(CamName.toStdString(), ACQUISITION_GRAP_ENABLE).toBool(true);
		bSaveRawImage = pFileVariable->getValue(CamName.toStdString(), ACQUISITION_SAVE_RAW_IMAGE_ENABLE).toBool(true);
		iresolutionX = pFileVariable->getValue(CamName.toStdString(), ACQUISITION_CAM_RESOLUTIONX).toInt();
		iresolutionY = pFileVariable->getValue(CamName.toStdString(), ACQUISITION_CAM_RESOLUTIONY).toInt();
		iCamCnt = pFileVariable->getValue(CamName.toStdString(), ACQUISITION_CAM_CNT).toInt();
		ioverlapX = pFileVariable->getValue(CamName.toStdString(), ACQUISITION_OVERLAPX).toInt();
		ioverlapY = pFileVariable->getValue(CamName.toStdString(), ACQUISITION_OVERLAPY).toInt();
		iImageWidth = pFileVariable->getValue(CamName.toStdString(), ACQUISITION_IMAGEWIDTH).toInt();
		iImageHeight = pFileVariable->getValue(CamName.toStdString(), ACQUISITION_IMAGEHEIGHT).toInt();

		camSN =  pFileVariable->getValue(CamName.toStdString(), HKCAM_ATTRIBUTE_SN).toCString();
		iExposeTime = pFileVariable->getValue(CamName.toStdString(), HKCAM_ATTRIBUTE_EXPOSURE).toInt();
		iGain = pFileVariable->getValue(CamName.toStdString(), HKCAM_ATTRIBUTE_GAIN).toInt();
		itrig_mode = pFileVariable->getValue(CamName.toStdString(), HKCAM_ATTRIBUTE_TRIGMODE).toInt();
		itrig_edge = pFileVariable->getValue(CamName.toStdString(), HKCAM_ATTRIBUTE_TRIGEDGE).toInt();
		delay_trig = pFileVariable->getValue(CamName.toStdString(), HKCAM_ATTRIBUTE_DELAY_TRIG).toDouble();

		channelCnt = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CHANNEL_CNT).toInt();
		snap_sig_name = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SNAP_SIG_NAME).toCString();
		acq_time_out = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_ACQ_TIME_OUT).toInt();

		itrigIndex = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_INDEX).toInt();;

	}

	setUIItemValue("checkBox_enable_grap", bEnableGrap);
	setUIItemValue("checkBox_enable_save_raw", bSaveRawImage);
	setUIItemValue("spinBox_resolutionX", iresolutionX);
	setUIItemValue("spinBox_resolutionY", iresolutionY);
	setUIItemValue("spinBox_camCnt", iCamCnt);
	setUIItemValue("spinBox_overlapX", ioverlapX);
	setUIItemValue("spinBox_overlapY", ioverlapY);
	setUIItemValue("spinBox_ImageWidth", iImageWidth);
	setUIItemValue("spinBox_ImageHeight", iImageHeight);

	setUIItemValue("lineEdit_cam_sn", QString::fromStdString(camSN));
	setUIItemValue("spinBox_cam_expose", iExposeTime);
	setUIItemValue("spinBox_cam_gain", iGain);

	setUIItemValue("lineEdit_triggle_IO", QString::fromStdString(snap_sig_name));
	setUIItemValue("spinBox_channel_cnt", channelCnt);
	setUIItemValue("spinBox_AcqTimeOut", acq_time_out);

	setUIItemValue("comboBox_trig_index", itrigIndex);

	ui->spinBox_channel_cnt->setMaximum(max_channel_cnt);

	ui->comboBox_trig_mode->setCurrentIndex(itrig_mode);
	ui->comboBox_trig_edge->setCurrentIndex(itrig_edge);

	//setUIItemValue("comboBox_trig_mode", itrig_mode);
	//setUIItemValue("comboBox_trig_edge", itrig_edge);

	setUIItemValue("doubleSpinBox_delay_trig", delay_trig);

	m_strCamSn = camSN;

}
void CAreaCamPage::hideEvent(QHideEvent *event)
{
	on_pushButton_stop_clicked();
	QWidget::hideEvent(event);
}
//void CAreaCamPage::hideEvent()
//{
//	on_pushButton_stop_clicked();
//}

void CAreaCamPage::on_CamvalueChanged(int iValue)
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
}

void CAreaCamPage::on_comboBox_cam_name_currentIndexChanged(const QString &text)
{
	onUpdate();
}

void CAreaCamPage::on_pushButton_snap_clicked()
{
	ICameraFactory* m_pCameraFactory = (ICameraFactory*)GetSDKInstance("hiksourcecamera", ICameraFactory::IID_CAMERAFACTORY);
	if (NULL == m_pCameraFactory)
	{
		QMessageBox::warning(this, "warning", QString::fromLocal8Bit("获取相机ICameraFactory 实例失败"));
		return;
	}
	string m_strSn = getUItemValue("lineEdit_cam_sn", "").toString().toStdString();
	ICamera* pCamera = m_pCameraFactory->CreateCamera(m_strSn.c_str(), 0);
	if (pCamera == NULL)
	{
		QMessageBox::warning(this, "warning", QString::fromLocal8Bit("获取相机 （%1）ICamera实例失败").arg(QString::fromStdString(m_strSn)));
		return;
	}
	int w; int h;

	if (!pCamera->GetImageSize(w, h))
	{
		QMessageBox::warning(this, "warning", QString::fromLocal8Bit("（%1） GetImageSize失败").arg(QString::fromStdString(m_strSn)));
		return;
	}

	if (!pCamera->SingleAcq())
	{
		QMessageBox::warning(this, "warning", QString::fromLocal8Bit("（%1） SingleAcq失败").arg(QString::fromStdString(m_strSn)));
		return;
	}

	unsigned char* pBuffer = new unsigned char[w*h];
	;
	if (!pCamera->GetImage(pBuffer, 0, 0, 0, 0, 0))
	{
		QMessageBox::warning(this, "warning", QString::fromLocal8Bit("GetImage失败"));
		delete pBuffer;
		return;
	}
	// send image show
	ui->widget_show_image->showPictureInLabel(pBuffer, NULL, NULL, w, h);
	delete pBuffer;
}

void CAreaCamPage::on_pushButton_continue_clicked()
{
	m_bRunRealtime = true;
	if (m_pTriggerThread)
	{
		m_pTriggerThread->StartThread(this, &CAreaCamPage::ThreadTrigger, this);
	}
}

void CAreaCamPage::on_pushButton_stop_clicked()
{
	m_bRunRealtime = false;
    m_pmotioncontrol->setOrderToController(m_itrigNetid, Order_WorkStus, 0);
	ICameraFactory* m_pCameraFactory = (ICameraFactory*)GetSDKInstance("hiksourcecamera", ICameraFactory::IID_CAMERAFACTORY);
	if (NULL == m_pCameraFactory)
	{
		return;
	}
	ICamera* pCamera = m_pCameraFactory->CreateCamera(m_strCamSn.c_str(), 0);
	if (pCamera == NULL)
	{
		return;
	}
	pCamera->ClearBuf();
}

void CAreaCamPage::on_image_display(void* pBuffer)
{
	unsigned char * pImage = (unsigned char *)pBuffer;
	ui->widget_show_image->showPictureInLabel(pImage, NULL, NULL, m_w, m_h);
	delete pImage;
}

void CAreaCamPage::ThreadTrigger(void *param)
{
	ICameraFactory* m_pCameraFactory = (ICameraFactory*)GetSDKInstance("hiksourcecamera", ICameraFactory::IID_CAMERAFACTORY);
	if (NULL == m_pCameraFactory)
	{
		return;
	}
	string m_strSn = m_strCamSn;
	ICamera* pCamera = m_pCameraFactory->CreateCamera(m_strSn.c_str(), 0);
	if (pCamera == NULL)
	{
		return;
	}
	int w; int h;
	if (!pCamera->GetImageSize(w, h))
	{
		return;
	}
	pCamera->SetTriggerFunction(NULL, this);
	pCamera->ClearBuf();
	m_w = w;
	m_h = h;
    m_plightcontrol_new->clearCounter();
	Sleep(100);
    m_pmotioncontrol->setOrderToController(m_itrigNetid, Order_WorkStus, 0);
	Sleep(200);
	//光源触发-代码
	ControllerParams_t st;
	st.net_id = m_itrigNetid;
	st.order = Order_SetParms;
	st.trigger_frequence = 1;
    m_pmotioncontrol->setToController(st);
	Sleep(100);
    m_pmotioncontrol->setOrderToController(st.net_id, Order_WorkType, 1);
	Sleep(100);
    m_pmotioncontrol->setOrderToController(st.net_id, Order_WorkStus, 1);
	//
	
	while (m_bRunRealtime)
	{
		unsigned char* pBuffer = new unsigned char[w*h];
		if (!pCamera->SingleAcq())
		{
			delete pBuffer;
			break;
		}
		if (!pCamera->GetImage(pBuffer, 0, 0, 0, 0, 0))
		{
			delete pBuffer;
			break;
		}
		emit sigSendImage((void*)pBuffer);
		Sleep(100);
	}

}
