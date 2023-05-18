#ifndef WORK_MOTOR_HOME_H
#define WORK_MOTOR_HOME_H

#include <vector>
#include "../actioncfgdefine.h"
#include "rtdefine.h"

using namespace std;

// 回原
#define POINT_STATION		"station"
#define HOME_SETPRI         "setPri"
#define HOME_PRI            "homePri"

//工作流程控制-工站回原
struct WORK_MOTOR_HOME : ACTION_PARAM
{
    string stationname; //工站名称
    bool bSetHomePri;                //设置回原优先级
    int  homePri[6];                 //回原优先级

    WORK_MOTOR_HOME()
    {
        stationname = "";
        bSetHomePri = false;
        memset(homePri,0, sizeof(homePri));
    }

    WORK_MOTOR_HOME& operator=(const WORK_MOTOR_HOME &other)
    {
        if (this == &other)
        {
            return *this;
        }

        stationname = other.stationname;
        bSetHomePri = other.bSetHomePri;
        memcpy(homePri, other.homePri, sizeof(homePri));
        return *this;
    }

    bool operator==(const WORK_MOTOR_HOME &other)
    {
        return stationname == other.stationname &&
            bSetHomePri == other.bSetHomePri &&
            homePri[0]==other.homePri[0] &&
            homePri[1]==other.homePri[1] &&
            homePri[2]==other.homePri[2] &&
            homePri[3]==other.homePri[3] &&
            homePri[4]==other.homePri[4] &&
            homePri[5]==other.homePri[5];
    }

    void setString(const string &data){}
    string toString()
    { 
        char strHomePri[32] = {0};
        sprintf(strHomePri, "%d,%d,%d,%d,%d,%d", homePri[0],homePri[1],homePri[2],homePri[3],homePri[4],homePri[5]);
        string str = "{"+ string(POINT_STATION) + string(":") + stationname + "," + string(HOME_SETPRI) + string(":") + string(bSetHomePri?"1":"0") \
            + "," + string(HOME_PRI) + string(":") + strHomePri + "}";
        return str;
    }
};

#endif 