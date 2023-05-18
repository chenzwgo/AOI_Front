#ifndef WORK_PARSE_STRING_H
#define WORK_PARSE_STRING_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

#define  ITEM_NAME_LEN  256             // ���̱༭�������ֳ���

#define  PARSE_FORMAT    "format"
#define  PARSE_VAR_NAME	 "varname"

/********************************************************************/
// format˵����
// 1��[]�����λ����[FixStation.x]��ʾ"FixStation"��վ�ĵ�ǰx������;
//    [FixStation.point1.x]��ʾ"FixStation"��վ��"point1"��λ��x������
// 2��<>�����������<var1>��ʾ����var1
// ���ӣ��������ݣ�1��T0,1,12.5,30.55
//       ��ʽ��<num>:T0,1,{station.fixPoint.x},{station.fixPoint.y}
//       ����������<num1>=1 {station.fixPoint.x}=12.5,{station.fixPoint.y}=30.55
/********************************************************************/
struct WORK_PARSE_STRING : ACTION_PARAM
{
    string format; // ��ʽ������,�������(��������,��λ���Ƶ�)
	string varName;    // ���ԭʼ���ݵı�����

	WORK_PARSE_STRING& operator=(const WORK_PARSE_STRING &other)
	{
		if (this == &other)
		{
			return *this;
		}

        format = other.format;
        varName = other.varName;
		return *this;
	}
	bool operator==(const WORK_PARSE_STRING &other)
	{
		return (format == other.format 
			&& varName == other.varName);
	}

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ \
            string(PARSE_FORMAT) + string(":") + format + "," + \
            string(PARSE_VAR_NAME) + string(":") + varName + \
            "}";
        return str;
    }
};


#endif
