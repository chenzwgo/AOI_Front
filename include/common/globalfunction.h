
#ifndef GLOBALFUNCTION_H
#define GLOBALFUNCTION_H

#include <string>
#include <algorithm>
using namespace std;

#include "basefuncdef.h"


#define RETURN_ERROR(ErrCode)       \
{                                   \
    dwStatus = ErrCode;             \
    return false;                   \
}                                   \

#ifdef NO_ERROR
#undef NO_ERROR
#define NO_ERROR        0
#endif

#ifndef MAX_CONFIG_SIZE
#define MAX_CONFIG_SIZE 256
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

//from GlobalFunction
class  CGlobalFunction {

private:
    CGlobalFunction();

public:

    static void ReleaseConfigFileMgr();

    /**
    *	读取程序运行目录(绝对路径)
    *@param
    *	无
    *@return my_wstring
    *	返回程序运行目录
    *    随着系统的不同其路径分隔符也不相同
    */


    static int GetModuleDirectory(my_wchar_t* pswzDir, int iLen);

    /**
    *	读取插件配置目录(绝对路径)
    *@param pwszModuleName
    *	[in] 模块名，为空则取上级目录
    *@return my_wstring
    *	返回插件配置目录
    *   随着系统的不同其路径分隔符也不相同
    */


    /**
    *	读取用户数据区的目录(绝对路径)
    *@param
    *	无
    *@return my_wstring
    *	返回插件配置目录
    *   随着系统的不同其路径分隔符也不相同
    */
    // static my_wstring GetUserDataDirectory();

    //static int GetUserDataDirectory(my_wchar_t* pswzDir, int iLen);


    /**
    *	读取用Log日志文件夹的目录(绝对路径)
    *@param
    *	无
    *@return my_wstring
    *	返回插件配置目录
    *   随着系统的不同其路径分隔符也不相同
    */
    //static my_wstring GetLogDirectory();

    static int GetLogDirectory(my_wchar_t* pswzDir, int iLen);

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
    static bool DelSetting(const my_wchar_t* pszModuleName, const my_wchar_t* pszItemName);


    static my_wstring MyMultiByteToWideChar(const char* pszMultiByte, int iCodePage = CP_ACP);
    /**
    *	多字节字符串转换为宽字符
    *@param pszMultiByte
    *	[in] 多字节字符串
    *@param iCodePage
    *	[in] 代码页（目前只支持CP_ACP和CP_UTF8两项）
    *@return my_wstring
    *	宽字符字符串
    */
    static int MultiByteToWideChar(const char* pszMultiByte, my_wchar_t* pswzValue ,int iLen, int iCodePage = CP_ACP);


    static string MyWideCharToMultiByte(const my_wchar_t* pwszWideChar, int iCodePage = CP_ACP);
    /**
    *	宽字符字符串转换为多字节
    *@param pwszWideChar
    *	[in] 宽字符字符串
    *@param iCodePage
    *	[in] 代码页，目前只支持CP_ACP和CP_UTF8两项
    *@return sring
    *	多字节字符串
    */
    static int WideCharToMultiByte(const my_wchar_t* pwszWideChar,char* pszValue, int iLen, int iCodePage = CP_ACP);


    static unsigned long long atox( const signed char* s,short string_type);


    static char * chrcpy(char * dst, unsigned long dstlen, const char * src, unsigned long ulSrcLen = 0);

    static my_wchar_t* wchrcpy(my_wchar_t* dst, unsigned long dstlen, const my_wchar_t* src, unsigned long ulSrcLen = 0);


    static int GetDataLength(unsigned char* puszSrc, int nSrclen, unsigned char ucTag);


    /**
    *	把字符串中的大写字母转化为小写字母,只对ASCII字符有效
    *@param strConvert
    *	[in] 要转化的字符串
    *@return string
    *	小写字符串
    */


    static bool SetAppPathName(const my_wchar_t* pswzAppPathName);

    static OS_TYPE GetOSType();

    //获取CSV文件中的分隔符，区分是;还是,

    static char GetCSVSeperator();


    static int GetWindowsVersion();

    /**
    *	获取windows系统服务的状态
    *@param strConvert
    *	[my_wstring&] 服务的名字
    *@return int
    *	服务的当前状态
    */
    //static int GetServiceStatus( const wchar_t* strSrvName );

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
    static bool GetIniSetting(const my_wchar_t * pszModuleName, const my_wchar_t * pszItemName, my_wchar_t * pszValue, int * pLen);


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
    static bool SetIniSetting(const my_wchar_t * pszModuleName, const my_wchar_t * pszItemName, const my_wchar_t * pszValue );

    /*检查文件是否存在*/
    static bool isExistFold(string strFoldPath);

    /**取出双引号中的字符 -- "" **/
    static string TakeOutDoubleQuotationMarks(const string& strSrc);

    /**添加双引号 -- "" **/
    static string AddDoubleQuotationMarks(const char *pszSrc);

    static bool ToLowerString(string& strConvert);
    static bool ToUpperString(string& strConvert);

    static bool  my_wcscmp(const my_wstring s1,const my_wstring s2);
    static bool  my_wcsncmp(const my_wstring s1,const my_wstring s2, size_t n);

    static my_wstring MyGetSetting(const my_wstring pszModuleName, const my_wstring pszItemName);

    // 获取应用程序运行目录
    static string GetRootDir();
    /*md5 处理*/
    /* 将给入字符进行md5加密
     * input -- 传入要加密字符
     * inputlen -- 传入buffer长度
     * digest  -- 加密后内容
     * return N/A
    */
    static void MD5(unsigned char * input, unsigned int inputlen, char digest[32]);
    /* 将给入路径的文件内容进行md5加密
     * filename -- 传入要加密字符
     * digest  -- 加密后内容
     * return N/A
    */
    static void MD5File(const char * filename, char digest[32]);
private:
    static my_wstring GetSetingsFromXml(const char* pszModule, const char* pszItem,const char* pszFilePath);

    static my_wstring m_wstrAppPathName;
    //static my_wstring GetLocalDataPath();

};

#endif // GLOBALFUNCTION_H
