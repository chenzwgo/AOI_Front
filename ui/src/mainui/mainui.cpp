#include "mainui.h"
#include "commonutils.h"
/*新ui修改*/
#include "mainframe/mainframe.h"
#include <QtGui>
#include "homeui/homeview.h"
#include "trace.h"
#include "uidefine.h"
#include <QDebug>
#include <QTextStream>
#include <iostream>
#include "cimageprocess.h"
#include "jthreadmonitor.h"
#include "workflow/workflow.h"
#include "workflow/workflowfun.h"
#include "sdkcommon/com/icommanager.h"
#include "sdkcommon/com/icom.h"


using namespace std;  
static const char *settingtitleText[] = {
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "OK" ),
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "Cancel"),
};


/*新ui修改*/
Mainui::Mainui()
    : m_pMainFrame(NULL)
{
	m_pMainFrame = MainFrame::GetInstance();
	m_bInit = false;
}

Mainui::~Mainui()
{
    int iCount = JThreadMonitor::GetThreadCount();
    int iRunCount = JThreadMonitor::GetRunningThreadCount();
    const char *pThreadName;
    for (int i = 0; i < iCount; i++)
    {
        pThreadName = JThreadMonitor::GetExistedThreadCallClassName(i);
        LOG_INFO(pThreadName);
    }
    for (int i = 0; i < iRunCount; i++)
    {
        pThreadName = JThreadMonitor::GetRunningThraedCallClassName(i);
        LOG_INFO(pThreadName);
    }

	/*新ui修改*/
    MainFrame::DestroyInstance();
	m_pMainFrame = NULL;
}

bool Mainui::init()
{
	/*新ui修改*/
    if (NULL != m_pMainFrame)
    {
		m_pMainFrame->init();
		
		/*if (!m_bInit)
		{
			IUIPlugin* pD = CommonUtils::getIUIPlugin("usermanagementui");//
			if (NULL != pD)
			{
				pD->init();
			}
			m_bInit = true;
		}
		*/
    }
    return true;
}

bool Mainui::uninit()
{
    /*新ui修改*/
    if (NULL != m_pMainFrame)
	{
		m_pMainFrame->deinit();
	}    
    
    return true;
}

bool Mainui::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    /*新ui修改*/
    if (NULL == m_pMainFrame)
    {
        return false;
	}
	m_pMainFrame->showWindow(parent, rect, bPopFlag);
	CommonUtils::getIUIPlugin("usermanagementui"); //权限开关 
	 return true;
}

bool Mainui::hideWindow()
{
    /*新ui修改*/
    if (NULL == m_pMainFrame)
    {
        return false;
    }
    return m_pMainFrame->hideWindow();
}

bool Mainui::queryInterface(const int iid_interface, void **ppInterface)
{
    if(IUIPlugin::GET_MAIN_WIDGET_OBJECT == iid_interface)
    {
	    /*新ui修改*/
        *ppInterface = m_pMainFrame;
    }
    return true;
}

void Mainui::fitSize(const QRect& rect)
{
    /*新ui修改*/
    if (NULL == m_pMainFrame)
    {
        return;
    }
	m_pMainFrame->fitSize(rect);
}

bool Mainui::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    /*新ui修改*/
    if (NULL == m_pMainFrame)
    {
        return false;
    }
    return m_pMainFrame->notify(event, wparam, lparam);
}


void Mainui::SetLanguage(QString strlanguage)
{
    /*新ui修改*/
    if (NULL == m_pMainFrame)
    {
        return ;
    }
	m_pMainFrame->SetLanguage(strlanguage);

}

