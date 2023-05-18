#include "arithmeticStatic.h"
#include "staticcorrection.h"
#include "g12common.h"
#include "common/jfile.h"
#include "stringcommon.h"
#include "g12globalenv.h"
#include "filevariable/ifilevariable.h"


ArithmeticStatic::ArithmeticStatic(string strChannel):m_strChannel(strChannel)
{
    m_pCFastStaticCorrection = new CFastStaticCorrection();
    m_pFactor = NULL;
    m_pfcorrectImage = NULL;
    m_iwidth = 0;
    m_iHeight = 0;
    m_bInit = false;
	m_DisEnable = false;
	m_bFloatFlag = false;
    m_mutex.Init();

}

ArithmeticStatic::~ArithmeticStatic()
{
    if(m_pCFastStaticCorrection != NULL)
    {
        delete m_pCFastStaticCorrection;
        m_pCFastStaticCorrection = NULL;
    }
}

string ArithmeticStatic::getArithmeticName()
{
    return "StaticCorrection";
}

void ArithmeticStatic::uninit()
{
    //m_pCFastStaticCorrection->ReleaseStatCorr();
    m_bInit = false;
    return ;
}

int ArithmeticStatic::Teaching(unsigned char *data, int imageWidth, int imageHeight, int standardGv, int startLine, int endLine, int startCol, int endCol, float *factors)
{
	IFileVariable* pFileVariable = getCurrentStaticCorrectVariable();
	if (pFileVariable != NULL)
	{
		bool bDisable = pFileVariable->getValue(m_strChannel, STATICCORRECT_DISABLE).toInt();
		if (bDisable)
		{
			return 0;
		}
	}

	return m_pCFastStaticCorrection->Teaching(data, imageWidth, imageHeight, standardGv, startLine, endLine, startCol,endCol, factors);
}

int ArithmeticStatic::iniStatCorr(int imageWidth,int icorrectHeight)
{
    CISAPUGuard<JMutex> autoLocker(m_mutex);
	IFileVariable* pFileVariable = getCurrentStaticCorrectVariable();
	if (pFileVariable != NULL)
	{
		m_DisEnable = pFileVariable->getValue(m_strChannel, STATICCORRECT_DISABLE).toInt();
		if (m_DisEnable)
		{
			return 0;
		}
	}
    if (m_bInit && imageWidth == m_iwidth && icorrectHeight == m_iHeight)
    {
        return 0;
    }
    if(!readFactorFile())
    {
        return -1;
    }

    m_iwidth =  imageWidth;
    m_iHeight = icorrectHeight;
	int iIRet = 0;
	iIRet = m_pCFastStaticCorrection->InitStatCorr(imageWidth, icorrectHeight, m_pFactor);
    if (iIRet == 0 )
    {
        m_bInit = true;
    }   
    return iIRet;
}

int ArithmeticStatic::StatCorr(unsigned char *image, unsigned char *correctImage)
{
	CISAPUGuard<JMutex> autoLocker(m_mutex);

	if (m_DisEnable)
	{
		return 0;
	}

	if (!m_bInit)
	{
		return -1;
	}

	if (m_pCFastStaticCorrection->StatCorr((unsigned char *)image, correctImage) < 0)
	{
		return -1;
	}

	return 0;
}

int ArithmeticStatic::GetStatParam(unsigned char *image, int imageWidth, int imageHeight, unsigned char &maxGray, unsigned char &minGray, double &mean, double &devi)
{
    CISAPUGuard<JMutex> autoLocker(m_mutex);
   
	if (m_DisEnable)
	{
		return 0;
	}

    if(m_pCFastStaticCorrection->GetStatParam(image,  imageWidth,  imageHeight, maxGray, minGray, mean, devi) <0)
    {

        return -1;
    }
    return 0;
}

bool ArithmeticStatic::readFactorFile()
{
    string fileName =  getCurrentProgramPath();
    fileName += "/"  +m_strChannel + ".fct";
    JFile fctFile(fileName);
    if(!fctFile.exists(fileName) || !fctFile.open("r"))
    {
        return false;
    }
    if (!fctFile.isOpen())
    {
        return false;
    }
    int isize = fctFile.size();
    char *pBuf = new char[isize+1];
    memset(pBuf,0,isize+1);
    int isize2 = fctFile.read(pBuf,isize);
    if(isize2 <0)
    {
        delete pBuf;
        fctFile.close();
        return false;
    }
    std::vector<std::string> factor= SString::split(pBuf,",");
	if (m_pFactor != NULL)
	{
		delete m_pFactor;
		m_pFactor = NULL;
	}
	m_pFactor = new float[factor.size()];

	for (int i = 0; i < factor.size(); i++)
	{
		m_pFactor[i] = SString::strtoFloat(factor[i]);
	}

	//TEST
	//for (int i = 0; i < 1500;i++)
	//{
	//	m_pFactor[i] = 1;
	//}
	//for (int i = factor.size()-1500; i < factor.size(); i++)
	//{
	//	m_pFactor[i] = 1;
	//}
	//END TEST

    delete pBuf;
    fctFile.close();

    return true;
}

ArithmeticStaticMgr::ArithmeticStaticMgr()
{

}

ArithmeticStaticMgr::~ArithmeticStaticMgr()
{
    map<string ,ArithmeticStatic*>::iterator it = m_map.begin();
    while(it != m_map.end())
    {
        delete it->second;
        it++;
    }
    m_map.clear();
}

IArithmeticStatic *ArithmeticStaticMgr::getArithmeticStatic(const string &strChannel)
{
    ArithmeticStatic * pIArithmeticStatic = NULL;
    if(m_map.find(strChannel) == m_map.end())
    {
        pIArithmeticStatic  = new ArithmeticStatic(strChannel);
        m_map.insert(pair<string ,ArithmeticStatic*>(strChannel,pIArithmeticStatic));
    }
    return m_map[strChannel];
}
