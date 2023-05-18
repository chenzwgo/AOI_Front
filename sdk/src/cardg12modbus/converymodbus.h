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

	// �Ƿ�����
	bool isConnect();

	// ��ȡ�¶�(��λ��ǰ)
	bool getTemperature(int index, float &value);

	// ��IO(16λ)
	bool readInput(ushort &value);
	// ��IO(16λ)
	bool readOutput(ushort &value);
	// дIO(16λ)
	bool writeOutput(ushort value);

	// ��ȡָ��λ��
	bool getCurPos(uint &value);
	// ��ȡʵ��λ��
	bool getActualPos(uint &value);

	// ���������ٶ�
	bool setVel(uint vel);

	// ���ô������(������)
	bool setTriggerInterval(uint pulse);

	// ���ô�����ʱ(������)
	bool setTriggerDelay(ushort pulse);

	// ���ô�������
	bool setTriggerCount(uint count);

	// �����������
	bool setMovePulse(uint pulse);

	// �����˶�ģʽ
	// 1����ˮ�� = 0x00
	// 2��ƹ��ģʽ = 0x01
	bool setMoveMode(ushort mode);

    // ����
	bool startMove();
	// ֹͣ
	bool stopMove();

	// �����Զ����޴���(01)
	bool startAutoTrigger();
	// ������������(02)
	bool startAutoTriggerByLines();
	// ֹͣ����
	bool stopAutoTrigger();

	// �ж��Ƿ񴥷����
	bool isTriggerFinished();
	// �ж��Ƿ��ƶ����
	bool isMoving();

private:
	ConveryModbus();
	~ConveryModbus();

	// ��ȡ��ַ
	void readAddressCfg(string file);

	// ˢ��modbusֵ�̣߳�paramΪ this
	void* refreshModbusThread(void* param);			

	// ��ȡModbus(2000-2100)
	void priReadModBusValue();

	// ��ȡmodbus����(��������ַ)
	ushort getModbusValue_16(int address);

	// ��ȡmodbus����(��������ַ)
	uint getModbusValue_32(int address);

	// дushort(˫�ֽ�)����
	bool writeModbusValue_16(int address, ushort value);

	// дuint(���ֽ�)����
	bool writeModbusValue_32(int address, uint value);

private:
	CModbusCom * m_pCom;                  // modbus���
	static ConveryModbus *m_pInstance;    // ��ʵ��
	map<string, int>  m_mapAddress;       // modbus��ַ
	JThreadTP<ConveryModbus, void *, void *>* m_hThreadRefresh;       //ˢ��modbus�߳̾��
	bool m_bRun; // �߳����б�ʶ
	ushort m_Value[MODBUS_ADDRESS_COUNT];
};

#endif // CONVERYMODBUS_H