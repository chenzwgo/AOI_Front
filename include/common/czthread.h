#ifndef CZTHREAD_H
#define CZTHREAD_H

#include "jthread.h"
//class JThread;

//�ص���������
typedef void *(*fnThreadCallBack)(void *param);

class CZThread : public JThread
{
public:
    CZThread();

    virtual void *Thread();

    //�����̻߳ص��ӿڼ�����, ����ֻ����ָ�루ǳ������
    virtual void SetThreadParam(fnThreadCallBack fun, void *param); 

    //�����̻߳ص��ӿڼ�����, ���������ڴ棬���Ϊ�ַ����� iSize�����������������
    virtual void SetThreadParam(fnThreadCallBack fun, void *param, int iSize);
private:
    fnThreadCallBack m_funThread;
    void *           m_param;
    bool             m_bIsAutoMalloc;
};

#endif // CZTHREAD_H
