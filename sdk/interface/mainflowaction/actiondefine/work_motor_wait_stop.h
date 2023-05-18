#ifndef WORK_MOTOR_WAIT_STOP_H
#define WORK_MOTOR_WAIT_STOP_H

#include "../actioncfgdefine.h"
#include "rtdefine.h"

#define ITEM_NAME_LEN  256             // 流程编辑各项名字长度


// 工站名称
#define STATION_NAME		"station"


//工作流程控制-工站停止
struct WORK_MOTOR_WAIT_STOP : ACTION_PARAM
{
    string stationname; //工站名称				

    WORK_MOTOR_WAIT_STOP()
    {
        stationname = "";
    }

    WORK_MOTOR_WAIT_STOP& operator=(const WORK_MOTOR_WAIT_STOP &other)
    {
        if (this == &other)
        {
            return *this;
        }
        stationname = other.stationname;
        return *this;
    }

    bool operator==(const WORK_MOTOR_WAIT_STOP &other)
    {
        return (stationname == other.stationname);
    }

    void setString(const string &data){}
    string toString()
    { 
        string str = "{"+ string(STATION_NAME) + string(":") + stationname + "}";
        return str;
    }
};

#endif 