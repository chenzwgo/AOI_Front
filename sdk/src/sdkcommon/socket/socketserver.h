#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H
#ifdef Q_OS_WIN
#include <WinSock2.h>
#else
#define SOCKET int
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif


#include "sdkcommon/socket/isocketserver.h"
#include "jthreadt.h"
#include <string>

using namespace  std ;

class SocketClientManager;
class SocketServerInfo
{
public:

    string strIPAddr;		//IPµØÖ·
    unsigned int iPort;		//¶Ë¿ÚºÅ
    int ServerSock;
    bool bServerInit;

    SocketServerInfo()
    {
        strIPAddr ="";
        iPort = 0;
        ServerSock = -1;
        bServerInit = false;
    }

    SocketServerInfo& operator = (const SocketServerInfo &other)
    {
        if (this == &other)
        {
            return *this;
        }

        strIPAddr = other.strIPAddr;
        strIPAddr = other.strIPAddr;
        iPort = other.iPort;

        return *this;
    }

    bool operator == (const SocketServerInfo &other)
    {
        if (this->strIPAddr == other.strIPAddr
            && this->iPort == other.iPort
            )
        {
            return true;
        }
        return false;
    }
};


class SocketServer: public ISocketServer
{
public:
    SocketServer();
    ~SocketServer();
    //
    virtual ISocketClient* GetSocketClient(const char* ClientIP, unsigned int Clientport);
    virtual bool StartSever();
    virtual bool StopSever();
    virtual std::list <ISocketClient*>* GetClientList();
    virtual void ReleaseSocketClient(ISocketClient* pClient);
	virtual bool IsClientStatusChanged();

    SocketServerInfo* GetServerInfo();
    bool InitServerSocket(const char* ServerIP, unsigned int Serverport);
private:

    bool CreateSock();
    void* AcceptThread(void* lpParameter);
    void closeSocket();


    SocketClientManager* m_pClientManager;
    SocketServerInfo m_ServerInfo;
    JThreadTP<SocketServer, void *, void *>* m_pThread;
    bool m_bAcceptThreadRunning;

};

#endif // SOCKETSERVER_H
