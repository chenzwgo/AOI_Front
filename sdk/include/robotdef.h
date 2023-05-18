#ifndef _ROBOTDEF_H_
#define _ROBOTDEF_H_
//#include "motor_common.h"
#include <string>
using namespace  std;
#define DISCONN_TIME	5*1000	//断开连接刷新点数据时间
#define TIME_OUT_2S		2000	//超时2s
#define ROBOT_JOINT_4	4		//4轴机器人
#define ROBOT_JOINT_6	6		//6轴机器人
#define PT_CONTENT_FLAG ";"		//解析字符
#define PT_DATA_FLAG ","		//解析字符

#define SOCKET_TYPE_SERVER	1	//类型 服务器
#define SOCKET_TYPE_CLIENT  2	//类型 客户端

enum EM_GET_POINT_LIST_STATUS	//获取点列表信息状态
{
	EM_GET_SUCCESS = 0,			//获取信息成功
	EM_GET_END,					//获取信息结束
	EM_GET_ERROR,				//出错
};

struct RobotPoint
{
	string strName;
	string strDesc;
	double x;
	double y;
	double z;
	double u;
	double v;
	double w;

	RobotPoint()
	{
		x = y = z = u = v = w = 0.0;
	}

	RobotPoint(const RobotPoint &other)
	{
		*this = other;
	}

	RobotPoint& operator=(const RobotPoint &other)
	{
		if (this == &other)
		{
			return *this;
		}

		strName = other.strName;
		strDesc = other.strDesc;
		x       = other.x;
		y       = other.y;
		z       = other.z;
		u       = other.u;
		v       = other.v;
		w       = other.w;
		return *this;
	}

	bool operator==(const RobotPoint &other)
	{
        if ( x - other.x <= 0.00001 && x - other.x  >= -0.0001
            && y - other.y <= 0.00001 && y - other.y  >= -0.0001
            && z - other.z <= 0.00001 && z - other.z  >= -0.0001
            && u - other.u <= 0.00001 && u - other.u  >= -0.0001
            && v - other.v <= 0.00001 && v - other.v  >= -0.0001
            && w - other.w <= 0.00001 && w - other.w  >= -0.0001 )
		{
			return true;
		}
		return false;
	}
};

struct RobotDegree
{
	string strName;
	string strDesc;
	double j1;
	double j2;
	double j3;
	double j4;
	double j5;
	double j6;

	RobotDegree()
	{
		j1 = j2 = j3 = j4 = j5 = j6 = 0.0;
	}

	RobotDegree(const RobotDegree &other)
	{
		*this = other;
	}

	RobotDegree& operator=(const RobotDegree &other)
	{
		if (this == &other)
		{
			return *this;
		}

		strName = other.strName;
		strDesc = other.strDesc;
		j1      = other.j1;
		j2      = other.j2;
		j3      = other.j3;
		j4      = other.j4;
		j5      = other.j5;
		j6      = other.j6;
		return *this;
	}

	bool operator==(const RobotDegree &other)
	{
        if ( j1 - other.j1 <= 0.00001 && j1 - other.j1 >= -0.0001
            && j2 - other.j2 <= 0.00001 && j2 - other.j2 >= -0.0001
            && j3 - other.j3 <= 0.00001 && j3 - other.j3 >= -0.0001
            && j4 - other.j4 <= 0.00001 && j4 - other.j4 >= -0.0001
            && j5 - other.j5 <= 0.00001 && j5 - other.j5 >= -0.0001
            && j6 - other.j6 <= 0.00001 && j6 - other.j6 >= -0.0001 )
		{
			return true;
		}
		return false;
	}
};

struct RobotInfo
{
	string strRobotName;	// 工站名字
	string strPtFileName;	// 点文件名字
	string strIoFileName;	// IO文件名字
	string strDesc;		    // 工站描述
	int    iAxisType;		// 4轴或6轴
	int    iNetType;		// 本地的作为服务器或客服端
	string strLocalIp;
	string strLocalPort;	// 注意：当需要多个端口时，每个端口用“,”隔开
	string strRobotIp;
	string strRobotPort;    // 注意：当需要多个端口时，每个端口用“,”隔开

	RobotInfo()
	{
		iAxisType = ROBOT_JOINT_4;
		iNetType = SOCKET_TYPE_CLIENT;
		strLocalIp = "192.168.0.10";
		strLocalPort = "9000";
		strRobotIp = "192.168.0.11";
		strRobotPort = "9001";
	}

	RobotInfo(const RobotInfo &other)
	{
		*this = other;
	}

	RobotInfo &operator = (const RobotInfo &other)
	{
		if (this != &other)
		{
			strRobotName  = other.strRobotName;
			strPtFileName = other.strPtFileName;
			strIoFileName = other.strIoFileName;
			strDesc		  = other.strDesc;	 
			iAxisType 	  = other.iAxisType;	 
			iNetType 	  = other.iNetType;	 
			strLocalIp    = other.strLocalIp;	 
			strLocalPort  = other.strLocalPort;
			strRobotIp 	  = other.strRobotIp;	 
			strRobotPort  = other.strRobotPort;
		}
		return *this;
	}
};
/********************************程序与机器人通信协议****************************************/
//发送给机器人的命令
#define CMD_SET_MODEL				"SetModel"
#define CMD_SET_TOOL				"SetTool"
#define CMD_SET_POWER				"SetPower"
#define CMD_STOP					"Stop"
#define CMD_HOME					"Home" 
#define CMD_RESET					"Reset"

#define CMD_MOVE_TO_POINT			"MoveToPoint"
#define CMD_GO_TO_POINT			    "GoToPoint"
#define CMD_JUMP_TO_POINT			"JumpToPoint"//--------
#define CMD_MOVE_OFFSET				"MoveOffset"
#define CMD_GO_OFFSET				"GoOffset"
#define CMD_MOVE_OFFSET_BY_TOOL		"MoveOffsetByTool"
#define	CMD_GO_OFFSET_BY_TOOL		"GoOffsetByTool"
#define CMD_GO_POINT_BY_TOOL		"GoToPointByTool"
#define CMD_MOVE_POS				"MovePos"
#define CMD_AXIS_MOVE_POS			"AxisMovePos"
#define CMD_MOVE_ARC				"MoveArc"
#define CMD_MOVE_ARC3				"MoveArc3"
#define CMD_MOVE_LINE				"MoveLine"
#define CMD_ADD_CP_LINE_POINT		"AddCPLinePoint"
#define CMD_ADD_CP_ARC_POINT		"AddCPArcPoint"
#define CMD_CP_START_MOVE			"CPStartMove"
#define CMD_MOVE_ALWAYS				"MoveAlways"

#define CMD_SET_HOME_POS			"SetHomePos"
#define CMD_WAIT_HOME_FINISH		"HomeFinished" //------
#define CMD_HOME_JOINT				"HomeJoint"
#define CMD_WAIT_HOME_JOINT_FINISH	"HomeJointFinished"

#define CMD_SET_SPEED				"SetSpeed"
#define CMD_GET_SPEED				"GetSpeed"
#define CMD_CREATE_TOOL				"CreateTool"
#define CMD_MODIFY_TOOL_VALUE		"ModifyToolValue"
#define CMD_GET_POS					"GetPos"
#define CMD_TEACH_POINT				"TeachPoint"
#define CMD_SAVE_POINT				"SavePoint"
#define CMD_SET_POINT				"SetPoint"
#define CMD_DEL_POINT				"DelPoint"
#define CMD_GET_POINT				"GetPoint"
#define CMD_SET_MOVE_FLAG			"SetMoveFlag"
#define CMD_GET_MOVE_FLAG			"GetMoveFlag"

#define CMD_GET_POINT_LIST			"GetPointInfoList"

#define CMD_IS_MOVING				"IsMoving"
#define CMD_IS_READY				"IsReady"
#define CMD_IS_NEAR_POINT			"IsNearPoint"

#define CMD_CCD_CALIB				"CCDCalib"
#define CMD_GO_TO_CCD_POINT			"GoToCCDPoint"
#define CMD_GET_POINT_BY_CALIB		"GetPointByCalib"
#define CMD_GET_ANGLE_BY_LINE		"GetAngleByLine"
#define CMD_CREAT_PALLET			"CreatPallet"
#define CMD_GO_TO_PALLET_POS		"GoToPalletPos"
#define CMD_GET_PALLET_POS			"GetPalletPos"


//机器人返回消息
#define MSG_OK         "ok"            //返回成功ST_
#define MSG_ERROR      "error"         //返回错误提示
#define MSG_RUNNING    "running"       //运动中
#define MSG_IDE        "ide"           //空闲
#define MSG_FINISH     "finish"        //返回的结束
#define MSG_END        "end"

//机器人IO信号
#define HOME_IO         "do_robot_home"			//io控制-回原
#define START_IO        "do_robot_start"		//io控制-启动
#define PROG_RESET_IO   "do_robot_program_reset"//io控制-程序复位
#define ALARM_RESET_IO  "do_robot_alarm_reset"	//io控制-报警复位
#define STOP_IO         "do_robot_stop"			//io控制-停止
#define PAUSE_IO        "do_robot_pause"		//io控制-暂停
#define PROG_START_IO   "do_robot_program_start"//io控制-程序复位

#define OPERATOR_IO     "do_robot_operator"		//io控制-操作权(自动模式/手动模式)
#define CTRL_IO			"do_robot_ctrl"			//io控制-控制权(示教模式)
#define POWER_ON_IO     "do_robot_power_on"		//io控制-打开电源
#define POWER_OFF_IO    "do_robot_power_off"	//io控制-关闭电源

#define HOME_FINISH_IO  "di_robot_home_finish"  //io获取原点信号
#define READY_IO        "di_robot_ready"        //io获取robot准备完成
#define ERROR_IO        "di_robot_error"        //io获取robot异常报警
#define IN_PAUSE_IO     "di_robot_pause"		//暂停信号
#define IN_ALARM_IO     "di_robot_alarm"		//报警信号
#define IN_POWER_ON     "di_robot_power_on"		//电源打开信号
#define IN_POWER_OFF    "di_robot_power_off"	//报警关闭信号
#define IN_RUNNING     "di_robot_running"		//电源打开信号


#endif
