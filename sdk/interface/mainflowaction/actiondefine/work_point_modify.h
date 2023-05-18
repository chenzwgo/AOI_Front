#ifndef WORK_POINT_MODIFY_H
#define WORK_POINT_MODIFY_H

#include "../actioncfgdefine.h"
#include "rtdefine.h"
#include "string.h"
#include "stringcommon.h"

using namespace std;

#define ITEM_NAME_LEN  256             // 流程编辑各项名字长度

// 点位动作属性
#define POINT_STATION		"station"
#define	POINT_NAME			"point"
#define POINT_BASE		    "pointBase"
#define POINT_VAR_X		"varlistX"
#define POINT_VAR_Y		"varlistY"
#define POINT_VAR_Z		"varlistZ"
#define POINT_VAR_U		"varlistU"
#define POINT_VAR_V		"varlistV"
#define POINT_VAR_W		"varlistW"
#define	POINT_WRITEFILE		"writeFile"

//工作流程控制-工站停止
struct WORK_POINT_MODIFY : ACTION_PARAM
{
    string station; //工站名称
    string point;   //被修改的点位
    string pointBase;   //参与运算的点位，可为空值
    string varlist[6];  //变量列表,对应工站6个维度
    bool   bWriteFile;  //是否写入点位文件

    WORK_POINT_MODIFY()
    {
        bWriteFile = false;
        point = "None";
        pointBase = "None";
        varlist[0] = "None";
        varlist[1] = "None";
        varlist[2] = "None";
        varlist[3] = "None";
        varlist[4] = "None";
        varlist[5] = "None";
    }

    WORK_POINT_MODIFY& operator=(const WORK_POINT_MODIFY &other)
    {
        if (this == &other)
        {
            return *this;
        }
        station = other.station;
        point = other.point;
        pointBase = other.pointBase;
        varlist[0] = other.varlist[0];
        varlist[1] = other.varlist[1];
        varlist[2] = other.varlist[2];
        varlist[3] = other.varlist[3];
        varlist[4] = other.varlist[4];
        varlist[5] = other.varlist[5];
        bWriteFile = other.bWriteFile;
        return *this;
    }

    bool operator==(const WORK_POINT_MODIFY &other)
    {
        return (0 == station.compare(other.station)
             && 0 == point.compare(other.point)
             && 0 == pointBase.compare(other.pointBase)
             && bWriteFile == other.bWriteFile
             && 0 == varlist[0].compare(varlist[0])
             && 0 == varlist[1].compare(varlist[1])
             && 0 == varlist[2].compare(varlist[2])
             && 0 == varlist[3].compare(varlist[3])
             && 0 == varlist[4].compare(varlist[4])
             && 0 == varlist[5].compare(varlist[5]));
    }

    void setString(const string &data){}
    string toString()
    { 
        string strVarList = varlist[0]+","+varlist[1]+","+varlist[2]+","+varlist[3]+","+varlist[4]+","+varlist[5];
        string str = "{"+ string(POINT_STATION) + string(":") + station + "}" \
                   + "{"+ string(POINT_NAME) + string(":") + point + "}" \
                   + "{"+ string(POINT_BASE) + string(":") + pointBase + "}" \
                   + "{"+ string(POINT_WRITEFILE) + string(":") + SString::IntToStr(bWriteFile) + "}" \
                   + "{"+ string("varlist") + string(":") + strVarList + "}";
        return str;
    }
};

#endif 