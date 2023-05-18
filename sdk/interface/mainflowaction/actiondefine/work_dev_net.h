#ifndef WORK_DEV_NET_H
#define WORK_DEV_NET_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

#define  ITEM_NAME_LEN  256             // 流程编辑各项名字长度

#define  TCP_SOCKET_NAME   "socketName"
#define  TCP_OPERATE_TYPE  "operateType"
#define  TCP_FORMAT_DATA   "formatData"
#define  TCP_SOURCE_DATA   "sourceData"
#define  TCP_READ_TIMEOUT  "readTimeOut"
#define  TCP_READ_MASK     "readMask"

#define  UDP_IP			"ip"
#define  UDP_PORT		"port"
#define  UDP_SERVER_IP			"serverip"
#define  UDP_SERVER_PORT		"serverport"
#define  UDP_TIMEOUT	"timeout"
#define  UDP_BSEND		"senddata"
#define  UDP_VAR_NAME	"varname"

/********************************************************************/
// formatData说明：
// 1、[]代表点位，如[FixStation.x]表示"FixStation"工站的当前x轴坐标;
//    [FixStation.point1.x]表示"FixStation"工站的"point1"点位的x轴坐标
// 2、<>代表变量，如<var1>表示变量var1
// 例子：发：<num>:T0,1,{station.x},{station.y}
//       收：<num1>:T0,1,OK,{station.fixPoint.x},{station.fixPoint.y}
/********************************************************************/
struct WORK_DEV_NET : ACTION_PARAM
{
	string socketName; // socket名称
	int iOperateType;  // 0-读 1-写 2-清缓存
    string formatData; // 格式化数据,传入参数(带变量名,点位名称等)
	string sourcedata; // 发送接收的原始数据,输出参数,用于显示
    string readMask;   // 读数据(或清除数据)的筛选条件

    WORK_DEV_NET()
	{
		iOperateType = 0;
	}

	WORK_DEV_NET& operator=(const WORK_DEV_NET &other)
	{
		if (this == &other)
		{
			return *this;
		}
		socketName = other.socketName;
        iOperateType = other.iOperateType;
        formatData = other.formatData;
        readMask = other.readMask;
		return *this;
	}
	bool operator==(const WORK_DEV_NET &other)
	{
		return (socketName == other.socketName 
			&& iOperateType == other.iOperateType 
			&& formatData == other.formatData 
            && readMask == other.readMask);
	}

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ \
            string(TCP_SOCKET_NAME) + string(":") + socketName + "," + \
            string(TCP_OPERATE_TYPE) + string(":") + SString::IntToStr(iOperateType) + "," + \
            string(TCP_FORMAT_DATA) + string(":") + formatData + "," + \
            string(TCP_SOURCE_DATA) + string(":") + sourcedata + "," + \
            string(TCP_READ_MASK) + string(":") + readMask + "," + \
            "}";
        return str;
    }
};


#endif // WORKFLOWDEFINE
