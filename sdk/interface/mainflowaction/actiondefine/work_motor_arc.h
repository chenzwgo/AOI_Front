#ifndef WORK_MOTOR_ARC_H
#define WORK_MOTOR_ARC_H

#include "../actioncfgdefine.h"
#include "rtdefine.h"
#include "stringcommon.h"

using namespace std;

// Բ���岹��������
#define POINT_STATION		"station"
#define	POINT_INDEX_BEGIN		"indexBegin"
#define	POINT_INDEX_MID		"indexMid"
#define	POINT_INDEX_END		"indexEnd"
#define POINT_SPEED		    "bSpeed"
#define	POINT_VEL			"vel"
#define POINT_ACC			"acc"
#define	POINT_DEC			"dec"

//�������̿���-��λ��Ϣ
struct WORK_MOTOR_ARC : ACTION_PARAM
{
    string			stationname; //��վ����
    bool            bSpeed;						//�Ƿ���ٶ�   0�����ٶȡ�1 ���ٶȡ�
    int             indexBegin;                 //Բ����ʼ��
    int				indexMid;					//Բ���м��λ����
    int             indexEnd;                   //Բ���յ��λ����
    int             vel;						//Ŀ���ٶ�       io����ʱ����ʶΪ����ǰ�ȴ�ʱ�� Ĭ��Ϊ0
    int             acc;						//���ٶ�         io����ʱ����ʶΪ������ȴ�ʱ�� Ĭ��Ϊ0
    int             dec;						//���ٶ�

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