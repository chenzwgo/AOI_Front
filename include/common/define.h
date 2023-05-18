#ifndef __DEFINE__H__
#define __DEFINE__H__

#include "jlibrary.h"

typedef JLibrary*   HDLL;
/**
*	װ�ض�̬���ӿ�
*@param PluginFileName
*	[in] ����ļ�����
*/
    #define LOAD_DYNAMIC_LIBRARY(PluginFileName)                    new JLibrary(PluginFileName)
/**
*	��ȡ������ַ
*@param hModule
*	[in] ����ļ�����
*/
    #define GET_FUNTION_ADDR(hModule, FunName)			hModule->resolve(FunName)
/**
*	�ͷŶ�̬���ӿ�
*@param PluginFileName
*	[in] ����ļ�����
*/
    #define FREE_DYNAMIC_LIBRARY(hModule)				{hModule->unload();if(NULL != hModule){delete hModule;hModule = NULL;}}



#endif // __DEFINE__H__