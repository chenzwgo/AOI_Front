#ifndef _I_ROBOT_INTERFACE_H_
#define _I_ROBOT_INTERFACE_H_

#include <vector>
#include <string>
using namespace std;

struct RobotPoint;
struct RobotDegree;

enum ReturnType
{
	RET_TYPE_OK = 0,					// �ɹ�
	RET_TYPE_PARAM_ERR,					// ��δ���
	RET_TYPE_COMM_ERR,					// ͨѶ����
	RET_TYPE_PORT_CERATE_ERR,			// �˿ڴ���ʧ��
	RET_TYPE_CFG_READ_ERR,				// �����ļ���ȡʧ��
	RET_TYPE_CFG_WRITE_ERR,				// �����ļ�д��ʧ��
	RET_TYPE_SEND_ERROR,				// �����˷���ʧ��
	RET_TYPE_ACK_ERROR,					// ������Ӧ�����
	RET_TYPE_RUNNING,					// �����������˶���

};

typedef void (*FnRobotLog)(const char *, int);

class IRobot
{
public:
	enum Status {
		Unconnect,	// ������δ����
		Ready,		// �����˾���
		Running,	// �������˶���
		Pause,		// ��������ͣ
		Alarm		// �����˱���
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

	// �����˳�ʼ���ͷ���ʼ��
	virtual int init() = 0;

	virtual int uninit() = 0;

	// �����ж���˿ڵ���� �˿���Ŵ�0��ʼ���˿������Զ���
	virtual int connect(int port = 0) = 0;

	virtual int disconnect(int port = 0) = 0;

	virtual bool isConnect(int port = 0) = 0;

	virtual int sendData(int port, const char* buff, int len) = 0;

	virtual int recvData(int port, char* buff, int size, int timeOut = 2000) = 0;

	// ��ȡ�����õ�ǰ�ٶ� SpeedType: �����ٶ�����
	virtual int getSpeed(SpeedType ty, double &vel, double &acc, double &dec) = 0;

	virtual int setSpeed(SpeedType ty, double vel, double acc, double dec) = 0;

	// һ�����
	virtual int stop() = 0;

	virtual int pause() = 0;

	virtual int continues() = 0;

	virtual bool isRunning() = 0;

	virtual int home() = 0;

	virtual int isHomeFinish() = 0;

	virtual int setHomePos() = 0;

	// ��ȡ��ǰ����
	virtual int getCurrentPos(RobotPoint &robotPoint) = 0;

	// ��ȡ��ǰ�ؽ�����ֵ
	virtual int getCurrentDeg(RobotDegree &robotDegree) = 0;

	// ��ȡ��ǰ״̬
	virtual int getStatus() = 0;

	// ʹ��
	virtual int powerOn(bool bOn = true) = 0;

	// �ƶ�����λ
	// MoveType:   �����ƶ�����
	// index:      ��λ���
	// szName:     ��λ����
	// robotPoint: ����
	// param��     MoveType ��Ҫ�Ķ������
	virtual int moveToPoint(MoveType ty, int index, void *param = NULL) = 0;

	virtual int moveToPoint(MoveType ty, const char *szName, void *param = NULL) = 0;

	virtual int moveToPoint(MoveType ty, const RobotPoint &robotPoint, void *param = NULL) = 0;

	virtual int moveToPoint(const RobotDegree &robotDegree) = 0;

	// ���ؽ��ƶ�ƫ���� 
	virtual int moveOffset(AxisIndex axis, double offset) = 0;

	virtual int moveOffset(JointIndex joint, double offset) = 0;

	// ���ؽ������˶�
	virtual int moveAxis(AxisIndex axis, bool forward = true) = 0;

	virtual int moveJoint(JointIndex joint, bool forward = true) = 0;

	// ���õ�ǰ�Ĺ�������ϵ
	// index�� ����������� 0 ��ʾ��ʹ�ù�������ϵ
	virtual int setCurrentTool(int index) = 0;

	virtual int getCurrentTool() = 0;

	virtual RobotPoint getTool(int index) = 0;

	virtual int setTool(int index, const RobotPoint* pPoint) = 0;

	// �ڹ����������ƶ�
	virtual int moveToToolPoint(MoveType ty, int index) = 0;

	virtual int moveToToolPoint(MoveType ty, const char *szName) = 0;

	virtual int moveToToolPoint(MoveType ty, const RobotPoint &robotPoint) = 0;

	virtual int moveToolOffset(AxisIndex axis, double offset) = 0;

	virtual int moveToolAxis(AxisIndex axis) = 0;

	// IO����
	virtual const vector<string> * getInputs() = 0;

	virtual const vector<string> * getOutputs() = 0;

	virtual bool getInput(int bit) = 0;

	virtual bool getOutput(int bit) = 0;

	virtual bool setOutPut(int bit,bool bValue) = 0;

	// ���ûص�����ˢlog
	virtual void setLogFunction(FnRobotLog fnRobotLog) = 0;

};

#endif // _I_ROBOT_INTERFACE_H_