#ifndef LIGHT_HEARDER_H
#define LIGHT_HEARDER_H
#include "OS_DEFINE.h"

typedef unsigned int				uint32_t;			/*!<[CH]32 bit unsigned <BR>[EN]32 bit unsigned */
typedef unsigned char				uint8_t;
typedef unsigned short				uint16_t;

typedef enum {         
   LS_TYPE_UP = 0,  			//上光源		0
   LS_TYPE_DOWN, 				//下光源		1 
   LS_TYPE_LOW,				//低角度光源	2
   LS_TYPE_UP_ARC,				//上弧形光	3		
   LS_TYPE_UP_FRONT_ARC,		//上前弧形光	4
   LS_TYPE_UP_REAR_ARC,		//上后弧形光	5
   LS_TYPE_UP_FLANK_ARC,		//上侧弧形光	6	
   LS_TYPE_DN_ARC,				//下弧形光	7	
   LS_TYPE_DN_FRONT_ARC,		//下前弧形光	8
   LS_TYPE_DN_REAR_ARC,		//下后弧形光	9
   LS_TYPE_DN_FLANK_ARC,		//下侧弧形光	10		
}LightSource_Type;

typedef enum {         
   LS_MODEL_FLASH = 0,			//频闪模式	0  
   LS_MODEL_NORMAL 			//常亮		1
}LightSourceModel;

//条光 1，2，4 弧光 1，2，3
typedef struct
{
	uint32_t  ls_switch_cd1_1:1;		//常亮模式，第1张卡第1通道光源开关   低亮
	uint32_t  ls_switch_cd1_2:1;
	uint32_t  ls_switch_cd1_3:1;
	uint32_t  ls_switch_cd1_4:1;
	
	uint32_t  ls_switch_cd2_1:1;		//常亮模式，第2张卡第1通道光源开关   高亮
	uint32_t  ls_switch_cd2_2:1;
	uint32_t  ls_switch_cd2_3:1;
	uint32_t  ls_switch_cd2_4:1;
	
	uint32_t  ls_switch_cd3_1:1;		//常亮模式，第3张卡第1通道光源开关   暗场5度
	uint32_t  ls_switch_cd3_2:1;
	uint32_t  ls_switch_cd3_3:1;
	uint32_t  ls_switch_cd3_4:1;
	
	uint32_t  ls_switch_cd4_1:1;		//常亮模式，第4张卡第1通道光源开关  暗场10度 
	uint32_t  ls_switch_cd4_2:1;
	uint32_t  ls_switch_cd4_3:1;
	uint32_t  ls_switch_cd4_4:1;
	
	uint32_t  ls_switch_cd5_1:1;		//常亮模式，第5张卡第1通道光源开关  测光源
	uint32_t  ls_switch_cd5_2:1;
	uint32_t  ls_switch_cd5_3:1;
	uint32_t  ls_switch_cd5_4:1;
	
	uint32_t  ls_switch_cd6_1:1;		//常亮模式，第6张卡第1通道光源开关   
	uint32_t  ls_switch_cd6_2:1;
	uint32_t  ls_switch_cd6_3:1;
	uint32_t  ls_switch_cd6_4:1;
	
	uint32_t  reserve:8;
}LightSourceSwitch , *pLightSourceSwitch;


typedef union 
{
	uint32_t  			ui_switch;
	LightSourceSwitch	lss_switch;
}LightSourceSwitch_t , *pLightSourceSwitch_t;

typedef struct
{
	uint8_t		ct_carema_1:1;		
	uint8_t		ct_carema_2:1;
	uint8_t		ct_carema_3:1;
	uint8_t		ct_carema_4:1;
	uint8_t		ct_reserve:4;
}CaremaConfig , *pCaremaConfig;

typedef union 
{
	uint8_t 		us_carema_config;
	CaremaConfig  	carema_config;
}CameraConfig_t , *pCameraConfig_t;

typedef struct
{
	LightSourceSwitch_t			ls_switch_cofnig;		//配置灯管开关
	CameraConfig_t				ls_carema_config;		//相机配置	
	uint16_t					ls_pulse[6];			//脉宽
	//uint16_t					ls_carema_delay;		//相机延时
	uint16_t					ls_scan_line_count;		//扫描行数-用于切换下一种灯管配置 0-5 段
}MutFlashParams , *pMutFlashParams;

typedef struct 
{
	uint8_t  				ls_type;							//请查看LightSource_Type-用于区分上下光源或者弧形光源等

	uint8_t					ls_model;							//光源控制器工作模式->频闪=LS_MODEL_FLASH；常亮=LS_MODEL_NORMAL-默认频闪;

	LightSourceSwitch_t		ls_switch;							//LightSourceSwitch-常亮模式下控制光源控制器开和关
	
	uint8_t					ls_auto_charge;					//开启自动充电模式/0关闭/1开启
	
	uint16_t  				ls_current[6][4];					//用于控制电流->6张卡，每张卡4通道电流参数;

	uint8_t					ls_config_id;						//配方选择 0，1
	MutFlashParams			ls_mut_params_config[2][8][5];	//MutFlashParams-2种配方8通道每通道5段参数配置	
}LightSource_Params , *pLightSource_Params;

/******************************************************************************************************
通讯协议
包头+指令+数据+CRC(16位)
*******************************************************************************************************
1:获取控制器类型-上下光源或者弧形光
发
0x55aa(包头)+0x01(指令)
回
0x55aa(包头)+0x01(指令)+ls_type

2:控制常亮或频闪模式
发
0x55aa(包头)+0x02(指令)+ls_model(8位)	--0频闪 1常亮
回
0x55aa(包头)+0x02(指令)+操作结果   --0失败 1 正常

3:常亮模式下灯管开关
发
0x55aa(包头)+0x03(指令)+ls_switch
回
0x55aa(包头)+0x03(指令)+操作结果   --0失败 1 正常

4:开启自充电模式
发
0x55aa(包头)+0x04(指令)+ls_auto_charge(8位)
回
0x55aa(包头)+0x04(指令)+操作结果   --0失败 1 正常

5:清除计数指令-即每扫完一次需要清除计数，为下一次做准备
发
0x55aa(包头)+0x05(指令)+ls_config_id(8位)   --从0开始
回
0x55aa(包头)+0x05(指令)+uint16_t[8](每个通道实际触发次数)

6:下发每个通道电流设置
发
0x55aa(包头)+0x06(指令)+ls_current+CRC(16位)
回
0x55aa(包头)+0x06(指令)+操作结果   --0失败 1 正常

7:单个通道下发
发
0x55aa(包头)+0x07(指令)+ls_config_id(8位)+N通道(8位)+MutFlashParams[5]+CRC(16位)
回
0x55aa(包头)+0x07(指令)+操作结果   --0失败 1 正常

8:单个通道下发
发
0x55aa(包头)+0x08(指令)+ls_config_id(8位)+MutFlashParams[8][5]+CRC(16位)
回
0x55aa(包头)+0x08(指令)+操作结果   --0失败 1 正常

9:一次下发所有数据
发
0x55aa(包头)+0x09(指令)+MutFlashParams[2][8][5]+CRC(16位)
回
0x55aa(包头)+0x09(指令)+操作结果   --0失败 1 正常




*******************************************************************************************************/

/********************************************add 2020 / 06 / 01

ControllerParams

1:net_id;					//网络ID号
客户端编号：1和2

2:order;					//命令控制
1->获取控制器
2->设置参数（延时触发脉冲、等间距脉冲、静态触发频率）
3->工作模式设置（0-等间距触发;1-静态触发）
4->停止工作和开始工作（0-停止工作;1-开始工作）

例如：
发送:0x01（网络ID号）+0x01（命令控制）+0x55+0xaa
返回:0x01（命令控制）+0x55+0xaa

发送:0x01（网络ID号）+0x02（命令控制）+ ControllerParams_t(结构体)
返回:0x01（命令控制）

发送:0x01（网络ID号）+0x03（命令控制）+ 1个字节（工作模式设置（0-等间距触发;1-静态触发））
返回:0x01（命令控制）

发送:0x01（网络ID号）+0x04（命令控制）+ 1个字节（工作模式设置（0-停止工作;1-开始工作）） -- 开始工作指：1：处于静态触发模式时，侧开始静态触发频闪。2：处于等间距触发时，侧开始流水等间跑触发拍照
返回:0x01（命令控制）

检测到光纤主动返回：

0x01（网络ID号:0x01=客户端1;0x02=客户端2）+0x05（命令控制）+0x01(边检=0x01;平面=0x02)+0x01 / 0x02 (0x01=开始触发; 0x02=结束触发)
***********************************************************/

typedef enum {
	Order_GetCrl = 1, //1->获取控制器
	Order_SetParms,     // 2->设置参数（延时触发脉冲、等间距脉冲、静态触发频率）
	Order_WorkType,     //3->工作模式设置（0 - 等间距触发; 1 - 静态触发）
	Order_WorkStus,     //4->停止工作和开始工作（0 - 停止工作; 1 - 开始工作）
	Order_Size
}Order_num;
typedef enum {
	Mark_Edgee  = 1,//边检=0x01
    Mark_Surface    //平面=0x02
}Mark_Type;
//延时脉冲个数
typedef struct DelayPulse
{
	uint32_t	delay_pulse_1;		//通道1
	uint32_t	delay_pulse_2;		//通道2
	DelayPulse()
	{
		delay_pulse_1 = 0;
		delay_pulse_2 = 0;
	}
}DelayPulse_t, *pDelayPulse_t;
//等间距触发个数
typedef struct EqualSpace
{
	uint16_t	pulse_number_1;		//通道1等间距脉冲数量
	uint16_t	pulse_number_2;		//通道2等间距脉冲数量
	EqualSpace()
	{
		pulse_number_1 = 0;
		pulse_number_2 = 0;
	}
}EqualSpace_t, *pEqualSpace_t;

//触发总数
typedef struct AllPulse
{
	uint32_t	all_pulse_1;		//通道1等间距触发总数
	uint32_t	all_pulse_2;		//通道2等间距触发总数
	AllPulse()
	{
		all_pulse_1 = 0;
		all_pulse_2 = 0;
	}
}AllPulse_t, *pAllPulse_t;

//控制器参数设置
typedef struct ControllerParams
{
	uint8_t		net_id;			    //网络ID号
	uint8_t		order;			    //命令控制
	DelayPulse_t	delay_pulse;    //延时触发脉冲数量
	EqualSpace_t	equal_space;    //等间距触发
	AllPulse_t	all_pulse;		    //等间距触发总脉冲数
	uint16_t	trigger_frequence;	//静态触发频率
	ControllerParams()
	{
		net_id = 1;
		order  = 1;
		trigger_frequence = 0;
		memset(&delay_pulse, 0, sizeof(DelayPulse_t));
		memset(&equal_space, 0, sizeof(EqualSpace_t));
		memset(&all_pulse, 0, sizeof(AllPulse_t));
	}
}ControllerParams_t, *pControllerParams_t;
#endif 