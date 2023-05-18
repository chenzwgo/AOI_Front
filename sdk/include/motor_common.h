/*
ע�����
1.GTS�������Ŀ���Ŵ�0��ʼ�����Ŵ�1��ʼ(cardggplugin�ڽ��п��ĳ�ʼ��ʱ���Զ�����������).
2.ҵ��㿨��Ŵ�0��ʼ�����Ŵ�0��ʼ(���ǵ����ݴ���).
*/

#ifndef _MOTOR_COMMON_H_
#define _MOTOR_COMMON_H_

#include <string>
#include <vector>
#include <list>
#include <map>
#include <string.h>
#include <stdio.h>
#include "errcode_def.h"
#include <math.h>
using namespace  std;

#define INVALID_DOUBLE_VAL 0xffffffffffffffff
#define INVALID_EXT         -1      //��Ч��չ����
#define INVALID_AXIS        "None"      //��Ч��
#define INVALID_STATIONID   255      //��ЧID
#define LIST_POINT_MAX		200		//���б���������
#define BUFFER_128			128		//����������
#define BUFFER_256			256
#define BUFFER_512			512
#define BUFFER_1024			1024
#define BUFFER_2048			2048
#define IP_ADDRESS_LEN		16		//IP�ַ�����
#define NAME_LEN			128      //���������ַ�����
#define DESC_LEN			128      //�����ַ�����

#ifndef INVALID_VALUE
#define INVALID_VALUE (unsigned int)(~0)//��Ч����
#endif

typedef enum _stopType
{
	STOP_SMOOTH = 0,    //ƽ��ֹͣ
	STOP_SUDDEN,    //��ͣ
}EnStopType;

typedef enum _MoveBool    // �˶���̬
{
	MOVE_FALSE = 0,     // ʧ��
	MOVE_TRUE,          // �ɹ�	
	MOVE_DOING,         // ������
}EnMoveBool;

typedef enum _IoBool    // IO��̬
{
	IO_ERROR = -1,     // IO��ȡʧ��
	IO_FALSE = 0,      // IO�ر�
	IO_TRUE,           // IO��
}EnIoBool;

enum EM_CARD_IO_TYPE		//��IO�ź�����
{
	EM_GPO = 0,				//ͨ�����
	EM_GPI,					//ͨ������
	EM_LIMIT_POS,			//����λ
	EM_LIMIT_NEG,			//����λ
	EM_ALARM,				//�����ź�
	EM_HOME,				//ԭ���ź�
	EM_ENABLE,				//��ʹ��
	EM_CLEAR,				//�������
};

enum EM_MOTOR_TYPE			//�������
{
	EN_SERVO_MOTOR,			//�ŷ����
	EN_STEP_MOTOR,			//�������
	EN_DD_MOTOR,			//DD���
};


enum STATION_INDEX			//��վ�ᶨ��
{
	STATION_X = 0,			//X��
	STATION_Y,				//Y��
	STATION_Z,				//Z��
	STATION_U,				//U��
	STATION_V,				//V��
	STATION_W,				//W��
	STATION_SIZE,			//��վ���������
};

enum EM_MOVE_TYPE				//�˶�ģʽ
{
	EMOVE_NONE,                 //δ�˶�
	EMOVE_PTP = 0,				//�����λ�˶�
	EMOVE_JOG,
	EMOVE_ARC,					//Բ���岹
	EMOVE_LINE,				    //ֱ�߲岹
	EMOVE_MULTI,                //����˶�
	EMOVE_CONTI_INTERP,		    //�����岹�˶�
	EMOVE_HOME,                 //��ԭ
};


typedef enum EM_STATION_STATUS				//��վ״̬
{	
	STATION_ERROR=-2,           //-2 �쳣����
	STATION_UNINIT,	            //-1δ��ʼ��
	STATION_IDLE,			    //0 ����
	STATION_MOVING,		        //1 �˶���
	STATION_HOMING,             //2 ��ԭ��
	STATION_MULTIMOVE,          //3 ����˶���
	STATION_CONTIINTERP,        //4 �����岹��
	STATION_PAUSE,              //5 ��ͣ�˶�	
}EnStationStatus;

typedef struct _pos
{
	double x;
	double y;
	double z;
	double u;
	double v;
	double w;

	_pos()
	{
		x=0.0; y=0.0; z=0.0; u=0.0; v=0.0; w=0.0;
	}

	_pos(double dx, double dy=0.0, double dz=0.0, double du=0.0, double dv=0.0, double dw=0.0)
	{
		x=dx; y=dy; z=dz; u=du;	v=dv; w=dw;
	}

	_pos &operator=(const _pos &other)
	{
		if (this == &other)
		{
			return *this;
		}

		x=other.x; y=other.y; z=other.z; u=other.u; v=other.v; w=other.w;
		return *this;
	}

    bool operator==(const _pos &other)
    {
        return fabs(x-other.x)<0.000001
            && fabs(y-other.y)<0.000001
            && fabs(z-other.z)<0.000001
            && fabs(u-other.u)<0.000001
            && fabs(v-other.v)<0.000001
            && fabs(w-other.w)<0.000001;
    }

    friend _pos operator+(const _pos &pos1, const _pos &pos2)
    {
        _pos temp;
        temp.x = pos1.x + pos2.x;
        temp.y = pos1.y + pos2.y;
        temp.z = pos1.z + pos2.z;
        temp.u = pos1.u + pos2.u;
        temp.v = pos1.v + pos2.v;
        temp.w = pos1.w + pos2.w;

        return temp;
    }

}StPos;

typedef struct _POINT				//��λ��Ϣ
{
	int		index;				//������
	char	name[NAME_LEN];		//������
	char	desc[DESC_LEN];		//������
	StPos   pos;            	//��λ��

	_POINT()
	{
		index = INVALID_VALUE;
		memset(name,0,sizeof(name));
		memset(desc,0,sizeof(desc));		
	}
	_POINT& operator=(const _POINT &other)
	{
		if (this == &other)
		{
			return *this;
		}

		index = other.index;
		strncpy(name,other.name,NAME_LEN);
		strncpy(desc,other.desc,DESC_LEN);
		pos = other.pos;
		return *this;
	}
}StPoint;


typedef struct _SPEED			//�ٶ���Ϣ
{
	double vel;					//�ٶ�
	double acc;					//���ٶ�
	double dec;					//���ٶ�
	double velStart;            //�����ٶ�,˲�����ٵ����ٶȣ�����acc���ٵ�vel
	double velStop;             //ֹͣ�ٶ�,���ٵ����ٶ�ʱ��˲��ֹͣ
	double smoothTime;          //ƽ��ʱ��,�̸�ר��
	_SPEED()
	{
		vel = 1.0;       //Ĭ��Ϊ1,��ֹ�ٶ�Ϊ0���˶�
		acc = 1.0;
		dec = 1.0;
		velStart = 0.0;
		velStop = 0.0;
		smoothTime = 0.0;
	}
	_SPEED(double dV, double dAcc, double dDec, double dVStart=0.0, double dVStop=0.0,double dSTime=0.0)
	{
		vel = dV;
		acc = dAcc;
		dec = dDec;
		velStart = dVStart;
		velStop = dVStop;
		smoothTime = dSTime;
	}
	_SPEED& operator=(const _SPEED &copy)
	{
		if (this == &copy)
		{
			return *this;
		}
		vel = copy.vel;
		acc = copy.acc;
		dec = copy.dec;
		velStart = copy.velStart;
		velStop = copy.velStop;
		return *this;
	}
}StSpeed;

typedef struct _IOConfig		//IO������Ϣ
{
    char	name[NAME_LEN];		//IO����
	char	cardName[NAME_LEN];	//�����ƣ����뿨������Ϣ�ҹ�
	char	desc[DESC_LEN];		//��������
	unsigned short cardIndex;	//��������(��ʼֵΪ1)
	unsigned short ioIndex;		//IO������(��ʼֵΪ-1)
	unsigned short ciotype;		//��IO���ͣ��ο�EM_CARD_IO_TYPE(��ʼֵΪ0��ͨ�����,1-ͨ������)
	unsigned short modelNO;		//�̸���չģ���ַ��,0�򲻴���չ����1Ϊ��չ��1...(��ʼֵΪ0)
	unsigned short sense;		//��Ч��ƽ 1 ��ʾ�ߵ�ƽ��0 ��ʾ�͵�ƽ (��ʼֵΪ0)
	_IOConfig()
	{
		memset(name,0,NAME_LEN);
		memset(cardName,0,NAME_LEN);
		memset(desc,0,DESC_LEN);
		ciotype = 0;
		cardIndex = -1;
		ioIndex = -1;
		modelNO = 0;
		sense = 0;
	}

	_IOConfig& operator=(const _IOConfig &other)
	{
		if (this == &other)
		{
			return *this;
		}
		ciotype = other.ciotype;
		cardIndex = other.cardIndex;
		ioIndex = other.ioIndex;
		modelNO = other.modelNO;
		sense = other.sense;
		strncpy(name,other.name,NAME_LEN -1);
		strncpy(cardName,other.cardName,NAME_LEN -1);
		strncpy(desc,other.desc,DESC_LEN -1);

		return *this;
	}
}StIOConfig;

typedef struct _IOCardInfo		//IO����Ϣ
{
    int index ;
    int incount;
    int outcount;

    _IOCardInfo()
    {
        index  = -1;
        incount = -1;
        outcount =-1;
    }

    _IOCardInfo& operator=(const _IOCardInfo &other)
    {
        if (this == &other)
        {
            return *this;
        }
        index  = other.index;
        incount = other.incount;
        outcount = other.outcount;
        return *this;
    }
}StIOCardInfo;

typedef struct _CardInfo		//����Ϣ
{
	char	cardName[NAME_LEN];	//�����ƣ����뿨������Ϣ�ҹ�
	char	cardDesc[DESC_LEN];	//������
	short	cardIndex;	//������[0,7]��(��ʼֵΪ-1)
	short	axisCnt;		//������[0,7]��(��ʼֵΪ0)
	short	extCnt;		//��չ������,0Ϊ����
	char	cardType[NAME_LEN];	//������(���������)
    vector<StIOCardInfo>* pIOCardInfoList;
    //_IOCardInfo  IOCardInfoVec[16]; // Ĭ��һ�ſ�����15 ����չIO��, 0 δ�����ϵ�IO
	_CardInfo()
	{
		cardIndex = -1;
		axisCnt = 0;
		extCnt = 0;
		memset(cardType,0,NAME_LEN);
		memset(cardName,0,NAME_LEN);
		memset(cardDesc,0,DESC_LEN);
        pIOCardInfoList = new vector<StIOCardInfo>();
	}
    _CardInfo(const _CardInfo& other)
    {
        cardIndex = other.cardIndex;
        extCnt = other.extCnt;
        axisCnt = other.axisCnt;
        strncpy(cardType,other.cardType, NAME_LEN-1);
        strncpy(cardName,other.cardName,NAME_LEN-1);
        strncpy(cardDesc,other.cardDesc,DESC_LEN-1);
        pIOCardInfoList = new vector<StIOCardInfo>();
        if (other.pIOCardInfoList != NULL )
        {
            vector<StIOCardInfo>::iterator it = other.pIOCardInfoList->begin();
            while (it != other.pIOCardInfoList->end())
            {
                pIOCardInfoList->push_back(*it);
                it++;
            }
        }
    }

    ~_CardInfo()
    {
        if (pIOCardInfoList != NULL)
        {
            delete pIOCardInfoList;
            pIOCardInfoList = NULL;
        }
    }

	_CardInfo& operator=(const _CardInfo &other)
	{
		if (this == &other)
		{
			return *this;
		}
		cardIndex = other.cardIndex;
		extCnt = other.extCnt;
		axisCnt = other.axisCnt;
		strncpy(cardType,other.cardType, NAME_LEN-1);
		strncpy(cardName,other.cardName,NAME_LEN-1);
		strncpy(cardDesc,other.cardDesc,DESC_LEN-1);
        if (pIOCardInfoList != NULL 
            && other.pIOCardInfoList != NULL )
        {
            pIOCardInfoList->clear();
            vector<StIOCardInfo>::iterator it = other.pIOCardInfoList->begin();
            while (it != other.pIOCardInfoList->end())
            {
                pIOCardInfoList->push_back(*it);
                it++;
            }
        }
      
		return *this;
	}
}StCardInfo;

// ��ԭ��ʽ���ڽ���combox indexһһ��Ӧ
typedef enum _homeMode
{
    HOMEMODE_NLIMIT_HOME = 0,		//����λ+ԭ�� ��ԭ��ʽ,�ܵ�����λͣ��Ȼ������ԭ�㣬�ҵ���ͣ
    HOMEMODE_PLIMIT_HOME,			//����λ+ԭ�� ��ԭ��ʽ,�ܵ�����λͣ��Ȼ������ԭ�㣬�ҵ���ͣ
	HOMEMODE_NLIMIT,				//����λ��ԭ��ʽ,�ܵ�����λ��ͣ
	HOMEMODE_PLIMIT,				//����λ��ԭ��ʽ,�ܸ�����λ��ͣ
    HOMEMODE_HOME_N,				//������ԭ�㣬�ҵ���ͣ��������λ������
	HOMEMODE_HOME_P,				//������ԭ�㣬�ҵ���ͣ��������λ������
	HOMEMODE_INDEX_N,				//������index���ҵ���ͣ
	HOMEMODE_INDEX_P,				//������index���ҵ���ͣ
	HOMEMODE_USER_DEFINE,			//�û��Զ����ԭ
	HOMEMODE_MOVE_POS_ZERO,         // �ƶ���λ��0
	HOMEMODE_NONE,				//��ԭ��ʽ�ޣ�������ԭ
}EnHomeMode;

// ��Ч��ƽ�������combox indexһһ��Ӧ
typedef enum _Ele_Level  
{   
    E_LEVEL_LOW = 0,  // �͵�ƽ
    E_LEVEL_HIGH,     // �ߵ�ƽ
    E_LEVEL_NONE,     // �����
     
}EnEleLevel;

typedef enum _PosMode
{	
	POSMODE_COMMAND,   //�滮λ��
	POSMODE_ACTUAL,   //ʵ��λ��
	POSMODE_C_A,   //�滮��ʵ��λ�ã�û�и�ģʽ����Ҫ����������λ��
}EnPosMode;

typedef struct _AxisParam		//�������Ϣ
{
	short cardIndex;	//������[0,7]
	short axisIndex;	//������[0,7]��ҵ����0��ʼ(��������CAxis�ڲ���Ч)
	char AxisName[NAME_LEN];	//������
	char CardName[NAME_LEN];	//�����ƣ��뿨��Ϣ�Ĺ�����ʶ������������֮��Ĺ���
	char AxisDesc[DESC_LEN];    //������

	short motorType;				//�����ͣ�0-�ŷ� 1����
	bool limitNConnect;			//����λ�ź��Ƿ�����
	bool limitPConnect;			//����λ�ź��Ƿ�����
	bool homeConnect;				//ԭ���ź��Ƿ�����
    bool servOnConnect;			//��ʹ���Ƿ�����
    bool alarmConnect;			//�����Ƿ�����
	bool limitAlarmConnect;     //��λ�����Ƿ�����
    EnEleLevel limitNLevel;		//����λ��Ч��ƽ
    EnEleLevel limitPLevel;		//����λ��Ч��ƽ
    EnEleLevel homeLevel;			//ԭ����Ч��ƽ
    EnEleLevel servOnLevel;		//��ʹ����Ч��ƽ
    EnEleLevel alarmLevel;		//�ᱨ����Ч��ƽ

	double velStart;              //��ʼ�ٶ�
	double velStop;                //ֹͣ�ٶ�
	double vel;					//�����ٶ�
	double acc;					//�������ٶ�
	double dec;					//�������ٶ�
	double smoothtime;			//ƽ��ʱ��
	double softLimitN;				//����λλ��
	double softLimitP;				//������λλ��
    bool softLimitNEnable;        //��������λ
    bool softLimitPEnable;        //����������λ
	double homeVel;				//��ԭ�ٶ�
	double homeAcc;				//��ԭ���ٶ�
	double homeDec;				//��ԭ���ٶ�
	double homeSmoothtime;		//��ԭƽ��ʱ��
	double pulsepermm;			//ת������ һ���׶�Ӧ��������ֵ
	double homePos;				//��ԭ��������
	double homeOffset;			//ԭ��ƫ����
	double indexPos;				//index��������
	double indexOffset;			//indexƫ����
    char dd_HomeStart[NAME_LEN];//DD����ԭ�����ź�
    char dd_HomeFinish[NAME_LEN];//DD���ԭ���ź�
	EnHomeMode  homeMode;              //��ԭ��ʽ��0Ϊ����λ��1Ϊ����λ
	EnPosMode posMode;               //�Ƿ�ʹ��ʵ��λ�ã�false��ʾ�滮λ�ã�
    
	_AxisParam()
	{
		cardIndex = -1;
		axisIndex = -1;
		motorType = 0;
		limitNConnect = 0;
		limitPConnect = 0;
		homeConnect = 0;
        servOnConnect = 0;
        alarmConnect = 0;
		limitAlarmConnect = 1;
        limitNLevel=E_LEVEL_LOW;
        limitPLevel=E_LEVEL_LOW;
        homeLevel=E_LEVEL_LOW;
        servOnLevel=E_LEVEL_LOW;
        alarmLevel=E_LEVEL_LOW;
		velStart =0.0;
		velStop = 0.0;
		vel = 0.0;
		acc = 0.0;
		dec = 0.0;
		smoothtime = 0.0;
		softLimitN = 0.0;
		softLimitP = 0.0;
        softLimitNEnable = false;
        softLimitPEnable = false;
		homeVel = 0.0;
		homeAcc = 0.0;
		homeDec = 0.0;
		homeSmoothtime = 0.0;
		pulsepermm = 1;
		homePos = 0;
		homeOffset = 0;
		indexPos = 0;
		indexOffset = 0;
		homeMode = HOMEMODE_NONE;
		posMode = POSMODE_COMMAND; 
		memset(dd_HomeStart,0,NAME_LEN);
		memset(dd_HomeFinish,0,NAME_LEN);
		memset(AxisName,0,NAME_LEN);
		memset(AxisDesc,0,NAME_LEN);
		memset(CardName,0,NAME_LEN);
    }

    _AxisParam& operator=(const _AxisParam &other)
    {
        if (this == &other)
        {
            return *this;
        }

        cardIndex = other.cardIndex;
        axisIndex = other.axisIndex;
        motorType = other.motorType;
        limitNConnect = other.limitNConnect;
        limitPConnect = other.limitPConnect;
		homeConnect = other.homeConnect;
        servOnConnect = other.servOnConnect;
        alarmConnect = other.alarmConnect;
		limitAlarmConnect = other.limitAlarmConnect;
        limitNLevel = other.limitNLevel;
        limitPLevel = other.limitPLevel;
        homeLevel = other.homeLevel;
        servOnLevel = other.servOnLevel;
        alarmLevel = other.alarmLevel;
		velStart = other.velStart;
		velStop = other.velStop;
        vel = other.vel;
        acc = other.acc;
        dec = other.dec;
        smoothtime = other.smoothtime;
        softLimitN = other.softLimitN;
        softLimitP = other.softLimitP;
        softLimitNEnable = other.softLimitNEnable;
        softLimitPEnable = other.softLimitPEnable;
        homeVel = other.homeVel;
        homeAcc = other.homeAcc;
        homeDec = other.homeDec;
        homeSmoothtime = other.homeSmoothtime;
        homePos = other.homePos;
        homeOffset = other.homeOffset;
        indexPos = other.indexPos;
        indexOffset = other.indexOffset;
		pulsepermm = other.pulsepermm;
		homeMode = other.homeMode;
		posMode = other.posMode;
		strncpy(dd_HomeStart,other.dd_HomeStart,NAME_LEN-1);
		strncpy(dd_HomeFinish,other.dd_HomeFinish,NAME_LEN-1);
		strncpy(AxisName,other.AxisName,NAME_LEN-1);
		strncpy(AxisDesc,other.AxisDesc,NAME_LEN-1);
		strncpy(CardName,other.CardName,NAME_LEN-1);

        return *this;
    }
}StAxisParam;


typedef struct _ROBOT_INFO
{
	char    serverIP[IP_ADDRESS_LEN];	//�����IP
	unsigned short sport;				//����˶˿ں�
	char    clientIP[IP_ADDRESS_LEN];	//�ͻ���IP
	unsigned short cport;				//�ͻ��˶˿ں�
}RobotInfo;


typedef struct _StationInfo			//��վ��Ϣ
{
	int  id;					//��վID
    int	 axiscnt;				//�������
    char name[NAME_LEN];		//��վ����
    char pointfname[NAME_LEN];	//���ļ�����
    char desc[DESC_LEN];		//��վ����
	char axisname[6][NAME_LEN];	 //��������Ϣ
    int  homePri[6];             //��ԭ���ȼ�,��ֵ�������
	_StationInfo& operator=(const _StationInfo &other)
	{
		if (this == &other)
		{
			return *this;
        }
		id = other.id;
        axiscnt = other.axiscnt;

		strncpy(name,other.name,NAME_LEN-1);
		strncpy(pointfname,other.pointfname,NAME_LEN-1);
		strncpy(desc,other.desc,DESC_LEN-1);
        memcpy(axisname,&other.axisname,sizeof(axisname));
        memcpy(homePri, other.homePri, sizeof(homePri));
		return *this;
	}
    _StationInfo()
    {
		id = INVALID_VALUE;
        axiscnt = 0;
		memset(name,0,NAME_LEN);
		memset(pointfname,0,NAME_LEN);
		memset(desc,0,DESC_LEN);
		memset(axisname,0,sizeof(axisname));
        memset(homePri,0, sizeof(homePri));
    }
}StStationInfo;

typedef enum _mask_axis
{
    Mask_ALL = 0x3f,
    Mask_X = 0x01,
    Mask_Y = 0x02,
    Mask_Z = 0x04,
    Mask_U = 0x08,
    Mask_V = 0x10,
    Mask_W = 0x20
}E_AXISMASK;

#endif
