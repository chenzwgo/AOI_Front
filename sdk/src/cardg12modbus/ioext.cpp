#include "ioext.h"
#include "motor_common.h"
#include "rtdefine.h"

#define ONE  0x01

CIOExt::CIOExt(short cardIndex, short extIndex)
{
    m_cardIndex = cardIndex;
    m_extIndex = extIndex;
    m_gpiOut = 0xffff;

    open();
    SetCardIndex(m_cardIndex);
}

CIOExt::~CIOExt(void)
{
}

bool CIOExt::open()
{
    short rnt = 0;
    return true;
}

bool CIOExt::close()
{
    short rnt = 0;
    return true;
}

bool CIOExt::reset()
{
    return true;
}

bool CIOExt::SetExtDaValue(short chn,unsigned short value)
{
    return true;
}

bool CIOExt::SetExtDaVoltage(short chn,double value)
{
    return true;
}

bool CIOExt::loadConfig(char *path)
{
    return true;
}

bool CIOExt::saveConfig(char *path)
{
    return true;
}

bool CIOExt::isOpen()
{
    return true;
}

bool CIOExt::SetCardIndex(short cardIndex)
{
    short rnt = 0;
    if (cardIndex != m_cardIndex)
    {
        m_cardIndex = cardIndex;
    }
    return true;
}

bool CIOExt::setDigOut(unsigned short portCount, unsigned char* portdata)
{
    return true;
}

bool CIOExt::setDigOutBit(short doIndex,short value)
{
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
    return true;
}

bool CIOExt::getDigInBit(short doIndex,short &value)
{
    return true;
}

bool CIOExt::getAdc(unsigned int index, double &AdcValue)			//读取模拟量输入的电压值，单位V
{
	return true;
}

bool CIOExt::getAdcValue(unsigned int index, short &AdcValue)		//读取模拟量输入的数字转换值，单位bit
{
	return true;
}

bool CIOExt::setAdcValue( unsigned int index , short AdcValue )
{
	return true;
}

bool CIOExt::setAdc( unsigned int index, double dcValue )
{
	return true;
}
