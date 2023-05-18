/*

    This file is a part of the JThread package, which contains some object-
    oriented thread wrappers for different thread implementations.

    Copyright (c) 2000-2004  Jori Liesenborgs (jori@lumumba.luc.ac.be)

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

*/

#ifndef JTHREAD_H

#define JTHREAD_H

#include "jmutex.h"
#include <string>
using namespace  std;

#ifndef Sleep
#ifdef  Q_OS_WIN
#include <Windows.h>    //using win api Sleep
#else
#include <unistd.h>
#define Sleep(n)   ::usleep((n)*1000)
#endif
#endif

#define ERR_JTHREAD_CANTINITMUTEX						-1
#define ERR_JTHREAD_CANTSTARTTHREAD						-2
#define ERR_JTHREAD_THREADFUNCNOTSET						-3
#define ERR_JTHREAD_NOTRUNNING							-4
#define ERR_JTHREAD_ALREADYRUNNING						-5

class JThread
{
public:
	JThread();
	virtual ~JThread();
	int Start();
	int Kill();
	virtual void *Thread() = 0;
	bool IsRunning();
	void *GetReturnValue();
    //static void sleep(unsigned long msec);
    const char* GetCallClassName();
protected:
    void ThreadStarted();   //called behind the set value of thread run function
    void SetThreadClassName(const char* className);
private:
#ifdef _WIN32
	static DWORD WINAPI TheThread(void *param);
	
	HANDLE m_threadhandle;
	DWORD m_threadid;
#else // pthread type threads
	static void *TheThread(void *param);
	
	pthread_t m_threadid;
#endif // WIN
	void *m_retval;
	bool m_running;

	JMutex m_runningmutex;
	JMutex m_continuemutex,m_continuemutex2;
	bool m_mutexinit;
    string m_strClassName;

};

#endif // JTHREAD_H

