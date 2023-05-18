#ifndef JTHREADT_H
#define JTHREADT_H

#include "jthread.h"
#include <time.h>
#include <typeinfo>
template<class T>
class ParamTraitor	//����������T��ȡ�������ͣ�������callertypeΪint
{
public:
        typedef T ParamType;
        typedef int CallerType;
};

template<>
class ParamTraitor<void>//��void�����ػ�ģ�棺�������Ͷ���Ϊchar��������callertypeΪchar
{
public:
        typedef char ParamType;
        typedef char CallerType;
};


template<class T_CLASS, class PARA , class RET = void>
class JThreadTP;

template<class T_CLASS, class PARA, class RET>
class JThreadTP : public JThread
{
        typedef typename ParamTraitor<PARA>::ParamType ParamType;
        typedef typename ParamTraitor<PARA>::CallerType CallerType;

        typedef RET (T_CLASS::*T_FUNC)(PARA);
public:
        JThreadTP(){JThread::SetThreadClassName(typeid(T_CLASS).name());}

        ~JThreadTP()
        {
                EndThread();
        }

        int StartThread(T_CLASS* pTClass, T_FUNC Func, ParamType Param = 0)
        {
                if (!IsRunning())
                {
                        m_Para = Param;
                        m_pTClass = pTClass;
                        m_Func = Func;
                }

                return JThread::Start();
        }

        void EndThread(unsigned int unMaxWait = 5)
        {
                if (!IsRunning())
                {
                        return;
                }

                time_t t = time(NULL);
                while (IsRunning() && (time(NULL)-t) < (time_t)unMaxWait) // wait max. 5 seconds
                {
                        Sleep(80);
                }
				
                if (IsRunning())
                {
                        Kill();
                }
        }

private:
        int Start()/*{ return JThread::Start(); }*/;

        void CallFunc(int )
        {
                if (m_pTClass)
                {
                        (m_pTClass->*m_Func)(m_Para);
                }
        }

        void CallFunc(char)
        {
                if (m_pTClass)
                {
                        (m_pTClass->*m_Func)();
                }
        }

        void* Thread()
        {
                JThread::ThreadStarted();
                CallFunc(CallerType());//ͨ��ͬ��callertype�������غ���ͳһ����void�ͷ�void���?
                return 0L;
        }

        int Kill()
        {
                return JThread::Kill();
        }

private:
        ParamType m_Para;	/*!< �������?*/
        T_FUNC m_Func;		/*!< �߳�ִ�к��� */
        T_CLASS* m_pTClass;	/*!< ����������ʵ�� */
};

template<class T_CLASS ,class RET = void>
class JThreadT : public JThread
{
        typedef typename ParamTraitor<void>::ParamType ParamType;
        typedef typename ParamTraitor<void>::CallerType CallerType;

        typedef RET (T_CLASS::*T_FUNC)(void);
public:
        JThreadT(){JThread::SetThreadClassName(typeid(T_CLASS).name());}

        ~JThreadT()
        {
                EndThread();
        }

        int StartThread(T_CLASS* pTClass, T_FUNC Func, ParamType Param = 0)
        {
                if (!IsRunning())
                {
                        m_Para = Param;
                        m_pTClass = pTClass;
                        m_Func = Func;

                }

                return JThread::Start();
        }

        void EndThread(int unMaxWait = 5)
        {
                if (!IsRunning())
                {
                        return;
                }

                time_t t = time(NULL);
                while (IsRunning() && (time(NULL)-t) < (time_t)unMaxWait) // wait max. 5 seconds
                {
                        Sleep(80);
                }

                if (IsRunning())
                {
                        Kill();
                }
        }
private:
        int Start();

        void CallFunc(int )
        {
                if (m_pTClass)
                {
                        (m_pTClass->*m_Func)(m_Para);
                }
        }

        void CallFunc(char)
        {
                if (m_pTClass)
                {
                        (m_pTClass->*m_Func)();
                }
        }

        void* Thread()
        {
                JThread::ThreadStarted();
                CallFunc(CallerType());//ͨ����ͬ��callertype�������غ�����ͳһ����void�ͷ�void���
                return 0L;
        }

        int Kill()
        {
                return JThread::Kill();
        }

private:
        ParamType m_Para;	/*!< �������� */
        T_FUNC m_Func;		/*!< �߳�ִ�к��� */
        T_CLASS* m_pTClass;	/*!< ����������ʵ�� */
};

#endif // JTHREADT_H
