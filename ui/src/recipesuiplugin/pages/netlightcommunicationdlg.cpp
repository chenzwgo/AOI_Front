#include "netlightcommunicationdlg.h"
#include "ui_netlightcommunicationdlg.h"
#include "g12common.h"
#include "g12globalenv.h"
#include <QFileInfo>
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "maindefine.h"
#include "lightcontrol_new.h"
#include <QFileDialog>
#include "inotifyservice.h"
#include <qmessagebox.h>
NetLightCommunicationDlg::NetLightCommunicationDlg(QWidget *parent) :
	AbstractPageWidget(parent),
    ui(new Ui::NetLightCommunicationDlg)
{
    ui->setupUi(this);
	m_plightcontrol_new = NULL;
	on_pushBtnIp_cancel_clicked();
}

NetLightCommunicationDlg::~NetLightCommunicationDlg()
{
    delete ui;
}
void NetLightCommunicationDlg::onEnable(bool enable)
{

}

void NetLightCommunicationDlg::onSave()
{
	on_pushBtnIp_ok_clicked();
}

void NetLightCommunicationDlg::onCancel()
{
	onUpdate();
}

void NetLightCommunicationDlg::onUpdate()
{
	on_pushBtnIp_cancel_clicked();
}

void NetLightCommunicationDlg::updateProductName(const QString& strProductName)
{
	AbstractPageWidget::updateProductName(strProductName);

}
void NetLightCommunicationDlg::on_pushBtnIp_ok_clicked()
{
	IFileVariable *pFile = getLightComFile();
	if (pFile)
	{

		for (int i = 0; i < LIGHT_SIZE; i++)
		{
	
			char ip[128] = { 0 };
			char port[128] = { 0 };
			char enable[128] = { 0 };

			sprintf(ip, "motionctl_ip_%d", i);
			sprintf(port, "motionctl_port_%d", i);
			sprintf(enable, "motionctl_enable_%d", i);

			QString lineEdit_ip = QString("lineEdit_ip_crl_%1").arg(i);
			QString spinBox_port_crl = QString("spinBox_port_crl_%1").arg(i);
			QString checkBox_light_crl = QString("checkBox_light_crl_%1").arg(i);

			pFile->setValue("motioncontrol", ip, getUItemValue(lineEdit_ip,"").toString().toStdString(), true);
			pFile->setValue("motioncontrol", port, getUItemValue(spinBox_port_crl, 0).toInt(),true);
			pFile->setValue("motioncontrol", enable, getUItemValue(checkBox_light_crl, 0).toInt(), true);
			enableUIItem(lineEdit_ip,false);
			enableUIItem(spinBox_port_crl, false);
			enableUIItem(checkBox_light_crl, false);
		}

		ui->pushBtnIp_ok->hide();
		ui->pushBtnIp_cancel->setText(QString::fromLocal8Bit("修改"));
		return;
	}
}
void NetLightCommunicationDlg::on_pushBtnIp_cancel_clicked()
{
	bool bEn = false;

	ui->pushBtnIp_ok->hide();
	if (QString::fromLocal8Bit("修改") == ui->pushBtnIp_cancel->text())
	{
		bEn = true;
		ui->pushBtnIp_ok->show();
		ui->pushBtnIp_cancel->setText(QString::fromLocal8Bit("取消"));
	}
	else {
		bEn = false;
		ui->pushBtnIp_cancel->setText(QString::fromLocal8Bit("修改"));
	}

	IFileVariable *pFile = getLightComFile();
	if (pFile)
	{
		for (int i = 0; i < LIGHT_SIZE; i++)
		{

			char ip[128] = { 0 };
			char port[128] = { 0 };
			char enable[128] = { 0 };

			sprintf(ip, "motionctl_ip_%d", i);
			sprintf(port, "motionctl_port_%d", i);
			sprintf(enable, "motionctl_enable_%d", i);

			QString lineEdit_ip = QString("lineEdit_ip_crl_%1").arg(i);
			QString spinBox_port_crl = QString("spinBox_port_crl_%1").arg(i);
			QString checkBox_light_crl = QString("checkBox_light_crl_%1").arg(i);
			
			setUIItemValue(lineEdit_ip, QString::fromStdString(pFile->getValue("motioncontrol", ip).toCString()));
			setUIItemValue(spinBox_port_crl, pFile->getValue("motioncontrol", port).toInt());
			setUIItemValue(checkBox_light_crl, pFile->getValue("motioncontrol", enable).toInt());

			enableUIItem(lineEdit_ip, bEn);
			enableUIItem(spinBox_port_crl, bEn);
			enableUIItem(checkBox_light_crl, bEn);
		}
	}
}

void NetLightCommunicationDlg::on_pushButton_getCtrType_crl_clicked()
{
	int index = ui->comboBox_ctrl_index->currentIndex();
	if (motioncontrol_new_manager::GetInstance()->getMotioncontrol(index) == NULL)
	{
		return;
	}
	uint8_t ls_type;
	int iRet = motioncontrol_new_manager::GetInstance()->getMotioncontrol(0)->setOrderToController(1, Order_GetCrl);

	QString msg = (0 == iRet) ? QString::fromLocal8Bit("测试通讯OK！") : QString::fromLocal8Bit("测试通讯失败！");

	QMessageBox::information(this, "Info", msg);;
}
void NetLightCommunicationDlg::on_pushButton_send_st_clicked()
{
	int index = ui->comboBox_ctrl_index->currentIndex();
	if (motioncontrol_new_manager::GetInstance()->getMotioncontrol(index) == NULL)
	{
		return;
	}
	ControllerParams_t params;
	params.net_id = ui->spinBox_net_id->value();
	params.order = ui->spinBox_order->value();
	params.trigger_frequence = ui->spinBox_frequence->value();


	params.all_pulse.all_pulse_1 = ui->spinBox_all_pulse_1->value();
	params.all_pulse.all_pulse_2 = ui->spinBox_all_pulse_2->value();


	params.equal_space.pulse_number_1 = ui->spinBox_pulse_number_1->value();
	params.equal_space.pulse_number_2 = ui->spinBox_pulse_number_2->value();

	params.delay_pulse.delay_pulse_1 = ui->spinBox_delay_pulse_1->value();
	params.delay_pulse.delay_pulse_2 = ui->spinBox_delay_pulse_2->value();

	int res = motioncontrol_new_manager::GetInstance()->getMotioncontrol(0)->setToController(params);
	QString msg = (0 == res) ? QString::fromLocal8Bit("下发成功！") : QString::fromLocal8Bit("下发失败！");

	QMessageBox::information(this, "Info", msg);;
	
}
void NetLightCommunicationDlg::on_pushButton_send_order_clicked()
{
	int index = ui->comboBox_ctrl_index->currentIndex();
	if (motioncontrol_new_manager::GetInstance()->getMotioncontrol(index) == NULL)
	{
		return;
	}
	uint8_t  NetId = ui->spinBox_t_net_id->value();
	Order_num Order = (Order_num)ui->spinBox_t_order->value();
	uint8_t CrlNumber = ui->spinBox_t_crl->value();

	int res = motioncontrol_new_manager::GetInstance()->getMotioncontrol(0)->setOrderToController(NetId, Order, CrlNumber);

	QString msg = (0 == res) ? QString::fromLocal8Bit("下发成功！") : QString::fromLocal8Bit("下发失败！");

	
	QMessageBox::information(this, "Info", msg);;
	

}