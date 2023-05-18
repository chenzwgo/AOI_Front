#ifndef WORKFLOWACTIONDEFINE_H
#define WORKFLOWACTIONDEFINE_H

#include "rtdefine.h"
#include "actioncfgdefine.h"
#include "mainflowaction/workflowoperator.h"

#define ITEM_NAME_LEN  256             // 流程编辑各项名字长度

#define ACTION_MOTOR_PTP_ABS			"move_ptp_abs"
#define ACTION_MOTOR_PTP_REL			"move_ptp_rel"
#define ACTION_MOTOR_LINE_ABS			"move_line_abs"
#define	ACTION_MOTOR_LINE_REL 			"move_line_rel"
#define	ACTION_MOTOR_ARC				"move_arc"	
#define	ACTION_MOTOR_INTERPOLATION		"move_interp"
#define	ACTION_MOTOR_STOP				"move_stop"	
#define	ACTION_MOTOR_JOG				"move_jog"
#define	ACTION_MOTOR_HOME				"move_home"
#define ACTION_MOTOR_POINT_NEAR			"move_point_near"
#define ACTION_MOTOR_AXIS_STATUS		"move_axisstatus"
#define ACTION_MOTOR_AXISMOVE			"move_axis"	
#define ACTION_MOTOR_WAIT_STOP			"move_wait_stop"	
#define ACTION_MOTOR_TEACH_POINT		"move_teach_point"

#define	ACTION_IO_OUTPUT				"io_output"		
#define	ACTION_IO_WAITIN				"io_waitin"	
#define	ACTION_IO_CYLINDER				"io_cylinder"	
#define	ACTION_IO_READ					"io_read"
#define	ACTION_FUN_FUNCTION				"fun"	
#define	ACTION_EVENT_SET_EVENT			"event_set"
#define	ACTION_EVENT_WAIT_EVENT			"event_wait"
#define	ACTION_VISION_JOB				"vision_job"	
#define	ACTION_SCRIPT_JOB				"script_job"		

#define	ACTION_VARIABLE_READ			"variable_read"	
#define	ACTION_VARIABLE_WRITE			"variable_write"

#define ACTION_JUMP                     "jump_action"
#define ACTION_WAIT_CONDITION           "wait_condition"

#define ACTION_DEV_COM                  "dev_com"
#define ACTION_DEV_TCP                  "dev_tcp"
#define ACTION_DEV_UDP                  "dev_udp"

#define	ACTION_LOG                      "log"
#define ACTION_COUNT                    "count"

#define ACTION_FILE_INI                 "file_ini"
#define ACTION_FILE_CSV                 "file_csv"

#define ACTION_POINT_MODIFY             "point_modify"

#define ACTION_PARSE_STRING             "parse_string"

#define JUMP_FAILED  "Failed"   // 流程失败
#define JUMP_NONE  "None"   // 不跳转
#define JUMP_END   "End"    // 结束
#define JUMP_PAUSE "Pause"  // 暂停

struct WORK_ACTION_LOG
{
    // 启用了log,但log信息为空时，将会打印默认log
    bool bLogStart;         // 启动动作是否打印UI LOG
    bool bLogStartFailed;   // 启动失败是否打印UI LOG
    bool bLogSucceed;       // 执行成功是否打印UI LOG
    bool bLogFailed;        // 执行失败是否打印UI LOG
    bool bLogTimeOut;       // 执行超时是否打印UI LOG
    char logStart[ITEM_NAME_LEN];        // 启动log
    char logStartFailed[ITEM_NAME_LEN];  // 启动失败log
    char logSucceed[ITEM_NAME_LEN];      // 执行成功log
    char logFailed[ITEM_NAME_LEN];       // 执行失败log
    char logTimeOut[ITEM_NAME_LEN];      // 执行超时log
    int  logLevelStart;                 // 启动log的级别
    int  logLevelStartFailed;           // 启动失败log的级别
    int  logLevelSucceed;               // 执行成功log的级别
    int  logLevelFailed;                // 执行失败log的级别
    int  logLevelTimeOut;               // 执行超时log的级别

    
    WORK_ACTION_LOG()
    {
        bLogStart = false;
        bLogStartFailed = true;
        bLogSucceed = false;
        bLogFailed = true;
        bLogTimeOut = true;
        memset(logStart, 0, sizeof(logStart));
        memset(logStartFailed, 0, sizeof(logStartFailed));
        memset(logSucceed, 0, sizeof(logSucceed));
        memset(logFailed, 0, sizeof(logFailed));
        memset(logTimeOut, 0, sizeof(logTimeOut));
        logLevelStart = 0;
        logLevelStartFailed = 4;
        logLevelSucceed = 0;
        logLevelFailed = 2;
        logLevelTimeOut = 2;
    }

    WORK_ACTION_LOG& operator=(const WORK_ACTION_LOG &other)
    {
        if (this == &other)
        {
            return *this;
        }

        bLogStart       = other.bLogStart      ;
        bLogStartFailed = other.bLogStartFailed;
        bLogSucceed     = other.bLogSucceed    ;
        bLogFailed      = other.bLogFailed     ;
        bLogTimeOut     = other.bLogTimeOut    ;
        strcpy(logStart, other.logStart);
        strcpy(logStartFailed, other.logStartFailed);
        strcpy(logSucceed, other.logSucceed);
        strcpy(logFailed, other.logFailed);
        strcpy(logTimeOut, other.logTimeOut);
        logLevelStart = other.logLevelStart;
        logLevelStartFailed = other.logLevelStartFailed;
        logLevelSucceed = other.logLevelSucceed;
        logLevelFailed = other.logLevelFailed;
        logLevelTimeOut = other.logLevelTimeOut;
        return *this;
    }

    bool operator==(const WORK_ACTION_LOG &other)
    {
        return (bLogStart       == other.bLogStart
            && bLogStartFailed == other.bLogStartFailed
            && bLogSucceed     == other.bLogSucceed
            && bLogFailed      == other.bLogFailed
            && bLogTimeOut     == other.bLogTimeOut
            && 0 == strcmp(logStart, other.logStart)
            && 0 == strcmp(logStartFailed, other.logStartFailed)
            && 0 == strcmp(logSucceed, other.logSucceed)
            && 0 == strcmp(logFailed, other.logFailed)
            && 0 == strcmp(logTimeOut, other.logTimeOut)
            && logLevelStart == other.logLevelStart
            && logLevelStartFailed == other.logLevelStartFailed
            && logLevelSucceed == other.logLevelSucceed
            && logLevelFailed == other.logLevelFailed
            && logLevelTimeOut == other.logLevelTimeOut);
    }
};

struct WORK_ACTION
{
    char            name[ITEM_NAME_LEN];		//步骤下动作名称 --item
    char            description[ITEM_NAME_LEN];       //点位功能描述
    bool            bEnable;                     //标志是否启用 0 - 不启用； 1 - 启用
    bool            bBlock;                      //是否同步，阻塞，true为等待执行完成,false为异步不等待
    char			type[ITEM_NAME_LEN];         //类型 0 WORK_POINT ； 1 - WORK_IO ； 2 -WORK_FUNCTION 3 - WORK_INTERPOLATION..
    unsigned short  nBeforeWait;             //操作前时间延时
    unsigned short  nAfterWait;			     //操作后时间延时
    int             index;                   //动作索引，不写入配置
    char            jumpStepWhenFailed[ITEM_NAME_LEN];         //执行失败后跳转到的步骤
    char            jumpActionWhenFailed[ITEM_NAME_LEN];       //执行失败后跳转到的动作
    char            jumpStepWhenSucceed[ITEM_NAME_LEN];        //执行成功后跳转到的步骤
    char            jumpActionWhenSucceed[ITEM_NAME_LEN];      //执行成功后跳转到的动作
    int             timeout;                                   //超时时间
    WORK_ACTION_LOG actiongLog;   // 执行动作的log信息
    ACTION_PARAM * pParamData;

    WORK_ACTION()
    {
        memset(name, 0, ITEM_NAME_LEN);      
        memset(description, 0, ITEM_NAME_LEN); 
		memset(type, 0, ITEM_NAME_LEN); 
        memset(jumpStepWhenFailed, 0, ITEM_NAME_LEN);
        memset(jumpActionWhenFailed, 0, ITEM_NAME_LEN);
        memset(jumpStepWhenSucceed, 0, ITEM_NAME_LEN);
        memset(jumpActionWhenSucceed, 0, ITEM_NAME_LEN);
        strcpy(jumpStepWhenFailed, JUMP_NONE);
        strcpy(jumpStepWhenSucceed, JUMP_NONE);
        strcpy(jumpActionWhenFailed, JUMP_NONE);
        strcpy(jumpActionWhenSucceed, JUMP_NONE);
        bEnable = false;
        bBlock = true;
		pParamData = NULL;
        nBeforeWait  = 0;
        nAfterWait = 0;
        index = 0;
        timeout = -1;
    }

    void clear()
    {
		SAFE_DELETE(pParamData); 
	}

    bool operator==(const WORK_ACTION &other)
    {
        return (0 == strcmp(name,other.name)) && 
            (0 == strcmp(description,other.description)) && 
            (0 == strcmp(jumpActionWhenFailed,other.jumpActionWhenFailed)) && 
            (0 == strcmp(jumpActionWhenSucceed,other.jumpActionWhenSucceed)) && 
            (0 == strcmp(jumpStepWhenFailed,other.jumpStepWhenFailed)) && 
            (0 == strcmp(jumpStepWhenSucceed,other.jumpStepWhenSucceed)) && 
            (0 == strcmp(type,other.type)) &&
            bEnable == other.bEnable &&
            nBeforeWait == other.nBeforeWait &&
            nAfterWait == other.nAfterWait &&
            index == other.index &&
            timeout == other.timeout &&
			bBlock == other.bBlock &&
            actiongLog == other.actiongLog;
    }

	WORK_ACTION& operator=(const WORK_ACTION &other)
	{
		if (this == &other)
		{
			return *this;
		}

		strcpy(name,other.name);
		strcpy(description,other.description);
		strcpy(jumpActionWhenFailed,other.jumpActionWhenFailed);
		strcpy(jumpActionWhenSucceed,other.jumpActionWhenSucceed);
		strcpy(jumpStepWhenFailed,other.jumpStepWhenFailed);
		strcpy(jumpStepWhenSucceed,other.jumpStepWhenSucceed);
		strcpy(type,other.type);
		bEnable = other.bEnable;
		bBlock = other.bBlock;
		nBeforeWait = other.nBeforeWait;
		nAfterWait = other.nAfterWait;
		index = other.index;
		timeout = other.timeout;
		actiongLog = other.actiongLog;
		pParamData = pParamData;
		return *this;
	}
};

//  动作执行状态
typedef enum _ACTION_STATUS
{	
	ACTION_STATUS_EXCUTE_ERROR = 0x200,	// 执行失败
	ACTION_STATUS_INIT,     // 初始状态
	ACTION_STATUS_RUNING,   // 动作执行中
	ACTION_STATUS_RESULT_OK,       // 动作结果OK(读IO,变量判断,轴状态判断),UI显示"True"
	ACTION_STATUS_RESULT_FAILED,   // 动作结果失败(读IO,变量判断,轴状态判断),UI显示"False",不标红
	ACTION_STATUS_PAUSE,    // 动作暂停
    ACTION_STATUS_STOP,     // 动作停止
	ACTION_STATUS_TIME_OUT   // 动作执行超时
}E_ACTION_STATUS;

#endif // WORKFLOWACTIONDEFINE
