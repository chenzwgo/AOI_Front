#ifndef _SOCKET_CLIENT_H_
#define _SOCKET_CLIENT_H_
#include <string>
#ifdef Q_OS_WIN
#include <winsock2.h>

#include <ws2tcpip.h>
#include <tchar.h>
#else
#include <sys/socket.h>
#include <pthread.h>
#endif
#include <list>

#include "sdkcommon/socket/isocketclient.h"
#include "jthreadt.h"
#include "jmutex.h"
#include "ringbuffer.h"

using namespace std;

void InitNetEvn();
void ClearNetEvn();

class SocketClient : public ISocketClient
{
public:
	SocketClient(void);
	~SocketClient(void);

public:
    virtual bool ReConnect();
    virtual bool Connect(const char* pServerIP,unsigned int ServerPort,bool enablePing = false);

    virtual bool DisConnect() ;

	virtual int ClientRecvMsg(char *pBuffer, unsigned int ibufferLength,const char *szMark = NULL) ;

	virtual int ClientSendMsg(const char *pData,unsigned int iDataLength) ;

	virtual int GetMsgPos(const char* pMsg);

	virtual bool IsClientOnline();

	virtual bool ClientHasDataArrived();

	virtual void ClearClientRecvBuffer() ;

    virtual void RegisterEeventCallBack(SocketCallBackFun pFun);

    virtual void SetSokectType(enSocketType CallBacktpye) ;

    virtual const ClientInfo* GetClientInfo();

	virtual bool IsClinetStatusChange();
	
    bool InitClient(const char* pClientIP,unsigned int ClientPort);
    bool SetClientInfo(const ClientInfo* pinfo);

    bool StartClient();



private:
    JThreadTP<SocketClient, void *, void *>*m_pThread;
    JThreadTP<SocketClient, void *, void *>*m_pPingThread;


    JMutex m_MutexLock;		 // 保护回调队列的锁
    JMutex m_MutexSocketLock;		 // socket 状态锁
    void* RecvThread(void *lpParameter);
    void* PingThread(void *lpParameter);

	bool m_bStarted;
	bool m_bRecvThread;
    bool m_bPingThread;
    bool m_bInit;
	bool m_bChanged;
    bool m_bEnablePing;
    ClientInfo m_myinfo;
    SocketCallBackFun m_CBFun;
	
	void ClearMsgBuffer();
	int WriteMsgToBuffer(char *pData,int DataLength);
	int ReadMsgFromBuffer(char *pData,int DataLength,const char* mask = NULL);
	int GetMsgDateLength();

    bool CreateSock();

    bool StopClient();

    bool myping( const char *pIP, int nOverTime );
    void closeSocket();
    unsigned short cal_chksum(unsigned short *addr, int len);
	void ChangeConnectStatus(bool bconnect);

    enSocketType m_SocketType;
	CRingBuffer* m_pCRingBuffer;
	
};
#endif
