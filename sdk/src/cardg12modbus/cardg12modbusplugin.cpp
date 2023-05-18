
// GuGaoCardPlugin.cpp : ���� sdk�����

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

// ��ѯ�ӿں�����Ҫ����ڲ��Զ���ӿڣ�ͨ������Ҫ��ô������˲��ģ�岻���κ�ʵ��
// ��ȷʵ��Ҫʱ����Ҫ�������ʵ�֡�
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