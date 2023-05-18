#ifndef SOCKETSERVERMANAGER_H
#define SOCKETSERVERMANAGER_H
#include "sdkcommon/socket/isocketservermanager.h"

#include <list>
#include <map>
using namespace std;

class ISocketServer;
class SocketServerManager: public ISocketServerManager
{
public:
    SocketServerManager();
    ~SocketServerManager();
    virtual ISocketServer* CreateSocketServer(const char* ServerIP, unsigned int port);

    virtual void ReleaseSocketServer(ISocketServer* pServer);

public:
    static  SocketServerManager* GetInstance();
    static void DestroyInstance();

private:


    void AddServerToList(ISocketServer* pServer);
    ISocketServer* FindServerFromList(const char* ServerIP, unsigned int port);
    bool  FindServerFromList(ISocketServer* pServer);
    void RemoveServer(ISocketServer* pServer);
    void ClearServerInfoList();
	map<ISocketServer*,int> m_mapRef;
    list <ISocketServer*> m_list_Server;
    static SocketServerManager* m_pSelf;
};

#endif // SOCKETSERVERMANAGER_H
