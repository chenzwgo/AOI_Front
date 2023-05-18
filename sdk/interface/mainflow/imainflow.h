#ifndef IWORKFLOW_H
#define IWORKFLOW_H

#include <string>
#include "mainflow/workflowdefine.h"
#include "mainflowaction/workflowactiondefine.h"
#include "mainflowaction/actionparam.h"
using namespace std;
class CAction;
class IMainFlowEvent;
class IInOutVariable;
class IMainFlowFun;
class iMainFlowCfg;
class IMainFlow
{
public:
    enum
    {
        IDD_MAIN_FLOW = 0x001
    };

    virtual ~IMainFlow(){}


    /************************************************************************/
    // @brief 启动流程,从beginStep执行到endStep
    // @param flowName					-- [in ]流程名称
    // @param beginStep					-- [in ]起始步骤名称，为空，表示从第一个步骤开始执行
    // @param endStep					-- [in ]结束步骤名称，为空，表示执行到最后一个步骤
    // @param bRunLoop				    -- [in ]是否循环执行
    // @param bStepByStep				-- [in ]是否单步执行
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
    virtual int runFlow(const string &flowName, const string &beginStep = "", const string &endStep = "", bool bRunLoop = false, bool bStepByStep = false) = 0;

    /************************************************************************/
    // @brief 启动流程,从beginStep执行到endStep
    // @param flowName					-- [in ]流程名称
    // @param beginStep					-- [in ]起始步骤索引，为-1，表示从第一个步骤开始执行
    // @param endStep					-- [in ]结束步骤索引，为-1，表示执行到最后一个步骤
    // @param bRunLoop				    -- [in ]是否循环执行
    // @param bStepByStep				-- [in ]是否单步执行
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int runFlow(const string &flowName, int stepBeginIndex, int stepEndIndex, bool bRunLoop = false, bool bStepByStep = false) = 0;

    /************************************************************************/
    // @brief 暂停流程
    // @param flowName					-- [in ]流程名称
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
    virtual int pauseFlow(const string &flowName) = 0;

    /************************************************************************/
    // @brief 继续流程
    // @param flowName					-- [in ]流程名称
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int continueFlow(const string &flowName, bool bStepByStep = false ) = 0;

    /************************************************************************/
    // @brief 停止流程
    // @param flowName					-- [in ]流程名称
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
    virtual int stopFlow(const string &flowName) = 0;

    /************************************************************************/
    // @brief 复位流程(预留,暂时无用)
    // @param flowName					-- [in ]流程名称
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual void resetFlow(const string &flowName) = 0;

    /************************************************************************/
    // @brief 获取当前动作(目前UI显示用到)
    // @param flowName					-- [in ]流程名称
    // @param stepname					-- [out]步骤名称
    // @param actionName				-- [out]动作名称
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int getCurrentAction(const string &flowName, string &stepname,string &actionName) = 0;

    /************************************************************************/
    // @brief 获取当前动作(目前UI显示用到)
    // @param flowName					-- [in ]流程名称
    // @param stepname					-- [out]步骤索引
    // @param actionName				-- [out]动作索引
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int getCurrentAction(const string &flowName, int &stepIndex, int &actionIndex) = 0;

    /************************************************************************/
    // @brief 获取流程当前执行状态
    // @param flowName					-- [in ]流程名称
    // @return E_FLOW_STATUS                                     
    /************************************************************************/ 
    virtual E_FLOW_STATUS runStatus(const string &flowName) = 0;

    /************************************************************************/
    // @brief 清除流程状态(包括步骤,动作的状态)
    // @param flowName					-- [in ]流程名称
    // @param stepBeginIndex			-- [in ]开始步骤
    // @param stepEndIndex				-- [in ]结束步骤
    // @return void                                     
    /************************************************************************/ 
    virtual void clearStatus(const string &flowName, int stepBeginIndex = 0, int stepEndIndex = -1) = 0;

	/***** 内部 单个step 操作， 与内部 单个action 的操作，方便上层 自定义操作******/
    /************************************************************************/
    // @brief 执行单个动作
    // @param flowName					-- [in ]流程名称
    // @param stepName			        -- [in ]步骤名称
    // @param actionName				-- [in ]动作名称
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int runAction(const string &flowName, const string &stepName ,const string &actionName)=0;

    /************************************************************************/
    // @brief 执行单个动作
    // @param flowName					-- [in ]流程名称
    // @param stepIndex			        -- [in ]步骤索引
    // @param actionIndex				-- [in ]动作索引
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int runAction(const string &flowName, int stepIndex ,int actionIndex) = 0;

    /************************************************************************/
    // @brief 停止单个动作
    // @param flowName					-- [in ]流程名称
    // @param stepIndex			        -- [in ]步骤索引
    // @param actionIndex				-- [in ]动作索引
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
    virtual int stopAction(const string &flowName, int stepIndex ,int actionIndex) = 0;

    /************************************************************************/
    // @brief 暂停单个动作
    // @param flowName					-- [in ]流程名称
    // @param stepIndex			        -- [in ]步骤索引
    // @param actionIndex				-- [in ]动作索引
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/
    virtual int pauseAction(const string &flowName, int stepIndex ,int actionIndex) = 0;

    /************************************************************************/
    // @brief 继续单个动作
    // @param flowName					-- [in ]流程名称
    // @param stepIndex			        -- [in ]步骤索引
    // @param actionIndex				-- [in ]动作索引
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/
    virtual int continueAction(const string &flowName, int stepIndex ,int actionIndex) = 0;

    /************************************************************************/
    // @brief 获取单个动作状态
    // @param flowName					-- [in ]流程名称
    // @param stepName			        -- [in ]步骤名称
    // @param actionName				-- [in ]动作名称
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/
	virtual E_ACTION_STATUS GetActionStatus(const string &flowName, const string &stepName ,const string &actionName) = 0; 

    /************************************************************************/
    // @brief 获取单个动作状态
    // @param flowName					-- [in ]流程名称
    // @param stepIndex			        -- [in ]步骤索引
    // @param actionIndex				-- [in ]动作索引
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/
	virtual E_ACTION_STATUS GetActionStatus(const string &flowName, int stepIndex ,int actionIndex) = 0; // 获取动作状态

    /************************************************************************/
    // @brief 获取单个动作执行耗时
    // @param flowName					-- [in ]流程名称
    // @param stepIndex			        -- [in ]步骤索引
    // @param actionIndex				-- [in ]动作索引
    // @return long                     -- 耗时时间               
    /************************************************************************/
    virtual long getActionTimeSpent(const string &flowName, int stepIndex, int actionIndex) = 0;// 获取单个IAciton 实例

    /************************************************************************/
    // @brief 获取流程执行耗时
    // @param flowName					-- [in ]流程名称
    // @return long                     -- 耗时时间              
    /************************************************************************/
    virtual long getFlowTimeSpent(const string &flowName) = 0;

    /************************************************************************/
    // @brief 获取流程配置接口
    // @return iMainFlowCfg
    /************************************************************************/
    virtual iMainFlowCfg* GetMainFlowCfg()= 0;
};

#endif // IWORKFLOW

