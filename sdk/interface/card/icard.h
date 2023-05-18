#ifndef _CARD_INTERFACE_H_
#define _CARD_INTERFACE_H_

#include "motor_common.h"
class IAxis;
class IIO;

class ICard 
{
public:
	virtual ~ICard(){}

	/**
	 * @brief 初始化卡
	 * @param cardIndex				-- [in]卡号，取值范围：[0,7]
	 * @param axisCnt				-- [in]轴数目，取值范围：[0,7]
	 * @param extIndex				-- [in]扩展卡索引,0为本卡
	 */
	virtual bool Init(unsigned short cardIndex, unsigned short axisCnt, unsigned short extIndex) = 0;					// 初始化卡
	virtual bool Deinit() = 0;					// 释放卡
public:
	
	virtual IAxis* GetAxis(unsigned short index) = 0;	// 获取轴
	virtual IIO* GetIO(unsigned short index) = 0;		// 获取IO

	// 运动控制卡的基本操作
	virtual bool Open() = 0;					// 打开运动卡
	virtual bool Close() = 0;					// 关闭运动卡
	virtual bool Reset() = 0;					// 复位,不是回原
	virtual unsigned int GetCardIndex() = 0;	// 获取卡索引
	virtual unsigned int GetAxisCnt() = 0;		// 获取轴数量
	virtual bool IsConnected() = 0;				// 判断卡是否连接控制器
	virtual bool EmgStop() = 0;                 // 停止所有轴
	virtual bool CardHome(int homeAxis[], int axisCount) =0; //卡回原，传入的各轴同时回原
	virtual EnMoveBool CardHomeStatus(int homeAxis[], int axisCount) = 0;    //卡回原状态，MOVE_DOING回原中/MOVE_TRUE回原完成/MOVE_FALSE回原失败

};

#endif
