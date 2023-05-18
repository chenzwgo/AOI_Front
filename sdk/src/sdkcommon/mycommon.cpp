#include "mycommon.h"
#include "common.h"
#include "jdir.h"

OS_wstring ToOs_wchar(const wchar_t* pswzChar)
    {
        OS_wstring wstrRet;
        if(NULL == pswzChar)
        {
            return wstrRet;
        }
        wstring wstr = pswzChar;
        if(0 == wstr.length())
        {
            return wstrRet;
        }
        if (sizeof(wchar_t) == sizeof(OS_wchar_t))
        {
            //on windows the wstring is same as the OS_wstring
            wstrRet = (OS_wchar_t*)wstr.c_str();
        }
        else
        {
            // only support wchar_t as 4 Bytes on Unix(Linux&Mac)

            size_t size = wstr.length();
            ushort *uc = new ushort[2*size+1];
            if (NULL == uc)
            {
                return OS_wstring();
            }
            memset(uc,0,(2*size+1)*sizeof(ushort));
            uint *wc = (uint*)wstr.c_str();
            ushort *p = uc;
            uint c = 0;
            for(size_t i = 0; i< size ;i++)
            {
                c = (*wc);
                if (c > 0xffff)
                {
                    // decompose into a surrogate pair
                    *p++ = (c>>10) + 0xd7c0;
                    c = c%0x400 + 0xdc00;
                }
                *p++ = c;
                wc++;
            }
            //uc[size] = 0;
            wstrRet = uc;
            delete[] uc;
        }
        return wstrRet;
    }
    
bool SetSetting(const OS_wstring pszModuleName, const OS_wstring pszItemName,const OS_wstring pszValue)
{
        return Common::SetSetting(pszModuleName.c_str(),pszItemName.c_str(),pszValue.c_str());
};

OS_wstring MyGetSetting(const OS_wstring pszModuleName, const OS_wstring pszItemName)
{
	OS_wstring wstr;
        int iSize = Common::GetSetting(pszModuleName.c_str(),pszItemName.c_str(),NULL,0);
	OS_wchar_t* pValue =new OS_wchar_t[iSize+1];
	memset(pValue,0,(iSize+1)*sizeof(OS_wchar_t));
        Common::GetSetting(pszModuleName.c_str(),pszItemName.c_str(),pValue,iSize);
	wstr = pValue;
	delete pValue;
	return wstr;
}


OS_wstring MyMultiByteToWideChar(const char* pszMultiByte, int iCodePage )
{
	OS_wstring wstrRet;
        int iSize = Common::MultiByteToWideChar(pszMultiByte,NULL,0,iCodePage);
	
	OS_wchar_t* pValue = new OS_wchar_t[iSize+1];
	memset(pValue,0,(iSize+1)*sizeof(OS_wchar_t));
        Common::MultiByteToWideChar(pszMultiByte,pValue,iSize,iCodePage);

	wstrRet = pValue;
	delete pValue;
	pValue = NULL;
	return wstrRet;

}
string MyWideCharToMultiByte(const OS_wchar_t* pwszWideChar, int iCodePage)
{
		string strRet;
                int iSize = Common::WideCharToMultiByte(pwszWideChar,NULL,0,iCodePage);
		char* pValue = new char[iSize+1];
		memset(pValue,0,(iSize+1)*sizeof(char));
                Common::WideCharToMultiByte(pwszWideChar,pValue,iSize,iCodePage);
		strRet = pValue;
		delete pValue;
		pValue = NULL;
		return strRet;
}
string TakeOutDoubleQuotationMarks(const string& strSrc)
{
	string strRet;
	int iLen = (int)strSrc.length();

	if (strSrc[0] == '\"' && strSrc[iLen - 1] == '\"')
	{
		strRet.resize(iLen - 2);

		int i = 0;
		for (i = 0; i < iLen - 2; i++)
		{
			strRet[i] = strSrc[i+1];
		}
	}
	else
	{
		strRet = strSrc;
	}

	return strRet;

}
string AddDoubleQuotationMarks(const char *pszSrc)
{
	string strRet = "\"";
	strRet += pszSrc;
	strRet +=  "\"";
	return strRet;
}

bool ToLowerString(string& strConvert)
{
	try
	{
		string::iterator it ;
		for (it=strConvert.begin(); it!=strConvert.end(); ++it)
		{
			if (isascii(*it) && isupper(*it))
			{
				*it = tolower(*it) ;
			}
		}
		return true ;
	}
	catch (...)
	{
		return false ;
	}
        return false;
}
bool ToUpperString(string& strConvert)
{
	try
	{
		string::iterator it ;
		for (it=strConvert.begin(); it!=strConvert.end(); ++it)
		{
			if (isascii(*it) && islower(*it))
			{
				*it = toupper(*it) ;
			}
		}
		return true ;
	}
	catch (...)
	{
		return false ;
	}
        return false;
}

OS_wstring MyGetUserDataPath()
{
    OS_wchar_t pswzDir[MAX_PATH];
    memset(pswzDir, 0, sizeof(pswzDir));
    Common::GetUserDataDirectory(pswzDir,MAX_PATH);
    OS_wstring wstr =  pswzDir;
    return wstr;
}
bool  OS_wcscmp(const OS_wstring s1,const OS_wstring s2)
{

    size_t nS1Len = s1.length();
    size_t nS2Len = s2.length();

    if(nS1Len != nS2Len)
    {
        return false;
    }

    for(size_t i = 0; i< nS1Len ;i++)
    {
        if(s1[i] != s2[i])
        {
            return false;
        }
    }
    return true;
}


bool  OS_wcsncmp(const OS_wstring s1,const OS_wstring s2, size_t n)
{
    size_t nS1Len = s1.length();
    size_t nS2Len = s2.length();

    if((n > nS1Len)&&(n > nS2Len))
    {
        return OS_wcscmp(s1,s2);
    }

    size_t nMinLen = (nS1Len > nS2Len)?nS2Len:nS1Len;
    if(n > nMinLen)
    {
        return false;
    }
    nMinLen = n;
    for(size_t i = 0; i < nMinLen ;i++)
    {
        if(s1[i] != s2[i])
        {
            return false;
        }
    }
    return true;
}
