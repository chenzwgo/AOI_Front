#include "resource.h"
#include "dllexport.h"
#include "dalsamanager.h"

CGigeResource::CGigeResource(void):
    m_bInit(false),
	m_pDalsaCamManager(NULL)
{

}


CGigeResource::~CGigeResource(void)
{

}

bool CGigeResource::Init()
{
    if (m_bInit)
    {
        return true;
    }

    m_pDalsaCamManager = CDalsaGigeCameraManager::getInstance();
	if (NULL != m_pDalsaCamManager)
	{
	   // m_pDalsaCamManager->init();
	}
    m_bInit = true;
    return true;
}

bool CGigeResource::UnInit()
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

void *CGigeResource::queryScriptObject(const int iid)
{
	iid;
    return NULL;
}

void *CGigeResource::queryPriInterface(const int iid)
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

#if defined(DALSA_LINE_RESOURCE_LIBRARY)
#  define RESOURCE_LIBRARY_EXPORT  Q_DECL_EXPORT
#else
#  define RESOURCE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

ISDKResource* m_pResource = 0;
extern "C" RESOURCE_LIBRARY_EXPORT ISDKResource* CreateResource(void)
{
    if (m_pResource == 0)
    {
        m_pResource = new CGigeResource();
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
