// GuGaoCardPlugin.h : GuGaoCardPlugin DLL 的主头文件
//

#pragma once
#include "isdkresource.h"

class IPluginContainer;
class iInterfacemanage;
class CGuGaoCardPlugin : public ISDKResource
{
public:
	CGuGaoCardPlugin();
	virtual ~CGuGaoCardPlugin(); 

	/****************************************************	
	Function: 插件初始化
	Intput: NULL
	Output:	NULL
	Return:	返回初始化插件是否成功
	******************************************************/
	virtual bool init(void);

	/****************************************************
	Function: 插件去初始化，释放资源等
	Intput: NULL
	Output:	NULL
	Return:	返回去初始化是否成功
	******************************************************/
	virtual bool deinit(void);

	/****************************************************	
	Function: 查询插件接口
	Intput: iid_interface 接口的iid
	Output:	ppInterface 接口指针的指针
	Return:	true成功，false失败
    ******************************************************/
    ISDKCallInterface* GetInterface(unsigned int InterfaceIndex = 0) ;

private:
    iInterfacemanage *m_pInterfacemanage;
    bool m_bInit;
};
