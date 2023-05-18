#ifndef _FILE_VARIABLE_H_
#define _FILE_VARIABLE_H_
#include "filevariable/ifilevariable.h"
#include "iniFile.h"
#include <string>
#include <map>
#include <vector>
#include "jmutex.h"

using namespace std;


class CKeyVariable:public IKeyVariable
{
public :
	CKeyVariable(const string& Group,const string& key, CIniFile* pIniFile);
	~CKeyVariable();
    bool setValue ( const CVariantValue & value ,bool bSyn = false);
	CVariantValue getValue (const CVariantValue & defaultValue = CVariantValue()) ;
    bool bindValue (const CVariantPoint & value );
private: 
	CVariantValue varValue;
    CVariantPoint pValue;
    vector<CVariantPoint> pValueVect;
	CIniFile* m_pIniFile;
	string m_KeyName;	
	string m_GroupName;
	JMutex m_mutex;
	bool m_bSyn;
};

class CGroupVariable:public IGroupVariable
{
public :
	CGroupVariable(const string& Group, CIniFile* pIniFile);
	const CKeyVariable* getKeyVariable(const string& strKeyName);
    bool setValue ( const string &Key, const CVariantValue & value ,bool bSyn = false);
	CVariantValue getValue (const string &Key, const CVariantValue & defaultValue = CVariantValue() ) ;
    bool bindValue ( const string &Key, const CVariantPoint & value );
	vector<string> childKeys();

private: 
	string m_GroupName;	
	 CIniFile* m_pIniFile;
     JMutex m_gorupmutex;
	map<string,CKeyVariable*> m_mapKeyVariable;
};

class CFileVariable:public IFileVariable
{
public :
	CFileVariable(const string& strfileName);	
	bool synAllValueToFile();
	const CGroupVariable* getGroupVariable(const string& strGroupName);
    bool setValue (const string &Group, const string &Key, const CVariantValue & value ,bool bSyn = false);
	CVariantValue getValue (const string &Group, const string &Key, const CVariantValue & defaultValue = CVariantValue()) ;
    bool bindValue(const string &Group, const string &Key, const CVariantPoint & value);
	vector<string> getAllKeys(const string& strGroupName);

private: 
    void createVariablemap();
	string m_fileName;	
	CIniFile m_file;
    JMutex m_mapmutex;
	map<string,CGroupVariable*> m_mapGroupVariable;
};

class CFileVariableMap:public IFileVariableManager
{
public:
	~CFileVariableMap();
	CFileVariable* getFileVariable(const string& fileName);
    static CFileVariableMap *GetInstance();
    static void DestroyInstance();
private:
	map<string,CFileVariable*> m_mapFileVariable;
    static CFileVariableMap *m_pInstance;
    JMutex m_mapmutex;
};


#endif
