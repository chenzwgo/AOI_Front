#ifndef __ERROR_CODE_SAFECHECK_DEFINE_H__
#define __ERROR_CODE_SAFECHECK_DEFINE_H__

#ifndef SYS_ERR_SAFECHECK_START
#define SYS_ERR_SAFECHECK_START   1500					// safecheck错误码起始 
#endif

enum EM_ERR_SAFECHECK						// safecheck错误码起始 
{
	//配置文件操作
	SYS_ERR_SAFE_INIT = SYS_ERR_SAFECHECK_START,				// 安全检查初始化失败
	SYS_ERR_SAFE_CFG_INIT,				// 安全检查配置初始化失败
	SYS_ERR_SAFE_LOAD_ITEM_CFG,			// 加载安全检测项目配置文件失败
	SYS_ERR_SAFE_LOAD_PARAM_CFG,			// 加载安全检测参数配置文件失败
	SYS_ERR_SAFE_SAVE_ITEM_CFG,			// 保存安全检测项目配置文件失败
	SYS_ERR_SAFE_SAVE_PARAM_CFG,			// 保存安全检测参数配置文件失败
	SYS_ERR_SAFE_PARAM,					// 参数错误
	SYS_ERR_SAFE_ALARM,					// 检测报警
	SYS_ERR_SAFE_NO_PROJECT,				// 没有检测流程
	SYS_ERR_SAFE_NO_GROUP,				// 没有检测流程
	SYS_ERR_SAFE_NO_ITEM,					// 没有检测项
	SYS_ERR_SAFE_IO,						// IO不安全
	SYS_ERR_SAFE_PT,						// 点位不安全
	SYS_ERR_SAFE_POS,						// 位置不安全
	SYS_ERR_SAFE_AXIS,					// 轴不在安全范围
	SYS_ERR_SAFE_NETWORK,					// 网络异常
	SYS_ERR_SAFE_COM,						// COM状态异常
	SYS_ERR_SAFE_CARD,					// Card状态异常
	SYS_ERR_SAFE_EXIST_PROJECT_NAME,		// project 名字冲突
	SYS_ERR_SAFE_EXIST_PROJECT_DESC,		// project 描述冲突
	SYS_ERR_SAFE_EXIST_GROUP_NAME,			// group 名字冲突
	SYS_ERR_SAFE_EXIST_GROUP_DESC,			// group 描述冲突
	SYS_ERR_SAFE_POINT_INFO,				// point 信息错误
	SYS_ERR_SAFE_IO_INFO					// io 信息错误
};
#endif  //__ERROR_CODE_SAFECHECK_DEFINE_H__
