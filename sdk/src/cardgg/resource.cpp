
#include "resource.h"
#include "cardmanagerservice.h"
#include "dllexport.h"
#include "define.h"

Resource::Resource(void):m_bInit(false)
{
	Init();
}


Resource::~Resource(void)
{
	UnInit();
}

bool Resource::Init()
{
	if (m_bInit)
	{
		return true;
	}

	m_bInit = true;
	return true;
}

bool Resource::UnInit()
{
	CCardManagerService::DestoryManagerService();
	return true;
}


void* Resource::queryScriptObject(const int iid)
{
    return NULL;
}

void* Resource::queryPriInterface(const int iid)
{
	if(ICardManagerInterface::IID_CARDMANAGERINTERFACE == iid)
	{
		return CCardManagerService::GetInstance();
	}
    return NULL;
}




#if defined(CARD_GG_RESOURCE_LIBRARY)
#  define RESOURCE_LIBRARY_EXPORT  Q_DECL_EXPORT
#else
#  define RESOURCE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

ISDKResource* m_pResource = 0;
extern "C" RESOURCE_LIBRARY_EXPORT ISDKResource* CreateResource(void)
{
    if (m_pResource == 0)
    {
        m_pResource = new Resource();
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
