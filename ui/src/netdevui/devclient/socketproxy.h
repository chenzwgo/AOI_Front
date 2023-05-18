#ifndef CSOCKETPROXY_CLIENT_H
#define CSOCKETPROXY_CLIENT_H
#include "socketerrcode.h"
#include "packetdef.h"
#include "jmutex.h"
#include <map>
#include <string>
#include "socketcommon.h"
#include "jthreadt.h"
#include "ringbuffer.h"
#include "isocketproxy.h"
using namespace std;

class IReportCenter;
class CSocketClient;
class CSocketproxy :public ISocketproxy
{
public:
    CSocketproxy( string strName);
    ~CSocketproxy();
    int sendHeartBeat();
    int sendRequestPacket(stPacket* pPack);
    int waitAckPacket(string sn, map<string, string>* pstrBody = NULL, int timeout = 500);
    int sendInquirePacket(stPacket* pPack);
    int sendRequest(string strFun,string strBody ="");
    int sendInquire(string strSN,bool&bProcessed );

    int sendSynPrdunctSN(unsigned int isn);
    int sendProductSNRes(unsigned int isn ,int iOk);
    int sendSysStart(unsigned int isn);
    int sendSysStop();
    int sendSysReset();
	int sendSysPause();
	int sendQuerySysStatus(int & iStatus);
	int sendQueryProductSN(unsigned int& isn);
	int connectSocket();
public:
    int stopProxy();
    int startProxy();
    static int processRecvMsg(void * param,char*pCtrlMsg);

    CSocketClient* getSocketClient();
 private:
    int RecvThread(void *param);
    int sendAckPacket(stPacket* pPack);

    void pushBackAckPacket(string sn, stPacket* pPack);
    bool isAckPacketExist(string Sn);
    stPacket* popAckPacket(string Sn);
    string timetoStr();
    int waitRegisterPkg(stSMSokcet* pstSMSokcet);
    int processRecvCtrlPacket(stPacket* packet);

private:
    JMutex m_AckPacketLock;
    JMutex m_snLock;
    unsigned long long ullsn;
    map<string, stPacket*>m_mapAckPacket;
    CSocketClient* m_CSocketClient;
    string m_strName;
    IReportCenter *m_pReportCenter;

};

#endif // CSOCKETPROXY_H
