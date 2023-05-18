#ifndef JTHREADMONITOR_H
#define JTHREADMONITOR_H
class JThread;
class JThreadMonitor
{
public:
    JThreadMonitor();
    ~JThreadMonitor();
    static void AddMonitorThread(JThread* pThread);
    static void RemoveMonitorThread(JThread* pThread);
    static unsigned int GetThreadCount();
    static unsigned int GetRunningThreadCount();
    static  long GetExistedThreadHandle(unsigned int index);
    static  long GetRunningThraedHandle(unsigned int index);
    static  const char* GetExistedThreadCallClassName(unsigned int index);
    static  const char* GetRunningThraedCallClassName(unsigned int index);
};

#endif // JTHREADMONITOR_H
