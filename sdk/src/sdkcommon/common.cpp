
#include "common.h"
#include "jdir.h"
#include "markupstl.h"
#include "configfilemgr.h"
#include "mycommon.h"

#ifdef Q_OS_WIN
#include "shlobj.h"
#endif


#define XML_HEADER_NAME "studiodoc"

//Set the Openstudio' userdata dir name as the [Install] [DefDir] in in sysconfig
#define APP_INSTALL_MODULE "Install"
#define APP_INSTALL_PATH "DefDir"


 CConfigFileMgr g_ConfigFileMgr;  //全局配置文件管理类


OS_wstring Common::m_wstrAppPathName = _OSW(L"");
ISDKContainer *Common::m_pPluginContainer = NULL;

inline short CharToNum(char c)
{
	if (c >= '0' && c <= '9')           // 0123456789
	{
		return (c - '0');
	}
	else if (c >= 'A' && c <= 'F')      // ABCDEF
	{
		return (c - 0x37);              // 0x37 = 'A' - 10;
	}
	else if (c >= 'a' && c <= 'f')      // abcdef
	{
		return (c - 0x57);              // 0x57 = 'a' - 10
	}
	// c is not a valid number
	return -1;
}



ISDKContainer *Common::getPluginContainer()
{
    return m_pPluginContainer;
}

// createPluginContainer, destroyPluginContainer
// 这俩个函数仅供proxy调用，其他插件禁止调用
void Common::setPluginContainer(ISDKContainer *pPluginContainer)
{
    m_pPluginContainer = pPluginContainer;
}

bool Common::SetAppPathName(const OS_wchar_t* pswzAppPathName)
{
	if(NULL == pswzAppPathName)
	{
		return false;
	}

	string strPath = MyWideCharToMultiByte(pswzAppPathName,CP_UTF8);
	strPath+="/";
	string::size_type iPos;
	iPos = strPath.find("\\",0);
	// Change "\" to "/" on all platform
	while(string::npos != iPos)
	{
		strPath.replace(iPos,1,"/");
		iPos = strPath.find("\\",0);
	}


	m_wstrAppPathName =MyMultiByteToWideChar(strPath.c_str(),CP_UTF8);
	g_ConfigFileMgr.InitConfig();  //UI设置完路径之后初始化配置文件
	return true;
}

int Common::GetModuleDirectory(OS_wchar_t* pswzDir, int iLen)
{
	try
	{		
		if(NULL == pswzDir || 0 == iLen)
		{
			return m_wstrAppPathName.length();
		}
		else
		{
			OS_wcsncpy(pswzDir,m_wstrAppPathName.c_str(),iLen);	
			return iLen;
			
		}
	}
	catch (...)
	{
		return 0;
	}
        return 0;
}

int Common::GetUserDataDirectory(OS_wchar_t* pswzDir, int iLen)
{
	string strConfig = g_ConfigFileMgr.GetConfigFromINIFile("userdata","support",RUNINFO);
	OS_wstring strUserDataPath;

	if ("1" == strConfig)
	{
		// For SD Card
		string strPathCfg;

#ifdef Q_OS_DARWIN
		JDir jdir;
		strPathCfg += jdir.getCurrentPath();

                // xxx.app/Contents/Resources/libxx.1.0.0.dylib
                // So we should enter ../../../(same as xxx.app) to create userdata folder.
                if(string::npos != strPathCfg.find(".app", 0) || string::npos != strPathCfg.find(".APP", 0))
		{
			strPathCfg += "/../../../"; 
		}
#endif

		// Read userdata path from Runinfo.ini
		// the path can be absolute path like:
		//     c:/appuserdata            on Windows
		//     /usr/local/appuserdata    on Linux/Mac
		// also can be relative path like:
		//     ../../../appuserdata      on Windows/Linux/Mac
		strPathCfg += g_ConfigFileMgr.GetConfigFromINIFile("userdata","path",RUNINFO);

                //trim the blankspace
		int nMyPos = strPathCfg.find_last_not_of(" ");
		if (nMyPos != -1)
		{
			strPathCfg = strPathCfg.substr(0, nMyPos+1);
		}
		// path should end with '/'
		if('/' != strPathCfg.at(strPathCfg.length()-1))
		{
			strPathCfg += "/";
		}
		strUserDataPath = MyMultiByteToWideChar(strPathCfg.c_str());
	}
	else
	{
		// for PC
		strUserDataPath = GetLocalDataPath();  
                strUserDataPath += _OSW(L"userdata/");//  Remove "/" in front of extend file name
	}

	if(!strUserDataPath.empty())
	{
		string strPath = MyWideCharToMultiByte(strUserDataPath.c_str());

		JDir DirUserdata(strPath);
		if(!DirUserdata.exists())
		{
			if(true == DirUserdata.mkdir(strPath))
			{
				strPath = strPath.substr(0,strPath.length()-1);
				ChangeFileRight(strPath.c_str());
			}
			else 
			{
				return 0;
#if 0
				// path is invalid like "/..fff./app"( '/' is root and have no parent folder)
				// and we set local data path as default on PC
				strUserDataPath = GetLocalDataPath();  
                                strUserDataPath += _OSW(L"userdata/");// Remove "/" in front of extend file name

				string strLocalPath = MyWideCharToMultiByte(strUserDataPath.c_str());

				JDir DirLocalUserdata(strLocalPath);
				if(!DirLocalUserdata.exists())
				{
					if(true == DirLocalUserdata.mkdir(strLocalPath))
					{
						strLocalPath = strLocalPath.substr(0,strLocalPath.length()-1);
						ChangeFileRight(strLocalPath.c_str());
					}
					else
					{
						// do nothing.
					}
				}
#endif
			}
		}
	}

	if (NULL == pswzDir || 0 == iLen)
	{
		return strUserDataPath.length();
	}else
	{
		OS_wcsncpy(pswzDir,strUserDataPath.c_str(),iLen);
	}
	return iLen;

}

int Common::GetLogDirectory(OS_wchar_t* pswzDir, int iLen)
{
	//All Log move to LocalData for Linux/Mac
	OS_wstring strLogPath = GetLocalDataPath();
	if(!strLogPath.empty())
	{
        strLogPath += _OSW(L"log/");// Remove "/" in front of extend file name
		string strPath = MyWideCharToMultiByte(strLogPath.c_str());
        if(strPath.empty())
        {
            return 0;
        }

		JDir DirUserdata(strPath);
		if(!DirUserdata.exists())
		{
			DirUserdata.mkdir(strPath);
			strPath = strPath.substr(0,strPath.length()-1);
			ChangeFileRight(strPath.c_str());
		}
	}
	
	if (NULL == pswzDir || 0 == iLen)
	{
		return strLogPath.length();
	}else
	{
		OS_wcsncpy(pswzDir,strLogPath.c_str(),iLen);
	}

	return iLen;
}



int  Common::MultiByteToWideChar(const char* pszMultiByte, OS_wchar_t* pswzValue ,int iLen, int iCodePage)
{
	OS_wstring wstrRet;
	if (NULL == pszMultiByte )
	{
		return 0;
	}
	if ( 0 == pszMultiByte[0])
	{
		return 0;
	}
        // Can't convert on unchinese PC which has installed chinese language.
#ifdef Q_OS_WIN
	wchar_t* pwszWideChar = NULL;
	int iWideCharSize = ::MultiByteToWideChar(iCodePage, 0, pszMultiByte, -1, NULL, 0);

	if(NULL == pswzValue || 0 == iLen)
	{
		return iWideCharSize;
	}


	pwszWideChar = new wchar_t[iWideCharSize];
	memset(pwszWideChar, 0 ,iWideCharSize*sizeof(wchar_t));
	iWideCharSize = ::MultiByteToWideChar(iCodePage, 0, pszMultiByte, -1, pwszWideChar, iWideCharSize);
	if (0 == iWideCharSize)
	{
		if (NULL != pwszWideChar)
		{
			delete[] pwszWideChar;
			pwszWideChar = NULL;
		}
		return 0;
	}
        wstrRet = wstring2OS(pwszWideChar);
	delete[] pwszWideChar;
	pwszWideChar = NULL;
	OS_wcsncpy(pswzValue,wstrRet.c_str(),iLen);
#else
	switch (iCodePage)
	{
	case CP_ACP:
		{
			size_t iWlen = mbstowcs(NULL,pszMultiByte,0);
                        if ((size_t)-1 == iWlen)
			{
				//Can not be Converted !
				return 0;
			}
			
			if(NULL == pswzValue || 0 == iLen)
			{	
				return iWlen;
			}


			wchar_t*  pswz = new wchar_t[iWlen+1];
			if (NULL == pswz)
			{
				return 0;
			}
			memset(pswz,0,(iWlen+1)*sizeof(wchar_t));
			mbstowcs(pswz,pszMultiByte,iWlen);
                        wstrRet = wstring2OS(pswz);
			delete[] pswz;
			OS_wcsncpy(pswzValue,wstrRet.c_str(),iLen);
			break;
		}
	case CP_UTF8:
                wstrRet = UTF8ToOSwstring(pszMultiByte);
                if(NULL == pswzValue || 0 == iLen)
                        {
                                return wstrRet.length();
                        }else
                {
                     OS_wcsncpy(pswzValue,wstrRet.c_str(),iLen);
                     return iLen;
                }
		break;
	default:
		break;
	}
#endif
	return iLen;
}

int Common::WideCharToMultiByte(const OS_wchar_t* pwszWideChar,char* pszValue, int iLen, int iCodePage)
{
	string strRet = "";
	if (NULL == pwszWideChar)
	{
		return 0;
	}
	if ( 0 == pwszWideChar[0])
	{
		return 0;
	}
        // Can't convert on unchinese PC which has installed chinese language.
#ifdef Q_OS_WIN
        wstring wstrTmp = OS2wstring(pwszWideChar);
	int iMultiByteSize = ::WideCharToMultiByte(iCodePage, 0, wstrTmp.c_str(), -1, pszValue, iLen, NULL, NULL);
	return iMultiByteSize;
#else
	switch (iCodePage)
	{
	case CP_ACP:
		//conver OS_wchar_t -> wchar_t Then Convert wchar_t to string
		{
			setlocale(LC_CTYPE,"");

                        wstring strTmp;
                        OS2wstring(pwszWideChar,strTmp);

                        size_t iSize = wcstombs(NULL,strTmp.c_str(),0);
                        if ( (size_t)-1 == iSize)
			{
				return 0;
			}
                        if(NULL == pszValue || iLen < iSize)
                        {
                            return iSize;
                        }
                        wcstombs(pszValue,strTmp.c_str(),iSize);
			return iSize;
		}
	case CP_UTF8:				
                OSwstringToUTF8(pwszWideChar,strRet);
                if (NULL == pszValue || 0 == iLen)
		{
			return strRet.length();
		}else
		{
			strncpy(pszValue,strRet.c_str(),iLen);
			return iLen;
		}

		break;
	default:
		break;
	}
#endif

	return 0;
}
int Common::GetSetting(const OS_wchar_t* pszModuleName, const OS_wchar_t* pszItemName, OS_wchar_t* pswzValue ,int iLen)
{

	try
	{
		//读取用户配置文件

		int iSize = g_ConfigFileMgr.GetSetting(pszModuleName, pszItemName, pswzValue, iLen, USERFILE);

		if (iSize > 0)  //UserFile有该项
		{
			return iSize;
		}

		//USerFile 不存在该项，则读取系统配置文件
		iSize = 0;
		iSize = g_ConfigFileMgr.GetSetting(pszModuleName, pszItemName, pswzValue, iLen, SYSFILE);

		if(iSize > 0)
		{
			return iSize;
		}
		//SysSetting Not Find that Item Looking for it in the RunInfo.ini
		string strModuleName = MyWideCharToMultiByte(pszModuleName);
		string strItemName = MyWideCharToMultiByte(pszItemName);

		string strValue = g_ConfigFileMgr.GetConfigFromINIFile(strModuleName.c_str(),strItemName.c_str(),RUNINFO);
		OS_wstring wstrRet;
		if(!strValue.empty())
		{
			wstrRet = MyMultiByteToWideChar(strValue.c_str());
		}
		
		if(NULL == pswzValue || 0 == iLen)
		{
			return wstrRet.length();
		}else
		{
			OS_wcsncpy(pswzValue,wstrRet.c_str(),iLen);
			return iLen;
		}
		return iLen;
	}
	catch (...)
	{
		
		return 0;
	}
    return 0;
}

bool Common::SetSetting(const OS_wchar_t* pszModuleName, const OS_wchar_t* pszItemName,const OS_wchar_t* pszValue)
{
	try
	{
		//只支持对USerFile的设置更改
		if (!g_ConfigFileMgr.SetSetting(pszModuleName, pszItemName, pszValue, USERFILE))
		{
			return false;
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
        return false;
}

bool Common::DelSetting(const OS_wchar_t*  pszModuleName, const OS_wchar_t*  pszItemName)
{
	try
	{
		//只支持对USerFile的设置更改
		if (!g_ConfigFileMgr.DelSetting(pszModuleName, pszItemName, USERFILE))
		{
			return false;
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
        return false;
}


bool Common::GetIniSetting(const OS_wchar_t * pszModuleName, const OS_wchar_t * pszItemName, OS_wchar_t * pszValue, int * pLen)
{
	if (NULL == pszModuleName || NULL == pszItemName || pLen == NULL)
	{
		return false;
	}

	int nBuffSize = *pLen;
	*pLen = 0;

    try
    {
                int iSize = Common::WideCharToMultiByte(pszModuleName,NULL,0,CP_ACP);
		char* pValue = new char[iSize+1];
		memset(pValue,0,(iSize+1)*sizeof(char));
                Common::WideCharToMultiByte(pszModuleName,pValue,iSize,CP_ACP);
        string strModuleName = pValue;
                delete []pValue;
		pValue = NULL;
		iSize = 0;

                iSize = Common::WideCharToMultiByte(pszItemName,NULL,0,CP_ACP);
		pValue = new char[iSize+1];
		memset(pValue,0,(iSize+1)*sizeof(char));
                Common::WideCharToMultiByte(pszItemName,pValue,iSize,CP_ACP);
        string strItemName = pValue;
                delete []pValue;
		pValue = NULL;
		iSize = 0;
        // first read sysconfig.dat
        string strValue = g_ConfigFileMgr.GetConfigFromINIFile(strModuleName.c_str(),strItemName.c_str(),SYSCONFIG);

        if (strValue.empty())
        {
            strValue = g_ConfigFileMgr.GetConfigFromINIFile(strModuleName.c_str(),strItemName.c_str(),RUNINFO);
        }

        if(!strValue.empty())
		{
			OS_wstring wstrRet;
                        iSize = Common::MultiByteToWideChar(strValue.c_str(),NULL,0,CP_ACP);

			OS_wchar_t* pwValue = new OS_wchar_t[iSize+1];
			memset(pwValue,0,(iSize+1)*sizeof(OS_wchar_t));
                        Common::MultiByteToWideChar(strValue.c_str(),pwValue,iSize,CP_ACP);

			OS_wstring wstrValue = pwValue;
                        delete []pwValue;
			pwValue = NULL;
			iSize = 0;
			*pLen = (int)(wstrValue.length() + 1);

			if (nBuffSize >= (int)wstrValue.length() && NULL != pszValue)
			{
				OS_wcscpy(pszValue, wstrValue.c_str());
				return true;
			}
        }
    }
    catch (...)
    {
    }
    return false;
}

bool Common::SetIniSetting(const OS_wchar_t * pszModuleName, const OS_wchar_t * pszItemName, const OS_wchar_t * pszValue)
{
	if (NULL == pszModuleName || NULL == pszItemName || NULL == pszValue)
	{
		return false;
	}

    try
    {
                int iSize = Common::WideCharToMultiByte(pszModuleName,NULL,0,CP_ACP);
		char* pValue = new char[iSize+1];
		memset(pValue,0,(iSize+1)*sizeof(char));
                Common::WideCharToMultiByte(pszModuleName,pValue,iSize,CP_ACP);
		string strModuleName = pValue;
                delete []pValue;
		pValue = NULL;
		iSize = 0;
      
                iSize = Common::WideCharToMultiByte(pszItemName,NULL,0,CP_ACP);
		pValue = new char[iSize+1];
		memset(pValue,0,(iSize+1)*sizeof(char));
                Common::WideCharToMultiByte(pszItemName,pValue,iSize,CP_ACP);
        string strItemName = pValue;
                delete []pValue;
		pValue = NULL;
		iSize = 0;

                iSize = Common::WideCharToMultiByte(pszValue,NULL,0,CP_ACP);
		pValue = new char[iSize+1];
		memset(pValue,0,(iSize+1)*sizeof(char));
                Common::WideCharToMultiByte(pszValue,pValue,iSize,CP_ACP);
        string strValue = pValue;
                delete []pValue;
		pValue = NULL;
		iSize = 0;

        if (g_ConfigFileMgr.SetConfigToIniFile(strModuleName.c_str(), strItemName.c_str(), strValue.c_str(), SYSCONFIG)
            || g_ConfigFileMgr.SetConfigToIniFile(strModuleName.c_str(), strItemName.c_str(), strValue.c_str(), RUNINFO))
        {
            return true;
        }
    }
    catch (...)
    {
    }
    return false;
}


char * Common::chrcpy(char * dst, unsigned long dstlen, const char * src, unsigned long ulSrcLen )
{
	size_t uCopyLen = ulSrcLen;
	if (0 == ulSrcLen)
	{
		uCopyLen = strlen(src);
	}

	int len = dstlen - 1;
	if ((int)uCopyLen > len)
	{
		uCopyLen = len;
	}

	char *cp = dst;

	*cp = 0;
	unsigned long i = 0;
	for ( i= 0; i < uCopyLen; ++i)
	{
		cp[i] = src[i];
	}
	dst[i] = 0;

	return cp;
}

OS_wchar_t* Common::wchrcpy(OS_wchar_t* dst, unsigned long dstlen, const OS_wchar_t* src, unsigned long ulSrcLen)
{
	size_t uCopyLen = ulSrcLen;
	if (0 == ulSrcLen)
	{
		uCopyLen = OS_wcslen(src);
	}

	int len = dstlen - 1;
	if ((int)uCopyLen > len)
	{
		uCopyLen = len;
	}

	OS_wchar_t *cp = dst;

	*cp = 0;
	unsigned long i = 0;
	for (i = 0; i < uCopyLen; ++i)
	{
		cp[i] = src[i];
	}
	dst[i] = (OS_wchar_t)'\0';

	return cp;
}

int Common::GetDataLength(unsigned char* puszSrc, int nSrclen, unsigned char ucTag)
{
	int i = 0;
	for (i = 0; i < nSrclen; ++i)
	{
		if (ucTag == puszSrc[i])
		{
			break;
		}
	}
	return i;
}


OS_wstring Common::GetSetingsFromXml(const char* pszModule, const char* pszItem,const char* pszFilePath)
{
	string strFilePath = pszFilePath;
	string strModule = pszModule;
	string strItem = pszItem;
	string strValue;
        OS_wstring wstrValue = _OSW(L"");

	CMarkupSTL markup;
	if(!markup.Load(strFilePath.c_str()))
	{
                return _OSW(L"");
	}

	if (!markup.IsWellFormed()||!markup.FindElem(XML_HEADER_NAME)||!markup.IntoElem())
	{
		markup.ResetPos();
                return _OSW(L"");
	}
	if (markup.FindElem(strModule.c_str()))
	{
		if (!markup.FindChildElem(strItem.c_str()))
		{
			markup.ResetPos();
                        return _OSW(L"");
		}
		else
		{
			strValue = markup.GetChildData();
			wstrValue = MyMultiByteToWideChar(strValue.c_str(),CP_UTF8);
			markup.ResetPos();
			return wstrValue;
		}
	}
	else
	{
		markup.ResetPos();
                return _OSW(L"");
	}
}


void Common::ReleaseConfigFileMgr()
{
	try
	{
		g_ConfigFileMgr.Release();
	}
	catch(...)
	{
		return;
	}    
}

OS_TYPE Common::GetOSType()
{
	try
	{
#ifdef Q_OS_WIN
		OSVERSIONINFOEX osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if (!GetVersionEx((OSVERSIONINFO *) &osvi))
		{
			osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
			if (!GetVersionEx ( (OSVERSIONINFO *) &osvi))
			{
				return OS_VERSION_UNKOWN;
			}
		}

		switch (osvi.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_NT:
			{
				if ( osvi.dwMajorVersion <= 4 )
				{
					return OSVERSION_MS_NT;          /* NT*/
				}

				if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
				{
					return OSVERSION_MS_2000;        /* 2000*/
				}

				if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion >= 1 )
				{
					return OSVERSION_MS_XP;          /* XP*/
				}

				if (osvi.dwMajorVersion > 5)
				{
					return OSVERSION_MS_XP;
				}
				break;
			}
		case VER_PLATFORM_WIN32_WINDOWS:
			{
				if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
				{
					return OSVERSION_MS_95;        /* 95*/
				}
				if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
				{
					return OSVERSION_MS_98;       /* 98*/
				}
				if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
				{
					return OSVERSION_MS_ME;        /* ME*/
				}
				break;
			}
		case VER_PLATFORM_WIN32s:
			{
				return OSVERSION_MS_32;         /* 32*/
			}
		default:
			break;
		}
		return OS_VERSION_UNKOWN;
#else
		return OS_VERSION_UNKOWN;
#endif  //Q_OS_WIN
	}
	catch (...)
	{
		return OS_VERSION_UNKOWN;
	}
        return OS_VERSION_UNKOWN;
}


UINT g_iLangInfo[]=
{
	0x0401, //Arabic (Saudi Arabia)
	0x0801, //Arabic (Iraq)
	0x0c01, //Arabic (Egypt)
	0x1001, //Arabic (Libya)
	0x1401, //Arabic (Algeria)
	0x1801, //Arabic (Morocco)
	0x1c01, //Arabic (Tunisia)
	0x2001, //Arabic (Oman)
	0x2401, //Arabic (Yemen)
	0x2801, //Arabic (Syria)
	0x2c01, //Arabic (Jordan)
	0x3001, //Arabic (Lebanon)
	0x3401, //Arabic (Kuwait)
	0x3801, //Arabic (U.A.E.)
	0x3c01, //Arabic (Bahrain)
	0x4001, //Arabic (Qatar)
	0x0408, //希腊语(Greek)  /*!< A48D02301, XiaoNingBo/51407 [5/21/2006]*/
	0x0415, //波兰语(Polish)
};

char Common::GetCSVSeperator()
{
	
#ifdef Q_OS_WIN
	UINT uiLanguage = GetSystemDefaultLangID();
        int iLangTypeNum = sizeof(g_iLangInfo)/sizeof(int);  // correct the number of the g_iLangInfo array.
	for (int i = 0;i < iLangTypeNum ; ++i)
	{
		if (g_iLangInfo[i] == uiLanguage)
		{
			return  ';';
		}
	}
#endif
	return ',';
}


int Common::GetWindowsVersion()
{
#ifdef Q_OS_WIN
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&osvi) == true)
	{
		return osvi.dwMajorVersion;
	}
#endif

	return 0;
}


OS_wstring Common::GetLocalDataPath()
{
	try
	{
		static OS_wstring strUserDataPath;
		if (!strUserDataPath.empty())
		{
			return strUserDataPath;
		}

		string strInstallName = g_ConfigFileMgr.GetConfigFromINIFile(APP_INSTALL_MODULE,APP_INSTALL_PATH,SYSCONFIG);
		
                // Add "/" in the end of dir
		if(!strInstallName.empty())
		{
			int iLastIndex = strInstallName.length()-1;
			if('\\' == strInstallName.at(iLastIndex))
			{
				strInstallName.replace(iLastIndex,1,"/");// Convert "\" to "/"
			}
			else if( '/'!=strInstallName.at(iLastIndex))
			{
				strInstallName += "/";//Add "/" in the end of dir
			}
		}
                // Add "/" in the end of dir

#ifdef Q_OS_WIN

		if (strInstallName.empty())
		{
			//do not get InstallName
                        strInstallName = "Open Studio/";
		}

		char pVar[MAX_PATH];
		memset(pVar, 0, sizeof(pVar));
		if(!SUCCEEDED(SHGetFolderPathA(NULL,CSIDL_COMMON_APPDATA,NULL,1,pVar)))
		{
                        return _OSW(L"");
		}

		string strPath = pVar;
		//Convert "//" to "/"
		size_t index = strPath.find("\\",0);
		while (string::npos != index)
		{
			strPath.replace(index,1,"/");
			index = strPath.find("\\",0);
		}	
		strPath += "/";
		strUserDataPath = MyMultiByteToWideChar(strPath.c_str());

#else
                //Change the Mac&Linux UserData Dirs to openstudio_userdata
                strUserDataPath = _OSW(L"/usr/local/openstudio_userdata/");
		
		string strTmpDir = MyWideCharToMultiByte(strUserDataPath.c_str());
		JDir DirTmp(strTmpDir);
		if(!DirTmp.exists())
		{
			DirTmp.mkdir(strTmpDir);
			strTmpDir = strTmpDir.substr(0,strTmpDir.length()-1);
			ChangeFileRight(strTmpDir.c_str());
		}

		//create the folder
		if (strInstallName.empty())
		{
                        strInstallName = "openstudio/";
		}

		size_t index = strInstallName.find(" ",0);
		while (string::npos != index)
		{
			strInstallName.replace(index,1,"_");
			index = strInstallName.find(" ",0);
		}	
#endif

		strUserDataPath = strUserDataPath + MyMultiByteToWideChar(strInstallName.c_str());
		string strDir = MyWideCharToMultiByte(strUserDataPath.c_str());
		JDir DirUserdata(strDir);
		if(!DirUserdata.exists())
		{
			DirUserdata.mkdir(strDir);
			strDir = strDir.substr(0,strDir.length()-1);  //Change The Dir's right
			ChangeFileRight(strDir.c_str());
		}

		return strUserDataPath;

	} 
	catch (...)
	{
                return _OSW(L"");
	}
        return _OSW(L"");
}

#ifdef Q_OS_WIN
// get Service status function BEGIN
int  Common::GetServiceStatus( const wchar_t* strSrvName )
{

	SC_HANDLE   schSCManager;
	SC_HANDLE  schService; 
	SERVICE_STATUS_PROCESS status;
	DWORD sz = 0;
	DWORD res = 0;

	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT); //open Service Manager
	if (schSCManager == NULL) 
	{
		res = GetLastError();
		return SRV_ERROR;
	}

	schService = OpenService(schSCManager, (WCHAR*)strSrvName, SERVICE_QUERY_STATUS); //open Service

	if (schService == NULL) 
	{
		//std::string strTmp;
		//char szMsg[260] = {0};
		//strTmp = MyWideCharToMultiByte( strSrvName.c_str() );
		//res = ::GetLastError();
		//sprintf( szMsg,"OpenService - %s: error = %d", strTmp.c_str(), res );
		//TRACE_INFO(  szMsg );

		CloseServiceHandle(schSCManager);
		if ((res == ERROR_INVALID_NAME)||(res == ERROR_SERVICE_DOES_NOT_EXIST)) return ERROR_NOSERVICE; //Windows WiFi service does not exist at all
		if (res == ERROR_ACCESS_DENIED) return SRV_ERROR;
		return SRV_ERROR;
	}

	res = QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(SERVICE_STATUS_PROCESS), &sz); //query service status
        if (0 == res)
	{
		//res = GetLastError();

		//std::string strTmp;
		//char szMsg[260] = {0};
		//strTmp = MyWideCharToMultiByte( strSrvName.c_str() );
		//res = GetLastError();
		//sprintf( szMsg,"QueryServiceStatusEx - %s: error = %d", strTmp.c_str(), res );

		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager); 
		if (res == ERROR_ACCESS_DENIED)
		{
			return SRV_ERROR; //根据评审结论，返回值用枚举进行处理
		}		
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	res = status.dwCurrentState; //some other state (see QueryServiceStatusEx description in MSDN)
	if (status.dwCurrentState == SERVICE_RUNNING) 
	{
		res = SRV_RUNNING;
	}
	if (status.dwCurrentState == SERVICE_STOPPED) 
	{
		res = SRV_STOPPED;
	}
	return res; 
}
#else
int  Common::GetServiceStatus( const wchar_t* strSrvName )
{
    (void)strSrvName;
	return SRV_ERROR;
}
#endif


void Common::ChangeFileRight(const char* pszFileName)
{
#ifdef Q_OS_UNIX
//Change the file's right to 777
	if(NULL == pszFileName)
		return;
	string strFileName = pszFileName;
	string strCmd = "chmod 777 \""+strFileName+"\"";
        (void)system(strCmd.c_str());
#endif
}







