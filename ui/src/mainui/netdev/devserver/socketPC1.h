#ifndef SOCK_CLIENT_PC1_H
#define SOCK_CLIENT_PC1_H

#include <string>
#include <list>
#include "socketerrcode.h"
#include "packetdef.h"
#include "jthreadt.h"
#include "ringbuffer.h"
#include "socketproxy.h"
#include "maindefine.h"

using namespace std;

#ifndef SOCKET
#define SOCKET int
#endif
#define SOCKET_PACKET_LENGTH 1024

class CSocketClientPC1
{
public:
    CSocketClientPC1(string strID);
    ~CSocketClientPC1();
    bool isConnected();
    void disConneced();
    int sendHeartBeat();
    int getIOStatus(const char* IOName,bool &bOpen);
    int setIO(const char* IOName,bool &bOpen);
    void setSocket(stSMSokcet& stSokcet);
    int sendRequest(string strFun,string strBody ="");
private:

    static int processRecvCtrlPacket(stPacket* packet) ;
private:

    stSMSokcet* m_pstSokcet;
    CSocketproxy* m_pCSocketproxy;
    string m_strID;
};

#endif // SOCK_CLIENT_PC1_H
