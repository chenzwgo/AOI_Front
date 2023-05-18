
// defines defines OPENSTUDIO SDK structures and macros.


#ifndef    _OS_SDK_DEFINE_H_
#define    _OS_SDK_DEFINE_H_

#ifndef _MSC_VER
#ifndef Q_OS_WIN32
        #define OSSDK_CALLTYPE
#else   // _WINDOWS
        #ifndef _WINSOCK2API_
        #define _WINSOCK2API_
        #define _WINSOCKAPI_
        #endif
        #ifndef _CDECL_EXPORTS_
                #define OSSDK_CALLTYPE    	__declspec(dllexport)
        #else
                #define OSSDK_CALLTYPE        __cdecl
        #endif  //WMC_HW_EXPORTS
        #define WMC_CDECL			__cdecl
#endif //_WINDOWS
#else
        #ifndef _WINSOCK2API_
        //#define _WINSOCK2API_   // ��<winsock2.h>��ͻ��������������ע��
        //#define _WINSOCKAPI_
        #endif
        #ifndef _CDECL_EXPORTS_
                #define OSSDK_CALLTYPE    	__declspec(dllexport)
        #else
                #define OSSDK_CALLTYPE        __cdecl
        #endif  //WMC_HW_EXPORTS
        #define WMC_CDECL			__cdecl
#endif


typedef signed char					OS_INT8;				/*!< [CH]8 bit signed <BR>[EN]8 bit signed*/
typedef unsigned char				OS_UINT8;				/*!< [CH]8 bit unsigned <BR>[EN]8 bit unsigned*/
typedef short						OS_INT16;             /*!< [CH]16 bit signed <BR>[EN]16 bit signed */
typedef unsigned short				OS_UINT16;				/*!< [CH]16 bit unsigned<BR>[EN]16 bit unsigned */
typedef int							OS_INT32;                /*!< [CH]32 bit signed <BR>[EN]32 bit signed*/
typedef unsigned int				OS_UINT32;			/*!<[CH]32 bit unsigned <BR>[EN]32 bit unsigned */

typedef long long					OS_INT64;				 /*!< [CH]64 bit signed <BR>[EN]64 bit signed*/
typedef unsigned long long			OS_UINT64;			/*!< [CH]64 bit unsigned <BR>[EN]64 bit unsigned*/

typedef float						OS_FLOAT;			/*!<[CH]float <BR>[EN]float*/
typedef double						OS_DOUBLE;		/*!< [CH]double <BR>[EN]double*/
typedef long						OS_LONG;			/*!< [CH]long <BR>[EN]long*/
typedef unsigned long				OS_ULONG;			/*!< [CH]unsigned long <BR>[EN]unsigned long*/

typedef long						OS_TIME;				/*!< [CH]time <BR>[EN]time*/
typedef long long					OS_TIME64;				/*!<[CH]64bits time <BR>[EN]64bits time*/
typedef unsigned char				OS_BYTE;       		/*!< [CH]8 bit unsigned <BR>[EN]8 bit unsigned*/
typedef unsigned char				OS_CHAR;			/*!< [CH]8 bit signed <BR>[EN]8 bit signed*/
typedef unsigned short				OS_WCHAR;			/*!< [CH]16 bit signed <BR>[EN]16 bit signed*/
typedef unsigned char				OS_BOOL;			/*!< [CH]8 bit unsigned as bool <BR>[EN]8 bit unsigned as bool */
typedef void*						OS_HANDLE	;		/*!<[CH]void* <BR>[EN]void* type*/

typedef long						OS_ERROR;              /*!<[CH]return value<BR>[EN]return value*/

typedef void*				OS_LPARAM;
typedef void*				OS_WPARAM;

#ifndef DWORD
typedef unsigned long       DWORD;
#endif

typedef long OS_ERROR;

#ifndef NULL
#define NULL 0
#endif

/**
*@note
*		[CH]ͳһ�Ļص�����ԭ��
*	<BR>[EN]uniform prototype of callback functions
*@param	[IN]enNotifyType
*	<BR>[CH]OS_INT32 �ص����¼�����
*	<BR>[EN]callback event type
*@param	[IN]pNotifyData
*	<BR>[CH]OS_HANDLE �ص�ʱ�ϱ�������
*	<BR>[EN]report data struct
*@return OS_BOOL
*	<BR>[CH]�����ķ���ֵ���ɹ�Ϊtrue��ʧ��Ϊfalse;
*	<BR>[EN]return of the fucntion ,true for success, false for failure
*@attention
*		[CH]�ص��¼��������Լ���ص��������Ͷ��壬��μ�����ģ����ڻص��¼����͵Ķ���
*	<BR>[EN]The definition of the callback event and the report data type ,please refer to each module which has it's own callback function
*@see
*	
*/
typedef OS_BOOL (*pfnOnUniCommEvent)(OS_INT32 enNotifyType, OS_HANDLE pNotifyData);


typedef OS_BOOL (*pfnDataRev)(OS_HANDLE pData,OS_INT32 iDataLen);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//****************************************************** structs and macros for statistics Begin  *************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*@note
*		[CH] �����Ĵ����붨��
*	<BR>[EN] Common error code definition
*/

typedef enum _OS_EN_COMMON_ERROR
{
	/*-999~999 Common Errors Begin*/
        OS_ERROR_FAILURE						= -1,               /*!<[CH]ֻ��ʾ����ʧ�ܣ����ܱ��������ʧ������ <BR>[EN]Failure */

        OS_ERROR_SUCCESS						= 0,				/*!< [CH]��ʾ�����ɹ� <BR>[EN]Success */
        OS_ERROR_BAD_PARAMETER				= 1,				/*!< [CH]��Ч���� <BR>[EN]invalidate parameter */
        OS_ERROR_BUFFER_TOO_SMALL				= 2,				/*!< [CH]�������ռ䲻�� <BR>[EN]buffer size is not enough*/
        OS_ERROR_INVALID_ID					= 3,				/*!< [CH]����ֵ�Ƿ� <BR>[EN]Index is invalid*/
        OS_ERROR_SDK_NOT_STARTED				= 4,

		// IO����������
		OS_ERROR_WRITE_IO_FAILED,
		OS_ERROR_READ_IO_FAILED,

} OS_EN_COMMON_ERROR;


enum EM_ERROR_OPER_RESULT
{
	EM_SUCCESS,
	EM_EXIST_STATION,
	EM_NOT_EXIST_STATION,

	EM_EXIST_DEVICE,
	EM_NOT_EXIST_DEVICE,

	EM_ERROR_SAVE_FILE_FAILED,
	EM_UNKNOWN_ERROR
};


#endif     //_OS_DEFINE_H_
