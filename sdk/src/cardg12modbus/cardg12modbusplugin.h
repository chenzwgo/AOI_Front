// GuGaoCardPlugin.h : GuGaoCardPlugin DLL ����ͷ�ļ�
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
	Function: �����ʼ��
	Intput: NULL
	Output:	NULL
	Return:	���س�ʼ������Ƿ�ɹ�
	******************************************************/
	virtual bool init(void);

	/****************************************************
	Function: ���ȥ��ʼ�����ͷ���Դ��
	Intput: NULL
	Output:	NULL
	Return:	����ȥ��ʼ���Ƿ�ɹ�
	******************************************************/
	virtual bool deinit(void);

	/****************************************************	
	Function: ��ѯ����ӿ�
	Intput: iid_interface �ӿڵ�iid
	Output:	ppInterface �ӿ�ָ���ָ��
	Return:	true�ɹ���falseʧ��
    ******************************************************/
    ISDKCallInterface* GetInterface(unsigned int InterfaceIndex = 0) ;

private:
    iInterfacemanage *m_pInterfacemanage;
    bool m_bInit;
};
