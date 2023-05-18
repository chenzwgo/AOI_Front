#ifndef _GG_IO_H_
#define _GG_IO_H_

#include "card/iio.h"
// 通用IO控制类
class CIO : public IIO
{
public:
	CIO(unsigned short cardIndex);
	virtual ~CIO();

	virtual bool setDigOut(unsigned short portCount = 16, unsigned char* portdata = 0);
	virtual bool setDigOutBit(short doIndex,short value);
	virtual bool getDigOut(unsigned short portCount = 16, unsigned char* portdata = 0);
	virtual bool getDigOutBit(short doIndex,short &value);
	virtual bool getDigIn(unsigned short portCount = 16, unsigned char* portdata = 0);
	virtual bool getDigInBit(short doIndex,short &value);

	// 输入操作接口
	virtual  bool getLmtsP(long &value);                                  // 获取正限位输入
	virtual  bool getLmtsN(long &value);                                  // 获取负限位输入
	virtual  bool getDrvAlm(long &value);                                 // 获取驱动报警输入
	virtual  bool getHome(long &value);                                   // 获取原点输入


	virtual  bool getLmtsPBit(short bitnum,short &value);                  // 获取正限位输入某一位
	virtual  bool getLmtsNBit(short bitnum,short &value);                  // 获取负限位输入某一位
	virtual  bool getDrvAlmBit(short bitnum,short &value);                 // 获取驱动报警输入某一位
	virtual  bool getHomeBit(short bitnum,short &value);                   // 获取正限位输入某一位

	// 输出操作接口
	virtual  bool setDrvEnable(long value);                               // 设置驱动器使能
	virtual  bool setClrAlarm(long value);                                // 设置报警清除
	virtual  bool getDrvEnable(long &value);                              // 获取驱动器使能
	virtual  bool getClrAlarm(long &value);                               // 获取报警清除

	virtual  bool setDrvEnableBit(short bitnum,short value);               // 设置驱动器使能某一位
	virtual  bool setClrAlarmBit(short bitnum,short value);                // 设置报警清除某一位
	virtual  bool getDrvEnableBit(short bitnum,short &value);              // 获取驱动器使能某一位
	virtual  bool getClrAlarmBit(short bitnum,short &value);               // 获取报警清除某一位

	virtual  bool getAdcValue(unsigned int index , short &AdcValue);		// 获取模拟值
	virtual  bool getAdc( unsigned int index, double &AdcValue );

private:
	unsigned short m_cardIndex;
};
#endif
