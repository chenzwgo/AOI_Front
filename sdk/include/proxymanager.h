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
     * @brief SDK入口函数.
     * @brief SDK入口函数，加载SDK插件容器，并保存容器句柄
     * @param pwszAppPathName
     * @return 返回初始化结果
     */
     RT_API OS_ERROR StartupSDK(const OS_WCHAR* pwszAppPathName);

    /**
     * @brief 制裁SDK插件容器
     * @return 卸载结果
     */
    RT_API OS_ERROR ShutdownSDK();

    RT_API void* GetSDKScriptObj(const char* pszCLSID, const int iid );

    RT_API void* GetSDKInstance(const char* pszCLSID, const int iid );


#ifdef __cplusplus
    }
#endif


#endif // PROXYMANAGER_C_H
