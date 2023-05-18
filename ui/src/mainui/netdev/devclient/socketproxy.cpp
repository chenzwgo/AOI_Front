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
#include "socketclient.h"
#include "sdkcommon/reportcenter/ireportcenter.h"

using namespace std;
using namespace rapidjson;

#define SOCKET_PROXY_LOG           "socketproxy.log"
CSocketproxy::CSocketproxy( string strName)
{
    ullsn = 0; // 读取上次文件记录的值
    m_strName = strName;
    m_CSocketClient = new CSocketClient();
    m_CSocketClient->setProcessPacketCB((void*)this,&CSocketproxy::processRecvMsg);
    m_pReportCenter = (IReportCenter *)GetSDKInstance("sdkcommon", IReportCenter::IID_REPORTCENTER);

}

CSocketproxy::~CSocketproxy()
{
     stopProxy();
     if (m_CSocketClient != NULL)
     {
         delete m_CSocketClient;
         m_CSocketClient = NULL;
     }
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

int CSocketproxy::processRecvCtrlPacket(stPacket* packet)
{
    int iRet = ERR_OK;
    map<string, string> mapBody = packet->mapBody;

    packet->status = PACKET_STATUS_OK;

    if (packet->fun == PACKET_FUN_SEND_PRODUCT_RESULT)
    {
        //"{\"productSn\": \"%d\",\"res\": \"%d\"}"
        unsigned int iproductSn =0;
        unsigned int ires = 0;
        if (mapBody.find("productSn") != mapBody.end())
        {
            iproductSn = SString::strtoInt(mapBody["productSn"]);
        }
        if (mapBody.find("res") != mapBody.end())
        {
            ires = SString::strtoInt(mapBody["res"]);
        }

        if (NULL != m_pReportCenter)
        {
            m_pReportCenter->reportSendTo("mainui", PACKET_FUN_SEND_PRODUCT_RESULT,(OS_LPARAM) iproductSn,(OS_LPARAM)ires);
        }
    }
    if (packet->fun == PACKET_FUN_SEND_PRODUCT_SN)
    {
        //"{\"productSn\": \"%d\"}"
        unsigned int iproductSn =0;
        unsigned int ires = 0;
        if (mapBody.find("productSn") != mapBody.end())
        {
            iproductSn = SString::strtoInt(mapBody["productSn"]);
        }
        if (NULL != m_pReportCenter)
        {
            m_pReportCenter->reportSendTo("mainui", PACKET_FUN_SEND_PRODUCT_SN,(OS_LPARAM) iproductSn,(OS_LPARAM)ires);
        }
    }
    else if(packet->fun == PACKET_FUN_SYS_START)
    {
        unsigned int iproductSn =0;
        if (mapBody.find("productSn") != mapBody.end())
        {
            iproductSn = SString::strtoInt(mapBody["productSn"]);
        }
        if (NULL != m_pReportCenter)
        {
            m_pReportCenter->reportSendTo("mainui", "start",(OS_LPARAM) iproductSn,0);
        }
        packet->body="";
    }
    else if(packet->fun == PACKET_FUN_SYS_STOP)
    {
        if (NULL != m_pReportCenter)
        {
            m_pReportCenter->reportSendTo("mainui", "stop");
        }
        packet->body="";
    }
    else if(packet->fun == PACKET_FUN_SYS_RESET)
    {
        if (NULL != m_pReportCenter)
        {
            m_pReportCenter->reportSendTo("mainui", "reset");
        }
        packet->body="";
    }
    else if(packet->fun == PACKET_FUN_SET_IO)
    {
        packet->body = "";
        packet->status = PACKET_STATUS_ERROR;
        iMotion* piMotion = (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);

        if (piMotion == NULL || piMotion->GetIOCtrlInterface() == NULL)
        {
            packet->status = PACKET_STATUS_ERROR;
        }
        else
        {
            // "{\"IOName\": \"%s\",\"open\": \"%d\"}"
            IIOCtrlInterface* pIIO = piMotion->GetIOCtrlInterface();
            string strIOName;
            bool bOpen = false;

            if (mapBody.find("IOName") != mapBody.end())
            {
                strIOName = mapBody["name"];
            }
            if (mapBody.find("open") != mapBody.end())
            {
                bOpen = SString::strtoBool(mapBody["open"]);
            }
            bool bRet = pIIO->writeio(strIOName.c_str(),bOpen);
            if (bRet)
            {
                packet->status = PACKET_STATUS_OK;
                packet->body;
            }
        }
    }
    else if(packet->fun == PACKET_FUN_GET_IO)
    {
        packet->body = "";
        packet->status = PACKET_STATUS_ERROR;
        iMotion* piMotion = (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);

        if (piMotion == NULL || piMotion->GetIOCtrlInterface() == NULL)
        {
            packet->status = PACKET_STATUS_ERROR;
        }
        else
        {
            // "{\"IOName\": \"%s\"}"
            IIOCtrlInterface* pIIO = piMotion->GetIOCtrlInterface();
            string strIOName;
            bool bOpen = false;

            if (mapBody.find("IOName") != mapBody.end())
            {
                strIOName = mapBody["name"];
            }
            bool bRet = pIIO->readio(strIOName.c_str(),bOpen);
            if (bRet)
            {
                packet->status = PACKET_STATUS_OK;
                char bodybuf[256] = { 0 };
                sprintf(bodybuf, "{\"IOName\":\"%s\",\"Open\":\"%d\"}", strIOName.c_str(),SString::IntToStr(bOpen));
                packet->body = bodybuf;
            }
        }
    }
    else if (packet->fun == PACKET_FUN_HEARTBEAT)
    {
        packet->body = "";
        packet->status = PACKET_STATUS_OK;
        bool bRet = true;// = sys_start();
        if (bRet)
        {
            packet->status = PACKET_STATUS_OK;
            packet->body;
        }
    }
    else if (packet->fun == "Test")
    {
        packet->body = "";
        packet->status = PACKET_STATUS_OK;
        bool bRet = true;// = sys_start();
        if (bRet)
        {
            packet->status = PACKET_STATUS_OK;
            packet->body;
        }
    }
    else
    {
        packet->status =PACKET_STATUS_FUN_NOEXIST;
    }
    return ERR_OK;
}


int CSocketproxy::stopProxy()
{
    m_CSocketClient->disConnect();
    return  ERR_OK;
}

int CSocketproxy::startProxy()
{
    m_CSocketClient->connectServer();
    return  ERR_OK;
}

int CSocketproxy::sendAckPacket(stPacket* pPack)
{
    pPack->target = m_strName;
    pPack->type = PACKET_TYPE_ACK;

    char buffer[1024] = { 0 };
    CSocketCommon::packetToJsonStr(pPack, buffer);
    buffer[sizeof(buffer) - 1] = 0;
    int length = 0;
    int iRet = m_CSocketClient->sendMsg(buffer,strlen(buffer),length);
    if (iRet < 0)
    {
        LOG_WARN_F(SOCKET_PROXY_LOG, "sendAckPacket  return =  (%s) \n", errorCodeToStr(iRet));
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
	if (!m_CSocketClient->isConnected())
	{
		m_CSocketClient->connectServer();
	}
    pPack->target = m_strName;
    pPack->type = PACKET_TYPE_REQUEST;
    pPack->sn = timetoStr();
    pPack->status = PACKET_STATUS_OK;

    char buffer[1024] = { 0 };
    CSocketCommon::packetToJsonStr(pPack, buffer);
    buffer[sizeof(buffer) - 1] = 0;
    int length = 0;
    int iRet =m_CSocketClient->sendMsg(buffer,strlen(buffer),length);
    if (iRet < 0)
    {
        LOG_WARN_F(SOCKET_PROXY_LOG, "sendRequestPacket  return =  (%s) \n", errorCodeToStr(iRet));
        return iRet;
    }
    else
    {
        LOG_INFO_F(SOCKET_PROXY_LOG, "sendRequestPacket(%d):  (%s) \n", iRet,buffer);
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

int CSocketproxy::sendInquirePacket( stPacket* pPack )
{
    pPack->target = m_strName;
    pPack->type = PACKET_TYPE_INQUIRE;
    pPack->sn = timetoStr();
    pPack->status = PACKET_STATUS_OK;

    char buffer[1024] = { 0 };
    CSocketCommon::packetToJsonStr(pPack, buffer);
    buffer[sizeof(buffer) - 1] = 0;
    int length = 0;
    int iRet = m_CSocketClient->sendMsg(buffer,strlen(buffer),length);
    if (iRet < 0)
    {
        LOG_WARN_F(SOCKET_PROXY_LOG, "sendInquirePacket  return =  (%s) \n", errorCodeToStr(iRet));
        return iRet;
    }
    else
    {
        LOG_INFO_F(SOCKET_PROXY_LOG, "sendInquirePacket(%d):  (%s) \n", iRet,buffer);
    }
    return ERR_OK;
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
    int iRet = m_CSocketClient->recvMsg(buffer,sizeof(buffer),iRecvLength,3000);
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
    iRet = m_CSocketClient->sendMsg(buffer2, strlen(buffer2), length);
    if (iRet < 0)
    {
        LOG_WARN_F(SOCKET_PROXY_LOG, "waitRegisterPkg sendex return =  (%s) \n", errorCodeToStr(iRet));
        return iRet;
    }

    return ERR_OK;
}

int CSocketproxy::processRecvMsg(void * param,char *pCtrlMsg)
{
    stPacket* packet = new stPacket();
    CSocketproxy* pThis = (CSocketproxy*)(param);
    int iRet = CSocketCommon::jsonStrToPacket(pCtrlMsg, packet, &packet->mapBody);
    if (iRet < 0)
    {
        LOG_WARN_F(SOCKET_PROXY_LOG, "processCtrlPacket jsonStrToPacket return =  (%s) \n", errorCodeToStr(iRet));
        delete packet;
        return iRet;
    }
    if (packet->type == PACKET_TYPE_REQUEST)
    {
        iRet = pThis->processRecvCtrlPacket(packet);
        iRet = pThis->sendAckPacket(packet);
        delete packet;
        return iRet;
    }
    else if (packet->type == PACKET_TYPE_ACK)
    {
        pThis->pushBackAckPacket(packet->sn, packet);
    }
    return ERR_OK;
}

CSocketClient *CSocketproxy::getSocketClient()
{
     return m_CSocketClient;
}

int CSocketproxy::sendRequest( string strFun,string strBody /*=""*/ )
{
    stPacket packet;
    packet.fun = strFun;
    if (strBody.length() >0)
    {
        char bodybuf[256] = { 0 };
        sprintf(bodybuf, "{%s}", strBody.c_str());
        packet.body = bodybuf;
    }
    int iRet = sendRequestPacket(&packet);
    if (iRet != ERR_OK)
    {
        return iRet;
    }
    iRet = waitAckPacket(packet.sn);

    return iRet;
}

int CSocketproxy::sendInquire( string strSN,bool &bProcessed )
{
    bProcessed  = false;
    stPacket packet;
    packet.fun = PACKET_FUN_INQUIRE_PACKET;

    char bodybuf[256] = { 0 };
    sprintf(bodybuf, "{\"Sn\": \"%s\"}", strSN.c_str());
    packet.body = bodybuf;

    int iRet = sendInquirePacket(&packet);
    if (iRet != ERR_OK)
    {
        return iRet;
    }
    map<string, string> strBody;
    iRet = waitAckPacket(packet.sn,&strBody);
    if(iRet == ERR_OK)
    {
        if (strBody.find("processed") != strBody.end())
        {
            bProcessed = SString::strtoBool(strBody["processed"]);
        }
    }
    return iRet;
}

int CSocketproxy::sendSynPrdunctSN(unsigned int isn)
{
    char bodybuf[256] = { 0 };
    sprintf(bodybuf, "\"productSn\": \"%d\"", isn);
    return sendRequest(PACKET_FUN_SEND_PRODUCT_SN,bodybuf);
}

int CSocketproxy::sendProductSNRes(unsigned int isn, int iOk)
{
    char bodybuf[256] = { 0 };
    sprintf(bodybuf, "\"productSn\": \"%d\",\"res\": \"%d\"", isn,iOk);
    return sendRequest(PACKET_FUN_SEND_PRODUCT_RESULT,bodybuf);

}

int CSocketproxy::sendSysStart(unsigned int isn)
{
    char bodybuf[256] = { 0 };
    sprintf(bodybuf, "\"productSn\": \"%d\"", isn);
    return sendRequest(PACKET_FUN_SYS_START);
}

int CSocketproxy::sendSysStop()
{
    return sendRequest(PACKET_FUN_SYS_STOP);
}

int CSocketproxy::sendSysReset()
{
    return sendRequest(PACKET_FUN_SYS_RESET);
}
