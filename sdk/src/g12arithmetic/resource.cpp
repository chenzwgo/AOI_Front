#include "resource.h"
#include "dllexport.h"
#include "arithmeticStatic.h"
#include "arithmeticmeasuresize.h"
#ifndef NULL
#define NULL 0
#endif

CArithmeticResource::CArithmeticResource(void):
    m_bInit(false)
{
    m_pArithmeticStaticMgr = NULL;
	m_pArithmeticMeasureSize = NULL;
}


CArithmeticResource::~CArithmeticResource(void)
{

}

bool CArithmeticResource::Init()
{
    if (m_bInit)
    {
        return true;
    }

    m_pArithmeticStaticMgr = new ArithmeticStaticMgr();
	m_pArithmeticMeasureSize = new ArithmeticMeasureSize();
    m_bInit = true;
    return true;
}

bool CArithmeticResource::UnInit()
{
    if (m_pArithmeticStaticMgr != NULL)
    {
        delete m_pArithmeticStaticMgr;
        m_pArithmeticStaticMgr = NULL;
    }
    if(m_pArithmeticMeasureSize != NULL)
    {
        delete m_pArithmeticMeasureSize;
		m_pArithmeticMeasureSize = NULL;
    }
    m_bInit = false;
    return true;
}

void *CArithmeticResource::queryScriptObject(const int iid)
{
    return NULL;
}

void *CArithmeticResource::queryPriInterface(const int iid)
{
    if(!m_bInit)
    {
        return NULL;
    }

    if (iid == IArithmeticStaticMgr::IDD_ARITHMETICSTATICMGR)
    {
        return m_pArithmeticStaticMgr;
    }
	else if (iid == IArithmeticMeasureSize::IID_MEASURE_SIZE)
    {
        return m_pArithmeticMeasureSize;
    }
    return NULL;
}

#if defined(G12ARITHMETIC_RESOURCE_LIBRARY)
#  define RESOURCE_LIBRARY_EXPORT  Q_DECL_EXPORT
#else
#  define RESOURCE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

ISDKResource* m_pResource = 0;
extern "C" RESOURCE_LIBRARY_EXPORT ISDKResource* CreateResource(void)
{
    if (m_pResource == 0)
    {
        m_pResource = new CArithmeticResource();
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

