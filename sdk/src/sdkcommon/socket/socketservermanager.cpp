#include "socketservermanager.h"
#include "socketserver.h"
JMutex g_SeverManagermutex;


SocketServerManager* SocketServerManager::m_pSelf = NULL;

SocketServerManager::SocketServerManager()
{
	g_SeverManagermutex.Init();
}

SocketServerManager::~SocketServerManager()
{

}

ISocketServer *SocketServerManager::CreateSocketServer(const char *ServerIP, unsigned int port)
{
    if(ServerIP == NULL)
    {
        return NULL;
    }

	CISAPUGuard<JMutex> automutex(g_SeverManagermutex);
	
    ISocketServer* ptmp = NULL;
    ptmp = FindServerFromList(ServerIP, port);

    if (NULL == ptmp)
    {
        SocketServer* server = new SocketServer();
        if (NULL != server)
        {
            server->InitServerSocket (ServerIP,port);
            AddServerToList(server);
            ptmp = server;
			m_mapRef[ptmp] = 0;
        }
    }
	m_mapRef[ptmp] = m_mapRef[ptmp] +1;
    return ptmp;
}

void SocketServerManager::ReleaseSocketServer(ISocketServer *pServer)
{
    if(pServer == NULL)
    {
        return ;
    }
    if(FindServerFromList(pServer))
    {
        RemoveServer(pServer);
    }
}

SocketServerManager *SocketServerManager::GetInstance()
{
    if(m_pSelf == NULL)
    {
        m_pSelf = new SocketServerManager();
    }
    return m_pSelf;
}

void SocketServerManager::DestroyInstance()
{
    if(m_pSelf != NULL)
    {
        delete m_pSelf;
        m_pSelf = NULL;
    }
}

void SocketServerManager::AddServerToList(ISocketServer *pServer)
{
    if(NULL == pServer)
    {
        return;
    }
    if(FindServerFromList(pServer))
    {
        return;
    }
    m_list_Server.push_back(pServer);
}

ISocketServer *SocketServerManager::FindServerFromList(const char *ServerIP, unsigned int port)
{
    list <ISocketServer* >::iterator it = m_list_Server.begin();
    while(it != m_list_Server.end())
    {
        SocketServer* pServer = dynamic_cast<SocketServer*>(*it);
        if(ServerIP != NULL
                && strcmp(pServer->GetServerInfo ()->strIPAddr.c_str (),ServerIP) == 0
                && pServer->GetServerInfo ()->iPort == port)
        {
            return *it;
        }
        it++;
    }
    return NULL;
}

bool SocketServerManager::FindServerFromList(ISocketServer *pServer)
{
    if(pServer == NULL)
    {
        return false;
    }

    list <ISocketServer* >::iterator it = m_list_Server.begin();
    while(it != m_list_Server.end())
    {
        if (pServer == *it)
        {
            return true;
        }
        it++;
    }
    return false;
}

void SocketServerManager::RemoveServer(ISocketServer *pServer)
{
    list <ISocketServer* >::iterator it = m_list_Server.begin();
    while(it != m_list_Server.end())
    {
        if (pServer == *it)
        {

			map<ISocketServer*,int>::iterator it2 = m_mapRef.find(pServer);
			if (it2 != m_mapRef.end())
			{
				it2->second = it2->second -1;
				if (it2->second <=0)
				{
					delete *it;
					m_list_Server.erase (it);
					m_mapRef.erase(it2);
					return;
				}
			}

        }
        it++;
    }
}

void SocketServerManager::ClearServerInfoList()
{
    list <ISocketServer* >::iterator it = m_list_Server.begin();
    while(it != m_list_Server.end())
    {
        if (NULL != *it)
        {
            delete *it;
        }
        it++;
    }
    m_list_Server.clear();
}

