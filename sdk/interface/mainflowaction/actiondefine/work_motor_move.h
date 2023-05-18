#ifndef WORK_MOTOR_MOVE_H
#define WORK_MOTOR_MOVE_H

#include "../actioncfgdefine.h"
#include "motor_common.h"
#include "rtdefine.h"
#include "stringcommon.h"
using namespace std;

#ifndef MOTOR_CFG
#define MOTOR_CFG
// ��λ��������
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


//�������̿���-��λ��Ϣ
struct WORK_MOTOR_MOVE : ACTION_PARAM
{
    string	stationname; //��վ����
    bool     bSpeed;					//�Ƿ���ٶ�   0�����ٶȡ�1 ���ٶȡ�
    int		index;						//��λ����
    int     vel;						//Ŀ���ٶ�
    int     acc;						//���ٶ�
    int     dec;						//���ٶ�
    StPos   offset;                     //������λ��
	E_AXISMASK  mask;                       //����,ѡ��Ҫ�ƶ�����

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