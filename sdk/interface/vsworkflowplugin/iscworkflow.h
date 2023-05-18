#pragma once

#include <string>
using std::string;

class IAutoJobItem;

class ISCWorkFlow
{
public:
    enum
    {
        IID_SC_WORKFLOW = 0x103,    //视觉流程执行接口
    };

    virtual ~ISCWorkFlow(){};

    /****************************************************
    *Function: 初始化操作
    *Intput: NULL
    *Output: NULL
    *Return: 成功返回true ，失败返回false
    *Remark: 初始化内部变量等
    ******************************************************/
    virtual bool Init() = 0;

    /****************************************************
    *Function: 去初始化操作
    *Intput: NULL
    *Output: NULL
    *Return: 成功返回true ，失败返回false
    *Remark: 释放资源等
    ******************************************************/
    virtual bool UnInit() = 0;

    /****************************************************
    *Function: 初始化Job
    *Intput: jobName：流程名称
    *Output: NULL
    *Return: 成功返回true ，失败返回false
    *Remark: 流程第一次执行之前，或者流程子项有添加、修改、删除之后调用
    ******************************************************/
    virtual bool InitJob(const char *jobName) = 0;

    /****************************************************
    *Function: 判断Job是否初始化
    *Intput: jobName：流程名称
    *Output: NULL
    *Return: 已经初始化则返回true，否则返回false
    ******************************************************/
    virtual bool IsJobInit(const char *jobName) = 0;

    /****************************************************
    *Function: 执行Job
    *Intput: jobName：流程名称
            strFailedItem: 失败的流程子项名称，成功的话为空
            bFailedExit:如果流程子项失败则退出，不执行后续流程子项；否则执行所有流程子项
    *Output: NULL
    *Return: 成功返回true ，失败返回false
    ******************************************************/
    virtual bool Execute(const char *jobName, string &strFailedItem, bool bFailedExit = true) = 0;
    
    /****************************************************
    *Function: 根据传入JobName->itemName执行对应item项
    *Intput: jobName：流程名称
            szItem: item名
            winID: 窗口ID， 用传入过程中用于调试处理
            pInnerParam：item功能的内部参数，用于调试， 保存时需要调用Save接口
    *Output: NULL
    *Return: 成功返回true ，失败返回false
    ******************************************************/
    virtual bool ExecuteItem(const char *jobName, const char *szItem, long winID, void *pInnerParam) = 0;
    
    /****************************************************
    *Function: 获取对应item项的内部参数，用于界面参数调试
    *Intput: jobName：流程名称
            szItem: item名
    *Output: pInnerParam：item功能项的内部加载的参数
    *Return: 成功返回true ，失败返回false
    ******************************************************/
    virtual bool GetItemParam(const char *jobName, const char *szItem, void *pInnerParam) = 0;
    
    /****************************************************
    *Function: 保存对应item项的内部参数
    *Intput: jobName：流程名称
            szItem: item名
            pInnerParam：item功能的内部参数
    *Output: NULL
    *Return: 成功返回true ，失败返回false
    ******************************************************/
    virtual bool SaveItemParam(const char *jobName, const char *szItem, void *pInnerParam) = 0;
    /****************************************************
    *Function: 获取流程子项执行结果
    *Intput: jobName：流程名称
            jobItemName：子项名称
            pName：参数名称
    *Output: pValue：输出结果
    *Return: 成功返回true ，失败返回false
    ******************************************************/
    virtual bool GetItemResult(const char *jobName, const char *jobItemName, const char *pName, void **pValue) = 0;

    /****************************************************
    *Function: 获取流程执行结果
    *Intput: jobName：流程名称
            pName：参数名称
    *Output: pValue：输出结果
    *Return: 成功返回true ，失败返回false
    *Remark：获取流程执行结果，如果流程执行完成，则返回最后一个item的结果，
            否则返回执行出错的item的结果，比如LOG 图片等
    ******************************************************/
    virtual bool GetResult(const char *jobName, const char *pName, void **pValue) = 0;

    /****************************************************
    *Function: 显示流程执行结果
    *Intput: jobName：流程名称
            jobItemName：子项名称
            nWinID：窗口ID
    *Output: NULL
    *Return: 成功返回true ，失败返回false
    ******************************************************/
    virtual bool DisplayResult(const char *jobName, const char *jobItemName, int nWinID) = 0;

    /****************************************************
    *Function: 显示流程执行结果
    *Intput: jobName：流程名称
            nWinID：窗口ID
    *Output: NULL
    *Return: 成功返回true ，失败返回false
    *Remark: 如果执行成功则显示最后一个Item结果，否则显示执行失败的Item的结果
    ******************************************************/
    virtual bool DisplayResult(const char *jobName, int nWinID) = 0;

    /****************************************************
    *Function: 获取参数路径
    *Intput: pJobName：流程名称
            jobItemName：参数名称
    *Output: NULL
    *Return: 返回参数的绝对路径
    ******************************************************/
    virtual string GetParamPath(const char *pJobName, const char *jobItemName) = 0;

    /****************************************************
    *Function: 获取IAutoJobItem指针
    *Intput: pJobName：流程名称
            jobItemName：参数名称
            iType: JobItem类型
    *Output: NULL
    *Return: 返回IAutoJobItem指针
    *Remark: 如果iType是自定义halcon过程，如果不存在JobItem，则创建JobItem，其他类型返回NULL
    ******************************************************/
    virtual IAutoJobItem* GetJobItem(const char *pJobName, const char *jobItemName, const char *iType = NULL) = 0;

	/****************************************************
    *Function: 获取工厂对象名
    *Intput: pJobName：流程名称
            jobItemName：参数名称
    *Output: NULL
    *Return: 返回工厂对象名
    ******************************************************/
	virtual string GetFactoryObjectName(const char *pJobName, const char *jobItemName) = 0;
};
