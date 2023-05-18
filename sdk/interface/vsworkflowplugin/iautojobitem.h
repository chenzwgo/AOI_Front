#ifndef _I_AUTO_JOB_ITEM_H_
#define _I_AUTO_JOB_ITEM_H_
//流程子项接口
class IAutoJobItem 
{
public:
    virtual ~IAutoJobItem(){};

    /****************************************************
    *Function: 设置流程子项名称
    *Intput: pszObjName：流程子项名称
    *Output: NULL
    *Return: NULL
    *Remark: 流程子项创建之后使用，方便调试
    ***************************************************** */
    virtual void SetName(const char *pszObjName) = 0;

    /****************************************************
    *Function: 设置流程子项内部参数文件路径
    *Intput: pszPath：流程子项内部参数文件路径
    *Output: NULL
    *Return: NULL
    *Remark: 读取、设置内部参数时使用
    ***************************************************** */
    virtual void SetPath(const char *pszPath) = 0;

    /****************************************************
    *Function: 修改且保存流程子项内部参数
    *Intput: NULL
    *Output: NULL
    *Return: 修改是否成功
    *Remark: 调用其他UI插件显示修改界面
    ***************************************************** */
    virtual bool SaveInnerParam(void *pInnerParam) = 0; 

    /****************************************************
    *Function: 获取流程子项内部参数
    *Intput: NULL
    *Output: pInnerParam， 传出参数
    *Return: 是否成功
    *Remark: 
    ***************************************************** */
    virtual bool GetInnerParam(void *pInnerParam) = 0; 

    /****************************************************
    *Function: 清空上次执行结果，初始化内部变量等
    *Intput: NULL
    *Output: NULL
    *Return: NULL
    *Remark: 设置参数之前调用
    ***************************************************** */
    virtual void Clear() = 0;

    /****************************************************
    *Function: 设置流程子项荐之间的输入参数
    *Intput: pName：参数名称，例如：image， line1等
             pData：数据指针
    *Output: NULL
    *Return: 返回设置是否成功
    *Remark: 来自此项之前流程子项的输出
    ***************************************************** */
    virtual bool SetInputParam(const char *pName, void *pData) = 0; 

    /****************************************************
    *Function: 重新加载流程子项内部参数
    *Intput: NULL
    *Output: NULL
    *Return: 返回设置是否成功
    *Remark: 通过SetInnerParam()函数实现修改，路径保存在"./Process/流程名称/流程子项/"目录下
    ***************************************************** */
    virtual bool ReloadInnerParam() = 0;  

    /****************************************************
    *Function: 执行流程子项算法
    *Intput: NULL
    *Output: NULL
    *Return: 返回执行是否成功
    *Remark: 如果执行失败，需要检查入参是否设置，执行过程是否异常等
    ***************************************************** */
    virtual bool Execute(const void *pInParam = 0, long halconWID = -1) = 0; 

    /****************************************************
    *Function: 获取执行结果
    *Intput: name：结果名称
    *Output: pResult： 结果数据指针
    *Return: 返回获取是否成功
    ***************************************************** */
    virtual bool GetResult(const char *name, void **pResult) = 0; //修改

    /****************************************************
    *Function: 显示执行结果
    *Intput: nWinID：窗口ID，halcon通过open_window打开的窗口ID，例如3600
    *Output: NULL
    *Return: 返回显示是否成功
    ***************************************************** */
    virtual bool DisplayResult(int nWinID) = 0;  //修改


	    /****************************************************
    *Function: 返回错误信息
    *Intput: 无
    *Output: NULL
    *Return: 返回错误码
    ***************************************************** */
	virtual int GetLastError() = 0;
};

#endif