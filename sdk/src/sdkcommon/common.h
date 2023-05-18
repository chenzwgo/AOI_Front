
#ifndef COMMON_H
#define COMMON_H
#include "dllexport.h"

#include <string>
#include <algorithm>
using namespace std;

#include "isdkcontainer.h"
#include "mystring.h"


#define RETURN_ERROR(ErrCode)       \
{                                   \
                                    dwStatus = ErrCode;             \
                                               return false;                   \
                                           }                                   \

#ifdef NO_ERROR
#undef NO_ERROR
#define NO_ERROR        0
#endif

#define DATA_PRESENT    1
#define INVALID_VALUE   DWORD(-1)


enum OS_TYPE
{
    OSVERSION_MS_NT,     //("Microsoft Windows NT")
    OSVERSION_MS_2000,   //("Microsoft Windows 2000")
    OSVERSION_MS_XP,     //("Microsoft Windows XP")
    OSVERSION_MS_95,     //("Microsoft Windows 95")
    OSVERSION_MS_98,     //("Microsoft Windows 98")
    OSVERSION_MS_ME,     //("Microsoft Windows Me")
    OSVERSION_MS_32,     //("Microsoft Win32s")
    OS_VERSION_UNKOWN
};


enum SRV_STATUS
{
	ERROR_NOSERVICE,
	SRV_ERROR,
	SRV_RUNNING,
	SRV_STOPPED,
};


class RT_API Common {

private:
    Common();
private:
    static ISDKContainer *m_pPluginContainer;
    // setPluginContainer函数仅供PluginContainer调用，其他插件禁止调用
    static void setPluginContainer(ISDKContainer *m_pPluginContainer);
    friend class CContainerFactory;
public:

    static ISDKContainer *getPluginContainer();



    template<typename T>
    static T GetPluginInterface(const char* pszPluginCLSID, const int iInterfaceId)
    {
        try
        {
            if (NULL == pszPluginCLSID || NULL == m_pPluginContainer)
            {
                return NULL;
            }

            IBaseInterface* pIBaseInterface = NULL;

            m_pPluginContainer->QueryInterface(pszPluginCLSID, iInterfaceId, &pIBaseInterface);

            return (T)(pIBaseInterface);
        }
        catch (...)
        {
            return NULL;
        }
    }

    static void ReleaseConfigFileMgr();

    /**
   *	读取程序运行目录(绝对路径)
   *@param
   *	无
   *@return OS_wstring
   *	返回程序运行目录,如"c:\Program Files\Mobile Manager\"
   *    随着系统的不同其路径分隔符也不相同
   */
    static int GetModuleDirectory(OS_wchar_t* pswzDir, int iLen);

    /**
    *	读取用户数据区的目录(绝对路径)
    *@param
    *	无
    *@return OS_wstring
    *	返回插件配置目录,如"c:\Program Files\Open Studio\UserData"
    *   随着系统的不同其路径分隔符也不相同
    */


    static int GetUserDataDirectory(OS_wchar_t* pswzDir, int iLen);


    /**
    *	读取用Log日志文件夹的目录(绝对路径)
    *@param
    *	无
    *@return OS_wstring
    *   返回插件配置目录,如"c:\Program Files\Open Studio\UserData\Log"
    *   随着系统的不同其路径分隔符也不相同
    */
    static int GetLogDirectory(OS_wchar_t* pswzDir, int iLen);

    /**
        *	获得配置项的值
        *@param pszModuleName
        *	[in] 模块名字
        *@param pszItemName
        *	[in] 配置项的名字
        *@return OS_wstring
        *	配置项的值,这里默认会读取用户配置文件中的数据，如果用户配置中没有该项，则读取系统配置项
    */
    static int GetSetting(const OS_wchar_t* pszModuleName, const OS_wchar_t* pszItemName,OS_wchar_t* pswzValue,int iLen);


    /**
        *	设置配置项的值
        *@param pszModuleName
        *	[in] 模块名字
        *@param pszItemName
        *	[in] 配置项的名字
        *@param pszValue
        *	[in] 配置项的值
        *@return bool
        *	成功返回true 失败返回false
        *       只能够对UserSetting发起写操作，SysSetting不允许修改
    */
    static bool SetSetting(const OS_wchar_t* pszModuleName, const OS_wchar_t* pszItemName, const OS_wchar_t* pszValue);



    /**
        *	删除某个配置项(只支持USerSetting)
        *@param pszModuleName
        *	[in] 模块名字
        *@param pszItemName
        *	[in] 配置项的名字
        *@param pszValue
        *	[in] 配置项的值
        *@return bool
        *	成功返回true 失败返回false
        *       只能够对UserSetting发起写操作，SysSetting不允许修改
    */
    static bool DelSetting(const OS_wchar_t* pszModuleName, const OS_wchar_t* pszItemName);



    /**
        *	多字节字符串转换为宽字符
        *@param pszMultiByte
        *	[in] 多字节字符串
        *@param iCodePage
        *	[in] 代码页（目前只支持CP_ACP和CP_UTF8两项）
        *@return OS_wstring
        *	宽字符字符串
    */
    static int MultiByteToWideChar(const char* pszMultiByte, OS_wchar_t* pswzValue ,int iLen, int iCodePage = CP_ACP);

    /**
        *	宽字符字符串转换为多字节
        *@param pwszWideChar
        *	[in] 宽字符字符串
        *@param iCodePage
        *	[in] 代码页，目前只支持CP_ACP和CP_UTF8两项
        *@return sring
        *	多字节字符串
    */

    static int WideCharToMultiByte(const OS_wchar_t* pwszWideChar,char* pszValue, int iLen, int iCodePage = CP_ACP);


    // from chrcpy.h
    static char * chrcpy(char * dst, unsigned long dstlen, const char * src, unsigned long ulSrcLen = 0);
    static OS_wchar_t* wchrcpy(OS_wchar_t* dst, unsigned long dstlen, const OS_wchar_t* src, unsigned long ulSrcLen = 0);
    static int GetDataLength(unsigned char* puszSrc, int nSrclen, unsigned char ucTag);


    static bool SetAppPathName(const OS_wchar_t* pswzAppPathName);

    
    static OS_TYPE GetOSType();

    //获取CSV文件中的分隔符，区分是;还是,
    static char GetCSVSeperator();

    static int GetWindowsVersion();


    /**
         *	获取windows系统服务的状态
         *@param strConvert
         *	[OS_wstring&] 服务的名字
         *@return int
         *	服务的当前状态
         */
	static int GetServiceStatus( const wchar_t* strSrvName ); 


	static void ChangeFileRight(const char* pszFileName);

    /**
    *	获得配置项的值
    *@param pszModuleName
    *	[in] 模块名字
    *@param pszItemName
    *	[in] 配置项的名字
    *@param pszValue
	*	配置项的值,这里默认会读取用户配置文件中的数据，如果用户配置中没有该项，则读取系统配置项
	*@param pLen
	*	传入pszValue所指向空间的大小,函数返回时pLen将被设成配置项字符串的长度,包含结束符'\0'。
	*@return bool
	*	成功返回true, 失败返回false.如果传入的pszValue空间不足,可通过pLen参数获取实际所需的空间大小.
    */
    static bool GetIniSetting(const OS_wchar_t * pszModuleName, const OS_wchar_t * pszItemName, OS_wchar_t * pszValue, int * pLen);


    /**
    *	设置配置项的值
    *@param pszModuleName
    *	[in] 模块名字
    *@param pszItemName
    *	[in] 配置项的名字
    *@param pszValue
    *	[in] 配置项的值
    *@return bool
    *	成功返回true 失败返回false
    *       只能够对UserSetting发起写操作，SysSetting不允许修改
    */
    static bool SetIniSetting(const OS_wchar_t * pszModuleName, const OS_wchar_t * pszItemName, const OS_wchar_t * pszValue );

private:
    static OS_wstring GetSetingsFromXml(const char* pszModule, const char* pszItem,const char* pszFilePath);

    static OS_wstring m_wstrAppPathName;

    static OS_wstring GetLocalDataPath();

};

#endif // COMMON_H
