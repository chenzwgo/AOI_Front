#include "defectlist.h"
#include "g12common.h"
#define DEFECT_QUALIFICATION_CONFIG_NAME   "defectlist.xml"
#define DEFECT_BJAI_INDEX_CONFIG_NAME   "bjai_defectlist_to_index.xml"
#define PRODUCT_CONFIG_FILE_PATH           "/programs/"

DefectList::DefectList()
{
	readDefectCfgFile();
	readBjAiDefectNameFile();
}

void DefectList::readDefectCfgFile()
{
    // 配置文件放在App同级目录，首先要获取App路径
    JDir dir;
    string strPath = ".";
     string productName = getCurrentProgramName();
    strPath += PRODUCT_CONFIG_FILE_PATH + productName + dir.separator() + DEFECT_QUALIFICATION_CONFIG_NAME;

    TiXmlDocument docConfig;
    bool bLoadOkay = docConfig.LoadFile(strPath);
    if (!bLoadOkay)
    {
        return;
    }

    TiXmlElement *pRoot = docConfig.RootElement();
    if (pRoot == NULL)
    {
        return;
    }

    TiXmlElement *pStationElem = pRoot->FirstChildElement("station");
    const char *szTmp = NULL;
	//multimap<int, DefectInfo> sortMap; //定义一个map根据index排序
    while (pStationElem != NULL)
    {
        szTmp = pStationElem->Attribute("name");  //工位名
        TiXmlElement *defectElem = pStationElem->FirstChildElement("defect");
        while (defectElem != NULL)
        {
			DefectInfo defectInfo;
            szTmp = defectElem->Attribute("name");
            if (NULL != szTmp)
            {
               defectInfo.name  = szTmp;
            }
            szTmp = defectElem->Attribute("alias");
            if (NULL != szTmp)
            {
                defectInfo.alias = szTmp;
            }
            szTmp = defectElem->Attribute("enable");
            if (NULL != szTmp)
            {
                defectInfo.isEnable = atoi(szTmp);
            }
            szTmp = defectElem->Attribute("index");
            if (NULL != szTmp)
            {
                defectInfo.index = atoi(szTmp);
            }
            szTmp = defectElem->Attribute("severity");
            if (NULL != szTmp)
            {
                defectInfo.severity = atoi(szTmp);
            }
            szTmp = defectElem->Attribute("color");
            if (NULL != szTmp)
            {
                defectInfo.color = szTmp;
            }
            szTmp = defectElem->Attribute("description");
            if (NULL != szTmp)
            {
                defectInfo.description = szTmp;
            }
            defectElem = defectElem->NextSiblingElement();
			m_defectInfoMap[defectInfo.name] = defectInfo;
			//sortMap.insert(make_pair(defectInfo.index, defectInfo));
			//----//
			m_defectInfoVec.push_back(defectInfo);  //vector要排序
			m_cnNameVec.push_back(defectInfo.description);
			m_enNameVec.push_back(defectInfo.name);
			m_aliasNameVec.push_back(defectInfo.alias);
			if (defectInfo.severity>0)//大于0为认是重复替换的索引如：脏污 尘点 毛丝 统一为 脏污 
			{
				m_indexVec.push_back(defectInfo.severity);
			}
			else
			{
				m_indexVec.push_back(defectInfo.index);
			}

			m_defectEnableVec.push_back(make_pair(defectInfo.name, defectInfo.isEnable));
			//----//
        }
		/*
		for (multimap<int, DefectInfo>::iterator it = sortMap.begin(); it != sortMap.end(); it++)
		{
			m_defectInfoVec.push_back(it->second);  //vector要排序
            m_cnNameVec.push_back(it->second.description);
            m_enNameVec.push_back(it->second.name);
            m_aliasNameVec.push_back(it->second.alias);
			if (it->second.severity>0)//大于0为认是重复替换的索引如：脏污 尘点 毛丝 统一为 脏污 
			{
				m_indexVec.push_back(it->second.severity);
			}
			else
			{
				m_indexVec.push_back(it->second.index);
			}
           
            m_defectEnableVec.push_back(make_pair(it->second.name, it->second.isEnable));
		}*/
		//sortMap.clear();
		pStationElem = pStationElem->NextSiblingElement();
    }
    return;
}

vector<string> DefectList::getDefectCnNameList()
{
//	vector<string> cnNameVec;
//	for (int i = 0; i < m_defectInfoVec.size(); ++i)
//	{
//		cnNameVec.push_back(m_defectInfoVec.at(i).description);
//	}
    return m_cnNameVec;
}

vector<string> DefectList::getDefectEnNameList()
{
//	vector<string> enNameVec;
//	for (int i = 0; i < m_defectInfoVec.size(); ++i)
//	{
//		enNameVec.push_back(m_defectInfoVec.at(i).name);
//	}
    return m_enNameVec;
}

vector<string> DefectList::getDefectAliasNameList()
{
//	vector<string> aliasNameVec;
//	for (int i = 0; i < m_defectInfoVec.size(); ++i)
//	{
//		aliasNameVec.push_back(m_defectInfoVec.at(i).alias);
//	}
    return m_aliasNameVec;
}

vector<pair<string, bool>> DefectList::getDefectEnableList()
{
//	m_defectEnableVec.clear();
//	for (int i = 0; i < m_defectInfoVec.size(); ++i)
//	{
//		m_defectEnableVec.push_back(make_pair(m_defectInfoVec.at(i).name, m_defectInfoVec.at(i).isEnable));
//	}
	return m_defectEnableVec;
}

 vector<int> DefectList::getDefectIndexList()
{
//	vector<int> indexVec;
//	for (int i = 0; i < m_defectInfoVec.size(); ++i)
//	{
//		indexVec.push_back(m_defectInfoVec.at(i).index);
//	}
    return m_indexVec;
}

 vector<DefectInfo> DefectList::getDefectInfo()
 {
	 return m_defectInfoVec;
 }

void DefectList::setDefectEnable(vector<pair<string ,bool > > defectEnableVec)
{
	// 配置文件放在App同级目录，首先要获取App路径
	JDir dir;
	string strPath = ".";
	string productName = getCurrentProgramName();
	strPath += PRODUCT_CONFIG_FILE_PATH + productName + dir.separator() + DEFECT_QUALIFICATION_CONFIG_NAME;

	TiXmlDocument docConfig;
	bool bLoadOkay = docConfig.LoadFile(strPath);
	if (!bLoadOkay)
	{
		return;
	}
	TiXmlElement *pRoot = docConfig.RootElement();
	if (pRoot == NULL)
	{
		return;
	}
	TiXmlElement *pStationElem = pRoot->FirstChildElement("station");
	const char *szTmp = NULL;
	while (pStationElem != NULL)
	{
		TiXmlElement *pDefectElem = pStationElem->FirstChildElement("defect");
		while(pDefectElem != NULL)
		{
			for (int i = 0; i < defectEnableVec.size(); ++i)
			{
				if (pDefectElem->Attribute("name") == defectEnableVec[i].first)
				{
					pDefectElem->SetAttribute("enable", defectEnableVec[i].second ? string("1") : string("0"));
					pDefectElem = pDefectElem->NextSiblingElement();
					break;
				}
			}
		}
		pStationElem = pStationElem->NextSiblingElement();
	}
	docConfig.SaveFile(strPath);
	//更新使能状态
	for (int i = 0; i < m_defectInfoVec.size(); ++i)
	{
		m_defectInfoVec.at(i).isEnable = defectEnableVec.at(i).second;
	}

    m_defectEnableVec.clear();
    for (int i = 0; i < m_defectInfoVec.size(); ++i)
    {
        m_defectEnableVec.push_back(make_pair(m_defectInfoVec.at(i).name, m_defectInfoVec.at(i).isEnable));
    }
}
void DefectList::readBjAiDefectNameFile()
{
	// 配置文件放在App同级目录，首先要获取App路径
	m_BjAiDefectNameToIndex.clear();
	JDir dir;
	string strPath = ".";
	string productName = getCurrentProgramName();
	strPath += PRODUCT_CONFIG_FILE_PATH + productName + dir.separator() + DEFECT_BJAI_INDEX_CONFIG_NAME;

	TiXmlDocument docConfig;
	bool bLoadOkay = docConfig.LoadFile(strPath);
	if (!bLoadOkay)
	{
		return;
	}

	TiXmlElement *pRoot = docConfig.RootElement();
	if (pRoot == NULL)
	{
		return;
	}

	TiXmlElement *pStationElem = pRoot->FirstChildElement("station");
	const char *szTmp = NULL;
	while (pStationElem != NULL)
	{
		szTmp = pStationElem->Attribute("name");
		TiXmlElement *defectElem = pStationElem->FirstChildElement("defect");
		while (defectElem != NULL)
		{
			szTmp = defectElem->Attribute("bjai_name");
			string strName = "";
			if (NULL != szTmp)
			{
				strName = szTmp;
			}
			szTmp = defectElem->Attribute("index");
			int index = 0;
			if (NULL != szTmp)
			{
				index = atoi(szTmp);
			}
			defectElem = defectElem->NextSiblingElement();
			m_BjAiDefectNameToIndex.push_back(make_pair(strName, index));
		}
		pStationElem = pStationElem->NextSiblingElement();
	}
}
vector<pair<string, int>> DefectList::getBjAiDefectNameToIndexList()
{
	return m_BjAiDefectNameToIndex;
}
DefectListManage *DefectListManage::m_pInstace = NULL;
DefectListManage::DefectListManage()
{
	for (int i = 0; i < sizeof(m_IDefectList) / sizeof(iDefectList*); i++)
	{
		m_IDefectList[i] = NULL;
	}
}
DefectListManage::~DefectListManage()
{
	for (int i = 0; i < sizeof(m_IDefectList) / sizeof(iDefectList*); i++)
	{
		if (m_IDefectList[i] != NULL)
		{
			delete m_IDefectList[i];
			m_IDefectList[i] = NULL;
		}
	}
}
DefectListManage * DefectListManage::GetInstance()
{
	if (NULL == m_pInstace)
	{
		m_pInstace = new DefectListManage;
	}
	return m_pInstace;
}

void DefectListManage::DestroyInstance()
{
	if (NULL != m_pInstace)
	{
		delete m_pInstace;
		m_pInstace = NULL;
	}
}
DefectList*  DefectListManage::getDefectListInstance(int iStation)
{

	if (iStation >= 0 && iStation < sizeof(m_IDefectList) / sizeof(iDefectList*))
	{
		if (m_IDefectList[iStation] == NULL)
		{
			m_IDefectList[iStation] = new DefectList;
		}
		return m_IDefectList[iStation];
	}
	return NULL;
}
void  DefectListManage::ReleaseStation(int iStation)
{
	if (iStation >= 0 && iStation < sizeof(m_IDefectList) / sizeof(iDefectList*))
	{
		if (m_IDefectList[iStation] != NULL)
		{
			delete m_IDefectList[iStation];
			m_IDefectList[iStation] = NULL;
		}
	}
}

