#ifndef _ISCRIPTOBJWRAP_H_
#define _ISCRIPTOBJWRAP_H_
#include "iscriptcallfunciton.h"
#include <string>
using namespace  std;

class QObject;
class IScriptObjWrap :public IScriptCallFunciton
{
public:

    virtual ~IScriptObjWrap() {}

    virtual string GetInterfaceDict() = 0;

    virtual const char* GetFunctionDict(const string& pFunctionName) = 0;

    virtual unsigned int GetInterfaceID() = 0;

    virtual string GetLastError() = 0;

    virtual bool IsFunctionExist(const string& pFunctionName) = 0;

    virtual string GetInterfaceXML() =0;

    virtual string GetInterfaceHeadFile() = 0;

    virtual QObject* GetInterfaceObject() = 0;


};
#endif
