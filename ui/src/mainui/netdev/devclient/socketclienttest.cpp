#include "socketclienttest.h"
CSocketClientTest* CSocketClientTest::m_pInstance = NULL;
CSocketClientTest *CSocketClientTest::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CSocketClientTest();
    }
    return m_pInstance;
}

void CSocketClientTest::releaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
    }
    return ;
}

bool CSocketClientTest::testConnect(int iCnt)
{
    m_pCSocketClient->initClient("127.0.0.1",0);
    int iTime = iCnt;
    bool bflag = true;
    while(iTime >0)
    {
        if(!m_pCSocketClient->isConnected())
        {
            m_pCSocketClient->setServerCfg("127.0.0.1",10001);
            if(ERR_OK != m_pCSocketClient->connectServer())
            {
                bflag = false;
                break;
            }
        }
        Sleep(10);
        iTime--;
    }
    return bflag;

}

bool CSocketClientTest::testSend()
{
    if(!m_pCSocketClient->isConnected())
    {
         m_pCSocketClient->setServerCfg("127.0.0.1",10000);
        if(ERR_OK != m_pCSocketClient->connectServer())
        {
           return false;
        }
    }
    if(ERR_OK != m_pCSocketClient->sendHeartBeat())
    {
       return false;
    }
    return true;

}

CSocketClientTest::CSocketClientTest()
{
    m_pCSocketClient = new CSocketClient();

}
