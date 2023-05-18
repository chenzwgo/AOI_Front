#ifndef WORK_MOTOR_ARC_H
#define WORK_MOTOR_ARC_H

#include "../actioncfgdefine.h"
#include "rtdefine.h"
#include "stringcommon.h"

using namespace std;

// 圆弧插补动作属性
#define POINT_STATION		"station"
#define	POINT_INDEX_BEGIN		"indexBegin"
#define	POINT_INDEX_MID		"indexMid"
#define	POINT_INDEX_END		"indexEnd"
#define POINT_SPEED		    "bSpeed"
#define	POINT_VEL			"vel"
#define POINT_ACC			"acc"
#define	POINT_DEC			"dec"

//工作流程控制-点位信息
struct WORK_MOTOR_ARC : ACTION_PARAM
{
    string			stationname; //工站名称
    bool            bSpeed;						//是否改速度   0不改速度。1 改速度。
    int             indexBegin;                 //圆弧开始点
    int				indexMid;					//圆弧中间点位索引
    int             indexEnd;                   //圆弧终点点位索引
    int             vel;						//目标速度       io操作时，标识为操作前等待时间 默认为0
    int             acc;						//加速度         io操作时，标识为操作后等待时间 默认为0
    int             dec;						//减速度

    WORK_MOTOR_ARC()
    {
        stationname = "";
        indexBegin = 0;
        indexMid    = 0;
        indexEnd = 0;
        bSpeed    = 0;   
        vel      = 0;    
        acc      = 0;    
        dec      = 0;    
    }

    WORK_MOTOR_ARC& operator=(const WORK_MOTOR_ARC &other)
    {
        if (this == &other)
        {
            return *this;
        }
        indexBegin = other.indexBegin;
        indexMid = other.indexMid;
        indexEnd = other.indexEnd;
        stationname =other.stationname;

        bSpeed = other.bSpeed;
        vel = other.vel;
        acc = other.acc;
        dec = other.dec;
        return *this;
    }

    bool operator==(const WORK_MOTOR_ARC &other)
    {
        return stationname == other.stationname && 
            indexBegin == other.indexBegin &&
            indexMid == other.indexMid && 
            indexEnd == other.indexEnd &&      
            bSpeed == other.bSpeed &&
            vel == other.vel &&
            acc == other.acc &&
            dec == other.dec;
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ \
            string(POINT_STATION) + string(":") + stationname + \
            string(POINT_INDEX_BEGIN) + string(":") + SString::IntToStr(indexBegin) + \
            string(POINT_INDEX_MID) + string(":") + SString::IntToStr(indexMid) + \
            string(POINT_INDEX_END) + string(":") + SString::IntToStr(indexEnd) + \
            string(POINT_SPEED) + string(":") + SString::IntToStr(bSpeed) + \
            string(POINT_VEL) + string(":") + SString::IntToStr(vel) + \
            string(POINT_ACC) + string(":") + SString::IntToStr(acc) + \
            string(POINT_DEC) + string(":") + SString::IntToStr(dec) + \
            "}";
        return str;
    }
};
#endif 