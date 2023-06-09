#ifndef __ERROR_CODE_MAINFLOW_DEFINE_H__
#define __ERROR_CODE_MAINFLOW_DEFINE_H__


#ifndef SYS_ERR_MAINFLOW_START
#define SYS_ERR_MAINFLOW_START   300					// mainflow错误码起始 
#endif


enum EM_ERR_MAINFLOW						//mainflow错误码起始 
{ 
	SYS_ERR_MAINFLOW_NOINSTANCE = SYS_ERR_MAINFLOW_START,				// 没有对象实例
	SYS_ERR_MAINFLOW_FILE,          //流程配置文件错误
	SYS_ERR_MAINFLOW_REPEATNAME,    //重名
	SYS_ERR_MAINFLOW_NOPOINT,        //流程没有此点位
	SYS_ERR_MAINFLOW_NOSTATION,      //流程没有此工站
	SYS_ERR_MAINFLOW_NOWORKPROC,             //没有此流程
	SYS_ERR_MAINFLOW_WORK_FLOW,				// WORK_FLOW  结构体 内部参数错误
	SYS_ERR_MAINFLOW_WORK_STEP,				// WORK_STEP  结构体 内部参数错误
	SYS_ERR_MAINFLOW_WORK_POINT,				// WORK_POINT 结构体 内部参数错误
	SYS_ERR_MAINFLOW_UNINIT,					//未初始化
	SYS_ERR_MAINFLOW_PARAM,					//入参错误
	SYS_ERR_MAINFLOW_ACTION_PARAM,           //Action 参数错误
	SYS_ERR_MAINFLOW_RUNING,           // 运行中
	SYS_ERR_MAINFLOW_AXIS_NO_INIT,      // 轴未初始化
	SYS_ERR_MAINFLOW_IO_NO_INIT,        // IO未初始化
	SYS_ERR_MAINFLOW_FLOWNAME,   // 流程名称为空
	SYS_ERR_MAINFLOW_STEPNAME,   // 步骤名称为空
	SYS_ERR_MAINFLOW_ACTIONNAME,   // 动作名称为空
	SYS_ERR_MAINFLOW_INTERPLOATE,  // 插补运动失败
	SYS_ERR_MAINFLOW_FUNCTION	,  // 动作函数执行失败
	SYS_ERR_MAINFLOW_CHANGE_SPEED	, //改速度失败
	SYS_ERR_MAINFLOW_MOVE_PIONT	, //行动点位执行失败
	SYS_ERR_MAINFLOW_STOP	,       //执行停止失败
	SYS_ERR_MAINFLOW_MISOPERATION,	  // 误操作，例如，没有暂停，点接继续
	SYS_ERR_MAINFLOW_NO_FUNCTION,     // 找不到要执行的函数
	SYS_ERR_MAINFLOW_NO_EVENT,        // 找不到该事件
	SYS_ERR_MAINFLOW_STATUS,		// 状态异常
    SYS_ERR_MAINFLOW_NO_ACTION,       //没有动作或没有动作勾选
    SYS_ERR_MAINFLOW_FLOW_FUN,         //流程函数执行失败
};
#endif
