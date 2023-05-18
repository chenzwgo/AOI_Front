#include "udpsocketmanager.h"
#include "udpsocket.h"
#include "jmutex.h"

UdpSocketManager* UdpSocketManager::m_pSelf = NULL;
JMutex g_Udpmutex;


UdpSocketManager::UdpSocketManager()
{
    g_Udpmutex.Init();
}

UdpSocketManager::~UdpSocketManager()
{
    ClearUdpList();
}

UdpSocketManager *UdpSocketManager::GetInstance()
{
    if(m_pSelf == NULL)
    {
        m_pSelf = new UdpSocketManager();
    }
    return m_pSelf;

}

void UdpSocketManager::DestroyInstance()
{
    if(m_pSelf != NULL)
    {
        delete m_pSelf;
        m_pSelf = NULL;
    }
}

IUdpSocket *UdpSocketManager::CreateUdpSocket(const char *ClientIP,  unsigned int port)
{
    if(ClientIP == NULL)
    {
        return NULL;
    }
	
    IUdpSocket* ptmp = NULL;
	
	if (port != 0)
	{
		ptmp = FindFromList(ClientIP, port);
	}

    if (NULL == ptmp)
    {
        udpsocket* client = new udpsocket();
        if (NULL != client)
        {
            client->Init(ClientIP,port);
            AddToList(client);
        }
    }
    return ptmp;
}



void UdpSocketManager::ReleaseUdpSocket(const char *pIP, const unsigned int port)
{
    if(pIP == NULL)
    {
        return ;
    }
    IUdpSocket* ptmp = NULL;
    ptmp = FindFromList(pIP, port);

    if (NULL != ptmp)
    {
        RemoveUdp(ptmp);
    }
    return ;
}

void UdpSocketManager::ReleaseUdpSocket(IUdpSocket *pUdpSocket)
{
    if(pUdpSocket == NULL)
    {
        return ;
    }
    if(FindFromList(pUdpSocket))
    {
        RemoveUdp(pUdpSocket);
    }
}

void UdpSocketManager::AddToList(IUdpSocket *pUdp)
{
    if(NULL == pUdp)
    {
        return;
    }
    if(FindFromList(pUdp))
    {
        return;
    }
    m_udplist.push_back(pUdp);
}

IUdpSocket *UdpSocketManager::FindFromList(const char* pIP, unsigned int port)
{
	CISAPUGuard<JMutex> automutex(g_Udpmutex);
    list <IUdpSocket* >::iterator it = m_udplist.begin();
    while(it != m_udplist.end())
    {
        udpsocket* pUDP = dynamic_cast<udpsocket*>(*it);
        string strIp;
        unsigned int iPort;
        if (pUDP != NULL )
        { 
            pUDP->getIPandPort(strIp,iPort);
            if( strIp.compare(pIP) == 0&& iPort == port)
            {
                return *it;
            }
        }        
        it++;
    }
    return NULL;
}

bool UdpSocketManager::FindFromList(IUdpSocket *pUdp)
{
    if(pUdp == NULL)
    {
        return false;
    }

	CISAPUGuard<JMutex> automutex(g_Udpmutex);
    list <IUdpSocket* >::iterator it = m_udplist.begin();
    while(it != m_udplist.end())
    {
        if (pUdp == *it)
        {
            return true;
        }
        it++;
    }
    return false;
}

void UdpSocketManager::RemoveUdp(IUdpSocket *pUdp)
{
	CISAPUGuard<JMutex> automutex(g_Udpmutex);
    list <IUdpSocket* >::iterator it = m_udplist.begin();
    while(it != m_udplist.end())
    {
        if (pUdp == *it)
        {			
            m_udplist.erase (it);
            return;
        }
        it++;
    }
}

void UdpSocketManager::ClearUdpList()
{
	CISAPUGuard<JMutex> automutex(g_Udpmutex);
    list <IUdpSocket* >::iterator it = m_udplist.begin();
    while(it != m_udplist.end())
    {
        if (NULL != *it)
        {
            delete *it;
        }
        it++;
    }
    m_udplist.clear();
}

