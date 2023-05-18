#include "reportcenter.h"
#include "sdkcommon/reportcenter/ireport.h"
#include <vector> 
#include <algorithm>
#include "jthreadt.h"
#include <stdio.h>
#include "jtime.h"
#include "jevent.h"
#include "sdkcore.h"

#define  RESEND_MAX_TIMES     3


CReportCenter* CReportCenter::m_instance = NULL;

bool CReportCenter::m_bStopRun = false;

CReportCenter::CReportCenter()
{
    try
    {
        m_pfun = NULL;
        m_MutexPostmsgList.Init();
        m_Event.Init(false,false);
        m_bThreadRunning = true;
        m_pThread = new JThreadTP<CReportCenter, void *, void *>;
        m_pThread->StartThread(this, &CReportCenter::ProcessPostMsg, this);

    }
    catch( ... )
    {

    }
}

CReportCenter::~CReportCenter()
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



CReportCenter* CReportCenter::GetInstance()
{
	if (m_bStopRun)
	{
		return NULL;
	}
    if ( NULL == m_instance )
    {
		m_instance = new CReportCenter;
    }
	return m_instance;
}


void CReportCenter::reportAddObserver(pFunUISelector pfun )
{
    if (pfun == NULL)
    {
        return ;
    }
    m_FunMutex.Lock();
    m_pfun = pfun;
    m_FunMutex.Unlock();
}

void CReportCenter::reportRemoveObserver( )
{
    if (m_pfun == NULL)
    {
        return ;
    }

    m_FunMutex.Lock();
    m_pfun = NULL;
    m_FunMutex.Unlock();
    return ;
}
bool CReportCenter::reportPostTo(const char* uiPluginGuid,const char* pEvent,OS_LPARAM pData1,OS_LPARAM pData2)
{
    if (uiPluginGuid == NULL || pEvent == NULL || m_bStopRun || m_pfun == NULL)
    {
        return false;
    }

    PostMsg *pMsg = new PostMsg();
    if(pMsg == NULL)
    {
        return false;
    }
    pMsg->struiPluginGuid = uiPluginGuid;
    pMsg->strEvent = pEvent;
    pMsg->pData1 =pData1;
    pMsg->pData2 =pData2;
    m_MutexPostmsgList.Lock();
    m_posmsglist.push_back(pMsg);
    m_MutexPostmsgList.Unlock();
    m_Event.SetEvent();
    return true;
}

bool CReportCenter::reportSendTo(const char* uiPluginGuid,const char* pEvent,OS_LPARAM pData1,OS_LPARAM pData2)
{
    if (uiPluginGuid == NULL || pEvent == NULL || m_bStopRun || m_pfun == NULL)
    {
        return false;
    }
    bool bflag = false;
    ISDKResource*  pISDKResource  =sdkcore::GetSDKResource(uiPluginGuid);
    if (pISDKResource != NULL)
    {
        IReport* pIReport = dynamic_cast<IReport* >(pISDKResource);
        if (pIReport == NULL)
        {
            return false;
        }
        bflag = pIReport->reprot(pEvent, pData1, pData2);
    }
    else 
    {   
        if (m_pfun != NULL)
            bflag = m_pfun(uiPluginGuid,pEvent,pData1,pData2);

    }
    
    return  bflag;
}


void* CReportCenter::ProcessPostMsg(void *pram)
{
    CReportCenter* pThis = ( CReportCenter* )pram;
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
                if(pTmp != NULL )
                {
                    // 发送失败重新发送
                    //if (!reportSendTo(pTmp->struiPluginGuid.c_str(),pTmp->strEvent.c_str(),pTmp->pData1,pTmp->pData2) && pTmp->iTryTimes < RESEND_MAX_TIMES)
                    //{
                    //    pThis->m_MutexPostmsgList.Lock();
                    //    pTmp->iTryTimes +=1;
                    //    pThis->m_posmsglist.push_back(pTmp);
                    //    pThis->m_MutexPostmsgList.Unlock();
                    //    Sleep(50);
                    //}
                    //else
                    //{
                    //    delete pTmp;
                    //}
                    reportSendTo(pTmp->struiPluginGuid.c_str(),pTmp->strEvent.c_str(),pTmp->pData1,pTmp->pData2);
                    delete pTmp;
                    
                }
                pThis->m_MutexPostmsgList.Lock();
                isize = pThis->m_posmsglist.size();
                pThis->m_MutexPostmsgList.Unlock();
             }
         }
         Sleep(10);
    }
    return NULL;
}

void CReportCenter::DestroyInstance()
{
	if ( NULL != m_instance )
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void CReportCenter::stopReportCenter()
{
	m_bStopRun = true;
	m_bThreadRunning = false;
	m_Event.SetEvent();
}

