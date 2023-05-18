#include "socketproxy.h"
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
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "trace.h"
#include "jtime.h"
#include "stringcommon.h"
#include <string>
#include "sdkcore.h"
#include "socketcommon.h"
#include "motion/iioctrlinterface.h"
#include "motion/imotion.h"
#include "callsdk.h"
#include "uidefine.h"
using namespace std;
using namespace rapidjson;

#define SOCKET_PROXY_LOG           (m_strID+"_socketproxy.log").c_str()
CSocketproxy::CSocketproxy(string strID):m_strID(strID)
{
    m_hThreadHandle = new JThreadTP<CSocketproxy, void *,int >();
    m_bThreadRunning = false;
    m_pCRingBuffer = new CRingBuffer(1024 * 4); // 4K buffer
    m_pPacketCB = NULL;
    ullsn = 0; // 读取上次文件记录的值
}

CSocketproxy::~CSocketproxy()
{
     stopProxy();
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
     CSocketCommon::closeSocket(&m_Socket);
}


int CSocketproxy::sendHeartBeat()
{
	stPacket packet;
	packet.fun = PACKET_FUN_HEARTBEAT;
    int iRet = sendRequestPacket(&packet);
	if (iRet != ERR_OK)
	{
		return iRet;
	}
    iRet = waitAckPacket(packet.sn);
    return iRet;
}

void CSocketproxy::setProcessPacketCB(FunPacketCB pPacketCB)
{
    if(pPacketCB != NULL)
    {
        m_pPacketCB = pPacketCB;
    }
}

int CSocketproxy::processRecvCtrlPacket(stPacket* packet)
{
    int iRet = ERR_OK;
    if(m_pPacketCB != NULL)
    {
        iRet = (*m_pPacketCB)(packet);
    }
    else
    {
        packet->status =PACKET_STATUS_FUN_NOEXIST;
    }

    return iRet;
}

void CSocketproxy::setSocket(stSMSokcet &stSokcet)
{
    if (m_Socket.iSocket != stSokcet.iSocket)
    {
        CSocketCommon::closeSocket(&m_Socket);
        stopProxy();
    }
    m_Socket = stSokcet;
}

stSMSokcet *CSocketproxy::getSocket()
{
    return &m_Socket;
}

int CSocketproxy::stopProxy()
{
    //CSocketCommon::closeSocket(&m_Socket);
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

    return  ERR_OK;
}

int CSocketproxy::startProxy()
{
    if(m_hThreadHandle  != NULL && !m_hThreadHandle->IsRunning())
    {
        m_hThreadHandle->StartThread(this,&CSocketproxy::RecvThread,(void*)(&m_Socket));
    }
    return  ERR_OK;
}

int CSocketproxy::RecvThread(void *param)
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
               LOG_INFO_F(SOCKET_PROXY_LOG, "RecvThread recv = %s",buffer);
               m_pCRingBuffer->PushBinary(buffer, length);
               int iPos = 0;
               bool bflag = m_pCRingBuffer->FindChar('#', iPos);
               while (bflag)
               {
                   char *pkgBuf = new char[iPos + 2];
                   memset(pkgBuf, 0, iPos + 2);
                   m_pCRingBuffer->PopBinary(pkgBuf, iPos + 1);
                   processRecvMsg(pkgBuf);
                   delete pkgBuf;
                   bflag = m_pCRingBuffer->FindChar('#', iPos);
               }
           }
           else if (0 == length)
           {
               CSocketCommon::closeSocket(pstSocket);
               CSocketCommon::sendLogToUI("recv error return 0", UILOG_ERR);
               LOG_WARN_F(SOCKET_PROXY_LOG, "RecvThread recv length = 0");
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
                   LOG_WARN_F(SOCKET_PROXY_LOG, "RecvThread recv error (-1 )= %d", iError);
                   return ERR_SOCKET_RECV_ERROR;
               }                     
           }
       }
       Sleep(10);
    }
    LOG_INFO_F(SOCKET_PROXY_LOG,"RecvThread Exit \n");

    return  ERR_OK;
}

int CSocketproxy::sendAckPacket(stPacket* pPack)
{
    pPack->target = PACKET_TARGET_PC1;
    pPack->type = PACKET_TYPE_ACK;

    char buffer[1024] = { 0 };
    CSocketCommon::packetToJsonStr(pPack, buffer);
    buffer[sizeof(buffer) - 1] = 0;
    int length = 0;
    int iRet = CSocketCommon::sendex(m_Socket.iSocket,buffer,strlen(buffer),length);
    if (iRet < 0)
    {
        LOG_WARN_F(SOCKET_PROXY_LOG, "sendAckPacket  return =  %d (%s) \n", iRet,errorCodeToStr(iRet));
        return iRet;
    }
    else
    {
        LOG_INFO_F(SOCKET_PROXY_LOG, "sendAckPacket(%d):  (%s) \n", iRet,buffer);
    }
    return ERR_OK;
}

int CSocketproxy::sendRequestPacket(stPacket* pPack)
{
    pPack->target = PACKET_TARGET_PC1;
    pPack->type = PACKET_TYPE_REQUEST;
    pPack->sn = timetoStr();
    pPack->status = PACKET_STATUS_OK;

    char buffer[1024] = { 0 };
    CSocketCommon::packetToJsonStr(pPack, buffer);
    buffer[sizeof(buffer) - 1] = 0;
    int length = 0;
    int iRet = CSocketCommon::sendex(m_Socket.iSocket,buffer,strlen(buffer),length);
    if (iRet < 0)
    {
        LOG_WARN_F(SOCKET_PROXY_LOG, "sendRequestPacket  return =  (%s) \n", errorCodeToStr(iRet));
        return iRet;
    }
    else
    {
        LOG_INFO_F(SOCKET_PROXY_LOG, "sendAckPacket(%d):  (%s) \n", iRet,buffer);
    }
    return ERR_OK;
}

int CSocketproxy::waitAckPacket(string sn, map<string, string>* pstrBody /*= NULL*/, int timeout /*= 500*/)
{
    stPacket* pPacket = NULL;
    int itime = 0;
    int iinterval = 50;
    int iRet = ERR_OK;

    do
    {
        if (isAckPacketExist(sn))
        {
            pPacket = popAckPacket(sn);
            break;
        }

        itime += iinterval;
        Sleep(iinterval);
    } while (itime < timeout);

    if (pPacket != NULL)
    {
        if (pPacket->status != PACKET_STATUS_OK)
        {
            iRet = ERR_PACKET_ACK_ERROR;
        }
        else
        {
            if (pstrBody != NULL)
            {
                *pstrBody = pPacket->mapBody;
            }
        }
        delete pPacket;
    }
    else
    {
        pushBackAckPacket(sn, NULL);
        iRet = ERR_PACKET_ACK_TIMEOUT;
    }

    return  iRet;
}

void CSocketproxy::pushBackAckPacket(string sn, stPacket* pPack)
{
    CISAPUGuard<JMutex> autoLock(m_AckPacketLock);
    map<string, stPacket*>::iterator it = m_mapAckPacket.find(sn);
    if (it == m_mapAckPacket.end())
    {
        m_mapAckPacket.insert(pair<string, stPacket *>(sn, pPack));
    }
    else
    {
        if (it->second != NULL)
        {
            delete it->second;
        }
        m_mapAckPacket.erase(it);
        if (pPack != NULL)
        {
            delete pPack;
        }
    }
}

bool CSocketproxy::isAckPacketExist(string Sn)
{
    CISAPUGuard<JMutex> autoLock(m_AckPacketLock);
    return m_mapAckPacket.find(Sn) != m_mapAckPacket.end();
}

stPacket* CSocketproxy::popAckPacket(string Sn)
{
    CISAPUGuard<JMutex> autoLock(m_AckPacketLock);
    stPacket *pstPacket = NULL;
    map<string, stPacket*>::iterator it = m_mapAckPacket.find(Sn);
    if (it == m_mapAckPacket.end())
    {
        return NULL;
    }
    else
    {
        pstPacket = it->second;
        m_mapAckPacket.erase(it);
    }
    return pstPacket;
}

string CSocketproxy::timetoStr()
{
    CISAPUGuard<JMutex> autoLock(m_snLock);
    char buffer[128] = { 0 };
    JTime LocalTime = JTime::CurrentLocalDateTime();
    int iHour = LocalTime.hour();
    int iMin = LocalTime.minute();
    int iSec = LocalTime.second();
    int iMSec = LocalTime.msec();
    sprintf(buffer, "%02d-%02d-%02d-%03d-%d", iHour, iMin, iSec, iMSec, ullsn);
    ullsn++;
    return string(buffer);
}

int CSocketproxy::waitRegisterPkg(stSMSokcet* pstSMSokcet)
{
    int iSocket =(int) pstSMSokcet->iSocket;
    char buffer[1024] = {0};
    int iRecvLength =0;
    int iRet = CSocketCommon::recvex(iSocket,buffer,sizeof(buffer),iRecvLength,3000);
    if(iRet <0 )
    {
        LOG_WARN_F(SOCKET_PROXY_LOG,"dispatchSocket  recvex return (%s) \n",errorCodeToStr(iRet));
        return iRet;
    }
    LOG_WARN_F(SOCKET_PROXY_LOG,"dispatchSocket  recvex  buffer =  %s \n",buffer);
    stPacket packet;
    iRet = CSocketCommon::jsonStrToPacket(buffer,&packet);
    if(iRet <0)
    {
        LOG_WARN_F(SOCKET_PROXY_LOG,"dispatchSocket  parserPacket return (%s) \n",errorCodeToStr(iRet));
        return iRet;
    }
    else
    {
        pstSMSokcet->enSocetType = InvalideSocket;
        return ERR_SOCKET_NOT_REGISTER_MSG;
    }

    packet.target = PACKET_TARGET_SERVER;
    packet.type = PACKET_TYPE_ACK;
    packet.status = PACKET_STATUS_OK;

    char buffer2[1024] = { 0 };
    CSocketCommon::packetToJsonStr(&packet, buffer2);
    buffer2[sizeof(buffer2) - 1] = 0;
    int length = 0;
    iRet = CSocketCommon::sendex(iSocket, buffer2, strlen(buffer2), length);
    if (iRet < 0)
    {
        LOG_WARN_F(SOCKET_PROXY_LOG, "waitRegisterPkg sendex return =  (%s) \n", errorCodeToStr(iRet));
        return iRet;
    }

    return ERR_OK;
}

int CSocketproxy::processRecvMsg(char *pCtrlMsg)
{
    stPacket* packet = new stPacket();
    int iRet = CSocketCommon::jsonStrToPacket(pCtrlMsg, packet, &packet->mapBody);
    if (iRet < 0)
    {
        LOG_WARN_F(SOCKET_PROXY_LOG, "processCtrlPacket jsonStrToPacket return =  (%s) \n", errorCodeToStr(iRet));
        delete packet;
        return iRet;
    }
    if (packet->type == PACKET_TYPE_REQUEST)
    {
        iRet = processRecvCtrlPacket(packet);
        if (iRet != ERR_OK)
        {
            LOG_WARN_F(SOCKET_PROXY_LOG, "processCtrlPacket  return =  %d (%s) \n", iRet,errorCodeToStr(iRet));
        }
       
        iRet = sendAckPacket(packet);
        if (iRet != ERR_OK)
        {
            LOG_WARN_F(SOCKET_PROXY_LOG, "sendAckPacket  return = %d (%s) \n", iRet,errorCodeToStr(iRet));
        }
        stPacket* Procpacket = new stPacket();
        *Procpacket=*packet;
        pushBackProcPacket(Procpacket);

        delete packet;
        return iRet;
    }
    else if (packet->type == PACKET_TYPE_ACK)
    {
        pushBackAckPacket(packet->sn, packet);
    }
    else if (packet->type == PACKET_TYPE_INQUIRE)
    {
        iRet = processRecvInquirePacket(packet);
        if (iRet != ERR_OK)
        {
            LOG_WARN_F(SOCKET_PROXY_LOG, "processRecvInquirePacket  return =  %d (%s) \n", iRet,errorCodeToStr(iRet));
        }

        iRet = sendAckPacket(packet);
        if (iRet != ERR_OK)
        {
            LOG_WARN_F(SOCKET_PROXY_LOG, "sendAckPacket  return = %d (%s) \n", iRet,errorCodeToStr(iRet));
        }
        delete packet;
    }
    return ERR_OK;
}

int CSocketproxy::processRecvInquirePacket( stPacket* packet )
{
    int iRet = ERR_OK;
    map<string, string> mapBody = packet->mapBody;

    packet->status = PACKET_STATUS_OK;

    if(packet->fun == PACKET_FUN_INQUIRE_PACKET)
    {
        packet->body = "";
        packet->status = PACKET_STATUS_ERROR;

        string strPckSn;
        if (mapBody.find("Sn") != mapBody.end())
        {
            strPckSn = mapBody["Sn"];
        }

        bool bRet  = isProcPacketExist(strPckSn);
        packet->status = PACKET_STATUS_OK;
        char bodybuf[256] = { 0 };
        sprintf(bodybuf, "{\"Sn\":\"%s\",\"processed\":\"%d\"}", strPckSn.c_str(),bRet);
        packet->body = bodybuf;
    }
    else
    {
        packet->status =PACKET_STATUS_FUN_NOEXIST;
    }
    return iRet;
}

void CSocketproxy::pushBackProcPacket( stPacket* pPack )
{
    CISAPUGuard<JMutex> autoLock(m_ProcPacketLock);
    int iSize = m_listProcPacket.size();
    if (iSize >100 ) // 预留最近的 100 数据包的空间
    {
        stPacket* pPacktmp = m_listProcPacket.front();
        m_listProcPacket.pop_front();
        if (pPacktmp != NULL)
        {
            delete pPacktmp;
        }
    }
    m_listProcPacket.push_back(pPack);
    ;
}

bool CSocketproxy::isProcPacketExist( string Sn )
{
    CISAPUGuard<JMutex> autoLock(m_ProcPacketLock);
    list<stPacket*>::iterator it = m_listProcPacket.begin();
    while(it != m_listProcPacket.end())
    {
        if ((*it)!= NULL && (*it)->sn == Sn)
        {
            return true;
        }
        it++;
    }
    return false;
}
