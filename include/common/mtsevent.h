/**
* \file mtsevent.h(based on old version MTSEvent, but use QT instead)
*
* \brief �¼���Ķ��壬����Ϊ���ȴ�һ�������¼������Խ��ź����ó��ޡ����ź�״̬
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
        * ���캯��
        */
    MTSEvent();
    ~MTSEvent();
    

    /**
        * ����, �ȴ���ʱ����
        * \param dwTimeout
        *   [DWORD] ��ʱʱ��
        * \return	�ɹ�����true��ʧ�ܷ���false
        */
    bool Wait(unsigned long dwTimeout = 0);

    /**
        * ����
        * \return	�ɹ�����true��ʧ�ܷ���false
        */
    bool Notify();

    /**
        * ���ó����ź�״̬
        * \return	�ɹ�����true��ʧ�ܷ���false
        */
    bool Reset();


    /**
        * �жϵ�ǰMutex�Ƿ���, false:��ʾû�б���;true:��ʾ�Ѿ�����.
        * \return	�ɹ�����true��ʧ�ܷ���false
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
