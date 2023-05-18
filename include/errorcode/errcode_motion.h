#ifndef __ERROR_CODE_MOTION_DEFINE_H__
#define __ERROR_CODE_MOTION_DEFINE_H__

#ifndef SYS_ERR_MOTION_START
#define SYS_ERR_MOTION_START   1000					// motion错误码起始 
#endif

enum EM_ERR_MOTION						//运动控制错误代码
{
	//配置文件操作
	SYS_ERR_STATION_POINT_CFG_OPEN = SYS_ERR_MOTION_START,    //点文件打开错误
	SYS_ERR_STATION_CFG,				//工站配置错误
	SYS_ERR_STATION_LOAD_AXIS_CFG,      // 加载配置文件失败
	SYS_ERR_MOTION_GET_INSTANCE,       //获取motion对象失败
	SYS_ERR_MOTION_GET_CFG,				//构造配置文件对像失败
	SYS_ERR_MOTION_CFG_INIT,					//初始化配置文件失败
	SYS_ERR_MOTION_STATION_LOAD,		//加载工站配置信息失败
	SYS_ERR_MOTION_LOAD_CARD_CFG,				//加载卡配置失败
	SYS_ERR_MOTION_LOAD_IO_CFG,				//加载IO配置失败
	SYS_ERR_MOTION_SAVE_CARD_CFG,				//保存卡配置失败
	SYS_ERR_MOTION_SAVE_AXIS_CFG,				//保存轴配置失败
	SYS_ERR_MOTION_SAVE_IO_CFG,				//保存IO配置失败
};


#endif // __ERROR_CODE_MOTION_DEFINE_H__
