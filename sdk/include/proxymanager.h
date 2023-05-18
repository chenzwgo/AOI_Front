#ifndef PROXYMANAGER_C_H
#define PROXYMANAGER_C_H

#include "OS_DEFINE.h"
#include "dllexport.h"
//#include <QObject>

#include <string>
using namespace std;

#ifdef __cplusplus
    extern "C" {
#endif

    /**
     * @brief SDK��ں���.
     * @brief SDK��ں���������SDK����������������������
     * @param pwszAppPathName
     * @return ���س�ʼ�����
     */
     RT_API OS_ERROR StartupSDK(const OS_WCHAR* pwszAppPathName);

    /**
     * @brief �Ʋ�SDK�������
     * @return ж�ؽ��
     */
    RT_API OS_ERROR ShutdownSDK();

    RT_API void* GetSDKScriptObj(const char* pszCLSID, const int iid );

    RT_API void* GetSDKInstance(const char* pszCLSID, const int iid );


#ifdef __cplusplus
    }
#endif


#endif // PROXYMANAGER_C_H
