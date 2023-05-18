#include "resource.h"
#include "dllexport.h"
#include "cameramanagerplugin.h"
#include "linecammanagerplugin.h"
#include "buffermanager.h"
#include "buffermanager.h"


CCameraManagerPluginResource::CCameraManagerPluginResource(void):
    m_bInit(false)
    , m_pBufferManager(NULL)
	, m_pBufferManagerFac(NULL)
{

}


CCameraManagerPluginResource::~CCameraManagerPluginResource(void)
{

}

bool CCameraManagerPluginResource::Init()
{
    if (m_bInit)
    {
        return true;
    }
    CLineCamManagerPlugin::getInstance()->Init();
    m_bInit = true;
    return true;
}

bool CCameraManagerPluginResource::UnInit()
{
    if (!m_bInit)
    {
        return true;
    }
	if (m_pBufferManagerFac != NULL)
	{
		BufferManagerFactory::releaseInstance();
		m_pBufferManagerFac = NULL;
	}
    CLineCamManagerPlugin::ReleaseInstance();
    m_bInit = false;
    return true;
}

void *CCameraManagerPluginResource::queryScriptObject(const int iid)
{
    return NULL;
}

void *CCameraManagerPluginResource::queryPriInterface(const int iid)
{
	if (iid == ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE)
    {
        return CLineCamManagerPlugin::getInstance();
    }
	else if (iid == IBufferManagerFactory::IID_CAMERA_BUFFER_MANAGER_FACTORY)
	{
		if (NULL == m_pBufferManagerFac)
		{
			m_pBufferManagerFac = (IBufferManagerFactory *)BufferManagerFactory::getInstance();
		}
		return m_pBufferManagerFac;
	}

    return NULL;
}

#if defined(CAMERAMANAGERPLUGIN_RESOURCE_LIBRARY)
#  define RESOURCE_LIBRARY_EXPORT  Q_DECL_EXPORT
#else
#  define RESOURCE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

ISDKResource* m_pResource = 0;
extern "C" RESOURCE_LIBRARY_EXPORT ISDKResource* CreateResource(void)
{
    if (m_pResource == 0)
    {
        m_pResource = new CCameraManagerPluginResource();
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

