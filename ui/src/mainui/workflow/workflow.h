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
    /*\ �ⲿ��¶�ӿ� */
	E_WORK_STATE GetWorkState();                                // ��ȡ������״̬
	void setStopLine(bool bStop);

	void setSignalFlag(SignalType ,bool );
	void setClearMode(bool flag);
	UiStatusStrut* GetUiStatus();
	void setAlarmFlag(bool flag);
	void setClearFinish(bool isFinish);
private:
    IMainFlow *getFlowList(const string &type, vector<WORK_FLOW*> &flowList);
    bool getStationList();

	//����ᱨ��    
	void clearAllAxisAlarm();
	//�ж����Ƿ񱨾�
	bool judgeAxisAlarm(QString &alarmMsg);

	bool isAlarmFlag();
	void stopUpFlowCg();//ֹͣ���β���

	static WorkFlow *m_pInstance;      
    JThreadT<WorkFlow> *m_pThreadMain;                  // ���߳�
    JThreadT<WorkFlow> *m_pThreadReset;                 // ҵ���߳�
    JThreadT<WorkFlow> *m_pThreadUiStatus;                 //UI ˢ��
    JThreadT<WorkFlow> *m_pThreadAlarm;                    // ������
    JThreadT<WorkFlow> *m_pThreadCGblocking;                 // ���˵��ϼ��

    bool  m_bRun;                                       // �̱߳�־
	bool isRun;
	bool  m_bInit;                                      // ��ʼ����־
	EnResetState m_resetState;                          // ��λ״̬
    E_WORK_STATE m_workState;                           // ����״̬
    QMutex m_workStateLocker;
private:
    WorkFlow();
    ~WorkFlow();
    void logUi(const QString &msg, UILogLevel type = UILOG_INFO);
    bool checkSystemSignal();
    /*\ �����̿��� */
    bool Start();                                       // ����
    bool Pause();                                       // ��ͣ
    bool Continues();                                   // ����
    bool Stop();                                        // ֹͣ
    bool Estop();                                       // ��ͣ              
    bool Reset();                                       // ��λ

	bool isStopFlag();
	bool customStop();					// �Զ���ֹͣ�¼�����
	bool isXiaStop();
	bool isXiaManLiao();
    /*\ �߳� */
    void threadMainflow();                              // �������̺߳���
    void threadReset();

    void threadUiStatus();                                  // UI ˢ��
    void threadAlarm();                                     // ����������
    void threadCGblocking();                                // ���˵��ϼ��

    /*\ ��ʼ�� */
    void setWorkState(E_WORK_STATE state);              // ����״̬

    IMainFlow               *m_workFlow;        // �������̵��Ƚӿ�
    IMainFlow               *m_resetFlow;       // ��λ���̵��Ƚ��
    vector<WORK_FLOW*>       m_workFlowList;    // ���������б�
    vector<WORK_FLOW*>       m_resetFlowList;   // ��λ�����б�
    QMap<QString, IStation*> m_mapStation;      // ��վ�б�
	bool  m_bLineStop; //�ⲿ�쳣ֹͣ


	bool m_bClearMode;
	bool m_alarmFlag;
	UiStatusStrut  m_UiStatusStrut;
    bool m_bReset; //��λ��־
    bool m_bCGblockingThread;
	IBufferManager *m_pCameraBufferManager[3];
    bool m_bCGBlockInStop;
    bool m_bCGBlockOutStop;
    bool m_bContinuousDvErr;
	bool m_bShangRunning;
	bool m_bIsXiaFull;
};

#endif // MAINFLOW_H
