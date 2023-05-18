#ifndef MY_COMMON_H
#define MY_COMMON_H

#include <string>
#include <algorithm>
using namespace std;
#include "mybasetype.h"


#define _OSW(x) ToOs_wchar(x)

OS_wstring ToOs_wchar(const wchar_t* pswzChar);

bool SetSetting(const OS_wstring pszModuleName, const OS_wstring pszItemName,const OS_wstring pszValue);

OS_wstring MyGetSetting(const OS_wstring pszModuleName, const OS_wstring pszItemName);

OS_wstring MyMultiByteToWideChar(const char* pszMultiByte, int iCodePage = CP_ACP);

string MyWideCharToMultiByte(const OS_wchar_t* pwszWideChar, int iCodePage = CP_ACP);

string TakeOutDoubleQuotationMarks(const string& strSrc);

string AddDoubleQuotationMarks(const char *pszSrc);

bool ToLowerString(string& strConvert);
bool ToUpperString(string& strConvert);

OS_wstring MyGetUserDataPath();
bool  OS_wcscmp(const OS_wstring s1,const OS_wstring s2);
bool  OS_wcsncmp(const OS_wstring s1,const OS_wstring s2, size_t n);

#endif // MY_COMMON_H
