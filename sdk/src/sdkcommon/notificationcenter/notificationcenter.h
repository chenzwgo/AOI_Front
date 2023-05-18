
#ifndef  __NOTIFICATIONCENTERSERVICE_H__
#define  __NOTIFICATIONCENTERSERVICE_H__

#include "sdkcommon/notificationcenter/inotificationcenter.h"
#include "jthreadt.h"
#include <vector>
#include <string>
#include <list>
#include <map>
#include <string>
#include "jmutex.h"
#include "jevent.h"
#include "refpointer.h"
using namespace std;


class CNotificationCenter : public INotificationCenter
{
private:
    CNotificationCenter();
    virtual ~CNotificationCenter();
public:

    static CNotificationCenter* GetInstance();
	static void DestroyInstance();
	
    void addObserver(const char* pName,pFunSelector pfun);
    void postNotification(const char* pName,unsigned int enNotifyType,void* pNotifyData);
    void sendNotification(const char* pName,unsigned int enNotifyType,void* pNotifyData);
    void removeObserver(const char* pName,pFunSelector pfun);
	void stopRunNotificationCenter() ;
	
private:

    struct PostMsg
    {
        int enNotifyType;
        void* pNotifyData;
        list<pFunSelector> ObserverList;
        PostMsg()
        {
            enNotifyType = 0;
            pNotifyData = NULL;
        }
    };
    void removeObserver(const char* pName);
    pFunSelector findObserver(const char* pName,pFunSelector pfun);
    bool insertObserver(const char* pName,pFunSelector pfun);
    void* ProcessPostMsg(void* pram);
    map<string,list<pFunSelector> > m_Map;
    list<PostMsg*> m_posmsglist;
    JMutex m_MapMutex;

    JMutex m_MutexPostmsgList;
    JEvent m_Event;
    JThreadTP<CNotificationCenter, void *, void *> *m_pThread;
    bool m_bThreadRunning;
	
private:
	friend class RefPointer<CNotificationCenter>;
	static CNotificationCenter* m_instance;
	static bool m_bStopRun;
};

#endif   //__NOTIFICATIONCENTERSERVICEIMPL_H__

