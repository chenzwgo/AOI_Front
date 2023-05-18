#include "usermanagementui.h"
#include "usermanagementuidlg.h"
#include <QtGui>

static const char *settingtitleText[] = {
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "OK" ),
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "Cancel"),
};

CUsermanagementui::CUsermanagementui()
    : m_pUsermanagementuiDlg(NULL)
{
    m_pUsermanagementuiDlg = new CUsermanagementuiDlg();
}

CUsermanagementui::~CUsermanagementui()
{

}

bool CUsermanagementui::init()
{
    if (NULL != m_pUsermanagementuiDlg)
    {
        m_pUsermanagementuiDlg->init();
    }
    return true;
}

bool CUsermanagementui::uninit()
{
    if (NULL != m_pUsermanagementuiDlg)
    {
        delete m_pUsermanagementuiDlg;
        m_pUsermanagementuiDlg = NULL;
    }
    return true;
}

bool CUsermanagementui::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (NULL == m_pUsermanagementuiDlg)
    {
        return false;
    }
    return m_pUsermanagementuiDlg->showWindow(parent, rect,bPopFlag);
}

bool CUsermanagementui::hideWindow()
{
    if (NULL == m_pUsermanagementuiDlg)
    {
        return false;
    }
    return m_pUsermanagementuiDlg->hideWindow();
}

bool CUsermanagementui::queryInterface(const int iid_interface, void **ppInterface)
{
    if (IUIPlugin::GET_MAIN_WIDGET_OBJECT == iid_interface)
    {
        *ppInterface = m_pUsermanagementuiDlg;
    }
    return true;
}

void CUsermanagementui::fitSize(const QRect& rect)
{
    if (NULL == m_pUsermanagementuiDlg)
    {
        return;
    }
    m_pUsermanagementuiDlg->fitSize(rect);
}

bool CUsermanagementui::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    if (NULL == m_pUsermanagementuiDlg)
    {
        return false;
    }
    return m_pUsermanagementuiDlg->notify(event, wparam, lparam);
}

void CUsermanagementui::SetLanguage(QString strlanguage)
{
    if (NULL == m_pUsermanagementuiDlg)
    {
        return ;
    }
    m_pUsermanagementuiDlg->SetLanguage(strlanguage);

}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
       Q_EXPORT_PLUGIN2(usermanagementui, CUsermanagementui);
#endif




