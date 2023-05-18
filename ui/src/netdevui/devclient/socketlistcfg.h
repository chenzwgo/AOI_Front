#ifndef SOCK_LIST__CFG_H
#define SOCK_LIST__CFG_H

#include <string>
#include <list>
#include <vector>
#include "socketerrcode.h"
#include "socketcommon.h"
#include "isocketlistcfg.h"
using namespace std;

class CSocketClient;
class CSocketproxy;
class ISocketproxy;
class CSocketClientListCfg  :public ISocketClientListCfg
{
public:
    static CSocketClientListCfg* getInstance();
    static void releaseInstance();
    CSocketClientListCfg();
    ~CSocketClientListCfg();
    
    ISocketproxy* getSocketClientProxy(string strName);
    serverCfg* getServerCfg();

    bool addClientCfg(const clientCfg& cfg);
    bool delClientCfg(const string& ClientName);
    bool ModifyClientCfg(const string& ClientName,const clientCfg& cfg);
    bool getClientList(vector<string>& ClientNamelist);
    clientCfg* getClientCfg(const string& ClientName);
    bool readServerConfig();
    bool writeServerConfig();
private:
    static CSocketClientListCfg* m_pInstance;
    map<string,clientCfg*> m_ClientInfomap;
    map<string,CSocketproxy*> m_Clientmap;
    serverCfg m_serverCfg;
    JMutex m_mapMutex;


};

#endif // SOCK_LIST__CFG_H
