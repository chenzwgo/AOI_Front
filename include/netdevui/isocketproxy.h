#ifndef I_SOCKETPROXY_CLIENT_H
#define I_SOCKETPROXY_CLIENT_H
#include "socketerrcode.h"
#include "packetdef.h"
#include <map>
#include <string>
#include "socketcommon.h"
using namespace std;

class ISocketproxy
{
public:
    enum
    {
        IID_ISOCKETPROXY = 0x88
    };
    ~ISocketproxy(){};
    virtual int sendHeartBeat() = 0;
    virtual int sendRequestPacket(stPacket* pPack) = 0;
    virtual int waitAckPacket(string sn, map<string, string>* pstrBody = NULL, int timeout = 500) = 0;
    virtual int sendInquirePacket(stPacket* pPack) = 0;
    virtual int sendRequest(string strFun,string strBody ="") = 0;
    virtual int sendInquire(string strSN,bool&bProcessed ) = 0;

    virtual int sendSynPrdunctSN(unsigned int isn) = 0;
    virtual int sendProductSNRes(unsigned int isn ,int iOk) = 0;
    virtual int sendSysStart(unsigned int isn) = 0;
    virtual int sendSysStop() = 0;
    virtual int sendSysReset() = 0;
	virtual int sendSysPause() = 0;
	virtual int sendQuerySysStatus(int & iStatus) = 0;
	virtual int sendQueryProductSN(unsigned int& isn) = 0;
	virtual int connectSocket() = 0;
};



#endif // I_SOCKETPROXY_CLIENT_H
