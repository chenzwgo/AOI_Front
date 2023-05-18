#ifndef WORK_DEV_COM_H
#define WORK_DEV_COM_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

using namespace std;

#define  COM_NAME			"comname"
#define  COM_BAUDRATE		"baudrate"
#define  COM_PARITY			"parity"
#define  COM_BYTESIZE		"byte"
#define  COM_STOPBIT		"stopbit"
#define  COM_TIMEOUT		"timeout"
#define  COM_BSEND			"senddata"
#define  COM_VAR_NAME		"varname"
#define  COM_END_SIGN       "endSign"

struct WORK_DEV_COM : ACTION_PARAM
{
	string ComName;
	int BaudRate;
	int Parity;
	int ByteSize;
	int StopBits;
	int iOperateType;   // 0-¶Á 1-Ð´ 2-Çå»º´æ 3-¹Ø±Õ
	string strVarName;
    string strEndSign;  // ½áÊø·û

	WORK_DEV_COM()
	{
		ComName = "";
		BaudRate = 9600;	
		Parity = 0;
		ByteSize = 8;
		StopBits = 1; 
		iOperateType = 0;
		strVarName = "";
	}

	WORK_DEV_COM& operator=(const WORK_DEV_COM &other)
	{
		if (this == &other)
		{
			return *this;
		}
		ComName = other.ComName;
		BaudRate = other.BaudRate;	
		Parity = other.Parity;
		ByteSize = other.ByteSize;
		StopBits = other.StopBits; 
		iOperateType = other.iOperateType;
		strVarName = other.strVarName;
        strEndSign = other.strEndSign;
		return *this;
	}
	bool operator==(const WORK_DEV_COM &other)
	{
        return (ComName == other.ComName
            && BaudRate == other.BaudRate
			&& Parity == other.Parity
			&& ByteSize == other.ByteSize
			&& StopBits == other.StopBits
			&& iOperateType == other.iOperateType
			&& strVarName == other.strVarName
            && strEndSign == other.strEndSign);
	}

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ \
                          string(COM_NAME) + string(":") + ComName + "," + \
                          string(COM_BAUDRATE) + string(":") + SString::IntToStr(BaudRate) + "," + \
                          string(COM_PARITY) + string(":") + SString::IntToStr(Parity) + "," + \
                          string(COM_BYTESIZE) + string(":") + SString::IntToStr(ByteSize) + "," + \
                          string(COM_STOPBIT) + string(":") + SString::IntToStr(StopBits) + "," + \
                          string(COM_BSEND) + string(":") + SString::IntToStr(iOperateType) + "," + \
                          string(COM_VAR_NAME) + string(":") + strVarName + "," + \
                          string(COM_END_SIGN) + string(":") + strEndSign + "," + \
                     "}";
        return str;
    }
};



#endif // WORKFLOWDEFINE
