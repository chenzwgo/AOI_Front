#ifndef _SDK_COMMON_RESOUCE_H_
#define _SDK_COMMON_RESOUCE_H_
#include "isdkresource.h"


class CSdkCommonResource :public ISDKResource
{
public:
    CSdkCommonResource(void);
    ~CSdkCommonResource(void);

    bool Init() ;
    bool UnInit() ;

    void* queryScriptObject(const int iid) ;

    void* queryPriInterface(const int iid);

private:
    bool m_bInit;
};



#endif

