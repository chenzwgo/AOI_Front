#include "imagestation.h"
#include "cimagelogicmanager.h"
#include "lightcontrol_new.h"
#include "g12common.h"
#include "filevariable\ifilevariable.h"
#include "stringcommon.h"
#include "motion/imotion.h"
#include "card/icardcontinterp.h"
#include "station/istation.h"
#include "callsdk.h"
#include "rtdefine.h"
#include "sendmessage.h"
#include "station\istation.h"
#include "station\istationmanage.h"
#include "utils.h"
#include "signalcheck.h"
#include "ProductOutUnion.h"
#include "chkimagelogic.h"
#include "ProductMark.h"
#include "frontpc.h"
#include "commonapi.h"
#include "workflow.h"
CImageStation::CImageStation(int index):m_iIndex(index)
{
	m_pImageLogicManager = NULL;
	m_pmotioncontrol = NULL;
	m_plightcontrol_new = NULL;
	m_bcardgg = false;
	m_iMotionCtl = 0;
	m_itrigNetid = 1;
	m_pCardConInterp = NULL;
	m_pTargetStation = NULL;
	m_bRunning = false;
    m_bLineCam = false;
	m_bEnable = false;

	m_startLastGrapTime = JTime::CurrentMillisecond();

	m_pGrapthread = new JThreadT<CImageStation, bool>();
	readStationCfg();
	m_iProNumber = 0;
}

CImageStation::~CImageStation()
{
	stopGrapProcess();
	if (m_pGrapthread != NULL)
	{
		do
		{

			Sleep(50);
		} while (m_pGrapthread->IsRunning());
		delete m_pGrapthread;
		m_pGrapthread = NULL;
	}

}

bool CImageStation::GrapProcessFun()
{
	if (!m_bEnable)
		return true;
	m_bRunning = true;
	const char *szIOName = m_strTrigIOName.c_str();
	CameraParam cfg;
	getCameraPara(cfg);
	unsigned long long iSn = 0;
	bool bIsProOnsig = false;
	do 
	{
		//add do  2022-12-28-by lucus
		while (MAIN_WORK_PAUSE == WorkFlow::getInstance()->GetWorkState())
		{
			if (SignalCheck::readIo(szIOName))
			{
				bIsProOnsig = true;
			}
			Sleep(50);
			RETURN_V_IF(!m_bRunning, false);
		};
		if (bIsProOnsig)
		{
			RETURN_V_IF(!m_bRunning, false);
			if (!SignalCheck::readIo(szIOName))
			{
				bIsProOnsig = false;
			}
			Sleep(50);
			continue;
		}
		//---2022-12-28-by lucus end
		iSn = 0;
		if (getFirstEnableStation() == m_iIndex)
		{
			CImageProduct * pCImageProduct = CProductUnion::getInstance()->createProduct();
			for (int i = 0; i <getImageStationCnt(); i++)
			{
				CImageStation *pCImageStation = CImageStationManager::GetInstance()->getImageStation(i);
				if (pCImageStation != NULL && pCImageStation->isEnable())
				{
					pCImageStation->pushBackProduct(pCImageProduct);
				}
			}
			iSn = pCImageProduct->getProductSn();
		}
		//��ֹ��ͼ
		AbortGrabImage();
		//Step 1: ��������ɼ�
		TriggerGrabImage();
		// ���Դ����
		ClearLightCrlNumber();
		//step 2: �ȴ������ź�
		while (1)
		{
			RETURN_V_IF(!m_bRunning, false);

			if (SignalCheck::readIo(szIOName))
			{
				Sleep(1);
				BREAK_IF(SignalCheck::readIo(szIOName));//�������
			}
			Sleep(1);
		}
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��(%1)��λ�����ʼ�ɼ�").arg(m_iIndex));
		if (getFirstEnableStation() == m_iIndex )//
		{
			COutUnion::getInstance()->createPrOut(iSn);      // ���Ͽڽ��
		}
		uint64 GrapTime = JTime::CurrentMillisecond();
		if (GrapTime - m_startLastGrapTime < cfg.MoveLenth)
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("����(%1)��λ���βɼ�ʱ��С��%2 ms����������󴥷�").arg(m_iIndex).arg(cfg.MoveLenth), UILOG_ERR);
			continue;
		}else
		{
			m_startLastGrapTime = GrapTime;
		}

		//Step 3 ����λ�ñȽ� 
		bool res = true;


		res = setControllerParams(m_bcardgg);
		

		RETURN_V_IF(!res, ERR_BUSSINESS_CARD_INSTANCE);

		//Step 4 �����ڴ�-- 

 		RETURN_V_IF(!StartAcqImage(), false);

		StartGetImage();
		//Step 4 �ȴ��ⴥ�����
		int iRet = waitTriggerFinish(m_bRunning);

		uint64 endTime = JTime::CurrentMillisecond();

		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("(number %2)finish����(%1)��λ����ɼ�������� time:%3 ms").arg(m_iIndex).arg(m_iProNumber).arg(endTime- GrapTime));
	} while (m_bRunning);

	return true;
}

void CImageStation::readStationCfg()
{
	m_bLineCam = getStationType(m_iIndex) == 0;
	IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), SString::IntToStr(m_iIndex));
	if (pFileVariable != NULL)
	{
		int imode = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_SOURCE, 0).toInt();

		m_bcardgg = imode == 0;
		if (!m_bLineCam)
		{
			m_bcardgg = false;
		}

		
		m_iMotionCtl = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_INDEX, 0).toInt();
		m_itrigNetid = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_TRIG_NETID).toInt();
		m_bEnable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_GRAP_ENABLE, true).toBool(true);
		m_strTrigIOName = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SNAP_SIG_NAME).toCString();

		pFileVariable->bindValue(ACQUISITION_IMAGECONFIG, ACQUISITION_GRAP_ENABLE, &m_bEnable);
	}
	
	
	if (!m_bcardgg&&m_bLineCam)
	{
        if(m_pmotioncontrol == NULL)
        {
            m_pmotioncontrol = motioncontrol_new_manager::GetInstance()->getMotioncontrol(m_iMotionCtl);
        }
    }


	iMotion *pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
	ICard *pCard = (ICard *)pMotion->GetCardInstance(CONTROLCARD_NAME);
	m_pTargetStation = pMotion->GetStationInstance(TARGET_STATION);
	m_pCardConInterp = dynamic_cast<ICardContInterp*>(pCard);
	
    if(m_pImageLogicManager == NULL)
    {
        if(m_bLineCam)
        {
            m_pImageLogicManager = new CImageLogicManager(m_iIndex);
        }
        else
        {
            m_pImageLogicManager = new CHkImageLogicManager(m_iIndex);
        }

    }
    if(m_plightcontrol_new == NULL)
    {
        m_plightcontrol_new = lightcontrol_new_manager::GetInstance()->getLightcontrol(m_iIndex);
    }

}


IImageLogicManager * CImageStation::getImageLogicManager()
{
	return m_pImageLogicManager;
}

lightcontrol_new * CImageStation::getLightcontrol()
{
	return m_plightcontrol_new;
}

bool CImageStation::isEnable()
{
	return m_bEnable;
}
bool CImageStation::startGrapProcess(bool bThread)
{
	if (!m_bEnable)
		return true;
	m_bRunning = true;
	if (bThread)
	{
		if (m_pGrapthread->IsRunning())
			return false;
		m_pGrapthread->StartThread(this, &CImageStation::GrapProcessFun);
	}
	return true;
}

bool CImageStation::stopGrapProcess()
{
	if (!m_bEnable)
		return true;
	m_bRunning = false;
	if (m_pCardConInterp != NULL)
	{
		m_pCardConInterp->stopComparePos(0);
	}
	if (m_pmotioncontrol != NULL)
	{
		m_pmotioncontrol->setOrderToController(m_itrigNetid, Order_WorkStus, 0);
	}
	return true;
}

bool CImageStation::startContinueGrap()
{
	if (!m_bEnable)
		return true;
	return false;
}

bool CImageStation::stopContinueGrap()
{
	if (!m_bEnable)
		return true;
	return false;
}

bool CImageStation::resetStation()
{
	if (!m_bEnable)
		return true;
	readStationCfg();
	removeAllProduct();
	//WriteEleAndSqe();					// д������ͨ����Դ���ã���������
	//AbortGrabImage();					// �����ֹ��ͼ
	//ResetAcq();							// buffer��ֹȡͼ
	//ClearLightCrlNumber();		
	//m_pImageLogicManager->init();		
	//m_pImageLogicManager->setImageParam();
	return true;
}

bool CImageStation::WriteEleAndSqe()
{
	if (!m_bEnable)
		return true;
	if (m_plightcontrol_new == NULL)
	{
		return false;
	}
	if (1 == getStationType(m_iIndex))//���-����Ҫд����
	{
		return setControllerParams(false);
	}
	if (!m_plightcontrol_new->writeEleCfgToFlash())
	{
		return false;
	}
	Sleep(350);
	if (0 != m_plightcontrol_new->setAllMutFlashParams())	 //	дͨ���䷽
	{
		return false;
	}
	Sleep(350);
	if (setControllerParams(false))				// �Զ�����ƿ���д�������䷽
	{
		return false;
	}
	return true;
}

bool CImageStation::setControllerParams(bool bGgCard)
{
	if (!m_bEnable)
		return true;

	IFileVariable* pAcquisitionFile = getAcquisitionFileVariable(getCurrentProgramName(), SString::IntToStr(m_iIndex));
	int m_BlockSize = pAcquisitionFile->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_BLOCKLINES).toInt(256);
	if (m_BlockSize == 0)
	{
		m_BlockSize = 256;
	}

	IFileVariable* pFileVariable = getCameraSetCfgFileVariable(getCurrentProgramName());
	bool m_bIsFast = pFileVariable->getValue(IMAGE_PARAM_FAST_GRAB_ENABLE).toBool(false);

	bool res = false;
	CameraParam cfg;
	RETURN_V_IF(NULL == m_pTargetStation, false);
	getCameraPara(cfg);
	int Encoder = m_pTargetStation->GetAxisInfo(0).axisIndex + 1;
	double dPulse = m_pTargetStation->GetAxisInfo(0).pulsepermm;
	//��ȼ�ഥ��
	double iSequeNumber = getSequenceNumber(SString::IntToStr(m_iIndex));
	iSequeNumber = iSequeNumber < 0.000001 ? 1 : iSequeNumber;
	double dunit = cfg.dMoveUnit / iSequeNumber;//��� = ����/ʱ��
												   //int iMoveNumber = pInstance->m_iSequeNumber*cfg.dGetImageLen/cfg.dMoveUnit;//�ܴ������� = ʱ����*��Ұ����/����

												   // �ܴ������� = ʱ����*���Ŵ���//
	int CamLines = cfg.Fivscount;
	if (m_bIsFast)
	{
		CamLines = CamLines / 2;
		dunit = dunit * 2;
	}

	int iMoveNumber = iSequeNumber*CamLines;
	if (0 != iMoveNumber % m_BlockSize) //����������Ҫ���룬 ��Ȼͼ����λ
	{
		iMoveNumber = (iMoveNumber / m_BlockSize + 1) * m_BlockSize;  //����
	}

	if (bGgCard)//�̸߿�
	{
		if (m_pCardConInterp != NULL)
		{
			m_pCardConInterp->SetCompareLiner(Encoder, 2, cfg.dFstartpos, iMoveNumber, dunit, cfg.dPluseWidgh * 1000, 1);
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��λ%3-���ô����������, param: [%1, %2]").arg(cfg.dFstartpos).arg(iMoveNumber).arg(m_iIndex));

		}
		return true;//�̸߿�
	}
	if (m_pmotioncontrol != NULL) // �Զ�����ƿ�
	{

		ClearLightCrlNumber();
		Sleep(150);
		//1�����
		ControllerParams_t stParams;
		stParams.net_id = m_itrigNetid;
		stParams.order = 2;
		stParams.trigger_frequence = 1000;
		stParams.all_pulse.all_pulse_1 = iMoveNumber + cfg.iFiv;
		stParams.delay_pulse.delay_pulse_1 = cfg.dFstartpos*dPulse;

		stParams.equal_space.pulse_number_1 = dunit*dPulse;

		//�������
		if (1 == getStationType(m_iIndex))
		{
			stParams.trigger_frequence = 10;
			stParams.equal_space.pulse_number_1 = cfg.dMoveUnit*dPulse;
			int ichannel_max = 1;
			char key[100] = { 0 };
			sprintf(key, "ImageStation_%d", m_iIndex);
			IFileVariable* file = getStationCfgVariable();
			if (file != NULL) //������ͨ������ʹ�������ļ�ָ��
			{
				ichannel_max = file->getValue(key, ACQUISITION_MAX_CHANNEL_CNT).toInt();
			}
			stParams.all_pulse.all_pulse_1 = cfg.Fivscount*ichannel_max + cfg.iFiv;
			
		}
		
		//

		int ires = m_pmotioncontrol->setToController(stParams);
		if (0 != ires)
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("������ ���ô�������  ʧ�� "), UILOG_ERR);
			return false;
		}
		Sleep(250);
		//2��ȼ�ഥ��ģʽ
		ires = m_pmotioncontrol->setOrderToController(stParams.net_id, Order_WorkType, 0);
		if (0 != ires)
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("������ ��ȼ�ഥ��ģʽ  ʧ�� "), UILOG_ERR);
			return false;
		}
		Sleep(250);
		//3 �迪ʼ���� 
		ires = m_pmotioncontrol->setOrderToController(stParams.net_id, Order_WorkStus, 1);
		if (0 != ires)
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("������ �迪ʼ����  ʧ�� "), UILOG_ERR);
			return false;
		}
	}
	return true;
}

bool CImageStation::ClearLightCrlNumber()
{
	if (!m_bEnable)
		return true;
	if (m_plightcontrol_new == NULL)
		return false;
	return m_plightcontrol_new->ClearNumber();
}

bool CImageStation::TriggerGrabImage()
{
	if (!m_bEnable)
		return true;
	if(m_pImageLogicManager == NULL)
		return false;

	return m_pImageLogicManager->triggerGrabImage();
}

bool CImageStation::StartGetImage()
{
	if (!m_bEnable)
		return true;
	if (m_pImageLogicManager == NULL)
		return false;
	return m_pImageLogicManager->startGetImage();
}

bool CImageStation::AbortGrabImage()
{
	if (!m_bEnable)
		return true;
	if (m_pImageLogicManager == NULL)
		return false;
	return m_pImageLogicManager->abortGrabImage();
}

bool CImageStation::waitTriggerFinish(bool& bRun)
{
	if (!m_bEnable)
		return true;
	QTime startTime = QTime::currentTime();
	QTime endTime = startTime;
	int iTimeOut = 3000;//�ȴ��������ʱ�䳬ʱ-��ȡͼ��ʱͬ��
	//��ȡ��ʱʱ��
	IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), SString::IntToStr(m_iIndex));
	if (pFileVariable != NULL)
	{
		iTimeOut = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_ACQ_TIME_OUT).toInt();
	}
	if (m_bcardgg)
	{
		RETURN_V_IF(NULL == m_pCardConInterp, ERR_BUSSINESS_CARD_INSTANCE);
		short istu = 0;//�����Ƿ���� ֵ1Ϊ��� ֵ 0Ϊ���ڽ���
		long  ldata[2] = { 0 };
		bool res = true;
		int ires = 0;
		do
		{
			Sleep(10);
			endTime = QTime::currentTime();
		} while (bRun && startTime.msecsTo(endTime) <= 50);//��ʱ50����
		do
		{
			res = m_pCardConInterp->GetCompareStatus(istu, ldata);
			if (!res)
			{
				break;
			}
			if (1 == istu)//�����Ƿ����
			{
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��λ-%1-���ƿ��Ѿ�������� LinerTarget:").arg(m_iIndex) + QString::number(ldata[0] + ldata[1]));				
				break;
			}
			Sleep(10);
			endTime = QTime::currentTime();
		} while (bRun && startTime.msecsTo(endTime) <= iTimeOut);
		endTime = QTime::currentTime();
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��λ-%1-���ƿ��Ѿ�������� time:%2").arg(m_iIndex).arg(startTime.msecsTo(endTime)));

	}
	else
	{
		CameraParam cfg;
		getCameraPara(cfg);
		do
		{
			endTime = QTime::currentTime();
			if (m_pImageLogicManager->isAcqFinished() && startTime.msecsTo(endTime) >= cfg.MoveLenth)//�����Ƿ����
			{								  //pInstance->GetLightCrlNumber_2();
				break;
			}
			Sleep(10);

		} while (bRun && startTime.msecsTo(endTime) <= iTimeOut);
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��λ-%1-m_pImageLogicManager time:%2").arg(m_iIndex).arg(startTime.msecsTo(endTime)));
	}
	//startTime = QTime::currentTime();
	getLightCrlNumber();
	//endTime = QTime::currentTime();
	//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��λ-%1-getLightCrlNumber time:%2").arg(m_iIndex).arg(startTime.msecsTo(endTime)));
	return true;
}

bool CImageStation::getLightCrlNumber()
{
	uint8_t  ls_config_id = 0;
	uint16_t trig[8] = { 0 };

	if (m_plightcontrol_new == NULL)
	{
		return false;
	}

	m_plightcontrol_new->getCounter(trig, (Light_Type)0);

	int totalcnt = 0;
	for (int i = 0; i < 8; i++)
	{
		totalcnt += trig[i];
	}
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��λ-%10-��Դ��������������(%9) :%1,%2,%3,%4,%5,%6,%7,%8")
		.arg(trig[0]).arg(trig[1]).arg(trig[2]).arg(trig[3]).arg(trig[4]).arg(trig[5]).arg(trig[6]).arg(trig[7]).arg(totalcnt).arg(m_iIndex));
	return true;
}

bool CImageStation::StartAcqImage()
{
    if (!m_bEnable)
        return true;

    bool bRet = false;
    CImageProduct *proInfo = NULL;
    IFileVariable* pAcquisitionFile = getAcquisitionFileVariable(getCurrentProgramName(), SString::IntToStr(m_iIndex));
    if ( pAcquisitionFile == NULL)
    {
		CSendMessage::GetInstance()->Send("StartAcqImage() getAcquisitionFileVariable(getCurrentProgramName(), SString::IntToStr(m_iIndex)); false", UILOG_POPRED);
		SignalCheck::SetSignal(SYS_STOP);
        return false;
    }

    int iTotalImageLen = pAcquisitionFile->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT).toInt();          // ������
    int iWidth = pAcquisitionFile->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH).toInt();

    if(getProductBuffSize() >0 && m_pImageLogicManager != NULL)
    {
        proInfo = popProduct();
		try
		{
			proInfo->setProductImageSize(iWidth, iTotalImageLen, m_iIndex);
		}
		catch (const std::exception& m_exception)
		{

			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��Ʒ���۵��������ڴ治�㣡"), UILOG_POPRED);
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit(m_exception.what()), UILOG_WARN);
			SignalCheck::SetSignal(SYS_STOP);
			return false;
		}
        m_pImageLogicManager->setProductPtr(proInfo);

		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("sn:%2 ��λ(%1)- ����ͼ��").arg(m_iIndex).arg(proInfo->getProductSn()));
	}
	else
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��λ(%1)-�����󴥷�����Ʒ�б�Ϊ��").arg(m_iIndex), UILOG_POPRED);
		SignalCheck::SetSignal(SYS_STOP);
		return false;
	}
	m_iProNumber=proInfo->getProductSn();
	return true;
}

bool CImageStation::IsAcqFinished()
{
	if (!m_bEnable)
		return true;
	if (m_pImageLogicManager == NULL)
		return false;
	return m_pImageLogicManager->isAcqFinished();
}

void CImageStation::ResetAcq()
{
	if (!m_bEnable)
		return ;
	if (m_pImageLogicManager == NULL)
		return ;
	m_pImageLogicManager->resetImageProcess();
}

bool CImageStation::SetCamerCfg()
{
	if (!m_bEnable)
		return true;
	if (m_pImageLogicManager == NULL)
		return false;

	m_pImageLogicManager->init();
	m_pImageLogicManager->setImageParam();

		
	return false;
}

bool CImageStation::saveRawImage(CImageProduct * pProduct)
{
	if (!m_bEnable)
		return true;
	IFileVariable *pCameraSetCfgFile = getCameraSetCfgFileVariable();
	if (pCameraSetCfgFile == NULL)
	{
		return false;
	}

	bool bSaveRawImge = pCameraSetCfgFile->getValue(IMAGE_PARAM_SAVE_SIMAGE_ENABLE, false).toBool();

	if (!bSaveRawImge)
	{
		return true;
	}

	IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), SString::IntToStr(m_iIndex));
	bSaveRawImge = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_SAVE_RAW_IMAGE_ENABLE).toBool(true);
	if (!bSaveRawImge)
	{
		return true;
	}
	vector<string> vect;
	getEnabelChannle(vect, SString::IntToStr(m_iIndex));
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	for (int i = 0; i < vect.size(); i++)
	{
		QString channel = QString::fromStdString(vect.at(i));
		QString imageName = QString::fromLocal8Bit("procuct(%1)-%2_%3-rawImge").arg(pProduct->getProductSn()).arg(QString::fromStdString(Utils::getCamSuffix(m_iIndex))).arg(channel);
		if (pProduct->getImageBuffer(channel, (int)m_iIndex) != NULL)
		{
			Utils::saveRawImgage(pProduct->getImageBuffer(channel, m_iIndex)->rawData.img_data, pProduct->getProductWidth(m_iIndex), pProduct->getProductHeight(m_iIndex), imageName);
		}
	}
    return true;
}

void CImageStation::pushBackProduct(CImageProduct *pProduct)
{
    CISAPUGuard<JMutex>   autoLock(m_stackMutex);
    m_productStack.push_back(pProduct);

}

CImageProduct *CImageStation::popProduct()
{
    CISAPUGuard<JMutex>   autoLock(m_stackMutex);
    CImageProduct *pCImageProduct  = NULL;
    if(m_productStack.size() >0)
    {
        pCImageProduct =m_productStack.front();
        m_productStack.pop_front();
    }
    return pCImageProduct;
}

void CImageStation::removeAllProduct()
{
    CISAPUGuard<JMutex>   autoLock(m_stackMutex);
    m_productStack.clear();
}

unsigned int CImageStation::getProductBuffSize()
{
    CISAPUGuard<JMutex>   autoLock(m_stackMutex);
    return m_productStack.size();
}

unsigned int CImageStation::getEnableChannelCnt()
{
    return getSequenceNumber(SString::IntToStr(m_iIndex));
}


bool CImageStation::getCameraPara(CameraParam &cfg)
{
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	string key = CAMERAPARAM;
	key = key + "_" + SString::IntToStr(m_iIndex);
	if (pFile)
	{

		cfg.dSpeed = pFile->getValue(CAMERAPARAM_SPEED).toDouble();
		cfg.dMoveUnit = pFile->getValue(key,CAMERAPARAM_MOVEUNIT).toDouble();

		cfg.dPluseWidgh = pFile->getValue(key,CAMERAPARAM_PLUSEWIDGH).toDouble();
		cfg.dGetImageLen = pFile->getValue(key,CAMERAPARAM_DGETIMAGELEN).toDouble();
		cfg.dFstartpos = pFile->getValue(key,CAMERAPARAM_FSTARTPOS).toDouble();

		cfg.MoveLenth = pFile->getValue(CAMERAPARAM_MOVELENTH).toInt();
		cfg.Fivscount = pFile->getValue(key,CAMERAPARAM_FIVSCOUNT).toInt();
		cfg.iSequence = pFile->getValue(CAMERAPARAM_ISEQUENCE).toInt();
		cfg.iFiv = pFile->getValue(key,CAMERAPARAM_FIVS).toInt();

		return true;
	}
	return false;
}


CImageStationManager* CImageStationManager::m_pManagerInstance = NULL;
CImageStationManager * CImageStationManager::GetInstance()
{
	if (m_pManagerInstance == NULL)
	{
		m_pManagerInstance = new CImageStationManager();
	}
	return m_pManagerInstance;

}

void CImageStationManager::ReleaseInstance()
{
	if (m_pManagerInstance != NULL)
	{
		delete m_pManagerInstance;
		m_pManagerInstance = NULL;
	}
}

CImageStation * CImageStationManager::getImageStation(int index)
{
	if (index >= 0 && index < sizeof(ImageStations) / sizeof(CImageStation*))
	{
		if (ImageStations[index] == NULL)
		{
			ImageStations[index] = new CImageStation(index);
		}
		return ImageStations[index];
	}
	return NULL;
}

void CImageStationManager::releaseImageStation(int index)
{
	if (index >= 0 && index < sizeof(ImageStations) / sizeof(CImageStation*))
	{
		if (ImageStations[index] != NULL)
		{
			delete ImageStations[index];
			ImageStations[index] = NULL;
		}
	}
}

CImageStationManager::CImageStationManager()
{
	for (int i = 0; i < sizeof(ImageStations) / sizeof(CImageStation*); i++)
	{
		ImageStations[i] = NULL;
	}
}

CImageStationManager::~CImageStationManager()
{
	for (int i = 0; i < sizeof(ImageStations) / sizeof(CImageStation*); i++)
	{
		if (ImageStations[i] != NULL)
		{
			delete ImageStations[i];
			ImageStations[i] = NULL;
		}
	}
}
