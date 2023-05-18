#include "Features.h"
#include "csvparser.h"

#define  BaseFeaturesPath           "./plugins/defectclassificationuiplugin/BaseFeatures.csv"
#define  DerivedFeaturesPath        "./plugins/defectclassificationuiplugin/DerivedFeatures.csv"

CFeatures* CFeatures::m_pInstance = NULL;

CFeatures* CFeatures::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CFeatures();
    }
    return m_pInstance;
}

void CFeatures::releaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance ;
        m_pInstance = NULL;
    }
    return ;
}


CFeatures::CFeatures(void):m_pCCSVParserBaseFeatures(NULL),
                           m_pCCSVParserDerivedFeatures(NULL) 
{
    m_pCCSVParserBaseFeatures = new CCSVParser();
    m_pCCSVParserDerivedFeatures = new CCSVParser();
    readBaseFeatureData();
    readDerivedFeatureData();

}


CFeatures::~CFeatures(void)
{
    
}


bool CFeatures::readBaseFeatureData()
{
    m_pCCSVParserBaseFeatures->setFileName(BaseFeaturesPath);

    if (!m_pCCSVParserBaseFeatures->readFileContent())
    {
        return false;
    }

    unsigned int irow = m_pCCSVParserBaseFeatures->rowCount() ;
    for (int i = 1 ;i < irow;i++)
    {
        stFeaturesInfo info;

        info.strFeaturesName = m_pCCSVParserBaseFeatures->getItemElement(i,1);
        info.strFeaturesUnit = m_pCCSVParserBaseFeatures->getItemElement(i,2);
        info.strFeaturesDefaultValue = m_pCCSVParserBaseFeatures->getItemElement(i,3);
        info.strFeaturesDescription = m_pCCSVParserBaseFeatures->getItemElement(i,4);

        m_mapBaseFeatures.insert(pair<string,stFeaturesInfo>(info.strFeaturesName,info));
    }
    
    return true;
}


bool CFeatures::readDerivedFeatureData()
{ 
    m_pCCSVParserDerivedFeatures->setFileName(DerivedFeaturesPath);

    if (!m_pCCSVParserDerivedFeatures->readFileContent())
    {
        return false;
    }

    unsigned int irow = m_pCCSVParserBaseFeatures->rowCount() ;
    for (int i = 1 ;i < irow;i++)
    {
        stFeaturesInfo info;

        info.strFeaturesName = m_pCCSVParserBaseFeatures->getItemElement(i,1);
        info.strFeaturesUnit = m_pCCSVParserBaseFeatures->getItemElement(i,2);
        info.strFeaturesDefaultValue = m_pCCSVParserBaseFeatures->getItemElement(i,3);
        info.strFeaturesDescription = m_pCCSVParserBaseFeatures->getItemElement(i,4);

        m_mapDerivedFeatures.insert(pair<string,stFeaturesInfo>(info.strFeaturesName,info));
    }
    return true;
}


stFeaturesInfo CFeatures::getBaseFeature( const string& FeaturesName )
{
    map<string,stFeaturesInfo>::iterator it = m_mapBaseFeatures.find(FeaturesName);
    if (it == m_mapBaseFeatures.end())
    {
        return stFeaturesInfo();
    }
    return it->second;
}


stFeaturesInfo CFeatures::getDerivedFeature( const string& FeaturesName )
{
    map<string,stFeaturesInfo>::iterator it = m_mapDerivedFeatures.find(FeaturesName);
    if (it == m_mapDerivedFeatures.end())
    {
        return stFeaturesInfo();
    }
    return it->second;
}


const map<string,stFeaturesInfo>* CFeatures::getBaseFeaturesMap()
{
    return &m_mapBaseFeatures;
}

const map<string,stFeaturesInfo>* CFeatures::getDerivedFeaturesMap()
{
    return &m_mapDerivedFeatures;
}

bool CFeatures::isBaseFeatureExist( const string& FeaturesName )
{
    map<string,stFeaturesInfo>::iterator it = m_mapBaseFeatures.find(FeaturesName);
    if (it == m_mapBaseFeatures.end())
    {
        return false;
    }
    return true;
}


bool CFeatures::isDerivedFeatureExist( const string& FeaturesName )
{
    map<string,stFeaturesInfo>::iterator it = m_mapDerivedFeatures.find(FeaturesName);
    if (it == m_mapDerivedFeatures.end())
    {
        return false;
    }
    return true;
}
