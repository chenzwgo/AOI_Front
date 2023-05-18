#ifndef _GG_IO_EXT_H_
#define _GG_IO_EXT_H_

#include "jmutex.h"
#include "card/iio.h"

// IO控制扩展类
class CIOExt : public IIO
{
public:
    CIOExt(short cardIndex, short extIndex); //pDllName 扩展卡与运动控制器一起用时传入运动控制器的dll名称(注意两个dll要放在同一目录)
    virtual ~CIOExt();

public:

	bool open();		// 打开 与运动卡一起使用时必须先打开运动卡   
	bool close();		// 关闭
	bool reset();		// 复位

	bool loadConfig(char *path);		// 加载配置
	bool saveConfig(char *path);		// 保持配置
	bool isOpen();						// 扩展卡是否打卡
	bool SetCardIndex(short cardIndex);	// 设置扩展卡索引

	//da写入
	bool SetExtDaValue(short chn,unsigned short value);
	bool SetExtDaVoltage(short chn,double value);

	virtual bool setDigOut(unsigned short portCount = 16, unsigned char portdata[] = NULL);
	virtual bool setDigOutBit(short doIndex,short value);	//实现，输出通用输出的某一位
	virtual bool getDigOut(unsigned short portCount = 16, unsigned char* portdata = NULL);					//实现,获取全部通用输出
	virtual bool getDigOutBit(short doIndex,short &value);	//实现,获取通用输出的某一位
	virtual bool getDigIn(unsigned short portCount = 16, unsigned char* portdata = NULL);					//实现，获取全部通用输入
	virtual bool getDigInBit(short doIndex,short &value);	//实现，获取通用输入某一位

	virtual  bool getAdcValue(unsigned int index , short &AdcValue);		//实现， 获取模拟值//
	virtual  bool getAdc( unsigned int index, double &AdcValue );			//实现

	virtual  bool setAdcValue(unsigned int index , short AdcValue);		//实现， 设置模拟值//
	virtual  bool setAdc( unsigned int index, double dcValue );			//实现

private:

    static bool m_isOpen;
    short m_cardIndex;
    short m_extIndex;
    long  m_gpiOut;
    static JMutex m_lock_IO;

	void sysEnterCriticalSection() {m_lock_IO.Lock();}
	void sysLeaveCriticalSection() {m_lock_IO.Unlock();}
};

#endif
