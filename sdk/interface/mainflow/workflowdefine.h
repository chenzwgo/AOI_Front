#ifndef WORKFLOWDEFINE_H
#define WORKFLOWDEFINE_H

#include "rtdefine.h"
#include <string>
#include <vector>
using namespace std;

class WORK_ACTION;
#define ITEM_NAME_LEN  256             // ���̱༭�������ֳ���

//�������̿���-������Ϣ
struct WORK_STEP
{
    char             name[ITEM_NAME_LEN];    //����
    char			 description[ITEM_NAME_LEN];       //��λ��������
    short            count;                  //��λ���� ���255
    short            nEnable;                //�Ƿ���Ч//0-��Ч 1-������Ч 2-ȫ����Ч	
    int              index;              //������������д������

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
{//�������̿���-������Ϣ
    char        name[ITEM_NAME_LEN];  //����
    char	    description[ITEM_NAME_LEN];       //��λ��������
    bool        bRunLoop;               //ѭ��ִ��
    short       count;                  //��������
    short       nEnable;                //�Ƿ���Ч//0-��Ч 1-������Ч 2-ȫ����Ч,����ִ��ȫ������ʱ�ж��Ƿ�ִ����
    int         index;              //������������д������

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
	FLOW_STATUS = 3000,     //Flow ״̬��Ϣ
	STEP_STATUS,            //Step ״̬��Ϣ
	ACTION_STATUS,          //Action ״̬��Ϣ
};

//  ���̵�ǰ״̬
typedef enum _MAINFLOW_STATUS
{	
	FLOW_STATUS_IDLE = 0x100,   // δ����
	FLOW_STATUS_OK,
	FLOW_STATUS_FAILED, // ʧ��
	FLOW_STATUS_RUNING,
	FLOW_STATUS_PAUSE,
	FLOW_STATUS_STOP,
	FLOW_STATUS_ERROR
}E_FLOW_STATUS;
//
//  ���̵�ǰ״̬
typedef enum _STEP_STATUS
{	
	STEP_STATUS_IDLE = 0x100,   // δ����
	STEP_STATUS_OK,
	STEP_STATUS_FAILED, // ʧ��
	STEP_STATUS_RUNING,
	STEP_STATUS_PAUSE,
	STEP_STATUS_STOP,
	STEP_STATUS_ERROR
}E_STEP_STATUS;


// ���̷�����Ϣ������UI����Action
typedef struct _uiActionType
{
    string actionType;   // action���ͣ�Ӣ����ĸ����
    string actionUIname; // action���ͣ�����UI��ʾ,��Ϊ����
    string actionDesc;   // action������������ϸ��������Ϣ

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
    string groupName;    // action�����������
    string groupDesc;    // action�����������
    vector<StUIActionType> vecActionType;
}StUIActionGroup;

typedef enum _statusBarItem   // ״̬��
{
    STATUSBAR_OPERATE,        // ����
    STATUSBAR_STATUS,         // ״̬
    STATUSBAR_MODE,           // ģʽ
    STATUSBAR_ACTION,         // ��ǰ����
    STATUSBAR_TIME,           // �ܺ�ʱ
    STATUSBAR_FILE            // ��ǰ�����ļ�
}EnStatusItem;

// ��ǰaction����������֪ͨ����(mainflowui)ִ�е���һ������
typedef struct _curRunAction
{
    int workIndex;           // ��������
    int stepIndex;           // ��������
    int actionIndex;         // ��������

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

// ����/����״̬������֪ͨ����(mainflowui)��ǰ����/����״̬
typedef struct _curRunStatus
{
    int workIndex;           // ��������
    int stepIndex;           // ��������
    E_FLOW_STATUS runStatus;     // ����ִ��״̬

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

//  �������е�״̬��״̬
typedef enum _FLOW_STATE
{	
    // ����
    FLOW_START,				// ��������
    FLOW_PAUSE,				// ������ͣ
    FLOW_CONTINUE,			// ���̼���
    FLOW_STOP,				// ����ֹͣ
    FLOW_RESET,				// ���̸�λ

    // ����
    FLOW_IDLE,			//  ����
    FLOW_IS_PAUSE,  // ��ͣ״̬	
    FLOW_FINISH,		// ���
    FLOW_FAILED,       // ʧ��
    FLOW_ERROR,      // ����	
    FLOW_NEXT_ACTION,	  // ��һ��
    FLOW_NEXT_FAIL_ACTION,	//   ִ�д�����ת��һ��
    FLOW_NEXT_SUCCEED_ACTION, // ִ�гɹ���ת��һ��
    FLOW_CHECK_ACTION_ENABLE,	// �жϵ�ǰ��������Ƿ�ʹ��
    FLOW_RUN_ACTION,		// ִ�и�����
    FLOW_ACTION_WAIT_BEFORE,  // ����ǰ�ȴ�
    FLOW_ACTION_WAIT_AFTER,   // ������ȴ�
	FLOW_ACTION_SUCCEED_WAIT_AFTER,   // �����ɹ���ȴ�
	FLOW_ACTION_FAILED_WAIT_AFTER,   // ����ʧ�ܺ�ȴ�
    FLOW_WAIT_ACTION_FINISH,	// action ִ�����
	FLOW_CHECK_HAS_ACTION_DOING, // ����Ƿ���action������,�������첽����ʱ    
}E_FLOW_STATE;


#endif // WORKFLOWDEFINE
