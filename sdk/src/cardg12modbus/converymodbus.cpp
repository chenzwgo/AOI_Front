#include "converymodbus.h"
#include "rtdefine.h"
#include "g12common.h"
#include "filevariable/ifilevariable.h"
#include "trace.h"
#include "stringcommon.h"

#define ADDRESS_PATH      "./plugins/cardg12modbus/modbusaddress.ini"

static long logIndex = 0; // 打log用

union unTemperature
{
	ushort nVal[2];
	unsigned char cVal[4];
	float fVal;
};

ConveryModbus *ConveryModbus::m_pInstance = NULL;
ConveryModbus::ConveryModbus()
	: m_pCom(NULL)
	, m_hThreadRefresh(NULL)
	, m_bRun(false)
{	
	memset(m_Value, 0, sizeof(short) * MODBUS_ADDRESS_COUNT);
	readAddressCfg(ADDRESS_PATH);

	string comName;
	m_pCom = CModbusComMgr::GetInstance()->getCModbusCom(MODBUS_CARD, comName);
	m_hThreadRefresh = new JThreadTP<ConveryModbus, void *, void *>;
	m_bRun = true;
	m_hThreadRefresh->StartThread(this, &ConveryModbus::refreshModbusThread, this);
}

ConveryModbus::~ConveryModbus()
{
	m_bRun = false;
	if (m_pCom)
	{
		m_pCom->ReleaseInstance();
	}

	SAFE_DELETE(m_hThreadRefresh);
}

ConveryModbus* ConveryModbus::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new ConveryModbus();
	}
	return m_pInstance;
}

void ConveryModbus::ReleaseInstance()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

bool ConveryModbus::isConnect()
{
	RETURN_V_IF(NULL == m_pCom, false);
	return m_pCom->isOpen();
}

void ConveryModbus::readAddressCfg(string file)
{
	IFileVariable *pfileVariable = getConveryModbusAddressFileVariable();
	RETURN_IF(NULL == pfileVariable);

	// 地址信息
	vector<string> keys = pfileVariable->getAllKeys(ADDRESS);
	for (int i = 0; i < keys.size(); i++)
	{
		int value = pfileVariable->getValue(ADDRESS, keys[i]).toInt();
		m_mapAddress[keys[i]] = value;
	}
}

void* ConveryModbus::refreshModbusThread(void* param)
{
	while (m_bRun)
	{
		priReadModBusValue();
		Sleep(20);
	}

	return NULL;
}

void ConveryModbus::priReadModBusValue()
{
	RETURN_IF(NULL == m_pCom);
	bool bRet = m_pCom->readMultiRegD(MODBUS_STATION, MODBUS_BEGIN_ADDRESS, MODBUS_ADDRESS_COUNT, (short*)m_Value);

	//LOG
	string strValue;
	for (int i = 0; i < MODBUS_ADDRESS_COUNT; i++)
	{
		if (i % 10 == 0)
		{
			strValue += "\n";
			int address = MODBUS_BEGIN_ADDRESS + (i/10) * 10;
			strValue += SString::IntToStr(address);
			strValue += " = ";
		}
		strValue += SString::IntToStr(m_Value[i]);
		strValue += " ";
	}
	//LOG_INFO_F("./log/g12card_recv.log", "read [%s ].", strValue.c_str());
}

ushort ConveryModbus::getModbusValue_16(int address)
{
	if (address < MODBUS_BEGIN_ADDRESS || address>MODBUS_BEGIN_ADDRESS+MODBUS_ADDRESS_COUNT-1)
	{
		return 0;
	}

	return m_Value[address - MODBUS_BEGIN_ADDRESS];
}

uint ConveryModbus::getModbusValue_32(int address)
{
	if (address < MODBUS_BEGIN_ADDRESS || address>MODBUS_BEGIN_ADDRESS + MODBUS_ADDRESS_COUNT - 1)
	{
		return 0;
	}

	ushort high = m_Value[address - MODBUS_BEGIN_ADDRESS];
	ushort low = m_Value[address - MODBUS_BEGIN_ADDRESS + 1];
	return ((uint(high) << 16) + uint(low));
}

bool ConveryModbus::writeModbusValue_16(int address, ushort value)
{
	RETURN_V_IF(NULL == m_pCom, false);
	short val;
	bool bRet = m_pCom->writeSingHoldReg(MODBUS_STATION, address, value);
	//LOG
	//LOG_INFO_F("./log/g12card_send.log", "(%d)write [%d=%d].", logIndex++, address, value);
	return bRet;
}

bool ConveryModbus::writeModbusValue_32(int address, uint value)
{
	RETURN_V_IF(NULL == m_pCom, false);

	if (address < MODBUS_BEGIN_ADDRESS || address>MODBUS_BEGIN_ADDRESS + MODBUS_ADDRESS_COUNT - 1)
	{
		return false;
	}

	ushort uVal[2] = { 0 };
	uVal[0] = value >> 16;
	uVal[1] = value & 0x0000ffff;
	bool bRet = m_pCom->writeMulHoldReg(MODBUS_STATION, address, 2, uVal);
	//LOG
	//LOG_INFO_F("./log/g12card_send.log", "(%d)write [%d=%d].", logIndex++, address, value);
	return bRet;
}

bool ConveryModbus::getTemperature(int index, float &value)
{
	RETURN_V_IF(index < 0 || index > 7, false);

	vector<string> temps;
	temps.push_back(SET_TEMPERATURE1);
	temps.push_back(SET_TEMPERATURE2);
	temps.push_back(SET_TEMPERATURE3);
	temps.push_back(SET_TEMPERATURE4);
	ushort nValueHigh = getModbusValue_16(m_mapAddress[temps[index]]);
	ushort nValueLow = getModbusValue_16(m_mapAddress[temps[index]] + 1);

	// int转float
	unTemperature data;
	data.nVal[0] = nValueLow;
	data.nVal[1] = nValueHigh;
	value = data.fVal;
	return true;
}

bool ConveryModbus::readInput(ushort &value)
{
	value = getModbusValue_16(m_mapAddress[GET_INPUT]);
	return true;
}

bool ConveryModbus::readOutput(ushort &value)
{
	value = getModbusValue_16(m_mapAddress[SET_OUTPUT]);
	return true;
}

bool ConveryModbus::writeOutput(ushort value)
{
	bool bRet = writeModbusValue_16(m_mapAddress[SET_OUTPUT], value);
	return bRet;
}

bool ConveryModbus::getCurPos(uint &value)
{
	value = getModbusValue_32(m_mapAddress[GET_CMDPOS]);
	return true;
}

bool ConveryModbus::getActualPos(uint &value)
{
	value = getModbusValue_32(m_mapAddress[GET_ACTUAL_POS]);
	return true;
}

bool ConveryModbus::setVel(uint vel)
{
	bool bRet = writeModbusValue_32(m_mapAddress[SET_VEL], vel);
	return bRet;
}

bool ConveryModbus::setTriggerInterval(uint pulse)
{
	bool bRet = writeModbusValue_16(m_mapAddress[SET_TRIGGER_INTERVAL], pulse);
	return bRet;
}

bool ConveryModbus::setTriggerDelay(ushort pulse)
{
	bool bRet = writeModbusValue_32(m_mapAddress[SET_TRIGGER_DELAY], pulse);
	return bRet;
}

bool ConveryModbus::setTriggerCount(uint count)
{
	bool bRet = writeModbusValue_32(m_mapAddress[SET_TRIGGER_COUNT], count);
	return bRet;
}

bool ConveryModbus::setMovePulse(uint pulse)
{
	bool bRet = writeModbusValue_32(m_mapAddress[SET_PULSECOUNT], pulse);
	return bRet;
}

bool ConveryModbus::setMoveMode(ushort mode)
{
	bool bRet = writeModbusValue_16(m_mapAddress[SET_DRIVERMODE], mode);
	return bRet;
}

bool ConveryModbus::startMove()
{
	bool bRet = writeModbusValue_16(m_mapAddress[SET_DRIVECMD], 0x55aa);
	return bRet;
}

bool ConveryModbus::stopMove()
{
	bool bRet = writeModbusValue_16(m_mapAddress[SET_DRIVECMD], 0x00);
	return bRet;
}

bool ConveryModbus::startAutoTrigger()
{
	bool bRet = writeModbusValue_16(m_mapAddress[SET_TRIGGERAUTO], 0x01);
	return bRet;
}

bool ConveryModbus::startAutoTriggerByLines()
{
	bool bRet = writeModbusValue_16(m_mapAddress[SET_TRIGGERAUTO], 0x02);
	return bRet;
}

bool ConveryModbus::stopAutoTrigger()
{
	bool bRet = writeModbusValue_16(m_mapAddress[SET_TRIGGERAUTO], 0x00);
	return bRet;
}

bool ConveryModbus::isTriggerFinished()
{
	ushort nRet = getModbusValue_16(m_mapAddress[GET_TRIGGER_FINISH]);
	if (nRet == 1)
	{
		return true;
	}
	return false;
}

bool ConveryModbus::isMoving()
{
	ushort nRet = getModbusValue_16(m_mapAddress[GET_MOVE_FINISH]);
	if (nRet == 1)
	{
		return true;
	}
	return false;
}
