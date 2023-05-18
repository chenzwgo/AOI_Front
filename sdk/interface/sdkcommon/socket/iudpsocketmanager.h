#ifndef _IUDP_SOCKET_MANAGER_H_
#define _IUDP_SOCKET_MANAGER_H_

class IUdpSocket;
class IUdpSocketManager
{
public:
		enum
		{
			IID_UDP_SOCKETMANAGER= 0x403
		};
    virtual ~IUdpSocketManager(){}

    virtual IUdpSocket* CreateUdpSocket(const char* ClientIP, unsigned int port) = 0;
    virtual void ReleaseUdpSocket(IUdpSocket* pSocketClient) = 0;
};
#endif
