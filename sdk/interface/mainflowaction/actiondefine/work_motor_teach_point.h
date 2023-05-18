#ifndef WORK_MOTOR_TEACH_POINT_H
#define WORK_MOTOR_TEACH_POINT_H

#include "../actioncfgdefine.h"
#include "rtdefine.h"
#include "stringcommon.h"
#include <string>
using namespace std;

#define POINT_STATION		"station"
#define POINT_NAME          "pointname"

struct WORK_MOTOR_TEACH_POINT : ACTION_PARAM
{
    string stationname; //工站名称
    string pointName;   //判断点的名字

    WORK_MOTOR_TEACH_POINT()
    {
        stationname = "";
        pointName = "";
    }

    WORK_MOTOR_TEACH_POINT& operator=(const WORK_MOTOR_TEACH_POINT &other)
    {
        if (this == &other)
        {
            return *this;
        }

        stationname = other.stationname;
        pointName = other.pointName;
        return *this;
    }

    bool operator==(const WORK_MOTOR_TEACH_POINT &other)
    {
        return stationname == other.stationname &&
            pointName == other.pointName;
    }

    void setString(const string &data){}
    string toString()
    { 
        string str = "{"+ \
                  string(POINT_STATION) + string(":") + stationname + "," + \
                  string(POINT_NAME) + string(":") + pointName + "," + \
                  "}";
        return str;
    }
};
#endif 