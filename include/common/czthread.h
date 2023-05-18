#ifndef CZTHREAD_H
#define CZTHREAD_H

#include "jthread.h"
//class JThread;

//回调函数类型
typedef void *(*fnThreadCallBack)(void *param);

class CZThread : public JThread
{
public:
    CZThread();

    virtual void *Thread();

    //设置线程回调接口及参数, 参数只保留指针（浅拷贝）
    virtual void SetThreadParam(fnThreadCallBack fun, void *param); 

    //设置线程回调接口及参数, 参数拷贝内存，如果为字符串， iSize包括结束符（深拷贝）
    virtual void SetThreadParam(fnThreadCallBack fun, void *param, int iSize);
private:
    fnThreadCallBack m_funThread;
    void *           m_param;
    bool             m_bIsAutoMalloc;
};

#endif // CZTHREAD_H
