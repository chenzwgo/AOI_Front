#ifndef BASEFUNCDEF_H
#define BASEFUNCDEF_H
#include "basedef.h"
#include <algorithm>
#include <string>
using namespace std;

#define _MYW(x) ToMy_wchar(x)

my_wstring ToMy_wchar(const wchar_t* pswzChar);

#ifdef MY_WCHAR_H
size_t  my_wcslen(const my_wchar_t* pswString);
bool  my_wcsncpy(my_wchar_t* dest,const my_wchar_t* src, size_t n);
bool  my_wcscpy(my_wchar_t* dest,const my_wchar_t* src);

int   my_wtoi(const my_wchar_t *string);
long  my_itow(int value,my_wchar_t *string, unsigned radix );
long  my_wtol(const my_wchar_t *string);

typedef unsigned short ushort;
typedef unsigned int uint;
my_wstring  wstring2my(wstring wstr);

void my2wstring(my_wstring wstrSrc, wstring& wstrRet);
wstring  my2wstring(my_wstring wstrSrc);
inline size_t g_f_wctou8(char * dest_str, const my_wchar_t src_wchar);
inline size_t g_f_u8towc(my_wchar_t &dest_wchar, const unsigned char * src_str);
inline size_t wstr_to_utfstr(const my_wchar_t* wstr,char* utfstr);
inline size_t utfstr_to_wstr(const char* utfstr,my_wchar_t* wstr);
string  MywstringToUTF8(my_wstring wstr);
void MywstringToUTF8(my_wstring wstr,string& strRet);
my_wstring  UTF8ToMywstring(string strUTF8);

#endif
#ifndef Q_OS_WIN

#define __int64 long long
#define _gcvt gcvt
#define _atoi64 atoll
void  xtoa (unsigned long val, char *buf, unsigned radix, int is_neg);


void  x64toa (uint64 val,char *buf,unsigned radix,int is_neg);


char* itoa( int iValue, char* buf, int iRoot );


char* _ui64toa(uint64 numVal, char* szBuf, int iRoot );
#endif

#ifdef  _MSC_VER
int strcasecmp (const char* szSrc, const char *szDes );
#define snprintf _snprintf
#endif   //MSVC_COMPILE

#endif // BASEFUNCDEF_H
