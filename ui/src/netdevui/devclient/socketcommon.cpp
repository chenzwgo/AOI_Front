#include "socketcommon.h"
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
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "trace.h"
#include "jtime.h"
#include "stringcommon.h"
#include <string>
#include "sdkcore.h"
#include "communicationtestform.h"

using namespace std;
using namespace rapidjson;

#define SOCKET_COMMON_LOG           "socketCommon.log"
#pragma comment(lib,"ws2_32.lib")
void CSocketCommon::closeSocket(int &iSocket)
{
    if(iSocket != INVALID_SOCKET)
    {
        closesocket(iSocket);
        iSocket = INVALID_SOCKET;
    }
}

void CSocketCommon::closeSocket(stSMSokcet *stSocket)
{
    if(stSocket != NULL && stSocket->iSocket != INVALID_SOCKET)
    {
        closesocket(stSocket->iSocket);
        stSocket->bConnected = false;
        stSocket->iSocket = INVALID_SOCKET;
        stSocket->IP = "";
        stSocket->iPort = -1;
    }
}

int CSocketCommon::recvex(int iSocket, char *buffer, unsigned int bufferLen, int &recvLength, int timeout)
{
    int iRet = ERR_OK;
    int itime = 0;
    int iinterval = 50;
    recvLength = 0;
    char *dataBuffer = buffer;

    do
    {
        if(isSocketReadable(iSocket))
        {
            int length = ::recv(iSocket,dataBuffer+recvLength,bufferLen -recvLength ,0);
            if(length <= 0)
            {
                if (errno!= EINTR && errno != EWOULDBLOCK && errno != EAGAIN)
                {
                    LOG_WARN_F(SOCKET_COMMON_LOG," system recv return error =%s \n",strerror(errno));
                    iRet = ERR_SOCKET_RECV_ERROR;
                    break;
                }
            }
            else
            {
                recvLength+= length;
                itime = 0;
            }

        }
        if(strchr(buffer,'#') != NULL)
            break;

        itime +=iinterval;
        Sleep(iinterval);
    }while(itime < timeout);

    if(itime >= timeout)
    {
        iRet = ERR_SOCKET_RECV_TIMEOUT;
    }

    return iRet;
}

int CSocketCommon::sendex(int iSocket, char *buffer, unsigned int bufferLen, int &sendLength, int timeout)
{
    int iRet = ERR_OK;
    int itime = 0;
    int iinterval = 50;
    sendLength = 0;
    char *dataBuffer = buffer;

    do
    {
        int length = send(iSocket,dataBuffer+sendLength,bufferLen -sendLength ,0);
        if(length <0)
        {
            if(errno != EWOULDBLOCK && errno != EAGAIN && errno != EINPROGRESS)
            {
                LOG_WARN_F(SOCKET_COMMON_LOG," System send return error =%s \n",strerror(errno));
                iRet = ERR_SOCKET_SEND_ERROR;
                break;
            }
            sendLogToUI(string("send error ")+string(strerror(errno)));
        }
        else
        {
            sendLength+= length;
        }

        if(sendLength >= bufferLen)
        {
            sendLogToUI(string("send:")+ string(buffer));
            break;
        }

        itime +=iinterval;
        Sleep(iinterval);

    }while(itime < timeout);

    if(itime >= timeout)
    {
        iRet = ERR_SOCKET_SEND_TIMEOUT;
    }

    return iRet;
}

int CSocketCommon::sendexUDP(int iSocket, const char* pDesIP, unsigned int iDesPort,  char* pBuf, unsigned int ilen,int &sendLength, int timeout )
{
	int iRet = ERR_OK;
	if (pBuf == NULL || ilen == 0 || pDesIP == NULL)
	{
		return ERR_SOCKET_SEND_ERROR;
	}
	struct sockaddr_in SockAdd;
	SockAdd.sin_family = AF_INET;
	SockAdd.sin_addr.s_addr = inet_addr(pDesIP);
	SockAdd.sin_port = htons(iDesPort);
#ifdef Q_OS_WIN
	 sendLength = sendto(iSocket, pBuf, ilen, 0, (struct sockaddr*)(&SockAdd), sizeof(SockAdd));
#else
	 sendLength = sendto(m_isock, pBuf, ilen, 0, (struct sockaddr*)(&SockAdd), sizeof(SockAdd));
#endif
	return iRet;
}


int CSocketCommon::recvexUDP(int iSocket, const char* pDesIP, unsigned int iDesPort,  char* pBuf, unsigned int ilen, int &recvLength, int timeout)
{
	if ( pBuf == NULL || ilen == 0 || pDesIP == NULL)
	{
		return ERR_SOCKET_SEND_ERROR;
	}

    int iRet = ERR_OK;
	struct sockaddr_in SockAdd;
	SockAdd.sin_family = AF_INET;
	SockAdd.sin_addr.s_addr = inet_addr(pDesIP);
	SockAdd.sin_port = htons(iDesPort);

    int itime = 0;
    int iinterval = 50;
    recvLength = 0;
    char *dataBuffer = pBuf;

    do
    {
        if(isSocketReadable(iSocket))
        {
            #ifdef Q_OS_WIN
                int addresslen = sizeof(SockAdd);
                int length = recvfrom(iSocket, (char*)(dataBuffer+recvLength), ilen -recvLength, 0, (struct sockaddr*)(&SockAdd), &addresslen);
            #else
                socklen_t addresslen = sizeof(SockAdd);
                int length = recvfrom(iSocket, (void*)(dataBuffer+recvLength), ilen -recvLength, 0, (struct sockaddr*)(&SockAdd), &addresslen);
            #endif
            if(length < 0)
            {
                if (errno!= EINTR && errno != EWOULDBLOCK && errno != EAGAIN)
                {
                    LOG_WARN_F(SOCKET_COMMON_LOG," system recv return error =%s \n",strerror(errno));
                    iRet = ERR_SOCKET_RECV_ERROR;
                    break;
                }
            }
            else
            {
                recvLength+= length;
                itime = 0;
            }

        }
        if(strchr(pBuf,'#') != NULL)
            break;

        itime +=iinterval;
        Sleep(iinterval);
    }while(itime < timeout);

    if(itime >= timeout)
    {
        iRet = ERR_SOCKET_RECV_TIMEOUT;
    }

    return iRet;
}

bool CSocketCommon::isSocketReadable(int iSocket)
{
    fd_set fd,fdtmp;
    FD_ZERO(&fd);
    FD_ZERO(&fdtmp);

    FD_SET(iSocket,&fd);
    SOCKET maxsocket= iSocket;

    struct timeval tmtmp;
    tmtmp.tv_sec = 0;
    tmtmp.tv_usec = 1000;
    fdtmp = fd;

    if(select(maxsocket+1,&fdtmp,NULL,NULL,&tmtmp) >0 && FD_ISSET(iSocket,&fd))
    {
        return true;
    }
    return false;
}

int CSocketCommon::jsonStrToPacket(char *pMsg, stPacket *packet, map<string, string> *bodyMap)
{
    int iRet = ERR_OK;
    char * pstr = strstr(pMsg, PACKET_END_CH);
    if (pstr != NULL)
    {
        int iPos = pstr - pMsg;
        pMsg[iPos] = 0;
    }
    Document djson;
    djson.Parse(pMsg);
    if (djson.HasParseError())
    {
        return ERR_PARSER_JSON;
    }

    if (djson.HasMember(PACKET_KEY_TARGET) && djson[PACKET_KEY_TARGET].IsString())
    {
        packet->target = djson[PACKET_KEY_TARGET].GetString();
    }
    else
    {
        iRet = ERR_PARSER_JSON_KEY_TARGET;
    }

    if (djson.HasMember(PACKET_KEY_TYPE) && djson[PACKET_KEY_TYPE].IsString())
    {
        packet->type = djson[PACKET_KEY_TYPE].GetString();
    }
    else
    {
        iRet = ERR_PARSER_JSON_KEY_TYPE;
    }

    if (djson.HasMember(PACKET_KEY_FUN) && djson[PACKET_KEY_FUN].IsString())
    {
        packet->fun = djson[PACKET_KEY_FUN].GetString();
    }
    else
    {
        iRet = ERR_PARSER_JSON_KEY_FUN;
    }

    if (djson.HasMember(PACKET_KEY_SN) && djson[PACKET_KEY_SN].IsString())
    {
        packet->sn = djson[PACKET_KEY_SN].GetString();
    }
    else
    {
        iRet = ERR_PARSER_JSON_KEY_SN;
    }

    if (djson.HasMember(PACKET_KEY_STATUS) && djson[PACKET_KEY_STATUS].IsString())
    {
        packet->status = djson[PACKET_KEY_STATUS].GetString();
    }
    else
    {
        iRet = ERR_PARSER_JSON_KEY_STATUS;
    }

    if (djson.HasMember(PACKET_KEY_BODY) && djson[PACKET_KEY_BODY].IsObject())
    {
        rapidjson::Value& valObj = djson[PACKET_KEY_BODY];
        rapidjson::StringBuffer sbBuf;
        rapidjson::Writer<rapidjson::StringBuffer> jWriter(sbBuf);
        valObj.Accept(jWriter);
        packet->body = std::string(sbBuf.GetString());
        if (!packet->body.empty() && valObj.IsObject() && bodyMap != NULL)
        {
            Value::MemberIterator it = valObj.MemberBegin();
            while (it != valObj.MemberEnd())
            {
                string strName, strValue;

                if (it->name.IsString())
                {
                    strName = it->name.GetString();
                }
                if (it->value.IsString())
                {
                    strValue = it->value.GetString();
                }
                if (strName.length() > 0 && strValue.length() > 0)
                {
                    bodyMap->insert(pair<string, string>(strName, strValue));
                }
                it++;
            }
        }
    }
    else
    {
        //iRet =  ERR_PARSER_JSON_KEY_BODY;
    }

    return iRet;
}

int CSocketCommon::packetToJsonStr(const stPacket *packet, char *JsonStr)
{
    string strbody = packet->body;
    if (strbody == "{}" || strbody.empty())
    {
        strbody = "\"\"";
    }

    sprintf(JsonStr, "{\"packet_target\": \"%s\",\"packet_type\": \"%s\",\"packet_fun\": \"%s\",\"packet_sn\": \"%s\",\"packet_status\": \"%s\",\"packet_body\": %s}#",
        packet->target.c_str(), packet->type.c_str(), packet->fun.c_str(),
        packet->sn.c_str(), packet->status.c_str(), strbody.c_str());
    return ERR_OK;
}

string CSocketCommon::timetoStr()
{
    char buffer[128] = { 0 };
    JTime LocalTime = JTime::CurrentLocalDateTime();
    int iHour = LocalTime.hour();
    int iMin = LocalTime.minute();
    int iSec = LocalTime.second();
    int iMSec = LocalTime.msec();
    sprintf(buffer, "%02d-%02d-%02d-%03d", iHour, iMin, iSec, iMSec);
    return string(buffer);
}

void CSocketCommon::sendLogToUI(string strmsg,int iLevel)
{
    CommunicationTestForm::iCallBackPrintLog(iLevel,QString::fromStdString(strmsg));
}

