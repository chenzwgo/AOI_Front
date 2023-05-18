#include "customsettingplugin.h"
#include "customui.h"
#include "debugui/debugview.h"
#include <QtGui>

static const char *settingtitleText[] = {
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "OK" ),
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "Cancel"),
};

CustomSettingUIPlugin::CustomSettingUIPlugin()
    : m_pCustomUIWidget(NULL),
	m_pDebugView(NULL)
{
    m_pCustomUIWidget = new CustomUi();
	if(NULL != m_pCustomUIWidget)
	{
		m_pDebugView = m_pCustomUIWidget->getDebugView();
	}
}

CustomSettingUIPlugin::~CustomSettingUIPlugin()
{
	/* if (NULL != m_pCustomUIWidget)
	{
	delete m_pCustomUIWidget;
	m_pCustomUIWidget = NULL;
	}*/
	//if (NULL != m_pDebugView)
	//{
	//	delete m_pDebugView;
	//	m_pDebugView = NULL;
	//}
	
}

bool CustomSettingUIPlugin::init()
{
    if (NULL != m_pCustomUIWidget)
    {
        m_pCustomUIWidget->init();
    }
    return true;
}

bool CustomSettingUIPlugin::uninit()
{
    if (NULL != m_pCustomUIWidget)
    {
        m_pCustomUIWidget->deinit();
    }
   return true;
}

bool CustomSettingUIPlugin::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (NULL == m_pCustomUIWidget)
    {
        return false;
    }
    return m_pCustomUIWidget->showWindow(parent, rect,bPopFlag);
}

bool CustomSettingUIPlugin::hideWindow()
{
    if (NULL == m_pCustomUIWidget)
    {
        return false;
    }
    return m_pCustomUIWidget->hideWindow();
}

bool CustomSettingUIPlugin::queryInterface(const int iid_interface, void **ppInterface)
{
    if (IUIPlugin::GET_MAIN_WIDGET_OBJECT == iid_interface)
    {
        *ppInterface = (void*)m_pCustomUIWidget;
        return true;
    }
	if (IUIPlugin::GET_DEBUG_VIEW_WIDGET_OBJECT== iid_interface)
	{
		*ppInterface = (void*)m_pDebugView;
		return true;
	}
	
    return false;
}

void CustomSettingUIPlugin::fitSize(const QRect& rect)
{
    if (NULL == m_pCustomUIWidget)
    {
        return;
    }
    m_pCustomUIWidget->fitSize(rect);
}

bool CustomSettingUIPlugin::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    if (NULL == m_pCustomUIWidget)
    {
        return false;
    }
    return m_pCustomUIWidget->notify(event, wparam, lparam);
}



void CustomSettingUIPlugin::SetLanguage(QString strlanguage)
{
    if (NULL == m_pCustomUIWidget)
    {
        return ;
    }
    m_pCustomUIWidget->SetLanguage(strlanguage);
}

