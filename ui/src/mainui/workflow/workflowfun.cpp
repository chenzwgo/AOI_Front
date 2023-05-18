#include "workflowfun.h"
#include "workflow.h"
#include "rtdefine.h"
#include "errcode_def.h"
#include "mainflowaction/imainflowfun.h"
#include "callsdk.h"
#include "sendmessage.h"
#include <stdio.h>
#include "uicommon/commonutils.h"//
#include "inotifyservice.h"
#include "card/icard.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "./vscameratools/camera/ilinecamera.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "proxymanager.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "lightcontrol_new.h"
#include "stringcommon.h"
#include "cimageprocess.h"
#include "trace.h"
#include "maindefine.h"
#include "ProductUnion.h"
#include "showdefect/diagramscene.h"
#include "workflow/InvokeArithmetic.h"
#include "commonapi.h"
#include "ProductOutUnion.h"
#include "dlArithmetic/idlArithmetic.h"
#include "netdevui/isocketlistcfg.h"
#include "netdevui/isocketproxy.h"
#include "iuiplugin.h"
#include "ProductMark.h"

#define  TARGET_STATION     "target"//
#define  CODEC_STATION      "Out"
QVector<QRgb> g_colors;

WorkFlowFun *WorkFlowFun::m_pInstance = NULL;

WorkFlowFun::WorkFlowFun(void)
{
	m_pMotion = NULL;
    m_pCard = NULL;


    //m_pCameraManager = NULL;

    m_pTargetStation = NULL;
	m_pCodecStation = NULL;


    for (int i=0; i<256; ++i)
    {
        g_colors.push_back(qRgb(i,i,i));
    }
    ////�������л�ȡʱ��
    string programs = getCurrentProgramRecipesList();
    vector<string> vect = SString::split(programs,",");
    for (int i  = 0;i < SEQUE_NUMBER;i++)
    {
         m_seq[i] = (i < vect.size())?vect.at(i):"NOcfg";//
    }
    ////
    m_iSequeNumber =1;

    QString strCamNames = getCurrentProgramCamNameList().c_str();
    
	for (int i = 0; i < getImageStationCnt(); i++)
	{
		CImageStation* pCImageStation = CImageStationManager::GetInstance()->getImageStation(i);
		m_ImageStations.push_back(pCImageStation);
	}

    CInvokeArithmetic::getInstance()->startDeepLearnProcess();
	
    m_bRun = true;
    m_bInitCfg = false;

    m_csvFile = NULL;
    init();
    registerMainFlowCallBackFun();

}


WorkFlowFun::~WorkFlowFun(void)
{
    m_bRun = false;
    m_bRunning = false;

	for (int i = 0; i < m_ImageStations.size(); i++)
	{
		if (m_ImageStations.at(i) != NULL)
		{
			CImageStationManager::GetInstance()->releaseImageStation(i);
			m_ImageStations[i] = NULL;
		}
	}
	m_ImageStations.clear();

    CInvokeArithmetic::releaseInstance();
	Sleep(10);

	CProductOutLightCtr::releaseInstance();
	COutUnion::releaseInstance();

    CPrPrinterCtr::releaseInstance();
    CPrMarkUnion::releaseInstance();

}

WorkFlowFun * WorkFlowFun::getInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new WorkFlowFun;
    }

    return m_pInstance;
}

void WorkFlowFun::releaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}
void WorkFlowFun::registerMainFlowCallBackFun()
{
	//RETURN_IF(NULL == m_pCodecStation);
    // ���̻ص������ӿ�
    IMainFlowFun *pMainFlowFun = (IMainFlowFun *)GetSDKInstance("mainflowaction", IMainFlowFun::IDD_MAIN_FLOW_FUN);
    RETURN_IF(NULL == pMainFlowFun);
    if (m_csvFile == NULL)
    {
        m_csvFile = new QFile();
    }

    pMainFlowFun->registerFun("callFunSetCamerCfg", &WorkFlowFun::callFunSetCamerCfg, this, "���������");

    //��ˮ��ģʽ�»ص��� ��ʵЧ����״̬��������
    pMainFlowFun->registerFun("callFunPieceByPiecework", &WorkFlowFun::callFunPieceByPiecework, this, "��ˮ�������ɼ�����");         //��ˮ�������ɼ����� 

    pMainFlowFun->registerFun("callFunResetCam", &WorkFlowFun::callFunResetCam, this, "��λ���");

    pMainFlowFun->registerFun("callFunMoveForCheck", &WorkFlowFun::callFunMoveForCheck, this, "����ֶ����ƶ�");
    pMainFlowFun->registerFun("callFunMoveJobForCheck", &WorkFlowFun::callFunMoveJobForCheck, this, "�������������");
    pMainFlowFun->registerFun("callFunSetRun", &WorkFlowFun::callFunSetRun, this, "�����б�־");      
    pMainFlowFun->registerFun("callFunUSleep", &WorkFlowFun::callFunUSleep, this, "Sleep(us)");
	//
	pMainFlowFun->registerFun("callFunWriteEleAndSqe", &WorkFlowFun::callFunWriteEleAndSqe, this, "��Դ������д�������䷽");

   
    pMainFlowFun->registerFun("callFunResetLight", &WorkFlowFun::callFunResetLight, this, "��չ�Դ����������");
	pMainFlowFun->registerFun("callFunCheckInProductOnTime", &WorkFlowFun::callFunCheckInProductOnTime, this, "����ָʾ�Ʋ���");

	//TEST
	pMainFlowFun->registerFun("callFunAutoTriggerByLines", &WorkFlowFun::callFunAutoTriggerByLines, this, "�����Զ�����(����)");

	pMainFlowFun->registerFun("callFunDeepLearnEnv", &WorkFlowFun::callFunDeepLearnEnv, this, "H�㷨��ʼ�����");
	

    m_pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
    RETURN_IF(NULL == m_pMotion);
    m_pCard = (ICard *)m_pMotion->GetCardInstance(CONTROLCARD_NAME);
    RETURN_IF(NULL == m_pCard);


    iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion", iMotionCfg::IID_MOTION_CONFIG);
    RETURN_IF(NULL == pMotionCfg);
    StCardInfo cardInfo;
    pMotionCfg->getCardInfo("card", cardInfo);

    m_pTargetStation = m_pMotion->GetStationInstance(TARGET_STATION);

    RETURN_IF(NULL == m_pTargetStation);
    m_pCodecStation = m_pMotion->GetStationInstance(CODEC_STATION);
	RETURN_IF(NULL == m_pCodecStation);
}


bool WorkFlowFun::getCameraPara( CameraParam &cfg )
{
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile)
	{
		
		cfg.dSpeed       = pFile->getValue(CAMERAPARAM_SPEED).toDouble();
	    cfg.MoveLenth    = pFile->getValue(CAMERAPARAM_MOVELENTH).toInt();

        cfg.bSaveSourceImage = pFile->getValue(IMAGE_PARAM_SAVE_SIMAGE_ENABLE).toBool();
        cfg.bEableStaticImage = pFile->getValue(IMAGE_PARAM_STATIC_ENABLE).toBool();
        cfg.bEableDynamic = pFile->getValue(IMAGE_PARAM_DYNAMIC_ENABLE).toBool();
        cfg.bEableFilterImage = pFile->getValue(IMAGE_PARAM_FILTER_ENABLE).toBool();
        cfg.bSaveCorrectImage = pFile->getValue(IMAGE_PARAM_SAVE_CORRENT_IMAGE).toBool();
		return true;
	}
    return false;
}

int WorkFlowFun::callFunSetCamerCfg( void* parent, string param )
{
    WorkFlowFun *pInstance = (WorkFlowFun*)parent;

	for (int i = 0; i < pInstance->m_ImageStations.size(); i++)
	{
		CImageStation* pCImageStation = pInstance->m_ImageStations.at(i);
		if (pCImageStation != NULL)
		{
			pCImageStation->SetCamerCfg();
		}
	}
    return 0;
}

int WorkFlowFun::callFunMoveForCheck( void* parent, string param )
{
    WorkFlowFun *pInstance = (WorkFlowFun*)parent;
    QString strInPara = QString::fromStdString(param);
    RETURN_V_IF(NULL == pInstance, ERR_BUSSINESS_CARD_INSTANCE);
    RETURN_V_IF(NULL == pInstance->m_pTargetStation, ERR_BUSSINESS_CARD_INSTANCE);

    //��ȡ������Ϣ
    CameraParam cfg;
    pInstance->getCameraPara(cfg);
    pInstance->m_pTargetStation->Stop();
    //----����ʹ��-��״̬-����-//
    pInstance->m_pTargetStation->EnableAxis(0,true);
    pInstance->m_pTargetStation->ClearAxisStatus(0);
    pInstance->m_pTargetStation->ResetAxisPos(0);
    //---���ٶ�--//
    StSpeed sp;
    sp.vel = cfg.dSpeed;
    sp.acc = cfg.dSpeed;
    sp.dec = cfg.dSpeed;
    pInstance->m_pTargetStation->AxisSetSpeed(0,sp);

	double dMoveLenth = cfg.MoveLenth;
	if (!strInPara.isEmpty())
	{
		dMoveLenth = -dMoveLenth;
	}
    return pInstance->m_pTargetStation->AxisMoveOffset(0, dMoveLenth);
}
int WorkFlowFun::callFunUSleep(void* parent, string param)                //�������ѧϰ�ӿ�
{
	QString strTime = QString::fromStdString(param);
	int uTime = strTime.toULongLong();
	if (0 < uTime)
	{
		UsSleep(uTime);
	}
	return SYS_ERR_OK;
}
#include "signalcheck.h"

int WorkFlowFun::callFunPieceByPiecework(void * parent, string param)
{
	WorkFlowFun *pInstance = (WorkFlowFun*)parent;
	unsigned int iStationIndex = 0;
	if (param.length() >0)
	{
		iStationIndex = SString::strtoInt(param);
	}
	if (iStationIndex < pInstance->m_ImageStations.size())
	{
		CImageStation* pCImageStation = pInstance->m_ImageStations.at(iStationIndex);
		if (pCImageStation != NULL)
		{
			pCImageStation->startGrapProcess(false);
		}
	}
	return 0;
}

bool WorkFlowFun::Crlmove(int imove)
{
	if (NULL != m_pTargetStation)//��ֹͣ
	{
		m_pTargetStation->Stop();
		m_pTargetStation->EnableAxis(0, true);
		m_pTargetStation->ClearAxisStatus(0);
		m_pTargetStation->ResetAxisPos(0);
	}
	if (NULL != m_pCodecStation)//��ֹͣ
	{
		m_pCodecStation->Stop();
		m_pCodecStation->EnableAxis(0, true);
		m_pCodecStation->ClearAxisStatus(0);
		m_pCodecStation->ResetAxisPos(0);
	}
	if (0 == imove)//0 ֹͣ
	{
		return true;
	}
	CameraParam cfg;
	getCameraPara(cfg);
	//---���ٶ�--//
	StSpeed sp;
	sp.vel = cfg.dSpeed;
	sp.acc = cfg.dSpeed;
	sp.dec = cfg.dSpeed;
	m_pTargetStation->AxisSetSpeed(0, sp);
	m_pTargetStation->AxisMoveJog(0, imove>0);

	m_pCodecStation->AxisSetSpeed(0, sp);
	m_pCodecStation->AxisMoveJog(0, imove > 0);


}
bool  WorkFlowFun::JobMoveForClearMode()
{
	if (NULL == m_pTargetStation)
	{
		return false;
	}
	bool bIsMoving = false;
	m_pTargetStation->IsAxisMoving(0, bIsMoving);
	if (bIsMoving)
	{
		return true;
	}
	CameraParam cfg;
	getCameraPara(cfg);
	//---���ٶ�--//
	StSpeed sp;
	sp.vel = cfg.dSpeed;
	sp.acc = cfg.dSpeed;
	sp.dec = cfg.dSpeed;
	m_pTargetStation->AxisSetSpeed(0, sp);
	return	m_pTargetStation->AxisMoveJog(0, true);

}
int WorkFlowFun::callFunMoveJobForCheck( void* parent, string param )
{
    WorkFlowFun *pInstance = (WorkFlowFun*)parent;
    QString strInPara = QString::fromStdString(param);
    RETURN_V_IF(NULL == pInstance, ERR_BUSSINESS_CARD_INSTANCE);
    RETURN_V_IF(NULL == pInstance->m_pTargetStation, ERR_BUSSINESS_CARD_INSTANCE);
    bool bMove = true;//Ĭ��ֵΪ����--�����ֵ С��0����Ϊ����
    if (strInPara.length()>0)
    {
        bMove = (strInPara.toInt()<0)?false:true;
    }
    //��ȡ������Ϣ

    //�ж��Ƿ����˶�//
    bool bIsMoving = false;
    pInstance->m_pTargetStation->IsAxisMoving(0, bIsMoving);
    if (bIsMoving)
    {
        //�����˶�����OK
        return 0;
    }

    CameraParam cfg;
    pInstance->getCameraPara(cfg);
    pInstance->m_pTargetStation->Stop();
    //----����ʹ��-��״̬-����-//
    pInstance->m_pTargetStation->EnableAxis(0,true);
    pInstance->m_pTargetStation->ClearAxisStatus(0);
    pInstance->m_pTargetStation->ResetAxisPos(0);
    //---���ٶ�--//
    StSpeed sp;
    sp.vel = cfg.dSpeed;
    sp.acc = cfg.dSpeed;
    sp.dec = cfg.dSpeed;
    pInstance->m_pTargetStation->AxisSetSpeed(0,sp);

    return pInstance->m_pTargetStation->AxisMoveJog(0,bMove);
 
}

int WorkFlowFun::callFunSetRun( void* parent, string param )
{
    WorkFlowFun *pInstance = (WorkFlowFun*)parent;
    QString strInPara = QString::fromStdString(param);
    RETURN_V_IF(NULL == pInstance, ERR_BUSSINESS_CARD_INSTANCE);
    if (strInPara.length()>0)
    {
        pInstance->m_bRun = (strInPara.toInt()<=0)?false:true;
    }
    return 0;
}


void WorkFlowFun::Reset()
{
    m_bRun = false;

    CInvokeArithmetic::getInstance()->SetUnInitDeepLearn();//����ʼ���㷨
    SignalCheck::writeIo("do_out_green", false);
	Sleep(150);
	SignalCheck::writeIo("do_out_red", false);
	Sleep(150);
	SignalCheck::writeIo("do_out_yellow", false);

    iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion", iMotionCfg::IID_MOTION_CONFIG);
    RETURN_IF(NULL == pMotionCfg);
    StCardInfo cardInfo;
    pMotionCfg->getCardInfo("card", cardInfo);


    if (m_pTargetStation)
    {
        m_pTargetStation->Stop();
        m_pTargetStation->ClearAxisStatus(0);
        m_pTargetStation->EnableAxis(0, true);
    }

    for (int i = 0; i < m_ImageStations.size(); i++)
    {
        CImageStation* pCImageStation = m_ImageStations.at(i);
        if (pCImageStation != NULL)
        {
            pCImageStation->resetStation();
        }
    }
    COutUnion::getInstance()->releaseAllPrOut();
    CPrMarkUnion::getInstance()->releaseAllPrMark();
}

void WorkFlowFun::Start()
{
    for (int i = 0; i < m_ImageStations.size(); i++)
    {
        CImageStation* pCImageStation = m_ImageStations.at(i);
        if (pCImageStation != NULL)
        {
            pCImageStation->startGrapProcess();
        }
    }
    CInvokeArithmetic::getInstance()->startPocess();
    m_bRun = true;
}

void WorkFlowFun::Stop()
{
    m_bRun = false;
    for (int i = 0; i < m_ImageStations.size(); i++)
    {
        CImageStation* pCImageStation = m_ImageStations.at(i);
        if (pCImageStation != NULL)
        {
            pCImageStation->stopGrapProcess();
        }
    }
}

void WorkFlowFun::Reinit()
{
    m_bInitCfg = false;
    init();
}


int WorkFlowFun::callFunWriteEleAndSqe(void* parent, string param)
{
    WorkFlowFun *pInstance = (WorkFlowFun*)parent;

	string strLog = "";

	for (int i = 0; i < pInstance->m_ImageStations.size(); i++)
	{
		CImageStation* pCImageStation = pInstance->m_ImageStations.at(i);
		if (pCImageStation != NULL)
		{
			pCImageStation->WriteEleAndSqe();
		}
	}
    pInstance->m_bInitCfg = true;
	return 0;
}

void WorkFlowFun::init()
{
	IFileVariable* pFileVariable = getAcquisitionFileVariable();
	pFileVariable = getCameraSetCfgFileVariable();

}
int WorkFlowFun::callFunResetCam(void* parent, string param)
{
    WorkFlowFun *pInstance = (WorkFlowFun*)parent;

	if (NULL == pInstance)
	{
		return -1;
	}
	for (int i = 0; i < pInstance->m_ImageStations.size(); i++)
	{
		CImageStation* pCImageStation = pInstance->m_ImageStations.at(i);
		if (pCImageStation != NULL)
		{
			if (pCImageStation->AbortGrabImage())
			{
				pCImageStation->ResetAcq();
			}
		}
	}

    return 0;
}

int WorkFlowFun::callFunResetLight(void* parent, string param)
{
	WorkFlowFun *pInstance = (WorkFlowFun*)parent;

	for (int i = 0; i < pInstance->m_ImageStations.size(); i++)
	{
		CImageStation* pCImageStation = pInstance->m_ImageStations.at(i);
		if (pCImageStation != NULL)
		{
			pCImageStation->ClearLightCrlNumber();
		}
	}

	return 0;
}

int WorkFlowFun::callFunCheckInProductOnTime(void* parent, string param)
{
	//��� 1 �̵�io 2 ���ϸ�Ӧ�� 3 ��ʱͶ��ʱ��
	WorkFlowFun *pInstance = (WorkFlowFun*)parent;
	QString strInPara = QString::fromStdString(param);
	if (strInPara.isEmpty())
	{
		return 111111;
	}
	QStringList inParaList = strInPara.split(",");
	if (2 > strInPara.count())
	{
		return 11111;
	}
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (NULL == pFile)
	{
		return 1111;
	}
	int iNalTime = pFile->getValue(CAMERAPARAM_NORMAL_TIME).toInt();	  // ����ʱ�ļ��ʱ��
	int iCurNumber = pFile->getValue(CAMERAPARAM_CURPRONUMBER,0).toInt(); // ��ǰ�Ĳ�Ʒ�ѻ�����
	QString strWriteCheck = inParaList.at(0);
	QString strInCheck = inParaList.at(1);

	uint64 iStartTime = JTime::CurrentMillisecond();
	uint64 iEndTime   = JTime::CurrentMillisecond();
	uint64 iTime = 0;
	bool bMessage = true;
	bool bhas = false;
	SignalCheck::writeIo(strWriteCheck.toStdString().c_str(), true); //�������̵�
	while (pInstance->m_bRun)
	{
		int iNalTime = pFile->getValue(CAMERAPARAM_NORMAL_TIME).toInt();	  // ����ʱ�ļ��ʱ��
		//�в�Ʒ�Ȳ�Ʒ���� ���̵�
        if(SignalCheck::readIoEx(strInCheck.toStdString().c_str(),true,10,0))
		{
			if (SignalCheck::readIo(strWriteCheck.toStdString().c_str()))
			{
				SignalCheck::writeIo(strWriteCheck.toStdString().c_str(), false);
			}
			iStartTime = JTime::CurrentMillisecond();

			if (iTime < iNalTime && !bMessage)
			{
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��ƷͶ�Ź����ܼ�����ע����ָʾ����ɫʱ��Ͷ��"), UILOG_WARN);
				bMessage = true;
			}
			//if (bhas)
			//{
			//	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("�в�Ʒ"), UILOG_WARN);
			//	bhas = false;
			//}
		}
		else // ���û�в�Ʒ
		{
			bMessage = false;
			iEndTime = JTime::CurrentMillisecond();

			iTime = iEndTime - iStartTime;
			if (iTime > iNalTime)
			{
				if (!SignalCheck::readIo(strWriteCheck.toStdString().c_str()))
				{
					SignalCheck::writeIo(strWriteCheck.toStdString().c_str(), true);
				}
			}
		}
		Sleep(50);
	}
	return 0;
}


#include "g12utils.h"
int WorkFlowFun::callFunAutoTriggerByLines(void * parent, string param)
{
	iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion", iMotionCfg::IID_MOTION_CONFIG);
	RETURN_V_IF(NULL == pMotionCfg, SYS_ERR_MAINFLOW_FUNCTION);
	iMotion *pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
	RETURN_V_IF(NULL == pMotion, SYS_ERR_MAINFLOW_FUNCTION);
	ICard *pCard = (ICard *)pMotion->GetCardInstance(CONTROLCARD_NAME);
	RETURN_V_IF(NULL == pCard, SYS_ERR_MAINFLOW_FUNCTION);
	ICardContInterp *pCardConInterp = dynamic_cast<ICardContInterp*>(pCard);
	int itimes = atoi(param.c_str());
	itimes = itimes > 0 ? itimes : 1000;
	do
	{
		pCardConInterp->SetComparePluse(2, 0, 50);
		usSleep(100);
		itimes--;
	}while (itimes>0);
	return 0;
}



int WorkFlowFun::callFunDeepLearnEnv(void* parent, string param)
{
	int iCount = 80;
	if (!param.empty())
	{
		iCount = SString::strtoInt(param);
	}
	 while (iCount>0)
	 {
         InitDeepLearnState res =  CInvokeArithmetic::getInstance()->GetDeepLearnEnvState();
		 if (DeepLearnInitOK == res || DeepLearnNoNeed == res)
		 {
			 return 0;
		 }
		 else if (DeepLearnIintError == res)
		 { 
			 return 111111;
		 }
		 Sleep(200);
		 iCount--;
	 }
	 return 11111112;
}
int WorkFlowFun::queryBackPcStatus(int& iStatus)
{
	ISocketproxy* pCSocketproxy = Utils::getPCSocketproxy();
	if (pCSocketproxy == NULL)
	{
		return 2;
	}
	if (SYS_ERR_OK != pCSocketproxy->sendQuerySysStatus(iStatus))
	{
		CSendMessage::GetInstance()->Send(QString(QString::fromLocal8Bit("��ѯBackPC����״̬ʧ��")), UILOG_WARN);
		return 3;
	}
	return 0;
}
