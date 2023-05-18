#include "resource.h"
#include "dllexport.h"
#include "defectqualification.h"
#include "defectlist.h"

CDefectResource::CDefectResource(void)
{
	m_bInit = false;
}


CDefectResource::~CDefectResource(void)
{
	DefectQualimanage::DestroyInstance();
}

bool CDefectResource::Init()
{
    if (m_bInit)
    {
        return true;
    }
	DefectQualimanage::GetInstance();
    m_bInit = true;
    return true;
}

bool CDefectResource::UnInit()
{
    m_bInit = false;
    return true;
}

void *CDefectResource::queryScriptObject(const int iid)
{
    return NULL;
}

void *CDefectResource::queryPriInterface(const int iid)
{
    if(!m_bInit)
    {
        return NULL;
    }

   if(iid == IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE)
    {
       return DefectQualimanage::GetInstance();
    }
   else if (iid == IDefectQualimanage::IDD_DEFECT_LIST_MANAGE)
   {
	   return DefectListManage::GetInstance();
   }
   return NULL;
}

#if defined(DEFECT_QUALIFICATION_RESOURCE_LIBRARY)
#  define RESOURCE_LIBRARY_EXPORT  Q_DECL_EXPORT
#else
#  define RESOURCE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

ISDKResource* m_pResource = 0;
extern "C" RESOURCE_LIBRARY_EXPORT ISDKResource* CreateResource(void)
{
    if (m_pResource == 0)
    {
        m_pResource = new CDefectResource();
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

