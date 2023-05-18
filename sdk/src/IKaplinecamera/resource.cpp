#include "resource.h"
#include "dllexport.h"
#include "IKapmanager.h"

CVSCameratools::CVSCameratools(void):
    m_bInit(false),
	m_pDalsaCamManager(NULL)
{

}


CVSCameratools::~CVSCameratools(void)
{

}

bool CVSCameratools::Init()
{
    if (m_bInit)
    {
        return true;
    }

    m_pDalsaCamManager = CIKAPCameraManager::getInstance();
	if (NULL != m_pDalsaCamManager)
	{
	    m_pDalsaCamManager->init();
	}
    m_bInit = true;
    return true;
}

bool CVSCameratools::UnInit()
{
    if (!m_bInit)
    {
        return true;
	}
	if (NULL != m_pDalsaCamManager)
	{
		m_pDalsaCamManager->deinit();
	}
    m_bInit = false;
    return true;
}

void *CVSCameratools::queryScriptObject(const int iid)
{
    return NULL;
}

void *CVSCameratools::queryPriInterface(const int iid)
{

    if(ILineCameraFactory::IID_LINE_CAMERAFACTORY == iid)
    {
        return m_pDalsaCamManager;
    }
    else
    {
        return NULL;
    }
    return NULL;
}

#if defined(IKAP_LINE_RESOURCE_LIBRARY)
#  define RESOURCE_LIBRARY_EXPORT  Q_DECL_EXPORT
#else
#  define RESOURCE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

ISDKResource* m_pResource = 0;
extern "C" RESOURCE_LIBRARY_EXPORT ISDKResource* CreateResource(void)
{
    if (m_pResource == 0)
    {
        m_pResource = new CVSCameratools();
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
