#include "workflow.h"
#include "QThread"
#include <QDebug>
#include "signalcheck.h"
#include "rtdefine.h"
#include <QStringList>
#include "trace.h"
#include "sendmessage.h"
#include "proxymanager.h"
#include "utils.h"
#include "workflowfun.h"
#include "../homeui/homeview.h"
#include "mainflow/imainflowcfg.h"
#include "mainflow/imainflowmgr.h"
#include "mainflow/imainflow.h"
#include "station/istation.h"
#include "motion/imotion.h"
#include "iodefine.h"
#include "InvokeArithmetic.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"

#include  "lightcontrol_new.h"
#include "stringcommon.h"

#include "ProductOutUnion.h"
#include "netdevui/isocketlistcfg.h"
#include "netdevui/isocketproxy.h"
#include "commonapi.h"
#include "./vscameratools/cameramanager/icamerabuffermanager.h"
#include "g12common.h"
#include "backpc.h"
#include "frontpc.h"
#include "robotcrl.h"
#define FIRST_FIXTURE  0
#define SECOND_FIXTURE 1

#define PROX_FXITURE 1
#define BIND_FXITURE 0

#define G12_ONLINE   1    //用于离线测试

#define JTHREAD_SAFE_DELETE(a)              \
 do                                  \
{                                   \
    if (NULL != (a))                  \
    {                               \
        while((a)->IsRunning()) { Sleep(50);} \
        delete (a);                   \
        (a) = NULL;                   \
    }                               \
}while(0)

static QMutex g_mainFlowMutex;

WorkFlow *WorkFlow::m_pInstance = NULL;
WorkFlow::WorkFlow()
    : m_bRun(false)	
	, m_workState(MAIN_WORK_INIT)
	, m_bInit(false)
	, m_resetState(ResetFailed)
	, m_pThreadReset(NULL)
    , m_pThreadMain(NULL)
	, m_pThreadUiStatus(NULL)
	, m_bClearMode(false)
    , m_pThreadAlarm(NULL)
    , m_pThreadCGblocking(NULL)
    , m_bCGblockingThread(false)
	, m_bShangRunning(false)
	, m_bIsXiaFull(false)
{
	m_pThreadMain = new JThreadT<WorkFlow>;
	m_pThreadReset = new JThreadT<WorkFlow>;
	m_pThreadUiStatus = new JThreadT<WorkFlow>;
    m_pThreadAlarm = new JThreadT<WorkFlow>;
    m_pThreadCGblocking = new JThreadT<WorkFlow>;

    m_workFlow  = NULL;
    m_resetFlow = NULL;
    m_bCGblockingThread = false;
    m_bCGBlockInStop  = false;
    m_bCGBlockOutStop = false;
    m_bContinuousDvErr = false;
    m_bLineStop = false;
    // 注册流程编辑器函数
    WorkFlowFun::getInstance();

    m_bRun = true;
	IBufferManagerFactory* pIBufferManagerFactory = (IBufferManagerFactory*)GetSDKInstance("cameramanager", IBufferManagerFactory::IID_CAMERA_BUFFER_MANAGER_FACTORY);
	if (pIBufferManagerFactory != NULL)
	{
        m_pCameraBufferManager[0] = pIBufferManagerFactory->createBufferManager("0");
        m_pCameraBufferManager[1] = pIBufferManagerFactory->createBufferManager("1");
		m_pCameraBufferManager[2] = pIBufferManagerFactory->createBufferManager("2");
	}
    init();
	if (NULL != m_pThreadMain)
	{
		m_bRun = true;
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("主流程线程开始!"));
		m_pThreadMain->StartThread(this,&WorkFlow::threadMainflow);
    }

	if (NULL != m_pThreadUiStatus)
	{
		m_pThreadUiStatus->StartThread(this, &WorkFlow::threadUiStatus);
	}
	
    SignalCheck::setCleanOn(false);
}

WorkFlow::~WorkFlow()
{
    m_bRun = false;
    m_alarmFlag = false;
    m_bCGblockingThread = false;
	m_bCGBlockOutStop = false;
	Stop();
	//平板启动条件	
	//1.任意一条线 DO_ALLOW_CLEAN_START为 true
	//2.两条线 DO_CLEAN_PAUSE 均为 false
//	if (!getEnableClearCrlSig())
//	{
//		SignalCheck::writeIo(DO_ALLOW_CLEAN_START, true);
//	}
    JTHREAD_SAFE_DELETE(m_pThreadMain);
	JTHREAD_SAFE_DELETE(m_pThreadReset);
	JTHREAD_SAFE_DELETE(m_pThreadUiStatus);
    JTHREAD_SAFE_DELETE(m_pThreadAlarm);
    JTHREAD_SAFE_DELETE(m_pThreadCGblocking);
	WorkFlowFun::releaseInstance();
}

bool WorkFlow::checkSystemSignal()
{
	if (SignalCheck::IsEstopSig())  // 急停
	{
		if (MAIN_WORK_ESTOP != m_workState)
		{
			logUi(QString::fromLocal8Bit("急停触发!"), UILOG_ERR);
			Estop();
			SignalCheck::SrvOffCard();

			setWorkState(MAIN_WORK_ESTOP);
		}
		return true;
	}
	else
	{
		if (MAIN_WORK_ESTOP == m_workState)
		{
			logUi(QString::fromLocal8Bit("急停消失!"), UILOG_WARN);
			clearAllAxisAlarm();
			m_resetState = ResetFailed;
			SignalCheck::SrvOnCard();
			setWorkState(MAIN_WORK_WAIT_RESET);
		}
	}

	if (SignalCheck::IsPowerOffSig()) // 断电
	{
		if (MAIN_WORK_POWEROFF != m_workState)
		{
			logUi(QString::fromLocal8Bit("设备断电!"), UILOG_ERR);
			Estop();
			setWorkState(MAIN_WORK_POWEROFF);
		}
		return true;
	}
	else
	{
		if (MAIN_WORK_POWEROFF == m_workState)
		{
			logUi(QString::fromLocal8Bit("设备上电!"), UILOG_WARN);
			Estop();
			clearAllAxisAlarm();
			m_resetState = ResetFailed;
			setWorkState(MAIN_WORK_WAIT_RESET);
		}
	}

	QString alarmMsg;
	if (judgeAxisAlarm(alarmMsg)) // 轴报警
	{
		if (MAIN_WORK_AXISALARM != m_workState)
		{
			logUi(alarmMsg, UILOG_ERR);
			Stop();
			setWorkState(MAIN_WORK_AXISALARM);
		}
		return true;
	}
	else
	{
		if (MAIN_WORK_AXISALARM == m_workState)
		{
			logUi(QString::fromLocal8Bit("轴报警已消除!"), UILOG_WARN);
			m_resetState = ResetFailed;
			setWorkState(MAIN_WORK_WAIT_RESET);
		}
	}
	return false;
}

UiStatusStrut* WorkFlow::GetUiStatus()
{
	return &m_UiStatusStrut;
}

void  WorkFlow::threadUiStatus()
{
	iMotion* pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
	bool bMark = false;
	bool bCheckOtherAoi = false;

	while (m_bRun)
	{
		//card
		if (NULL != pMotion)
		{
			ICard* pCard = (ICard *)pMotion->GetCardInstance(CONTROLCARD_NAME);
			if (NULL != pCard)
			{
				m_UiStatusStrut.bCradOnline = pCard->IsConnected();
			}
			else 
			{
				m_UiStatusStrut.bCradOnline = false;
			}
		}
		//aoi
		ISocketproxy* pISocketproxy = Utils::getPCSocketproxy();
		if (pISocketproxy == NULL)
		{
			m_UiStatusStrut.bAoiIsConnected = false;
		}
		else
		{
			if (getEnableFrontSig())
			{
		     	if(!m_UiStatusStrut.bAoiIsConnected)
			     {
				    int iRet = pISocketproxy->connectSocket();
			     	if (iRet != SYS_ERR_OK )
				   {
					  logUi(QString::fromLocal8Bit("重连前端AOI连接失败(%1)!").arg(iRet), UILOG_WARN);
			     	}
			    }
				m_UiStatusStrut.bAoiIsConnected = (SYS_ERR_OK == pISocketproxy->sendHeartBeat()) ? true : false;
			}
			/*
			if (m_UiStatusStrut.bAoiIsConnected)//联接上对线AOI
			{
				bMark = true;
			}
			if (bMark&&(!m_UiStatusStrut.bAoiIsConnected)&&( getEnableFrontSig())) //联线失败了
			{
				logUi(QString::fromLocal8Bit("检测对线AOI不在线，停止上料机清洗机！报警"), UILOG_POPRED);

				SignalCheck::writeIo(DO_OTHER_AOI_STOP, true);//使能对线AOI轴断电
				SignalCheck::writeIo(DO_RED_LIGHT, true);
				
				SignalCheck::setCleanPause(true);//同时停止清洗机//上料机
				SignalCheck::setShangStop();  //
				Sleep(300);
				SignalCheck::writeIo(DO_OTHER_AOI_STOP, false);//使能对线AOI轴上电
				SignalCheck::setShangStop(false);  //使能上料机能手机起动
				bMark = false;
			}*/
		}


		// 下料机，机器手
        m_UiStatusStrut.bShangOK = SignalCheck::readIo(DI_SHANG_RUN) ;
        m_UiStatusStrut.bXiaOK = SignalCheck::readIo(DI_XIA_RUN);
        m_UiStatusStrut.bCleanMoving  =  SignalCheck::readIo(DI_CLEAN_RUN);
        // 清料模式
        m_UiStatusStrut.bClearMode = SignalCheck::readIo(DI_CLEAR_MODE);
		//相机
		ILineCamera::X_LINE_CAM_STATUS  status = ILineCamera::STATUS_UNINIT;
		for (int i = 0; i < 3; i++)
		{

			if (i == 0)
			{
				if (NULL != m_pCameraBufferManager[i]) {
					status = (ILineCamera::X_LINE_CAM_STATUS)m_pCameraBufferManager[i]->getCamStatus(0);
					m_UiStatusStrut.bCameraOnline[0] = ILineCamera::STATUS_CAM_IDLE <= status;
				}
			}
			else if (i == 1)
			{
				if (NULL != m_pCameraBufferManager[i]) {
					status = (ILineCamera::X_LINE_CAM_STATUS)m_pCameraBufferManager[i]->getCamStatus(0);
					m_UiStatusStrut.bCameraOnline[1] = ILineCamera::STATUS_CAM_IDLE <= status;
					status = (ILineCamera::X_LINE_CAM_STATUS)m_pCameraBufferManager[i]->getCamStatus(1);
					m_UiStatusStrut.bCameraOnline[2] = ILineCamera::STATUS_CAM_IDLE <= status;
				}
			}
			else if (i == 2)
			{
				if (NULL != m_pCameraBufferManager[i]) {
					status = (ILineCamera::X_LINE_CAM_STATUS)m_pCameraBufferManager[i]->getCamStatus(0);
					m_UiStatusStrut.bCameraOnline[3] = ILineCamera::STATUS_CAM_IDLE <= status;
				}
			}
		}
		Sleep(1000);//改为一秒一次检测//需要检测对线AOI是否崩溃
	}
}

void WorkFlow::threadMainflow()
{
	m_workState = MAIN_WORK_INIT;
    QDateTime runTimeS;
    bool bNeedColse = false;
    QDateTime runTimeSwitch;
    bool bNeedColseSwitch = false;
	bool bClearMode = false;
	bool bCheckInGlass = false;//入料口叠料
	
	bool isxiaFull = true;    //满载

    while(m_bRun)
	{
        //检查电磁锁--开锁5秒后必须关闭， 否则会烧坏锁
        if (!bNeedColse && SignalCheck::readIo(DO_ELEC_LOCK))
        {
            runTimeS = QDateTime::currentDateTime();
            bNeedColse = true;
        }

        if (bNeedColse)
        {
            if (runTimeS.secsTo(QDateTime::currentDateTime()) > 10)
            {
                SignalCheck::writeIo(DO_ELEC_LOCK, false);
                bNeedColse = false;
            }
        }

		// <急停/断电/安全门/安全光栅/Robot断线> 触发时，信号常亮，不响应其他任何操作
		if (checkSystemSignal())
		{
			Sleep(50);
			continue;
		}

		// 正式工作流程
		switch (m_workState)
		{
		case MAIN_WORK_INIT:      // 初始状态
			{
				setWorkState(MAIN_WORK_WAIT_RESET);
			}
			break;
		case MAIN_WORK_ERROR:
		case MAIN_WORK_WAIT_RESET:      // 等待复位
		case MAIN_WORK_RESET_FAILED:
			{
				if (SignalCheck::IsResetSig())
				{
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("开始复位."));
                    m_resetState = ResetIng;
					bClearMode   = false;
                    setWorkState(MAIN_WORK_RESETING);
					Reset();	
				}				
			}
			break;
		case MAIN_WORK_RESETING:    // 复位中
			{
				if (ResetOK == m_resetState)
				{
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("复位成功."));
					setWorkState(MAIN_WORK_READY);

                    m_bCGBlockInStop = false;
                    m_bCGBlockOutStop = false;
                    m_bContinuousDvErr = false;
                    m_bLineStop = false;
                    m_alarmFlag = false;
					bCheckInGlass = false;
					

				}
				else if(ResetFailed == m_resetState)
				{
					
                    logUi(QString::fromLocal8Bit("复位失败!"), UILOG_ERR);
					setWorkState(MAIN_WORK_RESET_FAILED);
				}
			}
			break;
		case MAIN_WORK_READY:      // 准备好（等待启动）
			{	
				if (SignalCheck::IsResetSig())
				{
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("开始复位."));
                    m_resetState = ResetIng;
                    setWorkState(MAIN_WORK_RESETING);
					
					Reset();					
					break;
				}	

				if (SignalCheck::IsStartSig())
				{
                     if (isXiaStop()|| isXiaManLiao())
					 {
						 logUi(QString::fromLocal8Bit("--下料机-- 未启动运行!"), UILOG_POPRED);
						 break;
					 }
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("工作开始."));
					setWorkState(MAIN_WORK_WORK);
					Start();
					
				}
			}
			break;
		case MAIN_WORK_WORK:       // 工作中
			{	
				/*if (SignalCheck::readIo(DI_FULL_SIG))
				{
					isxiaFull = false;
					logUi(QString::fromLocal8Bit("停止!"), UILOG_WARN);
					setWorkState(MAIN_WORK_STOP);
					Stop();
				}*/
				if (SignalCheck::IsPauseSig())
				{
                    logUi(QString::fromLocal8Bit("暂停!"), UILOG_WARN);
                    setWorkState(MAIN_WORK_PAUSE);
					Pause();
					Stop();
				}
				else if(isStopFlag())
				{
					//aio停止
					setWorkState(MAIN_WORK_STOP);
					Pause();
					Stop();
					Sleep(50);
					//if (SignalCheck::readIo(DI_FULL_SIG))//防止满载信号比停止信号后给
					//{
					//	isxiaFull = false;
					//}
				}
				else if (SignalCheck::IsStopSig())
				{
                    logUi(QString::fromLocal8Bit("停止!"), UILOG_WARN);
                    setWorkState(MAIN_WORK_STOP);
					Stop();
				}
                else if (SignalCheck::IsResetSig())
                {
                   logUi(QString::fromLocal8Bit("工作中不响应复位!!"), UILOG_WARN);
                }
				else if (isXiaStop()|| isXiaManLiao())
				{
					logUi(QString::fromLocal8Bit("暂停!"), UILOG_WARN);
					setWorkState(MAIN_WORK_PAUSE);
					Pause();
				}
			}
			break;
		case MAIN_WORK_PAUSE:      // 暂停
			{
				if (SignalCheck::IsResetSig())
				{
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("开始复位."));
                    m_resetState = ResetIng;
                    setWorkState(MAIN_WORK_RESETING);
					Reset();					
				}
				else if (SignalCheck::IsStartSig()|| (!isXiaStop() && !isXiaManLiao()))
				{//
					if (isXiaStop() || isXiaManLiao())
					{
						logUi(QString::fromLocal8Bit("--下料机-- 未启动运行!"), UILOG_POPRED);
						break;
					}
					setWorkState(MAIN_WORK_WORK);

					WorkFlowFun::getInstance()->Crlmove(1);

					CBackPCtoFront::getInstance()->setSysToFront(SignalType::Start);
					if (NULL != m_pThreadCGblocking)
					{
						if (!m_pThreadCGblocking->IsRunning())
						{
							m_bCGBlockOutStop = true;
							m_pThreadCGblocking->StartThread(this, &WorkFlow::threadCGblocking);
						}
					}
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("暂停后启动."));
				}
				else if (SignalCheck::IsStopSig())
				{
                    logUi(QString::fromLocal8Bit("停止!"), UILOG_WARN);
                    setWorkState(MAIN_WORK_STOP);
					Stop();
				}
			}
			break;
		case MAIN_WORK_STOP:          // 停止
			{
				
				if (SignalCheck::IsResetSig())
				{
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("开始复位."));
                    m_resetState = ResetIng;
                    setWorkState(MAIN_WORK_RESETING);
					Reset();
				}
				//else if (SignalCheck::isXiaRunning1()&& !isxiaFull) //下料机有满载信号先清料在启动
				//{
				//	CBackPCtoFront::getInstance()->startClearMode();
				//	if (m_bIsXiaFull)
				//	{
				//		isxiaFull = true;
				//		m_bIsXiaFull = false;
				//		if (SignalCheck::isXiaRunning1())
				//		{
				//			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("开始复位."));
				//			m_resetState = ResetIng;
				//			setWorkState(MAIN_WORK_RESETING);
				//			Reset();
				//		}
				//	}
				//}
				else if (SignalCheck::IsStartSig())
				{
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("启动失败，请先复位！"),UILOG_WARN);
				}
			}
		
		default:
			break;
		}
       
        Sleep(50);
    }
	//如果电磁锁被打开， 则关闭电磁锁
	if (SignalCheck::readIo(DO_ELEC_LOCK))
	{
		SignalCheck::writeIo(DO_ELEC_LOCK, false);
	}
}

void WorkFlow::threadReset()
{
    //m_resetState = ResetOK;
    QVector<int> finishedFlow;   //回原结束的流程列表
    int succeedCount = 0;
    m_bRun = true;
    EnResetState resetState = ResetFailed;                          // 复位状态
#if G12_ONLINE
    // 等待回原完成
    while (m_bRun)
    {
        Sleep(50);

        // 依次判断流程状态
        for (int i=0; i<m_resetFlowList.size(); i++)
        {
            if (finishedFlow.contains(i))
            {
                continue;
            }

            E_FLOW_STATUS status = (E_FLOW_STATUS)m_resetFlow->runStatus(m_resetFlowList.at(i)->name);
            switch(status)
            {
            case FLOW_STATUS_RUNING:
            case FLOW_STATUS_PAUSE:
                break;
            case FLOW_STATUS_OK:
                {
                    succeedCount++;
                    finishedFlow.push_back(i);
                }
                break;
            case FLOW_STATUS_FAILED:
            case FLOW_STATUS_ERROR:
                {
                    finishedFlow.push_back(i);
                }
                break;
            case FLOW_STATUS_STOP:
                {
                    finishedFlow.push_back(i);
                }
                break;
            default:
                break;
            }

            if (status == FLOW_STATUS_ERROR || status == FLOW_STATUS_FAILED)
            {
                Stop();
                break;
            }
        }

        // 判断回原是否完成
        if (finishedFlow.size() == m_resetFlowList.size())
        {
            // 判断回原结果
            if (succeedCount == m_resetFlowList.size())
            {
                resetState = ResetOK;           //回原OK
            }

            break;
        }
    }
#else
    logUi(QString::fromLocal8Bit(" 离线测试!"), UILOG_WARN);
    resetState = ResetOK;           //回原OK
#endif
	
    CInvokeArithmetic::getInstance()->resetProcess();
    m_resetState = resetState;
	return;
}

bool WorkFlow::init()
{
	m_bInit = true;
    // 获取工站列表
    getStationList();
    m_workFlow  = getFlowList("work", m_workFlowList);
    m_resetFlow = getFlowList("reset", m_resetFlowList);
	return true;
}

bool WorkFlow::deinit()
{
	return true;
}

bool WorkFlow::Start()
{
	LOG_FUNTION;
    //流程启动
    RETURN_V_IF(NULL == m_workFlow, false);
    WorkFlowFun::getInstance()->Start(); //设置回调允许允许状态
	/*
    IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
    int iModel = 0;
    const char *szKey = NULL;
    if (NULL != pFileVariable)
    {
        iModel = pFileVariable->getValue(PRODUCT_LINE_RUN_MODEL, 0).toInt();
        string strNameList = "";
        if (iModel == 0)
        {
            strNameList = pFileVariable->getValue(PRODUCT_FLOW_NAMES_1, "").toCString();
        }
        else if (iModel == 1)
        {
            //szKey = PRODUCT_FLOW_NAMES_2;
            strNameList = pFileVariable->getValue(PRODUCT_FLOW_NAMES_2, "").toCString();
        }
        else
        {
            strNameList = pFileVariable->getValue(PRODUCT_FLOW_NAMES_3, "").toCString();
        }

        //string strNameList = pFileVariable->getValue(szKey, "").toCString();
        vector<string> vecName = SString::split(strNameList, ",");
        for (int i = 0; i < vecName.size(); i++)
        {
            string flowName = vecName.at(i);
            ERROR_CODE nRet = m_workFlow->runFlow(flowName, "", "", true);
            if (SYS_ERR_OK != nRet)
            {
                Stop();
                logUi(QString::fromLocal8Bit(" 流程启动失败!"), UILOG_ERR);
                setWorkState(MAIN_WORK_STOP);
                return false;
            }
        }
    }*/
	CBackPCtoFront::getInstance()->setRunflag(true);
    CBackPCtoFront::getInstance()->startPenma();
    CBackPCtoFront::getInstance()->startSendData();
	SignalCheck::setShangStop(false);//停机后启动需要关闭停止
    if (NULL != m_pThreadCGblocking)
    {
        if (!m_pThreadCGblocking->IsRunning())
        {
            m_bCGBlockOutStop = true;
            m_pThreadCGblocking->StartThread(this, &WorkFlow::threadCGblocking);
        }
    }
	//int iStatus = MAIN_WORK_WORK;
	//int iRet = WorkFlowFun::getInstance()->queryBackPcStatus(iStatus);
	//if (iRet == 0)	//另一台机处于工作状态才允许启动上料机
	//{
	//	if (iStatus == MAIN_WORK_WORK)
	//	{
	//		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("另一条线已启动，启动上料机"), UILOG_WARN);
	//		SignalCheck::setShangOn();
	//	}
	//	else
	//	{
	//		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("另一条线启动才能启动上料机"), UILOG_ERR);
	//	}
	//}
	WorkFlowFun::getInstance()->Crlmove(1);
	CBackPCtoFront::getInstance()->setSysToFront(SignalType::Start);
	
	return true;
}

bool WorkFlow::Pause()
{
	LOG_FUNTION;
	WorkFlowFun::getInstance()->Crlmove(0);
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("暂停流水线，并通知上游暂停！"), UILOG_WARN);
	CBackPCtoFront::getInstance()->setSysToFront(SignalType::Pause);
	m_bCGblockingThread = false;
    return true;
}

bool WorkFlow::Continues()
{
	LOG_FUNTION;
    RETURN_V_IF(NULL == m_workFlow,false);
    for (int i=0; i<m_workFlowList.size(); i++)
    {
        if(SYS_ERR_OK != m_workFlow->continueFlow(m_workFlowList.at(i)->name))
        {
            return false;
        }
    }
    return true;
}

bool WorkFlow::Stop()
{
	LOG_FUNTION;
    //SignalCheck::setCleanPause(true);
	//Sleep(200);
	//SignalCheck::setShangStop();
    //流程停止
	WorkFlowFun::getInstance()->Stop();
	CBackPCtoFront::getInstance()->setSysToFront(SignalType::Stop);
    RETURN_V_IF(NULL == m_workFlow,false);
    for (int i=0; i<m_workFlowList.size(); i++)
    {
        if(SYS_ERR_OK != m_workFlow->stopFlow(m_workFlowList.at(i)->name))
        {
        }
    }

    RETURN_V_IF(NULL == m_resetFlow,false);
    for (int i=0; i<m_resetFlowList.size(); i++)
    {
        if(SYS_ERR_OK != m_resetFlow->stopFlow(m_resetFlowList.at(i)->name))
        {
        }
    }

    // 工站停止
    QMap<QString, IStation*>::Iterator iter = m_mapStation.begin();
    for (iter; iter != m_mapStation.end(); iter++)
    {
        if (NULL != *iter)
        {
            (*iter)->Stop(STOP_SUDDEN);
        }
    }

    m_bCGblockingThread = false;
	customStop();
    CBackPCtoFront::getInstance()->stopSendData();
    
    Utils::CheckProductIsOnIO();
	CBackPCtoFront::getInstance()->setRunflag(false);
	return true;
}

bool WorkFlow::Estop()
{
	LOG_FUNTION;
    RETURN_V_IF(NULL == m_workFlow,false);
    for (int i=0; i<m_workFlowList.size(); i++)
    {
        if(SYS_ERR_OK != m_workFlow->stopFlow(m_workFlowList.at(i)->name))
        {
        }
    }

    RETURN_V_IF(NULL == m_resetFlow,false);
    for (int i=0; i<m_resetFlowList.size(); i++)
    {
        if(SYS_ERR_OK != m_resetFlow->stopFlow(m_resetFlowList.at(i)->name))
        {

        }
    }

    // 工站停止
    QMap<QString, IStation*>::Iterator iter = m_mapStation.begin();
    for (iter; iter != m_mapStation.end(); iter++)
    {
        if (NULL != *iter)
        {
            (*iter)->Stop(STOP_SUDDEN);
        }
    }
    if (isEnableClean())
    {
        SignalCheck::setCleanOn(false);
        SignalCheck::setCleanPause(true);
    }
	SignalCheck::setShangStop();
	CBackPCtoFront::getInstance()->setSysToFront(SignalType::Stop);
	return true;
}

bool WorkFlow::Reset()
{
	SENDMSG_INSTANCE->sendInitOtherUi();//add20210505
    if (Utils::CheckProductIsOnIO() )	//光纤上有产品不允许复位
    {
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("光纤上有产品，请先拿走后再复位启动！"), UILOG_POP);
        setWorkState(MAIN_WORK_RESET_FAILED);
        return false;
    }
	LOG_FUNTION;
    RETURN_V_IF(NULL == m_pThreadReset, false);
    RETURN_V_IF(NULL == m_resetFlow, false);
	SignalCheck::setXiaStop();
	CBackPCtoFront::getInstance()->setSysToFront(SignalType::Reset);
    // 1、设备停止
	CBackPCtoFront::getInstance()->stopPenma();
	CBackPCtoFront::getInstance()->stopClearMode();
    Stop();
    // 2、事件重置
    // 4、清除轴状态
    //clearAllAxisAlarm();
    // 5、有轴报警，复位失败
    // 6、清除复位流程状态
    for (int i=0; i<m_resetFlowList.size(); i++)
    {
        m_resetFlow->clearStatus(m_resetFlowList.at(i)->name);
    }
	CFrontPCtoBack::getInstance()->ClearFrontMsg();
	unsigned int iProductSN = CProductUnion::getInstance()->readCurrentDayPrSn();;
	//RobotCrl::GetInstance()->send_sn_to_xialiaoji(iProductSN);
	SENDMSG_INSTANCE->sendUpdateUiStus("0", update_reset);
    // 7、启动复位流程
    WorkFlowFun::getInstance()->Reset(); //复位回调状态
#if G12_ONLINE
    for (int i=0; i<m_resetFlowList.size(); i++)
    {
        if(SYS_ERR_OK != m_resetFlow->runFlow(m_resetFlowList.at(i)->name))
        { 
            Stop();
            Sleep(100);
            m_resetState = ResetFailed;    //回原失败
            return false;
        }
    }
#endif
    // 8、开线程等待回原结果
    logUi(QString::fromLocal8Bit("等待回原结束!"), UILOG_WARN);
    if (!m_pThreadReset->IsRunning())
    {
        m_pThreadReset->StartThread(this, &WorkFlow::threadReset);
        return true;
    }
	return false;
}

E_WORK_STATE WorkFlow::GetWorkState()
{
	return m_workState;
}


void WorkFlow::setWorkState( E_WORK_STATE state )
{
	QMutexLocker locker(&m_workStateLocker);
	m_workState = state;
    HomeView::GetInstance()->updateSysStateBtn((int)m_workState );
}

WorkFlow * WorkFlow::getInstance()
{
    QMutexLocker locker(&g_mainFlowMutex);
	if (NULL == m_pInstance)
	{
		m_pInstance = new WorkFlow;
	}

	return m_pInstance;
}

void WorkFlow::releaseInstance()
{
    QMutexLocker locker(&g_mainFlowMutex);
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
    }
}

void WorkFlow::logUi( const QString &msg, UILogLevel  type )
{
	CSendMessage::GetInstance()->Send(msg,type);
}
IMainFlow *WorkFlow::getFlowList(const string &type, vector<WORK_FLOW*> &flowList)
{
    IMainflowMgr *pMainFlowMgr = (IMainflowMgr *)GetSDKInstance("mainflow", IMainflowMgr::IDD_MAIN_FLOW_MGR);
    IMainFlow *pMainFlow = pMainFlowMgr->findMainflow(type.data());
    RETURN_V_IF(NULL == pMainFlow, NULL);
    // 流程配置接口
    iMainFlowCfg *pMainFlowCfg = pMainFlow->GetMainFlowCfg();
    RETURN_V_IF(NULL == pMainFlowCfg, NULL);

    for (int i=0; i<pMainFlowCfg->GetWorkCount(); i++)
    {
        WORK_FLOW *pFlow = pMainFlowCfg->GetWorkCfg(i);
        if (NULL != pFlow)
        {
            flowList.push_back(pFlow);
        }
    }

    return pMainFlow;
}
bool WorkFlow::getStationList()
{
    iMotion *pMotion =  (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
    RETURN_V_IF(NULL == pMotion, false);

    iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion",iMotionCfg::IID_MOTION_CONFIG);
    RETURN_V_IF(NULL == pMotionCfg, false);

    vector<string> stationNames;
    pMotionCfg->GetAllStationName(stationNames);
    for (int i=0; i<stationNames.size(); i++)
    {
        IStation *pStation = pMotion->GetStationInstance(stationNames.at(i).c_str());
        if (NULL != pStation)
        {
            m_mapStation[QString::fromStdString(stationNames.at(i))] = pStation;
        }
    }

    return true;
}
void WorkFlow::clearAllAxisAlarm()
{
	// 工站设置速度比例
	QMap<QString, IStation*>::Iterator iter = m_mapStation.begin();
	for (iter; iter != m_mapStation.end(); iter++)
	{
		if (NULL != *iter)
		{
			for (int i = 0; i < 6; i++)
			{
				(*iter)->ClearAxisStatus(i);
			}
		}
	}
}

bool WorkFlow::judgeAxisAlarm(QString &alarmMsg)
{
	QMap<QString, IStation*>::Iterator iter = m_mapStation.begin();
	for (iter; iter != m_mapStation.end(); iter++)
	{
		if (NULL != *iter)
		{
			StStationInfo stationInfo = (*iter)->GetStationInfo();
			bool bAlarm = false;
			for (int i = 0; i < stationInfo.axiscnt; i++)
			{
				if (SYS_ERR_OK == (*iter)->IsAxisDrvAlarm(i, bAlarm))
				{
					StAxisParam axisParam = (*iter)->GetAxisInfo(i);
					if (bAlarm)
					{
						alarmMsg = QString::fromLocal8Bit("轴报警：") + QString::fromUtf8(axisParam.AxisDesc);
						return true;
					}
				}
			}
		}
	}

	return false;
}
bool WorkFlow::customStop()
{
	//流水线停止
    //SignalCheck::writeIo(DO_CLEAN_PAUSE, true);
    //SignalCheck::writeIo(DO_SHANG_STOP, true);
    return true;
}

void WorkFlow::setStopLine(bool bStop)
{
	m_bLineStop = bStop;
}

bool WorkFlow::isXiaStop()
{
	// 下料机故障
	if (isEnableXia())
	{
		// 下料机故障
		if (!SignalCheck::readIo(DI_XIA_RUN))
		{
			logUi(QString::fromLocal8Bit("检测到下料机停止!"), UILOG_WARN);
			return true;
		}
	}
	return false;
}
bool WorkFlow::isXiaManLiao()
{
	// 下料机故障
	if (isEnableXia())
	{
		// 下料机故障
		if (SignalCheck::readIo(DI_XIA_MANLIAO))
		{
			logUi(QString::fromLocal8Bit("检测到下料机满料!"), UILOG_WARN);
			return true;
		}
	}
	return false;
}
bool WorkFlow::isStopFlag()
{
	return m_bLineStop;
}

void WorkFlow::setAlarmFlag(bool flag)
{
	if (flag)
	{
		if (m_bReset)	//复位后才能重新启动蜂鸣器报警线程
		{
			m_alarmFlag = true;
			if (m_pThreadAlarm != NULL)
			{
				if (!m_pThreadAlarm->IsRunning())
				{
					m_pThreadAlarm->StartThread(this, &WorkFlow::threadAlarm);
				}
			}
			m_bReset = false;
		}
	}
	else
	{
		m_alarmFlag = false;
	}
}

void WorkFlow::setClearFinish(bool isFinish)
{
	m_bIsXiaFull = isFinish;
}

void WorkFlow::threadAlarm()
{
	while (m_alarmFlag)
	{
		SignalCheck::writeIo(DO_BEE_ALARM, true);
		Sleep(1000);
		SignalCheck::writeIo(DO_BEE_ALARM, false);
		Sleep(1000);
    }
}


void WorkFlow::threadCGblocking()
{

    int itimeout = 3000; //叠料时间
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if (pFile)
    {
       itimeout = pFile->getValue(IMAGE_PARAM_CGBLOCK_TIMEOUT,3000).toInt();//
    }

    //const int length = 5;
    //const char* checkIOName[length] = {DI_SNAP_SIG_1,DI_SNAP_SIG_2,DI_SNAP_SIG_3,DI_OUT_END_SIG,DI_IN_SIG};
    //QString strDes[length] = {QString::fromLocal8Bit("拍照1光纤上叠料！"),
    //                    QString::fromLocal8Bit("拍照2光纤上叠料！"),
    //                   QString::fromLocal8Bit("拍照3光纤上叠料！"),
    //                   QString::fromLocal8Bit("出料口光纤上叠料！"),
    //                   QString::fromLocal8Bit("入料口光纤上叠料！")
    //                         };
	/*const int length = 1;
	const char* checkIOName[length] = {DI_IN_SIG };
    uint64 iStartTime[length];
    uint64 iEndTime[length];
    bool bStop[length] =  {false};
    for(int i=0 ;i <length ;i++)
    {
        iStartTime[i] = JTime::CurrentMillisecond();
    }

    while (m_bCGBlockOutStop)
    {
        int i = 0;
        bool bStop = false;
        for(;i < length;i++)
        {
            const char *szIOName = checkIOName[i];
             iStartTime[i] = JTime::CurrentMillisecond();
             iEndTime[i] = JTime::CurrentMillisecond();

            if (SignalCheck::readIo(szIOName))
            {
                iEndTime[i] = JTime::CurrentMillisecond();
            }
            else
            {
                iStartTime[i] = JTime::CurrentMillisecond();
            }
            int iEsTime= iEndTime[i]- iStartTime[i];
            if( iEsTime > itimeout)
            {
                bStop = true;
                break;
            }
        }
        if(bStop)
        {
             //CSendMessage::GetInstance()->Send(strDes[i], UILOG_POPRED);
			 m_bCGBlockInStop = true;
        }

        Sleep(100);

    }*/

	uint64 iStartTime= JTime::CurrentMillisecond();
	uint64 iEndTime= JTime::CurrentMillisecond();
	while (m_bCGBlockOutStop)
	{
		if (MAIN_WORK_WORK==m_workState&&SignalCheck::readIo(DI_IN_SIG))
		{
				iEndTime = JTime::CurrentMillisecond();
				m_bCGBlockInStop = (iEndTime - iStartTime) > itimeout ? true : false;
		}
		else
		{
			iStartTime = JTime::CurrentMillisecond();
			iEndTime = JTime::CurrentMillisecond();
			m_bCGBlockInStop = false;
		}

		Sleep(100);
	}
}
void WorkFlow::stopUpFlowCg()
{
	SignalCheck::setCleanPause(true);//先停清洗机--预防同时产品在清洗机交接处撞头
	int imax = 40;//最大等待2000ms
	do
	{
		Sleep(50);
		imax--;
	} while ((imax>0)&&m_bRun&&SignalCheck::readIo(DI_CLEAN_RUN));

	SignalCheck::setShangStop();
}
