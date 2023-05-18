#ifndef WORK_DEV_NET_H
#define WORK_DEV_NET_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

#define  ITEM_NAME_LEN  256             // ���̱༭�������ֳ���

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
// formatData˵����
// 1��[]�����λ����[FixStation.x]��ʾ"FixStation"��վ�ĵ�ǰx������;
//    [FixStation.point1.x]��ʾ"FixStation"��վ��"point1"��λ��x������
// 2��<>�����������<var1>��ʾ����var1
// ���ӣ�����<num>:T0,1,{station.x},{station.y}
//       �գ�<num1>:T0,1,OK,{station.fixPoint.x},{station.fixPoint.y}
/********************************************************************/
struct WORK_DEV_NET : ACTION_PARAM
{
	string socketName; // socket����
	int iOperateType;  // 0-�� 1-д 2-�建��
    string formatData; // ��ʽ������,�������(��������,��λ���Ƶ�)
	string sourcedata; // ���ͽ��յ�ԭʼ����,�������,������ʾ
    string readMask;   // ������(���������)��ɸѡ����

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
