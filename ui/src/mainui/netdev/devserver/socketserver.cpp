#include "socketserver.h"
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
#include "socketPC1.h"
#include "socketservercfg.h"
#include <QDomElement>
#include <QFile>
CSocketServer* CSocketServer::m_pInstance = NULL;
static int g_InitNet = 0;

void InitNetEvn()
{
	if (0 == g_InitNet)
	{
#ifdef Q_OS_WIN
		WORD wVersionRequested;
		WSADATA wsaData;
		wVersionRequested = MAKEWORD(2, 2);
		int err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			return;
		}
		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			WSACleanup();
			return;
		}
#endif
	}

	++g_InitNet;
}

void ClearNetEvn()
{
	--g_InitNet;
	if (0 == g_InitNet)
	{
#ifdef Q_OS_WIN
		WSACleanup();
#endif
	}
}

#define SOCKET_SERVER_LOG           "socketServer.log"
CSocketServer::CSocketServer()
{
    LOG_INFO_F(SOCKET_SERVER_LOG,"CSocketServer!\n");
    m_bServerInit = false;
    m_strServerIPAddr = "127.0.0.1";
    m_iServerPort = 10000;
    m_bThreadRunning = false;
    m_iServerSock = -1;
    m_pAcceptThread = new JThreadTP<CSocketServer, void *, void *>();
    m_pPC1 = new ClientPCUnion(PACKET_TARGET_PC1);
    m_pPC2 = new ClientPCUnion(PACKET_TARGET_PC2);;
    m_pPC3 = new ClientPCUnion(PACKET_TARGET_PC3);;
    InitNetEvn();
    initServer();

}

CSocketServer::~CSocketServer()
{
    LOG_INFO_F(SOCKET_SERVER_LOG,"~CSocketServer!\n");
    stopServer();
    if(m_pAcceptThread != NULL)
    {
        delete m_pAcceptThread;
        m_pAcceptThread = NULL;
    }

    if (m_pPC1 != NULL)
    {
        delete m_pPC1;
        m_pPC1 = NULL;
    }

    if (m_pPC2 != NULL)
    {
        delete m_pPC2;
        m_pPC2 = NULL;
    }

    if (m_pPC3 != NULL)
    {
        delete m_pPC3;
        m_pPC3 = NULL;
    }

    m_iServerPort = 0;
    m_iServerSock = -1;
	ClearNetEvn();
}

CSocketServer *CSocketServer::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CSocketServer();
    }
    return m_pInstance;
}

void CSocketServer::releaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
    return ;
}

int CSocketServer::initServer()
{
    LOG_INFO_F(SOCKET_SERVER_LOG,"init_server Enter!\n");
    CSocketServerCfg* pServerCfg = CSocketServerCfg::getInstance();
    if (pServerCfg == NULL)
    {
        return ERR_SOCKET_SERVER_INIT;
    }
    serverCfg* pserverCfg = pServerCfg->getServerCfg();
    if (pserverCfg == NULL || pserverCfg->serverIP.length() ==0 || pserverCfg->serverPort == 0)
    {
        return ERR_SOCKET_SERVER_INIT;
    }
    if(m_strServerIPAddr.compare(pserverCfg->serverIP) != 0 || m_iServerPort != pserverCfg->serverPort)
    {
        stopServer();
    }
    removeAllSocket();
    vector<string> vecClientName;
    if (!pServerCfg->getClientList(vecClientName))
    {
        return ERR_SOCKET_SERVER_INIT;
    }
    for (int i = 0; i < vecClientName.size(); ++i)
    {
        clientCfg* pSMCameraCfg = pServerCfg->getClientCfg(vecClientName[i]);
        if (NULL == pSMCameraCfg)
        {
            continue;
        }
        if (m_Sokcetmap.find(pSMCameraCfg->ClientName) == m_Sokcetmap.end())
        {
            stSMSokcet* pSMSokcet = new stSMSokcet();
            pSMSokcet->IP = pSMCameraCfg->ClientIP;
            pSMSokcet->iPort = pSMCameraCfg->ClientPort;
            m_Sokcetmap.insert(std::pair<string,stSMSokcet*>(pSMCameraCfg->ClientName,pSMSokcet));
        }
    }

    m_strServerIPAddr = pserverCfg->serverIP;
    m_iServerPort = pserverCfg->serverPort;
    return ERR_OK;
}

int CSocketServer::startServer()
{
    if(m_bServerInit)
        return ERR_OK;
    LOG_INFO_F(SOCKET_SERVER_LOG,"start_server Enter!\n");
    int tmpResult;
    struct sockaddr_in ServerSockAdd;
    ServerSockAdd.sin_family = AF_INET;
    if (m_strServerIPAddr.length() == 0 )
    {
        ServerSockAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        ServerSockAdd.sin_addr.s_addr =inet_addr(m_strServerIPAddr.c_str());
    }
#ifdef Q_OS_WIN
    int ilen = sizeof(ServerSockAdd);
#else
    socklen_t ilen = sizeof(ServerSockAdd);
#endif

    ServerSockAdd.sin_port =  htons(m_iServerPort);
    m_iServerSock= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_iServerSock == INVALID_SOCKET)
    {
        return ERR_SOCKET_CREATE;
    }

    unsigned long ul = 1;
    int r = ioctlsocket(m_iServerSock, FIONBIO, &ul);

    int iSendBufSize = SOCKET_PACKET_LENGTH;
    int iOptLen = sizeof(iSendBufSize);
    int iRet = setsockopt(m_iServerSock, SOL_SOCKET, SO_SNDBUF, (char *)&iSendBufSize, iOptLen);

    int iRcvBufSize = SOCKET_PACKET_LENGTH;
    iOptLen = sizeof(iRcvBufSize);
    iRet = setsockopt(m_iServerSock, SOL_SOCKET, SO_RCVBUF, (char *)&iRcvBufSize, iOptLen);

    const int yes = 1;
    setsockopt(m_iServerSock,SOL_SOCKET,SO_REUSEADDR,(const char*)&yes,sizeof(yes));

    tmpResult = ::bind(m_iServerSock,(const struct sockaddr*)(&ServerSockAdd), ilen);
    if (tmpResult == SOCKET_ERROR)
    {
        return ERR_SOCKET_BIND;
    }

    tmpResult = listen(m_iServerSock, SOMAXCONN);
    if (tmpResult == SOCKET_ERROR)
    {
        return ERR_SOCKET_LISTEN;
    }

    if(!m_pAcceptThread->IsRunning ())
    {
        m_pAcceptThread->StartThread(this, &CSocketServer::AcceptThread, this);
    }

    m_bServerInit = true;
    return ERR_OK;
}

int CSocketServer::stopServer()
{
    LOG_INFO_F(SOCKET_SERVER_LOG,"stop_server Enter!\n");
    if (m_bThreadRunning)
    {
        m_bThreadRunning = false;
        int icount  = 0;
        while (m_pAcceptThread->IsRunning())
        {
           Sleep(50);
           icount++;
           if(icount>=10)
           {
               break;
           }
        }
        if (m_pAcceptThread->IsRunning())
        {
            m_pAcceptThread->EndThread();
        }
    }

    m_bServerInit = false;
    CSocketCommon::closeSocket(m_iServerSock);
    removeAllSocket();
    return ERR_OK;
}

int CSocketServer::resetServer()
{
    LOG_INFO_F(SOCKET_SERVER_LOG,"reset_server Enter!\n");
    stopServer();
    initServer();
    startServer();
    return ERR_OK;
}

bool CSocketServer::isServerRunning()
{
    return m_bServerInit;
}

CSocketClientPC1 *CSocketServer::getSocketClient(string strClientName)
{
    if(strClientName == PACKET_TARGET_PC1)
    {
        return m_pPC1->getSocketClientPC(0);
    }
    else if(strClientName == PACKET_TARGET_PC2)
    {
        return m_pPC2->getSocketClientPC(0);
    }
    else if(strClientName == PACKET_TARGET_PC2)
    {
        return m_pPC3->getSocketClientPC(0);
    }
    return NULL;
}

void *CSocketServer::AcceptThread(void *lpParameter)
{
    LOG_INFO_F(SOCKET_SERVER_LOG,"AcceptThread Enter!\n");

    CSocketServer *pthis = NULL;
    if (NULL != lpParameter)
    {
       pthis = (CSocketServer*) lpParameter;
    }

    struct sockaddr_in   ClientAddIN;

#ifdef Q_OS_WIN
    int length = sizeof(ClientAddIN);
#else
    socklen_t length = sizeof(ClientAddIN);
#endif


    fd_set fd,fdtmp;
    FD_ZERO(&fd);
    FD_ZERO(&fdtmp);
    FD_SET(pthis->m_iServerSock,&fd);
    SOCKET maxsocket= pthis->m_iServerSock;
    struct timeval tmtmp;
    tmtmp.tv_sec = 0;
    tmtmp.tv_usec = 1000 *50;

    pthis->m_bThreadRunning = true;
    while (pthis->m_bThreadRunning )
    {
       fdtmp = fd ;
       int iResult = select(maxsocket+1,&fdtmp,NULL,NULL,&tmtmp);
       if (iResult >0 && FD_ISSET(pthis->m_iServerSock,&fdtmp))
       {
           SOCKET ClientSocket = accept(pthis->m_iServerSock,(struct sockaddr *)&ClientAddIN,&length);
           if (INVALID_SOCKET != ClientSocket)
           {
               unsigned long ul = 1;
               int r = ioctlsocket(ClientSocket, FIONBIO, &ul);
               string strClientIP = inet_ntoa(ClientAddIN.sin_addr);
               int ClientPort = ntohs(ClientAddIN.sin_port);

               LOG_INFO_F(SOCKET_SERVER_LOG,"Server accept ip = %s ,prot = %d \n",strClientIP.c_str(),ClientPort);

               stSMSokcet stSokcet;
               stSokcet.iSocket = ClientSocket;
               stSokcet.IP = strClientIP;
               stSokcet.iPort = ClientPort;
               dispatchSocket(stSokcet);
           }
       }
       Sleep(50);
    }
    LOG_INFO_F(SOCKET_SERVER_LOG,"AcceptThread Exit \n");
    return NULL;
}

int CSocketServer::dispatchSocket(stSMSokcet& pSocket)
{
    int iRet = 0;
    bool bflound = false;
    // 根据IP地址 或是 端口号来区分联入客户端
    CISAPUGuard<JMutex> autolock(m_SocketListLock);
    map<string,stSMSokcet*>::iterator it = m_Sokcetmap.begin();
    while(it != m_Sokcetmap.end())
    {
        if((it->second) != NULL && it->second->IP == pSocket.IP)
        {

            it->second->iSocket =pSocket.iSocket;
            it->second->iPort =pSocket.iPort;
            it->second->bConnected = true;

            if(it->first == PACKET_TARGET_PC1)
            {
                m_pPC1->setSocket(*(it->second));
            }
            else if(it->first == PACKET_TARGET_PC2)
            {
                m_pPC2->setSocket(*(it->second));
            }
            else if(it->first == PACKET_TARGET_PC2)
            {
                m_pPC3->setSocket(*(it->second));
            }
            bflound = true;
            break;
        }
        it++;
    }
    if(!bflound)
    {
        CSocketCommon::closeSocket(&pSocket);
    }

    return iRet;
}


void CSocketServer::removeAllSocket()
{
    CISAPUGuard<JMutex> autolock(m_SocketListLock);
    map<string,stSMSokcet*>::iterator it = m_Sokcetmap.begin();
    while(it != m_Sokcetmap.end())
    {
        if((it->second) != NULL)
        {
            if(it->first == PACKET_TARGET_PC1)
            {
                for (int i = 0 ;i <m_pPC1->getSocketClientPC1Size();i++)
                {
                    if (m_pPC1->getSocketClientPC(i))
                    {
                        m_pPC1->getSocketClientPC(i)->disConneced();
                    }
                }              
            }
            else if(it->first == PACKET_TARGET_PC2)
            {
                for (int i = 0 ;i <m_pPC2->getSocketClientPC1Size();i++)
                {
                    if (m_pPC2->getSocketClientPC(i))
                    {
                        m_pPC2->getSocketClientPC(i)->disConneced();
                    }
                }   
            }
            else if(it->first == PACKET_TARGET_PC2)
            {
                for (int i = 0 ;i <m_pPC3->getSocketClientPC1Size();i++)
                {
                    if (m_pPC3->getSocketClientPC(i))
                    {
                        m_pPC3->getSocketClientPC(i)->disConneced();
                    }
                }   
            }
            CSocketCommon::closeSocket(it->second);
            delete (it->second);
        }
        it++;
    }
    m_Sokcetmap.clear();
}


