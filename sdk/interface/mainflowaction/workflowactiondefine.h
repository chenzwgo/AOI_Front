#ifndef WORKFLOWACTIONDEFINE_H
#define WORKFLOWACTIONDEFINE_H

#include "rtdefine.h"
#include "actioncfgdefine.h"
#include "mainflowaction/workflowoperator.h"

#define ITEM_NAME_LEN  256             // ���̱༭�������ֳ���

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

#define JUMP_FAILED  "Failed"   // ����ʧ��
#define JUMP_NONE  "None"   // ����ת
#define JUMP_END   "End"    // ����
#define JUMP_PAUSE "Pause"  // ��ͣ

struct WORK_ACTION_LOG
{
    // ������log,��log��ϢΪ��ʱ�������ӡĬ��log
    bool bLogStart;         // ���������Ƿ��ӡUI LOG
    bool bLogStartFailed;   // ����ʧ���Ƿ��ӡUI LOG
    bool bLogSucceed;       // ִ�гɹ��Ƿ��ӡUI LOG
    bool bLogFailed;        // ִ��ʧ���Ƿ��ӡUI LOG
    bool bLogTimeOut;       // ִ�г�ʱ�Ƿ��ӡUI LOG
    char logStart[ITEM_NAME_LEN];        // ����log
    char logStartFailed[ITEM_NAME_LEN];  // ����ʧ��log
    char logSucceed[ITEM_NAME_LEN];      // ִ�гɹ�log
    char logFailed[ITEM_NAME_LEN];       // ִ��ʧ��log
    char logTimeOut[ITEM_NAME_LEN];      // ִ�г�ʱlog
    int  logLevelStart;                 // ����log�ļ���
    int  logLevelStartFailed;           // ����ʧ��log�ļ���
    int  logLevelSucceed;               // ִ�гɹ�log�ļ���
    int  logLevelFailed;                // ִ��ʧ��log�ļ���
    int  logLevelTimeOut;               // ִ�г�ʱlog�ļ���

    
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
    char            name[ITEM_NAME_LEN];		//�����¶������� --item
    char            description[ITEM_NAME_LEN];       //��λ��������
    bool            bEnable;                     //��־�Ƿ����� 0 - �����ã� 1 - ����
    bool            bBlock;                      //�Ƿ�ͬ����������trueΪ�ȴ�ִ�����,falseΪ�첽���ȴ�
    char			type[ITEM_NAME_LEN];         //���� 0 WORK_POINT �� 1 - WORK_IO �� 2 -WORK_FUNCTION 3 - WORK_INTERPOLATION..
    unsigned short  nBeforeWait;             //����ǰʱ����ʱ
    unsigned short  nAfterWait;			     //������ʱ����ʱ
    int             index;                   //������������д������
    char            jumpStepWhenFailed[ITEM_NAME_LEN];         //ִ��ʧ�ܺ���ת���Ĳ���
    char            jumpActionWhenFailed[ITEM_NAME_LEN];       //ִ��ʧ�ܺ���ת���Ķ���
    char            jumpStepWhenSucceed[ITEM_NAME_LEN];        //ִ�гɹ�����ת���Ĳ���
    char            jumpActionWhenSucceed[ITEM_NAME_LEN];      //ִ�гɹ�����ת���Ķ���
    int             timeout;                                   //��ʱʱ��
    WORK_ACTION_LOG actiongLog;   // ִ�ж�����log��Ϣ
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

//  ����ִ��״̬
typedef enum _ACTION_STATUS
{	
	ACTION_STATUS_EXCUTE_ERROR = 0x200,	// ִ��ʧ��
	ACTION_STATUS_INIT,     // ��ʼ״̬
	ACTION_STATUS_RUNING,   // ����ִ����
	ACTION_STATUS_RESULT_OK,       // �������OK(��IO,�����ж�,��״̬�ж�),UI��ʾ"True"
	ACTION_STATUS_RESULT_FAILED,   // �������ʧ��(��IO,�����ж�,��״̬�ж�),UI��ʾ"False",�����
	ACTION_STATUS_PAUSE,    // ������ͣ
    ACTION_STATUS_STOP,     // ����ֹͣ
	ACTION_STATUS_TIME_OUT   // ����ִ�г�ʱ
}E_ACTION_STATUS;

#endif // WORKFLOWACTIONDEFINE
