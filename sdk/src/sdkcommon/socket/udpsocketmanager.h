#ifndef _UDP_SOCKETSERVERMANAGER_H
#define _UDP_SOCKETSERVERMANAGER_H

#include "sdkcommon/socket/iudpsocket.h"
#include "sdkcommon/socket/iudpsocketmanager.h"


#include <list>
using namespace std;

class IUdpSocket;
class JMutex;
class UdpSocketManager : public IUdpSocketManager
{
public:
    static  UdpSocketManager* GetInstance();
    static void DestroyInstance();

    IUdpSocket* CreateUdpSocket(const char* ClientIP, unsigned int port);
    void ReleaseUdpSocket(IUdpSocket* pUdpSocket);
    
private:
    UdpSocketManager();
    ~UdpSocketManager();
    void ReleaseUdpSocket(const char* pIP,const unsigned int port);  
    void AddToList(IUdpSocket* pUdp);
    IUdpSocket* FindFromList(const char* pIP, unsigned int port);
    bool  FindFromList(IUdpSocket* pUdp);
    void RemoveUdp(IUdpSocket* pUdp);
    void ClearUdpList();

    list <IUdpSocket*> m_udplist;
    static UdpSocketManager* m_pSelf;


};

#endif // SOCKETSERVERMANAGER_H
