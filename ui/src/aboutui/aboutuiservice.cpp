#include "aboutuiservice.h"
#include "aboutdlg.h"

AboutUIService* AboutUIService::m_pInstance = NULL;


AboutUIService::AboutUIService()
    : m_pAboutDlg(NULL)
{

}

AboutUIService::~AboutUIService()
{

}

AboutUIService* AboutUIService::getInstance()
{
    try
    {
        if (NULL == m_pInstance)
        {
            m_pInstance = new AboutUIService;
        }
    }
    catch (...)
    {
    }
    return m_pInstance;
}

void AboutUIService::releaseInstance()
{
    try
    {
        if (NULL != m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }
    catch (...)
    {
    }
}

bool AboutUIService::init()
{
    m_pAboutDlg = AboutDlg::getInstance();
    if (NULL == m_pAboutDlg)
    {
        return false;
    }

    return m_pAboutDlg->init();
}

bool AboutUIService::deinit()
{
    AboutDlg::releaseInstance();
    return true;
}

bool AboutUIService::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (NULL == m_pAboutDlg)
    {
        return false;
    }

    return m_pAboutDlg->showWindow(parent, rect,bPopFlag);
}

bool AboutUIService::hideWindow()
{
    if (NULL == m_pAboutDlg)
    {
        return false;
    }

    return m_pAboutDlg->hideWindow();
}

void AboutUIService::setPluginContainer(void *pPluginContainer)
{
    if (NULL == m_pAboutDlg)
    {
        return;
    }
    m_pAboutDlg->setPluginContainer(pPluginContainer);
}

void AboutUIService::fitSize(const QRect& rect)
{
    if (NULL == m_pAboutDlg)
    {
        return;
    }
    m_pAboutDlg->fitSize(rect);
}

bool AboutUIService::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    if (NULL == m_pAboutDlg)
    {
        return false;
    }
    return m_pAboutDlg->notify(event, wparam, lparam);
}

void AboutUIService::SetLanguage(QString strPluginName)
{
    if (NULL == m_pAboutDlg)
    {
        return ;
    }

    return m_pAboutDlg->SetLanguage(strPluginName);
}

QWidget* AboutUIService::GetAboutWidget()
{
    return m_pAboutDlg;
}
