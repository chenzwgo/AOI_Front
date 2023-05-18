#pragma  once
#include "errcode_def.h"

// ϵͳ�ź�
typedef enum _SYS_STATE
{
	SYS_IDLE = 0,		     // �ȴ�״̬
	SYS_START,			     // ������	
	SYS_RESET,			     // ��λ
	SYS_ESTOP,			     // ��ͣ
	SYS_STOP,                // ֹͣ
	SYS_ESTOP_LIMITS,	     // ����λ����ͣ
	SYS_SAFE_DOOR,			 // ��ȫ�ſ���״̬
	SYS_DEVICE_POWER,		 // �豸�ϵ�
	SYS_PAUSE,				 // ��ͣ
	SYS_CONTINUE			 // ����
}E_SYS_STATE;

// ����״̬
typedef enum _MB_WORK_STATE
{
	MAIN_WORK_INIT,       // ��ʼ��
	MAIN_WORK_IDLE,       // ���У��ȴ���λ��
	MAIN_WORK_ESTOP,      // ��ͣ
	MAIN_WORK_SAFEDOOR,   // ��ȫ�ű���
	MAIN_WORK_POWEROFF,   // ����
	MAIN_WORK_RESET,      // ��λ
	MAIN_WORK_READY,      // ׼���ã��ȴ�������
	MAIN_WORK_START,      // ����
	MAIN_WORK_PAUSE,      // ��ͣ
	MAIN_WORK_STOP,       // ֹͣ(���ϵ�)
	MAIN_WORK_CONTINUE,   // ����
	MAIN_WORK_WORKING,     // ������
	MAIN_WORK_PAUSEING,    // ��ͣ��
	MAIN_WORK_RESETING,    // ��λ��
	MAIN_WORK_RESET_FAILED, // ��λʧ��
	MAIN_WORK_RESET_SUCCEED, // ��λ�ɹ�
	MAIN_WORK_FINISH,        // �������
	MAIN_WORK_ERROR,        // �����쳣
}E_MAIN_WORK_STATE;

//  �豸״̬
typedef enum _ROBOT_STATE
{
	ROBOT_ESTOP = 0,					// �����˼�ͣ״̬
	ROBOT_POWEROFF,						// ������δ�ϵ�
	ROBOT_ERR,							// �����˴���״̬
	ROBOT_NOREADY,						// ������û��׼��״̬
	ROBOT_OFFLINE,						// �������������
	ROBOT_RUNNING,						// ����������
}E_ROBOT_STATE;

typedef enum _DD_MOTOR_STATE
{
	MOTOR_NOTREADY = 0,			// ���No Ready ״̬
	MOTOR_ERROR,				// ������״̬
	MOTOR_HOMING,				// ����ԭ��
	MOTOR_DISENABLE,  			// ���û��ʹ��
	MOTOR_POWEROFF,				// û��ʹ��
	MOTOR_READY,				// 
}E_MOTOR_STATE;

typedef enum _CAMERA_STATE
{
	CAM_READY = 0,			// ���׼��״̬
	CAM_GET_IMAGE,			// ���������
	CAM_PROCESS,			// ���������
	CAM_PROCESS_FINISH,  	// ����������
	CAM_NOT_ONLINE,		    // ���������	
	CAM_POWER_OFF
}E_VISION_STATE;


//  ��վ�Ĺ���״̬
typedef enum _STATION_STATE
{	
	// StepStart()
	STATION_ALL_NONE,
	STATION_ALL_IDLE,	
	STATION_ALL_ERROR,
	STATION_ALL_FINISH,	
	STATION_ALL_STOP,
	STATION_ALL_RESET,
	STATION_ALL_PAUSE,
	STATION_ALL_CONTINUE,
	STATION_ALL_READY,
	STATION_ALL_START,
	// move
	STATION_DO_MOVE,
	STATION_DO_MOVE_FINISH,
	STATION_DO_VISION,
	STATION_DO_VISION_FINISH,
	STATION_DO_WAIT_IO,
	STATION_DO_HOMING,
	
}E_STATION_STATE;