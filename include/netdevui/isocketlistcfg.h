#ifndef I_SOCK_LIST__CFG_H
#define I_SOCK_LIST__CFG_H

#include <string>
#include <list>
#include <vector>
#include "socketerrcode.h"
#include "socketcommon.h"
using namespace std;

class ISocketproxy;
class ISocketClientListCfg
{
public:
    enum
    {
        IID_ISOCKETCLIENTLISTCFG = 0x99
    };
    ~ISocketClientListCfg(){};
    
    virtual ISocketproxy* getSocketClientProxy(string strName) = 0;
    virtual serverCfg* getServerCfg() = 0;

    virtual bool addClientCfg(const clientCfg& cfg) = 0;
    virtual bool delClientCfg(const string& ClientName) = 0;
    virtual bool ModifyClientCfg(const string& ClientName,const clientCfg& cfg) = 0;
    virtual bool getClientList(vector<string>& ClientNamelist) = 0;
    virtual clientCfg* getClientCfg(const string& ClientName) = 0;
    virtual bool readServerConfig() = 0;
    virtual bool writeServerConfig() = 0;

};

#endif // I_SOCK_LIST__CFG_H
