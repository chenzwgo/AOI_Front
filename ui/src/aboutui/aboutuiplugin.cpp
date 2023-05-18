#include "aboutuiplugin.h"
#include "aboutuiservice.h"
#include <QtGui>

static const char *settingtitleText[] = {
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "OK" ),
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "Cancel"),
};

AboutUIPlugin::AboutUIPlugin()
    : m_pAboutUIService(NULL)
{

}

AboutUIPlugin::~AboutUIPlugin()
{

}

bool AboutUIPlugin::init()
{
    m_pAboutUIService = AboutUIService::getInstance();
    if (NULL != m_pAboutUIService)
    {
        m_pAboutUIService->init();
    }
    return true;
}

bool AboutUIPlugin::uninit()
{
   AboutUIService::releaseInstance();
   return true;
}

bool AboutUIPlugin::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (NULL == m_pAboutUIService)
    {
        return false;
    }
    return m_pAboutUIService->showWindow(parent, rect,bPopFlag);
}

bool AboutUIPlugin::hideWindow()
{
    if (NULL == m_pAboutUIService)
    {
        return false;
    }
    return m_pAboutUIService->hideWindow();
}

bool AboutUIPlugin::queryInterface(const int iid_interface, void **ppInterface)
{
    if (IUIPlugin::GET_MAIN_WIDGET_OBJECT == iid_interface)
    {
        *ppInterface = (void*)m_pAboutUIService->GetAboutWidget();
        return true;
    }   
    return false;
}

void AboutUIPlugin::fitSize(const QRect& rect)
{
    if (NULL == m_pAboutUIService)
    {
        return;
    }
    m_pAboutUIService->fitSize(rect);
}

bool AboutUIPlugin::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    if (NULL == m_pAboutUIService)
    {
        return false;
    }
    return m_pAboutUIService->notify(event, wparam, lparam);
    return true;
}



void AboutUIPlugin::SetLanguage(QString strlanguage)
{
    if (NULL == m_pAboutUIService)
    {
        return ;
    }
    m_pAboutUIService->SetLanguage(strlanguage);
}


#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(aboutuiplugin, AboutUIPlugin);
#endif
