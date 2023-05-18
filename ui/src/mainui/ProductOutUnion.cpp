#include "ProductOutUnion.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "maindefine.h"
#include "filevariable/ifilevariable.h"
#include "signalcheck.h"
#include "sendmessage.h"
CProductOut::CProductOut(unsigned long long ullsn)
{
	m_bRun = true;
	m_pthread = NULL;
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	long ultime = 0;
	m_WcTime = 500;
	m_OffTime = 1200;
	if (NULL != pFile)
	{
		ultime    = pFile->getValue(CAMERAPARAM_BUSY_MARK ).toLong();// 
		m_OffTime = pFile->getValue(CAMERAPARAM_BUSY_TIME).toLong();// 
		m_WcTime = pFile->getValue(CAMERAPARAM_NORMAL_TIME).toLong();// 
		
	}
	m_Time = ultime;
	m_flag = en_Pr_Wait;
	ResultFlag = res_OK3;
	m_proSn = ullsn;
	m_starTime=JTime::CurrentMillisecond();
	m_strQListMsg.clear();

}

CProductOut::~CProductOut()
{
	if (m_bRun)
	{
		m_bRun = false;
	}
	if (NULL != m_pthread)
	{
		do 
		{
			Sleep(1);
		} while (m_pthread->IsRunning());
		delete m_pthread;
		m_pthread = NULL;
	}

}

enPrOutFlag CProductOut::getFlag()
{
	return m_flag;
}

void CProductOut::ThreadTimeOut(void* param)
{

	m_flag = en_Pr_Moving;
	uint64 iStartTime = JTime::CurrentMillisecond();
	uint64 iEndTime = 0;
	do
	{

		Sleep(50);
		iEndTime = JTime::CurrentMillisecond() - iStartTime;
	} while (m_bRun && m_Time > iEndTime);
	m_flag = en_Pr_OutBegin;
	
}

void CProductOut::startTimeOut()
{
	if (NULL != m_pthread)
	{
		m_pthread->StartThread(this, &CProductOut::ThreadTimeOut, this);
	}
}

void CProductOut::setResultFlag(enResFlag rstFlag)
{
	ResultFlag = rstFlag;
}
void CProductOut::setResultMsgToPenma(QStringList strMsg)
{
	m_strQListMsg = strMsg;
}
enResFlag CProductOut::getResultFlag()
{
	return ResultFlag;
}
unsigned long long CProductOut:: getSn()
{
	return m_proSn;
}
unsigned long long CProductOut::getStartTime()
{
	return m_starTime;
}
unsigned long  CProductOut::getMoveTimeBySet()
{
	return m_Time;
}
unsigned long  CProductOut::getOffTimeBySet()
{
	return m_OffTime;
}
unsigned long  CProductOut::getWcTimeBySet()
{
	return m_WcTime;
}
QStringList CProductOut::getPenmaMsg()
{
	return m_strQListMsg;
}
bool CProductOut::isCurrentProduct()
{
	bool res = true;
	long lTime = JTime::CurrentMillisecond() - m_starTime;
	UILogLevel leve = UILOG_WARN;
	if (abs(lTime - m_Time) > m_WcTime)
	{
		//leve = UILOG_POPRED;
		res = false;
	}
	QString msg = QString::fromLocal8Bit("采集位置到出口光纤预设时间 %1ms 实际时间 %2ms 预设误差 %3ms 实际误差 %4ms").arg(m_Time).arg(lTime).arg(m_WcTime).arg(lTime - m_Time);
	SENDMSG_INSTANCE->Send(msg, leve);//
	return res;
}
void CProductOut::setResultDataFlag(stProductDataRes* dataFlag)
{
	m_stProductDataRes = *dataFlag;

}
stProductDataRes* CProductOut::getResultDataFlag()
{

	return &m_stProductDataRes;

}
COutUnion *COutUnion::m_pInstance = NULL;
COutUnion::COutUnion(void)
{
	m_mutexPrOutList.Init();
	m_listPrOut.clear();
}
COutUnion::~COutUnion(void)
{
	releaseAllPrOut();
}
COutUnion * COutUnion::getInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new COutUnion();
	}
	return m_pInstance;
}

void COutUnion::releaseInstance()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void COutUnion::createPrOut(unsigned long long ullsn)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	CProductOut* pProduct = new CProductOut(ullsn);;
    m_listPrOut.push_back(pProduct);
}

enPrOutFlag COutUnion::getFirstFlag()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	if (m_listPrOut.size() > 0)
	{
		CProductOut* pProduct = m_listPrOut.at(0);
		return pProduct->getFlag();
	}
	return en_Pr_Wait;
}

void COutUnion::setResultDataFlag(unsigned long long ullsn, stProductDataRes* dataFlag)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	foreach(CProductOut* pstProduct, m_listPrOut)
	{
		if (pstProduct != NULL && pstProduct->getSn() == ullsn)
		{
			return pstProduct->setResultDataFlag(dataFlag);
		}
	}
}
CProductOut* COutUnion::getFirstProductOut()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	CProductOut* pProduct = NULL;
	if (m_listPrOut.size() > 0)
	{
		pProduct = m_listPrOut.at(0);
	}
	return pProduct;
}
unsigned long long COutUnion::getFirstSn()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	if (m_listPrOut.size() > 0)
	{
		CProductOut* pProduct = m_listPrOut.at(0);
		return pProduct->getSn();
	}
	return 0;
}

void COutUnion::releaseFirstPrOut()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	if (m_listPrOut.size() > 0)
	{
		CProductOut* pProduct = m_listPrOut.at(0);
		m_listPrOut.pop_front();
		delete pProduct;
		pProduct = NULL;
	}

}

void COutUnion::releasePrOut(CProductOut* pProOut)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	foreach(CProductOut* pstProduct, m_listPrOut)
	{
		if (pstProduct == pProOut)
		{
			delete pstProduct;
			m_listPrOut.removeOne(pProOut);
			return;
		}
	}
}
void COutUnion::releaseAllPrOut()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	foreach(CProductOut* pstProduct, m_listPrOut)
	{
		if (pstProduct != NULL)
		{
			delete pstProduct;;
		}
	}
	m_listPrOut.clear();
}

int COutUnion::getPrOutCnt()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	return m_listPrOut.size();
}
void  COutUnion::setResultMsgToPenma(unsigned long long ullsn, QStringList strMsg)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	foreach(CProductOut* pstProduct, m_listPrOut)
	{
		if (pstProduct != NULL && pstProduct->getSn() == ullsn)
		{
			return pstProduct->setResultMsgToPenma(strMsg);
		}
	}
}
void COutUnion::setResultFlag(unsigned long long ullsn, enResFlag ResultFlag)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	foreach(CProductOut* pstProduct, m_listPrOut)
	{
		if (pstProduct != NULL && pstProduct->getSn() == ullsn)
		{
			return pstProduct->setResultFlag(ResultFlag);
		}
	}
	return;
}

unsigned long long COutUnion::getProductOutSn(int index)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	if (index >= m_listPrOut.size())
	{
		CProductOut* pProduct = m_listPrOut.at(index);
		return pProduct->getSn();
	}
	return 0;
}


enResFlag COutUnion::getResultFlag(unsigned long long ullsn)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexPrOutList);
	foreach(CProductOut* pstProduct, m_listPrOut)
	{
		if (pstProduct != NULL && pstProduct->getSn() == ullsn)
		{
			return pstProduct->getResultFlag();
		}
	}
	return res_unknow;
}

//////////////////////////////////////
CProductOutLightCtr *CProductOutLightCtr::m_pInstance = NULL;
CProductOutLightCtr::CProductOutLightCtr(void)
{
	m_bRun = false;
	m_itimeMax = 1000;
	m_strRed    = "";
	m_strYellow ="";
	m_strGreen  = "";
	m_ResultFlag = res_OK3;
	m_pthread = NULL;
    m_pthread = new JThreadTP<CProductOutLightCtr, void*, void>;
	m_bRobot = false;
	m_iIodelay = 0;
	m_bRobotCheck = false;
	m_RobotResDataLock.Init();
}
CProductOutLightCtr::~CProductOutLightCtr(void)
{
	m_bRun = false;
	if (NULL != m_pthread)
	{
		do
		{
			Sleep(10);
		} while (m_pthread->IsRunning());
		delete m_pthread;
		m_pthread = NULL;
	}
}
CProductOutLightCtr * CProductOutLightCtr::getInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CProductOutLightCtr();
	}
	return m_pInstance;
}

void CProductOutLightCtr::releaseInstance()
{
	if (m_pInstance != NULL)
	{
		m_pInstance->m_bRun = false;
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void CProductOutLightCtr::ThreadLightCtr(void* param)
{
	bool bOpenG = false;
	bool bOpenY = false;
	bool bOpenR = false;
	productResData* pSendData = (productResData*)param;

	IFileVariable* pFileRobot = getCurrentRobotVariable();
	if( NULL == pFileRobot)
	{
		return ;
    }
    int iRobotTimes = pFileRobot->getValue(ROBOT_SET_RES_TIMES, 1000).toInt();
	if (!m_bRun)
	{
		return;
    }
    //控制哪个灯亮
	switch (pSendData->result)
	{
	case res_OK:
	case res_OK2:
		bOpenG = true;
		SignalCheck::writeIo(m_strGreen.toStdString().c_str(), bOpenG);
		break;
	case res_Ng:
		bOpenR = true;
		SignalCheck::writeIo(m_strRed.toStdString().c_str(), bOpenR);
		break;
	default:
		bOpenY = true;
		SignalCheck::writeIo(m_strYellow.toStdString().c_str(), bOpenY);
		break;
	}
	delete pSendData;

	uint64 iStartTime = JTime::CurrentMillisecond();
	uint64 iEndTime = 0;
	int i = 0;

	do  //灯亮持继时间
	{
		Sleep(50);

		iEndTime = JTime::CurrentMillisecond() - iStartTime;

		if (iRobotTimes <=iEndTime)//关灯
		{
			SignalCheck::writeIo(m_strYellow.toStdString().c_str(), false);
			Sleep(10);
			SignalCheck::writeIo(m_strGreen.toStdString().c_str(), false);
			Sleep(10);
            SignalCheck::writeIo(m_strRed.toStdString().c_str(), false);
			break;
		}
	} while (m_bRun);
}

bool CProductOutLightCtr::startOutLightCtr(int iSn,enResFlag ResultFlag,QString strRed, QString strYellow, QString strGreen, bool bRobot,int iIodelay)
{
    m_bRun = true;
    //m_itimeMax = timeMax;
    m_strRed = strRed;
    m_strYellow = strYellow;
    m_strGreen = strGreen;
    m_ResultFlag = ResultFlag;
    m_bRobot = bRobot;
    m_iIodelay = iIodelay;
    m_iSn = iSn;

    productResData* data = new productResData;
    data->Sn = m_iSn;
    data->result = m_ResultFlag;
    CProductOutLightCtr::ThreadLightCtr(data);
    return true;
    //if (NULL != m_pthread)
    //{
    //	if (!m_pthread->IsRunning())
    //	{
    //		productResData* data = new productResData;
    //		data->Sn = m_iSn;
    //		data->result = m_ResultFlag;

    		//m_pthread->StartThread(this, &CProductOutLightCtr::ThreadLightCtr, data);
    //		return true;
    //	}
    //	else
    //	{
    //		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("上次数据未发送完成，新数据又到 %1").arg(m_iSn), UILOG_ERR);
    //	}
    //}
    //return false;
}
