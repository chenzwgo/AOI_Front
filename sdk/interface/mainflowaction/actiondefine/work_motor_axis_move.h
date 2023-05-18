#ifndef WORK_MOTOR_AXIS_MOVE_H
#define WORK_MOTOR_AXIS_MOVE_H

#include "../actioncfgdefine.h"
#include "rtdefine.h"
#include "stringcommon.h"
using namespace std;

// 单轴运动动作属性
#define POINT_AXISMOVE      "work_axismove"
#define POINT_STATION		"station"
#define	POINT_DIM			"dim"
#define POINT_POS		    "pos"
#define POINT_IS_ABS		"isAbs"
#define POINT_SPEED		    "bSpeed"
#define	POINT_VEL			"vel"
#define POINT_ACC			"acc"
#define	POINT_DEC			"dec"


//工作流程控制-单轴点位信息
struct WORK_MOTOR_AXIS_MOVE : ACTION_PARAM
{
    string	stationname; //工站名称
    int     dimension;                  // 轴维度，0表示X,1表示Y ...
    double	pos;						// 点位索引
    bool    bAbs;                       // true为绝对点位，false为相对点位

    bool    bSpeed;					    //是否改速度   0不改速度。1 改速度。
    int     vel;						//目标速度
    int     acc;						//加速度
    int     dec;						//减速度

    WORK_MOTOR_AXIS_MOVE()
    {
        stationname = "";
        dimension = 0;
        bAbs = true;

        pos    = 0.0;
        bSpeed   = 0;
        vel      = 0;
        acc      = 0;
        dec      = 0;
    }

    WORK_MOTOR_AXIS_MOVE& operator=(const WORK_MOTOR_AXIS_MOVE &other)
    {
        if (this == &other)
        {
            return *this;
        }
        pos = other.pos;
        stationname =other.stationname;

        dimension = other.dimension;
        bAbs = other.bAbs;

        bSpeed = other.bSpeed;
        vel = other.vel;
        acc = other.acc;
        dec = other.dec;
        return *this;
    }

    bool operator==(const WORK_MOTOR_AXIS_MOVE &other)
    {
        return stationname == other.stationname &&
            dimension == other.dimension &&
            bAbs == other.bAbs &&
            pos == other.pos && 
            bSpeed == other.bSpeed &&
            vel == other.vel &&
            acc == other.acc &&
            dec == other.dec;
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ \
            string(POINT_STATION) + string(":") + stationname + "," + \
            string(POINT_DIM) + string(":") + SString::IntToStr(dimension) + "," + \
            string(POINT_IS_ABS) + string(":") + SString::IntToStr(bAbs) + "," + \
            string(POINT_POS) + string(":") + SString::DoubleToStr(pos) + "," + \
            string(POINT_SPEED) + string(":") + SString::IntToStr(bSpeed) + "," + \
            string(POINT_VEL) + string(":") + SString::IntToStr(vel) + "," + \
            string(POINT_ACC) + string(":") + SString::IntToStr(acc) + "," + \
            string(POINT_DEC) + string(":") + SString::IntToStr(dec) + "," + \
            "}";

        return str;
    }
};

#endif 