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
    int ShowResData();  // ���Ͽڷ�����Ϣ

    void startSendData();
    void stopSendData();

    void startPenma();
    void stopPenma();
	
	bool randomResult(int * ResultFlag); //����������
	//
	void startClearMode();//��ʼ����
	void stopClearMode();
	bool isClearMode();
	//���͸�λ��ֹͣ������������
	void setSysToFront(int type);
	//
	int SendProductSNToFront();
private:
    CBackPCtoFront();
    ~CBackPCtoFront();

    void threadPenma();                                  // ���ͽ�������ϻ�
    void threadSendData();                               // ���ͽ�������ϻ�
	void threadClearMode();                               // ���ͽ�������ϻ�
private:
    static CBackPCtoFront *m_pInstance;
    bool m_bEnable;
    QMap<unsigned int,int> m_proResult;
    QMutex		m_synResultocker;
    bool m_bRun;
    JThreadT<CBackPCtoFront> *m_pThreadpenma;                 // ���˵��ϼ��
    JThreadT<CBackPCtoFront> *m_pThreadSendData;                 // ���ͽ�������ϻ�
	JThreadT<CBackPCtoFront> *m_pThreadClearMode;                 // 
    bool  m_bPenmaThread;
    bool m_bSendDataThread;
	bool m_bClearModeThread;

	
};

#endif // BACKPC_H
