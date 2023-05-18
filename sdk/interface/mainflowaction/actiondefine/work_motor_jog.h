#ifndef WORK_MOTOR_JOG_H
#define WORK_MOTOR_JOG_H

#include "../actioncfgdefine.h"

// ��λ��������
#define POINT_STATION		"station"
// jog�˶�����
#define JOG_DIRECTION       "direction"


//�������̿���-��վjog�˶�
struct WORK_MOTOR_JOG : ACTION_PARAM
{
    string stationname; //��վ����				
    bool bDirection;                 //����,trueΪ������falseΪ������
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