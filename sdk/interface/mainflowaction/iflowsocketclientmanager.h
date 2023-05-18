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
    // @brief ����socketclient
    // @param clientIp:�ͻ���IP; port:�ͻ��˶˿�; socketName:socket����
    // @return  ����IFlowSocketClient
    /************************************************************************/
    virtual IFlowSocketClient* CreateSocketClient(const string &clientIP, unsigned int port, const string& socketName) = 0;
    
    /************************************************************************/
    // @brief ��ȡsocketclient
    // @param socketName:socket����
    // @return  ����IFlowSocketClient
    /************************************************************************/
    virtual IFlowSocketClient* GetSocketClient(const string& socketName) = 0;

    /************************************************************************/
    // @brief �ͷ�socketclient
    // @param socketName:socket����
    // @return
    /************************************************************************/
    virtual void ReleaseSocketClient(const string& socketName) = 0;

    /************************************************************************/
    // @brief ��ȡsocketclient�б�
    // @param
    // @return socketclient�б�key:���� value:ip��port
    /************************************************************************/
    virtual map<string, string> GetSocketClientList() = 0;
};
#endif
