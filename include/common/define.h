#ifndef __DEFINE__H__
#define __DEFINE__H__

#include "jlibrary.h"

typedef JLibrary*   HDLL;
/**
*	装载动态连接库
*@param PluginFileName
*	[in] 插件文件名称
*/
    #define LOAD_DYNAMIC_LIBRARY(PluginFileName)                    new JLibrary(PluginFileName)
/**
*	获取函数地址
*@param hModule
*	[in] 插件文件名称
*/
    #define GET_FUNTION_ADDR(hModule, FunName)			hModule->resolve(FunName)
/**
*	释放动态连接库
*@param PluginFileName
*	[in] 插件文件名称
*/
    #define FREE_DYNAMIC_LIBRARY(hModule)				{hModule->unload();if(NULL != hModule){delete hModule;hModule = NULL;}}



#endif // __DEFINE__H__