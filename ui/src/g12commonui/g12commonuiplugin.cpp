#include "g12commonuiplugin.h"
#include "record/cdefectrecord.h"
#include <QtGui>

G12CommoneUIPlugin::G12CommoneUIPlugin()
    : m_pRecordView(NULL)
{
    m_pRecordView = CDefectRecord::getInstance();
}

G12CommoneUIPlugin::~G12CommoneUIPlugin()
{
    if (NULL != m_pRecordView)
    {
        CDefectRecord::releaseInstance();
    }	
}

bool G12CommoneUIPlugin::init()
{
    if (NULL != m_pRecordView)
    {
        m_pRecordView->init();
    }
    return true;
}

bool G12CommoneUIPlugin::uninit()
{
    if (NULL != m_pRecordView)
    {
        m_pRecordView->deinit();
    }
   return true;
}

bool G12CommoneUIPlugin::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    return true;
}

bool G12CommoneUIPlugin::hideWindow()
{
    return true;
}

bool G12CommoneUIPlugin::queryInterface(const int iid_interface, void **ppInterface)
{
    if (IDefectRecordInterface::IID_DEFECT_RECORD_INTERFACE)
    {
        *ppInterface = (void*)m_pRecordView;
    }
	
    return true;
}

void G12CommoneUIPlugin::fitSize(const QRect& rect)
{
}

bool G12CommoneUIPlugin::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    return true;
}



void G12CommoneUIPlugin::SetLanguage(QString strlanguage)
{
}

