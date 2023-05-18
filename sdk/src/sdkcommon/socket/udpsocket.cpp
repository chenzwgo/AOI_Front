#include "udpsocket.h"
#ifdef Q_OS_WIN
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#endif

#define INVALID_SOCKET -1
#define SOCKET_ERROR  -1
#define SOCKET_PACKET_LENGTH 1024

udpsocket::udpsocket()
{
    m_binit = false;
    m_isock = -1;
}

udpsocket::~udpsocket()
{
    Uninit();
}

bool udpsocket::Init(const char* pIP, unsigned int iport)
{
    if(m_binit)
    {
        return true;
    }
    m_strIP = pIP;
    m_iPort = iport;
    
    int tmpResult;
    m_isock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if( -1 == m_isock)
    {
        return false;
    }

    struct sockaddr_in SockAdd;
    SockAdd.sin_family = AF_INET;
    //  Ìî³äIPµØÖ·
    if (pIP == NULL || strlen(pIP) == 0)
    {
        SockAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        SockAdd.sin_addr.s_addr =inet_addr(pIP);
    }
    // Ìî³ä ¶Ë¿ÚºÅ
    SockAdd.sin_port =  htons(iport);


#ifdef Q_OS_WIN
    u_long iMode = 1;
    ioctlsocket(m_isock, FIONBIO, &iMode);
#else
    int flags = fcntl(m_isock, F_GETFL, 0);
    fcntl(m_isock, F_SETFL, flags|O_NONBLOCK);
#endif

    int iSendBufSize = SOCKET_PACKET_LENGTH;
    int iOptLen = sizeof(iSendBufSize);
    int iRet = setsockopt(m_isock, SOL_SOCKET, SO_SNDBUF, (char *)&iSendBufSize, iOptLen);

    int iRcvBufSize = SOCKET_PACKET_LENGTH;
    iOptLen = sizeof(iRcvBufSize);
    iRet = setsockopt(m_isock, SOL_SOCKET, SO_RCVBUF, (char *)&iRcvBufSize, iOptLen);

    const int yes = 1;
    setsockopt(m_isock,SOL_SOCKET,SO_REUSEADDR,(const char*)&yes,sizeof(yes));

    tmpResult = bind(m_isock,(struct sockaddr*)(&SockAdd), sizeof(SockAdd));
    if (tmpResult == -1)
    {
#ifdef Q_OS_WIN
        closesocket(m_isock);
#else
        close(m_isock);
#endif
        m_isock = -1;
        return false;
    }

    m_binit = true;
    return true;
}

int udpsocket::SendMsg(const char* pDesIP,unsigned int iDesPort,const char* pBuf,unsigned int ilen)
{
    if(!m_binit || m_isock == -1 || pBuf == NULL || ilen == 0 || pDesIP == NULL)
    {
        return -1;
    }
    struct sockaddr_in SockAdd;
    SockAdd.sin_family = AF_INET;
    SockAdd.sin_addr.s_addr =inet_addr(pDesIP);
    SockAdd.sin_port =  htons(iDesPort);
#ifdef Q_OS_WIN
    int isize = sendto(m_isock, pBuf, ilen, 0, (struct sockaddr*)(&SockAdd), sizeof(SockAdd));
#else
    ssize_t isize = sendto(m_isock, pBuf,  ilen, 0,(struct sockaddr*)(&SockAdd),sizeof(SockAdd));
#endif
    return isize;
}


int udpsocket::RecvMsg(const char* pDesIP,unsigned int iDesPort,const char* pBuf,unsigned int ilen)
{
    if(!m_binit || m_isock == -1 || pBuf == NULL || ilen == 0 || pDesIP == NULL)
    {
        return -1;
    }
    struct sockaddr_in SockAdd;
    SockAdd.sin_family = AF_INET; 
    SockAdd.sin_addr.s_addr =inet_addr(pDesIP);
    SockAdd.sin_port =  htons(iDesPort);
#ifdef Q_OS_WIN
    int addresslen = sizeof(SockAdd);
    int isize = recvfrom(m_isock, (char*)&pBuf, ilen, 0, (struct sockaddr*)(&SockAdd), &addresslen);
#else
    socklen_t addresslen = sizeof(SockAdd);
    ssize_t isize = recvfrom(m_isock, (void*)pBuf,  ilen, 0,(struct sockaddr*)(&SockAdd),&addresslen);
#endif

    return isize;
}

bool udpsocket::Uninit()
{
    if(!m_binit)
    {
        return true;
    }

    if(m_isock != -1)
    {
#ifdef Q_OS_WIN
        closesocket(m_isock);
#else
        close(m_isock);
#endif
        m_isock = -1;
    }
    m_binit = false;
    return true;
}

void udpsocket::getIPandPort( string& strIp,unsigned int & iPort )
{
    strIp = m_strIP;
    iPort = m_iPort;
}


