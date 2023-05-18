#ifndef SOCK_SERVER_H
#define SOCK_SERVER_H

#include <string>
#include <list>
#include "socketerrcode.h"
#include "packetdef.h"
#include "jthreadt.h"
#include "ringbuffer.h"
#include "socketproxy.h"
#include "socketPC1.h"

using namespace std;

#ifndef SOCKET
#define SOCKET int
#endif
#define SOCKET_PACKET_LENGTH 1024
class CSocketproxy;
class CSocketClientPC1;


struct ClientPCUnion
{
#define  SocketClientPC1_SIZE  1
    string strClientName;
    CSocketClientPC1* pCSocketClientPC1[SocketClientPC1_SIZE];
    unsigned int iClientPC1Index;
    ClientPCUnion()
    {
        strClientName = "";
        for (int i = 0; i <SocketClientPC1_SIZE ;i++)
        {
            pCSocketClientPC1[i] = NULL;
        }   
        iClientPC1Index = 0;
    }

    ClientPCUnion(string strName)
    {
        strClientName = strName;
        for (int i = 0; i <2 ;i++)
        {
            pCSocketClientPC1[i] = new CSocketClientPC1(strName);
        }   
        iClientPC1Index = 0;
    }

    ~ClientPCUnion()
    {
        for (int i = 0; i <SocketClientPC1_SIZE ;i++)
        {
            if (pCSocketClientPC1[i] != NULL)
            {
                delete pCSocketClientPC1[i];
                pCSocketClientPC1[i] = NULL;
            }
        }  
        iClientPC1Index = -1;
    }
    void setSocket(stSMSokcet& stSokcet)
    {
        int index = iClientPC1Index %SocketClientPC1_SIZE;
        if (pCSocketClientPC1[index] != NULL)
        {
            pCSocketClientPC1[index]->setSocket(stSokcet);
        }
        iClientPC1Index++;
    }

    CSocketClientPC1* getSocketClientPC(unsigned int index)
    {
        if (index >=SocketClientPC1_SIZE)
        {
            return NULL;
        }
        return pCSocketClientPC1[index];
    }

    unsigned int  getCurrentIndex(){return iClientPC1Index;}

    int getSocketClientPC1Size() {return SocketClientPC1_SIZE;}

};


class CSocketServer
{
public:
    static CSocketServer* getInstance();
    static void releaseInstance();
    int initServer();
    int startServer();
    int stopServer();
    int resetServer();
    bool isServerRunning();

    CSocketClientPC1* getSocketClient(string strClientName);
private:
    CSocketServer();
    ~CSocketServer();

    void* AcceptThread(void* lpParameter);

    int dispatchSocket(stSMSokcet& pSocket);

    void removeAllSocket();

    void readConfig();


private:
    static CSocketServer* m_pInstance;
    string m_strServerIPAddr;
    unsigned int m_iServerPort;
    int m_iServerSock;
    bool m_bServerInit;
    bool m_bThreadRunning;
    JThreadTP<CSocketServer, void *, void *>* m_pAcceptThread;

    map<string,stSMSokcet*> m_Sokcetmap;
    JMutex m_SocketListLock;
    ClientPCUnion* m_pPC1;
    ClientPCUnion* m_pPC2;
    ClientPCUnion* m_pPC3;

};

#endif // SMCAMERASOCK_H
