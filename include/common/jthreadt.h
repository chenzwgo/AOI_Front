#ifndef JTHREADT_H
#define JTHREADT_H

#include "jthread.h"
#include <time.h>
#include <typeinfo>
template<class T>
class ParamTraitor	//对所有类型T提取参数类型，并定义callertype为int
{
public:
        typedef T ParamType;
        typedef int CallerType;
};

template<>
class ParamTraitor<void>//对void类型特化模版：参数类型定义为char，并定义callertype为char
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
                CallFunc(CallerType());//通锟斤拷同锟斤拷callertype锟斤拷锟斤拷锟斤拷锟截猴拷锟斤拷统一锟斤拷锟斤拷void锟酵凤拷void锟斤拷锟?
                return 0L;
        }

        int Kill()
        {
                return JThread::Kill();
        }

private:
        ParamType m_Para;	/*!< 锟斤拷锟斤拷锟斤拷锟?*/
        T_FUNC m_Func;		/*!< 锟竭筹拷执锟叫猴拷锟斤拷 */
        T_CLASS* m_pTClass;	/*!< 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷实锟斤拷 */
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
                CallFunc(CallerType());//通过不同的callertype访问重载函数，统一处理void和非void情况
                return 0L;
        }

        int Kill()
        {
                return JThread::Kill();
        }

private:
        ParamType m_Para;	/*!< 函数参数 */
        T_FUNC m_Func;		/*!< 线程执行函数 */
        T_CLASS* m_pTClass;	/*!< 函数所在类实例 */
};

#endif // JTHREADT_H
