#ifndef __ERROR_CODE_CARD_DEFINE_H__
#define __ERROR_CODE_CARD_DEFINE_H__

#ifndef SYS_ERR_CARD_START
#define SYS_ERR_CARD_START   2500					// card错误码起始 
#endif

enum EM_ERR_CARD						//card错误码起始 
{
	//卡错误码 
    SYS_ERR_CARD_INIT_FAIL = SYS_ERR_CARD_START,				//控制卡初始化失败
    SYS_ERR_CARD_OPEN_CARD_FALSE,			  //打开卡失败 
    SYS_ERR_CARD_NO_CARDINSTANCE,				  //没有对应卡实例
    SYS_ERR_CARD_AXIS_HOME,					  //查询自动回原点的运行状态失败
    SYS_ERR_CARD_SET_CAPTURE,				  //设置编码器捕获方式失败（回原）
    SYS_ERR_CARD_CLEAR_STATUS,		          //清状态失败
    SYS_ERR_CARD_SET_ZERO,                    //位置清零失败
    SYS_ERR_CARD_CFG,					      //加载控制卡配置文件失败
    SYS_ERR_CARD_AXIS_MOVE_N,			      //回原时移动到负限位失败
    SYS_ERR_CARD_AXIS_MOVE_P,			      //回原时移动到正限位失败
    SYS_ERR_CARD_AXIS_ON,					   //轴使能失败
    SYS_ERR_CARD_AXIS_IN_LIMITN,			  //正在负限位
    SYS_ERR_CARD_AXIS_STOP,			          //停止失败
    SYS_ERR_CARD_SPEED_ZERO,                  //速度为0
    SYS_ERR_CARD_HOME_MODE,                   //回原模式错误
    SYS_ERR_CARD_COMPARE_POS,                 //位置比较输出错误
    SYS_ERR_CARD_GET_ADC,                     //获取电压值失败
    SYS_ERR_CARD_CTRL_FAIL,                   // 操作失败

    //mcs卡错误码
    SYS_ERR_MOVE_N,					//回原时移动到限位失败
    SYS_ERR_CLEAR_STATUS_SET_ZERO,	//清状态失败或位置清零失败
    SYS_ERR_AXIS_HOME,				//查询自动回原点的运行状态失败
    SYS_ERR_HOME					//回原失败
};

#endif
