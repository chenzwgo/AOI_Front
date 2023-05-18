#ifndef _FILEVARIABLE_RESOUCE_H_
#define _FILEVARIABLE_RESOUCE_H_
#include "isdkresource.h"


class CFileVariableResource :public ISDKResource
{
public:
    CFileVariableResource(void);
    ~CFileVariableResource(void);

    bool Init() ;
    bool UnInit() ;

    void* queryScriptObject(const int iid) ;

    void* queryPriInterface(const int iid);

private:
    bool m_bInit;
};



#endif  //_FILEVARIABLE_RESOUCE_H_

