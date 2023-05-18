#ifndef _ROBOTDEF_H_
#define _ROBOTDEF_H_
//#include "motor_common.h"
#include <string>
using namespace  std;
#define DISCONN_TIME	5*1000	//�Ͽ�����ˢ�µ�����ʱ��
#define TIME_OUT_2S		2000	//��ʱ2s
#define ROBOT_JOINT_4	4		//4�������
#define ROBOT_JOINT_6	6		//6�������
#define PT_CONTENT_FLAG ";"		//�����ַ�
#define PT_DATA_FLAG ","		//�����ַ�

#define SOCKET_TYPE_SERVER	1	//���� ������
#define SOCKET_TYPE_CLIENT  2	//���� �ͻ���

enum EM_GET_POINT_LIST_STATUS	//��ȡ���б���Ϣ״̬
{
	EM_GET_SUCCESS = 0,			//��ȡ��Ϣ�ɹ�
	EM_GET_END,					//��ȡ��Ϣ����
	EM_GET_ERROR,				//����
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
	string strRobotName;	// ��վ����
	string strPtFileName;	// ���ļ�����
	string strIoFileName;	// IO�ļ�����
	string strDesc;		    // ��վ����
	int    iAxisType;		// 4���6��
	int    iNetType;		// ���ص���Ϊ��������ͷ���
	string strLocalIp;
	string strLocalPort;	// ע�⣺����Ҫ����˿�ʱ��ÿ���˿��á�,������
	string strRobotIp;
	string strRobotPort;    // ע�⣺����Ҫ����˿�ʱ��ÿ���˿��á�,������

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
/********************************�����������ͨ��Э��****************************************/
//���͸������˵�����
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


//�����˷�����Ϣ
#define MSG_OK         "ok"            //���سɹ�ST_
#define MSG_ERROR      "error"         //���ش�����ʾ
#define MSG_RUNNING    "running"       //�˶���
#define MSG_IDE        "ide"           //����
#define MSG_FINISH     "finish"        //���صĽ���
#define MSG_END        "end"

//������IO�ź�
#define HOME_IO         "do_robot_home"			//io����-��ԭ
#define START_IO        "do_robot_start"		//io����-����
#define PROG_RESET_IO   "do_robot_program_reset"//io����-����λ
#define ALARM_RESET_IO  "do_robot_alarm_reset"	//io����-������λ
#define STOP_IO         "do_robot_stop"			//io����-ֹͣ
#define PAUSE_IO        "do_robot_pause"		//io����-��ͣ
#define PROG_START_IO   "do_robot_program_start"//io����-����λ

#define OPERATOR_IO     "do_robot_operator"		//io����-����Ȩ(�Զ�ģʽ/�ֶ�ģʽ)
#define CTRL_IO			"do_robot_ctrl"			//io����-����Ȩ(ʾ��ģʽ)
#define POWER_ON_IO     "do_robot_power_on"		//io����-�򿪵�Դ
#define POWER_OFF_IO    "do_robot_power_off"	//io����-�رյ�Դ

#define HOME_FINISH_IO  "di_robot_home_finish"  //io��ȡԭ���ź�
#define READY_IO        "di_robot_ready"        //io��ȡrobot׼�����
#define ERROR_IO        "di_robot_error"        //io��ȡrobot�쳣����
#define IN_PAUSE_IO     "di_robot_pause"		//��ͣ�ź�
#define IN_ALARM_IO     "di_robot_alarm"		//�����ź�
#define IN_POWER_ON     "di_robot_power_on"		//��Դ���ź�
#define IN_POWER_OFF    "di_robot_power_off"	//�����ر��ź�
#define IN_RUNNING     "di_robot_running"		//��Դ���ź�


#endif
