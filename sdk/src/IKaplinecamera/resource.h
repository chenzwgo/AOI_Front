#ifndef _CVS_IKAP_CAMERATOOLS_RESOUCE_H_
#define _CVS_IKAP_CAMERATOOLS_RESOUCE_H_
#include "isdkresource.h"

class CIKAPCameraManager;
class CVSCameratools :public ISDKResource
{
public:
    CVSCameratools(void);
    ~CVSCameratools(void);

    bool Init() ;
    bool UnInit() ;

    void* queryScriptObject(const int iid) ;

    void* queryPriInterface(const int iid);

private:
    bool m_bInit;
    CIKAPCameraManager *m_pDalsaCamManager;
};



#endif // _CVSCAMERATOOLS_RESOUCE_H_
