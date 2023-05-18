#include "camerasettingui.h"
#include "linecameramanagerui.h"
#include <QtGui>

static const char *settingtitleText[] = {
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "OK" ),
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "Cancel"),
};

CCameraSettingUI::CCameraSettingUI()
    : m_pCameraSettingUIDlg(NULL)
{
    m_pCameraSettingUIDlg = new CLineCameraManagerUI();
}

CCameraSettingUI::~CCameraSettingUI()
{
    if (NULL != m_pCameraSettingUIDlg)
    {
        delete m_pCameraSettingUIDlg;
        m_pCameraSettingUIDlg = NULL;
    }
}

bool CCameraSettingUI::init()
{
    if (NULL != m_pCameraSettingUIDlg)
    {
        m_pCameraSettingUIDlg->init();
    }
    return true;
}

bool CCameraSettingUI::uninit()
{
    if (NULL != m_pCameraSettingUIDlg)
    {
        m_pCameraSettingUIDlg->deinit();
    }
    return true;
}

bool CCameraSettingUI::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (NULL == m_pCameraSettingUIDlg)
    {
        return false;
    }
    return m_pCameraSettingUIDlg->showWindow(parent, rect,bPopFlag);
}

bool CCameraSettingUI::hideWindow()
{
    if (NULL == m_pCameraSettingUIDlg)
    {
        return false;
    }
    return m_pCameraSettingUIDlg->hideWindow();
}

bool CCameraSettingUI::queryInterface(const int iid_interface, void **ppInterface)
{
    if (IUIPlugin::GET_MAIN_WIDGET_OBJECT == iid_interface)
    {
        *ppInterface = m_pCameraSettingUIDlg;
    }
    return true;
}

void CCameraSettingUI::fitSize(const QRect& rect)
{
    if (NULL == m_pCameraSettingUIDlg)
    {
        return;
    }
    m_pCameraSettingUIDlg->fitSize(rect);
}

bool CCameraSettingUI::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    if (NULL == m_pCameraSettingUIDlg)
    {
        return false;
    }
    return m_pCameraSettingUIDlg->notify(event, wparam, lparam);
}


void CCameraSettingUI::SetLanguage(QString strlanguage)
{
    if (NULL == m_pCameraSettingUIDlg)
    {
        return ;
    }
    m_pCameraSettingUIDlg->SetLanguage(strlanguage);

}

