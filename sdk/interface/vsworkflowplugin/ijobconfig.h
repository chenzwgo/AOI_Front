#ifndef _IJobConfig_h_
#define _IJobConfig_h_

#include "processbase.h"

class IJobConfig
{
public:
    enum
    {
        IID_JOB_CONFIG = 0x101,    //工作流配置解析接口
    };

    virtual ~IJobConfig(){};

    ///****************************************************
    //*Function: 获取流程类型
    //*Intput: iType: 流程类型
    //*Output: NULL
    //*Return: 返回字符流程类型
    //******************************************************/
    //virtual string GetJobType(int iType) = 0;

    ///****************************************************
    //*Function: 获取流程类型
    //*Intput: pszType:  
    //*Output: NULL
    //*Return: 返回流程类型
    //******************************************************/
    //virtual int GetJobType(const char *pszType) = 0;

    /****************************************************
    *Function: 查询所有流程信息
    *Intput: NULL
    *Output: NULL
    *Return: 返回流程列表
    ******************************************************/
    virtual list<StJob*> *QueryJobs(void) = 0;

    /****************************************************
    *Function: 查找流程，读内存
    *Intput: szJobName：流程名称
    *Output: job：流程
    *Return: 找到则返回true，否则返回false
    ******************************************************/
    virtual bool QueryJob(const char * szJobName, StJob **job) = 0;

    /****************************************************
    *Function: 查找流程子项，读内存
    *Intput: pszJobName：流程名称 pszJobItemName 流程子项名称
    *Output: StJobItem：流程子项
    *Return: 找到则返回true，否则返回false
    ******************************************************/
    virtual bool QueryJobItem(const char * pszJobName, const char * pszJobItemName, StJobItem **jobItem) = 0;

    /****************************************************
    *Function: 读取所有job,读配置文件
    *Return:  返回流程列表
    ******************************************************/
    virtual list<StJob*> *ReadAllJobsFromCfg() = 0;
    
    /****************************************************
    *Function: 保存说有job，写入配置文件
    *Return: 成功则返回true，否则返回false
    ******************************************************/
    virtual bool SaveAllJobsToCfg() = 0;

    
    /****************************************************
    *Function: 重命名,直接生效，写入文件
    *Input:  pszOldJobName: 旧流程名称
    *Input:  szJobName：流程名称
    *Output: vecParam：流程出参信息
    *Return: 找到则返回true，否则返回false
    ******************************************************/
    virtual bool RenameJob(const char *pszOldJobName, const char *szJobName) = 0;

    /****************************************************
    *Function: 查询流程出参(最后一个item的出参)
    *Intput: szJobName：流程名称
    *Output: vecParam：流程出参信息
    *Return: 找到则返回true，否则返回false
    ******************************************************/
    // 没用到 scj
    //virtual bool QueryJobOutputParam(const char * szJobName, vector<ProcessParam>& vecParam) = 0;

    /****************************************************
    *Function: 判断工作流是否存在
    *Intput: szJobName：工作流名称
    *Output: NULL
    *Return: 存在则返回true，否则返回false
    ******************************************************/
    virtual bool IsJobExist(const char * szJobName) = 0;

    /****************************************************
    *Function: 获取工作流数量
    *Intput: NULL
    *Output: NULL
    *Return: 返回工作流数量
    ******************************************************/
    virtual int GetJobCount() = 0;


    /****************************************************
    *Function: 插入新的工作流
    *Intput: NULL
    *Output: NULL
    *Return: 返回工作流数量
    ******************************************************/
    virtual bool InsertJob(const StJob &job) = 0;

    /****************************************************
    *Function: 修改流程
    *Intput: szJobName：流程名称
             job：修改之后的流程
    *Output: NULL
    *Return: 返回是否修改成功
    ******************************************************/
    virtual bool UpdateJob(const char *szJobName, const StJob &job) = 0;

    /****************************************************
    *Function: 删除流程
    *Intput: szJobName：流程名称
    *Output: NULL
    *Return: 返回是否删除成功
    ******************************************************/
    virtual bool DeleteJob(const char *szJobName) = 0;

     /****************************************************
    *Function: 插入新的工作项
    *Intput: szJobName：流程名称
    *Intput: szPreItemName：上一个项的名字，=NULL 表示插入到最上面
    *Intput: jobItem：要插入的项
    *Output: NULL
    *Return: 返回是否插入成功
    ******************************************************/
    virtual bool InsertJobItem(const char *szJobName, const char *szPreItemName, const StJobItem &jobItem) = 0;

    /****************************************************
    *Function: 删除流程项
    *Intput: szJobName：流程名称
    *Intput: jobItem：要删除的项
    *Output: NULL
    ******************************************************/
    virtual void DeleteJobItem(const char *szJobName, const char *pszItemName) = 0;

    /****************************************************
    *Function: 移动工作项，
    *Intput: szJobName：流程名称，
             szItemName：工作项名称
             bUp：上移或者下移
    *Output: NULL
    *Return: 返回是否移动成功
    ******************************************************/
    bool MoveJobItem(const char *szJobName, const char *szItemName, bool bUp);

    //编辑工作项
    /****************************************************
    *Function: 修改流程
    *Intput: szJobName：流程名称
             szItemName：工作项名称
             jobItem：修改之后的工作项
    *Output: NULL
    *Return: 返回是否修改成功
    ******************************************************/
    bool UpdateJobItem(const char *szJobName, const char *szItemName, const StJobItem &jobItem);

    /****************************************************
    *Function: 移动流程，
    *Intput: szJobName：流程名称，
            bUp：上移或者下移
    *Output: NULL
    *Return: 返回是否移动成功
    ******************************************************/
    virtual bool MoveJob(const char *szJobName, bool bUp) = 0;

    /****************************************************
    *Function: 加载流程，
    *Intput: pszJobPath：流程路径，
    *Output: NULL
    *Return: 返回是否加载成功
    ******************************************************/
    virtual bool ReadConfig(const char *pszJobPath) = 0;
};

#endif // _IJobConfig_h_