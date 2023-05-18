#ifndef WORK_MOTOR_STOP_H
#define WORK_MOTOR_STOP_H

#include "../actioncfgdefine.h"
#include "rtdefine.h"

#define ITEM_NAME_LEN  256             // ���̱༭�������ֳ���

#ifndef MOTOR_CFG
    #define MOTOR_CFG
    // ��λ��������
    #define POINT_STATION		"station"
    #define	POINT_INDEX			"index"
    #define POINT_MOVE_TYPE		"moveType"
    #define POINT_SPEED		    "bSpeed"
    #define	POINT_VEL			"vel"
    #define POINT_ACC			"acc"
    #define	POINT_DEC			"dec"
#endif 

//�������̿���-��վֹͣ
struct WORK_MOTOR_STOP : ACTION_PARAM
{
    string stationname; //��վ����				

    WORK_MOTOR_STOP()
    {
        stationname = "";
    }

    WORK_MOTOR_STOP& operator=(const WORK_MOTOR_STOP &other)
    {
        if (this == &other)
        {
            return *this;
        }
        stationname = other.stationname;
        return *this;
    }

    bool operator==(const WORK_MOTOR_STOP &other)
    {
        return (stationname == other.stationname);
    }

    void setString(const string &data){}
    string toString()
    { 
        string str = "{"+ string(POINT_STATION) + string(":") + stationname + "}";
        return str;
    }
};

#endif 