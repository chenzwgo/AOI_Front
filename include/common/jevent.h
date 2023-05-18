#ifndef JEVENT_H
#define JEVENT_H

#include "jmutex.h"

#define JEVENT_WAIT_OK  0
#define JEVENT_WAIT_TIME_OUT -1
#define JEVENT_WAIT_OBJECT_0 0
#define JEVENT_WAIT_TIME_INFINITE -1

class JEvent
{
public:
    JEvent();
    JEvent(bool bManualReset, bool bInitialState);
    ~JEvent();
    void Init(bool bManualReset, bool bInitialState);
    void SetEvent();//set event to singnaled
    void ResetEvent();//set event to nonsignaled
    bool IsSiangaled();
    bool IsManualReset();
    static JMutex mutexForObjects; // for WaitForMultipleObjects when it's called in multiple threads
public:
    static long WaitForSingleObject(JEvent * event, long time);
    static long WaitForMultipleObjects(unsigned int count, JEvent ** pEventArray, bool bWaitForAll, long time);
private:
    JMutex mutex;// mutex for bIsSignaled
    bool bIsSignaled;
    bool bIsManualReset;
};

typedef JEvent * JEventHandle;

#endif // JEVENT_H
