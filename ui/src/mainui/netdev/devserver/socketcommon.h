#ifndef CSOCKET_COMMON_H
#define CSOCKET_COMMON_H
#include "socketerrcode.h"
#include "packetdef.h"
#include "jmutex.h"
#include <map>
#include <string>
using namespace std;

typedef enum enSocketIndex
{
    InvalideSocket = -1,
    AllSocket = 0,
    CtrlSocket,
    DataSocket,
}enSocketIndex;

typedef struct serverCfg
{
public:
    serverCfg() {}
    serverCfg(const serverCfg& other)
    {
        serverName = other.serverName;
        serverIP = other.serverIP;
        serverPort = other.serverPort;
    }
    serverCfg&  operator =(const serverCfg& other)
    {
        serverName = other.serverName;
        serverIP = other.serverIP;
        serverPort = other.serverPort;
        return *this;
    }
    string serverName;
    string serverIP;
    unsigned  int serverPort;
}stserverCfg;


typedef struct clientCfg
{
public:
    clientCfg() {}
    clientCfg(const clientCfg& other)
    {
        ClientName = other.ClientName;
        ClientIP = other.ClientIP;
        ClientPort = other.ClientPort;
    }
    clientCfg&  operator =(const clientCfg& other)
    {
        ClientName = other.ClientName;
        ClientIP = other.ClientIP;
        ClientPort = other.ClientPort;
        return *this;
    }
    bool  operator ==(const clientCfg& other)
    {
        return ClientIP == other.ClientIP && ClientPort == other.ClientPort;
    }
    string ClientName;
    string ClientIP;
    unsigned  int ClientPort;
}stclientCfg;


typedef struct SMSokcet
{
    SMSokcet()
    {
        iSocket = INVALID_SOCKET;
        enSocetType = InvalideSocket;
        bConnected = false;
        IP = "";
        iPort = 0;
    }

    ~SMSokcet()
    {
        iSocket = INVALID_SOCKET;
        enSocetType = InvalideSocket;
        bConnected = false;
        IP = "";
        iPort = 0;
    }

    SMSokcet(const SMSokcet& other)
    {
        iSocket = other.iSocket;
        bConnected = other.bConnected;
        enSocetType = other.enSocetType;
        IP= other.IP;
        iPort= other.iPort;
    }
    SMSokcet& operator = (const SMSokcet& other)
    {
        iSocket = other.iSocket;
        bConnected = other.bConnected;
        enSocetType = other.enSocetType;
        IP= other.IP;
        iPort= other.iPort;
        return *this;
    }

    bool operator !=(const SMSokcet& other)
    {
        if(&other == this)
        {
            return false;
        }
        return other.iSocket != iSocket;
    }

    bool operator ==(const SMSokcet& other)
    {
        if(&other == this)
        {
            return true;
        }
        return other.iSocket == iSocket;
    }

    int iSocket;
    enSocketIndex enSocetType;
    bool bConnected;
    string IP;
    unsigned int iPort;
}stSMSokcet;

class CSocketCommon
{
public:
    static void closeSocket(int& iSocket);

    static void closeSocket(stSMSokcet *stSocket);

    static int recvex(int iSocket,char* buffer,unsigned int bufferLen,int &recvLength ,int timeout= 500 );

    static int sendex(int iSocket, char *buffer, unsigned int bufferLen, int &sendLength, int timeout = 500);

   static  bool isSocketReadable(int iSocket);

    static int jsonStrToPacket(char*pMsg, stPacket* packet, map<string, string> *bodyMap = NULL);

    static int packetToJsonStr(const stPacket* packet, char* JsonStr);

    static string timetoStr();

    static void sendLogToUI(string strmsg,int iLevel = 0);

};

#endif // CSOCKETPROXY_H
