#ifndef SOCK_CLIENT_H
#define SOCK_CLIENT_H

#include <string>
#include <list>
#include "socketerrcode.h"
#include "packetdef.h"
#include "jthreadt.h"
#include "ringbuffer.h"
#include "socketproxy.h"

using namespace std;

#ifndef SOCKET
#define SOCKET int
#endif
#define SOCKET_PACKET_LENGTH 1024

typedef int (*FunPacketCB)(void * param,char* packet) ;
class CSocketClient
{
public:
    CSocketClient();
    ~CSocketClient();
    int initClient(const char* ClinetIP, unsigned int Clinetport);
    int connectServer();
    int disConnect();
    bool isConnected();
    void setServerCfg(const char* ServerIP, unsigned int Serverport);
    void setProcessPacketCB(void * param,FunPacketCB pPacketCB);

    int recvMsg(char* buffer,unsigned int bufferLen,int &recvLength ,int timeout= 500 );

    int sendMsg(char *buffer, unsigned int bufferLen, int &sendLength, int timeout = 500);


private:
    int createSocket();
    int connectSocket();
    int RecvThread(void *param);
private:
    string m_strClinetIPAddr;
    unsigned int m_iClinetPort;
    string m_strServerIPAddr;
    unsigned int m_iServerPort;
    JThreadTP<CSocketClient, void *,int >* m_hThreadHandle;
    bool m_bThreadRunning;
    CRingBuffer* m_pCRingBuffer;
    stSMSokcet m_Socket;
    FunPacketCB m_pPacketCB;
    void* paramProxy;
};

#endif // SOCK_CLIENT_H
