#ifndef SOCKETCLIENTTEST_H
#define SOCKETCLIENTTEST_H
#include "socketclient.h"

class CSocketClientTest
{
public:

    static CSocketClientTest* getInstance();
    static void releaseInstance();
    bool testConnect(int iCnt = 10);
    bool testSend();
private:
    CSocketClientTest();
    static CSocketClientTest* m_pInstance;
    CSocketClient* m_pCSocketClient;
};

#endif // SOCKETCLIENTTEST_H
