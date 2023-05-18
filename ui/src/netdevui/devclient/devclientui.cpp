#include "Devclientui.h"
#include "commonutils.h"
/*��ui�޸�*/
#include <QtGui>
#include "trace.h"
#include "uidefine.h"
#include <QDebug>
#include <QTextStream>
#include <iostream>
#include "cimageprocess.h"
#include "jthreadmonitor.h"
#include "sdkcommon/com/icommanager.h"
#include "sdkcommon/com/icom.h"
#include "socketlistui.h"
#include "socketproxy.h"
#include "socketlistcfg.h"

static int g_InitNet = 0;

void InitNetEvn()
{
	if (0 == g_InitNet)
	{
#ifdef Q_OS_WIN
		WORD wVersionRequested;
		WSADATA wsaData;
		wVersionRequested = MAKEWORD(2, 2);
		int err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			return;
		}
		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			WSACleanup();
			return;
		}
#endif
	}

	++g_InitNet;
}

void ClearNetEvn()
{
	--g_InitNet;
	if (0 == g_InitNet)
	{
#ifdef Q_OS_WIN
		WSACleanup();
#endif
	}
}


using namespace std;  
static const char *settingtitleText[] = {
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "OK" ),
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "Cancel"),
};


/*��ui�޸�*/
Devclientui::Devclientui()
    : m_pSocketlistui(NULL),
      m_CSocketClientListCfg(NULL)
{
	InitNetEvn();
    m_pSocketlistui = new Socketlistui();
    m_CSocketClientListCfg = CSocketClientListCfg::getInstance();
	m_bInit = false;

}

Devclientui::~Devclientui()
{
    if(m_pSocketlistui != NULL)
    {
        delete m_pSocketlistui;
        m_pSocketlistui = NULL;
    }

    if(m_CSocketClientListCfg != NULL)
    {
        CSocketClientListCfg::releaseInstance();
        m_CSocketClientListCfg = NULL;
    }
	ClearNetEvn();
}

bool Devclientui::init()
{
	/*��ui�޸�*/
    if (NULL != m_pSocketlistui)
    {
        m_pSocketlistui->init();
		
    }
    return true;
}

bool Devclientui::uninit()
{
    /*��ui�޸�*/
    if (NULL != m_pSocketlistui)
	{
        m_pSocketlistui->deinit();
	}    
    
    return true;
}

bool Devclientui::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    /*��ui�޸�*/
    if (NULL == m_pSocketlistui)
    {
        return false;
	}
    m_pSocketlistui->showWindow(parent,rect,bPopFlag);
    return true;
}

bool Devclientui::hideWindow()
{
    /*��ui�޸�*/
    if (NULL == m_pSocketlistui)
    {
        return false;
    }
    return m_pSocketlistui->hideWindow();
}

bool Devclientui::queryInterface(const int iid_interface, void **ppInterface)
{
    if(IUIPlugin::GET_MAIN_WIDGET_OBJECT == iid_interface)
    {
	    /*��ui�޸�*/
        *ppInterface = m_pSocketlistui;
    }
    else if(ISocketClientListCfg::IID_ISOCKETCLIENTLISTCFG)
    {
        *ppInterface = m_CSocketClientListCfg;
    }
    else
    {

    }
    return true;
}

void Devclientui::fitSize(const QRect& rect)
{
    /*��ui�޸�*/
    if (NULL == m_pSocketlistui)
    {
        return;
    }
    m_pSocketlistui->fitSize(rect);
}

bool Devclientui::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    /*��ui�޸�*/
    if (NULL == m_pSocketlistui)
    {
        return false;
    }
    return m_pSocketlistui->notify(event, wparam, lparam);
}


void Devclientui::SetLanguage(QString strlanguage)
{
    /*��ui�޸�*/
    if (NULL == m_pSocketlistui)
    {
        return ;
    }
    m_pSocketlistui->SetLanguage(strlanguage);

}

