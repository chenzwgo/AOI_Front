/**
 *	
 */

#ifndef __BASE_DEF_H__
#define __BASE_DEF_H__

#include <memory.h>


typedef unsigned int    UINT;

typedef wchar_t WCHAR;

#ifndef uint64
#ifdef Q_OS_WIN32
#define uint64 unsigned __int64
#else
typedef unsigned long long   uint64;
#endif
#endif

#ifndef MY_WCHAR_H
#define MY_WCHAR_H
#include <string>
using std::basic_string;

#ifndef my_wchar_t
typedef unsigned short my_wchar_t;
#endif

#ifndef my_wstring
typedef basic_string<my_wchar_t> my_wstring;
#endif
#endif



#ifndef OS_WCHAR_H
#define OS_WCHAR_H
#include <string>
using std::basic_string;

#ifndef OS_wchar_t
typedef unsigned short OS_wchar_t;
#endif

#ifndef OS_wstring
typedef basic_string<OS_wchar_t> OS_wstring;
#endif
#endif


typedef unsigned short      ushort;
typedef unsigned int        uint;
typedef unsigned long       ulong;
typedef unsigned char       byte;
typedef ulong               DWORD;
typedef unsigned short      WORD;
typedef unsigned char		BYTE;
typedef void*               HANDLE;

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

typedef int                 BOOL;

#define CP_ACP                    0           // default to ANSI code page
#define CP_OEMCP                  1           // default to OEM  code page
#define CP_MACCP                  2           // default to MAC  code page
#define CP_THREAD_ACP             3           // current thread's ANSI code page
#define CP_SYMBOL                 42          // SYMBOL translations

#define CP_UTF7                   65000       // UTF-7 translation
#define CP_UTF8                   65001       // UTF-8 translation

#ifdef WIN32
#define MTSSleep     ::Sleep
#endif

#ifndef NULL
#define NULL 0
#endif

#define SAFE_DELETE(a)              \
do                                  \
{                                   \
    if (NULL != (a))                  \
    {                               \
        delete (a);                   \
        (a) = NULL;                   \
    }                               \
}while(0)

#define SAFE_DELETE_ARR(a)          \
do                                  \
{                                   \
    if (NULL != (a))                  \
    {                               \
        delete [](a);                 \
        (a) = NULL;                   \
    }                               \
}while(0)


#endif //__BASE_DEF_H__
