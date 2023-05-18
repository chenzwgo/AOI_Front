#include "otherpage.h"
#include "uicommon/extendstackwidget.h"
#include "uicommon/commonutils.h"
#include "filevariable/ifilevariable.h"
#include "g12common.h"
#include "g12utils.h"
#include "stringcommon.h"
#include "callsdk.h"
#include "g12globalenv.h"
#include "vscameratools/cameramanager/icamerafactorymanage.h"
#include "./vscameratools/cameramanager/icamerabuffermanager.h"
#include "netlightcommunicationdlg.h" 
#include "pccommunicationdlg.h"
#include "sendmessage.h"

OtherPage::OtherPage(QWidget *parent):AbstractPageWidget(parent)
  ,m_pExtendAcquisition(NULL)
{
   m_pExtendAcquisition = new ExtendStackWidget(this);
   m_pExtendAcquisition->setTitleBtnWidth(60);
   
   m_pNetLightCommunicationDlg = new NetLightCommunicationDlg();//
   m_pPcCommunicationDlg       = new PcCommunicationDlg();

	m_pExtendAcquisition->addWidget(QString::fromLocal8Bit("网络IP通讯设定"), m_pNetLightCommunicationDlg);
	m_pExtendAcquisition->addWidget(QString::fromLocal8Bit("后端PC通讯"), m_pPcCommunicationDlg);
   //
   m_pExtendAcquisition->setCurrentWidget(0);

   QObject::connect(m_pExtendAcquisition,SIGNAL(currentChanged(int)),this,SLOT(on_currentChanged(int)));

}

void OtherPage::onEnable(bool enable)
{
	int index = m_pExtendAcquisition->currentIndex();
	AbstractPageWidget* pWidget = NULL;
	if (index == 0 && m_pNetLightCommunicationDlg != NULL)
	{
     	pWidget = qobject_cast<AbstractPageWidget*>(m_pNetLightCommunicationDlg);
	}
	else if (index == 1 && m_pPcCommunicationDlg != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pPcCommunicationDlg);
	}

	if (pWidget == NULL)
	{
		return;
	}
	pWidget->onEnable(enable);
	pWidget->setEnabled(enable);
}

void OtherPage::onSave()
{
	int index = m_pExtendAcquisition->currentIndex();
	AbstractPageWidget* pWidget = NULL;
	if (index == 0 && m_pNetLightCommunicationDlg != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pNetLightCommunicationDlg);
	}
	else if (index == 1 && m_pPcCommunicationDlg != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pPcCommunicationDlg);
	}

	if (pWidget == NULL)
	{
		return;
	}
    pWidget->updateProductName(m_strProductName);
    pWidget->onSave();
}

void OtherPage::onCancel()
{
	int index = m_pExtendAcquisition->currentIndex();
	AbstractPageWidget* pWidget = NULL;
	if (index == 0 && m_pNetLightCommunicationDlg != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pNetLightCommunicationDlg);
	}
	else if (index == 1 && m_pPcCommunicationDlg != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pPcCommunicationDlg);
	}

	if (pWidget == NULL)
	{
		return;
	}
    pWidget->onCancel();
}

void OtherPage::onUpdate()
{
	int index = m_pExtendAcquisition->currentIndex();
	AbstractPageWidget* pWidget = NULL;
	if (index == 0 && m_pNetLightCommunicationDlg != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pNetLightCommunicationDlg);
	}
	else if (index == 1 && m_pPcCommunicationDlg != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pPcCommunicationDlg);
	}

	if (pWidget == NULL)
	{
		return;
	}
    pWidget->updateProductName(m_strProductName);
    pWidget->onUpdate();
}


void OtherPage::on_currentChanged(int index)
{
    AbstractPageWidget* pWidget =  qobject_cast<AbstractPageWidget*>(m_pExtendAcquisition->widgetOf(index));
    if (pWidget != NULL)
    {
        pWidget->updateProductName(m_strProductName);
        pWidget->onUpdate();
    }
}

