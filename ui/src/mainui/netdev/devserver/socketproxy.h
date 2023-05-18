#ifndef CSOCKETPROXY_SERVER_H
#define CSOCKETPROXY_SERVER_H
#include "socketerrcode.h"
#include "packetdef.h"
#include "jmutex.h"
#include <map>
#include <string>
#include <list>
#include "socketcommon.h"
#include "jthreadt.h"
#include "ringbuffer.h"
using namespace std;


typedef int (*FunPacketCB)(stPacket* packet) ;

class CSocketproxy
{
public:
    CSocketproxy(string strID);
    ~CSocketproxy();
    int sendHeartBeat();
    void setProcessPacketCB(FunPacketCB pPacketCB);
    int sendRequestPacket(stPacket* pPack);
    int waitAckPacket(string sn, map<string, string>* pstrBody = NULL, int timeout = 500);

public:
    void setSocket(stSMSokcet& stSokcet);
    stSMSokcet *getSocket();
    int stopProxy();
    int startProxy();
 private:
    int RecvThread(void *param);
    int sendAckPacket(stPacket* pPack);

    void pushBackAckPacket(string sn, stPacket* pPack);
    bool isAckPacketExist(string Sn);
    stPacket* popAckPacket(string Sn);
    string timetoStr();
    int waitRegisterPkg(stSMSokcet* pstSMSokcet);
    int processRecvMsg(char*pCtrlMsg);
    int processRecvCtrlPacket(stPacket* packet);
    int processRecvInquirePacket(stPacket* packet);

    void pushBackProcPacket(stPacket* pPack);
    bool isProcPacketExist(string Sn);

private:
    JMutex m_AckPacketLock;
    JMutex m_snLock;
    JMutex m_ProcPacketLock;
    stSMSokcet m_Socket;
    JThreadTP<CSocketproxy, void *,int >* m_hThreadHandle;
    bool m_bThreadRunning;

    unsigned long long ullsn;

    map<string, stPacket*>m_mapAckPacket;
    list<stPacket*> m_listProcPacket;
    CRingBuffer* m_pCRingBuffer;
    FunPacketCB m_pPacketCB;
    string m_strID;
};

#endif // CSOCKETPROXY_H
