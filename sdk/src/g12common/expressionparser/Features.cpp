#include "Features.h"
#include "csvparser.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "tinyxml.h"
#include "sdkcore.h"
#include "stringcommon.h"

#define  BaseFeaturesPath           "BaseFeatures.xml"
#define  DerivedFeaturesPath        "DerivedFeatures.xml"


CFeatures::CFeatures(string strChannel):m_strChannel(strChannel)
{
    readBaseFeatureData();
    readDerivedFeatureData();
}


CFeatures::~CFeatures(void)
{
    
}


bool CFeatures::readBaseFeatureData()
{

    string strPath = getCurrentProgramPath();
    strPath += "/";
    strPath += BaseFeaturesPath;

	TiXmlDocument doc(strPath);

	bool bOk = doc.LoadFile(TIXML_ENCODING_UTF8);
	if (!bOk)
	{
		return false;
	}

	TiXmlElement *root = doc.RootElement();
	if (0 != strcmp(root->Value(), "root"))
	{
		return false;
	}
	clearDerivedFeaturesMap();
	CISAPUGuard<JMutex> autoLock(m_FeaturesMapMutex);
	if (root)
	{
		TiXmlElement *basefeatureItem = root->FirstChildElement();
		int flowIndex = 0;
		while (basefeatureItem)
		{
			const char* basefeature = basefeatureItem->Value();
			if (basefeature == NULL || strcmp(basefeature, "basefeature") != 0)
			{
				basefeatureItem = basefeatureItem->NextSiblingElement();
				continue;
			}

			stFeaturesInfo baseInfo;
			const char* pName = basefeatureItem->Attribute("name");
			if (pName != NULL)
			{
				baseInfo.strName = pName;
			}

			pName = basefeatureItem->Attribute("Unit");
			if (pName != NULL)
			{
				baseInfo.strUnit = pName;
			}

			pName = basefeatureItem->Attribute("Default");
			if (pName != NULL)
			{
				baseInfo.strDefaultValue = pName;
			}

			pName = basefeatureItem->Attribute("Description");
			if (pName != NULL)
			{
				baseInfo.strDescription = pName;
			}

			pName = basefeatureItem->Attribute("Featuretype");
			if (pName != NULL)
			{
				baseInfo.strFeaturetype = pName;
			}

			pName = basefeatureItem->Attribute("Blockorder");
			if (pName != NULL)
			{
				baseInfo.strBlockorder = pName;
			}

			pName = basefeatureItem->Attribute("Shape");
			if (pName != NULL)
			{
				baseInfo.strShape = pName;
			}

			pName = basefeatureItem->Attribute("Intensity");
			if (pName != NULL)
			{
				baseInfo.strIntensity = pName;
			}

			pName = basefeatureItem->Attribute("Signal");
			if (pName != NULL)
			{
				baseInfo.strSignal = pName;
			}

			pName = basefeatureItem->Attribute("Channel");
			if (pName != NULL)
			{
				baseInfo.strChannele = pName;
			}

			m_mapBaseFeatures.insert(pair<string, stFeaturesInfo>(baseInfo.strName,baseInfo));
			basefeatureItem = basefeatureItem->NextSiblingElement();
		}
	}
    
    return true;
}


bool CFeatures::readDerivedFeatureData()
{ 

    string strPath  = getCurrentProgramPath();
    strPath += "/";
    strPath += DerivedFeaturesPath;

    TiXmlDocument doc(strPath);

    bool bOk = doc.LoadFile(TIXML_ENCODING_UTF8);
    if (!bOk)
    {
        return false;
    }

    TiXmlElement *root = doc.RootElement();
    if (0 != strcmp(root->Value(), "root"))
    {
        return false;
    }
	clearDerivedFeaturesMap();
	CISAPUGuard<JMutex> autoLock(m_FeaturesMapMutex);
    if (root)
    {
        TiXmlElement *categoryItem = root->FirstChildElement();
        int flowIndex = 0;
        while (categoryItem)
        {
            const char* pszcategory = categoryItem->Value();
			if (pszcategory == NULL )
			{
				categoryItem = categoryItem->NextSiblingElement();
				continue;
			}
			if (strcmp(pszcategory, "categorys") == 0)
			{
				const char* pName = categoryItem->Attribute("name");
				if (pName != NULL)
				{
					m_strCategorys = pName;
					std::vector<std::string> vect = SString::split(m_strCategorys, ",");
					for (int i = 0; i < vect.size();i++)
					{
						m_mapDerivedFeatures.insert(pair<string, vector<stDerivedFeature>* >(vect.at(i), new vector<stDerivedFeature>()));
					}				
				}
			}
            if (strcmp(pszcategory, "category") == 0)
            {           
				const char* pName = categoryItem->Attribute("name");
				if (pName != NULL)
				{
					string strcategoryName = pName;
					if (m_mapDerivedFeatures.find(strcategoryName) == m_mapDerivedFeatures.end())
					{
						m_mapDerivedFeatures.insert(pair<string, vector<stDerivedFeature>* >(strcategoryName, new vector<stDerivedFeature>()));
					}				
					TiXmlElement *DefectItem = categoryItem->FirstChildElement();
					int actionIndex = 0;
					while (DefectItem)
					{
						stDerivedFeature  derFeature;
						derFeature.strFeaturescategory = strcategoryName;
						if (DefectItem->Attribute("name") != NULL) derFeature.strFeaturesName = DefectItem->Attribute("name");
						if (DefectItem->Attribute("expression") != NULL) derFeature.strFeaturesexpression = DefectItem->Attribute("expression");
						if (DefectItem->Attribute("description") != NULL) derFeature.strFeaturesDescription = DefectItem->Attribute("description");
						if (DefectItem->Attribute("width") != NULL) derFeature.strWidth = DefectItem->Attribute("width");
						if (DefectItem->Attribute("size") != NULL) derFeature.strSize = DefectItem->Attribute("size");
						if (DefectItem->Attribute("length") != NULL) derFeature.strLength = DefectItem->Attribute("length");
						if (DefectItem->Attribute("DiagSize") != NULL) derFeature.strDiagSize = DefectItem->Attribute("DiagSize");
						if (DefectItem->Attribute("Aranking") != NULL) derFeature.strAranking = DefectItem->Attribute("Aranking");
						if (DefectItem->Attribute("Aarea") != NULL) derFeature.strAarea = DefectItem->Attribute("Aarea");
						m_mapDerivedFeatures[strcategoryName]->push_back(derFeature);
						DefectItem = DefectItem->NextSiblingElement();
					}
				}
			}
            categoryItem = categoryItem->NextSiblingElement();
        }
    }
        
    return true;
}


bool CFeatures::writeDerivedFeatureData()
{
	CISAPUGuard<JMutex> autoLock(m_FeaturesMapMutex);
    string strPath = getCurrentProgramPath();
    strPath += "/";
    strPath += DerivedFeaturesPath;

    TiXmlDocument doc(strPath);
    TiXmlDeclaration *pDec = new TiXmlDeclaration("1.0", "utf-8", "");

    TiXmlNode *pRoot = new TiXmlElement("root");
    doc.InsertEndChild(*pDec);
    doc.LinkEndChild(pRoot);

	TiXmlElement *categorys = new TiXmlElement("categorys");
	categorys->SetAttribute("name", m_strCategorys);
	pRoot->InsertEndChild(*categorys);

    map<string, vector<stDerivedFeature>* >::iterator it =  m_mapDerivedFeatures.begin();
    while (it != m_mapDerivedFeatures.end())
    {
        TiXmlElement *catelem = new TiXmlElement("category");
        string strCat = it->first;
        catelem->SetAttribute("name", strCat);
        vector<stDerivedFeature>* featureList = it->second;
        for (int i = 0 ;i < featureList->size();i++)
        {
            TiXmlElement *featureelem = new TiXmlElement("feature");
            featureelem->SetAttribute("name", featureList->at(i).strFeaturesName);
            featureelem->SetAttribute("expression", featureList->at(i).strFeaturesexpression);
            featureelem->SetAttribute("description", featureList->at(i).strFeaturesDescription);
			featureelem->SetAttribute("width", featureList->at(i).strWidth);
			featureelem->SetAttribute("size", featureList->at(i).strSize);
			featureelem->SetAttribute("length", featureList->at(i).strLength);
			featureelem->SetAttribute("DiagSize", featureList->at(i).strDiagSize);
			featureelem->SetAttribute("Aranking", featureList->at(i).strAranking);
			featureelem->SetAttribute("Aarea", featureList->at(i).strAarea);
            catelem->InsertEndChild(*featureelem);
        }
        pRoot->InsertEndChild(*catelem);
        it++;
    }
    doc.SaveFile();
    return true;
}

void CFeatures::clearDerivedFeaturesMap()
{
	CISAPUGuard<JMutex> autoLock(m_FeaturesMapMutex);
	map<string, vector<stDerivedFeature>* >::iterator it = m_mapDerivedFeatures.begin();
	while (it != m_mapDerivedFeatures.end())
	{

		vector<stDerivedFeature>* featureList = it->second;
		if (featureList != NULL)
		{
			featureList->clear();
			delete featureList;
		}
		it++;
	}
	m_mapDerivedFeatures.clear();
	return ;
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


stDerivedFeature CFeatures::getDerivedFeature( const string& FeaturesName )
{
	CISAPUGuard<JMutex> autoLock(m_FeaturesMapMutex);
    map<string, vector<stDerivedFeature>* >::iterator it = m_mapDerivedFeatures.begin();
    while (it != m_mapDerivedFeatures.end())
    {

        vector<stDerivedFeature>* featureList = it->second;
        for (int i = 0; i < featureList->size(); i++)
        {
            if (featureList->at(i).strFeaturesName == FeaturesName)
            {
                return  featureList->at(i);
            }
        }

        it++;
    }
    return stDerivedFeature();
}


bool CFeatures::delDerivedFeature(const string& FeaturesName)
{
	CISAPUGuard<JMutex> autoLock(m_FeaturesMapMutex);
	map<string, vector<stDerivedFeature>* >::iterator it = m_mapDerivedFeatures.begin();
	while (it != m_mapDerivedFeatures.end())
	{

		vector<stDerivedFeature>* featureList = it->second;
		vector<stDerivedFeature>::iterator it2 = featureList->begin();
		while (it2 != featureList->end())
		{
			if (it2->strFeaturesName == FeaturesName)
			{
				featureList->erase(it2);
				return true;
			}
			it2++;

		}
		it++;
	}
    return false;
}

bool CFeatures::addDerivedFeature(const string& FeaturesName, stDerivedFeature& Feature)
{
	CISAPUGuard<JMutex> autoLock(m_FeaturesMapMutex);
	map<string, vector<stDerivedFeature>* >::iterator it = m_mapDerivedFeatures.find(Feature.strFeaturescategory);
	if (it != m_mapDerivedFeatures.end())
	{
		vector<stDerivedFeature>* featureList = it->second;
		vector<stDerivedFeature>::iterator it2 = featureList->begin();
		while (it2 != featureList->end())
		{
			if (it2->strFeaturesName == FeaturesName)
			{
				return false;
			}
			it2++;
		}
		featureList->push_back(Feature);
	}
	else
	{
		m_mapDerivedFeatures.insert(pair<string, vector<stDerivedFeature>* >(Feature.strFeaturescategory, new vector<stDerivedFeature>()));
		m_mapDerivedFeatures[Feature.strFeaturescategory]->push_back(Feature);
	}
	
    return true;
}

bool CFeatures::modifyDerivedFeature(const string& FeaturesName, stDerivedFeature& Feature)
{
	CISAPUGuard<JMutex> autoLock(m_FeaturesMapMutex);
	map<string, vector<stDerivedFeature>* >::iterator it = m_mapDerivedFeatures.find(Feature.strFeaturescategory);
	if (it != m_mapDerivedFeatures.end())
	{
		vector<stDerivedFeature>* featureList = it->second;
		vector<stDerivedFeature>::iterator it2 = featureList->begin();
		while (it2 != featureList->end())
		{
			if (it2->strFeaturesName == FeaturesName)
			{
				*it2 = Feature;
				return true;
			}
			it2++;
		}	
	}
	return false;
}

bool CFeatures::saveDerivedFeature()
{
    return writeDerivedFeatureData();
}

const map<string,stFeaturesInfo>* CFeatures::getBaseFeaturesMap()
{
    return &m_mapBaseFeatures;
}

const map<string, vector<stDerivedFeature>* > *CFeatures::getDerivedFeaturesMap()
{
    return &m_mapDerivedFeatures;
}

void CFeatures::getDerivedFeatureNames(vector<string>& DerivedFeatures)
{
	DerivedFeatures.clear();
	map<string, vector<stDerivedFeature>* >::iterator it = m_mapDerivedFeatures.begin();
	while (it != m_mapDerivedFeatures.end())
	{
		vector<stDerivedFeature>* featureList = it->second;
		for (int i = 0; i < featureList->size(); i++)
		{
			DerivedFeatures.push_back(featureList->at(i).strFeaturesName);
		}
		it++;
	}
}

string CFeatures::getCategorysString()
{
	return m_strCategorys;
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

    map<string, vector<stDerivedFeature>* >::iterator it = m_mapDerivedFeatures.begin();
    while (it != m_mapDerivedFeatures.end())
    {
        vector<stDerivedFeature>* featureList = it->second;
        for (int i = 0; i < featureList->size(); i++)
        {
            if (featureList->at(i).strFeaturesName == FeaturesName)
            {
                return true;
            }
        }

        it++;
    }
    return false;
}

CFeaturesMgr*CFeaturesMgr::m_pInstance = NULL;

CFeaturesMgr* CFeaturesMgr::getCFeaturesMgr()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CFeaturesMgr();
    }
    return m_pInstance;
}

void CFeaturesMgr::releaseCFeaturesMgr()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

CFeaturesMgr::CFeaturesMgr()
{

}

CFeaturesMgr::~CFeaturesMgr()
{
    map<string, CFeatures*>::iterator it = m_map.begin();
    while (it != m_map.end())
    {
        delete it->second;
        it++;
    }
    m_map.clear();
}

CFeatures* CFeaturesMgr::getCurrentCFeatures()
{
    string  strProgramName = getCurrentProgramName();
    if (strProgramName.empty())
    {
        return NULL;
    }
    return getCFeatures(strProgramName);
}

CFeatures *CFeaturesMgr::getCFeatures(const string &strChannel)
{
    CFeatures * pCFeatures = NULL;
	string strChanneltmp = "ChannelA";

    if (m_map.find(strChanneltmp) == m_map.end())
    {
        pCFeatures = new CFeatures(strChanneltmp);
        m_map.insert(pair<string, CFeatures*>(strChanneltmp, pCFeatures));
    }
    return m_map[strChanneltmp];
}