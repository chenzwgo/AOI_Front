#ifndef _HIKSOURCECAMERA_RESOUCE_H_
#define _HIKSOURCECAMERA_RESOUCE_H_
#include "isdkresource.h"


class CHIKSourceCameraResource :public ISDKResource
{
public:
    CHIKSourceCameraResource(void);
    ~CHIKSourceCameraResource(void);

    bool Init() ;
    bool UnInit() ;

    void* queryScriptObject(const int iid) ;

    void* queryPriInterface(const int iid);

private:
    bool m_bInit;
};



#endif

