#ifndef WORK_MOTOR_AXIS_STATUS_H
#define WORK_MOTOR_AXIS_STATUS_H

#include "rtdefine.h"
#include "../actioncfgdefine.h"
#include "stringcommon.h"

using namespace std;

#define ITEM_NAME_LEN  256             // 流程编辑各项名字长度

// 单轴状态属性
#define AXIS_STATUS         "axisStatus"
#define AXIS_STATION        "station"
#define	AXIS_DIM			"dim"
#define AXIS_SIGTYPE		"sigtype"
#define AXIS_VALUE		    "value"

//工作流程控制-轴状态
struct WORK_MOTOR_AXIS_STATUS : ACTION_PARAM
{
    enum {SIGTYPE_LimitN = 0,SIGTYPE_LimitP,SIGTYPE_Home,SIGTYPE_Enable,SIGTYPE_Alarm, SIGTYPE_Moving};
    string	stationname; //工站名称
    int     dimension;                  //轴维度，0表示X,1表示Y ...
    int     sigType;                    //0-负限位， 1-正限位，2-原点，3-使能，4-报警 5-运动中
    bool    value;                      //设定值

    WORK_MOTOR_AXIS_STATUS()
    {
        stationname = "";
        dimension = 0;
        sigType = 0;
        value = false;
    }

    WORK_MOTOR_AXIS_STATUS& operator=(const WORK_MOTOR_AXIS_STATUS &other)
    {
        if (this == &other)
        {
            return *this;
        }

        stationname = other.stationname;
        dimension = other.dimension;
        sigType = other.sigType;
        value = other.value;
        return *this;
    }

    bool operator==(const WORK_MOTOR_AXIS_STATUS &other)
    {
        return stationname == other.stationname && 
            dimension == other.dimension &&
            sigType == other.sigType &&
            value == other.value;
    }

    void setString(const string &data){}
    string toString()
    { 
        string str = "{"+ \
                          string(AXIS_STATION) + string(":") + stationname + \
                          string(AXIS_DIM) + string(":") + SString::IntToStr(dimension) + \
                          string(AXIS_SIGTYPE) + string(":") + SString::IntToStr(sigType) + \
                          string(AXIS_VALUE) + string(":") + SString::IntToStr(value) + \
                      "}";
        return str;
    }
};
#endif 