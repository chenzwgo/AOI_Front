#ifndef _ISOCKET_SERVER_MANAGER_H_
#define _ISOCKET_SERVER_MANAGER_H_
#include "isocketserver.h"

class ISocketServer;
class ISocketServerManager
{
public:
			enum
		{
			IID_SOCKETSERVERMANAGER= 0x204
		};
    virtual ~ISocketServerManager(){}

    virtual ISocketServer* CreateSocketServer(const char* ServerIP, unsigned int port) = 0;

    virtual void ReleaseSocketServer(ISocketServer* pServer) = 0;
};
#endif
