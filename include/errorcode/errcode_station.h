#ifndef __ERROR_CODE_STATION_DEFINE_H__
#define __ERROR_CODE_STATION_DEFINE_H__


#ifndef SYS_ERR_STATION_START
#define SYS_ERR_STATION_START   2500					// station误码起始 
#endif

enum EM_ERR_STATION								// station误码起始 
{
    //工站错误码
    SYS_ERR_STATION_INIT_FAIL = SYS_ERR_STATION_START,			//工站初始化失败
    SYS_ERR_STATION_NO_ID,						//无效的工站ID
    SYS_ERR_STATION_NO_CARD,					//无可用卡
    SYS_ERR_STATION_NO_EXTMODULE,				//无扩展卡
    SYS_ERR_STATION_NO_VALIDAXIS,				//无可用轴
    SYS_ERR_STATION_ROBOT_CONTRL_FAIL,			//机器人操作失败
    SYS_ERR_STATION_CARD_CONTRL_FAIL,			//卡工站操作出错
    SYS_ERR_STATION_ROBOT_EXEC,				//robot返回失败
    SYS_ERR_STATION_NO_CONNET,					//工站连接失败
    SYS_ERR_STATION_GET_INSTANCE,				//获取对象失败
    SYS_ERR_STATION_GET_PARAM,					//获取参数失败
    SYS_ERR_STATION_QUERY_INTEFACE,			//查询接口失败
    SYS_ERR_STATION_CREAT_PT_FILE,				//创建点文件失败
    SYS_ERR_STATION_READ_INTEFACE,				//读取点文件失败
    SYS_ERR_STATION_SAVE_INTEFACE,				//保存点文件失败
    SYS_ERR_STATION_NO_POINT,					//没有找到对应的点信息
    SYS_ERR_STATION_NO_FUN,					//没有接口实现
    SYS_ERR_STATION_MOVING,            //下发运动指令时,工站正在运动中
    SYS_ERR_STATION_NO_IO,						//没有对应io实例
    SYS_ERR_STATION_NO_AXIS,					//没有对应轴实例
    SYS_ERR_STATION_NO_AXIS_PARA_MSG,			//没有对应轴的参数
    SYS_ERR_STATION_AXIS_PARA_MSG,				//轴参数错误
    SYS_ERR_STATION_NO_INIT,					//模块没有初始化
    SYS_ERR_STATION_NO_READY,                   //工站未准备好
    SYS_ERR_STATION_STOP,						//停止轴运动失败
    SYS_ERR_STATION_ALWAYS_MOVE,				//持续运动失败
    SYS_ERR_STATION_MOVE_POS,					//点位移动失败	
    SYS_ERR_STATION_IO,						   //IO读取或操作失败
    SYS_ERR_STATION_GET_POS,					//获取当前点位置数据失败
    SYS_ERR_STATION_SET_CRDPRM,				//建立坐标系失败
    SYS_ERR_STATION_SET_BUFF_CMD,				//设置插补缓存区命令失败
    SYS_ERR_STATION_MOVE_CRDPRM,				//插补运动失败
    SYS_ERR_STATION_STAR_CRTDPRM,				//启运插补运动失败
    SYS_ERR_STATION_GET_CRDPRM_STATUS,			//获取插补运动状态失败
    SYS_ERR_STATION_ALCEAR_CRD,				//清除插补缓存区内的插补数据失败
    SYS_ERR_STATION_SET_CRD_IO,				//缓存区内数字量IO输出设置指令失败
    SYS_ERR_STATION_SET_CRD_DELAY,				//缓存区内延时设置指令失败
    SYS_ERR_STATION_GET_CRD_SPACE,				//查询插补缓存区剩余空间失败
    SYS_ERR_STATION_GET_CRD_INDEX,				//获取未完成的插补段数失败
    SYS_ERR_STATION_MM_TO_PULSE,				   //毫米转化为脉冲失败
    SYS_ERR_STATION_RESET,						   //重设失败
    SYS_ERR_STATION_INIT_INTERP,			   //初始化插补失败
    SYS_ERR_STATION_PARAM,							//输入参数错误
    SYS_ERR_STATION_HOME_FAILED,		         //回原失败
    SYS_ERR_STATION_DEVIATE_POINT,               //偏离点位（误差大于设定的差值）
    SYS_ERR_STATION_SET_INTERP_SPEED,           // 设置插补速度失败
    SYS_ERR_STATION_PAUSE,							// 暂停失败
    SYS_ERR_STATION_HAS_ALARM,					// 有轴报警
};
#endif
