#ifndef WORK_MOTOR_JOG_H
#define WORK_MOTOR_JOG_H

#include "../actioncfgdefine.h"

// 点位动作属性
#define POINT_STATION		"station"
// jog运动方向
#define JOG_DIRECTION       "direction"


//工作流程控制-工站jog运动
struct WORK_MOTOR_JOG : ACTION_PARAM
{
    string stationname; //工站名称				
    bool bDirection;                 //方向,true为正方向，false为反方向
    WORK_MOTOR_JOG()
    {
        stationname = "";
        bDirection = true;
    }

    WORK_MOTOR_JOG& operator=(const WORK_MOTOR_JOG &other)
    {
        if (this == &other)
        {
            return *this;
        }

        stationname = other.stationname;
        bDirection = other.bDirection;
        return *this;
    }

    bool operator==(const WORK_MOTOR_JOG &other)
    {
        return stationname == other.stationname 
                && bDirection == other.bDirection;
    }

    void setString(const string &data){}
    string toString()
    { 
        string str = "{"+ string(POINT_STATION) + string(":") + stationname + "," + string(JOG_DIRECTION) + string(":") + string(bDirection?"1":"0") + "}";
        return str;
    }
};

#endif 