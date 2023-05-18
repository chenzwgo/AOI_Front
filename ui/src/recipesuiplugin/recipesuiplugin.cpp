#include "RecipesUIPlugin.h"
#include "RecipesUI.h"
#include <QtGui>

RecipesUIPlugin::RecipesUIPlugin()
    : m_pRecipesUI(NULL)
{

}

RecipesUIPlugin::~RecipesUIPlugin()
{

}

bool RecipesUIPlugin::init()
{
    m_pRecipesUI = RecipesUI::getInstance();
    if (NULL != m_pRecipesUI)
    {
        m_pRecipesUI->init();
    }
    return true;
}

bool RecipesUIPlugin::uninit()
{
    if (NULL != m_pRecipesUI)
    {
        m_pRecipesUI->deinit();
    }
   RecipesUI::releaseInstance();
   return true;
}

bool RecipesUIPlugin::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (NULL == m_pRecipesUI)
    {
        return false;
    }
    return m_pRecipesUI->showWindow(parent, rect,bPopFlag);
}

bool RecipesUIPlugin::hideWindow()
{
    if (NULL == m_pRecipesUI)
    {
        return false;
    }
    return m_pRecipesUI->hideWindow();
}

bool RecipesUIPlugin::queryInterface(const int iid_interface, void **ppInterface)
{
    if (IUIPlugin::GET_MAIN_WIDGET_OBJECT == iid_interface)
    {
        *ppInterface = (void*)m_pRecipesUI;
        return true;
    }   
    return false;
}

void RecipesUIPlugin::fitSize(const QRect& rect)
{
    if (NULL == m_pRecipesUI)
    {
        return;
    }
    m_pRecipesUI->fitSize(rect);
}

bool RecipesUIPlugin::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    if (NULL == m_pRecipesUI)
    {
        return false;
    }
    return m_pRecipesUI->notify(event, wparam, lparam);
    return true;
}


void RecipesUIPlugin::SetLanguage(QString strlanguage)
{
    if (NULL == m_pRecipesUI)
    {
        return ;
    }
    m_pRecipesUI->SetLanguage(strlanguage);
}


