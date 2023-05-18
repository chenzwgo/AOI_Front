#include "pccommunicationdlg.h"
#include "ui_pccommunicationdlg.h"
#include "g12common.h"
#include "g12globalenv.h"
#include <QFileInfo>
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "maindefine.h"
#include "lightcontrol_new.h"
#include <QFileDialog>
#include "inotifyservice.h"
#include <qmessagebox.h>
#include "netdevui/isocketlistcfg.h"
#include "netdevui/isocketproxy.h"
#include "iuiplugin.h"
#include "uicommon/commonutils.h"
PcCommunicationDlg::PcCommunicationDlg(QWidget *parent) :
	AbstractPageWidget(parent),
    ui(new Ui::PcCommunicationDlg)
{
    ui->setupUi(this);
	on_pushButton_backpc_Cancel_clicked();
}

PcCommunicationDlg::~PcCommunicationDlg()
{
    delete ui;
}
void PcCommunicationDlg::onEnable(bool enable)
{

}

void PcCommunicationDlg::onSave()
{
	on_pushButton_backpc_OK_clicked();
}

void PcCommunicationDlg::onCancel()
{
	onUpdate();
}

void PcCommunicationDlg::onUpdate()
{
	on_pushButton_backpc_Cancel_clicked();
}

void PcCommunicationDlg::updateProductName(const QString& strProductName)
{
	AbstractPageWidget::updateProductName(strProductName);

}
void PcCommunicationDlg::on_pushButton_backpc_OK_clicked()
{
	IFileVariable *pFile = getCameraSetCfgFileVariable(m_strProductName.toStdString());
	if (pFile)
	{
		pFile->setValue(BACKPC_ENABLE, ui->checkBox_enable_backPC->isChecked(), true);

		ui->pushButton_backpc_OK->hide();
		ui->pushButton_backpc_Cancel->setText(QString::fromLocal8Bit("修改"));

		ui->checkBox_enable_backPC->setEnabled(false);
		ui->pushButton_show_netdevui->setEnabled(false);
		ui->spinBox_syn_pro_sn->setEnabled(false);
		ui->pushButton_syn_sn->setEnabled(false);
		ui->pushButton_query_status->setEnabled(false);
		ui->pushButton_query_sn->setEnabled(false);

	}
	return;

}

void PcCommunicationDlg::on_pushButton_backpc_Cancel_clicked()
{
	bool bEn = false;

	IFileVariable *pFile = getCameraSetCfgFileVariable(m_strProductName.toStdString());
	if (pFile)
	{

		ui->pushButton_backpc_OK->hide();
	}

	if (QString::fromLocal8Bit("修改") == ui->pushButton_backpc_Cancel->text())
	{
		bEn = true;
		ui->pushButton_backpc_OK->show();
		ui->pushButton_backpc_Cancel->setText(QString::fromLocal8Bit("取消"));
	}
	else {
		bEn = false;
		ui->pushButton_backpc_Cancel->setText(QString::fromLocal8Bit("修改"));
	}

	bool bc = pFile->getValue(BACKPC_ENABLE).toBool();
	ui->checkBox_enable_backPC->setEnabled(bEn);
	ui->checkBox_enable_backPC->setChecked(bc);
	ui->pushButton_show_netdevui->setEnabled(bEn);
	ui->spinBox_syn_pro_sn->setEnabled(bEn);
	ui->pushButton_syn_sn->setEnabled(bEn);
	ui->pushButton_query_status->setEnabled(bEn);
	ui->pushButton_query_sn->setEnabled(bEn);

}
void PcCommunicationDlg::on_pushButton_show_netdevui_clicked()
{
	CommonUtils::ShowPlugin("netdevui");
}

void PcCommunicationDlg::on_pushButton_syn_sn_clicked()
{
	int iProSN = ui->spinBox_syn_pro_sn->value();
	if (QMessageBox::No == QMessageBox::warning(NULL, "warn", QString::fromLocal8Bit("请确认是否同步两台机的SN？"), QMessageBox::Yes | QMessageBox::No))
		return;

	INotifyService* pNotifyService = CommonUtils::getNotifyService();
	if (NULL == pNotifyService)
	{
		QMessageBox::information(this, "Info", QString::fromLocal8Bit("消息服务插件加载失败"));
		return;
	}

	pNotifyService->send("mainui", "set_product_sn", (OS_LPARAM)iProSN, NULL);
}

void PcCommunicationDlg::on_pushButton_query_sn_clicked()
{
	ISocketproxy* pCSocketproxy = NULL;;
	unsigned int  proSN = 0;
	IUIPlugin* pIUIPlugni = CommonUtils::getIUIPlugin("netdevui");
	if (pIUIPlugni == NULL)
	{
		return;
	}
	ISocketClientListCfg* pISocketClientListCfg = NULL;
	pIUIPlugni->queryInterface(ISocketClientListCfg::IID_ISOCKETCLIENTLISTCFG, (void**)&pISocketClientListCfg);
	if (pISocketClientListCfg == NULL)
	{
		return;
	}
	pCSocketproxy = pISocketClientListCfg->getSocketClientProxy("SendPC");
	if (pCSocketproxy == NULL)
	{
		QMessageBox::information(this, "Info", QString::fromLocal8Bit("获取通讯socket为空，请检查通讯IP与port"));;
		return;
	}
	if (SYS_ERR_OK != pCSocketproxy->sendQueryProductSN(proSN))
	{
		QMessageBox::information(this, "Info", QString::fromLocal8Bit("查询BackPC 产品SN失败"));
		return;
	}
	IFileVariable *pFileVar = getGlobalSNFileVariable();
	QString dateTime = QDateTime::currentDateTime().toString("yyyyMMdd");

	QString strMarkDay = QDateTime::currentDateTime().toString("yyyyMMdd");
	unsigned int mySn = 0;
	if (pFileVar)
	{
		mySn = pFileVar->getValue("ProductSN", strMarkDay.toStdString(), 0).toUInt();
	}
	QMessageBox::information(this, "Info", QString::fromLocal8Bit("查询  当前产品SN ( %1 )BackPC 产品SN ( %2 )").arg(mySn).arg(proSN));;
	return;
}

void PcCommunicationDlg::on_pushButton_query_status_clicked()
{
	ISocketproxy* pCSocketproxy = NULL;;
	int  iStatus = 0;
	IUIPlugin* pIUIPlugni = CommonUtils::getIUIPlugin("netdevui");
	if (pIUIPlugni == NULL)
	{
		return;
	}
	ISocketClientListCfg* pISocketClientListCfg = NULL;
	pIUIPlugni->queryInterface(ISocketClientListCfg::IID_ISOCKETCLIENTLISTCFG, (void**)&pISocketClientListCfg);
	if (pISocketClientListCfg == NULL)
	{
		return;
	}
	pCSocketproxy = pISocketClientListCfg->getSocketClientProxy("SendPC");
	if (pCSocketproxy == NULL)
	{
		QMessageBox::information(this, "Info", QString::fromLocal8Bit("获取通讯socket为空，请检查通讯IP与port"));;
		return;
	}
	if (SYS_ERR_OK != pCSocketproxy->sendQuerySysStatus(iStatus))
	{
		QMessageBox::information(this, "Info", QString::fromLocal8Bit("查询BackPC 工作状态失败"));;
		return;
	}

	QMessageBox::information(this, QString::fromLocal8Bit("BackPC工作状态:"), QString::fromLocal8Bit(szEWorkStateCn[iStatus]));;
	return;
}