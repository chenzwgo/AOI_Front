/*
注意事项：
1.GTS控制器的卡编号从0开始，轴编号从1开始(cardggplugin在进行卡的初始化时会自动设置轴索引).
2.业务层卡编号从0开始，轴编号从0开始(考虑到数据处理).
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
#define INVALID_EXT         -1      //无效扩展卡号
#define INVALID_AXIS        "None"      //无效轴
#define INVALID_STATIONID   255      //无效ID
#define LIST_POINT_MAX		200		//点列表最大点数量
#define BUFFER_128			128		//缓冲区长度
#define BUFFER_256			256
#define BUFFER_512			512
#define BUFFER_1024			1024
#define BUFFER_2048			2048
#define IP_ADDRESS_LEN		16		//IP字符长度
#define NAME_LEN			128      //名称描述字符长度
#define DESC_LEN			128      //描述字符长度

#ifndef INVALID_VALUE
#define INVALID_VALUE (unsigned int)(~0)//无效参数
#endif

typedef enum _stopType
{
	STOP_SMOOTH = 0,    //平滑停止
	STOP_SUDDEN,    //急停
}EnStopType;

typedef enum _MoveBool    // 运动三态
{
	MOVE_FALSE = 0,     // 失败
	MOVE_TRUE,          // 成功	
	MOVE_DOING,         // 进行中
}EnMoveBool;

typedef enum _IoBool    // IO三态
{
	IO_ERROR = -1,     // IO获取失败
	IO_FALSE = 0,      // IO关闭
	IO_TRUE,           // IO打开
}EnIoBool;

enum EM_CARD_IO_TYPE		//卡IO信号类型
{
	EM_GPO = 0,				//通用输出
	EM_GPI,					//通用输入
	EM_LIMIT_POS,			//正限位
	EM_LIMIT_NEG,			//负限位
	EM_ALARM,				//报警信号
	EM_HOME,				//原点信号
	EM_ENABLE,				//轴使能
	EM_CLEAR,				//清除报警
};

enum EM_MOTOR_TYPE			//马达类型
{
	EN_SERVO_MOTOR,			//伺服马达
	EN_STEP_MOTOR,			//步进马达
	EN_DD_MOTOR,			//DD马达
};


enum STATION_INDEX			//工站轴定义
{
	STATION_X = 0,			//X轴
	STATION_Y,				//Y轴
	STATION_Z,				//Z轴
	STATION_U,				//U轴
	STATION_V,				//V轴
	STATION_W,				//W轴
	STATION_SIZE,			//工站最大轴数量
};

enum EM_MOVE_TYPE				//运动模式
{
	EMOVE_NONE,                 //未运动
	EMOVE_PTP = 0,				//单点点位运动
	EMOVE_JOG,
	EMOVE_ARC,					//圆弧插补
	EMOVE_LINE,				    //直线插补
	EMOVE_MULTI,                //多点运动
	EMOVE_CONTI_INTERP,		    //连续插补运动
	EMOVE_HOME,                 //回原
};


typedef enum EM_STATION_STATUS				//工站状态
{	
	STATION_ERROR=-2,           //-2 异常报警
	STATION_UNINIT,	            //-1未初始化
	STATION_IDLE,			    //0 就绪
	STATION_MOVING,		        //1 运动中
	STATION_HOMING,             //2 回原中
	STATION_MULTIMOVE,          //3 多点运动中
	STATION_CONTIINTERP,        //4 连续插补中
	STATION_PAUSE,              //5 暂停运动	
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

typedef struct _POINT				//点位信息
{
	int		index;				//点索引
	char	name[NAME_LEN];		//点名称
	char	desc[DESC_LEN];		//点描述
	StPos   pos;            	//点位置

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


typedef struct _SPEED			//速度信息
{
	double vel;					//速度
	double acc;					//加速度
	double dec;					//减速度
	double velStart;            //启动速度,瞬间提速到该速度，再以acc加速到vel
	double velStop;             //停止速度,减速到该速度时，瞬间停止
	double smoothTime;          //平滑时间,固高专用
	_SPEED()
	{
		vel = 1.0;       //默认为1,防止速度为0不运动
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

typedef struct _IOConfig		//IO配置信息
{
    char	name[NAME_LEN];		//IO名称
	char	cardName[NAME_LEN];	//卡名称，将与卡配置信息挂勾
	char	desc[DESC_LEN];		//功能描述
	unsigned short cardIndex;	//卡索引，(初始值为1)
	unsigned short ioIndex;		//IO索引，(初始值为-1)
	unsigned short ciotype;		//卡IO类型，参考EM_CARD_IO_TYPE(初始值为0，通用输出,1-通用输入)
	unsigned short modelNO;		//固高扩展模块地址码,0则不带扩展卡，1为扩展卡1...(初始值为0)
	unsigned short sense;		//有效电平 1 表示高电平，0 表示低电平 (初始值为0)
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

typedef struct _IOCardInfo		//IO卡信息
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

typedef struct _CardInfo		//卡信息
{
	char	cardName[NAME_LEN];	//卡名称，将与卡配置信息挂勾
	char	cardDesc[DESC_LEN];	//卡描述
	short	cardIndex;	//卡索引[0,7]，(初始值为-1)
	short	axisCnt;		//轴数量[0,7]，(初始值为0)
	short	extCnt;		//扩展卡数量,0为本卡
	char	cardType[NAME_LEN];	//卡类型(卡插件名称)
    vector<StIOCardInfo>* pIOCardInfoList;
    //_IOCardInfo  IOCardInfoVec[16]; // 默认一张卡挂着15 个扩展IO卡, 0 未本卡上的IO
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

// 回原方式，于界面combox index一一对应
typedef enum _homeMode
{
    HOMEMODE_NLIMIT_HOME = 0,		//负限位+原点 回原方式,跑到负限位停，然后反向找原点，找到就停
    HOMEMODE_PLIMIT_HOME,			//正限位+原点 回原方式,跑到正限位停，然后反向找原点，找到就停
	HOMEMODE_NLIMIT,				//负限位回原方式,跑到负限位就停
	HOMEMODE_PLIMIT,				//正限位回原方式,跑负正限位就停
    HOMEMODE_HOME_N,				//负向找原点，找到就停，碰到限位则反向找
	HOMEMODE_HOME_P,				//正向找原点，找到就停，碰到限位则反向找
	HOMEMODE_INDEX_N,				//负向找index，找到就停
	HOMEMODE_INDEX_P,				//正向找index，找到就停
	HOMEMODE_USER_DEFINE,			//用户自定义回原
	HOMEMODE_MOVE_POS_ZERO,         // 移动到位置0
	HOMEMODE_NONE,				//回原方式无，即不回原
}EnHomeMode;

// 有效电平，与界面combox index一一对应
typedef enum _Ele_Level  
{   
    E_LEVEL_LOW = 0,  // 低电平
    E_LEVEL_HIGH,     // 高电平
    E_LEVEL_NONE,     // 不检测
     
}EnEleLevel;

typedef enum _PosMode
{	
	POSMODE_COMMAND,   //规划位置
	POSMODE_ACTUAL,   //实际位置
	POSMODE_C_A,   //规划和实际位置，没有该模式，主要用于设置轴位置
}EnPosMode;

typedef struct _AxisParam		//轴参数信息
{
	short cardIndex;	//卡索引[0,7]
	short axisIndex;	//轴索引[0,7]，业务层从0开始(该索引在CAxis内部无效)
	char AxisName[NAME_LEN];	//轴名称
	char CardName[NAME_LEN];	//卡名称，与卡信息的关联标识，废弃卡索引之间的关联
	char AxisDesc[DESC_LEN];    //轴描述

	short motorType;				//轴类型，0-伺服 1步进
	bool limitNConnect;			//负限位信号是否连接
	bool limitPConnect;			//正限位信号是否连接
	bool homeConnect;				//原点信号是否连接
    bool servOnConnect;			//轴使能是否启用
    bool alarmConnect;			//报警是否启用
	bool limitAlarmConnect;     //限位报警是否启用
    EnEleLevel limitNLevel;		//负限位有效电平
    EnEleLevel limitPLevel;		//正限位有效电平
    EnEleLevel homeLevel;			//原点有效电平
    EnEleLevel servOnLevel;		//轴使能有效电平
    EnEleLevel alarmLevel;		//轴报警有效电平

	double velStart;              //初始速度
	double velStop;                //停止速度
	double vel;					//工作速度
	double acc;					//工作加速度
	double dec;					//工作减速度
	double smoothtime;			//平滑时间
	double softLimitN;				//软负限位位置
	double softLimitP;				//软正限位位置
    bool softLimitNEnable;        //启用软负限位
    bool softLimitPEnable;        //启用软正限位
	double homeVel;				//回原速度
	double homeAcc;				//回原加速度
	double homeDec;				//回原减速度
	double homeSmoothtime;		//回原平滑时间
	double pulsepermm;			//转换基数 一毫米对应多少脉冲值
	double homePos;				//回原搜索距离
	double homeOffset;			//原点偏移量
	double indexPos;				//index搜索距离
	double indexOffset;			//index偏移量
    char dd_HomeStart[NAME_LEN];//DD马达回原启动信号
    char dd_HomeFinish[NAME_LEN];//DD马达原点信号
	EnHomeMode  homeMode;              //回原方式，0为负限位，1为正限位
	EnPosMode posMode;               //是否使用实际位置（false表示规划位置）
    
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
	char    serverIP[IP_ADDRESS_LEN];	//服务端IP
	unsigned short sport;				//服务端端口号
	char    clientIP[IP_ADDRESS_LEN];	//客户端IP
	unsigned short cport;				//客户端端口号
}RobotInfo;


typedef struct _StationInfo			//工站信息
{
	int  id;					//工站ID
    int	 axiscnt;				//轴的数量
    char name[NAME_LEN];		//工站名字
    char pointfname[NAME_LEN];	//点文件名字
    char desc[DESC_LEN];		//工站描述
	char axisname[6][NAME_LEN];	 //轴名称信息
    int  homePri[6];             //回原优先级,数值大的优先
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
