#include "configfilemgr.h"
#include "jdir.h"
#include "jfile.h"
#include "globalfunction.h"


#define  NEWLINE_CHAR    "\n"
#define  EQUAL_MARK_CHAR       "="

CConfigFileMgr::CConfigFileMgr()
    : m_bIsSysFileInit(false)
    , m_bIsUserFileInit(false)
    , m_bIsSrvFileInit(false)
    , m_bMutex(true)
{
	m_strSysFileName.clear();
	m_strUserFileName.clear();
	m_strSrvFileName.clear();
    m_Locker.Init();
    m_LockerConfig.Init();
}

CConfigFileMgr::~CConfigFileMgr()
{
	try
	{
		Release();   
	}
	catch(...)
	{

	}
}

void CConfigFileMgr::Release()
{
    try
    {
        m_RunInfoInI.Close();
        m_SysConfigDat.Close();
        m_bIsSysFileInit = false;
        m_bIsUserFileInit = false;
        m_bIsSrvFileInit = false;
        m_strSysFileName.clear();
        m_strUserFileName.clear();
        m_strSrvFileName.clear();
    }
    catch(...)
    {
        return;
    }
}

bool CConfigFileMgr::InitConfig()
{

	try
	{

		OS_wstring wstrMoudlePath;

                int iSize = CGlobalFunction::GetModuleDirectory(NULL,0);

		if(0 != iSize)
		{
			OS_wchar_t* pswz = new OS_wchar_t[iSize+1];
			memset((void*)pswz,0,(iSize+1)*sizeof(OS_wchar_t));
                        CGlobalFunction::GetModuleDirectory(pswz,iSize);
			wstrMoudlePath = pswz;
                        delete []pswz;
		}


		
		string strINIFileName = MyWideCharToMultiByte(wstrMoudlePath.c_str())+"RunInfo.ini";
		string strConfigDat = MyWideCharToMultiByte(wstrMoudlePath.c_str())+"SysConfig.dat";
		m_RunInfoInI.Open(strINIFileName.c_str());
		m_SysConfigDat.Open(strConfigDat.c_str());

		OS_wchar_t pswzDir[MAX_PATH];
		memset(pswzDir, 0, sizeof(pswzDir));
                CGlobalFunction::GetUserDataDirectory(pswzDir,MAX_PATH);
		OS_wstring wstrUserDataPath = pswzDir;

		if(wstrMoudlePath.empty()||wstrUserDataPath.empty())
		{
			printf("GetMoudlePath or UserDataPath Failed \n");
                        return false;
		}
		m_strSysFileName.append(wstrMoudlePath);
//windows 下的用户数据区和linux不同
		m_strUserFileName.append(wstrUserDataPath);
		m_strSrvFileName.append(wstrMoudlePath);
        m_strSysFileName.append(_OSW(L"SysSetting.xml"));

                m_strUserFileName.append(_OSW(L"UserSetting.xml"));

                m_strSrvFileName.append(_OSW(L"SrvSetting.xml"));

		if(!(InitConfigFile(m_strSysFileName.c_str() ,SYSFILE)))
		{
			m_strSysFileName.clear();
		}
		if(!(InitConfigFile(m_strUserFileName.c_str() ,USERFILE)))
		{
			m_strUserFileName.clear();
		}

		return true;
	}catch(...)
	{
		return false;
	}
        return false;
}

bool CConfigFileMgr::InitConfigFile(const OS_wchar_t* pszFile ,E_SETTING_FILE_TYPE enFileType)
{
    JFile file;
    try
    {

        CISAPUGuard<JMutex> grard(m_Locker);
        if (NULL == pszFile || pszFile[0] == 0 || OS_wcslen(pszFile)/**2*/ > MAX_PATH )
        {
            return false;
        }

        OS_wstring wstrFile = pszFile;



        bool bFindSysFile = FindConfigFile(m_strSysFileName);
        bool bFindUserFile = FindConfigFile(m_strUserFileName);
        bool bFindSrvFile = FindConfigFile(m_strSrvFileName);
        bool bFindAnyFile = FindConfigFile(wstrFile);

        if ( ((enFileType == SYSFILE && m_strSysFileName.empty()) || !bFindSysFile )
            || ((enFileType == SRVFILE && m_strSrvFileName.empty()) || !bFindSrvFile )
            || ((enFileType == USERFILE && m_strUserFileName.empty())|| !bFindUserFile ) || !bFindAnyFile ) //g122020 2010.02.10 modify for lint
            {
            OS_wstring wstrPath = _OSW(L"");
            OS_wstring::size_type iPos = wstrFile.find_last_of(MyMultiByteToWideChar("/"));
            // 取目录
            if(iPos != OS_wstring::npos)
            {
                wstrPath = wstrFile.substr(0, iPos + 1);
            }
            // 创建目录
            JDir dir(MyWideCharToMultiByte(wstrPath.c_str()));
            if(!dir.exists())
            {
                dir.mkdir(MyWideCharToMultiByte(wstrPath.c_str()));
            }

            //去掉只读属性

            file.setFileName(MyWideCharToMultiByte(wstrFile.c_str()));

            if(file.exists())
                file.setPermissions(JFile::WriteUser|JFile::ReadUser|JFile::ReadOwner |JFile::WriteOwner |JFile::ExeOwner|JFile::ExeUser
                	|JFile::ReadGroup|JFile::WriteGroup|JFile::ExeGroup|JFile::ReadOther|JFile::WriteOther|JFile::ExeOther);
       	    else
       	    {
       	    	     bool bRet = file.open("ab+");  //打开文件，设置为可读可写，如果文件不存在则自动创建一个

            	if(!bRet)   //文件打开失败
            	{
              	  return false;
          	  }
			char pszCheck[10];
			memset(pszCheck, 0, sizeof(pszCheck));
          	  unsigned int size = file.read(pszCheck,sizeof(pszCheck) - 1);
         	   if ( 0 == size)
         	   {
                char pszInitInfo[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<studiodoc>\r\n</studiodoc>";
           	     char pszUTF8head[128] ={0xEF,0xBB,0xBF};
                strcat(pszUTF8head,pszInitInfo);
                size =  file.write(pszUTF8head,strlen(pszUTF8head)*sizeof(char));
                if(0 == size)
                {
                    file.close();
                    return false;
                }
            }

            file.close();
       	    } 

            switch(enFileType)
            {
                                case SYSFILE:
                m_strSysFileName = wstrFile;
                break;
                                case USERFILE:
                m_strUserFileName = wstrFile;
                break;
                                case SRVFILE:
                m_strSrvFileName = wstrFile;
                break;
                                default:
                break;
            }
        }

        if (!InitXMLFile(wstrFile.c_str(),enFileType))
        {
            //  TraceErrorMessage(TL_ERRO,"Init System file failed!");
            return false;
        }

        return true;
    }
    catch (...)
    {
        if (file.isOpen())
        {
            file.close();
        }
        //  TraceErrorMessage(TL_ERRO,"InitSysConfigFile() exception!");
        return false;
    }
    return true;
}
bool CConfigFileMgr::FindConfigFile(const OS_wstring& wstrPath)
{
    bool bFindFile = false;
    JFile::exists(MyWideCharToMultiByte(wstrPath.c_str()));
    return bFindFile;

}


bool CConfigFileMgr::InitXMLFile(const OS_wchar_t* pszFile,E_SETTING_FILE_TYPE enFileType)
{
    try
    {
        //在InitConfigFile函数中已经对pszFile进行有效性判断,并且该函数为private,这里不做判断
        if ((enFileType == SYSFILE && m_bIsSysFileInit)
            || (enFileType == USERFILE && m_bIsUserFileInit)
            || (enFileType == SRVFILE && m_bIsSrvFileInit))
            {
            if(enFileType == SYSFILE)
            {
                // TraceErrorMessage(TL_INFO,"System file has been inited!");
            }
            else if(enFileType == USERFILE)
            {
                // TraceErrorMessage(TL_INFO,"User file has been inited!");
            }
            else if (enFileType == SRVFILE)
            {
                // TraceErrorMessage(TL_INFO,"Service file has been inited!");
            }
            return true;
        }
        string strFileName = CGlobalFunction::MyWideCharToMultiByte(pszFile,CP_ACP);
        switch(enFileType)
        {
                case SYSFILE:
            {
                if (m_SysMarkUp.Load(strFileName.c_str()))
                {
                    if(m_SysMarkUp.IsWellFormed() && m_SysMarkUp.FindElem(XML_HEADER_NAME))
                    {
                        m_SysMarkUp.ResetPos();
                        m_bIsSysFileInit = true;
                        return true;
                    }
                }
            }
            break;
                case USERFILE:
            {

                /* 如果文件被破坏,将自动处理成符合xml文件的格式. */
                if (!(m_UserMarkUp.Load(strFileName.c_str())))
                {
                    /*
                                        DWORD dwAttribute = ::GetFileAttributes(pszFile);
                                        if(INVALID_FILE_ATTRIBUTES != dwAttribute)
                                        {
                                                ::SetFileAttributesW(pszFile, FILE_ATTRIBUTE_NORMAL);
                                        }

                                        ::DeleteFileW(pszFile);
                                        HANDLE hFile = ::CreateFileW(pszFile,GENERIC_READ|GENERIC_WRITE,NULL,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
                                    */
                    //删除原先损坏的文件
                    JFile file(MyWideCharToMultiByte(pszFile));
                    if(file.isOpen())
                    {
                        file.close();
                    }

                    file.remove();

                    file.open("a+b");

                    char pszInitInfo[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<studiodoc>\r\n</studiodoc>";
                    char pszUTF8head[128] ={0xEF,0xBB,0xBF};
                    strcat(pszUTF8head,pszInitInfo);

                    unsigned long size = file.write(pszUTF8head,strlen(pszUTF8head)*sizeof(char));
                    if(0 == size)
                    {
                        file.close();
                        //   TraceErrorMessage(TL_ERRO,"Write file failed!");
                        return false;
                    }

                    file.close();
                }

                if (m_UserMarkUp.Load(strFileName.c_str()))
                {
                    if(m_UserMarkUp.IsWellFormed() && m_UserMarkUp.FindElem(XML_HEADER_NAME))
                    {
                        m_UserMarkUp.ResetPos();
                        m_bIsUserFileInit = true;
                        return true;
                    }
                }
            }
            break;
                default:
            break;
        }
        return false;
    }
    catch (...)
    {
        //TraceErrorMessage(TL_ERRO,"InitSysFile() exception!");
        return false;
    }
        return false;
}

int CConfigFileMgr::GetSetting(const OS_wchar_t* pszModuleName,const OS_wchar_t* pszItemName,OS_wchar_t* pszValue,unsigned int uiLength,E_SETTING_FILE_TYPE enFileType)
{
    CMarkupSTL markup;
    unsigned int iValLength = 0;
    try
    {


        CISAPUGuard<JMutex> grard(m_Locker);
        if (NULL == pszModuleName || pszModuleName[0] == 0
            || NULL == pszItemName || pszItemName[0]== 0)
        {
            //          TraceErrorMessage(TL_ERRO,"NULL == strModuleName || NULL == strItemName!");
            return iValLength;
        }

        string strModuleName("");
        string strItemName("");
        string strValue("");

        strModuleName = MyWideCharToMultiByte(pszModuleName,CP_UTF8);
        strItemName = MyWideCharToMultiByte(pszItemName,CP_UTF8);

        if (strModuleName.length() > MAX_PATH || strItemName.length() > MAX_PATH )
        {
            //      TraceErrorMessage(TL_ERRO,"NULL == strModuleName || NULL == strItemName!");
            return iValLength;
        }

        if ( (enFileType == SYSFILE && m_strSysFileName.empty())
            || (enFileType == USERFILE && m_strUserFileName.empty())
            || (enFileType == SRVFILE && m_strSrvFileName.empty()) )
            {
            //     TraceErrorMessage(TL_ERRO,"System file is empty!");
            return iValLength;
        }

        SetMarkupSTL(enFileType,markup);

        if (!markup.FindElem(XML_HEADER_NAME))
        {
            markup.ResetPos();
            //      TraceErrorMessage(TL_ERRO,"FindElem(main) failed!");
            return iValLength;
        }
        if (markup.IntoElem() && markup.FindElem(strModuleName.c_str()))
        {
            if (!markup.FindChildElem(strItemName.c_str()))
            {
                markup.ResetPos();
                return iValLength;
            }
            else
            {
                strValue = markup.GetChildData();

                OS_wstring wstrValue = _OSW(L"");
                wstrValue = MyMultiByteToWideChar(strValue.c_str(),CP_UTF8);
                iValLength = (unsigned int)(wstrValue.length() + 1);

                if (uiLength <= 0 || NULL == pszValue)
                {
                    markup.ResetPos();
                    return iValLength;
                }

                if( uiLength < wstrValue.length())
                {
                    markup.ResetPos();
                    return 0;
                }
                OS_wcscpy(pszValue,wstrValue.c_str());
                markup.ResetPos();

                return iValLength;
            }

        }
        markup.ResetPos();
        return iValLength;
    }
    catch (...)
    {
        markup.ResetPos();
        return iValLength;
    }
    return iValLength;
}
bool CConfigFileMgr::SetSetting(const OS_wchar_t* pszModuleName,const OS_wchar_t* pszItemName,
                                const OS_wchar_t* pszValue,E_SETTING_FILE_TYPE enFileType)
{
    CMarkupSTL markup;
    try
    {

        CISAPUGuard<JMutex> grard(m_Locker);
        if (NULL == pszModuleName || pszModuleName[0] ==0
            || NULL == pszItemName || pszItemName[0]== 0)
        {
            return false;
        }

        string strModuleName("");
        string strItemName("");
        string strValue("");

        strModuleName = MyWideCharToMultiByte(pszModuleName,CP_UTF8);
        strItemName = MyWideCharToMultiByte(pszItemName,CP_UTF8);
        strValue = MyWideCharToMultiByte(pszValue,CP_UTF8);

        if (strModuleName.length() > MAX_PATH	|| strItemName.length() > MAX_PATH || OS_wcslen(pszValue) > MAX_PATH)
        {
            return false;
        }

        if ( (enFileType == SYSFILE && m_strSysFileName.empty())
            || (enFileType == USERFILE && m_strUserFileName.empty())
            || (enFileType == SRVFILE && m_strSrvFileName.empty() ))
            {
            return false;
        }

        SetMarkupSTL(enFileType,markup);

        if (!markup.FindElem(XML_HEADER_NAME))
        {
            markup.ResetPos();
            return false;
        }
        if (markup.FindChildElem(strModuleName.c_str()))
        {
            if (markup.IntoElem())
            {
                if (markup.FindChildElem(strItemName.c_str()))
                {
                    if ( !markup.SetChildData(strValue.c_str()))
                    {
                        markup.ResetPos();
                        return false;
                    }
                }
                else
                {
                    if (!markup.AddChildElem(strItemName.c_str(),strValue.c_str()))
                    {
                        markup.ResetPos();
                        return false;
                    }
                }
            }
            else
            {
                markup.ResetPos();
                return false;
            }
        }
        else
        {
            if (!markup.AddChildElem(strModuleName.c_str()) || !markup.IntoElem()
                || !markup.AddChildElem(strItemName.c_str(),strValue.c_str()))
                {
                markup.ResetPos();
                return false;
            }
        }

        string strFileName = GetFileName(enFileType);
        if (!markup.Save(strFileName.c_str()))
        {
            markup.ResetPos();
            return false;
        }

        markup.ResetPos();
        RestoreMarkupSTL(enFileType,markup);

        return true;
    }
    catch (...)
    {
        markup.ResetPos();
        return false;
    }
    return false;
}

bool CConfigFileMgr::DelSetting(const OS_wchar_t* pszModuleName,const OS_wchar_t* pszItemName,E_SETTING_FILE_TYPE enFileType)
{
    CMarkupSTL markup;
    try
    {

        CISAPUGuard<JMutex> grard(m_Locker);

        if ( (enFileType == SYSFILE && m_strSysFileName.empty())
            || (enFileType == USERFILE && m_strUserFileName.empty())
            || (enFileType == SRVFILE && m_strSrvFileName.empty() ))
            {
            return false;
        }

        string strModuleName("");
        string strItemName("");

        strModuleName = MyWideCharToMultiByte(pszModuleName,CP_UTF8);
        strItemName =MyWideCharToMultiByte(pszItemName,CP_UTF8);

        if (strModuleName.length() > MAX_PATH	|| strItemName.length() > MAX_PATH)
        {
            return false;
        }

        SetMarkupSTL(enFileType,markup);

        if (!markup.FindElem(XML_HEADER_NAME))
        {
            markup.ResetPos();
            return false;
        }
        if (markup.FindChildElem(strModuleName.c_str()))
        {
            if ("" != strItemName)//是否查找子键
            {
                if (markup.IntoElem())
                {
                    if (!markup.FindChildElem(strItemName.c_str()))
                    {
                        markup.ResetPos();
                        return false;
                    }
                }
            }

            string strFileName = GetFileName(enFileType);
            if ( markup.RemoveChildElem()&& markup.Save(strFileName.c_str()))
            {
                markup.ResetPos();

                return true;
            }
        }
        markup.ResetPos();
        return false;
    }
    catch (...)
    {
        markup.ResetPos();
        return false;
    }
    return false;
}

void CConfigFileMgr::SetMarkupSTL(E_SETTING_FILE_TYPE enFileType,CMarkupSTL& markup)
{
    switch(enFileType)
    {
    case SYSFILE:
        markup = m_SysMarkUp;
        break;
    case USERFILE:
        markup = m_UserMarkUp;
        break;
    default:
        break;
    }
}


void CConfigFileMgr::RestoreMarkupSTL(E_SETTING_FILE_TYPE enFileType,CMarkupSTL& markup)
{
    switch(enFileType)
    {
    case SYSFILE:
        m_SysMarkUp = markup;
        break;
    case USERFILE:
        m_UserMarkUp = markup;
        break;
    default:
        break;
    }
}

string CConfigFileMgr::GetFileName(E_SETTING_FILE_TYPE enFileType)
{
    try
    {
        OS_wstring wstrFileName =_OSW(L"");
        string strFileName ="";
        switch(enFileType)
        {
                case SYSFILE:
            wstrFileName = m_strSysFileName;
            break;
                case USERFILE:
            wstrFileName = m_strUserFileName;
            break;
                default:
            break;
        }



        if(!OS_wcscmp(wstrFileName,_OSW(L"")))
        {
            strFileName  = MyWideCharToMultiByte(wstrFileName.c_str(),CP_ACP);
        }
        return strFileName;
    }
    catch (...)
    {
        return "";
    }
        return "";

}


string CConfigFileMgr::GetConfigFromINIFile(const char* pszModuleName, const char* pszItemName, E_INI_FILE_TYPE enType)
{
    CISAPUGuard<JMutex> grard(m_LockerConfig);
	string strValue;
	switch (enType)
	{
		case RUNINFO:
			strValue = m_RunInfoInI.GetConfig(pszModuleName,pszItemName);
			break;
		case SYSCONFIG:
			strValue = m_SysConfigDat.GetConfig(pszModuleName,pszItemName);
			break;
		default:
			strValue = "";
	}
	return strValue;
}

bool CConfigFileMgr::SetConfigToIniFile(const char * pszModuleName, const char * pszItemName, const char * pszValue, E_INI_FILE_TYPE enType)
{
    CISAPUGuard<JMutex> grard(m_LockerConfig);
    switch (enType)
    {
    case RUNINFO:
        return m_RunInfoInI.SetConfig(pszModuleName,pszItemName, pszValue);
    case SYSCONFIG:
        return m_SysConfigDat.SetConfig(pszModuleName,pszItemName, pszValue);
    default:
        return false;
    }
    return false;
}

#define  NEWLINE_CHAR    "\n"
#define  EQUAL_MARK_CHAR       "="


CGetINIConfig::CGetINIConfig(void)
{
	m_pFile = NULL;
	m_bOpen = false;

}

CGetINIConfig::~CGetINIConfig()
{
        // Close file
	if(NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}
string CGetINIConfig::GetConfig(const char *pszSectionName, const char *pszItemName)
{
	if (NULL == pszSectionName || NULL == pszItemName || NULL == m_pFile)
	{
		return "";
	}
	string strResult = "";
	int iPos = FindMapSectionPos(pszSectionName);
	if (iPos >= 0)
	{
		fseek(m_pFile, iPos, SEEK_SET);
		strResult = ReadString(pszItemName, m_pFile);
	}
	return strResult;
}

bool CGetINIConfig::SetConfig(const char * pszSectionName, const char * pszItemName, const char * pszValue)
{
    if (NULL == pszSectionName || NULL == pszItemName || NULL == m_pFile)
    {
        return false;
    }

    fseek(m_pFile, 0, SEEK_END);
    long fileLen = ftell(m_pFile);
    fseek(m_pFile, 0, SEEK_SET);
    int iPos = FindMapSectionPos(pszSectionName);
    if (iPos >= 0 && iPos < fileLen)
    {
        fseek(m_pFile, iPos, SEEK_SET);
        int buffSize = fileLen - iPos;
        char * pBuff = new char[buffSize];
        if (pBuff == NULL)
        {
            return false;
        }

        memset(pBuff, 0, buffSize);

        while (!feof(m_pFile))
        {
            long fPos = ftell(m_pFile);  // remain the place we should replace
            if (NULL != fgets(pBuff, buffSize, m_pFile))
            {
                if (strstr(pBuff, pszItemName) == pBuff)
                {
                    size_t remains = fread(pBuff, 1, buffSize, m_pFile);
                    fseek(m_pFile, 0, SEEK_SET);
                    char * pReload = new char[fPos];
                    fread(pReload, 1, fPos, m_pFile);
                    fclose(m_pFile);
                    m_pFile = fopen(m_strFilename.c_str(), "wb");
                    fwrite(pReload, 1, fPos, m_pFile);
                    delete []pReload;
                    fputs(pszItemName, m_pFile);
                    fputs("=", m_pFile);
                    if (NULL != pszValue)
                    {
                        fputs(pszValue, m_pFile);
                    }
                    fputs("\n", m_pFile);
                    fwrite(pBuff, 1, remains, m_pFile);
                    delete []pBuff;
                    fclose(m_pFile);
                    m_pFile = fopen(m_strFilename.c_str(), "rb");

                    m_mapSectionPos.clear();
                    return true;
                }
            }
        }

        delete []pBuff;
    }
    return false;
}

int CGetINIConfig::FindMapSectionPos(const char *pszSectionName)
{
	int iPos = 0;

	if (NULL == pszSectionName)
	{
		return iPos;
	}

	string strSectionName = pszSectionName;
	if ("" == strSectionName)
	{
		return iPos;
	}

	map<string, int>::iterator itel;
	itel = m_mapSectionPos.find(strSectionName);
	if (itel != m_mapSectionPos.end())
	{
		iPos = itel->second;
		return iPos;
	}

	iPos = FindSectionPos(strSectionName);


	if (iPos >= 0)
	{
		m_mapSectionPos[strSectionName] = iPos;
	}
	return iPos;
}
int CGetINIConfig::FindSectionPos(const string strSectionName)
{
	int iPos = 0;

	char* pBuf = new char[1025];
     
        // judge whether the room has been apply
	if(NULL == pBuf)
	{
		return -1;
	}
	bool bFound = false;

	string strBuf = "";
	string strTemp1 = "[" + strSectionName + "]";
	string strTemp2 = NEWLINE_CHAR + strTemp1;

	if (NULL == m_pFile)
	{
                delete[] pBuf;  // the room should be free.
		return -1;
	}

	fseek(m_pFile, 0, SEEK_SET);
	while (1)
	{
        memset(pBuf, 0, sizeof(char)*1025);
        (void)fread(pBuf, sizeof(char), sizeof(char)*1024, m_pFile);
		strBuf += pBuf;
		size_t iIndex = string::npos;
		iIndex = strBuf.find(strTemp2);
		if (string::npos != iIndex)
		{
			iPos = sizeof(char)*iIndex;
			bFound =true;
			break;
		}
		iIndex = strBuf.find(strTemp1 + "\r\n");
		if (string::npos != iIndex)
		{
			iPos = sizeof(char)*iIndex;
			bFound =true;
			break;
		}
		iIndex = strBuf.find(strTemp1 + "\r");
		if (string::npos != iIndex)
		{
			iPos = sizeof(char)*iIndex;
			bFound =true;
			break;
		}
		iIndex = strBuf.find(strTemp1 + "\n");
		if (string::npos != iIndex)
		{
			iPos = sizeof(char)*iIndex;
			bFound =true;
			break;
		}
		if (feof(m_pFile))
		{
			break;
		}
	}

	delete[] pBuf;

	if (!bFound)
	{
		iPos = -1;
	}

	return iPos;
}
string CGetINIConfig::ReadString(const char* pszKeyName, FILE* pFile)
{
	string strBuf = "";
	string strTemp = pszKeyName;
	strTemp = NEWLINE_CHAR + strTemp + EQUAL_MARK_CHAR;
	string strSub = "";
	char* pBuf = new char[1025];
	while (1)
	{
		memset(pBuf, 0, sizeof(char)*1025);
        (void)fread(pBuf, sizeof(char), sizeof(char)*1024, pFile);
		strBuf = pBuf;

		strBuf = strSub + strBuf;
		if ("" == strBuf)
		{
			break;
		}

		size_t iIndex = string::npos;
		size_t iIndex2 = iIndex;
		do{
			iIndex = strBuf.find(strTemp);
			if (string::npos == iIndex)
			{
				strSub = strBuf;
				break;
			}
			else
			{
				size_t iSize = strBuf.length();
				strSub = strBuf.substr(iIndex + strTemp.length(), iSize - iIndex - strTemp.length());
				iIndex2 = strSub.find_first_of(NEWLINE_CHAR);
				if (string::npos == iIndex2)
				{
					if (feof(pFile))
					{
						delete[] pBuf;
						return ReplaceEnterNewline(strSub);
					}

					strSub = strBuf.substr(iIndex, iSize - iIndex);
					break;
				}
				string strResult = "";
				if (0 == iIndex2)
				{
					delete[] pBuf;
					return ReplaceEnterNewline(strResult);
				}

				if ('\r' == strSub[iIndex2 -1])
				{
					strResult = strSub.substr(0, iIndex2 - 1);
				}
				else
				{
					strResult = strSub.substr(0, iIndex2);
				}

				delete[] pBuf;
				return ReplaceEnterNewline(strResult);
			}
		}while (string::npos != iIndex);

		if (feof(pFile))
		{
			break;
		}

	}

	delete[] pBuf;
	return "";
}

bool CGetINIConfig::Open(const char* pFileName)
{
	if (NULL == pFileName || strlen(pFileName) == 0)
	{
		return false;
	}

	FILE*	pFile = NULL;
	pFile = fopen(pFileName, "rb");
	if (NULL == pFile)
	{
		return false;
	}

	if (NULL != m_pFile)
	{
		Close() ;
	}

    m_bOpen = true;
    m_strFilename = pFileName;
	m_pFile = pFile;

	m_mapSectionPos.clear();

	return true;
}

bool CGetINIConfig::Close()
{
	if (NULL != m_pFile)
	{
		fclose(m_pFile) ;
		m_pFile = NULL ;
	}
	m_bOpen = false ;
	return true;
}


string CGetINIConfig::ReplaceEnterNewline(string& strSource)
{
	size_t iIndex = 0;
	while (1)
	{
		iIndex = strSource.find("\\r", iIndex);
		if (string::npos != iIndex)
		{
			if ('\\' == strSource[iIndex - 1])
			{
				strSource = strSource.replace(iIndex, 2, "r");
			}
			else
			{
				strSource = strSource.replace(iIndex, 2, "\r");
			}
		}
		else
		{
			break;
		}
	}

	iIndex = 0;
	while (1)
	{
		iIndex = strSource.find("\\n", iIndex);
		if (string::npos != iIndex)
		{
			if ('\\' == strSource[iIndex - 1])
			{
				strSource = strSource.replace(iIndex, 2, "n");
			}
			else
			{
				strSource = strSource.replace(iIndex, 2, "\n");
			}
		}
		else
		{
			break;
		}
	}
	return strSource;
}

