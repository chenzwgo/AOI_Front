#include "io.h"
#include "motor_common.h"
#include "converymodbus.h"
#include "trace.h"

CIO::CIO(unsigned short cardIndex)
{
	m_cardIndex = cardIndex;
}

CIO::~CIO()
{

}


bool CIO::getDigOut(unsigned short portCount, unsigned char* portdata)
{
	ushort output = 0;
	bool bRet = ConveryModbus::GetInstance()->readOutput(output);
	if (bRet)
	{
		for (int i = 0; i < portCount && i < 16; i++)
		{
			ushort val = (output & (ushort(1) << i));
			portdata[i] = (val!=0);
		}
	}
	return bRet;
}

bool CIO::setDigOutBit( short doIndex,short value )
{
	static int index = 0;

	ushort output = 0;
	bool bRet = ConveryModbus::GetInstance()->readOutput(output);
	if (bRet)
	{
		if (value == 0)
		{
			output = output & (~(short(1) << doIndex));
		}
		else
		{
			output = output | (short(1) << doIndex);
		}
	}

	bRet = ConveryModbus::GetInstance()->writeOutput(output);
	return bRet;
}

bool CIO::setDigOut(unsigned short portCount, unsigned char* portdata)
{
	int error_num = -1;
	short allvalue = 0;
	for (int index =0;index <portCount;index++)
	{
		bool bbit = portdata[index];
		if (bbit)
		{
			allvalue = allvalue | ((short)0x01 << index);
		}
		else
		{
			allvalue = allvalue & ~((long)0x01 << index);
		}
	}
	
	bool bRet = ConveryModbus::GetInstance()->writeOutput(allvalue);
	return bRet;
}

bool CIO::getDigOutBit( short doIndex,short &value )
{
	ushort output = 0;
	bool bRet = ConveryModbus::GetInstance()->readOutput(output);
	if (bRet)
	{
		ushort val = (output & (ushort(1) << doIndex));
		value = (val!=0);
	}
	return bRet;
}

bool CIO::getDigIn(unsigned short portCount, unsigned char* portdata)
{
	ushort input = 0;
	bool bRet = ConveryModbus::GetInstance()->readInput(input);
	if (bRet)
	{
		for (int i = 0; i < portCount && i < 16; i++)
		{
			ushort val = (input & (ushort(1) << i));
			portdata[i] = (val!=0);
		}
	}
	return bRet;
}

bool CIO::getDigInBit( short diIndex,short &value )
{
	ushort intput = 0;
	bool bRet = ConveryModbus::GetInstance()->readInput(intput);
	if (bRet)
	{
		ushort val = (intput & (ushort(1) << diIndex));
		value = (val!=0);
	}
	return bRet;
}

bool CIO::getLmtsP(long &value)
{
    return true;
}

bool CIO::getLmtsN( long &value )
{
    return true;
}

bool CIO::getDrvAlm( long &value )
{
    return true;
}

bool CIO::getHome( long &value )
{
    return true;
}

bool CIO::getLmtsPBit(short bitnum,short &value )
{
	long allvalue = 0;
	if (!getLmtsP(allvalue))
	{
		return false;
	}
	value = (0x01 << bitnum) & allvalue;
	return true;
}

bool CIO::getLmtsNBit( short bitnum,short &value )
{
	long allvalue = 0;
	if (!getLmtsN(allvalue))
	{
		return false;
	}
	value = (0x01 << bitnum) & allvalue;
	return true;
}

bool CIO::getDrvAlmBit( short bitnum,short &value )
{
	long allvalue = 0;
	if (!getDrvAlm(allvalue))
	{
		return false;
	}
	value = (0x01 << bitnum) & allvalue;
	return true;
}

bool CIO::getHomeBit( short bitnum,short &value )
{
	long allvalue = 0;
	if (!getHome(allvalue))
	{
		return false;
	}
	value = (0x01 << bitnum) & allvalue;
	return true;
}

bool CIO::setDrvEnable( long value )
{
	return true;
}

bool CIO::setClrAlarm( long value )
{
	return true;
}

bool CIO::getDrvEnable( long &value )
{
	return true;
}

bool CIO::getClrAlarm( long &value )
{
	return true;
}

bool CIO::setDrvEnableBit( short bitnum,short value )
{
	return true;
}

bool CIO::setClrAlarmBit( short bitnum,short value )
{
	return true;
}

bool CIO::getDrvEnableBit( short bitnum,short &value )
{
	long allvalue = 0;
	if (!getDrvEnable(allvalue))
	{
		return false;
	}
	value = (0x01 << bitnum) & allvalue;
	return true;
}

bool CIO::getClrAlarmBit( short bitnum,short &value )
{
	long allvalue = 0;
	if (!getClrAlarm(allvalue))
	{
		return false;
	}
	value = (0x01 << bitnum) & allvalue;
	return true;
}

bool CIO::getAdcValue( unsigned int index , short &AdcValue )
{
	return true;
}


bool CIO::getAdc( unsigned int index , double &AdcValue )
{
	float value = 0.0;
	ConveryModbus::GetInstance()->getTemperature(index,value);
	AdcValue = value;
	return true;
}
