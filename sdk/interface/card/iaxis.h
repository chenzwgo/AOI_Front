#ifndef _AXIS_INTERFACE_H_
#define _AXIS_INTERFACE_H_

#include "motor_common.h"

class IAxis
{
public:
	//不在构造函数里面进行轴的初始化，需要调用init()
	enum
	{
		IID_AXIS = 0x302
	};
	virtual ~IAxis(){}

	virtual bool Init() = 0;	// 轴初始化
	virtual bool On() = 0;		// 轴使能
	virtual bool Off() = 0;		// 轴关闭

	virtual bool SetAxisParam(StAxisParam* pAxisParam) = 0;		//设置轴参数
	virtual bool GetAxisParam(StAxisParam* pAxisParam) = 0;		//获取轴参数
	virtual bool SetLmtValue(double limitP, double limitN) = 0;		//设置轴正向软限位和负向软限位
	virtual bool GetLmtValue(double &limitP, double &limitN) = 0;	//获取轴正向软限位和负向软限位
	
	virtual bool SetSpeed(const StSpeed &speed) = 0;    // 设置速度
	virtual bool GetSpeed(StSpeed &speed) = 0;           // 获取速度
	virtual bool MovePosRel(double pos) = 0;            // 移动到指定位置,相对位置
	virtual bool MovePosAbs(double pos) = 0;            // 移动到指定位置,绝对位置
	virtual bool MoveSucceed(double pos) = 0;		    // 是否已经移动到指定位置
	virtual bool GetCurPos(double &pos) = 0;		    // 获取当前位置
	virtual bool GetActualPos(double &pos) = 0;         // 获取实际位置,专用于卡IO调机界面
	virtual bool GetCommandPos(double &pos) = 0;        // 获取规划位置,专用于卡IO调机界面

	virtual bool MoveJog(bool bDirection = true) = 0;   // JOG运动，默认正方向
	
	virtual bool EStop() = 0;							// 急停
	virtual bool SmoothStop() = 0;						// 平滑停止
	virtual bool SetPosZero() = 0;						// 位置清零
	virtual bool ClearStatus() = 0;                     // 清除状态
	virtual bool ClearAlarm() = 0;                     // 清除报警
	virtual bool IsMoving() = 0;						// 轴是否在移动中
	virtual bool IsOn() = 0;							// 判断轴已经打开
	virtual bool IsLimitP() = 0;						// 判断是否到达正限位
	virtual bool IsLimitN() = 0;                        // 判断是否到达负限位
	virtual bool IsDrvAlarm() = 0;                      // 判断驱动器报警
	virtual bool IsHomeSig() = 0;						// 判断原点信号

	//回原
	virtual bool MoveToLimitN(bool bFindNLimit = true) = 0;	// 移动到负限位,true-必须找到限位，false-找到原点停下
	virtual bool MoveToLimitP(bool bFindNLimit = true) = 0;	// 移动到正限位,true-必须找到限位，false-找到原点停下
	virtual bool Home() = 0;							// 回原点
	virtual EnMoveBool HomeStatus() = 0;					// 是否回原完成
	virtual bool SetPos(EnPosMode model, double dValue) = 0;	// 设置位置
};

#endif
