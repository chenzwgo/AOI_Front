#include "visionimagetool.h"
#include "uitoolbox/visionimagetooldlg.h"
#include <QtGui>

static const char *settingtitleText[] = {
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "OK" ),
    QT_TRANSLATE_NOOP( "QDialogButtonBox", "Cancel"),
};

CVisionimagetool::CVisionimagetool()
    : m_pVisionimagetoolDlg(NULL)
{
    m_pVisionimagetoolDlg = new CVisionimagetoolDlg();
}

CVisionimagetool::~CVisionimagetool()
{

}

bool CVisionimagetool::init()
{
    if (NULL != m_pVisionimagetoolDlg)
    {
        m_pVisionimagetoolDlg->init();
    }
    return true;
}

bool CVisionimagetool::uninit()
{
    if (NULL != m_pVisionimagetoolDlg)
    {
        delete m_pVisionimagetoolDlg;
        m_pVisionimagetoolDlg = NULL;
    }
    return true;
}

bool CVisionimagetool::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (NULL == m_pVisionimagetoolDlg)
    {
        return false;
    }
    return m_pVisionimagetoolDlg->showWindow(parent, rect,bPopFlag);
}

bool CVisionimagetool::hideWindow()
{
    if (NULL == m_pVisionimagetoolDlg)
    {
        return false;
    }
    return m_pVisionimagetoolDlg->hideWindow();
}

bool CVisionimagetool::queryInterface(const int iid_interface, void **ppInterface)
{
    return true;
}

void CVisionimagetool::fitSize(const QRect& rect)
{
    if (NULL == m_pVisionimagetoolDlg)
    {
        return;
    }
    m_pVisionimagetoolDlg->fitSize(rect);
}

bool CVisionimagetool::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    if (NULL == m_pVisionimagetoolDlg)
    {
        return false;
    }
    return m_pVisionimagetoolDlg->notify(event, wparam, lparam);
}


void CVisionimagetool::SetLanguage(QString strlanguage)
{
    if (NULL == m_pVisionimagetoolDlg)
    {
        return ;
    }
    m_pVisionimagetoolDlg->SetLanguage(strlanguage);

}


#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(visionimagetool, CVisionimagetool);
#endif
