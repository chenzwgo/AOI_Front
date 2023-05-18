#ifndef CONVERYMODBUS_H
#define CONVERYMODBUS_H

#include "modbuscom.h"
#include "jthreadt.h"

#define MODBUS_STATION 01
#define MODBUS_BEGIN_ADDRESS 2000
#define MODBUS_ADDRESS_COUNT 40

class ConveryModbus
{
public:
	static ConveryModbus* GetInstance();
	static void ReleaseInstance();

	// 是否连接
	bool isConnect();

	// 获取温度(高位在前)
	bool getTemperature(int index, float &value);

	// 读IO(16位)
	bool readInput(ushort &value);
	// 读IO(16位)
	bool readOutput(ushort &value);
	// 写IO(16位)
	bool writeOutput(ushort value);

	// 获取指令位置
	bool getCurPos(uint &value);
	// 获取实际位置
	bool getActualPos(uint &value);

	// 设置驱动速度
	bool setVel(uint vel);

	// 设置触发间隔(脉冲数)
	bool setTriggerInterval(uint pulse);

	// 设置触发延时(脉冲数)
	bool setTriggerDelay(ushort pulse);

	// 设置触发次数
	bool setTriggerCount(uint count);

	// 输出定量脉冲
	bool setMovePulse(uint pulse);

	// 设置运动模式
	// 1：流水线 = 0x00
	// 2：乒乓模式 = 0x01
	bool setMoveMode(ushort mode);

    // 启动
	bool startMove();
	// 停止
	bool stopMove();

	// 设置自动无限触发(01)
	bool startAutoTrigger();
	// 设置行数触发(02)
	bool startAutoTriggerByLines();
	// 停止触发
	bool stopAutoTrigger();

	// 判断是否触发完成
	bool isTriggerFinished();
	// 判断是否移动完成
	bool isMoving();

private:
	ConveryModbus();
	~ConveryModbus();

	// 读取地址
	void readAddressCfg(string file);

	// 刷新modbus值线程，param为 this
	void* refreshModbusThread(void* param);			

	// 读取Modbus(2000-2100)
	void priReadModBusValue();

	// 获取modbus数据(参数：地址)
	ushort getModbusValue_16(int address);

	// 获取modbus数据(参数：地址)
	uint getModbusValue_32(int address);

	// 写ushort(双字节)数据
	bool writeModbusValue_16(int address, ushort value);

	// 写uint(四字节)数据
	bool writeModbusValue_32(int address, uint value);

private:
	CModbusCom * m_pCom;                  // modbus句柄
	static ConveryModbus *m_pInstance;    // 单实例
	map<string, int>  m_mapAddress;       // modbus地址
	JThreadTP<ConveryModbus, void *, void *>* m_hThreadRefresh;       //刷新modbus线程句柄
	bool m_bRun; // 线程运行标识
	ushort m_Value[MODBUS_ADDRESS_COUNT];
};

#endif // CONVERYMODBUS_H