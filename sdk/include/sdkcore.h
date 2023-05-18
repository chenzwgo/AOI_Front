#ifndef _SDK_CORE_H_
#define _SDK_CORE_H_

#include "sdkcontainer/isdkcontainer.h"
#include "isdkresource.h"
#include "dllexport.h"

class RT_API sdkcore
{
private:
    sdkcore();
public:
    static ISDKContainer* GetSDKContainer();
    static ISDKResource*  GetSDKResource(const char* pszCLSID);
    static void* queryPriInterface(const char* pszCLSID, const int iid);
    static void* queryScriptObject(const char* pszCLSID, const int iid);
    static void SetSDKContainer(ISDKContainer* pSDKContainer);

private:
    static ISDKContainer* m_pSDKContainer;
};

#endif
