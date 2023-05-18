#ifndef _IFLOW_SOCKET_CLIENT_MANAGER_H_
#define _IFLOW_SOCKET_CLIENT_MANAGER_H_

#include <map>
#include <string>
using namespace std;

class IFlowSocketClient;
class IFlowSocketClientManager
{
public:
		enum
		{
			IID_FLOWSOCKETCLIENTMANAGER= 0x301
		};
    virtual ~IFlowSocketClientManager(){}

    /************************************************************************/
    // @brief 创建socketclient
    // @param clientIp:客户端IP; port:客户端端口; socketName:socket名称
    // @return  返回IFlowSocketClient
    /************************************************************************/
    virtual IFlowSocketClient* CreateSocketClient(const string &clientIP, unsigned int port, const string& socketName) = 0;
    
    /************************************************************************/
    // @brief 获取socketclient
    // @param socketName:socket名称
    // @return  返回IFlowSocketClient
    /************************************************************************/
    virtual IFlowSocketClient* GetSocketClient(const string& socketName) = 0;

    /************************************************************************/
    // @brief 释放socketclient
    // @param socketName:socket名称
    // @return
    /************************************************************************/
    virtual void ReleaseSocketClient(const string& socketName) = 0;

    /************************************************************************/
    // @brief 获取socketclient列表
    // @param
    // @return socketclient列表，key:名称 value:ip和port
    /************************************************************************/
    virtual map<string, string> GetSocketClientList() = 0;
};
#endif
