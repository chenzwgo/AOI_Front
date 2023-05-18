#ifndef WORK_MOTOR_MOVE_H
#define WORK_MOTOR_MOVE_H

#include "../actioncfgdefine.h"
#include "motor_common.h"
#include "rtdefine.h"
#include "stringcommon.h"
using namespace std;

#ifndef MOTOR_CFG
#define MOTOR_CFG
// 点位动作属性
#define POINT_STATION		"station"
#define	POINT_INDEX			"index"
#define POINT_MASK          "mask"
#define POINT_MOVE_TYPE		"moveType"
#define POINT_SPEED		    "bSpeed"
#define	POINT_VEL			"vel"
#define POINT_ACC			"acc"
#define	POINT_DEC			"dec"
#define POINT_OFFSET        "pointOffset"
#endif 


//工作流程控制-点位信息
struct WORK_MOTOR_MOVE : ACTION_PARAM
{
    string	stationname; //工站名称
    bool     bSpeed;					//是否改速度   0不改速度。1 改速度。
    int		index;						//点位索引
    int     vel;						//目标速度
    int     acc;						//加速度
    int     dec;						//减速度
    StPos   offset;                     //点的相对位置
	E_AXISMASK  mask;                       //掩码,选择要移动的轴

    WORK_MOTOR_MOVE()
    {
        stationname = "";
        index    = 0;
        bSpeed   = 0;
        vel      = 0;
        acc      = 0;
        dec      = 0;
		mask = Mask_ALL;
    }

    WORK_MOTOR_MOVE& operator=(const WORK_MOTOR_MOVE &other)
    {
        if (this == &other)
        {
            return *this;
        }
        index = other.index;
        stationname = other.stationname;

        bSpeed = other.bSpeed;
        vel = other.vel;
        acc = other.acc;
        dec = other.dec;
        offset = other.offset;
		mask = other.mask;
        return *this;
    }

    bool operator==(const WORK_MOTOR_MOVE &other)
    {
        return stationname == other.stationname && 
            index == other.index && 
            bSpeed == other.bSpeed &&
            vel == other.vel &&
            acc == other.acc &&
            dec == other.dec &&
            offset == other.offset &&
			mask == other.mask;
    }

    void setString(const string &data){}
    string toString()
    { 
        string strPos = SString::DoubleToStr(offset.x)+ "," \
                      + SString::DoubleToStr(offset.y)+ "," \
                      + SString::DoubleToStr(offset.z)+ "," \
                      + SString::DoubleToStr(offset.u)+ "," \
                      + SString::DoubleToStr(offset.v)+ "," \
                      + SString::DoubleToStr(offset.w);

        string str = "{"+ \
                          string(POINT_STATION) + string(":") + stationname + "," + \
                          string(POINT_INDEX) + string(":") + SString::IntToStr(index) + "," + \
                          string(POINT_SPEED) + string(":") + SString::IntToStr(bSpeed) + "," + \
                          string(POINT_VEL) + string(":") + SString::IntToStr(vel) + "," + \
                          string(POINT_ACC) + string(":") + SString::IntToStr(acc) + "," + \
                          string(POINT_DEC) + string(":") + SString::IntToStr(dec) + "," + \
                          string(POINT_OFFSET) + string(":") + strPos + "," + \
                          string(POINT_MASK) + string(":") + SString::IntToStr(mask) + "," + \
                     "}";
        return str;
    }
};

#endif 