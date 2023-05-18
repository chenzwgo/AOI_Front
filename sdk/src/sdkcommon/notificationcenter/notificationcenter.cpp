#include "notificationcenter.h"
#include <vector> 
#include <algorithm>
#include "jthreadt.h"
#include <stdio.h>
#include "jtime.h"
#include "jevent.h"

CNotificationCenter* CNotificationCenter::m_instance = NULL;
bool CNotificationCenter::m_bStopRun = false;

CNotificationCenter::CNotificationCenter()
{
    try
    {
        m_MutexPostmsgList.Init();
        m_Event.Init(false,false);
        m_bThreadRunning = true;
        m_pThread = new JThreadTP<CNotificationCenter, void *, void *>;
        m_pThread->StartThread(this, &CNotificationCenter::ProcessPostMsg, this);

    }
    catch( ... )
    {

    }
}

CNotificationCenter::~CNotificationCenter()
{
	m_bStopRun = true;
    m_bThreadRunning = false;
	m_Event.SetEvent();
	if (m_pThread->IsRunning())
	{
		m_pThread->EndThread();
	}
	if (m_pThread != NULL)
	{
		delete m_pThread;
		m_pThread = NULL;
	}
	
    m_MutexPostmsgList.Lock();
    list<PostMsg*>::iterator it = m_posmsglist.begin();
    while(it != m_posmsglist.end())
    {
        if((*it) != NULL)
        {
            delete (*it);
        }
        it++;
    }
    m_posmsglist.clear();
    m_MutexPostmsgList.Unlock();
    
}



CNotificationCenter* CNotificationCenter::GetInstance()
{
	if (m_bStopRun)
	{
		return NULL;
	}
    if ( NULL == m_instance )
    {
        m_instance = new CNotificationCenter;
    }
    return m_instance;
}


void CNotificationCenter::addObserver( const char* pName,pFunSelector pfun )
{
    if (pName == NULL || pfun == NULL || m_bStopRun)
    {
        return ;
    }
    if (findObserver(pName,pfun))
    {
        return ;
    }
    insertObserver(pName,pfun);
}

void CNotificationCenter::postNotification(const char *pName, unsigned int  enNotifyType, void* pNotifyData)
{
    if (pName == NULL || m_bStopRun)
    {
        return ;
    }

    m_MapMutex.Lock();
    list<pFunSelector> tmp;
    map<string,list<pFunSelector> > ::iterator it = m_Map.find(pName);
    if (it == m_Map.end())
    {
       m_MapMutex.Unlock();
       return;
    }
    tmp = it->second;
    m_MapMutex.Unlock();

    if(tmp.size() == 0)
    {
        return;
    }

    PostMsg *pMsg = new PostMsg();
    if(pMsg == NULL)
    {
        return;
    }
    pMsg->enNotifyType = enNotifyType;
    pMsg->pNotifyData = pNotifyData;
    pMsg->ObserverList =tmp;
    m_MutexPostmsgList.Lock();
    m_posmsglist.push_back(pMsg);
    m_MutexPostmsgList.Unlock();

    m_Event.SetEvent();
    return;
}

void CNotificationCenter::sendNotification(const char *pName, unsigned int  enNotifyType, void* pNotifyData)
{
    if (pName == NULL|| m_bStopRun)
    {
        return ;
    }
    m_MapMutex.Lock();
    map<string,list<pFunSelector> > ::iterator it = m_Map.find(pName);
    if (it != m_Map.end())
    {
        list<pFunSelector> tmp = it->second;
        m_MapMutex.Unlock();

        list<pFunSelector>::iterator listit = tmp.begin();
        while(listit != tmp.end())
        {
            if ((*listit) != NULL)
            {
                (*listit)(enNotifyType,pNotifyData);
            }
            listit++;
        }
    }
    else
    {
        m_MapMutex.Unlock();
    }
    return ;
}

void CNotificationCenter::removeObserver(const char *pName)
{
    if (pName == NULL)
    {
        return ;
    }

    m_MapMutex.Lock();
    map<string,list<pFunSelector> > ::iterator it = m_Map.find(pName);
    if (it != m_Map.end())
    {
         it->second.clear();
    }
    m_MapMutex.Unlock();
    return ;
}

void CNotificationCenter::removeObserver( const char* pName,pFunSelector pfun )
{
    if (pName == NULL || pfun == NULL)
    {
        return ;
    }

    m_MapMutex.Lock();
    map<string,list<pFunSelector> > ::iterator it = m_Map.find(pName);
    if (it != m_Map.end())
    {
        list<pFunSelector> *tmp = &(it->second);
        list<pFunSelector>::iterator listit = tmp->begin();
        while(listit != tmp->end())
        {
            if ((*listit) == pfun)
            {
                tmp->erase(listit);
                m_MapMutex.Unlock();
                return ;
            }
            listit++;
        }
        tmp->push_back(pfun);
    }
    m_MapMutex.Unlock();
    return ;
}

pFunSelector CNotificationCenter::findObserver( const char* pName,pFunSelector pfun)
{
    if (pName == NULL || pfun == NULL)
    {
        return NULL;
    }
    m_MapMutex.Lock();
    map<string,list<pFunSelector> > ::iterator it = m_Map.find(pName);
    if (it != m_Map.end())
    {
        list<pFunSelector> *tmp = &(it->second);
        list<pFunSelector>::iterator listit = tmp->begin();
        while(listit != tmp->end())
        {
            if ((*listit) == pfun)
            {
                m_MapMutex.Unlock();
                return pfun;
            }
            listit++;
        }
    }
    m_MapMutex.Unlock();
    return NULL;
}

bool CNotificationCenter::insertObserver( const char* pName,pFunSelector pfun)
{
    if (pName == NULL || pfun == NULL || m_bStopRun)
    {
        return false;
    }

    m_MapMutex.Lock();
    map<string,list<pFunSelector> > ::iterator it = m_Map.find(pName);
    if (it != m_Map.end())
    {
        list<pFunSelector> *tmp = &(it->second);
        list<pFunSelector>::iterator listit = tmp->begin();
        while(listit != tmp->end())
        {
            if ((*listit) == pfun)
            {
                m_MapMutex.Unlock();
                return false;
            }
            listit++;
        }
        tmp->push_back(pfun);
    }
    else
    {
        m_Map[pName].push_back(pfun);
    }
    m_MapMutex.Unlock();
    return true;
}

void* CNotificationCenter::ProcessPostMsg(void *pram)
{
    CNotificationCenter* pThis = ( CNotificationCenter* )pram;
    while(pThis->m_bThreadRunning)
    {
         if (JEVENT_WAIT_OK == JEvent::WaitForSingleObject(&(pThis->m_Event),50))
         {
             pThis->m_MutexPostmsgList.Lock();
             int isize = pThis->m_posmsglist.size();
             pThis->m_MutexPostmsgList.Unlock();
             while(isize >0 && !m_bStopRun)
             {
                pThis->m_MutexPostmsgList.Lock();
                PostMsg* pTmp = pThis->m_posmsglist.front();
                pThis->m_posmsglist.pop_front();
                pThis->m_MutexPostmsgList.Unlock();

                if(pTmp != NULL && pTmp->ObserverList.size()>0)
                {
                    list<pFunSelector> *pobserverlist =  &(pTmp->ObserverList);
                    list<pFunSelector>::iterator it = pobserverlist->begin();
                    while(it != pobserverlist->end())
                    {
                        if((*it) != NULL)
                        {
                            (*it)(pTmp->enNotifyType,pTmp->pNotifyData);
                        }
                        it++;
                    }
                    delete pTmp;
                }
                pThis->m_MutexPostmsgList.Lock();
                isize = pThis->m_posmsglist.size();
                pThis->m_MutexPostmsgList.Unlock();
             }
         }
         Sleep(50);
    }
    return NULL;
}

void CNotificationCenter::DestroyInstance()
{
	if ( NULL != m_instance )
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void CNotificationCenter::stopRunNotificationCenter()
{
	m_bStopRun = true;
	m_bThreadRunning = false;
	m_Event.SetEvent();
}

