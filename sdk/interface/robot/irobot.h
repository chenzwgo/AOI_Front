#ifndef _I_ROBOT_INTERFACE_H_
#define _I_ROBOT_INTERFACE_H_

#include <vector>
#include <string>
using namespace std;

struct RobotPoint;
struct RobotDegree;

enum ReturnType
{
	RET_TYPE_OK = 0,					// 成功
	RET_TYPE_PARAM_ERR,					// 入参错误
	RET_TYPE_COMM_ERR,					// 通讯错误
	RET_TYPE_PORT_CERATE_ERR,			// 端口创建失败
	RET_TYPE_CFG_READ_ERR,				// 配置文件读取失败
	RET_TYPE_CFG_WRITE_ERR,				// 配置文件写入失败
	RET_TYPE_SEND_ERROR,				// 机器人发送失败
	RET_TYPE_ACK_ERROR,					// 机器人应答错误
	RET_TYPE_RUNNING,					// 机器人正在运动中

};

typedef void (*FnRobotLog)(const char *, int);

class IRobot
{
public:
	enum Status {
		Unconnect,	// 机器人未连接
		Ready,		// 机器人就绪
		Running,	// 机器人运动中
		Pause,		// 机器人暂停
		Alarm		// 机器人报警
	};

	enum AxisIndex{
		AxisX, AxisY, AxisZ, AxisU, AxisV, AxisW
	};

	enum JointIndex{
		Joint1, Joint2, Joint3, Joint4, Joint5, Joint6
	};

	enum SpeedType {
		SpeedDefault = 0,
		PTPSpeed,
		LineSpeed,
	};

	enum MoveType {
		MoveDefault = 1,
		MoveLine = MoveDefault,
		MovePTP,
		MoveJump,
		MoveDeg,
	};

	enum MessageType
	{
		MsgInfo = 0,
		MsgWarn,
		MsgError,
		MsgSend,
		MsgRecv
	};
    virtual ~IRobot(){}

	virtual const std::vector<RobotPoint*> & getAllPoint() = 0;

	virtual const RobotPoint * getPoint(const char *szPointName) = 0;

	virtual bool setPoint(const char *szPointName, const RobotPoint *pPoint) = 0;

	virtual bool insertPoint(int index, const RobotPoint * pPoint) = 0;

	virtual void removePoint(const char *szPointName) = 0;

	virtual void pointMove(const char *szPointName, int index) = 0;

	virtual bool readPointsCfg() = 0;

	virtual bool writePointsCfg() = 0;

	virtual const RobotInfo * getRobotInfo() = 0;

	virtual void setRobotInfo(const RobotInfo *pRobotInfo) = 0;

	virtual string getErrorDesc(int iError) = 0;

	// 机器人初始化和反初始化
	virtual int init() = 0;

	virtual int uninit() = 0;

	// 可能有多个端口的情况 端口序号从0开始，端口意义自定义
	virtual int connect(int port = 0) = 0;

	virtual int disconnect(int port = 0) = 0;

	virtual bool isConnect(int port = 0) = 0;

	virtual int sendData(int port, const char* buff, int len) = 0;

	virtual int recvData(int port, char* buff, int size, int timeOut = 2000) = 0;

	// 获取和设置当前速度 SpeedType: 各种速度类型
	virtual int getSpeed(SpeedType ty, double &vel, double &acc, double &dec) = 0;

	virtual int setSpeed(SpeedType ty, double vel, double acc, double dec) = 0;

	// 一般操作
	virtual int stop() = 0;

	virtual int pause() = 0;

	virtual int continues() = 0;

	virtual bool isRunning() = 0;

	virtual int home() = 0;

	virtual int isHomeFinish() = 0;

	virtual int setHomePos() = 0;

	// 获取当前坐标
	virtual int getCurrentPos(RobotPoint &robotPoint) = 0;

	// 获取当前关节脉冲值
	virtual int getCurrentDeg(RobotDegree &robotDegree) = 0;

	// 获取当前状态
	virtual int getStatus() = 0;

	// 使能
	virtual int powerOn(bool bOn = true) = 0;

	// 移动到点位
	// MoveType:   各种移动类型
	// index:      点位序号
	// szName:     点位名称
	// robotPoint: 坐标
	// param：     MoveType 需要的额外参数
	virtual int moveToPoint(MoveType ty, int index, void *param = NULL) = 0;

	virtual int moveToPoint(MoveType ty, const char *szName, void *param = NULL) = 0;

	virtual int moveToPoint(MoveType ty, const RobotPoint &robotPoint, void *param = NULL) = 0;

	virtual int moveToPoint(const RobotDegree &robotDegree) = 0;

	// 轴或关节移动偏移量 
	virtual int moveOffset(AxisIndex axis, double offset) = 0;

	virtual int moveOffset(JointIndex joint, double offset) = 0;

	// 轴或关节连续运动
	virtual int moveAxis(AxisIndex axis, bool forward = true) = 0;

	virtual int moveJoint(JointIndex joint, bool forward = true) = 0;

	// 设置当前的工具坐标系
	// index： 工具坐标序号 0 表示不使用工具坐标系
	virtual int setCurrentTool(int index) = 0;

	virtual int getCurrentTool() = 0;

	virtual RobotPoint getTool(int index) = 0;

	virtual int setTool(int index, const RobotPoint* pPoint) = 0;

	// 在工具坐标中移动
	virtual int moveToToolPoint(MoveType ty, int index) = 0;

	virtual int moveToToolPoint(MoveType ty, const char *szName) = 0;

	virtual int moveToToolPoint(MoveType ty, const RobotPoint &robotPoint) = 0;

	virtual int moveToolOffset(AxisIndex axis, double offset) = 0;

	virtual int moveToolAxis(AxisIndex axis) = 0;

	// IO操作
	virtual const vector<string> * getInputs() = 0;

	virtual const vector<string> * getOutputs() = 0;

	virtual bool getInput(int bit) = 0;

	virtual bool getOutput(int bit) = 0;

	virtual bool setOutPut(int bit,bool bValue) = 0;

	// 设置回调函数刷log
	virtual void setLogFunction(FnRobotLog fnRobotLog) = 0;

};

#endif // _I_ROBOT_INTERFACE_H_