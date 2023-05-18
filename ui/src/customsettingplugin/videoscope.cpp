#include "videoscope.h"
#include "ui_videoscope.h"
#include "uicommon/curvegraphwidget.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "rtdefine.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "vscameratools/camera/ilinecamera.h"
#include "vscameratools/cameramanager/icamerabuffermanager.h"
#include "g12common.h"
#include "uicommon/commonutils.h"
#include "callsdk.h"
#include "inotifyservice.h"
#include "motion/imotion.h"
#include "card/icardcontinterp.h"
#include "g12arithmetic/iarithmeticStatic.h"
#include "filevariable/ifilevariable.h"
#include "rtdefine.h"
#include "g12utils.h"
#include "maindefine.h"
#include <Windows.h>
#include "customwidget.h"
#include "lightcontrol_new.h"
#include "stringcommon.h"

videoscope::videoscope(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::videoscope),
	m_pCurGraph(NULL),
	m_imageInfoBuffer(NULL),
	m_holdSize(0),
	m_bTriggerRunning(false),
	m_bInitCamRunning(false),
	//m_pThreadTrigger(NULL),
	m_duSleepTime(10000),
	m_trigger_frequence(1000),
    m_iCurrentSelect(0),
    m_iCurrentProType(0),
    m_pIArithmeticStaticMgr(NULL),
    m_pIArithmeticStatic(NULL),
    m_pCfgFile(NULL),
    m_pTriggerThread(NULL),
    m_bIsInit(false),
    m_iUnionWidth(8192),
    m_iUnionHeight(1),
    m_pCameraBufferManager(NULL)
{
    IFileVariable* pFileVariable = getAcquisitionFileVariable();
    if (pFileVariable != NULL)
    {
        m_iUnionWidth = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH, 8192).toInt();
        m_iUnionHeight = 1;// pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT, 1).toInt();
        pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH, &m_iUnionWidth);
        //pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT, &m_iUnionHeight);
    }
    ui->setupUi(this);
    init();

	new CustomWidget(this);
}

videoscope::~videoscope()
{
    uninit();
    while (m_pTriggerThread->IsRunning())
    {
        Sleep(100);
    }
    while (m_pInitThread->IsRunning())
    {
        Sleep(100);
    }
    //on_pushButton_continues_clicked(false);
    delete ui;
}

void videoscope::init()
{
    if (NULL == m_pCurGraph)
    {
        m_pCurGraph = new CurveGraph();
        ui->stackedWidget_videoscope->addWidget(m_pCurGraph);
        m_pCurGraph->setXAxisUpper(m_iUnionWidth);
        m_pCurGraph->setXAxisLower(0);
        m_pCurGraph->setYAxisUpper(255);
        m_pCurGraph->setYAxisLower(0);
        m_pCurGraph->setRangeUpper(255);
        m_pCurGraph->setRangeLower(0);
    }
    iMotion *pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
    if (NULL != pMotion)
    {
        ICard *pCard = (ICard *)pMotion->GetCardInstance(CONTROLCARD_NAME);
        if (NULL != pCard)
        {
            m_pCardConInterp = dynamic_cast<ICardContInterp*>(pCard);
        }
    }

    m_pIArithmeticStaticMgr = (IArithmeticStaticMgr*)GetSDKInstance("g12arithmetic", IArithmeticStaticMgr::IDD_ARITHMETICSTATICMGR);

    m_pTriggerThread = new JThreadTP<videoscope, void *, void>;
    m_DispFinished.Init(true, false);
	//添加工站
	int stationCnt = getImageStationCnt();
	for (int i = 0; i < stationCnt; ++i)
	{
		QString stationName = QString::fromLocal8Bit(getStationName(i).c_str());
		ui->comboBox_image->addItem(stationName);
	}
	connect(ui->comboBox_image, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboBox_image_currentIndexChanged(int)));


    m_pCfgFile = getCameraSetCfgFileVariable();
    connect(this, SIGNAL(sigShowData()), this, SLOT(on_showData()));
    connect(this, SIGNAL(sigInitUI()), this, SLOT(slotInitUI()));
    m_pInitThread = new JThreadTP<videoscope, void *, void>;

    SetUIEnable(false);
    m_bInitCamRunning = true;
    m_pInitThread->StartThread(this, &videoscope::ThreadInitCam, this);
}

void videoscope::uninit()
{
    m_bTriggerRunning = false;
    m_bInitCamRunning = false;
}

void videoscope::initUI()
{
    if (!m_bIsInit)
    {
        bool bChannelOn[SEQUE_NUMBER] = { 0 };
        int iVal[CAMERA_NUMBER] = { 0 };           //获取相机数
        ui->comboBox_channel->clear();
		int iTmpiSpe = getSequenceNumber(SString::IntToStr(0));
		QString strSeqName = getCurrentProgramRecipesList().c_str();
		QStringList strSeqList = strSeqName.split(",");
		if (iTmpiSpe > strSeqList.size())
		{
			return;
		}
		disconnect(ui->comboBox_channel, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboBox_channel_currentIndexChanged(int)));
		ui->comboBox_channel->clear();
		for (int i = 0; i < iTmpiSpe; ++i)
		{
			ui->comboBox_channel->addItem(strSeqList[i]);
		}
		connect(ui->comboBox_channel, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboBox_channel_currentIndexChanged(int)));
		ui->comboBox_channel->setCurrentIndex(0);
        m_bIsInit = true;
    }
}

void videoscope::showEvent(QShowEvent *event)
{
	slot_comboBox_image_currentIndexChanged(ui->comboBox_image->currentIndex());
    QWidget::showEvent(event);
}

void videoscope::hideEvent(QHideEvent *event)
{
    if (ui->pushButton_continues->isChecked())
    {
        on_pushButton_continues_clicked(false);
        ui->pushButton_continues->setChecked(false);
    }
    QWidget::hideEvent(event);
}
void videoscope::on_pushButton_hold_clicked(bool checked)
{
    if (m_holdSize < MAX_LINE_COUNT)
    {
        m_holdSize++;
    }
}

void videoscope::on_pushButton_clear_clicked(bool checked)
{
    m_holdSize = 0;
    m_pCurGraph->clearValue();
}

void videoscope::on_pushButton_continues_clicked(bool checked)
{
    int index = ui->comboBox_channel->currentIndex();
	if (index < 0)
		index = 0;
	int iBufferindex = ui->comboBox_image->currentIndex();
	if (iBufferindex < 0)
		iBufferindex = 0;

	bool bIsGGCard = false;
	bool bEnable = true;
	int netid = 1;

	IBufferManagerFactory* pIBufferManagerFactory = (IBufferManagerFactory*)GetSDKInstance("cameramanager", IBufferManagerFactory::IID_CAMERA_BUFFER_MANAGER_FACTORY);
	if (pIBufferManagerFactory == NULL)
	{
		return;
	}
	string strScene = QString::number(iBufferindex).toStdString();

	IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), strScene);
	if (pFileVariable != NULL)
	{
		int imode = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_SOURCE, 0).toInt();
		if (0 == imode)
		{
			bIsGGCard = true;
		}
		bEnable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_GRAP_ENABLE, true).toBool(true);
	}
	if (!bEnable)
	{
		QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("该工位未使能！"));
        return;
	}

    if (getStationType(iBufferindex) == 1)
    {
        QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("面阵相机无曲线功能！"));
        return;
    }


	m_pCameraBufferManager = pIBufferManagerFactory->createBufferManager(strScene);

    UpdateImageBuffer();

	iMotion *pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
	RETURN_IF(NULL == pMotion);
	ICard *pCard = (ICard *)pMotion->GetCardInstance(CONTROLCARD_NAME);
	RETURN_IF(NULL == pCard);
    ICardContInterp *pCardConInterp = dynamic_cast<ICardContInterp*>(pCard);
    iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion", iMotionCfg::IID_MOTION_CONFIG);
    RETURN_IF(NULL == pMotionCfg);
    StCardInfo cardInfo;
    pMotionCfg->getCardInfo("card", cardInfo);
	motioncontrol_new *	pMotioncontrol = NULL;

    //if (0 == strcmp(cardInfo.cardType, "cardgg")) 2.5d_P_2.0不使用固高卡触发
    //{
    //    bIsGGCard = true;
    //}

    if (checked)
    {
        LINE_CAM_PARAM info;
        m_pCurGraph->clearValue();
        ui->pushButton_continues->setEnabled(true);
        bool bRet = false;
        do 
        {

			uint8_t  ls_config_id = 0;
			lightcontrol_new* plightcontrol_new = lightcontrol_new_manager::GetInstance()->getLightcontrol(iBufferindex);
			if (0 != plightcontrol_new->clearCounter())
			{
				QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("Network清计数失败失败！"));
			}

	
			Sleep(200);
			m_pCameraBufferManager->reset();
            if (m_pCameraBufferManager->isBusying())
            {
                m_pCameraBufferManager->stopContinuations(m_iCurrentSelect);
            }

            bRet = m_pCameraBufferManager->continuations(m_iCurrentSelect, videoscope::CallbackGrapFunction, this);
        } while (0);
        if (!bRet)
        {
            ui->pushButton_continues->setChecked(false);
            m_pCameraBufferManager->stopContinuations(m_iCurrentSelect);
            m_bTriggerRunning = false;

        }
        else
        {
            m_DispFinished.SetEvent();
            m_bTriggerRunning = true;

            if (bIsGGCard)
            { 
                if (NULL != m_pTriggerThread)
                {
                    m_pTriggerThread->StartThread(this, &videoscope::ThreadTrigger, this);
                }
            }
            else
            {

				int iMotionCtl = 0;
				IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), QString::number(iBufferindex).toStdString());
				if (pFileVariable != NULL)
				{
					iMotionCtl = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_INDEX, 0).toInt();	
					netid = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_NETID).toInt();
				}
				pMotioncontrol = motioncontrol_new_manager::GetInstance()->getMotioncontrol(iMotionCtl);
			
				if (pMotioncontrol != NULL)
				{
					ControllerParams_t st;
					st.net_id = netid;
					st.order = Order_SetParms;
					st.trigger_frequence = m_trigger_frequence;
					pMotioncontrol->setToController(st);
					Sleep(100);
					pMotioncontrol->setOrderToController(st.net_id, Order_WorkType, 1);
					Sleep(100);
					pMotioncontrol->setOrderToController(st.net_id, Order_WorkStus, 1);
				}
            }
        }
    }
    else
    {
       
		if (m_bTriggerRunning)
		{
			if(bIsGGCard)
			{
				m_bTriggerRunning = false;
				pCardConInterp->stopComparePos(0);
			}
			else
			{
				int iMotionCtl = 0;
				IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), QString::number(iBufferindex).toStdString());
				if (pFileVariable != NULL)
				{
					iMotionCtl = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_INDEX, 0).toInt();
					netid = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_NETID).toInt();
				}
				pMotioncontrol = motioncontrol_new_manager::GetInstance()->getMotioncontrol(iMotionCtl);
				if (pMotioncontrol != NULL)
				{
					pMotioncontrol->setOrderToController(netid, Order_WorkStus, 0);
				}
			}	
		}
		
		m_pCameraBufferManager->stopContinuations(m_iCurrentSelect);
        m_bTriggerRunning = false;

    }
}

void videoscope::slot_comboBox_channel_currentIndexChanged(int index)
{
	if (index < 0)
	{
		index = 0;
	}
	if (ui->pushButton_continues->isChecked())
	{
		on_pushButton_continues_clicked(false);
	}
	
	ui->pushButton_continues->setChecked(false);
    m_currentChName = ui->comboBox_channel->currentText().toLocal8Bit().data();
    if (m_pIArithmeticStaticMgr != NULL)
    {
        m_pIArithmeticStatic = m_pIArithmeticStaticMgr->getArithmeticStatic(m_currentChName);
    }
    m_iCurrentSelect = index;

 //   string strCamList[CAMERA_NUMBER];
 //   int iType = 0;
 //   int iCamIndex = 0;
 //   //获取当前通道对应的相机索引
 //   m_iCurrentSelect = -1;
 //   getSqueLightTypeAndCameraIndex(m_currentChName, iType, iCamIndex);
 //   for (int i = 0; i <= index; i++)
 //   {
 //       int iType;
 //       int iCamEableIndex = 0;
 //       if (0 == getSqueLightTypeAndCameraIndex(getSeqNameWithID(i), iType, iCamEableIndex)
 //           && iCamEableIndex > 0 && iCamEableIndex == iCamIndex)
 //       {
 //           m_iCurrentSelect++;
 //       }
 //   }

	//if (m_iCurrentSelect < 0)
	//	m_iCurrentSelect = 0;
}

void videoscope::on_comboBox_pro_type_currentIndexChanged(int index)
{
    m_iCurrentProType = index;
	ImageProcess();
}

void videoscope::slot_comboBox_image_currentIndexChanged(int index)
{
	if (index < 0)
	{
		index = 0;
	}
	if (ui->pushButton_continues->isChecked())
	{
		on_pushButton_continues_clicked(false);
	}
	
	ui->pushButton_continues->setChecked(false);

	int iTmpiSpe = getSequenceNumber(SString::IntToStr(index));
	QString strSeqName = getCurrentProgramRecipesList().c_str();
	QStringList strSeqList = strSeqName.split(",");
	if (iTmpiSpe > strSeqList.size())
	{
		return;
	}
	disconnect(ui->comboBox_channel, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboBox_channel_currentIndexChanged(int)));
	ui->comboBox_channel->clear();
	for (int i = 0; i < iTmpiSpe; ++i)
	{
		ui->comboBox_channel->addItem(strSeqList[i]);
	}
	connect(ui->comboBox_channel, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboBox_channel_currentIndexChanged(int)));
	ui->comboBox_channel->setCurrentIndex(0);
}

void videoscope::on_doubleSpinBox_rate_valueChanged(double dValue)
{
    m_duSleepTime =  1000000/dValue;
	m_trigger_frequence = dValue;
}

void videoscope::on_spinBox_min_limit_valueChanged()
{
    double min = ui->spinBox_min_limit->value();
    double max = ui->spinBox_max_limit->value();
    if (min < max)
    {
        m_pCurGraph->setRangeLower(min);
    }
    m_pCurGraph->setRangeLower(ui->spinBox_min_limit->value());
}

void videoscope::on_spinBox_max_limit_valueChanged()
{
    double min = ui->spinBox_min_limit->value();
    double max = ui->spinBox_max_limit->value();
    if (min < max)
    {
        m_pCurGraph->setRangeUpper(max);
    }
}


void videoscope::on_showData()
{
    ImageProcess();
    updateVideo(m_holdSize);
    m_DispFinished.SetEvent();
}

void videoscope::slotInitUI()
{
    initUI();
    SetUIEnable(true);
}

void videoscope::CallbackGrapFunction(void * obj, unsigned char *buffer, int width, int height)
{
    if (NULL != obj && NULL != buffer)
    {
        videoscope * pVideoScope = (videoscope *)obj;
        if (NULL != pVideoScope)
        {
            //if (pVideoScope->m_DispFinished.IsSiangaled())
            {
                //CISAPUGuard<JMutex> locker(pCamera->m_imageLock);
                //pVideoScope->m_imageLock.Lock();
                if (NULL == pVideoScope->m_imageInfoBuffer)
                {
                    pVideoScope->m_imageInfoBuffer = new stImageInfo;
                }
                if (pVideoScope->m_imageInfoBuffer->width != width
                    || pVideoScope->m_imageInfoBuffer->height != height)
                {
                    delete []pVideoScope->m_imageInfoBuffer->pixelR;
                    pVideoScope->m_imageInfoBuffer->pixelR = NULL;
                    pVideoScope->m_imageInfoBuffer->width = width;
                    pVideoScope->m_imageInfoBuffer->height = height;
                }
                if (NULL == pVideoScope->m_imageInfoBuffer->pixelR)
                {
                    pVideoScope->m_imageInfoBuffer->pixelR = new unsigned char[pVideoScope->m_imageInfoBuffer->width * pVideoScope->m_imageInfoBuffer->height];
                }
                //pVideoScope->m_imageInfoBuffer->width = width;
                //pVideoScope->m_imageInfoBuffer->height = height;
                int iHeight = pVideoScope->m_imageInfoBuffer->height;// > height ? height : pVideoScope->m_imageInfoBuffer->height;
                for (int i = 0; i < iHeight; i++)
                {
                    int iStart = i*pVideoScope->m_imageInfoBuffer->width;
                    memcpy(pVideoScope->m_imageInfoBuffer->pixelR + iStart, buffer + i*width, sizeof(unsigned char) * width);
                }
            }
            //通知更新图像
            if (pVideoScope->m_DispFinished.IsSiangaled())
            {
                pVideoScope->m_DispFinished.ResetEvent();
                emit pVideoScope->sigShowData();
            }
        }
    }
}

bool videoscope::updateVideo(int valueIndex)
{
    if (m_imageInfoBuffer == NULL)
    {
        return false;
    }
    int maxValue = 0;
    int minValue = 255;
    double meanValue = 0;
    double varianceValue = 0;
    QVector<double> x;
    QVector<double> y;
    QVector<double> b;
    int iCurValue = 0;
    for (int i = 0; i < m_imageInfoBuffer->width; i++)
    {
        iCurValue = m_imageInfoBuffer->pixelR[0 + i];
        x << i;
        y << iCurValue;
        if (iCurValue > maxValue)
        {
            maxValue = iCurValue;
        }
        if (iCurValue < minValue)
        {
            minValue = iCurValue;
        }
        meanValue += iCurValue;
    }
    meanValue = meanValue / m_imageInfoBuffer->width;
    ui->doubleSpinBox_mean->setValue(meanValue);
    ui->spinBox_min->setValue(minValue);
    ui->spinBox_max->setValue(maxValue);

    for (int i = 0; i < m_imageInfoBuffer->width; i++)
    {
        double dOffset = 0;
        iCurValue = m_imageInfoBuffer->pixelR[0 + i];
        dOffset = meanValue - iCurValue;
        varianceValue += (dOffset * dOffset);
    }
    varianceValue = varianceValue/ m_imageInfoBuffer->width;
    ui->doubleSpinBox_variance->setValue(varianceValue);

    m_pCurGraph->setValues(x, y, valueIndex);
    update();
    return true;
}
void videoscope::ImageProcess()
{
    bool bStaticShading = false;
    bool bDynamicShading = false;
    bool bSignalFilter = false;
    unsigned char maxGray = 0.0;
    unsigned char  minGray = 0.0;
    double mean = 0.0;
    double devi = 0.0;
    int width = m_imageInfoBuffer->width;
    int height = m_imageInfoBuffer->height;
    if (m_pCfgFile != NULL)
    {
        bStaticShading = m_pCfgFile->getValue(IMAGE_PARAM_STATIC_ENABLE).toBool();
        bDynamicShading = m_pCfgFile->getValue(IMAGE_PARAM_DYNAMIC_ENABLE).toBool();
        bSignalFilter = m_pCfgFile->getValue(IMAGE_PARAM_FILTER_ENABLE).toBool();
    }
    unsigned char *pSrcBuffer = m_imageInfoBuffer->pixelR;
    unsigned char *pDstBuffer = new unsigned char[width * height];
    memcpy(pSrcBuffer, m_imageInfoBuffer->pixelR, width * height);
    m_imageInfoBuffer->pixelR = pSrcBuffer;
    //静态校准
    if (NULL != m_pIArithmeticStatic && m_iCurrentProType > 0 && bStaticShading)
    {
        RETURN_IF(0 > m_pIArithmeticStatic->iniStatCorr(width, height));
        RETURN_IF(0 > m_pIArithmeticStatic->StatCorr(pSrcBuffer,pDstBuffer));
        m_pIArithmeticStatic->GetStatParam(pDstBuffer, width, 1, maxGray, minGray, mean, devi);
        ui->doubleSpinBox_mean->setValue(mean);
        ui->spinBox_min->setValue(minGray);
        ui->spinBox_max->setValue(maxGray);
        ui->doubleSpinBox_variance->setValue(devi);
        memcpy(m_imageInfoBuffer->pixelR, pDstBuffer, width * height);
        pSrcBuffer = m_imageInfoBuffer->pixelR;
    }
    return;
}

void videoscope::ThreadTrigger(void *param)
{
    while (m_bTriggerRunning)
    {
        usSleep(m_duSleepTime);
        m_pCardConInterp->SetComparePluse(2, 0, 50);
    }
}

void videoscope::ThreadInitCam(void *param)
{
    while (m_bInitCamRunning)
    {
		int iBufferindex = ui->comboBox_image->currentIndex();
		if (iBufferindex < 0)
			iBufferindex = 0;

		IBufferManagerFactory* pIBufferManagerFactory = (IBufferManagerFactory*)GetSDKInstance("cameramanager", IBufferManagerFactory::IID_CAMERA_BUFFER_MANAGER_FACTORY);
		if (pIBufferManagerFactory != NULL)
		{
			string strScene  = QString::number(iBufferindex).toStdString();
			if (isImageStationEnable(iBufferindex))
			{
				m_pCameraBufferManager = pIBufferManagerFactory->createBufferManager(strScene);
				if (NULL != m_pCameraBufferManager && m_pCameraBufferManager->isInitFinished())
				{
					emit sigInitUI();
					break;
				}
			}
			else
			{
				emit sigInitUI();
				break;
			}
		}
        Sleep(1000);
    }
    m_bInitCamRunning = false;
}

void videoscope::UpdateImageBuffer()
{
    if (NULL == m_imageInfoBuffer)
    {
        m_imageInfoBuffer = new stImageInfo;
    }
    if (NULL != m_imageInfoBuffer->pixelR
        && m_imageInfoBuffer->width != m_iUnionWidth
        && m_imageInfoBuffer->height != m_iUnionHeight)
    {
        delete[]m_imageInfoBuffer->pixelR;
        m_imageInfoBuffer->pixelR = NULL;
    }

    if (NULL == m_imageInfoBuffer->pixelR)
    {
        m_imageInfoBuffer->width = m_iUnionWidth;
        m_imageInfoBuffer->height = m_iUnionHeight;
        m_imageInfoBuffer->pixelR = new unsigned char[m_imageInfoBuffer->width * m_imageInfoBuffer->height];
        updateVideo(m_holdSize);
    }
}

void videoscope::SetUIEnable(bool bEnable)
{
    ui->comboBox_channel->setEnabled(bEnable);
	ui->comboBox_image->setEnabled(bEnable);
    ui->comboBox_pro_type->setEnabled(bEnable);
    ui->pushButton_continues->setEnabled(bEnable);
    ui->pushButton_hold->setEnabled(bEnable);
    ui->pushButton_clear->setEnabled(bEnable);
    ui->doubleSpinBox_rate->setEnabled(bEnable);
}

