#ifndef _IPROCESSLIBCONFIG_H_
#define _IPROCESSLIBCONFIG_H_

#include "ProcessBase.h"

#include <list>
using std::list;

//流程类型解析
class IProcessLibConfig
{
public:
    enum
    {
        IID_PROCESS_LIB_CONFIG = 0x102,    //流程类别配置文件解析接口
    };

    virtual ~IProcessLibConfig() {};

    /****************************************************
    *Function: 获取流程类型
    *Intput: NULL
    *Output: NULL
    *Return: 返回流程列表
    ******************************************************/
    virtual list<ProcessSet*> *GetProcessInfo() = 0;

    /****************************************************
    *Function: 获取参数列表
    *Intput: szMethod：流程类型
    *Output: NULL
    *Return: 返回参数列表
    ******************************************************/
    virtual const vector<ProcessParam> *GetProcessParams(const char *szMethod) = 0;

    /****************************************************
    *Function: 获取参数的数据类型
    *Intput: szMethod：流程类型
            szParamName:参数名称
            iParamType：入参、出参
    *Output: NULL
    *Return: 返回参数数据类型
    ******************************************************/
    virtual string GetParamDataType(const char *szMethod, const char *szParamName, int iParamType) = 0;


    virtual string GetMethodUIGuidName(const char *szMethod) = 0;
};

#endif // _IPROCESSLIBCONFIG_H_