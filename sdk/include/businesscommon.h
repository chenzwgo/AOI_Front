#ifndef BUSINISS_COMMON_H
#define BUSINISS_COMMON_H

#include <stdio.h>
#include <string>
#include "jtime.h"
#include "errcode_def.h"

using namespace std;

#define MAX_OPS_BUFF_LEN    256             //缓冲区大小
#define BUFF_CNT            20              //缓冲区个数
#define STEPCIRCLETIME		200             //circle time

enum EM_SYS_WORK_STATE
{//系统状态
    EM_SYS_NOINIT = 0,    //未初始化
    EM_SYS_INIT,          //初始化中
    EM_SYS_WAIT_RESET,    //等待复位
    EM_SYS_ESTOP,         //急停
    EM_SYS_RESET,         //复位中
    EM_SYS_RESET_FAILED,  //复位失败
    EM_SYS_STOP,          //停止中
    EM_SYS_READY,         //就绪
    EM_SYS_PAUSE,         //暂停
    EM_SYS_WORK,          //工作中
    EM_SYS_FINISH,        // 工作完成
    EM_SYS_ERROR,         // 出现异常
    EM_SYS_SAFE_DOOR_OPEN, // 安全门被打开
    EM_SYS_POWER_OFF,      // 断电
    EM_SYS_LIMIT_ON,       // 限位触发
};

#define INVALID_VALUE         -1       //无效值
//
#define CCD_DATA_MAX          14        //ccd拍照最大返回14个数据
#define MAX_SN_LENGTH         23       //最大SN码长度
#define MAX_PIC_NAME_LENGTH   256      //最大图片名字长度

//工站提示灯类型
typedef enum E_POLIT_LIGHT
{
    GREEN_LIGHT = 0,        //绿灯
    YELLOW_LIGHT,           //黄灯
    RED_LIGHT,              //红灯
    CLOSE_ALL_LIGHT,        //关闭所有
}PolitLight; 


enum SYSTEM_MSG_TYPE
{
    EVENT_USUALLY = 0,          //通用消息
    EVENT_SYS_STATUS,           //系统工作状态
    EVENT_WORK_FLOW_STATUS,     //工作流程状态
    EVENT_RECHECK_DATA,         //复检数据
    EVENT_PILOT_MSG,            //提示信息
    EVENT_ALARM_MSG             //报警信息
};


#define PDCA_PATH_MAX   256
#define PDCA_BUFFER_MAX 64
typedef struct _PdcaConfig
{
    char server[16];                         // 服务器IP
    int sport;                               // 服务器端口号
    char client[16];                         // 本地IP
    int cport;                               // 本地端口号
    char userName[PDCA_BUFFER_MAX];          // 用户名
    char userKey[PDCA_BUFFER_MAX];           // 密码
    char localDataPath[PDCA_PATH_MAX];       // 本地数据路径
    char localPicPath[PDCA_PATH_MAX];        // 本地图片路径
    char serverDataPath[PDCA_PATH_MAX];      // 服务器数据路径
    char serverPicPath[PDCA_PATH_MAX];       // 服务器图片路径
    char deviceName[PDCA_BUFFER_MAX];        // 设备名

    _PdcaConfig()
    {
        memset( this, 0, sizeof(_PdcaConfig) );
    }
}PdcaConfig;

// 串口参数
typedef struct _ComConfig
{    
    string name;
    int baudRate;   
    int parity;          /* Parity  0-4=None,Odd,Even,Mark,Space    */
    int byteSize;        /* ByteSize Number of bits/byte, 4-8       */
    int stopBits;        /* //停止位///* StopBits 1,3,2 = 1, 1.5, 2  */ 

    _ComConfig()
    {
        baudRate = 0;
        parity = 0;
        byteSize = 0;
        stopBits = 1;
    }
}ComConfig;

// 压力表相关参数
typedef struct _PressureConfig
{    
    char szName[16];
    int baudRate;   
    int parity;          /* Parity  0-4=None,Odd,Even,Mark,Space    */
    int byteSize;        /* ByteSize Number of bits/byte, 4-8       */
    int stopBits;        /* StopBits 0,1,2 = 1, 1.5, 2              */
    double upper;        /* 压力上限                                */
    double lower;        /* 压力下限                                */
    double pressure;     /* 压力阀值                                */
    int stopTime;        /* 保压时间                                */

    _PressureConfig()
    {
        memset( this, 0, sizeof(_PressureConfig) );
    }
}PressureConfig;

// 贴合补偿参数
typedef struct _SeatOffsetConfig
{
    double x;
    double y;
    double z;
    double ang;

    _SeatOffsetConfig()
    {
        memset( this, 0, sizeof(_SeatOffsetConfig) );
    }
}SeatOffsetConfig;

// 复检标准
typedef struct _ReviewConfig
{
    double xUp;         // x 上限
    double xDown;       // x 下限
    double yUp;         // y 上限
    double yDown;       // y 下限
    double angUp;       // 角度 上限
    double angDown;     // 角度 下限

    _ReviewConfig()
    {
        memset( this, 0, sizeof(_ReviewConfig) );
    }
}ReviewConfig;

//CCD测量数据结构
typedef struct ST_CCD_DATA
{
    double data[CCD_DATA_MAX];      //测量数据
    bool bResult;
    char szSN[64];
    ST_CCD_DATA()
    {
        memset(data, 0,sizeof(data));
        memset(szSN, 0,sizeof(szSN));
        bResult = false;
    }

    ST_CCD_DATA operator &=(ST_CCD_DATA copy)
    {
        for (int i = 0; i < CCD_DATA_MAX; i++)
        {
            data[i] = copy.data[i];
        }
        bResult = copy.bResult;
        strncpy(szSN, copy.szSN, strlen(copy.szSN));
        return *this;
    }

}CCDData;

//复检数据结构
struct  RecheckWorkData
{
    double data[CCD_DATA_MAX];
};

typedef struct ST_WORK_TIME
{
    uint64 StartTime;
    uint64 EndTime;

	ST_WORK_TIME()
	{
		StartTime = 0;
		EndTime = 0;
	}

    ST_WORK_TIME &operator=(ST_WORK_TIME copy)
    {
        StartTime = copy.StartTime;
        EndTime = copy.EndTime;
        return *this;
    }
}stWorkTime;

typedef struct ST_WORK_PRODUCE_TIME
{
    JTime StartTime;
    JTime EndTime;

    ST_WORK_PRODUCE_TIME &operator=(ST_WORK_PRODUCE_TIME copy)
    {
        StartTime = copy.StartTime;
        EndTime = copy.EndTime;
        return *this;
    }
}stWorkProduceTime;

//工作数据结构
typedef struct WORK_DATA
{
    CCDData SuckCatchAlignData;     //机器手吸嘴对位取料数据
    CCDData SuckFixAlignData;       //机器手吸嘴对位贴合数据
    CCDData PFAlignData;            //保护膜对位数据
    CCDData RecheckData;            //复检数据
    CCDData CheckAppearanceData;    //外观检测数据
    CCDData HSGAlignData;           //HSG检测数据
    char szResStr[64];				//NG PASS 结果 没有检测到物料
    stWorkTime WorkTime;            //生产时间记录
    char SN[MAX_SN_LENGTH];         //BarCode二维码
    char FGWIP_SN[MAX_SN_LENGTH];   //PDCA返回的码
    char PicName[MAX_PIC_NAME_LENGTH];  //图片名字
    double dLoadCell;               //机器手贴合保压值
	double dPressureValue;          //保压压力值
    int    Color;                   //CG物料颜色
    uint64 CircleTime;              //CT时间
    bool Result;                    //复检结果
	int Tossing;					// 做一个HSG抛了几次膜

	WORK_DATA()
	{
		memset(szResStr, 0, sizeof(szResStr));
		memset(SN, 0, sizeof(SN));
		memset(FGWIP_SN, 0, sizeof(FGWIP_SN));
		memset(PicName, 0, sizeof(PicName));
		dLoadCell = 0.0;
		dPressureValue = 0.0;
		Color = 0;
		CircleTime = 0;
		Result = false;
		Tossing = 0;
	}

	WORK_DATA &operator=(WORK_DATA copy)
	{
		SuckCatchAlignData = copy.SuckCatchAlignData;
		SuckFixAlignData = copy.SuckFixAlignData;
		PFAlignData = copy.PFAlignData;
		RecheckData = copy.RecheckData;
        CheckAppearanceData = copy.CheckAppearanceData;
		HSGAlignData = copy.HSGAlignData;
		WorkTime = copy.WorkTime;

		strncpy(szResStr, copy.szResStr, strlen(szResStr));
		strncpy(SN, copy.SN, strlen(SN));
		strncpy(FGWIP_SN, copy.FGWIP_SN, strlen(FGWIP_SN));
		strncpy(PicName, copy.PicName, strlen(PicName));
		dLoadCell = copy.dLoadCell;
		dPressureValue = copy.dPressureValue;
		Color = copy.Color;
		CircleTime = copy.CircleTime;
		Result = copy.Result;
		Tossing = copy.Tossing;
		return *this;
	}
}WorkData;
//线扫相杨参数
typedef struct _CAMERA_PARAM
{
    bool    bSaveSourceImage;    //是否保存原始图像
    bool    bEableStaticImage;   //是否使能静态校准
    bool    bEableDynamic;       //是否使能动态校准
    bool    bEableFilterImage;   //是否使能滤波
    bool    bSaveCorrectImage;   //是否保存校准图像
	double	dMoveUnit;			 // 相素精度(mm)
	double  dGetImageLen;        //视野长度(mm)
	double	dPluseWidgh;	     // 触发持续脉宽(ms)
	double	dFstartpos;		     // 到位延时距离(mm)
	int     Fivscount;           //单张图像采集行数
	int     iSequence;           //时序数
	int     iFiv;         //补偿触发次数  
    int  MoveLenth;           //最小采集时间---
	double  dSpeed;              //移动速度
	
	_CAMERA_PARAM()
	{
        bSaveSourceImage = true;
        bEableStaticImage = true;
        bEableDynamic = true;
        bEableFilterImage = true;
        bSaveCorrectImage = true;
		dMoveUnit    = 0.01;
		dGetImageLen = 10;
		dPluseWidgh  =0.001;
		dFstartpos   =0;	
		Fivscount    = 1000;   
		iSequence    = 1;   
		MoveLenth    = 500;   
		dSpeed       = 20;
		iFiv = 0;

	}

	_CAMERA_PARAM& operator=(const _CAMERA_PARAM &other)
	{
		if (this == &other)
		{
			return *this;
		}

        bSaveSourceImage  = other.bSaveSourceImage;
        bEableStaticImage = other.bEableStaticImage;
        bEableDynamic     = other.bEableDynamic;
        bEableFilterImage = other.bEableFilterImage;
        bSaveCorrectImage = other.bSaveCorrectImage;
		dMoveUnit   = other.dMoveUnit;
		dPluseWidgh = other.dPluseWidgh;
		dFstartpos= other.dFstartpos;
		dGetImageLen = other.dGetImageLen;
		Fivscount= other.Fivscount;
        MoveLenth = other.MoveLenth;
		dSpeed = other.dSpeed;
		iFiv = other.iFiv;
		iSequence = other.iSequence;
		return *this;
    }
    bool operator==(const _CAMERA_PARAM &other)
    {
        if (bSaveSourceImage != other.bSaveSourceImage
            || bEableStaticImage != other.bEableStaticImage
            || bEableDynamic     != other.bEableDynamic
            || bEableFilterImage != other.bEableFilterImage
            || bSaveCorrectImage != other.bSaveCorrectImage
            || dMoveUnit   != other.dMoveUnit
            || dPluseWidgh != other.dPluseWidgh
            || dFstartpos != other.dFstartpos
			|| iFiv != other.iFiv
            || dGetImageLen != other.dGetImageLen
            || Fivscount != other.Fivscount
            || MoveLenth != other.MoveLenth
            || dSpeed != other.dSpeed
			||iSequence != other.iSequence)
        {
            return false;
        }
        return true;
    }
}CameraParam;
#endif // BUSINISS_COMMON_H

