#pragma  once
#include "errcode_def.h"

// 系统信号
typedef enum _SYS_STATE
{
	SYS_IDLE = 0,		     // 等待状态
	SYS_START,			     // 单启动	
	SYS_RESET,			     // 复位
	SYS_ESTOP,			     // 急停
	SYS_STOP,                // 停止
	SYS_ESTOP_LIMITS,	     // 因负限位而急停
	SYS_SAFE_DOOR,			 // 安全门开启状态
	SYS_DEVICE_POWER,		 // 设备断电
	SYS_PAUSE,				 // 暂停
	SYS_CONTINUE			 // 继续
}E_SYS_STATE;

// 主控状态
typedef enum _MB_WORK_STATE
{
	MAIN_WORK_INIT,       // 初始化
	MAIN_WORK_IDLE,       // 空闲（等待复位）
	MAIN_WORK_ESTOP,      // 急停
	MAIN_WORK_SAFEDOOR,   // 安全门被打开
	MAIN_WORK_POWEROFF,   // 掉电
	MAIN_WORK_RESET,      // 复位
	MAIN_WORK_READY,      // 准备好（等待启动）
	MAIN_WORK_START,      // 启动
	MAIN_WORK_PAUSE,      // 暂停
	MAIN_WORK_STOP,       // 停止(不断电)
	MAIN_WORK_CONTINUE,   // 继续
	MAIN_WORK_WORKING,     // 工作中
	MAIN_WORK_PAUSEING,    // 暂停中
	MAIN_WORK_RESETING,    // 复位中
	MAIN_WORK_RESET_FAILED, // 复位失败
	MAIN_WORK_RESET_SUCCEED, // 复位成功
	MAIN_WORK_FINISH,        // 工作完成
	MAIN_WORK_ERROR,        // 出现异常
}E_MAIN_WORK_STATE;

//  设备状态
typedef enum _ROBOT_STATE
{
	ROBOT_ESTOP = 0,					// 机器人急停状态
	ROBOT_POWEROFF,						// 机器人未上电
	ROBOT_ERR,							// 机器人错误状态
	ROBOT_NOREADY,						// 机器人没有准备状态
	ROBOT_OFFLINE,						// 机器人网络掉线
	ROBOT_RUNNING,						// 机器在运行
}E_ROBOT_STATE;

typedef enum _DD_MOTOR_STATE
{
	MOTOR_NOTREADY = 0,			// 马达No Ready 状态
	MOTOR_ERROR,				// 马达错误状态
	MOTOR_HOMING,				// 马达回原中
	MOTOR_DISENABLE,  			// 马达没有使能
	MOTOR_POWEROFF,				// 没有使能
	MOTOR_READY,				// 
}E_MOTOR_STATE;

typedef enum _CAMERA_STATE
{
	CAM_READY = 0,			// 相机准备状态
	CAM_GET_IMAGE,			// 相机拍照中
	CAM_PROCESS,			// 相机处理中
	CAM_PROCESS_FINISH,  	// 相机处理完成
	CAM_NOT_ONLINE,		    // 相机不在线	
	CAM_POWER_OFF
}E_VISION_STATE;


//  工站的工作状态
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