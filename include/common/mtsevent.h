/**
* \file mtsevent.h(based on old version MTSEvent, but use QT instead)
*
* \brief 事件类的定义，功能为：等待一个或多个事件，可以将信号设置成无、有信号状态
*
* 
* \version 1.0
*/

#ifndef __MTS_EVENT_H__
#define __MTS_EVENT_H__


#include "jevent.h"

class  MTSEvent
{
public:
    /**
        * 构造函数
        */
    MTSEvent();
    ~MTSEvent();
    

    /**
        * 挂起, 等待超时或唤醒
        * \param dwTimeout
        *   [DWORD] 超时时长
        * \return	成功返回true，失败返回false
        */
    bool Wait(unsigned long dwTimeout = 0);

    /**
        * 唤醒
        * \return	成功返回true，失败返回false
        */
    bool Notify();

    /**
        * 设置成无信号状态
        * \return	成功返回true，失败返回false
        */
    bool Reset();


    /**
        * 判断当前Mutex是否被锁, false:表示没有被锁;true:表示已经被锁.
        * \return	成功返回true，失败返回false
        */
    bool IsLocked()
    {
        return m_bLocked;
    }

protected:

private:
    JEvent event;

    bool    m_bLocked;
};

/////////////////////////////////////////////////////////////////////
/////////////////

template <class T>
        class MsgedEvent : public MTSEvent
{
public:
    MsgedEvent()   {}
    MsgedEvent   (const T& msg) : m_dwData(msg){}
    void operator =   (const T& msg)    { m_dwData = msg;}
    void SetData (const T& dwData) {m_dwData = dwData;}
    T    GetData () const          {return m_dwData;}

private:
    T m_dwData;
};

#endif //__MTS_EVENT_H__
