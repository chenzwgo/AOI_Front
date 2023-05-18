#include "ioext.h"
//#include "ExtMdl.h"
#include "motor_common.h"
#include "gts.h"
#include "rtdefine.h"
#include "gts_common.h"

#define DLL_NAME "gts.dll"
#define CFG_NAME "./plugins/cardgg/ExtMdl.cfg"

#define EXT_MODEL 0 //直接控制为0
#define VST_MODEL 1 //扩展卡工作模式 由程序控制为1

#define ONE  0x01

bool CIOExt::m_isOpen = false;  //所有扩展模块只需要打开一次 
JMutex CIOExt::m_lock_IO;

CIOExt::CIOExt(short cardIndex, short extIndex)
{
    m_cardIndex = cardIndex;
    m_extIndex = extIndex;
    m_gpiOut = 0xffff;

    if(!m_lock_IO.IsInitialized())
    {
        m_lock_IO.Init();
    }
    open();
    SetCardIndex(m_cardIndex);
}

CIOExt::~CIOExt(void)
{
}

bool CIOExt::open()
{
    short rnt = 0;

	RETURN_V_IF(m_isOpen,true);
	sysEnterCriticalSection();
    rnt = GT_OpenExtMdl(m_cardIndex,(char*) DLL_NAME);
    sysLeaveCriticalSection();
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);

	m_isOpen = true;
    RETURN_V_IF(!loadConfig((char*)CFG_NAME),false);
    return true;
}

bool CIOExt::close()
{
    short rnt = 0;

    RETURN_V_IF(m_isOpen,true);
	sysEnterCriticalSection();
    rnt = GT_CloseExtMdl(m_cardIndex);
    sysLeaveCriticalSection();
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
    m_isOpen = false;
    return true;
}

bool CIOExt::reset()
{
    short rnt = 0;
	sysEnterCriticalSection();
    rnt = GT_ResetExtMdl(m_cardIndex);
    sysLeaveCriticalSection();
	RETURN_V_IF(GTS_SUCCESS != rnt, false);
    return true;
}

bool CIOExt::SetExtDaValue(short chn,unsigned short value)
{
    short rnt = 0;

	RETURN_V_IF(!m_isOpen, false);
    sysEnterCriticalSection();
    rnt = GT_SetExtDaValue(m_cardIndex, m_extIndex, chn, value);
	sysLeaveCriticalSection();
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
    return true;
}

bool CIOExt::SetExtDaVoltage(short chn,double value)
{
    short rnt = 0;

	RETURN_V_IF(!m_isOpen, false);
    sysEnterCriticalSection();
    rnt = GT_SetExtDaVoltage(m_cardIndex, m_extIndex, chn, value);
	sysLeaveCriticalSection();
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
    return true;
}

bool CIOExt::loadConfig(char *path)
{
    short rnt = 0;
	RETURN_V_IF(!m_isOpen, false);
	sysEnterCriticalSection();
    rnt = GT_LoadExtConfig(m_cardIndex, path);
    sysLeaveCriticalSection();
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
    return true;
}

bool CIOExt::saveConfig(char *path)
{
    return true;
}

bool CIOExt::isOpen()
{
    return m_isOpen;
}

bool CIOExt::SetCardIndex(short cardIndex)
{
    short rnt = 0;
	RETURN_V_IF(!m_isOpen, false);
    if (cardIndex != m_cardIndex)
    {
        m_cardIndex = cardIndex;
    }
    return true;
}

bool CIOExt::setDigOut(unsigned short portCount, unsigned char* portdata)
{
    short rnt = 0;
	unsigned short value = 0;
	for (int index =0;index <portCount;index++)
	{
		bool bbit = portdata[index];
		if (bbit)
		{
			value = value | (0x01 << index);
		}
		else
		{
			value = value & ~(0x01 << index);
		}
	}
    RETURN_V_IF(!m_isOpen, false);
    sysEnterCriticalSection();
    rnt = GT_SetExtIoValue(m_cardIndex, m_extIndex,(unsigned short)value);
    sysLeaveCriticalSection();
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
    m_gpiOut = value;
    return true;
}

bool CIOExt::setDigOutBit(short doIndex,short value)
{
    short rnt = 0;

    RETURN_V_IF(!m_isOpen, false);
    sysEnterCriticalSection();
    rnt = GT_SetExtIoBit(m_cardIndex, m_extIndex, doIndex, value);
    sysLeaveCriticalSection();
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);

    if (value == 0)
    {
        m_gpiOut &= (~(ONE<<doIndex));
    }
    else
    {
        m_gpiOut |= ONE<<doIndex;
    }

    return true;
}

bool CIOExt::getDigOut(unsigned short portCount, unsigned char* portdata)
{
	for (int index =0;index <portCount;index++)
	{
		portdata[index] = (long)0x01 & (m_gpiOut>>index);
	}
    return true;
}

bool CIOExt::getDigOutBit(short doIndex,short &value)
{
    value = m_gpiOut>>doIndex & ONE;
    return true;
}

bool CIOExt::getDigIn(unsigned short portCount, unsigned char* portdata)
{
    short rnt = 0;
	unsigned short value;
    RETURN_V_IF(!m_isOpen, false);
    sysEnterCriticalSection();
    rnt = GT_GetExtIoValue(m_cardIndex, !m_extIndex, (unsigned short*)&value);
    sysLeaveCriticalSection();
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
	for (int index =0;index <portCount;index++)
	{
		portdata[index] = 0x01 & (value>>index);
	}
    return true;
}

bool CIOExt::getDigInBit(short doIndex,short &value)
{
    short rnt = 0;

    RETURN_V_IF(!m_isOpen, false);
    sysEnterCriticalSection();
    rnt = GT_GetExtIoBit(m_cardIndex, m_extIndex, doIndex, (unsigned short*)&value);
    sysLeaveCriticalSection();
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
    return true;
}

bool CIOExt::getAdc(unsigned int index, double &AdcValue)			//读取模拟量输入的电压值，单位V
{
	short rnt = 0;

	RETURN_V_IF(!m_isOpen, false);
	sysEnterCriticalSection();
	rnt = GT_GetExtAdVoltage(m_cardIndex, m_extIndex, index, &AdcValue);
	sysLeaveCriticalSection();
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
	return true;
}

bool CIOExt::getAdcValue(unsigned int index, short &AdcValue)		//读取模拟量输入的数字转换值，单位bit
{
	short rnt = 0;

	RETURN_V_IF(!m_isOpen, false);
	sysEnterCriticalSection();
	rnt = GT_GetExtAdValue(m_cardIndex, m_extIndex, index, (unsigned short*)&AdcValue);
	sysLeaveCriticalSection();
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
	return true;
}

bool CIOExt::setAdcValue( unsigned int index , short AdcValue )
{
	short rnt = 0;

	RETURN_V_IF(!m_isOpen, false);
	sysEnterCriticalSection();
	rnt = GT_SetExtDaValue(m_cardIndex, m_extIndex, index, (unsigned short)AdcValue);
	sysLeaveCriticalSection();
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
	return true;
}

bool CIOExt::setAdc( unsigned int index, double dcValue )
{
	short rnt = 0;

	RETURN_V_IF(!m_isOpen, false);
	sysEnterCriticalSection();
	rnt = GT_SetExtDaVoltage(m_cardIndex, m_extIndex, index, dcValue);
	sysLeaveCriticalSection();
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != rnt, false);
	return true;
}
