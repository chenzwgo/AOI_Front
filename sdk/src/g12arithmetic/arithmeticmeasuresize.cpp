#include "arithmeticmeasuresize.h"
#include "g12common.h"
#include "common/jfile.h"
#include "stringcommon.h"
#include "g12globalenv.h"
#include "filevariable/ifilevariable.h"

ArithmeticMeasureSize::ArithmeticMeasureSize()
{
    m_iwidth = 0;
    m_iHeight = 0;
    m_bInit = false;
    m_mutex.Init();	
	m_CMeasureSize = NULL;// new MeasureSize();
}

ArithmeticMeasureSize::~ArithmeticMeasureSize()
{
	if (m_CMeasureSize != NULL)
	{
		delete m_CMeasureSize;
		m_CMeasureSize = NULL;
	}
}

string ArithmeticMeasureSize::getArithmeticName()
{
    return "measuresize";
}

void ArithmeticMeasureSize::uninit()
{
    m_bInit = false;
	if (NULL != m_CMeasureSize)
	{
		delete m_CMeasureSize;
		m_CMeasureSize = NULL;
		//m_CMeasureSize->release();
	}
    return ;
}

int ArithmeticMeasureSize::init(int imageWidth, int imageHeight, double mmPerX/*=1*/, double mmPerY/*=1*/)
{
	CISAPUGuard<JMutex> autoLocker(m_mutex);
	if (m_bInit)
	{
		return 0;
	}

	m_iwidth = imageWidth;
	m_iHeight = imageHeight;

	IFileVariable *pCameraSetCfgFile = getCameraSetCfgFileVariable();
	if (pCameraSetCfgFile == NULL)
	{
		return -1;
	}

	IFileVariable* pFileVariable = getCurrentRoiVariable();
	if (pFileVariable == NULL)
	{
		return -1;
	}

	int roix1 = pFileVariable->getValue(ROI, ROI_X1).toInt();
	int roiy1 = pFileVariable->getValue(ROI, ROI_Y1).toInt();
	int roix2 = pFileVariable->getValue(ROI, ROI_X2).toInt();
	int roiy2 = pFileVariable->getValue(ROI, ROI_Y2).toInt();
	int iIRet = 0;

	if (NULL == m_CMeasureSize)
	{
		m_CMeasureSize = new MeasureSize(imageWidth, imageHeight,roix1,roiy1,roix2,roiy2);		
	}
	if (m_CMeasureSize != NULL)
	{
		m_CMeasureSize->setmmPerPix(mmPerX, mmPerY);
	}
	
	m_bInit = true;
	return iIRet;

}

int ArithmeticMeasureSize::calcSize(unsigned char* srcImage)
{
	int nRet = 0;
	if (NULL != m_CMeasureSize)
	{
		nRet = m_CMeasureSize->calcSize(srcImage);
	}
	return nRet;
}

int ArithmeticMeasureSize::getResultPix(map<string, double> &result)
{
	int nRet = 0;
	if (NULL != m_CMeasureSize)
	{
		nRet = m_CMeasureSize->getResultPix(result);
	}
	return nRet;
}

int ArithmeticMeasureSize::getResultmm(map<string, double> &result)
{
	int nRet = 0;
	if (NULL != m_CMeasureSize)
	{
		nRet = m_CMeasureSize->getResultmm(result);
	}
	return nRet;
}

int ArithmeticMeasureSize::getResultImage(unsigned char* pImgResult)
{
	if (NULL != m_CMeasureSize)
	{
		
	}

	return 0;
}
