#ifndef __ISOCKETSERVER_H__
#define __ISOCKETSERVER_H__
#include <list>
class ISocketClient;
class ISocketServer
{
	public:
    ISocketServer()
    {
    }

    virtual ~ISocketServer()
    {
    }

    //virtual bool InitServerSocket(const char* ServerIP, unsigned int Serverport) = 0;
    
    virtual bool StartSever() = 0;

    virtual bool StopSever() = 0;

    virtual ISocketClient* GetSocketClient(const char* ClientIP, unsigned int Clientport) =0;

    virtual void ReleaseSocketClient(ISocketClient* pClient) = 0;

    virtual std::list <ISocketClient*>* GetClientList() = 0;

	virtual bool IsClientStatusChanged() = 0;


};

#endif //__ISOCKETSERVER_H__
