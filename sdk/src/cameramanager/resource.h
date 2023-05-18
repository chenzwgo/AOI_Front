#ifndef _CAMERAMANAGERPLUGIN_RESOUCE_H_
#define _CAMERAMANAGERPLUGIN_RESOUCE_H_
#include "isdkresource.h"

class IBufferManager;
class IBufferManagerFactory;

class CCameraManagerPluginResource :public ISDKResource
{
public:
    CCameraManagerPluginResource(void);
    ~CCameraManagerPluginResource(void);

    bool Init() ;
    bool UnInit() ;

    void* queryScriptObject(const int iid) ;

    void* queryPriInterface(const int iid);

private:
    bool m_bInit;
    IBufferManager     *m_pBufferManager;
	IBufferManagerFactory* m_pBufferManagerFac;
};



#endif

