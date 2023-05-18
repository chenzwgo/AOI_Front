#include "socketclientmanager.h"
#include "socketclient.h"
#include "jmutex.h"

SocketClientManager* SocketClientManager::m_pSelf = NULL;
JMutex g_mutex;

SocketClientManager *SocketClientManager::GetInstance()
{
    if(m_pSelf == NULL)
    {
        m_pSelf = new SocketClientManager();
    }
    return m_pSelf;

}

void SocketClientManager::DestroyInstance()
{
    if(m_pSelf != NULL)
    {
        delete m_pSelf;
        m_pSelf = NULL;
    }
}

ISocketClient *SocketClientManager::CreateSocketClient(const char *ClientIP,  unsigned int port)
{
    if(ClientIP == NULL)
    {
        return NULL;
    }
	
    ISocketClient* ptmp = NULL;
	
	if (port != 0)
	{
		ptmp = FindClientFromList(ClientIP, port);
	}

    if (NULL == ptmp)
    {
        SocketClient* client = new SocketClient();
        if (NULL != client)
        {
            client->InitClient (ClientIP,port);
			client->SetSokectType(SocketDataUseListCallBack);
            AddClientToList(client);
            ptmp = client;
			m_mapRef[ptmp] = 0;
        }
    }
	m_mapRef[ptmp] = m_mapRef[ptmp] +1;
    return ptmp;
}

std::list<ISocketClient *> *SocketClientManager::GetClientList()
{
	CISAPUGuard<JMutex> automutex(g_mutex);
    return &m_list_clientInfo;
}

void SocketClientManager::ReleaseSocketClient(const char *ClientIP, const unsigned int port)
{
    if(ClientIP == NULL)
    {
        return ;
    }
    ISocketClient* ptmp = NULL;
    ptmp = FindClientFromList(ClientIP, port);

    if (NULL != ptmp)
    {
        RemoveClient(ptmp);
    }
    return ;
}

void SocketClientManager::ReleaseSocketClient(ISocketClient *pSocketClient)
{
    if(pSocketClient == NULL)
    {
        return ;
    }
    if(FindClientFromList(pSocketClient))
    {
        RemoveClient(pSocketClient);
    }
}

void SocketClientManager::ReleaseAllClient()
{
    ClearClientInfoList();
}

SocketClientManager::SocketClientManager()
{
	g_mutex.Init();
	m_bClientStatusChange = false;
}

SocketClientManager::~SocketClientManager()
{
    ClearClientInfoList();
}

void SocketClientManager::AddClientToList(ISocketClient *pclientinfo)
{
    if(NULL == pclientinfo)
    {
        return;
    }
    if(FindClientFromList(pclientinfo))
    {
        return;
    }
    m_list_clientInfo.push_back(pclientinfo);
	ChangeStatusFlag(true);
}

ISocketClient *SocketClientManager::FindClientFromList(const char* ClientIP, unsigned int port)
{
	CISAPUGuard<JMutex> automutex(g_mutex);
    list <ISocketClient* >::iterator it = m_list_clientInfo.begin();
    while(it != m_list_clientInfo.end())
    {
        SocketClient* pclient = dynamic_cast<SocketClient*>(*it);
        if(pclient != NULL
                && strcmp(pclient->GetClientInfo ()->strClientIP.c_str (),ClientIP) == 0
                && pclient->GetClientInfo ()->ClientPort == port)
        {
            return *it;
        }
        it++;
    }
    return NULL;
}

bool SocketClientManager::FindClientFromList(ISocketClient *pClient)
{
    if(pClient == NULL)
    {
        return false;
    }
	CISAPUGuard<JMutex> automutex(g_mutex);

    list <ISocketClient* >::iterator it = m_list_clientInfo.begin();
    while(it != m_list_clientInfo.end())
    {
        if (pClient == *it)
        {
            return true;
        }
        it++;
    }
    return false;
}

void SocketClientManager::RemoveClient(ISocketClient *pclientinfo)
{
	CISAPUGuard<JMutex> automutex(g_mutex);
    list <ISocketClient* >::iterator it = m_list_clientInfo.begin();
    while(it != m_list_clientInfo.end())
    {
        if (pclientinfo == *it)
        {
			map<ISocketClient*,int>::iterator it2 = m_mapRef.find(pclientinfo);
			if (it2 != m_mapRef.end())
			{
				it2->second = it2->second -1;
				if (it2->second <=0)
				{
					delete *it;
					m_list_clientInfo.erase (it);
					m_mapRef.erase(it2);
					ChangeStatusFlag(true);
					return;
				}
			}

        }
        it++;
    }
}

void SocketClientManager::ClearClientInfoList()
{
	CISAPUGuard<JMutex> automutex(g_mutex);
    list <ISocketClient* >::iterator it = m_list_clientInfo.begin();
    while(it != m_list_clientInfo.end())
    {
        if (NULL != *it)
        {
            delete *it;
        }
        it++;
    }
	m_mapRef.clear();
    m_list_clientInfo.clear();
}

void SocketClientManager::ChangeStatusFlag( bool bflag )
{
	m_bClientStatusChange = bflag;
}

bool SocketClientManager::IsClientStatusChange()
{
	CISAPUGuard<JMutex> automutex(g_mutex);
	list <ISocketClient* >::iterator it = m_list_clientInfo.begin();
	while(it != m_list_clientInfo.end())
	{
		if ((*it)->IsClinetStatusChange())
		{
			return true;
		}
		it++;		
	}
	if (m_bClientStatusChange)
	{
		ChangeStatusFlag(false);
		return true;
	}
	return false;
}



