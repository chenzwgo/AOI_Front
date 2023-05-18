#ifndef	RUTIME_DEFINE_H
#define RUTIME_DEFINE_H

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

//计算数组个数
#ifndef ARR_SIZE
	#define ARR_SIZE(a)  (sizeof((a))/sizeof((a[0])))
#endif

//此处判断空值，0值得宏
#ifndef RETURN_V_IF
	#define RETURN_V_IF(p, v) if (p) \
		{	\
		return v; \
		}
#endif

//此处判断空值，0值返回
#ifndef RETURN_IF
	#define RETURN_IF(p) if (p) \
		{	\
		return; \
		}
#endif

#ifndef BREAK_IF
	#define BREAK_IF(p) if (p) \
		{	\
		break; \
		}
#endif

#ifndef CONTINUE_IF
#define CONTINUE_IF(p) if (p) \
{	\
	continue; \
}
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


#ifndef MAX
	#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
	#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define CHK_DEFUALT_VAL(x, y) if (0 == (x)) \
{ \
	(x) = (y); \
}

#ifndef _MSC_VER || WIN32 || _WIN64 || __WIN32__ || WINVER
#define _snprintf snprintf
//#define sscanf  sscanf_s
#endif

#endif // RUTIME_DEFINE_H