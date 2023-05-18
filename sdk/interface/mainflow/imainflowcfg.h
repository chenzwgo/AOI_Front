#ifndef IMAINFLOWCFG_H
#define IMAINFLOWCFG_H

#include "workflowdefine.h"

class iMainFlowCfg
{
public:
	enum
	{
		IID_MAINFLOW_CFG = 0x002
	};

	virtual ~iMainFlowCfg(){}

    /************************************************************************/
    // @brief 判断配置文件是否有修改
    // @return bool
    /************************************************************************/
	virtual bool IsModify( void ) = 0;

    /************************************************************************/
    // @brief 插入一个工作流程
    // @param wIndex					-- [in ]流程索引
    // @param pstWork					-- [in ]流程配置
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int InsertWork(int wIndex, const WORK_FLOW* pstWork) = 0;

    /************************************************************************/
    // @brief 插入一个步骤
    // @param wIndex					-- [in ]流程索引
    // @param sIndex					-- [in ]步骤索引
    // @param pstStep					-- [in ]步骤配置
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int InsertStep(int wIndex, int sIndex, const WORK_STEP* pstStep) = 0;

    /************************************************************************/
    // @brief 插入一个动作
    // @param wIndex					-- [in ]流程索引
    // @param sIndex					-- [in ]步骤索引
    // @param pIndex					-- [in ]动作索引
    // @param pstItem					-- [in ]动作配置
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int InsertAction(int wIndex, int sIndex, int pIndex, const WORK_ACTION* pstItem) = 0;

    /************************************************************************/
    // @brief 动作名是否存在
    // @param wIndex					-- [in ]流程索引
    // @param name					    -- [in ]动作名称
    // @return bool
    /************************************************************************/
	virtual bool IsActionNameExist(int wIndex, const char* name) = 0 ;

    /************************************************************************/
    // @brief 动作是否重名
    // @param wIndex					-- [in ]流程索引
    // @param name					    -- [in ]动作名称
    // @return bool
    /************************************************************************/
    virtual bool IsActionNameRepeat(int wIndex, const char* name) = 0 ;

    /************************************************************************/
    // @brief 移动一个流程
    // @param oldIndex					-- [in ]流程当前索引
    // @param newIndex					-- [in ]流程新的索引
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual int MoveWork( int oldIndex, int newIndex ) = 0;

    /************************************************************************/
    // @brief 移动一个步骤
    // @param wIndex					-- [in ]流程索引
    // @param oldIndex					-- [in ]步骤当前索引
    // @param newIndex					-- [in ]步骤新的索引
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual int MoveStep( int wIndex, int oldIndex, int newIndex ) = 0;

    /************************************************************************/
    // @brief 移动一个动作
    // @param wIndex					-- [in ]流程索引
    // @param sIndex					-- [in ]步骤索引
    // @param oldIndex					-- [in ]动作当前索引
    // @param newIndex					-- [in ]动作新的索引
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual int MoveAction(int wIndex, int sIndex, int oldIndex, int newIndex) = 0;

    /************************************************************************/
    // @brief 获取流程个数
    // @return int           -- 流程个数
    /************************************************************************/
	virtual int GetWorkCount() = 0;

    /************************************************************************/
    // @brief 获取步骤的个数
    // @param wIndex					-- [in ]流程索引
    // @return int                      -- 步骤个数
    /************************************************************************/
	virtual int GetStepCount(int wIndex) =0 ;

    /************************************************************************/
    // @brief 获取动作的个数
    // @param wIndex					-- [in ]流程索引
    // @param sIndex					-- [in ]步骤索引
    // @return int                      -- 动作个数
    /************************************************************************/
	virtual int GetActionCount(int wIndex, int sIndex) =0;

    /************************************************************************/
    // @brief 获取流程配置
    // @param wIndex					-- [in ]流程索引
    // @return WORK_FLOW*               -- 流程配置
    /************************************************************************/
	virtual WORK_FLOW* GetWorkCfg(int wIndex) = 0;

    /************************************************************************/
    // @brief 获取流程配置
    // @param WorkName					-- [in ]流程名称
    // @return WORK_FLOW*               -- 流程配置
    /************************************************************************/
    virtual WORK_FLOW* GetWorkCfg(const char* WorkName) = 0;

    /************************************************************************/
    // @brief 获取步骤配置
    // @param wIndex					-- [in ]流程索引
    // @param sIndex					-- [in ]步骤索引
    // @return WORK_STEP*               -- 步骤配置
    /************************************************************************/
	virtual WORK_STEP* GetStepCfg(int wIndex, int sIndex) = 0;

    /************************************************************************/
    // @brief 获取步骤配置
    // @param WorkName					-- [in ]流程名称
    // @param sIndex					-- [in ]步骤索引
    // @return WORK_STEP*               -- 步骤配置
    /************************************************************************/
	virtual WORK_STEP* GetStepCfg(const char* WorkName, int sIndex) = 0;

    /************************************************************************/
    // @brief 获取步骤配置
    // @param WorkName					-- [in ]流程名称
    // @param StepName					-- [in ]步骤名称
    // @return WORK_STEP*               -- 步骤配置
    /************************************************************************/
	virtual WORK_STEP* GetStepCfg(const char* WorkName, const char* StepName) = 0;

    /************************************************************************/
    // @brief 获取动作配置
    // @param WorkName					-- [in ]流程索引
    // @param sIndex					-- [in ]步骤索引
    // @param pIndex				    -- [in ]动作索引
    // @return WORK_ACTION*               -- 步骤配置
    /************************************************************************/
	virtual WORK_ACTION* GetActionCfg(int wIndex, int sIndex, int pIndex) = 0;

    /************************************************************************/
    // @brief 获取动作配置
    // @param WorkName					-- [in ]流程名称
    // @param StepName					-- [in ]步骤名称
    // @param ActionName				-- [in ]动作名称
    // @return WORK_ACTION*               -- 步骤配置
    /************************************************************************/
	virtual WORK_ACTION* GetActionCfg(const char* WorkName, const char* StepName,const char* ActionName) = 0;

    /************************************************************************/
    // @brief 获取动作配置
    // @param WorkName					-- [in ]流程名称
    // @param StepName					-- [in ]步骤索引
    // @param ActionName				-- [in ]动作索引
    // @return WORK_ACTION*               -- 步骤配置
    /************************************************************************/
	virtual WORK_ACTION* GetActionCfg(const char* WorkName, int sIndex, int pIndex) = 0;

    /************************************************************************/
    // @brief 判断动作参数是否OK
    // @param wIndex					-- [in ]流程索引
    // @param pActionParam				-- [in ]动作参数
    // @return bool
    /************************************************************************/
	virtual bool IsActionParamReady(int wIndex, const WORK_ACTION* pActionParam) = 0;

    /************************************************************************/
    // @brief 判断step中的动作参数是否都OK
    // @param WorkName					-- [in ]流程名称
    // @param sIndex				    -- [in ]步骤索引
    // @return bool
    /************************************************************************/
	virtual bool IsStepParamReady(const char* WorkName, int sIndex) = 0;

    /************************************************************************/
    // @brief 删除一个工作流程
    // @param wIndex				    -- [in ]流程索引
    // @return bool
    /************************************************************************/
	virtual int DeleteWork(int wIndex) = 0;

    /************************************************************************/
    // @brief 删除一个步骤
    // @param wIndex				    -- [in ]流程索引
    // @param sIndex				    -- [in ]步骤索引
    // @return bool
    /************************************************************************/
	virtual int DeleteStep(int wIndex, int sIndex) = 0;

    /************************************************************************/
    // @brief 删除一个动作
    // @param wIndex				    -- [in ]流程索引
    // @param sIndex				    -- [in ]步骤索引
    // @param pIndex				    -- [in ]动作索引
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int DeleteAction(int wIndex, int sIndex, int pIndex) = 0;

    /************************************************************************/
    // @brief 从文件读取流程配置
    // @param fileName				    -- [in ]流程文件名
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int ReadCfg(const char *fileName = NULL) = 0;

    /************************************************************************/
    // @brief 将流程配置保存到文件
    // @param fileName				    -- [in ]流程文件名
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int SaveCfg(const char *fileName = NULL) = 0;

    /************************************************************************/
    // @brief 新建流程配置(不写文件)
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual int NewCfg() = 0;

    /************************************************************************/
    // @brief 获取默认配置(正常工作使用的流程配置文件路径)
    // @return string             -- 流程文件路径
    /************************************************************************/
    virtual string getCurrentCfgPath() = 0 ;

    /************************************************************************/
    // @brief 写入默认配置(正常工作使用的流程配置文件路径)
    // @param path				       -- [in ]流程文件路径
    // @return bool
    /************************************************************************/
    virtual bool writeCurrentCfgPath(string &path) = 0;
};

#endif // IWORKFLOWCFG

