#ifdef Q_OS_WIN
#include <Winsock2.h>
#include <process.h>
#pragma comment(lib,"ws2_32.lib")
#else
#include <fcntl.h>
#define MAX_PATH 260
#endif
#define INVALID_SOCKET -1
#define SOCKET_ERROR  -1
#define SOCKET_PACKET_LENGTH -1
#include "socketserver.h"
#include "socketclientmanager.h"
#include "socketclient.h"


SocketServer::SocketServer():m_pClientManager(NULL),
                             m_pThread(NULL),
                             m_bAcceptThreadRunning(false)
{
    m_pClientManager = new SocketClientManager();
    m_pThread = new JThreadTP<SocketServer, void *, void *>;
}

SocketServer::~SocketServer()
{
    StopSever();

    if (m_pThread !=NULL)
    {
		delete m_pThread;
		m_pThread = NULL;
    }
	if(m_pClientManager != NULL)
    {
        delete m_pClientManager;
        m_pClientManager = NULL;
    }
	
}

bool SocketServer::InitServerSocket(const char *ServerIP, unsigned int Serverport)
{
    if(m_ServerInfo.bServerInit)
    {
        return true;
    }
    m_ServerInfo.strIPAddr = ServerIP;
    m_ServerInfo.iPort = Serverport;
    return true;
}

ISocketClient *SocketServer::GetSocketClient(const char *ClientIP, unsigned int Clientport)
{
    return m_pClientManager->CreateSocketClient (ClientIP,Clientport);
}

bool SocketServer::StartSever()
{
   return  CreateSock();
}

bool SocketServer::StopSever()
{
    m_pClientManager->ReleaseAllClient();
    if (m_bAcceptThreadRunning)
    {
        m_bAcceptThreadRunning = false;
        int icount  = 0;
        while (m_pThread->IsRunning())
        {
           Sleep(50);
           icount++;
           if(icount>=10)
           {
               break;
           }
        }
		if (m_pThread->IsRunning())
		{
			m_pThread->EndThread();
		}

    }
    m_ServerInfo.bServerInit = false;

    if (INVALID_SOCKET !=  m_ServerInfo.ServerSock)
    {
        closeSocket();
    }
//#ifdef Q_OS_WIN
    //WSACleanup();
//#endif
    ClearNetEvn();
    return true;
}

void SocketServer::closeSocket()
{
    if (m_ServerInfo.ServerSock == INVALID_SOCKET)
    {
        return ;
    }
    int iRet=0;
#ifdef Q_OS_WIN
    iRet=closesocket(m_ServerInfo.ServerSock);
#else
    iRet=close(m_ServerInfo.ServerSock);
#endif
    if(iRet==0)
    {
        m_ServerInfo.ServerSock = -1;
    }
}


std::list<ISocketClient *> *SocketServer::GetClientList()
{
    if (m_pClientManager == NULL)
    {
        return NULL;
    }
    return m_pClientManager->GetClientList();
}

void SocketServer::ReleaseSocketClient(ISocketClient *pClient)
{
    if(m_pClientManager == NULL)
    {
        return;
    }
    m_pClientManager->ReleaseSocketClient(pClient);
}

SocketServerInfo *SocketServer::GetServerInfo()
{
    return &m_ServerInfo;
}

bool SocketServer::CreateSock()
{
    int tmpResult;   // 保存返回结果
#ifdef Q_OS_WIN
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;                   // 监听socket
    SOCKET ClientSocket = INVALID_SOCKET;                   // 连接socket
    //SOCKADDR_IN    serverinfo;

    if (m_ServerInfo.bServerInit)
    {
        return true;
    }
    if(m_ServerInfo.ServerSock != INVALID_SOCKET)
    {
        return true;
    }
    //tmpResult = WSAStartup(MAKEWORD(2,2), &wsaData);        // 加载Ws2_32.dll
    //if (tmpResult != 0)
    //{
    //    return false;
    //};
    InitNetEvn();
#endif
    struct sockaddr_in ServerSockAdd;
    ServerSockAdd.sin_family = AF_INET;
    //  填充IP地址
    if (m_ServerInfo.strIPAddr.length() == 0 )
    {
        ServerSockAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        ServerSockAdd.sin_addr.s_addr =inet_addr(m_ServerInfo.strIPAddr.c_str());
    }
    // 填充 端口号
    ServerSockAdd.sin_port =  htons(m_ServerInfo.iPort);
    // 创建监听 Socket
    m_ServerInfo.ServerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_ServerInfo.ServerSock == INVALID_SOCKET)
    {
#ifdef Q_OS_WIN
        ClearNetEvn();
#endif
        return false;
    }

#ifdef Q_OS_WIN
    u_long iMode = 1;
    ioctlsocket(m_ServerInfo.ServerSock, FIONBIO, &iMode);
#else
    int flags = fcntl(m_ServerInfo.ServerSock, F_GETFL, 0);
    fcntl(m_ServerInfo.ServerSock, F_SETFL, flags|O_NONBLOCK);
#endif

    int iSendBufSize = SOCKET_PACKET_LENGTH;
    int iOptLen = sizeof(iSendBufSize);
    int iRet = setsockopt(m_ServerInfo.ServerSock, SOL_SOCKET, SO_SNDBUF, (char *)&iSendBufSize, iOptLen);

    int iRcvBufSize = SOCKET_PACKET_LENGTH;
    iOptLen = sizeof(iRcvBufSize);
    iRet = setsockopt(m_ServerInfo.ServerSock, SOL_SOCKET, SO_RCVBUF, (char *)&iRcvBufSize, iOptLen);

    const int yes = 1;
    setsockopt(m_ServerInfo.ServerSock,SOL_SOCKET,SO_REUSEADDR,(const char*)&yes,sizeof(yes));


    tmpResult = bind(m_ServerInfo.ServerSock,(const struct sockaddr*)(&ServerSockAdd), (socklen_t)sizeof(ServerSockAdd));
    if (tmpResult == SOCKET_ERROR)
    {
        return false;
    }

    // 开始监听
    tmpResult = listen(m_ServerInfo.ServerSock, SOMAXCONN);
    if (tmpResult == SOCKET_ERROR)
    {
        return false;
    }

    if(m_pThread == NULL)
    {
        return false;
    }
    if(!m_pThread->IsRunning ())
    {
        if (m_pThread->StartThread(this, &SocketServer::AcceptThread, this)<0)
        {
            return false;
        }
        do
        {
            Sleep(50);
        } while (!m_bAcceptThreadRunning);
    }


    m_ServerInfo.bServerInit = true;
    return true;
}


void* SocketServer:: AcceptThread(void* lpParameter)
{
    SocketServer *pthis = NULL;
    if (NULL != lpParameter)
    {
        pthis = (SocketServer*) lpParameter;
    }
    struct sockaddr_in   ClientAddIN;
#ifdef Q_OS_WIN
    ZeroMemory(&ClientAddIN, sizeof(ClientAddIN));
    int length = sizeof(ClientAddIN);
#else
    bzero(&ClientAddIN, sizeof(ClientAddIN));
     socklen_t length = sizeof(ClientAddIN);
#endif
    fd_set fd,fdtmp;
    FD_ZERO(&fd);
    FD_ZERO(&fdtmp);
    FD_SET(pthis->m_ServerInfo.ServerSock,&fd);
    SOCKET maxsocket= pthis->m_ServerInfo.ServerSock;
    struct timeval tmtmp;
    tmtmp.tv_sec = 0;
    tmtmp.tv_usec = 1000 *50;

    pthis->m_bAcceptThreadRunning = true;
    while (pthis->m_bAcceptThreadRunning)
    {
        fdtmp = fd ;
        int iResult = select(maxsocket+1,&fdtmp,NULL,NULL,&tmtmp);
        if (iResult >0 && FD_ISSET(pthis->m_ServerInfo.ServerSock,&fdtmp))
        {
            // 如果是服务器可读，就是有socket连入
            SOCKET ClientSocket = accept(pthis->m_ServerInfo.ServerSock,(struct sockaddr *)&ClientAddIN,&length);
            if (INVALID_SOCKET != ClientSocket)
            {
#ifdef Q_OS_WIN
                u_long iMode = 1;
                ioctlsocket(ClientSocket, FIONBIO, &iMode);
#else
                int flag = fcntl(ClientSocket, F_GETFL, 0);
                fcntl(ClientSocket, F_GETFL, flag|O_NONBLOCK);
#endif
                ClientInfo clientinfotmp;
                clientinfotmp.strClientIP = inet_ntoa(ClientAddIN.sin_addr);
                clientinfotmp.ClientPort = ntohs(ClientAddIN.sin_port);
                clientinfotmp.strServerIP = pthis->m_ServerInfo.strIPAddr;
                clientinfotmp.ServerPort = pthis->m_ServerInfo.iPort;
                clientinfotmp.bConnected = true;
                clientinfotmp.ClientSock = ClientSocket;

                ISocketClient *iClient = pthis->m_pClientManager->CreateSocketClient (clientinfotmp.strClientIP.c_str (),clientinfotmp.ClientPort);
				iClient->SetSokectType(SocketDataUseListCallBack);
                SocketClient* ptmp = dynamic_cast<SocketClient*>(iClient);
                if(ptmp != NULL)
                {
                    ptmp->SetClientInfo (&clientinfotmp);
                    ptmp->StartClient();
                    ptmp->ClearClientRecvBuffer ();
                }
            }
        }
        Sleep(50);
    }
    return NULL;
}

bool SocketServer::IsClientStatusChanged()
{
	bool bflag = false;
	if (m_pClientManager == NULL)
	{
		return false;
	}
	return m_pClientManager->IsClientStatusChange();
}

