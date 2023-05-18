#ifndef WORK_MOTOR_AXIS_STATUS_H
#define WORK_MOTOR_AXIS_STATUS_H

#include "rtdefine.h"
#include "../actioncfgdefine.h"
#include "stringcommon.h"

using namespace std;

#define ITEM_NAME_LEN  256             // ���̱༭�������ֳ���

// ����״̬����
#define AXIS_STATUS         "axisStatus"
#define AXIS_STATION        "station"
#define	AXIS_DIM			"dim"
#define AXIS_SIGTYPE		"sigtype"
#define AXIS_VALUE		    "value"

//�������̿���-��״̬
struct WORK_MOTOR_AXIS_STATUS : ACTION_PARAM
{
    enum {SIGTYPE_LimitN = 0,SIGTYPE_LimitP,SIGTYPE_Home,SIGTYPE_Enable,SIGTYPE_Alarm, SIGTYPE_Moving};
    string	stationname; //��վ����
    int     dimension;                  //��ά�ȣ�0��ʾX,1��ʾY ...
    int     sigType;                    //0-����λ�� 1-����λ��2-ԭ�㣬3-ʹ�ܣ�4-���� 5-�˶���
    bool    value;                      //�趨ֵ

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