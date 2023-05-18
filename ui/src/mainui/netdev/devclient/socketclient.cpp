#include "socketclient.h"
#ifdef Q_OS_WIN
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#else
#include "socketserver.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include "trace.h"
#include "jtime.h"
#include "motion/iioctrlinterface.h"
#include "motion/imotion.h"
#include "callsdk.h"
#include "stringcommon.h"
#include "uidefine.h"


#define SOCKET_CLIENT_LOG           "socketClient.log"

CSocketClient::CSocketClient()
{
    m_hThreadHandle = new JThreadTP<CSocketClient, void *,int >();
    m_pCRingBuffer = new CRingBuffer(1024 * 4); // 4K buffer

}

CSocketClient::~CSocketClient()
{
    CSocketCommon::closeSocket(&m_Socket);

    m_bThreadRunning = false;
    if(m_hThreadHandle != NULL)
    {
        delete m_hThreadHandle;
        m_hThreadHandle = NULL;
    }
    if(m_pCRingBuffer != NULL)
    {
        delete m_pCRingBuffer;
        m_pCRingBuffer = NULL;
    }
}

int CSocketClient::initClient(const char *ClinetIP, unsigned int Clinetport)
{
    LOG_INFO_F(SOCKET_CLIENT_LOG,"initClient Enter!\n");
    if(m_strClinetIPAddr.compare(ClinetIP) != 0 || m_iClinetPort != Clinetport)
    {
        disConnect();
    }
    m_strClinetIPAddr = ClinetIP;
    m_iClinetPort = Clinetport;
    return ERR_OK;
}

int CSocketClient::connectServer()
{
    int iRet = ERR_OK;
    LOG_INFO_F(SOCKET_CLIENT_LOG,"connectServer Enter!\n");


    if(m_Socket.bConnected)
    {
        return ERR_OK;
    }
    else
    {
        iRet = connectSocket();
    }
    return iRet;

}

int CSocketClient::disConnect()
{
    m_bThreadRunning = false;
    int icount  = 0;
    while (m_hThreadHandle->IsRunning())
    {
       Sleep(50);
       icount++;
       if(icount>=10)
       {
           break;
       }
    }
    if (m_hThreadHandle->IsRunning())
    {
        m_hThreadHandle->EndThread();
    }

    if (m_pCRingBuffer != NULL)
    {
        m_pCRingBuffer->ClearBuffer();
    }
    CSocketCommon::closeSocket(&m_Socket);
    return ERR_OK;
}

bool CSocketClient::isConnected()
{
    return m_Socket.bConnected;
}


void CSocketClient::setServerCfg( const char* ServerIP, unsigned int Serverport )
{
    LOG_INFO_F(SOCKET_CLIENT_LOG,"initClient Enter!\n");
    if(m_strServerIPAddr.compare(ServerIP) != 0 || m_iServerPort != Serverport)
    {
        disConnect();
    }
    m_strServerIPAddr = ServerIP;
    m_iServerPort = Serverport;
    m_Socket.serverIP = ServerIP;
    m_Socket.iserverPort =Serverport;
    return ;
}

void CSocketClient::setProcessPacketCB(void * param,FunPacketCB pPacketCB)
{
    if(pPacketCB != NULL && param != NULL )
    {
        m_pPacketCB = pPacketCB;
        paramProxy = param;
    }
}

int CSocketClient::recvMsg(char *buffer, unsigned int bufferLen, int &recvLength, int timeout)
{
    return CSocketCommon::recvexUDP(m_Socket.iSocket,m_Socket.serverIP.c_str(),m_Socket.iserverPort,buffer,bufferLen,recvLength);
    //return CSocketCommon::recvex(m_Socket.iSocket,buffer,bufferLen,recvLength,timeout);
}

int CSocketClient::sendMsg(char *buffer, unsigned int bufferLen, int &sendLength, int timeout)
{
    return CSocketCommon::sendexUDP(m_Socket.iSocket,m_Socket.serverIP.c_str(),m_Socket.iserverPort,buffer,bufferLen,sendLength);
    //return CSocketCommon::sendex(m_Socket.iSocket,buffer,bufferLen,sendLength,timeout);
}

int  CSocketClient::createSocket()
{
    if (m_Socket.iSocket != INVALID_SOCKET)
    {
        return ERR_OK;
    }

    /*struct sockaddr_in sockAddr;*/
     m_Socket.iSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   // m_pstSokcet->iSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_Socket.iSocket == INVALID_SOCKET)
    {
        return ERR_SOCKET_CREATE;
    }

//    struct sockaddr_in SockAdd;
//    SockAdd.sin_family = AF_INET;
//    //  填充IP地址
//    if (strlen(m_strClinetIPAddr.c_str()) == 0)
//    {
//        SockAdd.sin_addr.s_addr = htonl(INADDR_ANY);
//    }
//    else
//    {
//        SockAdd.sin_addr.s_addr =inet_addr(m_strClinetIPAddr.c_str());
//    }
    // 填充 端口号
//    SockAdd.sin_port =  htons(m_iClinetPort);
//    ::bind(m_Socket.iSocket,(struct sockaddr*)(&SockAdd), sizeof(SockAdd));

//    const int yes = 1;
//    setsockopt(m_pstSokcet->iSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));

    //unsigned long ul = 1;
    //int r = ioctlsocket(stSocket.iSocket, FIONBIO, &ul);
    return ERR_OK;
}


int CSocketClient::connectSocket()
{
    int iSocket = m_Socket.iSocket;

    if (m_Socket.bConnected)
    {
        return ERR_OK;
    }
    int iErr = ERR_OK;
    iErr = createSocket();
    if (iErr != ERR_OK)
        return iErr;


    struct sockaddr_in myselfsockAddr;
    myselfsockAddr.sin_addr.s_addr = inet_addr(m_strClinetIPAddr.c_str());
    myselfsockAddr.sin_family = AF_INET;
    myselfsockAddr.sin_port = htons(m_iClinetPort);
#ifdef Q_OS_WIN
    int ilen = sizeof(sockaddr_in);
#else
    socklen_t ilen = sizeof(sockaddr_in);
#endif

    if (::bind(m_Socket.iSocket, (struct sockaddr*)&myselfsockAddr, ilen) == SOCKET_ERROR)
    {
        CSocketCommon::closeSocket(&m_Socket);
        return ERR_SOCKET_BIND;
    }

//    struct sockaddr_in sockAddr;
//    sockAddr.sin_addr.s_addr = inet_addr(m_strServerIPAddr.c_str());
//    sockAddr.sin_family = AF_INET;
//    sockAddr.sin_port = htons(m_iServerPort);
//    if (connect(m_pstSokcet->iSocket, (struct sockaddr*)&sockAddr, ilen) == SOCKET_ERROR)
//    {
//        int iError = WSAGetLastError();
//        CSocketCommon::closeSocket(m_pstSokcet);
//        return ERR_SOCKET_CONNECT_ERROR;
//    }
    unsigned long ul = 1;
    int r = ioctlsocket(m_Socket.iSocket, FIONBIO, &ul);
    m_Socket.bConnected = true;
    if(m_hThreadHandle  != NULL && !m_hThreadHandle->IsRunning())
    {
        m_hThreadHandle->StartThread(this,&CSocketClient::RecvThread,(void*)(&m_Socket));
    }
    return ERR_OK;
}

int CSocketClient::RecvThread(void *param)
{

    stSMSokcet *pstSocket = (stSMSokcet *)param;
    fd_set fd,fdtmp;
    SOCKET maxsocket;

    struct timeval tmtmp;
    tmtmp.tv_sec = 0;
    tmtmp.tv_usec = 1000 *20;

    FD_ZERO(&fd);
    FD_ZERO(&fdtmp);
    FD_SET(pstSocket->iSocket,&fd);
    maxsocket= pstSocket->iSocket;
    m_bThreadRunning = true;
    do
    {
        Sleep(50);
    }while(m_bThreadRunning && pstSocket->iSocket== INVALID_SOCKET);


    while ( m_bThreadRunning && pstSocket->iSocket!= INVALID_SOCKET)
    {
       fdtmp = fd ;
       int iResult = select(maxsocket+1,&fdtmp,NULL,NULL,&tmtmp);
       if (iResult >0 && FD_ISSET(pstSocket->iSocket,&fdtmp))
       {
           int length = 0;
           char buffer[1024] = { 0 };
           length = recv(pstSocket->iSocket, buffer, sizeof(buffer), 0);
           if (length > 0 && m_pCRingBuffer != NULL)
           {
               CSocketCommon::sendLogToUI(string("recv: ") + string(buffer),UILOG_WARN);
               LOG_INFO_F(SOCKET_CLIENT_LOG, "RecvThread recv = %s",buffer);
               m_pCRingBuffer->PushBinary(buffer, length);
               int iPos = 0;
               bool bflag = m_pCRingBuffer->FindChar('#', iPos);
               while (bflag)
               {
                   char *pkgBuf = new char[iPos + 2];
                   memset(pkgBuf, 0, iPos + 2);
                   m_pCRingBuffer->PopBinary(pkgBuf, iPos + 1);
                   m_pPacketCB(paramProxy,pkgBuf);
                   delete pkgBuf;
                   bflag = m_pCRingBuffer->FindChar('#', iPos);
               }
           }
           else if (0 == length)
           {
               CSocketCommon::closeSocket(pstSocket);
               CSocketCommon::sendLogToUI("recv error return 0", UILOG_ERR);
               LOG_WARN_F(SOCKET_CLIENT_LOG, "RecvThread recv length = 0");
               return ERR_SOCKET_RECV_ERROR;

           }
           else if (-1 == length)
           {
#ifdef Q_OS_WIN
               int iError = WSAGetLastError();
               if (WSAENETDOWN == iError || WSAECONNRESET == iError || WSAESHUTDOWN == iError)
               {
                   CSocketCommon::closeSocket(pstSocket);
#endif
                   CSocketCommon::sendLogToUI("recv error return -1",UILOG_ERR);
                   LOG_WARN_F(SOCKET_CLIENT_LOG, "RecvThread recv error (-1 )= %d", iError);
                   return ERR_SOCKET_RECV_ERROR;
               }
           }
       }
       Sleep(10);
    }
    LOG_INFO_F(SOCKET_CLIENT_LOG,"RecvThread Exit \n");

    return  ERR_OK;
}
