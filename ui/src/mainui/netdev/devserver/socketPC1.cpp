#include "socketPC1.h"
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
#include "workflow/utils.h"


#define SOCKET_CLIENT_LOG           (m_strID+"_socketClient.log").c_str()

CSocketClientPC1::CSocketClientPC1(string strID):m_strID(strID)
{
    m_pCSocketproxy = new CSocketproxy(strID); // 从服务器获取
    m_pstSokcet = m_pCSocketproxy->getSocket();
    m_pCSocketproxy->setProcessPacketCB(&CSocketClientPC1::processRecvCtrlPacket);

}

CSocketClientPC1::~CSocketClientPC1()
{
    if (m_pCSocketproxy != NULL)
    {
        delete m_pCSocketproxy;
        m_pCSocketproxy = NULL;
    }
}

bool CSocketClientPC1::isConnected()
{
    return m_pstSokcet->bConnected;
}

void CSocketClientPC1::disConneced()
{
    CSocketCommon::closeSocket(m_pstSokcet);
}

int CSocketClientPC1::sendHeartBeat()
{
    stPacket packet;
    packet.fun = PACKET_FUN_HEARTBEAT;
    int iRet = m_pCSocketproxy->sendRequestPacket(&packet);
    if (iRet != ERR_OK)
    {
        return iRet;
    }
    iRet = m_pCSocketproxy->waitAckPacket(packet.sn);
    return iRet;
}

int CSocketClientPC1::getIOStatus(const char *IOName, bool &bOpen)
{
    stPacket packet;
    packet.fun = PACKET_FUN_GET_IO;
    char bodybuf[256] = { 0 };
    sprintf(bodybuf, "{\"IOName\": \"%s\"}", IOName);
    packet.body = bodybuf;
    int iRet = m_pCSocketproxy->sendRequestPacket(&packet);
    if (iRet != ERR_OK)
    {
        return iRet;
    }
    map<string, string> strBody;
    iRet = m_pCSocketproxy->waitAckPacket(packet.sn,&strBody);
    if(iRet == ERR_OK)
    {
        if (strBody.find("open") != strBody.end())
        {
            bOpen = SString::strtoBool(strBody["open"]);
        }
    }
    return iRet;
}

int CSocketClientPC1::setIO(const char *IOName, bool &bOpen)
{
    stPacket packet;
    packet.fun = PACKET_FUN_GET_IO;
    char bodybuf[256] = { 0 };
    sprintf(bodybuf, "{\"IOName\": \"%s\",\"open\": \"%d\"}", IOName,bOpen);
    packet.body = bodybuf;
    int iRet = m_pCSocketproxy->sendRequestPacket(&packet);
    if (iRet != ERR_OK)
    {
        return iRet;
    }
    iRet = m_pCSocketproxy->waitAckPacket(packet.sn);
    return iRet;
}

void CSocketClientPC1::setSocket(stSMSokcet &stSokcet)
{
    if(m_pCSocketproxy != NULL)
    {
        m_pCSocketproxy->setSocket(stSokcet);
        m_pCSocketproxy->startProxy();
    }
}

int CSocketClientPC1::sendRequest( string strFun,string strBody /*=""*/ )
{
    stPacket packet;
    packet.fun = strFun;
    if (strBody.length() >0)
    {
        char bodybuf[256] = { 0 };
        sprintf(bodybuf, "{%s}", strBody.c_str());
        packet.body = bodybuf;
    }
    int iRet = m_pCSocketproxy->sendRequestPacket(&packet);
    if (iRet != ERR_OK)
    {
        return iRet;
    }
    iRet = m_pCSocketproxy->waitAckPacket(packet.sn);
    return iRet;
}

int CSocketClientPC1::processRecvCtrlPacket(stPacket *packet)
{
    int iRet = ERR_OK;
    map<string, string> mapBody = packet->mapBody;

    packet->status = PACKET_STATUS_OK;

    if(packet->fun == PACKET_FUN_SET_IO)
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
                strIOName = mapBody["IOName"];
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
                strIOName = mapBody["IOName"];
            }
            bool bRet = pIIO->readio(strIOName.c_str(),bOpen);
            if (bRet)
            {
                packet->status = PACKET_STATUS_OK;
                char bodybuf[256] = { 0 };
                sprintf(bodybuf, "{\"IOName\":\"%s\",\"open\":\"%d\"}", strIOName.c_str(),bOpen);
                packet->body = bodybuf;
            }
        }
    }
	//else if(packet->fun == PACKET_FUN_GET_BATCH_IO)
	//{
	//	packet->body = "";
	//	packet->status = PACKET_STATUS_ERROR;
	//	iMotion* piMotion = (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);

	//	if (piMotion == NULL || piMotion->GetIOCtrlInterface() == NULL)
	//	{
	//		packet->status = PACKET_STATUS_ERROR;
	//	}
	//	else
	//	{
	//		// "{\"IOName\": \"%s\"}"
	//		IIOCtrlInterface* pIIO = piMotion->GetIOCtrlInterface();
	//		string strIOName;
	//		bool bOpen = false;

	//		//遍历map
	//		map<string, string>::reverse_iterator iter;
	//		char bodybuf[256] = { 0 };
	//		string strText = "";
	//		
	//		for (iter = mapBody.rbegin(); iter != mapBody.rend();iter++)
	//		{
	//			
	//			strIOName = iter->first;
	//			bool bRet = pIIO->readio(strIOName.c_str(),bOpen);
	//			if (bRet)
	//			{
	//				packet->status = PACKET_STATUS_OK;
	//				memset(bodybuf,0,256);
	//				sprintf(bodybuf, "\"%s\":\"%d\",",strIOName.c_str(),bOpen);
	//				strText += bodybuf;
	//			}
	//		}
	//		if (strText != "")
	//		{
	//			strText = strText.substr(0, strText.length()-1);
	//			strText ="{"+strText+"}";
	//			packet->body = strText;
	//		}


	//	}
	//}
    else if (packet->fun == PACKET_FUN_SYS_START)
    {
        packet->body = "";
        packet->status = PACKET_STATUS_ERROR;
        bool bRet;// = sys_start();
        if (bRet)
        {
            packet->status = PACKET_STATUS_OK;
            packet->body;
        }

    }
	else if (packet->fun == PACKET_FUN_HEARTBEAT)
	{
		packet->body = "";
		packet->status = PACKET_STATUS_OK;
		bool bRet;// = sys_start();
		if (bRet)
		{
			packet->status = PACKET_STATUS_OK;
			packet->body;
		}

	}
	else if(packet->fun == PACKET_FUN_STATUS)
	{
		string status;
		if (mapBody.find("key") != mapBody.end())
        {
            status = mapBody["key"];
        }
    }
    else
    {
        packet->status =PACKET_STATUS_FUN_NOEXIST;
    }
    return ERR_OK;
}
