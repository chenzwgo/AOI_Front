#ifndef WORK_PARSE_STRING_H
#define WORK_PARSE_STRING_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

#define  ITEM_NAME_LEN  256             // 流程编辑各项名字长度

#define  PARSE_FORMAT    "format"
#define  PARSE_VAR_NAME	 "varname"

/********************************************************************/
// format说明：
// 1、[]代表点位，如[FixStation.x]表示"FixStation"工站的当前x轴坐标;
//    [FixStation.point1.x]表示"FixStation"工站的"point1"点位的x轴坐标
// 2、<>代表变量，如<var1>表示变量var1
// 例子：变量内容：1：T0,1,12.5,30.55
//       格式：<num>:T0,1,{station.fixPoint.x},{station.fixPoint.y}
//       解析出来：<num1>=1 {station.fixPoint.x}=12.5,{station.fixPoint.y}=30.55
/********************************************************************/
struct WORK_PARSE_STRING : ACTION_PARAM
{
    string format; // 格式化数据,传入参数(带变量名,点位名称等)
	string varName;    // 存放原始数据的变量名

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
