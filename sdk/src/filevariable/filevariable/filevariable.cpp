#include "filevariable.h"
#include "jdir.h"

#define  FILEVARIABLE_ROOT_PATH   "./"

CFileVariable::CFileVariable(const string& strfileName)
{
    m_fileName=strfileName;
	createVariablemap();

}
void CFileVariable::createVariablemap()
{
	string path = FILEVARIABLE_ROOT_PATH;
	path += m_fileName;
	JDir tmpdir(FILEVARIABLE_ROOT_PATH);
	if (!tmpdir.exists())
	{
		tmpdir.mkdir(FILEVARIABLE_ROOT_PATH);
	}
	m_file.SetPath(path);
	m_file.ReadFile();
	for (int i=0 ;i<m_file.GetNumKeys();i++)
	{
		string strGroup = m_file.GetKeyName(i);
		for (int j=0;j<m_file.GetNumValues(strGroup);j++)
		{
			string strKey = m_file.GetValueName(i,j);
			string strVaule = m_file.GetValue(strGroup,strKey);
			setValue(strGroup,strKey,strVaule);
		}
	}
}

bool CFileVariable::synAllValueToFile()
{
	return true;
}

const CGroupVariable* CFileVariable::getGroupVariable( const string& strGroupName )
{
    CISAPUGuard<JMutex> autolock(m_mapmutex);
	if (m_mapGroupVariable.find(strGroupName) == m_mapGroupVariable.end())
	{
		return NULL;
	}
	return m_mapGroupVariable[strGroupName];
}

bool CFileVariable::setValue( const string &Group, const string &Key, const CVariantValue & value ,bool bSyn /*= false*/ )
{
    CISAPUGuard<JMutex> autolock(m_mapmutex);
	if (m_mapGroupVariable.find(Group) == m_mapGroupVariable.end()|| m_mapGroupVariable[Group] == NULL )
	{
		m_mapGroupVariable.insert(pair<string,CGroupVariable*>(Group,new CGroupVariable(Group,&m_file)));
	}
    return m_mapGroupVariable[Group]->setValue(Key,value,bSyn);
 
}

CVariantValue CFileVariable::getValue( const string &Group, const string &Key, const CVariantValue & defaultValue /*= CVariantValue() */ )
{
    CISAPUGuard<JMutex> autolock(m_mapmutex);
	if (m_mapGroupVariable.find(Group) == m_mapGroupVariable.end() || m_mapGroupVariable[Group] == NULL)
	{
		setValue(Group,Key,defaultValue);
		return defaultValue;
	}
    return m_mapGroupVariable[Group]->getValue(Key,defaultValue);
}

bool CFileVariable::bindValue(const string &Group, const string &Key, const CVariantPoint &value)
{
    CISAPUGuard<JMutex> autolock(m_mapmutex);
    if (m_mapGroupVariable.find(Group) == m_mapGroupVariable.end()|| m_mapGroupVariable[Group] == NULL )
    {
        m_mapGroupVariable.insert(pair<string,CGroupVariable*>(Group,new CGroupVariable(Group,&m_file)));
    }
    return m_mapGroupVariable[Group]->bindValue(Key,value);
}


vector<string> CFileVariable::getAllKeys(const string& strGroupName)
{
	CISAPUGuard<JMutex> autolock(m_mapmutex);
	if (m_mapGroupVariable.find(strGroupName) == m_mapGroupVariable.end() || m_mapGroupVariable[strGroupName] == NULL)
	{
		return vector<string>();
	}
	return m_mapGroupVariable[strGroupName]->childKeys();
}

CGroupVariable::CGroupVariable( const string& Group , CIniFile* pIniFile)
{
	m_GroupName = Group;
	m_pIniFile = pIniFile;
}

const CKeyVariable* CGroupVariable::getKeyVariable( const string& strKeyName )
{
    CISAPUGuard<JMutex> autolock(m_gorupmutex);
	if (m_mapKeyVariable.find(strKeyName) == m_mapKeyVariable.end())
	{
		return NULL;
	}
	return m_mapKeyVariable[strKeyName];
}

bool CGroupVariable::setValue( const string &Key, const CVariantValue & value ,bool bSyn /*= false*/ )
{
    CISAPUGuard<JMutex> autolock(m_gorupmutex);
	if (m_mapKeyVariable.find(Key) == m_mapKeyVariable.end() || m_mapKeyVariable[Key] == NULL)
	{
		m_mapKeyVariable.insert(pair<string,CKeyVariable*>(Key,new CKeyVariable(m_GroupName,Key,m_pIniFile)));
	}
    return m_mapKeyVariable[Key]->setValue(value,bSyn);
}

CVariantValue CGroupVariable::getValue( const string &Key, const CVariantValue & defaultValue /*= CVariantValue() */ )
{
    CISAPUGuard<JMutex> autolock(m_gorupmutex);
	if (m_mapKeyVariable.find(Key) == m_mapKeyVariable.end() || m_mapKeyVariable[Key] == NULL)
	{
		setValue(Key,defaultValue);
		return defaultValue;
	}
    return m_mapKeyVariable[Key]->getValue(defaultValue);
}

bool CGroupVariable::bindValue(const string &Key, const CVariantPoint &value)
{
    CISAPUGuard<JMutex> autolock(m_gorupmutex);
    if (m_mapKeyVariable.find(Key) == m_mapKeyVariable.end() || m_mapKeyVariable[Key] == NULL)
    {
        m_mapKeyVariable.insert(pair<string,CKeyVariable*>(Key,new CKeyVariable(m_GroupName,Key,m_pIniFile)));
    }
    return m_mapKeyVariable[Key]->bindValue(value);
}

vector<string> CGroupVariable::childKeys()
{
	vector<string> keys;
	map<string, CKeyVariable*>::iterator iter = m_mapKeyVariable.begin();
	for (iter; iter != m_mapKeyVariable.end(); iter++)
	{
		keys.push_back(iter->first);
	}

	return keys;
}

CKeyVariable::CKeyVariable(const string& Group,const string& key , CIniFile* pIniFile)
{
	m_KeyName = key;
	m_GroupName = Group;
	m_pIniFile = pIniFile;
	m_bSyn = false;
}

CKeyVariable::~CKeyVariable()
{
    CISAPUGuard<JMutex> autolock(m_mutex);
	if (m_bSyn)
	{
		m_pIniFile->SetValue(m_GroupName,m_KeyName,varValue.getValueString());
		m_pIniFile->WriteFile();
	}
    pValue.clearPoint();
}


bool CKeyVariable::setValue( const CVariantValue & value ,bool bSyn /*= false*/ )
{
    CISAPUGuard<JMutex> autolock(m_mutex);
	varValue = value;
    m_bSyn = bSyn;
    bool bflag = true;
    if (m_bSyn)
    {
        bflag = m_pIniFile->SetValue(m_GroupName,m_KeyName,value.getValueString());
        bflag = m_pIniFile->WriteFile();
	}
    //pValue.setValue(varValue);
    for (unsigned int index = 0 ;index < pValueVect.size();index++)
    {
        pValueVect.at(index).setValue(varValue);
    }
    
    return bflag;
}

CVariantValue CKeyVariable::getValue( const CVariantValue & defaultValue /*= CVariantValue() */ )
{
    return varValue;
}

bool CKeyVariable::bindValue(const CVariantPoint &value)
{
    CISAPUGuard<JMutex> autolock(m_mutex);
    bool bflag = true;
    //pValue = value;
    //if(varValue.getValueString().length() >0)
    //{
    //    bflag = pValue.setValue(varValue);
    //}
    if(varValue.getValueString().length() >0)
    {
        pValueVect.push_back(value);
        pValueVect.at(pValueVect.size()-1).setValue(varValue);
    }

    return bflag;
}


CFileVariableMap* CFileVariableMap::m_pInstance =NULL;
CFileVariable* CFileVariableMap::getFileVariable( const string& fileName )
{
    CISAPUGuard<JMutex> autolock(m_mapmutex);
	if (m_mapFileVariable.find(fileName) == m_mapFileVariable.end())
	{
		m_mapFileVariable.insert(pair<string,CFileVariable*>(fileName,new CFileVariable(fileName)));
	}
	return m_mapFileVariable[fileName];
}

CFileVariableMap::~CFileVariableMap()
{
    CISAPUGuard<JMutex> autolock(m_mapmutex);
	map<string,CFileVariable*>::iterator it =  m_mapFileVariable.begin();
	while(it != m_mapFileVariable.end())
	{
		if (it->second != NULL)
		{
			delete it->second;
		}
		it++;
	}
	m_mapFileVariable.clear();
}

CFileVariableMap *CFileVariableMap::GetInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new CFileVariableMap();
    }
    return m_pInstance;
}

void CFileVariableMap::DestroyInstance()
{
    if(m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

