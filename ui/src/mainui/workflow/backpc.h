#ifndef BACKPC_H
#define BACKPC_H
#include <QMap>
#include <QMutex>
#include "jthread.h"
#include "jthreadt.h"


class CBackPCtoFront
{
public :
    static CBackPCtoFront *getInstance();
    static void releaseInstance();

    bool isSynResultExist(unsigned int iSn);
    int getSynResult(unsigned int iSn);
    void pushBackSynResult(unsigned int iSn,int iRest);
    void removeSynResult(unsigned int iSn);
    void clearSynResult();

    void setRunflag(bool bFlag);
    int ShowResData();  // 出料口发送信息

    void startSendData();
    void stopSendData();

    void startPenma();
    void stopPenma();
	
	bool randomResult(int * ResultFlag); //生成随机结果
	//
	void startClearMode();//开始清料
	void stopClearMode();
	bool isClearMode();
	//发送复位，停止。启动到上游
	void setSysToFront(int type);
	//
	int SendProductSNToFront();
private:
    CBackPCtoFront();
    ~CBackPCtoFront();

    void threadPenma();                                  // 发送结果给下料机
    void threadSendData();                               // 发送结果给下料机
	void threadClearMode();                               // 发送结果给下料机
private:
    static CBackPCtoFront *m_pInstance;
    bool m_bEnable;
    QMap<unsigned int,int> m_proResult;
    QMutex		m_synResultocker;
    bool m_bRun;
    JThreadT<CBackPCtoFront> *m_pThreadpenma;                 // 光纤叠料检测
    JThreadT<CBackPCtoFront> *m_pThreadSendData;                 // 发送结果给下料机
	JThreadT<CBackPCtoFront> *m_pThreadClearMode;                 // 
    bool  m_bPenmaThread;
    bool m_bSendDataThread;
	bool m_bClearModeThread;

	
};

#endif // BACKPC_H
