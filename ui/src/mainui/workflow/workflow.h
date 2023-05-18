#ifndef MAINFLOW_H
#define MAINFLOW_H
#include <QVector>
#include "jthreadt.h"
#include "maindefine.h"
#include <QMap>
#include <QString>
#include <QMutexLocker>
#include <QQueue>
#include <QBitArray>
#include "uidefine.h"
#include "jevent.h"
#include "utils.h"
#include <QThreadPool>

#define FIRST_PLATE  0
#define SECOND_PLATE 1

#define SUCKER_PLATE 0
#define SUCKER_FIXTURE 1

typedef enum{ ResetFailed, ResetIng, ResetOK} EnResetState;

class IStation;
class IMainFlow;
class WORK_FLOW;
class IBufferManager;
class WorkFlow
{
    friend class WorkFlowFun;
public:
	static WorkFlow *getInstance();
	static void releaseInstance();
	bool init();
	bool deinit();
    /*\ 外部暴露接口 */
	E_WORK_STATE GetWorkState();                                // 获取主流程状态
	void setStopLine(bool bStop);

	void setSignalFlag(SignalType ,bool );
	void setClearMode(bool flag);
	UiStatusStrut* GetUiStatus();
	void setAlarmFlag(bool flag);
	void setClearFinish(bool isFinish);
private:
    IMainFlow *getFlowList(const string &type, vector<WORK_FLOW*> &flowList);
    bool getStationList();

	//清除轴报警    
	void clearAllAxisAlarm();
	//判断轴是否报警
	bool judgeAxisAlarm(QString &alarmMsg);

	bool isAlarmFlag();
	void stopUpFlowCg();//停止上游玻璃

	static WorkFlow *m_pInstance;      
    JThreadT<WorkFlow> *m_pThreadMain;                  // 主线程
    JThreadT<WorkFlow> *m_pThreadReset;                 // 业务线程
    JThreadT<WorkFlow> *m_pThreadUiStatus;                 //UI 刷新
    JThreadT<WorkFlow> *m_pThreadAlarm;                    // 蜂鸣器
    JThreadT<WorkFlow> *m_pThreadCGblocking;                 // 光纤叠料检测

    bool  m_bRun;                                       // 线程标志
	bool isRun;
	bool  m_bInit;                                      // 初始化标志
	EnResetState m_resetState;                          // 复位状态
    E_WORK_STATE m_workState;                           // 工作状态
    QMutex m_workStateLocker;
private:
    WorkFlow();
    ~WorkFlow();
    void logUi(const QString &msg, UILogLevel type = UILOG_INFO);
    bool checkSystemSignal();
    /*\ 主流程控制 */
    bool Start();                                       // 启动
    bool Pause();                                       // 暂停
    bool Continues();                                   // 继续
    bool Stop();                                        // 停止
    bool Estop();                                       // 急停              
    bool Reset();                                       // 复位

	bool isStopFlag();
	bool customStop();					// 自定义停止事件处理
	bool isXiaStop();
	bool isXiaManLiao();
    /*\ 线程 */
    void threadMainflow();                              // 主流程线程函数
    void threadReset();

    void threadUiStatus();                                  // UI 刷新
    void threadAlarm();                                     // 蜂鸣器报警
    void threadCGblocking();                                // 光纤叠料检测

    /*\ 初始化 */
    void setWorkState(E_WORK_STATE state);              // 工作状态

    IMainFlow               *m_workFlow;        // 工作流程调度接口
    IMainFlow               *m_resetFlow;       // 复位流程调度借口
    vector<WORK_FLOW*>       m_workFlowList;    // 工作流程列表
    vector<WORK_FLOW*>       m_resetFlowList;   // 复位流程列表
    QMap<QString, IStation*> m_mapStation;      // 工站列表
	bool  m_bLineStop; //外部异常停止


	bool m_bClearMode;
	bool m_alarmFlag;
	UiStatusStrut  m_UiStatusStrut;
    bool m_bReset; //复位标志
    bool m_bCGblockingThread;
	IBufferManager *m_pCameraBufferManager[3];
    bool m_bCGBlockInStop;
    bool m_bCGBlockOutStop;
    bool m_bContinuousDvErr;
	bool m_bShangRunning;
	bool m_bIsXiaFull;
};

#endif // MAINFLOW_H
