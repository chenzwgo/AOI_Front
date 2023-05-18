
// GuGaoCardPlugin.cpp : 定义 sdk插件。

//#include "guid.h"
#include "cardggplugin.h"
#include "cardmanagerservice.h"
#include "interfacemanagefac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGuGaoCardPlugin::CGuGaoCardPlugin()
{
    m_bInit = false;
}; 

CGuGaoCardPlugin::~CGuGaoCardPlugin(void)
{
}; 

bool CGuGaoCardPlugin::init(void)
{
    if (m_bInit)
    {
        return true;
    }

    m_pInterfacemanage =  InterfaceManageFac::CreateInterfacemanage();
    if (m_pInterfacemanage != NULL)
    {
        m_pInterfacemanage->RegisterInterface(new CCardManagerService());
    }
    m_bInit = true;
	return true;
}

bool CGuGaoCardPlugin::deinit(void)
{
	//CCardManagerService::releaseInstance();
	return true;
}

// 查询接口函数需要插件内部自定义接口，通常不需要这么做，因此插件模板不做任何实现
// 当确实需要时，需要自行添加实现。
ISDKCallInterface* CGuGaoCardPlugin::GetInterface(unsigned int iid_interface)
//bool CGuGaoCardPlugin::queryInterface(const int iid_interface, void **ppInterface)
{
    ISDKCallInterface *ppInterface = NULL;
	//if (ICreateCardService::IID_CARDSERVICE == iid_interface)
	{
		ppInterface = (ISDKCallInterface *)(CCardManagerService::getInstance());
	}
	return ppInterface;
}