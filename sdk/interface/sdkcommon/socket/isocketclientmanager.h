#ifndef _ISOCKET_CLIENT_MANAGER_H_
#define _ISOCKET_CLIENT_MANAGER_H_
#include <list>
class ISocketClient;
class ISocketClientManager
{
public:
		enum
		{
			IID_SOCKETCLIENTMANAGER= 0x203
		};
    virtual ~ISocketClientManager(){}

    virtual ISocketClient* CreateSocketClient(const char* ClientIP, unsigned int port) = 0;
    virtual void ReleaseSocketClient(ISocketClient* pSocketClient) = 0;
};
#endif
