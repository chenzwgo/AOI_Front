#ifndef MYSTRING_H
#define MYSTRING_H

#include "mybasetype.h"
#include "dllexport.h"

#ifdef Q_OS_WIN
#else
#include <iconv.h>
#endif

#include <string>
using std::wstring;
using std::string;


#ifdef OS_WCHAR_H
size_t RT_API OS_wcslen(const OS_wchar_t* pswString);
bool RT_API OS_wcsncpy(OS_wchar_t* dest,const OS_wchar_t* src, size_t n);
bool RT_API OS_wcscpy(OS_wchar_t* dest,const OS_wchar_t* src);

int    RT_API OS_wtoi(const OS_wchar_t *string);
long  RT_API OS_itow(int value,OS_wchar_t *string, unsigned radix );
long RT_API OS_wtol(const OS_wchar_t *string);

typedef unsigned short ushort;
typedef unsigned int uint;
OS_wstring  wstring2OS(wstring wstr);

void OS2wstring(OS_wstring wstrSrc, wstring& wstrRet);
wstring  OS2wstring(OS_wstring wstrSrc);
inline size_t g_f_wctou8(char * dest_str, const OS_wchar_t src_wchar);
inline size_t g_f_u8towc(OS_wchar_t &dest_wchar, const unsigned char * src_str);
inline size_t wstr_to_utfstr(const OS_wchar_t* wstr,char* utfstr);
inline size_t utfstr_to_wstr(const char* utfstr,OS_wchar_t* wstr);
string  OSwstringToUTF8(OS_wstring wstr);
void OSwstringToUTF8(OS_wstring wstr,string& strRet);
OS_wstring  UTF8ToOSwstring(string strUTF8);

#endif

#ifdef Q_OS_WIN
#else
    int code_convert(const char *from_charset,const char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen);
#endif

#ifndef Q_OS_WIN

#define __int64 long long
#define _gcvt gcvt
#define _atoi64 atoll

void  RT_API xtoa (unsigned long val, char *buf, unsigned radix, int is_neg);
void RT_API x64toa (uint64 val,char *buf,unsigned radix,int is_neg);
char * RT_API  itoa( int iValue, char* buf, int iRoot );
char*  RT_API _ui64toa(uint64 numVal, char* szBuf, int iRoot );

#endif

#ifdef  _MSC_VER
int RT_API strcasecmp (const char* szSrc, const char *szDes );
#define snprintf _snprintf
#endif   //MSVC_COMPILE






#endif // MYSTRING_H
