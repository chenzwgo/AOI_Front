#include "signalcheck.h"
#include "proxymanager.h"
#include "motion/iioctrlinterface.h"
#include "card/icard.h"
#include "motion/imotion.h"
#include "sendmessage.h"
#include "rtdefine.h"
#include "iodefine.h"
#include "g12common.h"


E_SYS_STATE SignalCheck::m_state = SYS_IDLE;


iMotion* SignalCheck::m_pImotion = NULL;
IIOCtrlInterface * SignalCheck::m_pIo = NULL;

SignalCheck::SignalCheck(void)
{
}


SignalCheck::~SignalCheck(void)
{
}

void SignalCheck::SetSignal( E_SYS_STATE signal )
{
	m_state = signal;
}

bool SignalCheck::IsStopSig()
{	
	return IsUIClick(SYS_STOP);	
}

bool SignalCheck::IsEstopSig()
{
    return readIo(DI_ESTOP_SIGNAL);
}

bool SignalCheck::IsResetSig()
{
    return readIo(DI_RESET_SIGNAL) || IsUIClick(SYS_RESET);
}

bool SignalCheck::IsPauseSig()
{
    return readIo(DI_PAUSE_SIGNAL) || IsUIClick(SYS_PAUSE);
}

bool SignalCheck::IsStartSig()
{
    return readIo(DI_START_SIGNAL) || IsUIClick(SYS_START);
}

bool SignalCheck::IsPowerOffSig()
{
	return false;
}

bool SignalCheck::IsSafeDoorSig()
{
    return  false;
}

bool SignalCheck::readIo( const char* ioName )
{
    bool bOpen = false;

    if(m_pImotion == NULL)
        m_pImotion = (iMotion*) GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);

    if(m_pIo == NULL && m_pImotion != NULL)
        m_pIo = m_pImotion->GetIOCtrlInterface();

    if (NULL != m_pIo)
    {
        m_pIo->readio(ioName, bOpen);
    }
    return bOpen;
}


bool SignalCheck::writeIo( const char* ioName, bool bOpen )
{
    if(m_pImotion == NULL)
        m_pImotion = (iMotion*) GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);

    if(m_pIo == NULL && m_pImotion != NULL)
        m_pIo = m_pImotion->GetIOCtrlInterface();

    if (NULL != m_pIo)
    {
        return m_pIo->writeio(ioName, bOpen);
    }

    return false;
}

bool SignalCheck::IsUIClick( E_SYS_STATE signal )
{
	if (m_state == signal)
	{
		m_state = SYS_IDLE;
		return true;
	}

	return false;
}

bool SignalCheck::IsCardOnline()
{
    if(m_pImotion == NULL)
        m_pImotion = (iMotion*) GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
    RETURN_V_IF(NULL == m_pImotion, false);
    static ICard* pCard = m_pImotion->GetCardInstance("Card1");
	if (pCard){
        if (pCard->IsConnected()){
			return true;
		}
	}
	return false;
}

bool SignalCheck::OpenCard()
{
    static iMotion *pMotion = (iMotion*) GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
    RETURN_V_IF(NULL == pMotion, false);
    static ICard* pCard = pMotion->GetCardInstance("Card1");
	if (pCard){
        CSendMessage::GetInstance()->Send("控制卡未连接,正在重连中.....!", UILOG_WARN);
        if (pCard->Open()){
            CSendMessage::GetInstance()->Send("控制卡重连成功!", UILOG_INFO);
			return true;
		}
        CSendMessage::GetInstance()->Send("控制卡重连失败!", UILOG_WARN);
	}
	return false;
}

bool SignalCheck::SrvOnCard()
{
	iMotion *pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
	RETURN_V_IF(NULL == pMotion, false);
	ICard* pCard = pMotion->GetCardInstance("card1");
	if (pCard) {
		pCard->Reset();
	}
	return true;
}

bool SignalCheck::SrvOffCard()
{
	iMotion *pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
	RETURN_V_IF(NULL == pMotion, false);
	ICard* pCard = pMotion->GetCardInstance("card1");
	if (pCard) {
		
		return pCard->EmgStop();
	}

	return false;
}

bool SignalCheck::CrlLightToRobot(const char* ioName)
{
	return writeIo(ioName, true);
}
bool SignalCheck::CloseLightToRobot()
{
	writeIo(DO_OUT_NG, false);
	writeIo(DO_OUT_OK, false);
	writeIo(DO_OUT_RI, false);
	return true;
}
bool SignalCheck::OpenLight(const char* ioName)
{
	if (0 == QString::compare(ioName, DO_YELLOW_LIGHT))
	{
		writeIo(DO_YELLOW_LIGHT, true);
		writeIo(DO_GREED_LIGHT, false);
		writeIo(DO_RED_LIGHT, false);
	}
	else if (0 == QString::compare(ioName, DO_GREED_LIGHT))
	{
		writeIo(DO_YELLOW_LIGHT, false);
		writeIo(DO_GREED_LIGHT, true);
		writeIo(DO_RED_LIGHT, false);
	}
	else if (0 == QString::compare(ioName, DO_RED_LIGHT))
	{

		writeIo(DO_YELLOW_LIGHT, false);
		writeIo(DO_GREED_LIGHT, false);
		writeIo(DO_RED_LIGHT, true);

	}
	else
	{
		writeIo(DO_YELLOW_LIGHT, false);
		writeIo(DO_GREED_LIGHT, false);
		writeIo(DO_RED_LIGHT, false);
	}
	return true;
}

bool SignalCheck::readIoEx(const char* ioName, bool bFlag,  int iDurTime,int tryTime)
{
    bool bOpen = true;

    if(m_pImotion == NULL)
        m_pImotion = (iMotion*) GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);

    if(m_pIo == NULL && m_pImotion != NULL)
        m_pIo = m_pImotion->GetIOCtrlInterface();

     if(m_pIo == NULL)
         return false;

    int iTimes = tryTime;
    unsigned int iSleepTime = iDurTime;
    do
    {
        m_pIo->readio(ioName, bOpen);
        if (bFlag != bOpen)
        {
            return false;
        }
        Sleep(iSleepTime);
        iTimes =iTimes - 1;
    }while(iTimes >0);

    return bOpen;
}

bool SignalCheck::isCleanRunning()
{
    if(!isEnableClean())
    {
        return  true;
    }
    return  SignalCheck::readIo(DI_CLEAN_RUN);
}

bool SignalCheck::isShangRunning()
{
    if(!isEnableShang())
    {
        return  true;
    }
    return  SignalCheck::readIo(DI_SHANG_RUN);
}

bool SignalCheck::isXiaRunning()
{
    if(!isEnableXia())
    {
        return  true;
    }
    return  SignalCheck::readIo(DI_XIA_RUN);
}
bool SignalCheck::isXiaRunning1()
{
	if (!isEnableXia())
	{
		return  false;
	}
	return  SignalCheck::readIo(DI_XIA_RUN);
}

bool SignalCheck::isClearMode()
{
    if(!isEnableClearMode())
    {
        return false;
    }
    return  SignalCheck::readIo(DI_CLEAR_MODE);
}

void SignalCheck::setCleanOn(bool bON)
{
    if(!isEnableClean())
    {
        return ;
    }
    SignalCheck::writeIo(DO_CLEAN_RUN,bON);
}

void SignalCheck::setCleanPause(bool bPause)
{
    if(!isEnableClean())
    {
        return ;
    }
    SignalCheck::writeIo(DO_CLEAN_PAUSE,bPause);
}
void SignalCheck::setShangStop(bool bstop)
{
	if (!isEnableShang())
	{
		return;
	}
	SignalCheck::writeIo(DO_SHANG_STOP, bstop);
	//Sleep(300);
	//SignalCheck::writeIo(DO_SHANG_STOP, false);
}
void SignalCheck::setShangOn()
{
    if(!isEnableShang())
    {
        return ;
    }
    SignalCheck::writeIo(DO_SHANG_START,true);
	Sleep(300);
	SignalCheck::writeIo(DO_SHANG_START, false);
}

void SignalCheck::setXiaStop()
{
    if(!isEnableXia())
    {
        return ;
    }
    SignalCheck::writeIo(DO_XIA_STOP,true);
	Sleep(300);
	SignalCheck::writeIo(DO_XIA_STOP, false);
}

