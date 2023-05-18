
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
    // setPluginContainer��������PluginContainer���ã����������ֹ����
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
   *	��ȡ��������Ŀ¼(����·��)
   *@param
   *	��
   *@return OS_wstring
   *	���س�������Ŀ¼,��"c:\Program Files\Mobile Manager\"
   *    ����ϵͳ�Ĳ�ͬ��·���ָ���Ҳ����ͬ
   */
    static int GetModuleDirectory(OS_wchar_t* pswzDir, int iLen);

    /**
    *	��ȡ�û���������Ŀ¼(����·��)
    *@param
    *	��
    *@return OS_wstring
    *	���ز������Ŀ¼,��"c:\Program Files\Open Studio\UserData"
    *   ����ϵͳ�Ĳ�ͬ��·���ָ���Ҳ����ͬ
    */


    static int GetUserDataDirectory(OS_wchar_t* pswzDir, int iLen);


    /**
    *	��ȡ��Log��־�ļ��е�Ŀ¼(����·��)
    *@param
    *	��
    *@return OS_wstring
    *   ���ز������Ŀ¼,��"c:\Program Files\Open Studio\UserData\Log"
    *   ����ϵͳ�Ĳ�ͬ��·���ָ���Ҳ����ͬ
    */
    static int GetLogDirectory(OS_wchar_t* pswzDir, int iLen);

    /**
        *	����������ֵ
        *@param pszModuleName
        *	[in] ģ������
        *@param pszItemName
        *	[in] �����������
        *@return OS_wstring
        *	�������ֵ,����Ĭ�ϻ��ȡ�û������ļ��е����ݣ�����û�������û�и�����ȡϵͳ������
    */
    static int GetSetting(const OS_wchar_t* pszModuleName, const OS_wchar_t* pszItemName,OS_wchar_t* pswzValue,int iLen);


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
    static bool SetSetting(const OS_wchar_t* pszModuleName, const OS_wchar_t* pszItemName, const OS_wchar_t* pszValue);



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
    static bool DelSetting(const OS_wchar_t* pszModuleName, const OS_wchar_t* pszItemName);



    /**
        *	���ֽ��ַ���ת��Ϊ���ַ�
        *@param pszMultiByte
        *	[in] ���ֽ��ַ���
        *@param iCodePage
        *	[in] ����ҳ��Ŀǰֻ֧��CP_ACP��CP_UTF8���
        *@return OS_wstring
        *	���ַ��ַ���
    */
    static int MultiByteToWideChar(const char* pszMultiByte, OS_wchar_t* pswzValue ,int iLen, int iCodePage = CP_ACP);

    /**
        *	���ַ��ַ���ת��Ϊ���ֽ�
        *@param pwszWideChar
        *	[in] ���ַ��ַ���
        *@param iCodePage
        *	[in] ����ҳ��Ŀǰֻ֧��CP_ACP��CP_UTF8����
        *@return sring
        *	���ֽ��ַ���
    */

    static int WideCharToMultiByte(const OS_wchar_t* pwszWideChar,char* pszValue, int iLen, int iCodePage = CP_ACP);


    // from chrcpy.h
    static char * chrcpy(char * dst, unsigned long dstlen, const char * src, unsigned long ulSrcLen = 0);
    static OS_wchar_t* wchrcpy(OS_wchar_t* dst, unsigned long dstlen, const OS_wchar_t* src, unsigned long ulSrcLen = 0);
    static int GetDataLength(unsigned char* puszSrc, int nSrclen, unsigned char ucTag);


    static bool SetAppPathName(const OS_wchar_t* pswzAppPathName);

    
    static OS_TYPE GetOSType();

    //��ȡCSV�ļ��еķָ�����������;����,
    static char GetCSVSeperator();

    static int GetWindowsVersion();


    /**
         *	��ȡwindowsϵͳ�����״̬
         *@param strConvert
         *	[OS_wstring&] ���������
         *@return int
         *	����ĵ�ǰ״̬
         */
	static int GetServiceStatus( const wchar_t* strSrvName ); 


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
    static bool GetIniSetting(const OS_wchar_t * pszModuleName, const OS_wchar_t * pszItemName, OS_wchar_t * pszValue, int * pLen);


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
    static bool SetIniSetting(const OS_wchar_t * pszModuleName, const OS_wchar_t * pszItemName, const OS_wchar_t * pszValue );

private:
    static OS_wstring GetSetingsFromXml(const char* pszModule, const char* pszItem,const char* pszFilePath);

    static OS_wstring m_wstrAppPathName;

    static OS_wstring GetLocalDataPath();

};

#endif // COMMON_H
