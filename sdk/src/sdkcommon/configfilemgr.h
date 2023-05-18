
#ifndef CCONFIGFILEMGR_H
#define CCONFIGFILEMGR_H

#include "jmutex.h"
#include "jthread.h"
#include "markupstl.h"
#include <string>
using std::string;
#include <map>
using std::map;

#include "mystring.h"


#define XML_HEADER_NAME "studiodoc"


enum E_SETTING_FILE_TYPE
{
        SYSFILE		= 0,
        USERFILE	= 1,
        SRVFILE		= 2,
        OTHERFILE	= 3
};

enum E_INI_FILE_TYPE
{
	RUNINFO = 0,
	SYSCONFIG =1
};



class CGetINIConfig
{

public:
	CGetINIConfig(void);
public:
	~CGetINIConfig(void);

public:
	bool Open(const char* pszFileName);
	bool Close();
	string GetConfig(const char* pszSectionName,const char* pszItemName);
    bool SetConfig(const char * pszSectionName, const char * pszItemName, const char * pszValue);

private:
	int FindMapSectionPos(const char* pszSectionName);
	int FindSectionPos(const string strSectionName);
	string ReadString(const char* pszKeyName, FILE* pFile);
	string ReplaceEnterNewline(string& strSource);
private:
	FILE*							m_pFile ;					/*!< 数据文件指针 */
	bool							m_bOpen;					/*!< 文件是否已经打开，true－已经打开，false－未打开 */
    string                          m_strFilename;
	map<string, int>                m_mapSectionPos;  
};


class CConfigFileMgr
{
public:

    CConfigFileMgr();
	~CConfigFileMgr();

private:
    void SetMarkupSTL(E_SETTING_FILE_TYPE enFileType,CMarkupSTL& markup);
    void RestoreMarkupSTL(E_SETTING_FILE_TYPE enFileType,CMarkupSTL& markup);
    bool InitXMLFile(const OS_wchar_t* pszFile,E_SETTING_FILE_TYPE enFileType);
    string GetFileName(E_SETTING_FILE_TYPE enFileType);
    bool FindConfigFile(const OS_wstring& wstrPath);
    bool InitConfigFile(const OS_wchar_t* pszFile ,E_SETTING_FILE_TYPE enFileType);
public:
    void Release();
    int GetSetting(const OS_wchar_t* pszModuleName,const OS_wchar_t* pszItemName,OS_wchar_t* pszValue,unsigned int uiLength,E_SETTING_FILE_TYPE enFileType);
    bool SetSetting(const OS_wchar_t* pszModuleName,const OS_wchar_t* pszItemName,const OS_wchar_t* pszValue,E_SETTING_FILE_TYPE enFileType);
    bool DelSetting(const OS_wchar_t* pszModuleName,const OS_wchar_t* pszItemName,E_SETTING_FILE_TYPE enFileType);
    bool InitConfig();
    string GetConfigFromINIFile(const char* pszModuleName, const char* pszItemName,E_INI_FILE_TYPE enType);
    bool SetConfigToIniFile(const char * pszModuleName, const char * pszItemName, const char * pszValue, E_INI_FILE_TYPE enType);
private:
	int FindMapSectionPos(const char* pszSectionName);
	int FindSectionPos(const string strSectionName);
	string ReadString(const char* pszKeyName, FILE* pFile);
	string ReplaceEnterNewline(string& strSource);

private:
        OS_wstring m_strSysFileName;
        OS_wstring m_strUserFileName;
        OS_wstring m_strSrvFileName;
        JMutex m_Locker;
        CMarkupSTL m_SysMarkUp;
        CMarkupSTL m_UserMarkUp;
        CMarkupSTL m_SrvMarkUp;
        bool	m_bIsSysFileInit;
        bool	m_bIsUserFileInit;
        bool	m_bIsSrvFileInit;
        bool    m_bMutex;
        JMutex m_LockerConfig;
		CGetINIConfig m_RunInfoInI;
		CGetINIConfig m_SysConfigDat;
};




#endif // CCONFIGFILEMGR_H
