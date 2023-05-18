#include "resource.h"
#include "dllexport.h"
#include "hiksourcecameramanager.h"


CHIKSourceCameraResource::CHIKSourceCameraResource(void):
    m_bInit(false)
{

}


CHIKSourceCameraResource::~CHIKSourceCameraResource(void)
{
    CHIKSourceCameraManagerPlugin::releaseInstance();
}

bool CHIKSourceCameraResource::Init()
{
    if (m_bInit)
    {
        return true;
    }

    m_bInit = CHIKSourceCameraManagerPlugin::getInstance()->init();
    return true;
}

bool CHIKSourceCameraResource::UnInit()
{
    if (!m_bInit)
    {
        return true;
    }
    bool bRet = CHIKSourceCameraManagerPlugin::getInstance()->deinit();
    m_bInit = !bRet;
    return true;
}

void *CHIKSourceCameraResource::queryScriptObject(const int iid)
{
    return NULL;
}

void *CHIKSourceCameraResource::queryPriInterface(const int iid)
{
    if(ICameraFactory::IID_CAMERAFACTORY == iid)
    {
        return CHIKSourceCameraManagerPlugin::getInstance();
    }
    else
    {
        return NULL;
    }
    return NULL;
}

#if defined(HIKINGSOURCECAMERA_RESOURCE_LIBRARY)
#  define RESOURCE_LIBRARY_EXPORT  Q_DECL_EXPORT
#else
#  define RESOURCE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

ISDKResource* m_pResource = 0;
extern "C" RESOURCE_LIBRARY_EXPORT ISDKResource* CreateResource(void)
{
    if (m_pResource == 0)
    {
        m_pResource = new CHIKSourceCameraResource();
    }
    return m_pResource;
}
extern "C" RESOURCE_LIBRARY_EXPORT void ReleaseResource(void)
{
    if (m_pResource != 0)
    {
        delete m_pResource;
        m_pResource = 0;
    }
}

