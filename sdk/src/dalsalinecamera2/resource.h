#ifndef _CVS_DH_CAMERATOOLS_RESOUCE_H_
#define _CVS_DH_CAMERATOOLS_RESOUCE_H_
#include "isdkresource.h"

class CDalsaCameraManager;
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
	CDalsaCameraManager *m_pDalsaCamManager;
};



#endif // _CVSCAMERATOOLS_RESOUCE_H_
