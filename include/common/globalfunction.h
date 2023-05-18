
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
    *	��ȡ��������Ŀ¼(����·��)
    *@param
    *	��
    *@return my_wstring
    *	���س�������Ŀ¼
    *    ����ϵͳ�Ĳ�ͬ��·���ָ���Ҳ����ͬ
    */


    static int GetModuleDirectory(my_wchar_t* pswzDir, int iLen);

    /**
    *	��ȡ�������Ŀ¼(����·��)
    *@param pwszModuleName
    *	[in] ģ������Ϊ����ȡ�ϼ�Ŀ¼
    *@return my_wstring
    *	���ز������Ŀ¼
    *   ����ϵͳ�Ĳ�ͬ��·���ָ���Ҳ����ͬ
    */


    /**
    *	��ȡ�û���������Ŀ¼(����·��)
    *@param
    *	��
    *@return my_wstring
    *	���ز������Ŀ¼
    *   ����ϵͳ�Ĳ�ͬ��·���ָ���Ҳ����ͬ
    */
    // static my_wstring GetUserDataDirectory();

    //static int GetUserDataDirectory(my_wchar_t* pswzDir, int iLen);


    /**
    *	��ȡ��Log��־�ļ��е�Ŀ¼(����·��)
    *@param
    *	��
    *@return my_wstring
    *	���ز������Ŀ¼
    *   ����ϵͳ�Ĳ�ͬ��·���ָ���Ҳ����ͬ
    */
    //static my_wstring GetLogDirectory();

    static int GetLogDirectory(my_wchar_t* pswzDir, int iLen);

    /**
    *	ɾ��ĳ��������(ֻ֧��USerSetting)
    *@param pszModuleName
    *	[in] ģ������
    *@param pszItemName
    *	[in] �����������
    *@param pszValue
    *	[in] �������ֵ
    *@return bool
    *	�ɹ�����true ʧ�ܷ���false
    *       ֻ�ܹ���UserSetting����д������SysSetting�������޸�
    */
    static bool DelSetting(const my_wchar_t* pszModuleName, const my_wchar_t* pszItemName);


    static my_wstring MyMultiByteToWideChar(const char* pszMultiByte, int iCodePage = CP_ACP);
    /**
    *	���ֽ��ַ���ת��Ϊ���ַ�
    *@param pszMultiByte
    *	[in] ���ֽ��ַ���
    *@param iCodePage
    *	[in] ����ҳ��Ŀǰֻ֧��CP_ACP��CP_UTF8���
    *@return my_wstring
    *	���ַ��ַ���
    */
    static int MultiByteToWideChar(const char* pszMultiByte, my_wchar_t* pswzValue ,int iLen, int iCodePage = CP_ACP);


    static string MyWideCharToMultiByte(const my_wchar_t* pwszWideChar, int iCodePage = CP_ACP);
    /**
    *	���ַ��ַ���ת��Ϊ���ֽ�
    *@param pwszWideChar
    *	[in] ���ַ��ַ���
    *@param iCodePage
    *	[in] ����ҳ��Ŀǰֻ֧��CP_ACP��CP_UTF8����
    *@return sring
    *	���ֽ��ַ���
    */
    static int WideCharToMultiByte(const my_wchar_t* pwszWideChar,char* pszValue, int iLen, int iCodePage = CP_ACP);


    static unsigned long long atox( const signed char* s,short string_type);


    static char * chrcpy(char * dst, unsigned long dstlen, const char * src, unsigned long ulSrcLen = 0);

    static my_wchar_t* wchrcpy(my_wchar_t* dst, unsigned long dstlen, const my_wchar_t* src, unsigned long ulSrcLen = 0);


    static int GetDataLength(unsigned char* puszSrc, int nSrclen, unsigned char ucTag);


    /**
    *	���ַ����еĴ�д��ĸת��ΪСд��ĸ,ֻ��ASCII�ַ���Ч
    *@param strConvert
    *	[in] Ҫת�����ַ���
    *@return string
    *	Сд�ַ���
    */


    static bool SetAppPathName(const my_wchar_t* pswzAppPathName);

    static OS_TYPE GetOSType();

    //��ȡCSV�ļ��еķָ�����������;����,

    static char GetCSVSeperator();


    static int GetWindowsVersion();

    /**
    *	��ȡwindowsϵͳ�����״̬
    *@param strConvert
    *	[my_wstring&] ���������
    *@return int
    *	����ĵ�ǰ״̬
    */
    //static int GetServiceStatus( const wchar_t* strSrvName );

    static void ChangeFileRight(const char* pszFileName);

    /**
    *	����������ֵ
    *@param pszModuleName
    *	[in] ģ������
    *@param pszItemName
    *	[in] �����������
    *@param pszValue
    *	�������ֵ,����Ĭ�ϻ��ȡ�û������ļ��е����ݣ�����û�������û�и�����ȡϵͳ������
    *@param pLen
    *	����pszValue��ָ��ռ�Ĵ�С,��������ʱpLen��������������ַ����ĳ���,����������'\0'��
    *@return bool
    *	�ɹ�����true, ʧ�ܷ���false.��������pszValue�ռ䲻��,��ͨ��pLen������ȡʵ������Ŀռ��С.
    */
    static bool GetIniSetting(const my_wchar_t * pszModuleName, const my_wchar_t * pszItemName, my_wchar_t * pszValue, int * pLen);


    /**
    *	�����������ֵ
    *@param pszModuleName
    *	[in] ģ������
    *@param pszItemName
    *	[in] �����������
    *@param pszValue
    *	[in] �������ֵ
    *@return bool
    *	�ɹ�����true ʧ�ܷ���false
    *       ֻ�ܹ���UserSetting����д������SysSetting�������޸�
    */
    static bool SetIniSetting(const my_wchar_t * pszModuleName, const my_wchar_t * pszItemName, const my_wchar_t * pszValue );

    /*����ļ��Ƿ����*/
    static bool isExistFold(string strFoldPath);

    /**ȡ��˫�����е��ַ� -- "" **/
    static string TakeOutDoubleQuotationMarks(const string& strSrc);

    /**���˫���� -- "" **/
    static string AddDoubleQuotationMarks(const char *pszSrc);

    static bool ToLowerString(string& strConvert);
    static bool ToUpperString(string& strConvert);

    static bool  my_wcscmp(const my_wstring s1,const my_wstring s2);
    static bool  my_wcsncmp(const my_wstring s1,const my_wstring s2, size_t n);

    static my_wstring MyGetSetting(const my_wstring pszModuleName, const my_wstring pszItemName);

    // ��ȡӦ�ó�������Ŀ¼
    static string GetRootDir();
    /*md5 ����*/
    /* �������ַ�����md5����
     * input -- ����Ҫ�����ַ�
     * inputlen -- ����buffer����
     * digest  -- ���ܺ�����
     * return N/A
    */
    static void MD5(unsigned char * input, unsigned int inputlen, char digest[32]);
    /* ������·�����ļ����ݽ���md5����
     * filename -- ����Ҫ�����ַ�
     * digest  -- ���ܺ�����
     * return N/A
    */
    static void MD5File(const char * filename, char digest[32]);
private:
    static my_wstring GetSetingsFromXml(const char* pszModule, const char* pszItem,const char* pszFilePath);

    static my_wstring m_wstrAppPathName;
    //static my_wstring GetLocalDataPath();

};

#endif // GLOBALFUNCTION_H
