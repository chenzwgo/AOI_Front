#ifndef __IREPORTCENTER_H__
#define __IREPORTCENTER_H__
#include "OS_DEFINE.h"

typedef bool (*pFunUISelector)(const char* uiPluginGuid,const char* pEvent,OS_LPARAM pData1,OS_LPARAM pData2);

class IReportCenter
{
public:
	enum
	{
		IID_REPORTCENTER = 0x303
	};
    virtual bool reportPostTo(const char* uiPluginGuid,const char* pEvent,OS_LPARAM pData1= 0,OS_LPARAM pData2 =0) = 0;
    virtual bool reportSendTo(const char* uiPluginGuid,const char* pEvent,OS_LPARAM pData1=0,OS_LPARAM pData2 =0) = 0;
	
    virtual void reportRemoveObserver() = 0;
	virtual void reportAddObserver(pFunUISelector pfun) = 0;
};
#endif  //__IREPORTCENTER_H__

