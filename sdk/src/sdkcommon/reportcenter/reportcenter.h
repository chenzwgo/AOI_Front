
#ifndef  __REPORTCENTER_H__
#define  __REPORTCENTER_H__

#include "sdkcommon/reportcenter/ireportcenter.h"
#include "jthreadt.h"
#include <vector>
#include <string>
#include <list>
#include <map>
#include <string>
#include "jmutex.h"
#include "jevent.h"
using namespace std;


class CReportCenter : public IReportCenter
{
private:
    CReportCenter();
    virtual ~CReportCenter();
	void stopReportCenter() ;
public:

    static CReportCenter* GetInstance();
	static void DestroyInstance();
  
    bool reportPostTo(const char* uiPluginGuid,const char* pEvent,OS_LPARAM pData1= 0,OS_LPARAM pData2 =0);
    bool reportSendTo(const char* uiPluginGuid,const char* pEvent,OS_LPARAM pData1= 0,OS_LPARAM pData2 =0);
	
    void reportRemoveObserver();
	void reportAddObserver(pFunUISelector pfun);
	
	
private:

    struct PostMsg
    {
        string struiPluginGuid;
        string strEvent;
        OS_LPARAM pData1;
		OS_LPARAM pData2;
        unsigned int iTryTimes;
        PostMsg()
        {
             struiPluginGuid = "";
             strEvent = "";;
             pData1 = 0;
             pData2 = 0;
             iTryTimes = 0;
        }
        PostMsg(const PostMsg& other)
        {
            struiPluginGuid = other.struiPluginGuid;
            strEvent = other.strEvent;
            pData1 = other.pData1;
            pData2 = other.pData2;
            iTryTimes = other.iTryTimes;
        }
        PostMsg& operator=(const PostMsg& other)
        {
            if (this != &other)
            {
                struiPluginGuid = other.struiPluginGuid;
                strEvent = other.strEvent;
                pData1 = other.pData1;
                pData2 = other.pData2;
                iTryTimes = other.iTryTimes;
            }
            return *this;
        }

    };
    void* ProcessPostMsg(void* pram);
    pFunUISelector m_pfun;
    list<PostMsg*> m_posmsglist;
    JMutex m_FunMutex;

    JMutex m_MutexPostmsgList;
    JEvent m_Event;
    JThreadTP<CReportCenter, void *, void *> *m_pThread;
    bool m_bThreadRunning;
	
private:
	static CReportCenter* m_instance;
	static bool m_bStopRun;
    
};

#endif   //__REPORTCENTER_H__

