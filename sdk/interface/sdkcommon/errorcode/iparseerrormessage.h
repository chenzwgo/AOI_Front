#ifndef _I_PARSEERRORMESSAGE_H
#define _I_PARSEERRORMESSAGE_H
#include <string>
using namespace std;

class IParseErrorMessage
{
public:
    enum
    {
        IID_PARSEERRORMESSAGE= 0x205
    };
    virtual ~IParseErrorMessage(){}
    virtual const char* FindIDmessage(int errorid) = 0;
    virtual void sendLogMessage(int logType,string logMoudle,string logContent) =0;
};

#endif // PARSEERRORMESSAGE_H
