#ifndef WORK_MOTOR_POINT_NEAR_H
#define WORK_MOTOR_POINT_NEAR_H

#include "../actioncfgdefine.h"
#include "rtdefine.h"
#include "stringcommon.h"
#include <string>
using namespace std;

#define POINT_STATION		"station"
#define POINT_NAME_BASE          "pointname"
#define POINT_NAME_MOVE     "pointnamemove"
#define POINT_OFFSET        "pointoffset"
#define AXIS_POINT_ENABLE    "pointenable"

struct WORK_MOTOR_POINT_NEAR : ACTION_PARAM
{
    string stationname; //工站名称
    string pointNameMove;   //要比较的点(为空表示当前位置)
    string pointNameBase;   //基准点位
    double pointOffset;              // 点的偏差值
    bool AxisJudgeEnable[6];         //是否使能判断

    WORK_MOTOR_POINT_NEAR()
    {
        pointNameMove = "None";
        pointOffset = 0.0;
        memset(AxisJudgeEnable,0, sizeof(AxisJudgeEnable));
    }

    WORK_MOTOR_POINT_NEAR& operator=(const WORK_MOTOR_POINT_NEAR &other)
    {
        if (this == &other)
        {
            return *this;
        }

        stationname = other.stationname;
        pointNameBase = other.pointNameBase;
        pointNameMove = other.pointNameMove;
        pointOffset = other.pointOffset;
        memcpy(AxisJudgeEnable, other.AxisJudgeEnable, sizeof(AxisJudgeEnable));
        return *this;
    }

    bool operator==(const WORK_MOTOR_POINT_NEAR &other)
    {
        return stationname == other.stationname &&
            pointNameMove == other.pointNameMove&&
            pointNameBase == other.pointNameBase&&
            pointOffset == other.pointOffset &&
            AxisJudgeEnable[0]==other.AxisJudgeEnable[0] &&
            AxisJudgeEnable[1]==other.AxisJudgeEnable[1] &&
            AxisJudgeEnable[2]==other.AxisJudgeEnable[2] &&
            AxisJudgeEnable[3]==other.AxisJudgeEnable[3] &&
            AxisJudgeEnable[4]==other.AxisJudgeEnable[4] &&
            AxisJudgeEnable[5]==other.AxisJudgeEnable[5];
        return true;
    }

    void setString(const string &data){}
    string toString()
    { 
        string str = "{"+ \
                  string(POINT_STATION) + string(":") + stationname + "," + \
                  string(POINT_NAME_BASE) + string(":") + pointNameBase + "," + \
                  string(POINT_NAME_MOVE) + string(":") + pointNameMove + "," + \
                  string(POINT_OFFSET) + string(":") + SString::DoubleToStr(pointOffset) + "," + \
                  string(AXIS_POINT_ENABLE) + string(":") + SString::BoolArrayToStr(AxisJudgeEnable, 6) + "," + \
                  "}";
        return str;
    }
};
#endif 