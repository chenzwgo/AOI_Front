#include "arithmeticcoordtransform.h"
#include "g12common.h"
#include "common/jfile.h"
#include "stringcommon.h"
#include "g12globalenv.h"
#include "filevariable/ifilevariable.h"

ArithmeticCoordTransform::ArithmeticCoordTransform()
{
    m_iwidth = 0;
    m_iHeight = 0;
    m_bInit = false;
    m_mutex.Init();	
	m_coordTransform = NULL;
}

ArithmeticCoordTransform::~ArithmeticCoordTransform()
{
	if (m_coordTransform != NULL)
	{
		delete m_coordTransform;
		m_coordTransform = NULL;
	}
}

string ArithmeticCoordTransform::getArithmeticName()
{
    return "coordTransform";
}

void ArithmeticCoordTransform::uninit()
{
    m_bInit = false;
	if (NULL != m_coordTransform)
	{
		delete m_coordTransform;
		m_coordTransform = NULL;
	}
    return ;
}

int ArithmeticCoordTransform::calcOffset(unsigned char* pSrc, int width, int height, int &x, int &y, double &angle)
{
	int nRet = 0;
	if (NULL == m_coordTransform)
	{
		nRet = m_coordTransform->calcOffset(pSrc, width, height, x,y,angle);
	}

	return nRet;
}

int ArithmeticCoordTransform::initOffset(int x, int y, double angle)
{
	CISAPUGuard<JMutex> autoLocker(m_mutex);
	if (m_bInit)
	{
		return 0;
	}

	if (NULL == m_coordTransform)
	{
		m_coordTransform = new CoordTransform();
	}
	int iRet = m_coordTransform->initOffset(x, y, angle);
	m_bInit = true;
	return iRet;
}

void ArithmeticCoordTransform::map(int x, int y, int &tx, int &ty)
{
	if (NULL == m_coordTransform)
	{
		m_coordTransform->map(x, y, tx, ty);
	}
}