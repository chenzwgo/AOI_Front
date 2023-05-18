#ifndef _SOCKETSERVERMANAGER_H
#define _SOCKETSERVERMANAGER_H

#include "sdkcommon/socket/isocketclient.h"
#include "sdkcommon/socket/isocketclientmanager.h"


#include <list>
#include <map>
using namespace std;

class ISocketClient;
class JMutex;
class SocketClientManager : public ISocketClientManager
{
public:
    static  SocketClientManager* GetInstance();
    static void DestroyInstance();
    SocketClientManager();
    ~SocketClientManager();
    ISocketClient* CreateSocketClient(const char* ClientIP, unsigned int port);
    std::list <ISocketClient*>* GetClientList();

    void ReleaseSocketClient(ISocketClient* pSocketClient);
    void ReleaseAllClient();

	bool IsClientStatusChange();

private:
    void ReleaseSocketClient(const char* ClientIP,const unsigned int port);
    list <ISocketClient*> m_list_clientInfo;
    void AddClientToList(ISocketClient* pclientinfo);
    ISocketClient* FindClientFromList(const char* ClientIP, unsigned int port);
    bool  FindClientFromList(ISocketClient* pClient);
    void RemoveClient(ISocketClient* pclientinfo);
    void ClearClientInfoList();
	map<ISocketClient*,int> m_mapRef;
    static SocketClientManager* m_pSelf;
	void ChangeStatusFlag(bool bflag);
	bool m_bClientStatusChange;
};

#endif // SOCKETSERVERMANAGER_H
