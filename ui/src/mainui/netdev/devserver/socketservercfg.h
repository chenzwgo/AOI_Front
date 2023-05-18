#ifndef SOCK_SERVER__CFG_H
#define SOCK_SERVER__CFG_H

#include <string>
#include <list>
#include <vector>
#include "socketerrcode.h"
#include "socketcommon.h"
using namespace std;


class CSocketServerCfg
{
public:
    static CSocketServerCfg* getInstance();
    static void releaseInstance();
    CSocketServerCfg();
    ~CSocketServerCfg();
    
    serverCfg* getServerCfg();

    bool addClientCfg(const clientCfg& cfg);
    bool delClientCfg(const string& ClientName);
    bool ModifyClientCfg(const string& ClientName,const clientCfg& cfg);
    bool getClientList(vector<string>& ClientNamelist);
    clientCfg* getClientCfg(const string& ClientName);
    bool readServerConfig();
    bool writeServerConfig();
private:
    static CSocketServerCfg* m_pInstance;
    map<string,clientCfg*> m_ClientInfomap;
    serverCfg m_serverCfg;
    JMutex m_mapMutex;


};

#endif // SMCAMERASOCK_H
