#ifndef PARSEERRORMESSAGE_H
#define PARSEERRORMESSAGE_H
#include <iostream>
#include <map>
#include "../socket/udpsocket.h"
#include "sdkcommon/errorcode/iparseerrormessage.h"
#include "jmutex.h"
using namespace std;
class ParseErrorMessage:public IParseErrorMessage
{
public:
    ParseErrorMessage();
    bool parseErrorMessage();
    const char* FindIDmessage(int errorid);
	void sendLogMessage(int logType,string logMoudle,string logContent);
    static ParseErrorMessage *GetInstance();
    static void DestroyInstance();
private:
    map<int,string> m_errorInfor;
    bool m_isParseComplete;
	udpsocket m_udpsocket;
	 JMutex m_MutexLock;		 
	static ParseErrorMessage *m_self;
};

#endif // PARSEERRORMESSAGE_H
