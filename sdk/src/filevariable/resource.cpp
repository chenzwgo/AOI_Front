#include "resource.h"
#include "dllexport.h"
#include "./filevariable/filevariable.h"

CFileVariableResource::CFileVariableResource(void):
    m_bInit(false)
{
    
}


CFileVariableResource::~CFileVariableResource(void)
{
	CFileVariableMap::DestroyInstance();
}

bool CFileVariableResource::Init()
{
    if (m_bInit)
    {
        return true;
    }
    CFileVariableMap::GetInstance();
    m_bInit = true;
    return true;
}

bool CFileVariableResource::UnInit()
{
    m_bInit = false;
    return true;
}

void *CFileVariableResource::queryScriptObject(const int iid)
{
    return NULL;
}

void *CFileVariableResource::queryPriInterface(const int iid)
{
    if(!m_bInit)
    {
        return NULL;
    }

    if(iid == IFileVariableManager::IID_FILEVARIABLEMANAGER)
    {
        return CFileVariableMap::GetInstance();
    }
    return NULL;
}

#if defined(FILEVARIABLE_RESOURCE_LIBRARY)
#  define RESOURCE_LIBRARY_EXPORT  Q_DECL_EXPORT
#else
#  define RESOURCE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

ISDKResource* m_pResource = 0;
extern "C" RESOURCE_LIBRARY_EXPORT ISDKResource* CreateResource(void)
{
    if (m_pResource == 0)
    {
        m_pResource = new CFileVariableResource();
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

