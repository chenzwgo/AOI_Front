#include "socketclient.h"
//#include <process.h>
#ifdef Q_OS_WIN
    #include <IPHlpApi.h>
    #include <IcmpAPI.h>
    #pragma comment(lib,"ws2_32.lib")
    #pragma comment(lib, "IPHLPAPI.lib")
#else
    #define SOCKET int
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <sys/time.h>

#endif

#define INVALID_SOCKET -1
#define SOCKET_ERROR  -1
#define SOCKET_PACKET_STRING_LENGTH 1024

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
    if(0 == g_InitNet)
    {
#ifdef Q_OS_WIN
        WSACleanup();
#endif
    }
}


SocketClient::SocketClient(void)
            :m_pThread(NULL)
            ,m_pPingThread(NULL)
{
	m_pCRingBuffer = new CRingBuffer(1024);
	m_MutexLock.Init();
	m_MutexSocketLock.Init();
    m_bEnablePing = false;
    m_bInit = false;
    m_bStarted = false;
    m_bRecvThread = false;
    m_bPingThread = false;
	m_bChanged =false;
    m_CBFun = NULL;
    m_SocketType = SocketNoCallBack;
    m_myinfo.strServerIP = "";
	m_pThread = new JThreadTP<SocketClient, void *, void *>;
	m_pPingThread = new JThreadTP<SocketClient, void *, void *>;

}

SocketClient::~SocketClient(void)
{
    StopClient();
    ClearClientRecvBuffer();
    m_CBFun = NULL;
    if(m_pThread != NULL)
    {
        delete m_pThread;
        m_pThread = NULL;
    }
	if (m_pPingThread != NULL)
	{
		delete m_pPingThread;
		m_pPingThread = NULL;
	}
	if (m_pCRingBuffer != NULL)
	{
		delete m_pCRingBuffer;
		m_pCRingBuffer = NULL;
	}
    closeSocket();
}
bool SocketClient::ReConnect()
{
    if (m_myinfo.strServerIP.empty())
    {
        return false;
    }
    return Connect(m_myinfo.strServerIP.c_str(), m_myinfo.ServerPort,m_bEnablePing);
}

bool SocketClient::Connect(const char *pServerIP, unsigned int ServerPort,bool enablePing)
{

    m_bEnablePing = enablePing;
    if(IsClientOnline())
    {
        return true;
    }

    if (pServerIP == NULL)
    {
        return false;
    }
    CreateSock();    
    if(INVALID_SOCKET == m_myinfo.ClientSock)
    {
        return false;
    }

    m_myinfo.strServerIP = pServerIP;
    m_myinfo.ServerPort = ServerPort;

    struct sockaddr_in sockAddr;
    struct sockaddr_in myselfsockAddr;

    sockAddr.sin_addr.s_addr = inet_addr(pServerIP);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(ServerPort);

    myselfsockAddr.sin_addr.s_addr = inet_addr(m_myinfo.strClientIP.c_str());
    myselfsockAddr.sin_family = AF_INET;
    myselfsockAddr.sin_port = htons(m_myinfo.ClientPort);
#ifdef Q_OS_WIN
    int ilen = sizeof(sockAddr);
#else
    socklen_t ilen = sizeof(sockAddr);
#endif
	const int yes = 1;
	setsockopt(m_myinfo.ClientSock,SOL_SOCKET,SO_REUSEADDR,(const char*)&yes,sizeof(yes));
    if(bind(m_myinfo.ClientSock,(struct sockaddr*)&myselfsockAddr, ilen) == SOCKET_ERROR)
    {
		closeSocket();
        return false;
    }

    if (connect(m_myinfo.ClientSock, (struct sockaddr*)&sockAddr, ilen) == SOCKET_ERROR)
	{
		closeSocket();
        return false;
    }
	
//	struct sockaddr_in myname;
//	int mylen = sizeof(myname);
//	if (!getsockname(m_myinfo.ClientSock,(struct sockaddr*)&myname,&mylen))
//	{
//		m_myinfo.strClientIP = inet_ntoa(myname.sin_addr);
//		m_myinfo.ClientPort = ntohs(myname.sin_port);
//	}

	ChangeConnectStatus(true);
    StartClient ();
    return true;
}

bool SocketClient::DisConnect()
{
    bool bflag = false;
    if(!IsClientOnline())
    {
        return true;
    }

    if (m_myinfo.ClientSock == INVALID_SOCKET)
    {
        return false;
    }
    closeSocket();
    ClearClientRecvBuffer();
    return true;

}


void SocketClient::closeSocket()
{
    if (m_myinfo.ClientSock == INVALID_SOCKET)
    {
        return ;
    }
    m_MutexSocketLock.Lock();
    int iRet=0;
#ifdef Q_OS_WIN
    iRet=closesocket(m_myinfo.ClientSock);
#else
    iRet=close(m_myinfo.ClientSock);
#endif
    if(iRet==0)
    {
        m_myinfo.ClientSock = -1;
        ChangeConnectStatus(false);
    }
    m_MutexSocketLock.Unlock();
    m_bInit = false;
}
bool SocketClient::InitClient(const char *pClientIP, unsigned int ClientPort)
{
    m_myinfo.strClientIP = pClientIP;
    m_myinfo.ClientPort =ClientPort;
    return true;
}

bool SocketClient::SetClientInfo(const ClientInfo *pinfo)
{
    if(pinfo == NULL)
    {
        return false;
    }
    m_myinfo = *pinfo;
	ChangeConnectStatus(true);
    return true;
}

const ClientInfo *SocketClient::GetClientInfo()
{
    return &m_myinfo;
}

bool SocketClient::CreateSock()
{
    if(m_myinfo.ClientSock != -1)
    {
        return false;
    }
    //#ifdef Q_OS_WIN
        //WORD wVersionRequested;
        //WSADATA wsaData;
        //int err;

        //wVersionRequested = MAKEWORD(2, 2);
        //err = WSAStartup(wVersionRequested, &wsaData);
        //if (err != 0)
        //{
        //    return false;
        //}
        //if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        //{
        //    WSACleanup();
        //    return false;
        //}
    //#endif
    InitNetEvn();
    if (m_bInit)
    {
        //客户端套接口已初始化，无需再初始化
        return true;
    }
    /*struct sockaddr_in sockAddr;*/
    m_myinfo.ClientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_myinfo.ClientSock == INVALID_SOCKET)
    {
        //创建socket失败
        return false;
    }
    const int yes = 1;
    setsockopt(m_myinfo.ClientSock,SOL_SOCKET,SO_REUSEADDR,(const char*)&yes,sizeof(yes));

    ClearClientRecvBuffer();
    m_bInit = true;
    return true;
}

bool SocketClient::StartClient()
{
    if (m_bStarted)
    {
        return true;
    }
    if(m_pThread == NULL)
     {
        return false;
    }
    if (m_pThread->StartThread(this, &SocketClient::RecvThread, this)<0)
    {
        return false;
    }
    if(m_pPingThread->StartThread(this,&SocketClient::PingThread,this) <0)
    {
        return false;
    }

    m_bStarted = true;
    return true;
}

bool SocketClient::StopClient()
{
    if (!m_bStarted)
    {
        return true;
    }
    if (m_bRecvThread)
    {
        m_bRecvThread = false;
        while (m_pThread->IsRunning())
        {
            Sleep(50);

        }
    }
    if (m_bPingThread)
    {
        m_bPingThread = false;
        while (m_pPingThread->IsRunning())
        {
            Sleep(50);
        }
    }

    m_bStarted = false;
    return true;
}

bool SocketClient::myping( const char *pIP, int nOverTime )
{
    bool bPing = false;

    if (NULL == pIP)
    {
        return bPing;
    }
#ifdef Q_OS_WIN
    // 创建ping句柄
    HANDLE hIcmp = IcmpCreateFile();

    if (NULL == hIcmp)
    {
        return bPing;
    }

    IPAddr pAdd = inet_addr(pIP);

    // ping数据包类型
    ICMP_ECHO_REPLY pData;

        // 发送“回声”数据
    IcmpSendEcho(hIcmp, pAdd, NULL, 0, NULL, &pData, sizeof(pData), nOverTime);

    if (IP_SUCCESS == pData.Status)
    {
        bPing = true;
    }

    // 关闭句柄
    IcmpCloseHandle(hIcmp);
#else
    struct timeval *tval;
    int maxfds = 0;
    fd_set readfds;
    unsigned int icount = 1;

    struct sockaddr_in addr;
    struct sockaddr_in from;
    // 设定Ip信息
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(pIP);

    int sockfd;
    // 取得socket
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
       return false;
    }

    struct timeval timeo;
    // 设定TimeOut时间
    timeo.tv_sec = nOverTime / 1000;
    timeo.tv_usec = nOverTime % 1000;

    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo)) == -1)
    {
       return false;
    }

    char sendpacket[4096];
    char recvpacket[4096];
    // 设定Ping包
    memset(sendpacket, 0, sizeof(sendpacket));

    pid_t pid;
    // 取得PID，作为Ping的Sequence ID
    pid=getpid();

    struct ip *iph;
    struct icmp *icmp;


    icmp=(struct icmp*)sendpacket;
    icmp->icmp_type=ICMP_ECHO;  //回显请求
    icmp->icmp_code=0;
    icmp->icmp_cksum=0;
    icmp->icmp_seq=0;
    icmp->icmp_id=pid;
    tval= (struct timeval *)icmp->icmp_data;
    gettimeofday(tval,NULL);
    icmp->icmp_cksum=cal_chksum((unsigned short *)icmp,sizeof(struct icmp));  //校验

    int n;
    // 发包
    n = sendto(sockfd, (char *)&sendpacket, sizeof(struct icmp), 0, (struct sockaddr *)&addr, sizeof(addr));
    if (n < 1)
    {
       return false;
    }

    // 接受
    // 由于可能接受到其他Ping的应答消息，所以这里要用循环
    while(1)
    {
       // 设定TimeOut时间，这次才是真正起作用的
       FD_ZERO(&readfds);
       FD_SET(sockfd, &readfds);
       maxfds = sockfd + 1;
       n = select(maxfds, &readfds, NULL, NULL, &timeo);
       if (n <= 0)
       {
          // printf("ip:%s,Time out error\n",ips);
           close(sockfd);
           return false;
       }

       // 接受
       memset(recvpacket, 0, sizeof(recvpacket));
       int fromlen = sizeof(from);
       n = recvfrom(sockfd, recvpacket, sizeof(recvpacket), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
       if (n < 1)
       {
           break;
       }


       char *from_ip = (char *)inet_ntoa(from.sin_addr);
           // 判断是否是自己Ping的回复
       if (strcmp(from_ip,pIP) != 0)
       {
           //printf("NowPingip:%s Fromip:%s\nNowPingip is not same to Fromip,so ping wrong!\n",ips,from_ip);
           return false;
       }

       iph = (struct ip *)recvpacket;

       icmp=(struct icmp *)(recvpacket + (iph->ip_hl<<2));

       printf("ip:%s\n,icmp->icmp_type:%d\n,icmp->icmp_id:%d\n",pIP,icmp->icmp_type,icmp->icmp_id);
       // 判断Ping回复包的状态
       if (icmp->icmp_type == ICMP_ECHOREPLY && icmp->icmp_id == pid)   //ICMP_ECHOREPLY回显应答
       {
           // 正常就退出循环
           bPing = true;
           break;
       }
       else
       {
           // 否则继续等
           if(icount*50 >= nOverTime)
           {
               break;
           }
           Sleep(50);
           icount++;
           continue;
       }
    }
#endif
    return bPing;
}


unsigned short SocketClient::cal_chksum(unsigned short *addr, int len)
{
    int nleft=len;
    int sum=0;
    unsigned short *w=addr;
    unsigned short answer=0;

    while(nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }

    if( nleft == 1)
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}



int SocketClient::ClientRecvMsg(char *pBuffer, unsigned int ibufferLength,const char *szMark)
{    
    int iRecvLen = 0;
    if (!IsClientOnline() ||  -1 == m_myinfo.ClientSock || NULL  == pBuffer )
    {
        return -1;
    }
    if (!ClientHasDataArrived())
    {
        return 0;
    }

    if (m_SocketType == SocketNoCallBack || m_SocketType ==  SocketUseCallBack)
    {
        iRecvLen = recv(m_myinfo.ClientSock,pBuffer,ibufferLength,0);   
    }
    else
    {
		iRecvLen =  ReadMsgFromBuffer(pBuffer,ibufferLength,szMark);
        //返回链表中接收完成的第一个数据    
    }
    return iRecvLen;
}

 int SocketClient::ClientSendMsg(const char *pData,unsigned int iDataLength)  
{
    if (!IsClientOnline()  ||  NULL == pData || m_myinfo.ClientSock == -1)
    {
        return -1;
    }
	unsigned int iSendLength= 0;
	const char* pbuffer = pData;
	unsigned int iTotalLength =iDataLength;
    do 
    {
        int iLength = send(m_myinfo.ClientSock,pData+iSendLength,iDataLength - iSendLength,0);
        if (iLength <0)
        {
            iSendLength = iLength;
            break;
        }
        iSendLength +=iLength;
    } while (iSendLength < iTotalLength);
    return iSendLength;
}

 bool SocketClient::IsClientOnline() 
{
    return m_myinfo.bConnected;
}

 bool SocketClient::ClientHasDataArrived() 
{
    if (!IsClientOnline() ||  -1 == m_myinfo.ClientSock)
    {
        return false;
    }

    if (m_SocketType == SocketNoCallBack || m_SocketType ==  SocketUseCallBack)
    {
        m_MutexLock.Lock();
        fd_set fd,fdtmp;    
        FD_ZERO(&fd);
        FD_ZERO(&fdtmp);
   
        FD_SET(m_myinfo.ClientSock,&fd); 
        SOCKET maxsocket= m_myinfo.ClientSock;

        struct timeval tmtmp;
        tmtmp.tv_sec = 0;
        tmtmp.tv_usec = 1000;
        fdtmp = fd;
        
        if(select(maxsocket+1,&fdtmp,NULL,NULL,&tmtmp) >0 && FD_ISSET(m_myinfo.ClientSock,&fd))
        {
            m_MutexLock.Unlock();
            return true;
        }
        else
        {
             m_MutexLock.Unlock();
        }
    }
    else
    {
        unsigned int MsgCount = GetMsgDateLength();
        return MsgCount >0 ? true :false;
    }

    return false;

}

void SocketClient::ClearClientRecvBuffer() 
{
    if (!IsClientOnline()  ||  -1 == m_myinfo.ClientSock)
    {
        return;
    }

    if (m_SocketType == SocketNoCallBack || m_SocketType ==  SocketUseCallBack)
    {
        fd_set fd,fdtmp;    
        FD_ZERO(&fd);    
        FD_ZERO(&fdtmp);   
        FD_SET(m_myinfo.ClientSock,&fd); 
        SOCKET maxsocket= m_myinfo.ClientSock;
        fdtmp = fd ;

        struct timeval tmtmp;
        tmtmp.tv_sec = 0;
        tmtmp.tv_usec = 1000;
        int length = 0;
		if (select(maxsocket+1,&fdtmp,NULL,NULL,&tmtmp) >0 && FD_ISSET(m_myinfo.ClientSock,&fdtmp))
		{
			do 
			{
				fdtmp = fd ;
				char buffer[1024] = {0};
				length = 0;
				length = recv(m_myinfo.ClientSock,buffer,sizeof(buffer),0);  
				if (length > 0)
				{
				}
				else
				{
					break;
				}

			} while (select(maxsocket+1,&fdtmp,NULL,NULL,&tmtmp) >0 && FD_ISSET(m_myinfo.ClientSock,&fdtmp));
		}	
    }
    else
    {
         ClearMsgBuffer();
    }
    return ;
}

 void* SocketClient::RecvThread( void* lpParameter )
 {
     SocketClient *pthis = NULL;
     if (NULL != lpParameter)
     {
         pthis = (SocketClient*) lpParameter;
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
     SOCKET maxsocket;

     struct timeval tmtmp;
     tmtmp.tv_sec = 0;
     tmtmp.tv_usec = 1000 *50;

     pthis->m_bRecvThread = true;

     // 初始化时，就根据配置创建的客户对象没有socket，等待客户连接到对象获得socket
     while (pthis->m_bRecvThread)
     {
         if (-1 != pthis->m_myinfo.ClientSock && pthis->IsClientOnline() )
         {
             FD_ZERO(&fd);    
             FD_ZERO(&fdtmp);   
             FD_SET(pthis->m_myinfo.ClientSock,&fd); 
             maxsocket= pthis->m_myinfo.ClientSock;
             break;
         }
         Sleep(100);
     }
     
     while (pthis->m_bRecvThread)
     {
         // 断线之后，socket = -1 重新等待新的socket
         if (-1 == pthis->m_myinfo.ClientSock)
         {
             while (pthis->m_bRecvThread)
             {
                 if (-1 != pthis->m_myinfo.ClientSock)
                 {
                     FD_ZERO(&fd);    
                     FD_ZERO(&fdtmp);   
                     FD_SET(pthis->m_myinfo.ClientSock,&fd); 
                     maxsocket= pthis->m_myinfo.ClientSock;
                     break;
                 }
                 Sleep(100);
             }
         }

         fdtmp = fd ;
         int iResult = select(maxsocket+1,&fdtmp,NULL,NULL,&tmtmp);
         if (iResult >0)
         {
             if (FD_ISSET(pthis->m_myinfo.ClientSock,&fdtmp))
             {
                 if (pthis->m_SocketType == SocketNoCallBack)
                 {
                 }
                 // 如果注册回调函数，就调用回调函数。针对那些数据不固定，有上层分配的内存，直接调用Read进行操作
                 else if (pthis->m_SocketType ==  SocketUseCallBack)
                 {
                     if (pthis->m_CBFun!= NULL)
                     {
                         pthis->m_CBFun(SocketReadCallBack);
                     }
                 }
                 else
                 {
                     // 传输数据默认分配1K 大小的内存
                     char buffer[1024] = {0};
                     int length = recv(pthis->m_myinfo.ClientSock,buffer,SOCKET_PACKET_STRING_LENGTH,0);  
                     if (length > 0)
                     {
						 pthis->WriteMsgToBuffer(buffer,length);
                     }
                     else if(0 == length)
                     {
   
		                 closeSocket();
                         pthis->ClearClientRecvBuffer();
                     }
                     else if (-1 == length)
                     {
#ifdef Q_OS_WIN
                         int iError = WSAGetLastError();
                         if (WSAENETDOWN == iError || WSAECONNRESET == iError || WSAESHUTDOWN == iError)
                         {
                             closeSocket();
                             pthis->ClearClientRecvBuffer();
                         }                     
#endif
                     }
                     else
                     {

                     }
                 }
             }
         }
         Sleep(2);
     } 
     return NULL;
 }

 void *SocketClient::PingThread(void *lpParameter)
 {
      SocketClient *pthis = NULL;
      if (NULL != lpParameter)
      {
          pthis = (SocketClient*) lpParameter;
      }

      pthis->m_bPingThread = true;
      int iFailTime = 0;
      const int c_maxFailTimes = 5;
	  bool bflag = false;
      int iSleepTime = 0;
      while (pthis->m_bPingThread)
      { 
	  
	      if(pthis->m_myinfo.strServerIP.length () == 0 || !m_bEnablePing)
          {
              Sleep(100);
              continue;
          }
          if (iSleepTime < 20)
          {
              Sleep(50);
              iSleepTime++;
              continue;
          }
          iSleepTime = 0;
          iFailTime = 0;
		  do 
		  {
              bflag = myping(pthis->m_myinfo.strServerIP.c_str(),(iFailTime+1)*200);
              if (bflag)
              {
                  break;
              }
              else
              {
                  iFailTime ++;
                  if (iFailTime >= c_maxFailTimes)
                  {
                      break;
                  }
              }

              if (!bflag)
              {
                  break;
              }
		  } while (pthis->m_bPingThread);
   

          if (!bflag && pthis->IsClientOnline() )
          {
              iFailTime = 0;
              closeSocket();
              pthis->ClearClientRecvBuffer();
          }
      }
      return NULL;
 }

 void SocketClient::RegisterEeventCallBack( SocketCallBackFun pFun)
 {
     if (m_SocketType != SocketUseCallBack || pFun == NULL)
     {
         return ;
     }
     m_CBFun = pFun;
     return;
 }

 void SocketClient::SetSokectType(enSocketType CallBacktpye )
 {
     if (CallBacktpye >= SocketCallBackMax)
     {
         return;
     }
     m_SocketType = CallBacktpye;
 }

 bool SocketClient::IsClinetStatusChange()
 {
	 bool bflag = false;
	 if (m_bChanged)
	 {
		 bflag = true;
		 m_bChanged =false;
	 }
	 return bflag;
 }

 void SocketClient::ChangeConnectStatus( bool bconnect )
 {
	 m_myinfo.bConnected = bconnect;
	 m_bChanged = true;
 }

 void SocketClient::ClearMsgBuffer()
 {
	 m_MutexLock.Lock();
	 m_pCRingBuffer->ClearBuffer();
	 m_MutexLock.Unlock();

 }

 int SocketClient::WriteMsgToBuffer(char *pData,int DataLength )
 {
	 
	 int ilength = 0;
	 CISAPUGuard<JMutex> autoMutex(m_MutexLock);
	 ilength = m_pCRingBuffer->PushBinary(pData,DataLength);
	 return ilength;
 }

 int SocketClient::ReadMsgFromBuffer( char *pData,int DataLength ,const char* mask)
 {
	 int ilength = 0;
	 CISAPUGuard<JMutex> autoMutex(m_MutexLock);
	 if (mask != NULL && strlen(mask) != 0)
	 {
		 string strmsg;
		 bool bflag = m_pCRingBuffer->ReadTextByMask(strmsg,mask);
		 if (bflag)
		 {
			 int copylength = DataLength > strmsg.length()? strmsg.length() : DataLength;
			 strncpy(pData,strmsg.c_str(),copylength);
			 ilength = copylength;
		 }
	 }
	 else
	 {
		ilength = m_pCRingBuffer->PopBinary(pData,DataLength);
	 }
	 
	 return ilength;
 }

 int SocketClient::GetMsgDateLength()
 {
	 int isize = 0;
	 CISAPUGuard<JMutex> autoMutex(m_MutexLock);
	 isize = m_pCRingBuffer->GetMaxReadSize();
	 return isize;
 }

 int SocketClient::GetMsgPos( const char* pMsg )
 {
	 int isize = -1;
	 CISAPUGuard<JMutex> autoMutex(m_MutexLock);
	 bool bflag = m_pCRingBuffer->FindChars(pMsg,isize);
	 if (!bflag)
	 {
		 return -1;
	 }
	 return isize;
 }
