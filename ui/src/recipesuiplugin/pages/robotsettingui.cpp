#include "robotsettingui.h"
#include "ui_robotsettingui.h"
#include "filevariable/ifilevariable.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "inotifyservice.h"
#include "QMessageBox"
#include "uicommon/commonutils.h"
#include "sdkcommon\com\icommanager.h"
#include "sdkcommon\com\icom.h"
#include "callsdk.h"
#include "motion\imotion.h"
#include "motion\iioctrlinterface.h"
#include "motion\imotioncfg.h"
#include <qmessagebox.h>
#include "sdkcommon\socket\isocketclient.h"
#include "robotcrl.h"
RobotSettingUi::RobotSettingUi(QWidget *parent) :
	AbstractPageWidget(parent),
    ui(new Ui::RobotSettingUi)
{
	m_pICom = NULL;
    ui->setupUi(this);
	onUpdate();
	m_testSn = 1;
}

RobotSettingUi::~RobotSettingUi()
{
	RobotCrl::GetInstance()->ReleaseInstance();
    delete ui;
}

void RobotSettingUi::on_pushButton_RI_clicked()
{

	RobotCrl::GetInstance()->send_sn_to_xialiaoji(m_testSn);
	m_testSn++;
	Sleep(100);
	bool res = RobotCrl::GetInstance()->send_sn_and_res_to_xialiaoji(m_testSn, 1);
	QString str = res ? "ok" : "error";
	QMessageBox::information(this, "warning", str);
}

void RobotSettingUi::on_pushButton_NG_clicked()
{

	RobotCrl::GetInstance()->send_sn_to_xialiaoji(m_testSn);
	m_testSn++;
	Sleep(100);
	bool res = RobotCrl::GetInstance()->send_sn_and_res_to_xialiaoji(m_testSn, 2);
	QString str = res ? "ok" : "error";
	QMessageBox::information(this, "warning", str);
}

void RobotSettingUi::on_pushButton_OK_clicked()
{

	RobotCrl::GetInstance()->send_sn_to_xialiaoji(m_testSn);
	Sleep(100);
	m_testSn++;
	bool res = RobotCrl::GetInstance()->send_sn_and_res_to_xialiaoji(m_testSn, 0);
	QString str = res?"ok":"error";
	QMessageBox::information(this, "warning", str);
	
}


void  RobotSettingUi::onEnable(bool enable)
{

}
void  RobotSettingUi::onSave()
{
	IFileVariable* pFileVariable = getCurrentRobotVariable(m_strProductName.toStdString());
	if (NULL != pFileVariable)
	{
		pFileVariable->setValue(ROBOT_SET_RES_DELAY, ui->spinBox_send_robot_delay_time->value(), true);
		pFileVariable->setValue(ROBOT_SET_RES_TIMES, ui->spinBox_send_robot_times->value(), true);
		pFileVariable->setValue(ROBOT_SET_AOI_IP, ui->lineEdit_aoi_ip->text().toStdString(), true);
		pFileVariable->setValue(ROBOT_SET_AOI_PORT, ui->spinBox_aoi_port->value(), true);
		pFileVariable->setValue(ROBOT_SET_ROBOT_IP, ui->lineEdit_robot_ip->text().toStdString(), true);
		pFileVariable->setValue(ROBOT_SET_ROBOT_PORT, ui->spinBox_robot_port->value(), true);
		pFileVariable->setValue(ROBOT_SET_ROBOT_LINE, ui->comboBox_a_b->currentIndex(), true);
		pFileVariable->setValue(ROBOT_SET_RECHECK_DATA, ui->checkBox__recheck_enable->isChecked(), true);
		pFileVariable->setValue(ROBOT_SET_RCV_TIMES, ui->spinBox_robot_rcv_timeout->value(), true);

		pFileVariable->setValue(PLC_SET_DELAY_TIMES, ui->spinBox_plc_time->value(), true);

		pFileVariable->setValue(PENMA_ENABLE, ui->checkBox_enable_penma->isCheckable(), true);
		pFileVariable->setValue(PENMA_COM_NAME, ui->lineEdit_comName->text().toStdString(), true);
		pFileVariable->setValue(PENMA_DELAY_TIMES, ui->spinBox_penma_delay->value(), true);

	}
}
void  RobotSettingUi::onCancel()
{
	onUpdate();
}
void  RobotSettingUi::onUpdate()
{
	IFileVariable* pFileVariable = getCurrentRobotVariable(m_strProductName.toStdString());
	if (NULL != pFileVariable)
	{
		int idelaytime = pFileVariable->getValue(ROBOT_SET_RES_DELAY, 1000).toInt();
		int itimes = pFileVariable->getValue(ROBOT_SET_RES_TIMES, 200).toInt();
		int iRecvTimeout = pFileVariable->getValue(ROBOT_SET_RCV_TIMES, 3500).toInt();
		ui->spinBox_send_robot_delay_time->setValue(idelaytime);
		ui->spinBox_send_robot_times->setValue(itimes);
		ui->spinBox_robot_rcv_timeout->setValue(iRecvTimeout);

		ui->lineEdit_aoi_ip->setText(pFileVariable->getValue(ROBOT_SET_AOI_IP).toCString().c_str());
		ui->spinBox_aoi_port->setValue(pFileVariable->getValue(ROBOT_SET_AOI_PORT,1000).toInt());
		ui->lineEdit_robot_ip->setText(pFileVariable->getValue(ROBOT_SET_ROBOT_IP).toCString().c_str());
		ui->spinBox_robot_port->setValue(pFileVariable->getValue(ROBOT_SET_ROBOT_PORT, 2000).toInt());
		ui->comboBox_a_b->setCurrentIndex(pFileVariable->getValue(ROBOT_SET_ROBOT_LINE, 0).toInt());
		ui->checkBox__recheck_enable->setChecked(pFileVariable->getValue(ROBOT_SET_RECHECK_DATA, true).toBool());

		ui->checkBox_enable_penma->setChecked(pFileVariable->getValue(PENMA_ENABLE, true).toBool());
		ui->lineEdit_comName->setText(pFileVariable->getValue(PENMA_COM_NAME).toCString().c_str());
		ui->spinBox_penma_delay->setValue(pFileVariable->getValue(PENMA_DELAY_TIMES, 1000).toInt());

		ui->spinBox_plc_time->setValue(pFileVariable->getValue(PLC_SET_DELAY_TIMES, 300).toInt());
	}
}
void RobotSettingUi::updateProductName(const QString& strProductName)
{
	AbstractPageWidget::updateProductName(strProductName);
	ui->label_programName->setText(strProductName);
}
void  RobotSettingUi::on_pushButton_send_clicked()
{
	//unsigned short lvale = ui->spinBox_plc_time->value()/10;
	//bool res = RobotCrl::GetInstance()->send_msg_to_plc(lvale);
	//
	//
	//if (res)
	//{
	//	QMessageBox::information(this, "OK", QString::fromLocal8Bit("OK"));
	//	return;
	//}
	//QMessageBox::warning(this, "error", QString::fromLocal8Bit("NG"));
	//RobotCrl::GetInstance()->send_sn_and_res_to_xialiaoji(9999,1);

	//RobotCrl::GetInstance()->send_sn_to_xialiaoji(10000);
	unsigned long  sn = 0;
	bool res = RobotCrl::GetInstance()->get_xialiaoji_sn(sn);
	QString str = res ? "ok" : "error";
	QMessageBox::information(this, "warning", str);
}

void  RobotSettingUi::on_pushButton_DisConnect_clicked()
{
    RobotCrl::GetInstance()->DisConnect();
}

void  RobotSettingUi::on_pushButton_ReConnect_clicked()
{
    RobotCrl::GetInstance()->ReConnect();
}

void RobotSettingUi::on_pushButton_opencom_clicked()
{
	QString strComName = ui->lineEdit_comName->text();
	if (strComName.length() == 0)
	{
		//QMessageBox::information(this, "warning", QString::fromLocal8Bit("串口名为空"));
		return;
	}
	if (m_pICom == NULL)
	{
		IComManager* pIComManager = (IComManager*)GetSDKInstance("sdkcommon", IComManager::IID_COMMANAGER);
		if (pIComManager != NULL)
		{
			m_pICom = pIComManager->CreateCom(strComName.toStdString().c_str());		
		}			
	}
	m_pICom->SetComName(strComName.toStdString().c_str());
	if (m_pICom->IsOpen())
	{
		bool bflag = m_pICom->CloseCom();
		if (!bflag)
		{
			QMessageBox::information(this, "warning", QString::fromLocal8Bit("close com fail"));
			return;
		}
		else
		{
			QMessageBox::information(this, "warning", QString::fromLocal8Bit("close com"));
		}
	}
	else
	{
		bool bflag = m_pICom->OpenCom();
		if (!bflag)
		{
			QMessageBox::information(this, "warning", QString::fromLocal8Bit("openCom Fail"));
			return;
		}
		else
		{
			unsigned long BaudRate = 9600;
			unsigned long Parity = 0;
			unsigned long ByteSize = 8;
			unsigned long StopBits = 1;
			m_pICom->SetComState(BaudRate, Parity, ByteSize, StopBits);
			QMessageBox::information(this, "warning", QString::fromLocal8Bit("openCom  ok"));
		}
	}
}

void RobotSettingUi::on_pushButton_trigpenma_clicked()
{
	IIOCtrlInterface* pIo = NULL;
	iMotion* pImotion = NULL;
    const char* ioName = "do_pen_out";
	pImotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);

	if (pImotion != NULL)
		pIo = pImotion->GetIOCtrlInterface();

	if (NULL != pIo)
	{
		bool bflag = pIo->writeio(ioName, true);
		if (!bflag)
		{
			QMessageBox::information(this, "warning", QString::fromLocal8Bit("trig IO fail"));
			return;
		}
		Sleep(100);
		pIo->writeio(ioName, false);
	}
}

void RobotSettingUi::on_pushButton_sendcontent_clicked()
{
	QString strComName = ui->lineEdit_comName->text();
	QString strContent = ui->lineEdit_content->text();
	if (strComName.length() == 0 )
	{
		return;
	}
	if (strContent.length() == 0)
	{
		return;
	}
	if (m_pICom != NULL)
	{
		IComManager* pIComManager = (IComManager*)GetSDKInstance("sdkcommon", IComManager::IID_COMMANAGER);
		if (pIComManager != NULL)
			m_pICom = pIComManager->CreateCom(strComName.toStdString().c_str());
	}
	if (!m_pICom->IsOpen())
	{
		bool bflag = m_pICom->OpenCom();
	}

	QStringList strPrinterMsg = strContent.split(",");

	char msg[4][64] = { 0 };
	toPenmaMsg(strPrinterMsg, msg);

	for (int i = 0; i < 4; i++)
	{
		char buff[64] = {0};
		buff[0] = 0x02;
		sprintf(buff+1, "SCM00%d:::0:1%s:", i + 1, msg[i]);
		buff[strlen(buff)] = 0x03;

		int ilenght = m_pICom->WriteCom((void*)buff, strlen(buff));
		if (ilenght <0)
		{
			QMessageBox::information(this, "warning", QString::fromLocal8Bit("WriteCom fail"));
			return;
		}
		else
		{
			Sleep(50);
			char rcvBuff[128] = { 0 };
			int iReclength = m_pICom->ReadCom(rcvBuff, 1);
			if (iReclength >0)
			{
				int irecv = rcvBuff[0];
				if (rcvBuff[0] == 0x06)
				{
					QMessageBox::information(this, "warning", QString::fromLocal8Bit("send successfull!"));
				}
				else
				{
					QMessageBox::information(this, "warning", QString::fromLocal8Bit("send fail，errcode(%1)").arg(irecv));
				}
			}
			else
			{
				QMessageBox::information(this, "warning", QString::fromLocal8Bit("ReadCom fail"));
			}
		}
	}

}



void RobotSettingUi::toPenmaMsg(QStringList & msgList, char penmsg[4][64])
{
	int msgNumber = msgList.size();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < msgNumber && j <64; j++)
		{
			QString msg = msgList.at(j);
			if (msg.length() == 4)
			{
				penmsg[i][j] = msg.at(i).toLatin1();
			}
		}
	}
}
