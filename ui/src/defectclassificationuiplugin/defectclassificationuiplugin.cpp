#include "DefectClassificationUIPlugin.h"
#include "DefectClassificationUI.h"
#include <QtGui>

DefectClassificationUIPlugin::DefectClassificationUIPlugin()
    : m_pDefectClassificationUI(NULL)
{

}

DefectClassificationUIPlugin::~DefectClassificationUIPlugin()
{

}

bool DefectClassificationUIPlugin::init()
{
    m_pDefectClassificationUI = DefectClassificationUI::getInstance();
    if (NULL != m_pDefectClassificationUI)
    {
        m_pDefectClassificationUI->init();
    }
    return true;
}

bool DefectClassificationUIPlugin::uninit()
{
    if (NULL != m_pDefectClassificationUI)
    {
        m_pDefectClassificationUI->deinit();
    }
   DefectClassificationUI::releaseInstance();
   return true;
}

bool DefectClassificationUIPlugin::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (NULL == m_pDefectClassificationUI)
    {
        return false;
    }
    return m_pDefectClassificationUI->showWindow(parent, rect,bPopFlag);
}

bool DefectClassificationUIPlugin::hideWindow()
{
    if (NULL == m_pDefectClassificationUI)
    {
        return false;
    }
    return m_pDefectClassificationUI->hideWindow();
}

bool DefectClassificationUIPlugin::queryInterface(const int iid_interface, void **ppInterface)
{
    if (IUIPlugin::GET_MAIN_WIDGET_OBJECT == iid_interface)
    {
        *ppInterface = (void*)m_pDefectClassificationUI;
        return true;
    }   
    return false;
}

void DefectClassificationUIPlugin::fitSize(const QRect& rect)
{
    if (NULL == m_pDefectClassificationUI)
    {
        return;
    }
    m_pDefectClassificationUI->fitSize(rect);
}

bool DefectClassificationUIPlugin::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    if (NULL == m_pDefectClassificationUI)
    {
        return false;
    }
    return m_pDefectClassificationUI->notify(event, wparam, lparam);
    return true;
}



void DefectClassificationUIPlugin::SetLanguage(QString strlanguage)
{
    if (NULL == m_pDefectClassificationUI)
    {
        return ;
    }
    m_pDefectClassificationUI->SetLanguage(strlanguage);
}


