#ifndef WORKFLOWDEFINE_H
#define WORKFLOWDEFINE_H

#include "rtdefine.h"
#include <string>
#include <vector>
using namespace std;

class WORK_ACTION;
#define ITEM_NAME_LEN  256             // 流程编辑各项名字长度

//工作流程控制-步骤信息
struct WORK_STEP
{
    char             name[ITEM_NAME_LEN];    //名字
    char			 description[ITEM_NAME_LEN];       //点位功能描述
    short            count;                  //点位数量 最大255
    short            nEnable;                //是否有效//0-无效 1-部分有效 2-全部有效	
    int              index;              //步骤索引，不写入配置

    WORK_STEP()
    {
        memset(name, 0, ITEM_NAME_LEN);
        memset(description, 0, ITEM_NAME_LEN);
        count   = 0;
        nEnable   = 0;
        index = 0;
    }

    bool operator==(const WORK_STEP &other)
    {
        return (0 == strcmp(name, other.name)) && 
            (0 == strcmp(description, other.description)) && 
            count == other.count && 
            nEnable == other.nEnable &&
            index == other.index;
    }

	WORK_STEP& operator=(const WORK_STEP &other)
	{
		if (this == &other)
		{
			return *this;
		}

		strcpy(name, other.name);
		strcpy(description, other.description);
		count = other.count;
		nEnable = other.nEnable;
		index = other.index;
		return *this;
	}
};

struct WORK_FLOW
{//工作流程控制-流程信息
    char        name[ITEM_NAME_LEN];  //名字
    char	    description[ITEM_NAME_LEN];       //点位功能描述
    bool        bRunLoop;               //循环执行
    short       count;                  //步骤数量
    short       nEnable;                //是否有效//0-无效 1-部分有效 2-全部有效,用于执行全部流程时判断是否执行它
    int         index;              //流程索引，不写入配置

    WORK_FLOW()
    {
        memset(name, 0, ITEM_NAME_LEN);
        memset(description, 0, ITEM_NAME_LEN);
        count    = 0;
        nEnable   = 0;
        index = 0;
        bRunLoop = false;
    }

    bool operator==(const WORK_FLOW &other)
    {
        return (0 == strcmp(name, other.name)) && 
            (0 == strcmp(description, other.description)) && 
            count == other.count && 
            nEnable == other.nEnable &&
            index == other.index &&
            bRunLoop == other.bRunLoop;
    }

	WORK_FLOW& operator=(const WORK_FLOW &other)
	{
		if (this == &other)
		{
			return *this;
		}

		strcpy(name, other.name);
		strcpy(description, other.description);
		count = other.count;
		nEnable = other.nEnable;
		index = other.index;
		bRunLoop = other.bRunLoop;
		return *this;
	}
};

enum StatusType
{
	INVAILD_STATUS = 0,
	FLOW_STATUS = 3000,     //Flow 状态信息
	STEP_STATUS,            //Step 状态信息
	ACTION_STATUS,          //Action 状态信息
};

//  流程当前状态
typedef enum _MAINFLOW_STATUS
{	
	FLOW_STATUS_IDLE = 0x100,   // 未启动
	FLOW_STATUS_OK,
	FLOW_STATUS_FAILED, // 失败
	FLOW_STATUS_RUNING,
	FLOW_STATUS_PAUSE,
	FLOW_STATUS_STOP,
	FLOW_STATUS_ERROR
}E_FLOW_STATUS;
//
//  流程当前状态
typedef enum _STEP_STATUS
{	
	STEP_STATUS_IDLE = 0x100,   // 未启动
	STEP_STATUS_OK,
	STEP_STATUS_FAILED, // 失败
	STEP_STATUS_RUNING,
	STEP_STATUS_PAUSE,
	STEP_STATUS_STOP,
	STEP_STATUS_ERROR
}E_STEP_STATUS;


// 流程分组信息，用于UI新增Action
typedef struct _uiActionType
{
    string actionType;   // action类型，英文字母命名
    string actionUIname; // action类型，用于UI显示,可为中文
    string actionDesc;   // action类型描述，详细的描述信息

    _uiActionType()
    {
    }

    _uiActionType(string strType, string strUIName, string strDesc)
    {
        actionType = strType;
        actionUIname = strUIName;
        actionDesc = strDesc;
    }
}StUIActionType;

typedef struct _uiActionGroup
{
    string groupName;    // action分组的组名称
    string groupDesc;    // action分组的组描述
    vector<StUIActionType> vecActionType;
}StUIActionGroup;

typedef enum _statusBarItem   // 状态栏
{
    STATUSBAR_OPERATE,        // 操作
    STATUSBAR_STATUS,         // 状态
    STATUSBAR_MODE,           // 模式
    STATUSBAR_ACTION,         // 当前动作
    STATUSBAR_TIME,           // 总耗时
    STATUSBAR_FILE            // 当前配置文件
}EnStatusItem;

// 当前action索引，用于通知界面(mainflowui)执行到哪一个动作
typedef struct _curRunAction
{
    int workIndex;           // 流程索引
    int stepIndex;           // 步骤索引
    int actionIndex;         // 动作索引

    _curRunAction()
    {
        workIndex = -1;
        stepIndex = -1;
        actionIndex = -1;
    }

    _curRunAction(int wIndex, int sIndex, int aIndex)
    {
        workIndex = wIndex;
        stepIndex = sIndex;
        actionIndex = aIndex;
    }
}StCurRunAction;

// 流程/步骤状态，用于通知界面(mainflowui)当前流程/步骤状态
typedef struct _curRunStatus
{
    int workIndex;           // 流程索引
    int stepIndex;           // 步骤索引
    E_FLOW_STATUS runStatus;     // 流程执行状态

    _curRunStatus()
    {
        workIndex = -1;
        stepIndex = -1;
        runStatus = FLOW_STATUS_IDLE;
    }

    _curRunStatus(int wIndex, int sIndex, E_FLOW_STATUS status)
    {
        workIndex = wIndex;
        stepIndex = sIndex;
        runStatus = status;
    }
}StCurRunStatus;

//  流程运行的状态机状态
typedef enum _FLOW_STATE
{	
    // 操作
    FLOW_START,				// 流程启动
    FLOW_PAUSE,				// 流程暂停
    FLOW_CONTINUE,			// 流程继续
    FLOW_STOP,				// 流程停止
    FLOW_RESET,				// 流程复位

    // 动作
    FLOW_IDLE,			//  空闲
    FLOW_IS_PAUSE,  // 暂停状态	
    FLOW_FINISH,		// 完成
    FLOW_FAILED,       // 失败
    FLOW_ERROR,      // 出错	
    FLOW_NEXT_ACTION,	  // 下一步
    FLOW_NEXT_FAIL_ACTION,	//   执行错误跳转下一步
    FLOW_NEXT_SUCCEED_ACTION, // 执行成功跳转下一步
    FLOW_CHECK_ACTION_ENABLE,	// 判断当前这个步骤是否使能
    FLOW_RUN_ACTION,		// 执行该流程
    FLOW_ACTION_WAIT_BEFORE,  // 动作前等待
    FLOW_ACTION_WAIT_AFTER,   // 动作后等待
	FLOW_ACTION_SUCCEED_WAIT_AFTER,   // 动作成功后等待
	FLOW_ACTION_FAILED_WAIT_AFTER,   // 动作失败后等待
    FLOW_WAIT_ACTION_FINISH,	// action 执行完成
	FLOW_CHECK_HAS_ACTION_DOING, // 检测是否有action在运行,用于有异步动作时    
}E_FLOW_STATE;


#endif // WORKFLOWDEFINE
