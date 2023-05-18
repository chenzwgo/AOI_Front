#ifndef _DEFECT_QUALIFICATION_RESOUCE_H_
#define _DEFECT_QUALIFICATION_RESOUCE_H_
#include "isdkresource.h"


class CDefectResource :public ISDKResource
{
public:
    CDefectResource(void);
    ~CDefectResource(void);

    bool Init() ;
    bool UnInit() ;

    void* queryScriptObject(const int iid) ;

    void* queryPriInterface(const int iid);

private:
    bool m_bInit;
};



#endif  //_DEFECT_QUALIFICATION_RESOUCE_H_

