#ifndef I_STATIONMANAGE_H
#define I_STATIONMANAGE_H

class IStation;
class IStationManage
{
public:
    enum
	{
		IDD_STATION_MANAGE = 0x89
	};
	
	virtual ~IStationManage(){}
	virtual IStation*   getStationInstance(const char *psrtStationName) = 0;
	virtual void ReleaseStation(const char *psrtStationName) = 0;
};

#endif // I_AXIS_STATIONMANAGE_H
