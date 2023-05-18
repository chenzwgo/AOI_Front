#ifndef IBUSINESS_H
#define IBUSINESS_H

#include "StatusEnumInfo.h"

class IBusiness
{
public:
	enum
	{
        IDD_BUSINESS = 0x001
	};

    virtual ~IBusiness(){}
	// 初始化
	virtual bool init() = 0;
	// 去初始化
	virtual bool deinit() = 0;
	// 开始
	virtual bool StartWork() = 0;
	// 暂停
	virtual bool PauseWork() = 0;
	// 继续
	virtual bool ContinueWork() = 0;
	// 停止
	virtual bool StopWork() = 0;
	// 急停
	virtual bool EstopWork() = 0;
	// 复位
	virtual bool ResetWork() = 0;
	// 工作状态
    virtual E_MAIN_WORK_STATE GetWorkState() = 0;
};

#endif // IWORKFLOW

