#include "fileuiplugin.h"
#include "fileuimanager.h"
#include <QtGui>

static const char *settingtitleText[] = {
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "OK" ),
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "Cancel"),
};

FileUiPlugin::FileUiPlugin()
    : m_pUIService(NULL)
{

}

FileUiPlugin::~FileUiPlugin()
{

}

bool FileUiPlugin::init()
{
    m_pUIService = new FileUiManager();
    if (NULL != m_pUIService)
    {
        m_pUIService->init();
    }
    return true;
}

bool FileUiPlugin::uninit()
{
   if(m_pUIService != NULL)
   {
       delete m_pUIService;
   }
   return true;
}

bool FileUiPlugin::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (NULL == m_pUIService)
    {
        return false;
    }
    return m_pUIService->showWindow(parent, rect,bPopFlag);
}

bool FileUiPlugin::hideWindow()
{
    if (NULL == m_pUIService)
    {
        return false;
    }
    return m_pUIService->hideWindow();
}

bool FileUiPlugin::queryInterface(const int iid_interface, void **ppInterface)
{
    if (IUIPlugin::GET_MAIN_WIDGET_OBJECT == iid_interface)
    {
        *ppInterface = (void*)m_pUIService;
        return true;
    }   
    return false;
}

void FileUiPlugin::fitSize(const QRect& rect)
{
    if (NULL == m_pUIService)
    {
        return;
    }
    m_pUIService->fitSize(rect);
}

bool FileUiPlugin::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    if (NULL == m_pUIService)
    {
        return false;
    }
      return m_pUIService->notify(event, wparam, lparam);
    return true;
}



void FileUiPlugin::SetLanguage(QString strlanguage)
{
    if (NULL == m_pUIService)
    {
        return ;
    }
    m_pUIService->SetLanguage(strlanguage);
}


