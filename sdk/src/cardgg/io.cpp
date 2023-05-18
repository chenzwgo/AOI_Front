#include "io.h"
#include "gts_common.h"
#include "motor_common.h"
#include "gts.h"

CIO::CIO(unsigned short cardIndex)
{
	m_cardIndex = cardIndex;
}

CIO::~CIO()
{

}


bool CIO::getDigOut(unsigned short portCount, unsigned char* portdata)
{
	int error_num = -1;
	long allvalue = 0;
	error_num = GT_GetDo(m_cardIndex,MC_GPO,&allvalue);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	for (int index =0;index <portCount;index++)
	{
		portdata[index] = (long)0x01 & (allvalue>>index);
	}
	return true;
}

bool CIO::setDigOutBit( short doIndex,short value )
{
	int error_num = -1;
	error_num = GT_SetDoBit(m_cardIndex,MC_GPO,doIndex+1,value);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	return true;
}

bool CIO::setDigOut(unsigned short portCount, unsigned char* portdata)
{
	int error_num = -1;
	long allvalue = 0;
	for (int index =0;index <portCount;index++)
	{
		bool bbit = portdata[index];
		if (bbit)
		{
			allvalue = allvalue | ((long)0x01 << index);
		}
		else
		{
			allvalue = allvalue & ~((long)0x01 << index);
		}
	}
	error_num = GT_SetDo(m_cardIndex,MC_GPO,allvalue);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}

	return true;
}

bool CIO::getDigOutBit( short doIndex,short &value )
{
	long allvalue = 0;
	int error_num = -1;
	error_num = GT_GetDo(m_cardIndex,MC_GPO,&allvalue);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	value = 0x01 & (allvalue>>doIndex);
	return true;
}

bool CIO::getDigIn(unsigned short portCount, unsigned char* portdata)
{
	int error_num = -1;
	long allvalue;
	error_num = GT_GetDi(m_cardIndex,MC_GPI,&allvalue);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	for (int index =0;index <portCount;index++)
	{
		portdata[index] = (long)0x01 & (allvalue>>index);
	}
	return true;
}

bool CIO::getDigInBit( short diIndex,short &value )
{
	long allvalue = 0;
	int error_num = GT_GetDi(m_cardIndex,MC_GPI,&allvalue);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	value = 0x01 & (allvalue>>diIndex);
	return true;
}

bool CIO::getLmtsP(long &value)
{
    int error_num = -1;
    error_num = GT_GetDi(m_cardIndex,MC_LIMIT_POSITIVE,&value);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CIO::getLmtsN( long &value )
{
    int error_num = -1;
    error_num = GT_GetDi(m_cardIndex,MC_LIMIT_NEGATIVE,&value);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CIO::getDrvAlm( long &value )
{
    int error_num = -1;
    error_num = GT_GetDi(m_cardIndex,MC_ALARM,&value);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CIO::getHome( long &value )
{
    int error_num = -1;
    error_num = GT_GetDi(m_cardIndex,MC_HOME,&value);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
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
	int error_num = -1;
	error_num = GT_SetDo(m_cardIndex,MC_ENABLE,value);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	return true;
}

bool CIO::setClrAlarm( long value )
{
	int error_num = -1;
	error_num = GT_SetDo(m_cardIndex,MC_CLEAR,value);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	return true;
}

bool CIO::getDrvEnable( long &value )
{
	int error_num = -1;
	error_num = GT_GetDo(m_cardIndex,MC_ENABLE,&value);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	return true;
}

bool CIO::getClrAlarm( long &value )
{
	int error_num = -1;
	error_num = GT_GetDo(m_cardIndex,MC_CLEAR,&value);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	return true;
}

bool CIO::setDrvEnableBit( short bitnum,short value )
{
	int error_num = -1;
	error_num = GT_SetDoBit(m_cardIndex,MC_ENABLE,bitnum+1,value);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	return true;
}

bool CIO::setClrAlarmBit( short bitnum,short value )
{
	int error_num = -1;
	error_num = GT_SetDoBit(m_cardIndex,MC_CLEAR,bitnum+1,value);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
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
	int error_num = -1;
	short Value = 0;
	//error_num = GT_GetAdc(m_cardIndex,index,&value2);
	error_num = GT_GetAdcValue(m_cardIndex,index,&Value);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		AdcValue = 0;
		return false;
	}
	AdcValue = Value;
	return true;
}


bool CIO::getAdc( unsigned int index , double &AdcValue )
{
	int error_num = -1;
	error_num = GT_GetAdc(m_cardIndex,index,&AdcValue);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		AdcValue = 0.0;
		return false;
	}
	return true;
}
