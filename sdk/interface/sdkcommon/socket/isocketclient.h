#ifndef _ISOCKET_CLIENT_H_
#define _ISOCKET_CLIENT_H_

#include <string>
using std::string;

class ISocketClient;
typedef enum
{
    SocketNoCallBack = 0,			// 接收到数据不处理
    SocketDataUseListCallBack ,                 // 接收到数据放入链表
    SocketUseCallBack,                          // 接收到数据调用回调
}enSocketType;

typedef enum
{
    SocketReadCallBack =0,              // socket 为 SocketUseCallBack 类型时触发的事件
    SocketAcceptCallBack ,
    SocketConnectCallBack ,
    SocketDisConnectCallBack,
    SocketWriteCallBack ,
    SocketErrorCallBack ,
    SocketCallBackMax	// 在此之前增加事件类型
}enSocketCallBackEvent;

class ClientInfo
{
public:
    string strClientIP;
    unsigned int ClientPort;
    string strClientType;
    int ClientSock;
    bool bConnected;
    string strServerIP;
    unsigned int ServerPort;

    bool operator ==(const ClientInfo& info)
    {
        if (&info == this)
        {
            return true;
        }
        if (info.strClientIP == this->strClientIP
            && info.ClientPort == this->ClientPort)
        {
            return true;
        }
        return false;
    }

    ClientInfo()
    {
        strClientIP = "";
        ClientPort = 0;
        strClientType = "";
        ClientSock = -1;
        bConnected = false;
        strServerIP = "";
        ServerPort = 0;
    }
    ~ClientInfo()
    {
        strClientIP = "";
        ClientPort = 0;
        strClientType = "";
        ClientSock = -1;
        bConnected = false;
        strServerIP = "";
        ServerPort = 0;
    }
    void clear()
    {
        strClientIP = "";
        ClientPort = 0;
        strClientType = "";
        ClientSock = -1;
        bConnected = false;
        strServerIP = "";
        ServerPort = 0;
    }
};

typedef void (*SocketCallBackFun)(enSocketCallBackEvent event);

class ISocketClient
{
public:
	enum
	{
		IID_SOCKET_CLIENT = 0x1001
	};
	virtual ~ISocketClient(){}
public:

    virtual bool ReConnect() = 0;
    virtual bool Connect(const char* pServerIP,unsigned int ServerPort,bool enablePing = false) = 0;

    virtual bool DisConnect() = 0;

    virtual int ClientRecvMsg(char *pBuffer, unsigned int ibufferLength,const char *szMark = NULL) = 0;

    virtual int ClientSendMsg(const char *pData,unsigned int iDataLength) =0;
    virtual int GetMsgPos(const char* pMsg) = 0;

    virtual bool IsClientOnline() =0;

    virtual bool ClientHasDataArrived() =0;

    virtual void ClearClientRecvBuffer() =0;

    virtual void RegisterEeventCallBack(SocketCallBackFun pFun) = 0;

    virtual void SetSokectType(enSocketType CallBacktpye) = 0;

    virtual const ClientInfo* GetClientInfo() = 0;

    virtual bool IsClinetStatusChange() = 0;
};


#endif
