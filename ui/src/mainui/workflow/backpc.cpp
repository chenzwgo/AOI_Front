#include "backpc.h"
#include <QMutex>
#include "maindefine.h"
#include "iodefine.h"
#include "signalcheck.h"
#include "utils.h"
#include "station/istation.h"
#include "filevariable/ifilevariable.h"
#include "ProductOutUnion.h"
#include "rtdefine.h"
#include "sendmessage.h"
#include "signalcheck.h"
#include "g12common.h"
#include "commonapi.h"
#include "workflow/workflow.h"
#include "workflow/workflowfun.h"
#include "ProductMark.h"
#include "trace.h"
#include "utils.h"
#include "netdevui/isocketproxy.h"
#include "robotcrl.h"
#include "outputDataSave.h"
#include "frontpc.h"
static QMutex g_frontpcMutex;

CBackPCtoFront *CBackPCtoFront::m_pInstance = NULL;


CBackPCtoFront::CBackPCtoFront():m_pThreadpenma(NULL)
{
    m_bRun = false;
    m_bPenmaThread = false;
    m_bSendDataThread = false;
	m_bClearModeThread = false;
    m_pThreadpenma = new JThreadT<CBackPCtoFront>;
    m_pThreadSendData = new JThreadT<CBackPCtoFront>;
	m_pThreadClearMode = new JThreadT<CBackPCtoFront>;
	
}

CBackPCtoFront::~CBackPCtoFront()
{
    m_bRun = false;
    m_bPenmaThread = false;
    m_bSendDataThread = false;
	m_bClearModeThread = false;
    if(m_pThreadpenma != NULL)
    {
        while(m_pThreadpenma->IsRunning())
        {
            Sleep(50);
        }
        delete m_pThreadpenma;
        m_pThreadpenma = NULL;
    }

    if(m_pThreadSendData != NULL)
    {
        while(m_pThreadSendData->IsRunning())
        {
            Sleep(50);
        }
        delete m_pThreadSendData;
        m_pThreadSendData = NULL;
    }
	if (m_pThreadClearMode != NULL)
	{
		while (m_pThreadClearMode->IsRunning())
		{
			Sleep(50);
		}
		delete m_pThreadClearMode;
		m_pThreadClearMode = NULL;
	}
}


CBackPCtoFront * CBackPCtoFront::getInstance()
{
    QMutexLocker locker(&g_frontpcMutex);
    if (NULL == m_pInstance)
    {
        m_pInstance = new CBackPCtoFront;
    }

    return m_pInstance;
}

void CBackPCtoFront::releaseInstance()
{
    QMutexLocker locker(&g_frontpcMutex);
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

bool CBackPCtoFront::isSynResultExist(unsigned int iSn)
{
    QMutexLocker locker(&m_synResultocker);
    return m_proResult.contains(iSn);
}

int CBackPCtoFront::getSynResult(unsigned int iSn)
{
    QMutexLocker locker(&m_synResultocker);
    if(m_proResult.contains(iSn))
    {
        return m_proResult.value(iSn);

    }
    return res_OK;
}

void CBackPCtoFront::pushBackSynResult(unsigned int iSn, int iRest)
{
    QMutexLocker locker(&m_synResultocker);
    m_proResult.insert(iSn,iRest);
}

void CBackPCtoFront::removeSynResult(unsigned int iSn)
{
    QMutexLocker locker(&m_synResultocker);
    if(m_proResult.contains(iSn))
    {
         m_proResult.remove(iSn);
    }
}

void CBackPCtoFront::clearSynResult()
{
    QMutexLocker locker(&m_synResultocker);
    m_proResult.clear();
}

void CBackPCtoFront::setRunflag(bool bFlag)
{
    m_bRun = bFlag;
}

int CBackPCtoFront::ShowResData()
{
    enResFlag ResultFlag = res_unknow;
    CProductOut* pCProductOut = NULL;
    unsigned long long getSn = 0;//Ĭ���쳣SN 0
	stProductDataRes DataRes;
    uint64 iStartTime = JTime::CurrentMillisecond();
	//long movetimeByset = 15000;
	long offtimeByset = 2000;
	//uint64 wctimeByset  = 500;
    double iTime;
    do
    {
        RETURN_V_IF(!m_pInstance->m_bRun, 1111);
		//add do  2022-12-28-by lucus
		while (MAIN_WORK_PAUSE == WorkFlow::getInstance()->GetWorkState())
		{
			Sleep(50);
			RETURN_V_IF(!m_pInstance->m_bRun, 1111);
		};
        if (SignalCheck::readIoEx(DI_OUT_END_SIG, true, 10,1))
        {
            // ������IO ��Ʒ�����Ͽ�//
            iTime = JTime::CurrentMillisecond() - iStartTime;
			//
			int idelayTime = 0;
			IFileVariable* pFileVariable = getCurrentRobotVariable();
			if (NULL != pFileVariable)
			{
				idelayTime = pFileVariable->getValue(PENMA_DELAY_TIMES, 0).toInt();
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("�趨���Ͽڹ����ϲ�Ʒ�ƶ�ʱ��:%1ms").arg(idelayTime));
			}
			//
			DataRes.init();
			//log-ct
			QString strPath = getSaveResultFilePath();
			strPath += "/" + getCurrentProducsTimeStrPath() + "/ct_result.log";
			QString strCt = QString("ct:%1 ms").arg(iTime);
			LOG_INFO_F_SIMP(strPath.toStdString().data(), strCt.toLatin1());

            SENDMSG_INSTANCE->sendCtToUi(iTime / 1000);
            iStartTime = JTime::CurrentMillisecond();
			bool enableRandomSend = randomResult((int*)&ResultFlag);
            //getSn++;
            getSn = 0;
            CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("���Ͽ��ź�"));
			//
			IFileVariable *pFile = getCameraSetCfgFileVariable();
			if (pFile)
			{
				offtimeByset = pFile->getValue(CAMERAPARAM_BUSY_TIME).toLong();// 
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("���Ͽ��ӳ�:%1msȡ�㷨���").arg(offtimeByset));
			}
			do
			{
				Sleep(50);
			} while (m_bRun&&JTime::CurrentMillisecond()- iStartTime<offtimeByset);
			//
            pCProductOut = COutUnion::getInstance()->getFirstProductOut();
            if (pCProductOut != NULL)//�в�Ʒ
            {
                //��Ʒ������Ԥ��ʱ�䵽λ�õĲ�Ʒ����ȡ���ͬʱ����--
                //�Է���һ�вɼ�λ�ù��˲�ʶ��Ĳ�Ʒ���������½������
                getSn = pCProductOut->getSn();
				
				DataRes = *pCProductOut->getResultDataFlag();
                if (!enableRandomSend)
                {
                    //ResultFlag = pCProductOut->getResultFlag();
					ResultFlag = DataRes.ResultFlag;
                }
				
				bool bisCurren = pCProductOut->isCurrentProduct();
				COutUnion::getInstance()->releasePrOut(pCProductOut);
				//if (!bisCurren)----����ͣ���Բ��ټ���Ʒ�ǲ��ǵ�ǰ�ġ�ʱ�䵼�²�һ��2022-12-27
				//{
				//	m_bRun = false;
				//	WorkFlow::getInstance()->setStopLine(true);
				//	return -1;
				//}
				
            }
			else
			{
				SENDMSG_INSTANCE->Send(QString::fromLocal8Bit("û�в�Ʒ����ȷ���Ƿ���Ͽ��󴥷���"), UILOG_POPRED);//
				WorkFlow::getInstance()->setStopLine(true);
				m_bRun = false;
				return -1;
			}

			int cnt = COutUnion::getInstance()->getPrOutCnt();
			QString strSendData = QString("%1|%2|%3").arg(ResultFlag).arg(DataRes.iNgOrRiDefectType).arg(QString::fromStdString(DataRes.str2Dcode));
			QString msg = QString::fromLocal8Bit("���Ͳ�Ʒ�������ˣ�SN:%1,Res:%2,Data:%3��ǰʣ���Ʒ:%4��").arg(getSn).arg(getResByResultFlag(ResultFlag)).arg(strSendData).arg(cnt);
			outputDataSave::GetInstance()->saveLog("FrontSendResData", msg);
            CSendMessage::GetInstance()->Send(msg);
			// ������������ϻ� add 20210520  bg
			if (res_OK== ResultFlag || res_OK2 == ResultFlag)
			{
				SignalCheck::CrlLightToRobot(DO_OUT_OK);
				//RobotCrl::GetInstance()->send_sn_and_res_to_xialiaoji(getSn, 0);
			}
			else //����OKֻ����NG
			{
				SignalCheck::CrlLightToRobot(DO_OUT_NG);
				//RobotCrl::GetInstance()->send_sn_and_res_to_xialiaoji(getSn, 2);
			}
			//else {
			//	SignalCheck::CrlLightToRobot(DO_OUT_RI);
			//	RobotCrl::GetInstance()->send_sn_and_res_to_xialiaoji(getSn, 1);
			//}
			//add 20210520  end
            //CProductOutLightCtr::getInstance()->startOutLightCtr(getSn, ResultFlag, DO_OUT_NG, DO_OUT_RI, DO_OUT_OK,false);//dele  20210520
             CSendMessage::GetInstance()->sendShowOkNGEnd(getSn, ResultFlag, DataRes.str2Dcode);
			 int icheckMax = 3;
			do
			{
				Sleep(50);
			    if (!SignalCheck::readIo(DI_OUT_END_SIG))//��Ʒ�뿪
			    {
					icheckMax--;
			    }

			} while (m_bRun&&icheckMax>0);
			//---ʱ�������һ��ѭ��
			do
			{
				Sleep(50);
			} while (m_bRun&&JTime::CurrentMillisecond() - iStartTime<idelayTime);
			//
            SignalCheck::CloseLightToRobot();

        }
        Sleep(20);
    } while (m_bRun);

    return 0;
}

void CBackPCtoFront::startSendData()
{
    if(m_pThreadSendData != NULL)
    {
        if (!m_pThreadSendData->IsRunning())
        {
            m_bSendDataThread = true;
            m_pThreadSendData->StartThread(this, &CBackPCtoFront::threadSendData);
        }
    }
}

void CBackPCtoFront::stopSendData()
{
    m_bSendDataThread = false;
}
void CBackPCtoFront::startClearMode()//��ʼ����
{
	if (m_pThreadClearMode != NULL)
	{
		if (!m_pThreadSendData->IsRunning())
		{
			m_bClearModeThread = true;
			m_pThreadClearMode->StartThread(this, &CBackPCtoFront::threadClearMode);
		}
	}
}
void CBackPCtoFront::stopClearMode()
{
	m_bClearModeThread = false;
}
bool CBackPCtoFront::isClearMode()
{
	return m_bClearModeThread;
}
void CBackPCtoFront::startPenma()
{
	if (!isEnablePenma())
	{
		return;
	}
    if(m_pThreadpenma != NULL )
    {
        if (!m_pThreadpenma->IsRunning())
        {
            m_bPenmaThread = true;
            m_pThreadpenma->StartThread(this, &CBackPCtoFront::threadPenma);
        }
    }
}

void CBackPCtoFront::stopPenma()
{
    m_bPenmaThread = false;
}

void CBackPCtoFront::threadPenma()
{
    enResFlag ResultFlag = res_unknow;
    CProductMark* pCProductOut = NULL;
    unsigned long long getSn = 0;//Ĭ���쳣SN 0
    IFileVariable* pFileRobot = getCurrentRobotVariable();
    if ( NULL == pFileRobot)
    {
        return ;
    }
    uint64 iStartTime = JTime::CurrentMillisecond();
    double iTime;
    stPrMark prMark;
	int iFirstPenma=1;
    do
    {
        CProductMark* pCProductOut = NULL;
        RETURN_IF(!m_pInstance->m_bRun);
        if (SignalCheck::readIoEx(DI_PENMA_SIG, true, 10,1))
        {
            // ������IO ��Ʒ�����Ͽ�//
            CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("������ź�"));
			//
			if (iFirstPenma<=2)//��ǰ��һ����������ǰ������������//2021-4-26
			{
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("��λ������� %1 ������Ҫ����").arg(iFirstPenma));
				iFirstPenma++;
				Utils::WaitGlassLeave(DI_PENMA_SIG, 500, 4000, m_bRun);//�Ȳ����뿪
				continue;//�����ȴ�����ѭ��
			}
			//
            pCProductOut = CPrMarkUnion::getInstance()->getFirstProductMark();
            if (pCProductOut != NULL)//�в�Ʒ
            {

                getSn = pCProductOut->getSn();
                prMark = pCProductOut->getMarkFlag();
                ResultFlag = pCProductOut->getMarkFlag().eFlag;
                CPrMarkUnion::getInstance()->releasePrMark(pCProductOut);
            }
            else
            {
                 //Utils::saveSn0Log(getSn);
            }
            QString msg = "PenmaSN:" + QString::number(getSn) + "  PenmaRes:" + getResByResultFlag(ResultFlag);
            int cnt = CPrMarkUnion::getInstance()->getPrMarkCnt();
            SENDMSG_INSTANCE->sendRobotLog(QString::fromLocal8Bit("Penma Sn %1 Res %2 PrMarkCnt cnt %3").arg(getSn).arg(getResByResultFlag(ResultFlag)).arg(cnt));//
            CSendMessage::GetInstance()->Send(msg);

            CPrPrinterCtr::getInstance()->startPrinterCtr(prMark);

            Utils::WaitGlassLeave(DI_PENMA_SIG,500,3000,m_bRun);

        }
        Sleep(20);
    } while (m_bRun && m_bPenmaThread);

    return ;
}

void CBackPCtoFront::threadSendData()
{
    while (m_bRun && m_bSendDataThread)
    {
        ShowResData();
        Sleep(5);
    }
}
void CBackPCtoFront::threadClearMode()
{
	enResFlag ResultFlag = res_Ng;
	unsigned long long getSn = 0;//Ĭ���쳣SN 0
	long offtimeByset = 1800;
	long clear = 1000;
	double iTime;
	uint64 iStartTime;
    WorkFlowFun::getInstance()->JobMoveForClearMode();
    CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("����ģʽ����,��ʼ����..."),UILOG_WARN);
	while (m_bClearModeThread)
	{
		
		if (SignalCheck::readIoEx(DI_OUT_END_SIG, true, 10, 1))
		{
			// ������IO ��Ʒ�����Ͽ�//
			iStartTime = JTime::CurrentMillisecond();
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("����ģʽ���Ͽ��ź�"));
			CProductOutLightCtr::getInstance()->startOutLightCtr(getSn, ResultFlag, DO_OUT_NG, DO_OUT_RI, "do_out_green", false);
			IFileVariable *pFile = getCameraSetCfgFileVariable();
            if (pFile)
			{
				offtimeByset = pFile->getValue(CAMERAPARAM_BUSY_TIME).toLong();// 
				clear = pFile->getValue(CAMERAPARAM_CLAR_TIME).toLong();
            }
			int imax = 4;
			do
			{
				Sleep(50);
				if (!SignalCheck::readIoEx(DI_OUT_END_SIG, true, 10, 1))
				{
					imax--;
				}

			} while (m_bClearModeThread&& imax>0);
		}
		Sleep(20);
		/*if (JTime::CurrentMillisecond() - iStartTime >clear)
		{
			stopClearMode();
			WorkFlow::getInstance()->setClearFinish(true);
		}*/
	}
	WorkFlowFun::getInstance()->Crlmove();
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("...����ģʽ����..."), UILOG_WARN);
}

bool CBackPCtoFront::randomResult(int* ResultFlag)
{
	IFileVariable* pFile = getCameraSetCfgFileVariable();
	sendResultMode resultMode = sendResultMode::randomClose;
	resultForOneResult result1 = resultForOneResult::OKResult;
	resultForTwoResult result2 = resultForTwoResult::OKRIResult;
	if (pFile != NULL)
	{
		resultMode = (sendResultMode)pFile->getValue(RANDOM_SEND_RESULT_MODE, 0).toInt();
		result1 = (resultForOneResult)pFile->getValue(RANDOM_ONE_RESULT_MODE, 0).toInt();
		result2 = (resultForTwoResult)pFile->getValue(RANDOM_TWO_RESULT_MODE, 0).toInt();
	}
	if (resultMode == sendResultMode::randomClose)
	{
		return false;
	}
	else if (resultMode == sendResultMode::randomTwoBin)
	{
		int resultArr[2] = { res_OK,res_OK3 };
		if (result2 == resultForTwoResult::OKNGResult)
		{
			resultArr[0] = res_OK;
			resultArr[1] = res_Ng;
		}
		else if (resultForTwoResult::RINGResult)
		{
			resultArr[0] = res_OK3;
			resultArr[1] = res_Ng;
		}
		else
		{
			resultArr[0] = res_OK;
			resultArr[1] = res_OK3;
		}
		int result = rand() % 2;
		if (result == 0)
		{
			*ResultFlag = resultArr[0];
		}
		else if (result == 1)
		{
			*ResultFlag = resultArr[1];
		}
	}
	else if (resultMode == sendResultMode::randomThreeBin)
	{
		int result = rand() % 3;
		if (result == 0)
		{
			*ResultFlag = res_Ng;
		}
		else if (result == 1)
		{
			*ResultFlag = res_OK;
		}
		else if (result == 2)
		{
			*ResultFlag = res_OK3;
		}
	}
	else if (resultMode == sendResultMode::randomOneBin)
	{
		if (result1 == resultForOneResult::RIResult)
		{
			*ResultFlag = res_OK3;
		}
		else if (result1 == resultForOneResult::NGResult)
		{
			*ResultFlag = res_Ng;
		}
		else
		{
			*ResultFlag = res_OK;
		}
	}
	return true;
}
void CBackPCtoFront::setSysToFront(int type)
{
	if (!getEnableFrontSig())
	{
		return;
	}
	QString typeStr = "";
	ISocketproxy* pISocketproxy = Utils::getPCSocketproxy();
	if (pISocketproxy != NULL)
	{
		int iRet = 0;
		switch (type)
		{
		case Start:
			iRet = pISocketproxy->sendSysStart(getCurrentDaySn());
			typeStr = QString::fromLocal8Bit("����");
			
			break;
		case Reset:
			iRet = pISocketproxy->sendSysReset();
			typeStr = QString::fromLocal8Bit("��λ");
			SendProductSNToFront();
			break;
		case Stop:
			iRet = pISocketproxy->sendSysStop();
			typeStr = QString::fromLocal8Bit("ֹͣ");
			break;
		case Pause:
			iRet = pISocketproxy->sendSysPause();
			typeStr = QString::fromLocal8Bit("��ͣ");
			break;
		default:
			return;
		}
		QString str =( SYS_ERR_OK == iRet) ? QString::fromLocal8Bit("�ɹ�") : QString::fromLocal8Bit("ʧ��");
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("����%1�źŸ�����%2").arg(typeStr).arg(str), UILOG_WARN);
	}
}
int CBackPCtoFront::SendProductSNToFront()
{
	if (!getEnableFrontSig())
	{
		return 1;
	}
	ISocketproxy* pCSocketproxy = Utils::getPCSocketproxy();
	if (pCSocketproxy == NULL)
	{
		return 2;
	}
	unsigned int proSN = CProductUnion::getInstance()->readCurrentDayPrSn();;
	if (SYS_ERR_OK != pCSocketproxy->sendSynPrdunctSN(proSN))
	{
		CSendMessage::GetInstance()->Send(QString(QString::fromLocal8Bit("ͬ��BackPC��ƷSNʧ��!")), UILOG_WARN);
		return 3;
	}
	return 0;
}