#include "defectqualification.h"

#include "g12globalenv.h"
#include "expressionparser/Features.h"
#include "filevariable/ifilevariable.h"
#include "expressionparser/scriptLexer.h"
#include "g12common.h"
#include "rtdefine.h"
#include "tinyxml.h"
#include "jdir.h"
#include "stringcommon.h"
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"
#define DEFECT_QUALIFICATION_CONFIG_NAME   "defectqualification.xml"
#define PRODUCT_CONFIG_FILE_PATH           "./programs/"
#define DEFECTLEVEL_XML_CONFIG             "defectlevel.xml"
#define  VAR_EXPRESSION_XML_CONFIG			"varexpression.xml"
#include "defectlist.h"
static const char *g_szDefectXmlBuf = {
    "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\
    <vstudiodoc version=\"2.0\">\
    </vstudiodoc>" };

void DefectQualification::init()
{
    string m_strCurProductName = getCurrentProgramName();

	string channels = getCurrentProgramRecipesList();
	string strSttion = std::to_string(m_iStation);
	vector<string> ChannelVec;

	getEnabelChannle(ChannelVec, strSttion);
	m_mapScriptLexer = new map<string, map<string, ScriptLexer *> *>();
	for (int index = 0; index < ChannelVec.size(); index++)
	{		
		map<string, map<string, ScriptLexer*>*> *	pmapScriptLexer = new map<string, map<string, ScriptLexer *> *>();
		string channle = ChannelVec.at(index);
		if (m_iStation>0)
		{
			channle += strSttion;
		}
		m_mapMultiChannelScriptLexer.insert(pair<string, map<string, map<string, ScriptLexer*>*> *>(channle, pmapScriptLexer));
		getDefectLevelParamMap(channle);
	}
    getQualificationParam(m_strCurProductName);
	initDefectEnableCfg();

	m_defectNameVec = DefectListManage::GetInstance()->getDefectListInstance(0)->getDefectCnNameList();
	m_defectEnNameVec = DefectListManage::GetInstance()->getDefectListInstance(0)->getDefectEnNameList();
	m_defectEnableVec = DefectListManage::GetInstance()->getDefectListInstance(0)->getDefectEnableList();
}

bool DefectQualification::readConfig(string strProduct, map<string, Defect_Qualification_Param> *param)
{
    // 配置文件放在App同级目录，首先要获取App路径
    if (NULL == param)
    {
        return false;
    }
    JDir dir;
    string strPath = dir.getCurrentPath();
    strPath += PRODUCT_CONFIG_FILE_PATH + strProduct + dir.separator() + DEFECT_QUALIFICATION_CONFIG_NAME;

    TiXmlDocument docConfig;
    bool bLoadOkay = docConfig.LoadFile(strPath);
    if (!bLoadOkay)
    {
        return false;
    }

    TiXmlElement *pRoot = docConfig.RootElement();
    if (pRoot == NULL
        || pRoot->Value() == NULL
        || strcmp(pRoot->Value(), "vstudiodoc") != 0
        || pRoot->Attribute("version") == NULL
        || strcmp(pRoot->Attribute("version"), "2.0") != 0)
    {
        return false;
    }

    TiXmlElement *pDefectElem = pRoot->FirstChildElement("defect");
    const char *szTmp = NULL;
    while (pDefectElem != NULL)
    {
        Defect_Qualification_Param quaParam;
        szTmp = pDefectElem->Attribute("name");
        if (NULL != szTmp)
        {
            quaParam.strName = szTmp;
        }
        szTmp = pDefectElem->Attribute("des");
        if (NULL != szTmp)
        {
            quaParam.strDes = szTmp;
        }
        TiXmlElement *pSeverityElem = pDefectElem->FirstChildElement("severity");
        while (pSeverityElem != NULL)
        {
            string strType;
            Defect_Severity_Param paramSeve;
            szTmp = pSeverityElem->Attribute("name");
            if (NULL != szTmp)
            {
                strType = szTmp;
            }
            szTmp = pSeverityElem->Attribute("enable");
            if (NULL != szTmp)
            {
                paramSeve.bEnableSeverity = atoi(szTmp);
            }
            szTmp = pSeverityElem->Attribute("level5");
            if (NULL != szTmp)
            {
                paramSeve.dLevel5 = atof(szTmp);
            }
            szTmp = pSeverityElem->Attribute("level4");
            if (NULL != szTmp)
            {
                paramSeve.dLevel4 = atof(szTmp);
            }
            szTmp = pSeverityElem->Attribute("level3");
            if (NULL != szTmp)
            {
                paramSeve.dLevel3 = atof(szTmp);
            }
            szTmp = pSeverityElem->Attribute("level2");
            if (NULL != szTmp)
            {
                paramSeve.dLevel2 = atof(szTmp);
            }
            pSeverityElem = pSeverityElem->NextSiblingElement("severity");
            quaParam.mapSeverityParam.insert(make_pair(strType, paramSeve));
        }
        TiXmlElement *pQualificationElem = pDefectElem->FirstChildElement("qualification");
        while (NULL != pQualificationElem)
        {
            string strResultType = "";
            Defect_Result_Param paramResult;
            szTmp = pQualificationElem->Attribute("name");
            if (NULL != szTmp)
            {
                strResultType = szTmp;
            }
            szTmp = pQualificationElem->Attribute("enable");
            if (NULL != szTmp)
            {
                paramResult.bEnableQualification = atoi(szTmp);
            }
			szTmp = pQualificationElem->Attribute("level7");
			if (NULL != szTmp)
			{
				paramResult.iLevelCountArr[7] = atoi(szTmp);
			}
			szTmp = pQualificationElem->Attribute("level6");
			if (NULL != szTmp)
			{
				paramResult.iLevelCountArr[6] = atoi(szTmp);
			}
            szTmp = pQualificationElem->Attribute("level5");
            if (NULL != szTmp)
            {
                paramResult.iLevelCountArr[5] = atoi(szTmp);
            }
            szTmp = pQualificationElem->Attribute("level4");
            if (NULL != szTmp)
            {
                paramResult.iLevelCountArr[4] = atoi(szTmp);
            }
            szTmp = pQualificationElem->Attribute("level3");
            if (NULL != szTmp)
            {
                paramResult.iLevelCountArr[3] = atoi(szTmp);
            }
            szTmp = pQualificationElem->Attribute("level2");
            if (NULL != szTmp)
            {
                paramResult.iLevelCountArr[2] = atoi(szTmp);
            }
            szTmp = pQualificationElem->Attribute("level1");
            if (NULL != szTmp)
            {
                paramResult.iLevelCountArr[1] = atoi(szTmp);
            }
            pQualificationElem = pQualificationElem->NextSiblingElement("qualification");
            quaParam.mapResultParam.insert(make_pair(strResultType, paramResult));
        }
        pDefectElem = pDefectElem->NextSiblingElement("defect");
        param->insert(make_pair(quaParam.strName, quaParam));
    }
    return true;
}

bool DefectQualification::saveConfig(string strProduct, const map<string, Defect_Qualification_Param> *param)
{
    TiXmlDocument docConfig;
    if (NULL == param)
    {
        return false;
    }
    JDir dir;
    string strPath = dir.getCurrentPath();
    strPath += PRODUCT_CONFIG_FILE_PATH + strProduct + dir.separator() + DEFECT_QUALIFICATION_CONFIG_NAME;

    docConfig.Parse(g_szDefectXmlBuf, NULL, TIXML_ENCODING_UTF8);

    TiXmlElement *pRoot = docConfig.RootElement();
    if (pRoot != NULL)
    {
        map<string, Defect_Qualification_Param>::const_iterator it = param->begin();
        for (; it != param->end(); ++it)
        {
            Defect_Qualification_Param param = it->second;
            TiXmlElement *pDefectElem = new TiXmlElement("defect");
            if (NULL != pDefectElem)
            {
                pDefectElem->SetAttribute("name", param.strName);
                pDefectElem->SetAttribute("des", param.strDes);
                map<string, Defect_Severity_Param>::const_iterator itS;
                for (itS = it->second.mapSeverityParam.begin(); itS != it->second.mapSeverityParam.end(); itS++)
                {
                    TiXmlElement *pItemSeverity = new TiXmlElement("severity");
                    if (NULL != pItemSeverity)
                    {
                        pItemSeverity->SetAttribute("name", itS->first);
                        pItemSeverity->SetAttribute("enable", itS->second.bEnableSeverity);
                        pItemSeverity->SetAttribute("level5", itS->second.dLevel5);
                        pItemSeverity->SetAttribute("level4", itS->second.dLevel4);
                        pItemSeverity->SetAttribute("level3", itS->second.dLevel3);
                        pItemSeverity->SetAttribute("level2", itS->second.dLevel2);
                        pDefectElem->LinkEndChild(pItemSeverity);
                    }
                }
                map<string, Defect_Result_Param>::const_iterator itR = it->second.mapResultParam.begin();
                for (; itR != it->second.mapResultParam.end(); itR++)
                {
                    TiXmlElement *pItemResult = new TiXmlElement("qualification");
                    if (NULL != pItemResult)
                    {
                        pItemResult->SetAttribute("name", itR->first);
                        pItemResult->SetAttribute("enable", itR->second.bEnableQualification);
						pItemResult->SetAttribute("level7", itR->second.iLevelCountArr[7]);
						pItemResult->SetAttribute("level6", itR->second.iLevelCountArr[6]);
                        pItemResult->SetAttribute("level5", itR->second.iLevelCountArr[5]);
                        pItemResult->SetAttribute("level4", itR->second.iLevelCountArr[4]);
                        pItemResult->SetAttribute("level3", itR->second.iLevelCountArr[3]);
                        pItemResult->SetAttribute("level2", itR->second.iLevelCountArr[2]);
                        pItemResult->SetAttribute("level1", itR->second.iLevelCountArr[1]);
                        pDefectElem->LinkEndChild(pItemResult);
                    }
                }
                pRoot->LinkEndChild(pDefectElem);
            }
        }
    }
    return docConfig.SaveFile(strPath);
}

//DefectQualification *DefectQualification::m_pInstace = NULL;

DefectQualification::DefectQualification(int iStation) :  m_pScriptLexer(NULL)
											, m_pCFeatures(NULL)
											, m_pScriptLexerLevel(NULL)
											, m_mapScriptLexer(NULL)
{
	m_iStation = iStation;
    init();
}

DefectQualification::~DefectQualification()
{
    map<string, map<string, Defect_Qualification_Param>*>::iterator it = m_DefectQualificationParam.begin();
    for (; it != m_DefectQualificationParam.end(); it++)
    {
        if (NULL != it->second)
        {
            delete it->second;
            it->second = NULL;
        }
    }
    m_DefectQualificationParam.clear();
	map<string, map<string, stVarExpression>* >::iterator it2 = m_mapVarExpression.begin();
	for (; it2 != m_mapVarExpression.end(); it2++)
	{
		if (NULL != it2->second)
		{
			delete it2->second;
			it2->second = NULL;
		}
	}
	m_mapVarExpression.clear();
}

vector<string> DefectQualification::getDefectSeverityType(const string &defectName)
{
    vector<string> natureList;
    string proName = getCurrentProgramName();
    map<string, map<string, Defect_Qualification_Param>*>::iterator it = m_DefectQualificationParam.find(proName);
    if (it == m_DefectQualificationParam.end())
    {
        //初始化配置
        getQualificationParam(proName);
        it = m_DefectQualificationParam.find(proName);
    }
    if (it != m_DefectQualificationParam.end())
    {
        map<string, Defect_Qualification_Param>::iterator itParam = it->second->find(defectName);
        if (itParam != it->second->end())
        {
            for (map<string, Defect_Severity_Param>::iterator itSe = itParam->second.mapSeverityParam.begin();
            itSe != itParam->second.mapSeverityParam.end(); itSe++)
            {
                if (itSe->second.bEnableSeverity)
                {
                    natureList.push_back(itSe->first);
                }
            }
        }
    }
    return natureList;
}


bool DefectQualification::getDefectSeverityLevel(const int defectType, stDefectFeature* pDefectFeature)
{
	if (pDefectFeature == NULL)
	{
		return false;
	}
	
	pDefectFeature->iDefectLevel = 0;

	//m_defectEnableVec = DefectListManage::GetInstance()->getDefectListInstance(0)->getDefectEnableList();


	// 根据中开关进行过滤,没有使能该缺陷检测直接返回，不进行等级判断
	if (defectType >= m_defectEnNameVec.size() || !m_defectEnableVec[defectType].second)
	{
		return false;
	}

	string proName = getCurrentProgramName();
	string strdefectType = m_defectEnNameVec[defectType];
	vector<stdefectlevel>* pDefLevelVec = getDefectLevelParamByType(proName, strdefectType, pDefectFeature->iposition);
	if (pDefLevelVec == NULL)
	{
		return false;
	}
	map<string, ScriptLexer*>* pDefectLexerMap = NULL;
	m_DefectLexerMapMutex.Lock();
	if (m_mapScriptLexer->find(strdefectType) != m_mapScriptLexer->end())
	{
		pDefectLexerMap = m_mapScriptLexer->at(strdefectType);
	}
	else
	{		
		pDefectLexerMap = new map<string, ScriptLexer*>();
		m_mapScriptLexer->insert(pair<string, map<string, ScriptLexer*>*>(strdefectType, pDefectLexerMap));		
	}
	m_DefectLexerMapMutex.Unlock();
    int jMax = pDefLevelVec->size()-1;
    if (jMax<0)
    {
        return false;
    }
	for (int j = jMax; j >= 0;j--)
	{
		int iLevel = pDefLevelVec->at(j).iLevel;
		if (!pDefLevelVec->at(j).bEnable || !m_LevelEnableCfg.defectLevel[iLevel]) // 该等级未使能
		{
			continue;
		}
		string strExpression;
		bool bRes = false;
		for (int i = 0; i  <pDefLevelVec->at(j).defectlevelItemVec.size(); i++)
		{
			stdefectlevelItem*  pItem = &(pDefLevelVec->at(j).defectlevelItemVec[i]);
		
			if (pItem == NULL || !pItem->bEnable || pItem->strItemExpression.length() == 0)
			{
				continue;
			}
			string strExpTmp = pItem->strItemExpression;
	
			if (strExpression.empty())
			{
				strExpression = "(" + strExpTmp + ")";
			}
			else
			{
				strExpression += "| (" + strExpTmp + ")";
			}
		}
		//  根据表达式执行获取结果
		do
		{
            m_DefectLexerMapMutex.Lock();
			ScriptLexer* pScriptLexerLevel = NULL;
			if (pDefectLexerMap->find(strExpression) != pDefectLexerMap->end())
			{
				pScriptLexerLevel = pDefectLexerMap->at(strExpression);
			}

			if (pScriptLexerLevel == NULL)
			{
                
				pScriptLexerLevel = new ScriptLexer(m_iStation);
				
				pDefectLexerMap->insert(pair<string, ScriptLexer*>(strExpression, pScriptLexerLevel));
			}
            m_DefectLexerMapMutex.Unlock();
			strExpression.append("\n");

			if (strExpression != pScriptLexerLevel->getParserString())
			{
				pScriptLexerLevel->setParserString(strExpression);
			}

			if (!pScriptLexerLevel->isParser() && 0 != pScriptLexerLevel->parser())
			{
				break;;
			}

			pScriptLexerLevel->addVarValue("Area", pDefectFeature->dSizeArea);
            pScriptLexerLevel->addVarValue("Gray", pDefectFeature->DefectBAvgGray);
			pScriptLexerLevel->addVarValue("Score", pDefectFeature->fdetect_score);
			pScriptLexerLevel->addVarValue("Contrast", pDefectFeature->contrast);
			pScriptLexerLevel->addVarValue("Width", pDefectFeature->defectWidth);
			pScriptLexerLevel->addVarValue("Height", pDefectFeature->defectHeight);
			pScriptLexerLevel->addVarValue("SkeletonLength", pDefectFeature->SkeletonLength);
			pScriptLexerLevel->addVarValue("PosX", pDefectFeature->posx);
			pScriptLexerLevel->addVarValue("PosY", pDefectFeature->posy);
			pScriptLexerLevel->addVarValue("Svm", pDefectFeature->idetect_flag);

			double dResult = 0.0;
			if (0 != pScriptLexerLevel->evaluation(dResult))
			{
				break;;
			}
			if (dResult > 0.0000001 || dResult < -0.0000001)
			{
				bRes = true;
			}
		} while (0);

		if (bRes)
		{
			pDefectFeature->iDefectLevel = iLevel;
			return true;
		}
	}
	
	return true;
}

bool DefectQualification::getDefectSeverityLevel(string strChannel, const int defectType, stDefectFeature* pDefectFeature)
{
	if (pDefectFeature == NULL)
	{
		return false;
	}

	pDefectFeature->iDefectLevel = 0;

	///m_defectEnableVec = DefectListManage::GetInstance()->getDefectListInstance(0)->getDefectEnableList();
	// 根据中开关进行过滤,没有使能该缺陷检测直接返回，不进行等级判断
	if (defectType >= m_defectEnNameVec.size() || !m_defectEnableVec[defectType].second)
	{
		return false;
	}

	string strdefectType = m_defectEnNameVec[defectType];
	vector<stdefectlevel>* pDefLevelVec = getDefectLevelParamByType(strChannel, strdefectType, pDefectFeature->iposition);
	if (pDefLevelVec == NULL)
	{
		return false;
	}
	map<string, map<string, ScriptLexer*>*> *mapScriptLexer = NULL;
	if (m_mapMultiChannelScriptLexer.find(strChannel) != m_mapMultiChannelScriptLexer.end())
	{
		mapScriptLexer = m_mapMultiChannelScriptLexer.at(strChannel);
	}
	if (mapScriptLexer == NULL)
	{
		return false;
	}
	map<string, ScriptLexer*>* pDefectLexerMap = NULL;
	m_DefectLexerMapMutex.Lock();
	if (mapScriptLexer->find(strdefectType) != mapScriptLexer->end())
	{
		pDefectLexerMap = mapScriptLexer->at(strdefectType);
	}
	else
	{
		pDefectLexerMap = new map<string, ScriptLexer*>();
		mapScriptLexer->insert(pair<string, map<string, ScriptLexer*>*>(strdefectType, pDefectLexerMap));
	}
	m_DefectLexerMapMutex.Unlock();
	int jMax = pDefLevelVec->size() - 1;
	if (jMax < 0)
	{
		return false;
	}
	for (int j = jMax; j >= 0; j--)
	{
		int iLevel = pDefLevelVec->at(j).iLevel;
		if (!pDefLevelVec->at(j).bEnable || !m_LevelEnableCfg.defectLevel[iLevel]) // 该等级未使能
		{
			continue;
		}
		string strExpression;
		bool bRes = false;
		for (int i = 0; i < pDefLevelVec->at(j).defectlevelItemVec.size(); i++)
		{
			stdefectlevelItem*  pItem = &(pDefLevelVec->at(j).defectlevelItemVec[i]);

			if (pItem == NULL || !pItem->bEnable || pItem->strItemExpression.length() == 0)
			{
				continue;
			}
			string strExpTmp = pItem->strItemExpression;

			if (strExpression.empty())
			{
				strExpression = "(" + strExpTmp + ")";
			}
			else
			{
				strExpression += "| (" + strExpTmp + ")";
			}
		}
		//  根据表达式执行获取结果
		do
		{
			m_DefectLexerMapMutex.Lock();
			ScriptLexer* pScriptLexerLevel = NULL;
			if (pDefectLexerMap->find(strExpression) != pDefectLexerMap->end())
			{
				pScriptLexerLevel = pDefectLexerMap->at(strExpression);
			}

			if (pScriptLexerLevel == NULL)
			{

				pScriptLexerLevel = new ScriptLexer(m_iStation);
				pScriptLexerLevel->setChannel(strChannel);
				pDefectLexerMap->insert(pair<string, ScriptLexer*>(strExpression, pScriptLexerLevel));
			}
			m_DefectLexerMapMutex.Unlock();
			strExpression.append("\n");

			if (strExpression != pScriptLexerLevel->getParserString())
			{
				pScriptLexerLevel->setParserString(strExpression);
			}

			if (!pScriptLexerLevel->isParser() && 0 != pScriptLexerLevel->parser())
			{
				break;;
			}

			pScriptLexerLevel->addVarValue("Area", pDefectFeature->dSizeArea);
			pScriptLexerLevel->addVarValue("Gray", pDefectFeature->DefectBAvgGray);
			pScriptLexerLevel->addVarValue("Score", pDefectFeature->fdetect_score);
			pScriptLexerLevel->addVarValue("Contrast", pDefectFeature->contrast);
			pScriptLexerLevel->addVarValue("Width", pDefectFeature->defectWidth);
			pScriptLexerLevel->addVarValue("Height", pDefectFeature->defectHeight);
			pScriptLexerLevel->addVarValue("SkeletonLength", pDefectFeature->SkeletonLength);
			pScriptLexerLevel->addVarValue("PosX", pDefectFeature->posx);
			pScriptLexerLevel->addVarValue("PosY", pDefectFeature->posy);
			pScriptLexerLevel->addVarValue("Svm", pDefectFeature->idetect_flag);

			double dResult = 0.0;
			if (0 != pScriptLexerLevel->evaluation(dResult))
			{
				break;;
			}
			if (dResult > 0.0000001 || dResult < -0.0000001)
			{
				bRes = true;
			}
		} while (0);

		if (bRes)
		{
			pDefectFeature->iDefectLevel = iLevel;
			
			return true;
		}
	}

	return true;
}

const map<string, Defect_Qualification_Param> * DefectQualification::getQualificationParam(string strProduct/* = ""*/)
{
    map<string, Defect_Qualification_Param>* pItem = NULL;
    string proName = strProduct;
    if (proName.size() <= 0)
    {
        proName = getCurrentProgramName();
    }
    map<string, map<string, Defect_Qualification_Param>*>::iterator it = m_DefectQualificationParam.find(proName);
    if (it == m_DefectQualificationParam.end())
    {
        pItem = new map<string, Defect_Qualification_Param>;
        readConfig(proName, pItem);
        m_DefectQualificationParam.insert(make_pair(proName, pItem));
    }
    else
    {
        pItem = it->second;
    }
	return pItem;
}

bool DefectQualification::setQualificationParam(const map<string, Defect_Qualification_Param> *pParam, string strProduct/* = ""*/)
{
    bool bRet = true;
    map<string, Defect_Qualification_Param>* pItem = NULL;
    string proName = strProduct;
    if (proName.size() <= 0)
    {
        proName = getCurrentProgramName();
    }
    map<string, map<string, Defect_Qualification_Param>*>::iterator it = m_DefectQualificationParam.find(proName);
    if (it == m_DefectQualificationParam.end())
    {
        pItem = new map<string, Defect_Qualification_Param>;
        *pItem = *pParam;
        m_DefectQualificationParam.insert(make_pair(proName, pItem));
    }
    else
    {
        *it->second = *pParam;
    }
    saveConfig(proName, pParam);
    return bRet;
}

map<string, vector<stdefectlevelRegion>* >* DefectQualification::getDefectLevelParamMap(string strChannel /*= ""*/)
{
	map<string, vector<stdefectlevelRegion>* >* pItem = NULL;
	map<string, map<string, vector<stdefectlevelRegion>*>* >::iterator it = m_mapAllDefectLevel.find(strChannel);
	if (it == m_mapAllDefectLevel.end())
	{
		pItem = new map<string, vector<stdefectlevelRegion>*>;
		readDefectLevelXml(strChannel, pItem);
		m_mapAllDefectLevel.insert(make_pair(strChannel, pItem));
	}
	else
	{
		pItem = it->second;
	}
	return pItem;
}

vector<stdefectlevel>* DefectQualification::getDefectLevelParamByType(string strChannel /*= ""*/, string strDefectType /*= ""*/, int iRegion)
{
	if (strDefectType.length() == 0)
	{
		return NULL;
	}
	map<string, vector<stdefectlevelRegion>* >* pItemMap = NULL;
	vector<stdefectlevel>* pItemVec = NULL;

	m_DefectLexerMapMutex.Lock();

	map<string, map<string, vector<stdefectlevelRegion>*>* >::iterator it = m_mapAllDefectLevel.find(strChannel);
	if (it == m_mapAllDefectLevel.end())
	{
		
		pItemMap = new map<string, vector<stdefectlevelRegion>*>;
		readDefectLevelXml(strChannel, pItemMap);
		m_mapAllDefectLevel.insert(make_pair(strChannel, pItemMap));
		m_DefectLexerMapMutex.Unlock();
	}
	pItemMap = m_mapAllDefectLevel.at(strChannel);

	m_DefectLexerMapMutex.Unlock();

	if (pItemMap == NULL || pItemMap->find(strDefectType) == pItemMap->end())
	{
		return NULL;
	}
	for (int i = 0; i < pItemMap->at(strDefectType)->size();i++)
	{
		if (pItemMap->at(strDefectType)->at(i).iRegion == iRegion)
		{
			return pItemMap->at(strDefectType)->at(iRegion).pDefectlevelVec;
		}
	}
	return NULL;	
}

bool DefectQualification::readVarExpressionConfig(string strChannel, map<string, stVarExpression> *pVarExpressionMap)
{
	if (pVarExpressionMap == NULL)
	{
		return false;
	}

	JDir dir;
	string strPath = getCurrentProgramPath();
	if (strChannel.length() >0)
	{
		strPath = strPath + dir.separator() + strChannel + "_" + VAR_EXPRESSION_XML_CONFIG;
	}
	else
	{
		strPath = strPath + dir.separator() + VAR_EXPRESSION_XML_CONFIG;
	}

	TiXmlDocument docConfig;
	bool bLoadOkay = docConfig.LoadFile(strPath);
	if (!bLoadOkay)
	{
		return false;
	}
	TiXmlElement *pRoot = docConfig.RootElement();
	if (pRoot == NULL)
	{
		return false;
	}
	
	TiXmlElement *pDefectType = pRoot->FirstChildElement("multivar"); // 缺陷类型
	const char *szTmp = NULL;
	while (pDefectType != NULL) //取 1~11缺陷类型
	{
		stVarExpression pstVarExpression ;
		const char *typeName = NULL;
		typeName = pDefectType->Attribute("name");
		if (typeName != NULL)
		{
			pstVarExpression.varName = typeName;
			
		}

		const char *typeDes = NULL;
		typeDes = pDefectType->Attribute("des");
		if (typeDes != NULL)
		{
			pstVarExpression.varDes = typeDes;
		}

		const char *typeexpression = NULL;
		typeexpression = pDefectType->Attribute("expression");
		if (typeName != NULL)
		{
			pstVarExpression.varExp = typeexpression;
		}
		pVarExpressionMap->insert(pair<string, stVarExpression>(typeName, pstVarExpression));
		pDefectType = pDefectType->NextSiblingElement("multivar");
	}
	return true;
}

bool DefectQualification::saveVarExpressionConfig(string strChannel, map<string, stVarExpression> *pVarExpression)
{
	TiXmlDocument docConfig;
	if (NULL == pVarExpression)
	{
		return false;
	}

	JDir dir;
	string strPath = getCurrentProgramPath();
	if (strChannel.length() >0)
	{
		strPath = strPath + dir.separator() + strChannel + "_" + VAR_EXPRESSION_XML_CONFIG;
	}
	else
	{
		strPath = strPath + dir.separator() + VAR_EXPRESSION_XML_CONFIG;
	}

	docConfig.Parse(g_szDefectXmlBuf, NULL, TIXML_ENCODING_UTF8);

	TiXmlElement *pRoot = docConfig.RootElement();
	if (pRoot != NULL)
	{
		map<string, stVarExpression>::iterator it = pVarExpression->begin();
		while (it != pVarExpression->end())
		{
			string strName = it->first;
			stVarExpression pVarExpression = it->second;
			TiXmlElement *pTypeElem = new TiXmlElement("multivar");
			pTypeElem->SetAttribute("name", pVarExpression.varName);
			pTypeElem->SetAttribute("des", pVarExpression.varDes);
			pTypeElem->SetAttribute("expression", pVarExpression.varExp);
			pRoot->LinkEndChild(pTypeElem);
			it++;
		}
	}
	return docConfig.SaveFile(strPath);
}

bool DefectQualification::saveDefectLevelParam(string strChannel /*= ""*/)
{
	if (m_mapAllDefectLevel.find(strChannel) == m_mapAllDefectLevel.end())
	{
		return false;
	}

	return saveDefectLevelXml(strChannel, m_mapAllDefectLevel.at(strChannel));
}

enResFlag DefectQualification::getProductFinalRes(stProductSummary* pProductSummary ,int  iqualifyIndex)
{
	enResFlag flag = res_OK;//默认OK。都为 0 级的时候需要OK
	if (pProductSummary == NULL)
	{
		return res_OK3;
	}
	else if (res_recheck == pProductSummary->ResultFlag|| res_MaxNumber == pProductSummary->ResultFlag)
	{
		return res_OK3;
	}
	bool bcontinueOK = true;
	bool bcontinueOK2 = true;
	bool bcontinueOK3 = true;

    string strOK = "OK";
    string strRecheck = "Rechek";
    string strReClean = "ReClean";
    string strNg = "NG";
    if(iqualifyIndex !=0)
    {
        strOK = "OK2";
        strRecheck = "Rechek2";
        strReClean = "ReClean2";
        strNg = "NG2";
    }

    int i = 1;
    for (int k = 1; k < m_defectEnNameVec.size(); k++)
    {
        // NG 判断NG 是OR 关系,只有一个值超过就是NG
        //i = m_defectTypeNumber[k];
		i = k;
        const Defect_Result_Param* pParam = getCurrDefectQualificationCfg(strNg, m_defectEnNameVec[i]);
        if (pParam != NULL && pParam->bEnableQualification)
        {
            for (int j = DEFECT_LEVEL_SIZE-1; j > 0; j--)
            {
                int iLevel = j;
                int iDefectCnt = pProductSummary->defectSingleLevelCntArr[i][j];
                if (iDefectCnt > pParam->iLevelCountArr[j])
                {
                    pProductSummary->iProductNgDefectType = i;
                    return res_Ng;
                }
            }
        }

		if (bcontinueOK3)
		{
			const Defect_Result_Param* pParam3 = getCurrDefectQualificationCfg(strReClean, m_defectEnNameVec[i]);
			if (pParam3 != NULL &&pParam3->bEnableQualification)
			{
				for (int j = DEFECT_LEVEL_SIZE-1; j > 0 &&bcontinueOK3; j--)
				{
					int iLevel = j;
                    int iDefectCnt = pProductSummary->defectSingleLevelCntArr[i][j];
					if (iDefectCnt > pParam3->iLevelCountArr[j])
					{
						bcontinueOK3 = false;
					}
				}
			}
		}

		if (bcontinueOK2)
		{
            const Defect_Result_Param* pParam2 = getCurrDefectQualificationCfg(strRecheck, m_defectEnNameVec[i]);
			if (pParam2 != NULL &&pParam2->bEnableQualification)
			{
				for (int j = 1; j < DEFECT_LEVEL_SIZE &&bcontinueOK2; j++)
				{
					int iLevel = j;
                    int iDefectCnt = pProductSummary->defectSingleLevelCntArr[i][j];
					if (iDefectCnt > pParam2->iLevelCountArr[j])
					{
						bcontinueOK2 = false;
					}
				}
			}
		}

		// OK 判断OK 是 AND 关系,全部满足才是OK
		if (bcontinueOK)
		{
			const Defect_Result_Param* pParamOK = getCurrDefectQualificationCfg(strOK, m_defectEnNameVec[i]);
			if (pParamOK != NULL &&pParamOK->bEnableQualification)
			{
				for (int j = 1; j < DEFECT_LEVEL_SIZE &&bcontinueOK; j++)
				{
					int iLevel = j;
                    int iDefectCnt = pProductSummary->defectSingleLevelCntArr[i][j];
					if (iDefectCnt > pParamOK->iLevelCountArr[j])
					{
						bcontinueOK = false;
					}
				}
			}
		}
	}
    if (!bcontinueOK3)
    {
        return res_OK3;     //重洗
    }
	//if (!bcontinueOK2|| !bcontinueOK)
	//{
    //    return res_OK2;     //重测
	//}
	if (!bcontinueOK2 || !bcontinueOK)
    {
        return res_OK;
    }

	return flag;
}

string DefectQualification::getDefectDesByName(const string& defectName)
{
	if (m_mapDefectDes.find(defectName) == m_mapDefectDes.end())
	{
		return "";
	}
	return m_mapDefectDes[defectName];

}

const map<string, stVarExpression> * DefectQualification::getVarExpressionParam(string strChannel)
{
	map<string, stVarExpression>* pItem = NULL;
	string proName = getCurrentProgramName();
	map<string, map<string, stVarExpression>* >::iterator it = m_mapVarExpression.find(strChannel);
	if (it == m_mapVarExpression.end())
	{
		pItem = new map<string, stVarExpression>();
		readVarExpressionConfig(strChannel, pItem );
		m_mapVarExpression.insert(make_pair(strChannel, pItem));
	}
	else
	{
		pItem = it->second;
	}
	return pItem;
}

bool DefectQualification::setVarExpressionParam(const map<string, stVarExpression> *pParam, string strChannel)
{
	bool bRet = true;
	map<string, stVarExpression>* pItem = NULL;
	map<string, map<string, stVarExpression>* > ::iterator it = m_mapVarExpression.find(strChannel);
	if (it == m_mapVarExpression.end())
	{
		pItem = new map<string, stVarExpression>;
		*pItem = *pParam;
		m_mapVarExpression.insert(make_pair(strChannel, pItem));
	}
	else
	{
		*it->second = *pParam;
		pItem = it->second;
	}
	saveVarExpressionConfig(strChannel, pItem);
	return bRet;
}

stVarExpression DefectQualification::getVarExpression(const string& VartName, string strChannel)
{

	const map<string, stVarExpression> * pVarExpressionMap = getVarExpressionParam(strChannel);
	if (pVarExpressionMap->find(VartName) == pVarExpressionMap->end())
	{
		return stVarExpression();
	}
	return pVarExpressionMap->at(VartName);
}

bool DefectQualification::isVarExpression(const string& VartName, string strChannel)
{
	const map<string, stVarExpression> * pVarExpressionMap = getVarExpressionParam(strChannel);
	if (pVarExpressionMap->find(VartName) == pVarExpressionMap->end())
	{
		return false;
	}
	return true;
}

bool DefectQualification::readDefectLevelXml(string strChannel, map<string, vector<stdefectlevelRegion>*>* param)
{
	if (NULL == param)
	{
		return false;
	}
	JDir dir;
	string strPath = getCurrentProgramPath();
	if (strChannel.length() >0)
	{
		strPath = strPath + dir.separator() + strChannel+"_"+DEFECTLEVEL_XML_CONFIG;
	}
	else
	{
		strPath = strPath + dir.separator() + DEFECTLEVEL_XML_CONFIG;
	}
	

	TiXmlDocument docConfig;
	bool bLoadOkay = docConfig.LoadFile(strPath);
	if (!bLoadOkay)
	{
		return false;
	}
	TiXmlElement *pRoot = docConfig.RootElement();
	if (pRoot == NULL)
	{
		return false;
	}
	TiXmlElement *pDefectattribute = pRoot->FirstChildElement("attribute"); // 缺陷类型
	if (pDefectattribute != NULL)
	{
		const char *category = pDefectattribute->Attribute("category");
		if (category != NULL)
		{
			m_defectAttributeCategory = category;
		}
	}
	
	TiXmlElement *pDefectType = pRoot->FirstChildElement("type"); // 缺陷类型
	const char *szTmp = NULL;
	while (pDefectType != NULL) //取 1~11缺陷类型
	{
		const char *typeName = NULL;
		typeName = pDefectType->Attribute("name");
		if (typeName == NULL)
		{
			pDefectType = pDefectType->NextSiblingElement("type");
			continue;
		}

		const char *typeDes = NULL;
		typeDes = pDefectType->Attribute("des");
		if (typeDes != NULL)
		{
			m_mapDefectDes.insert(pair<string,string>(typeName, typeDes));
		}

		int iRegion = 0;
		vector<stdefectlevelRegion>* pDefectlevelRegion = NULL;
		
		if (param->find(typeName) == param->end())
		{	
			pDefectlevelRegion = new vector<stdefectlevelRegion>();
			param->insert(std::pair<string, vector<stdefectlevelRegion>*>(string(typeName), pDefectlevelRegion));
		}
		else {
			//以防重复崩溃//
			continue;
		}

		TiXmlElement *pDefectRegionLevel = pDefectType->FirstChildElement("region");  // 按区域区分缺陷
		while (NULL != pDefectRegionLevel)
		{
			int iRegion = 0;
			const char* pRegionType = pDefectRegionLevel->Attribute("type");
			if (NULL != pRegionType)
			{
				iRegion = SString::strtoInt(pRegionType);
			}

			vector<stdefectlevel>* vecLevel = new vector<stdefectlevel>;
			pDefectlevelRegion->push_back(stdefectlevelRegion(iRegion, vecLevel));
	
			TiXmlElement *pDefectLevel = pDefectRegionLevel->FirstChildElement("level");  //取1~7 等级 
			while (NULL != pDefectLevel)
			{
				stdefectlevel defectlevelUnion;
				defectlevelUnion.strDefectName = typeName;
				const char *plevelItem = NULL;
				defectlevelUnion.strDefectName = typeName;
				const char* pItem = pDefectLevel->Attribute("name");
				if (NULL != pItem)
				{
					defectlevelUnion.strLevel = pItem;
					defectlevelUnion.iLevel = SString::strtoInt(pItem);
				}

			pItem = pDefectLevel->Attribute("Enable"); 
			if (NULL != pItem)
			{
				defectlevelUnion.bEnable = (bool)SString::strtoBool(pItem);
			}

			TiXmlElement *pFilter = pDefectLevel->FirstChildElement("filter");	//取等级下面的描述
			while (NULL != pFilter)
			{
				defectlevelItem Item;
				Item.strDefectName = typeName;
				Item.iLevel = SString::strtoInt(pItem);
				const char *cItem = NULL;
				cItem = pFilter->Attribute("name");
				if (NULL != cItem)
				{
					Item.strItemExpression = cItem;
				}
				cItem = pFilter->Attribute("des");
				if (NULL != cItem)
				{
					Item.strItemDes = cItem;
				}
				cItem = pFilter->Attribute("Enable");
				if (NULL != cItem)
				{
					Item.bEnable = (bool)SString::strtoBool(cItem);
				}

					defectlevelUnion.defectlevelItemVec.push_back(Item);

					pFilter = pFilter->NextSiblingElement("filter");
				}

				vecLevel->push_back(defectlevelUnion);
				pDefectLevel = pDefectLevel->NextSiblingElement("level");
			}			
			pDefectRegionLevel = pDefectRegionLevel->NextSiblingElement("region");
		}
		pDefectType = pDefectType->NextSiblingElement("type");
	}
	return true;
}

bool DefectQualification::saveDefectLevelXml(string strChannel, map<string, vector<stdefectlevelRegion>*> *param)
{
	TiXmlDocument docConfig;
	if (NULL == param)
	{
		return false;
	}
	JDir dir;
	string strPath = getCurrentProgramPath();
	if (strChannel.length() > 0)
	{
		strPath = strPath + dir.separator() + strChannel + "_"+ DEFECTLEVEL_XML_CONFIG;
	}
	else
	{
		strPath = strPath + dir.separator() + DEFECTLEVEL_XML_CONFIG;
	}

	docConfig.Parse(g_szDefectXmlBuf, NULL, TIXML_ENCODING_UTF8);

	TiXmlElement *pRoot = docConfig.RootElement();
	if (pRoot != NULL)
	{
		TiXmlElement *pAttributeElem = new TiXmlElement("attribute");
		pAttributeElem->SetAttribute("category", m_defectAttributeCategory);
		pRoot->LinkEndChild(pAttributeElem);

		
		int iRegion = 0;
		vector<stdefectlevelRegion>* pDefectlevelRegion = NULL;
		map<string, vector<stdefectlevelRegion>*>::iterator it = param->begin();
		while (it != param->end())
		{
			string strDefectType = it->first;
			string strDefectDes = getDefectDesByName(strDefectType);
			pDefectlevelRegion = it->second;
			TiXmlElement *pTypeElem = new TiXmlElement("type");	
			pTypeElem->SetAttribute("name", strDefectType);// type 1~11
			pTypeElem->SetAttribute("des", strDefectDes);// type 1~11
			if (NULL != pDefectlevelRegion)
			{
				for (int iRegionType = 0; iRegionType < pDefectlevelRegion->size(); iRegionType++)
				{
					TiXmlElement *pRegionElem = new TiXmlElement("region");		// 缺陷在哪个区域  0 边缘区域 1 视窗区域 
					pRegionElem->SetAttribute("type", pDefectlevelRegion->at(iRegionType).iRegion);
					vector<defectlevel>* pVecLevel = pDefectlevelRegion->at(iRegionType).pDefectlevelVec;

					if (NULL != pRegionElem &&NULL != pVecLevel)
					{
						defectlevel* pDefect = NULL;
						for (int iDect = 0; iDect < pVecLevel->size(); iDect++)
						{
							TiXmlElement *pLevelElem = new TiXmlElement("level");//1~7
							pDefect = &pVecLevel->at(iDect);

						if (NULL != pLevelElem && NULL != pDefect)
						{
							
							pLevelElem->SetAttribute("name", pDefect->strLevel);
							pLevelElem->SetAttribute("Enable", (int)pDefect->bEnable); 

							for (int iFilter = 0; iFilter < pDefect->defectlevelItemVec.size(); iFilter++)
							{
									TiXmlElement *pFilterElem = new TiXmlElement("filter"); //面积，得分，对比度
									if (NULL != pFilterElem){

										pFilterElem->SetAttribute("name", pDefect->defectlevelItemVec[iFilter].strItemExpression);
										pFilterElem->SetAttribute("Enable", pDefect->defectlevelItemVec[iFilter].bEnable);
										pFilterElem->SetAttribute("des", pDefect->defectlevelItemVec[iFilter].strItemDes);
										pLevelElem->LinkEndChild(pFilterElem);
									}
								}
								pRegionElem->LinkEndChild(pLevelElem);
							}
						}
					}
					pTypeElem->LinkEndChild(pRegionElem);
				}
			}
			pRoot->LinkEndChild(pTypeElem);
			it++;
		}
	}
	return docConfig.SaveFile(strPath);
}

const Defect_Result_Param* DefectQualification::getCurrDefectQualificationCfg(string ResType, string defectType)
{
	const map<string, Defect_Qualification_Param> * pMap = getQualificationParam();
	if (pMap == NULL)
	{
		return NULL;
	}
	map<string, Defect_Qualification_Param>::const_iterator itParam = pMap->find(defectType);
	if (itParam == pMap->end())
	{
		return NULL;
	}
	const map<string, Defect_Result_Param>::const_iterator it = itParam->second.mapResultParam.find(ResType);
	if (it == itParam->second.mapResultParam.end())
	{
		return NULL;
	}
	return &(it->second);
}

void DefectQualification::initDefectEnableCfg()
{
	IFileVariable *pFileVariable = getQualificationCfgFileVariable();
	if (pFileVariable == NULL)
	{
		return;
	}
	//
	m_LevelEnableCfg.defectLevel[0] = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL0, true).toBool();
	m_LevelEnableCfg.defectLevel[1] = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL1, true).toBool();
	m_LevelEnableCfg.defectLevel[2] = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL2, true).toBool();
	m_LevelEnableCfg.defectLevel[3] = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL3, true).toBool();
	m_LevelEnableCfg.defectLevel[4] = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL4, true).toBool();
	m_LevelEnableCfg.defectLevel[5] = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL5, true).toBool();
	m_LevelEnableCfg.defectLevel[6] = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL6, true).toBool();
	m_LevelEnableCfg.defectLevel[7] = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL7, true).toBool();

	pFileVariable->bindValue(QUALIFICATION_ENABLE_LEVEL0, &m_LevelEnableCfg.defectLevel[0]);
	pFileVariable->bindValue(QUALIFICATION_ENABLE_LEVEL1, &m_LevelEnableCfg.defectLevel[1]);
	pFileVariable->bindValue(QUALIFICATION_ENABLE_LEVEL2, &m_LevelEnableCfg.defectLevel[2]);
	pFileVariable->bindValue(QUALIFICATION_ENABLE_LEVEL3, &m_LevelEnableCfg.defectLevel[3]);
	pFileVariable->bindValue(QUALIFICATION_ENABLE_LEVEL4, &m_LevelEnableCfg.defectLevel[4]);
	pFileVariable->bindValue(QUALIFICATION_ENABLE_LEVEL5, &m_LevelEnableCfg.defectLevel[5]);
	pFileVariable->bindValue(QUALIFICATION_ENABLE_LEVEL6, &m_LevelEnableCfg.defectLevel[6]);
	pFileVariable->bindValue(QUALIFICATION_ENABLE_LEVEL7, &m_LevelEnableCfg.defectLevel[7]);

}

///*************************************************************************************//
DefectQualimanage *DefectQualimanage::m_pInstace = NULL;
DefectQualimanage::DefectQualimanage()
{
	for (int i = 0; i < sizeof(m_IDefectQualification) / sizeof(IDefectQualification*); i++)
	{
		m_IDefectQualification[i] = NULL;
	}
}
DefectQualimanage::~DefectQualimanage()
{
	for (int i = 0; i < sizeof(m_IDefectQualification) / sizeof(IDefectQualification*); i++)
	{
		if (m_IDefectQualification[i] != NULL)
		{
			delete m_IDefectQualification[i];
			m_IDefectQualification[i] = NULL;
		}
	}

}
DefectQualimanage * DefectQualimanage::GetInstance()
{
	if (NULL == m_pInstace)
	{
		m_pInstace = new DefectQualimanage;
	}
	return m_pInstace;
}

void DefectQualimanage::DestroyInstance()
{
	if (NULL != m_pInstace)
	{
		delete m_pInstace;
		m_pInstace = NULL;
	}
}
IDefectQualification*   DefectQualimanage::getDefectQualificationInstance(int iStation)
{

	if (iStation >= 0 && iStation < sizeof(m_IDefectQualification) / sizeof(IDefectQualification*))
	{
		if (m_IDefectQualification[iStation] == NULL)
		{
			m_IDefectQualification[iStation] = new DefectQualification(iStation);
		}
		return m_IDefectQualification[iStation];
	}
	return NULL;
}
void  DefectQualimanage::ReleaseStation(int iStation)
{
	if (iStation >= 0 && iStation < sizeof(m_IDefectQualification) / sizeof(IDefectQualification*))
	{
		if (m_IDefectQualification[iStation] != NULL)
		{
			delete m_IDefectQualification[iStation];
			m_IDefectQualification[iStation] = NULL;
		}
	}
}
