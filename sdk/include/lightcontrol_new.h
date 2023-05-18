#ifndef LIGHTCONTROL_NEW_H
#define LIGHTCONTROL_NEW_H
#include<list>
#include "g12globalenv.h"
#include "modbuscom.h"
#include "g12commonexport.h"
#include "lighthearder.h"
#include <map>
#include <string>
using namespace std;
#define  CD_Size			6		//	6 张卡
#define  Section_Size		4       //  分4 段 ; 条光 1，2，4 弧光 1，2，3
#define  Cam_Cnt			4	    //  相机个数
#define	 CFG_2K			0
#define	 CFG_8K			1
typedef enum {
	LIGHT_0 = 0,  				//主上光源		0
	LIGHT_1, 					//主下光源		1 
	LIGHT_2,					//上弧形光		2		
	LIGHT_3,					//侧面下光源	3
	LIGHT_4,					//垂直
	LIGHT_SIZE
}Light_Type;


typedef enum {
	Channel_Index_A = 0,  				// Channel	A 
	Channel_Index_B, 					// Channel	B 	
	Channel_Index_C,					// Channel	C 
	Channel_Index_D,  					// Channel	D 
	Channel_Index_E,					// Channel	E 
	Channel_Index_F,					// Channel	F 
	Channel_Index_G,  					// Channel	H 
	Channel_Index_H,					// Channel	G 
	Channel_Index_Size,
}Channel_Index;

typedef enum {
	Scan_line_1 = 0,  				//扫描0段
	Scan_line_2, 					//扫描1段
	Scan_line_3,					//扫描2段
	Scan_line_4,					//扫描3段
	Scan_line_5,					//扫描4段
	Scan_line_Size
}Scan_line_num;


typedef uint16_t EleValueArray[6][5];

typedef struct EleCfg
{
	LightSourceModel model;				 // 频闪模式0 , 常亮1
	EleValueArray EleValue[LIGHT_SIZE];  // 频闪电流值
	bool alwaysOn[LIGHT_SIZE][CD_Size];  // 常亮使能
}stEleCfg;

typedef struct LightEnaleCfg
{
	Light_Type type;					 // 光源类型
	bool enable[CD_Size][Section_Size];	 // 灯管使能
	uint16_t ipulse[6];	
};

typedef struct IllumChannelCfg
{
	LightEnaleCfg LightEnale[LIGHT_SIZE];	 // 使能;
	//int ipulse;							 // 脉宽
	//int icarema_delay;					 // 相机延时
	int iscan_line_count;					 // 扫描行数-用于切换下一种灯管配置
	bool CamEnable[Cam_Cnt];				 // 相机使能
	bool ChannelEnable;						 // 根据页面选项判断，通道是否使能
	int  CamEnableIndex;					 // 哪个相机使能,相机索引
}stIllumChannelCfg;


class IFileVariable;
class LightControlTcp;
class RESOURCE_LIBRARY_EXPORT lightcontrol_new
{
public:
	//static lightcontrol_new* GetInstance();
	//void   ReleaseInstance();
	lightcontrol_new(int index = 0);
	~lightcontrol_new() {};

	//清光源计数。
	bool ClearNumber();
	bool setCurrentInitEleFile(const string& FileName);
	string getCurrentInitEleFileName();

	/******************** 电流设置接口 ***************************/
	// 从文件获取电流配方。
	bool getInitEleCfg(EleCfg *lightParam);
	// 从保存电流配方到文件。
	bool setInitEleCfg(EleCfg *lightParam = NULL);
	// 下发电流配方到控制器。
	bool writeEleCfgToFlash(EleCfg *lightParam = NULL);
	// 读取电流配方从控制器
	bool readEleCfgFromFlash(EleCfg *lightParam);

	/******************** 通道配方接口 ***************************/
	bool getllumChannelCfg( Channel_Index index = Channel_Index_A, Scan_line_num line_index = Scan_line_1,stIllumChannelCfg *lightParam = NULL);
	bool setllumChannelCfg( Channel_Index index = Channel_Index_A, Scan_line_num line_index = Scan_line_1, stIllumChannelCfg *lightParam = NULL, string strSence ="");

	bool writellumChannelCfgToFlash(Channel_Index index = Channel_Index_A, Scan_line_num line_index = Scan_line_1, stIllumChannelCfg *lightParam = NULL, Light_Type type= LIGHT_SIZE);
	bool readllumChannelCfgToFlash(Channel_Index index = Channel_Index_A, Scan_line_num line_index = Scan_line_1, stIllumChannelCfg *lightParam = NULL);

	/* 1:获取控制器类型-上下光源或者弧形光,函数返回0成功，-1失败*/
	int  getCtrType(uint8_t &ls_type, Light_Type type = LIGHT_SIZE);
	
	/*
	*2:控制常亮或频闪模式
	* ls_model(8位)--0频闪 1常亮
	* 函数返回0成功，-1失败
	*/
	int setFlashMode( uint8_t ls_model, Light_Type type = LIGHT_SIZE);

	/*3:常亮模式下灯管开关,函数返回0成功，-1失败*/
	int setLsSwitch(Light_Type type = LIGHT_SIZE);

	/*
	* 4:开启自充电模式,
	* uint8_t					ls_auto_charge;					//开启自动充电模式/0关闭/1开启
	*函数返回0成功，-1失败
	*/
	int setAutoCharge(uint8_t ls_auto_charge, Light_Type type = LIGHT_SIZE);

	/* 5:清除计数指令-即每扫完一次需要清除计数，为下一次做准备,
	*	uint8_t	ls_config_id;						//配方选择 0，1 
	*	uint16_t[8](每个通道实际触发次数)
	*   函数返回0成功，-1失败
	*/
	int clearCounter(  Light_Type type = LIGHT_SIZE);

	/* 5:获取计数指令,
	*	uint8_t	ls_config_id;						//配方选择 0，1
	*	uint16_t[8](每个通道实际触发次数)
	*   函数返回0成功，-1失败
	*/
	int getCounter( uint16_t  trig[8], Light_Type type = LIGHT_SIZE);

	/*6:下发每个通道电流设置,函数返回0成功，-1失败*/
	//用于控制电流->6张卡，每张卡4通道电流参数;
	int setLsCurrent(Light_Type type = LIGHT_SIZE);

	/*
	7:单个通道下发
	*  uint8_t	ls_config_id;						//配方选择 0，1 
	*  uint8_t channel								通道 0-7
	*  函数返回0成功，-1失败
	*/
	int setMutFlashParamsByChannel( uint8_t channel,  Light_Type type = LIGHT_SIZE);

	/*
	8:单个通道下发
	* uint8_t	ls_config_id;						//配方选择 0，1 
	* 函数返回0成功，-1失败
	*/
	int setMutFlashParams( Light_Type type = LIGHT_SIZE);

	/*
	* 9:一次下发所有数据
	* 函数返回0成功，-1失败
	*/
	int setAllMutFlashParams( Light_Type type = LIGHT_SIZE);

	static string  ChannelIndexToName(int index);
	static Channel_Index ChannelNameToIndex(string strChannelName);

private:

    bool readEleConfig();
    bool saveEleConfig();
	bool readAllChannelCfg();
	bool saveAllChannelCfg();
	void initLightTcp();
    string getSenceName();
private:
	string m_strLightEleFileFileName;
	stEleCfg m_currEleCfg;    //用于控制电流->6张卡，每张卡4通道电流参数;
	IFileVariable* m_pCurrentLightEleFile;
	IFileVariable* m_pDefalueLightEleFile;
    LightControlTcp* m_lightControlTcp[LIGHT_SIZE];

	bool m_lightEnabled[LIGHT_SIZE];
	LightSource_Params m_LightSource_Params;
	int m_index;
private:
	stIllumChannelCfg m_stIllumChannelCfg[Channel_Index_Size][Scan_line_Size];
};

class RESOURCE_LIBRARY_EXPORT motioncontrol_new
{
public :

	motioncontrol_new(int index = 0);
	~motioncontrol_new() {};
	////////
	/*
	* 10:一次下发控制器数据
	* 函数返回0成功，-1失败
	*/
	int setToController(ControllerParams_t Params);
	/*   NetId  1 2
	* 11:下发命令到控制器-仅支持
	Order;//命令控制
	1->获取控制器           (CrlNumber 任意值（返回0正常）
	3->工作模式设置        （CrlNumber 0-等间距触发;1-静态触发）
	4->停止工作和开始工作  （CrlNumber 0-停止工作;1-开始工作）
	* 函数返回0成功，-1失败
	*/
	int setOrderToController(uint8_t NetId, Order_num Order, uint8_t CrlNumber = 0);
	//0x01（网络ID号:0x01=客户端1;0x02=客户端2）+0x05（命令控制）+0x01(边检=0x01;平面=0x02)+0x01 / 0x02 (0x01=开始触发; 0x02=结束触发)
	// 返回值  1 开始触发  2 结束触发   其他 未就绪
	int getControllerTargetStatus(uint8_t NetId, Mark_Type stType);


	bool isEnable();
private:
	LightControlTcp* m_Control;
	ControllerParams_t m_ControllerParams;
	int m_index;
	bool m_bEnalbe;
};

class RESOURCE_LIBRARY_EXPORT lightcontrol_new_manager
{
public:
	static lightcontrol_new_manager* GetInstance();
	void   ReleaseInstance();
	lightcontrol_new*  getLightcontrol(int index);
	void releaseLightcontrol(int index);

private:
	lightcontrol_new_manager();
	~lightcontrol_new_manager();
	static lightcontrol_new_manager* m_pManagerInstance;
	lightcontrol_new* lightcontrols[8];

};

class RESOURCE_LIBRARY_EXPORT motioncontrol_new_manager
{
public:
	static motioncontrol_new_manager* GetInstance();
	void   ReleaseInstance();
	motioncontrol_new*  getMotioncontrol(int index);
	void releaseMotioncontrol(int index);

private:
	motioncontrol_new_manager();
	~motioncontrol_new_manager();
	static motioncontrol_new_manager* m_pManagerInstance;
	motioncontrol_new* motioncontrols[8];

};

#endif // LIGHTCONTROL_NEW_H
