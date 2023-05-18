#ifndef I_ROBOT_STATIONMANAGE_H
#define I_ROBOT_STATIONMANAGE_H

#include <vector>
class IRobot;
struct RobotInfo;

class IRobotManage
{
public:
	enum
	{
		IDD_ROBOT_STATION_MANAGE = 0x88
	};
	virtual ~IRobotManage(){}

	virtual IRobot * getRobotInstance(const char *szRobotName) = 0;

	virtual void releaseRobotInstance(const char *szRobotName) = 0;

	virtual const std::vector<RobotInfo*> & getAllRobotInfo() const = 0;

	virtual const RobotInfo * getRobotInfo(const char *szRobotName) const = 0;

	virtual bool setRobotInfo(const char *szRobotName, const RobotInfo *pRobotInfo) = 0;

	virtual bool insertRobotInfo(int index, const RobotInfo * pRobotInfo) = 0;

	virtual void removeRobotInfo(const char *szRobotName) = 0;

	virtual void robotInfoMove(const char *szRobotName, int index) = 0;
};

#endif // ISTATIONMANAGE_H
