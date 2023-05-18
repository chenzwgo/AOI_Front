#include "openfiledlg.h"
#include "ui_openfiledlg.h"
#include "fileuicommon.h"
#include <QMessageBox>
#include "lostfileshowdlg.h"

OpenFileDlg::OpenFileDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenFileDlg)
{
	ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
	m_bPressed = false;


	ui->tableWidget_programs->setColumnWidth(0, 106);
	ui->tableWidget_programs->setColumnWidth(1, 180);
	ui->tableWidget_programs->horizontalHeader()->setDefaultSectionSize(150);
	ui->tableWidget_programs->horizontalHeader()->setMinimumHeight(58);
	ui->tableWidget_programs->setFocusPolicy(Qt::NoFocus);
}

OpenFileDlg::~OpenFileDlg()
{
    delete ui;
}

void OpenFileDlg::InitUi(const QString& strCurrentFile, const QMap<QString, StProgramInfo>& mapProInfo)
{
	ui->label_program->setText(strCurrentFile);

	ui->tableWidget_programs->setRowCount(mapProInfo.size());
	ui->tableWidget_programs->clearContents();
	for (int i = 0; i < mapProInfo.keys().size(); i++)
	{
		if (ui->tableWidget_programs->item(i, 0))
		{
			ui->tableWidget_programs->item(i, 0)->setText(QString::number(i));
		}
		else
		{
			ui->tableWidget_programs->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
		}
		ui->tableWidget_programs->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		if (ui->tableWidget_programs->item(i, 1))
		{
			ui->tableWidget_programs->item(i, 1)->setText(mapProInfo.keys().at(i));
		}
		else
		{
			ui->tableWidget_programs->setItem(i, 1, new QTableWidgetItem(mapProInfo.keys().at(i)));
		}
		ui->tableWidget_programs->item(i, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		if (ui->tableWidget_programs->item(i, 2))
		{
			ui->tableWidget_programs->item(i, 2)->setText(mapProInfo[mapProInfo.keys().at(i)].strProgramDesc);
		}
		else
		{
			ui->tableWidget_programs->setItem(i, 2, new QTableWidgetItem(mapProInfo[mapProInfo.keys().at(i)].strProgramDesc));
		}
		ui->tableWidget_programs->item(i, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}
}

QString OpenFileDlg::GetCurrentProgram()
{
	return ui->label_program->text();
}

void OpenFileDlg::setAreaMovable(const QRect rt)
{
	if (m_areaMovable != rt)
	{
		m_areaMovable = rt;
	}
}

void OpenFileDlg::mousePressEvent(QMouseEvent *e)
{
	//鼠标左键
	if (e->button() == Qt::LeftButton)
	{
		m_ptPress = e->pos();
		m_bPressed = m_areaMovable.contains(m_ptPress);
		setAreaMovable(this->rect());
	}
}

void OpenFileDlg::mouseMoveEvent(QMouseEvent *e)
{
	if (m_bPressed)
	{
		this->move(pos() + e->pos() - m_ptPress);
	}
}

void OpenFileDlg::mouseReleaseEvent(QMouseEvent *e)
{
	m_bPressed = false;
}
void OpenFileDlg::showEvent(QShowEvent *)
{
	setAreaMovable(this->rect());
}

void OpenFileDlg::on_pushButton_ok_clicked()
{

	int icurRow = ui->tableWidget_programs->currentRow();
	if (icurRow == -1)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请选中要切换的程序！"));
		return;
	}
	QString strFileName = ui->tableWidget_programs->item(icurRow, 1)->text();
	if (0 == strFileName.compare(ui->label_program->text()))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请勿重复切换当前程序！"));
		return;
	}

	//判断文件是否有丢失
	QStringList lostFileList;
	QString strTmp = QString::fromLocal8Bit("是否保存当前程序？");
	if (FileUiCommon::isLostFile(PROGRAM_MODE, PROGRAM_PATH + strFileName, lostFileList) && lostFileList.size() != 0)
	{
		LostFileShowDlg tempDlg(this);
		tempDlg.setLostFile(lostFileList);
		tempDlg.exec();
		int iRs = tempDlg.result();
		if (LostFileShowDlg::Operator_Stop == iRs)
		{
			return;
		}
		else if (LostFileShowDlg::Operator_UseDefault == iRs)
		{
			if (FileUiCommon::qCopyDirectory("programs/program_model", "programs/" + strFileName, false))
			{
				strTmp = QString::fromLocal8Bit("设置完成！\n") + strTmp;
			}
			else
			{
				strTmp = QString::fromLocal8Bit("设置失败！\n") + strTmp;
			}
		}
	}

	int iRes = QMessageBox::information(this, QString::fromLocal8Bit("提示"), strTmp, QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
	if (QMessageBox::Cancel == iRes)
	{
		return;
	}
	ui->label_program->setText(strFileName);
	if (QMessageBox::Yes == iRes)
	{
		done(NO_SAVE_CRENT);
	}
	done(SAVE_CURRENT);
}

void OpenFileDlg::on_pushButton_cancel_clicked()
{
	done(CANCEL);
}

void OpenFileDlg::SetDayModel()
{
	setStyleSheet("QWidget#widget{border-image: url(:/image/day/selectprotect.png);}");

	ui->tableWidget_programs->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:rgba(0,0,0,0);padding-left: 1px;border: 1px solid #e8e8e8;font: 75 15pt \"微软雅黑\";}");
	ui->tableWidget_programs->setStyleSheet("background-color: rgba(255, 255, 255, 0);gridline-color: rgb(232, 232, 232);color: rgb(0, 0, 0); border: 1px solid #e8e8e8;}");

	ui->pushButton_ok->setStyleSheet("border-image: url(:/image/general/buttonok.png); image: none; background-image: none; color: rgb(255, 255, 255);background-color: rgba(0,0,0,0)");
	ui->pushButton_cancel->setStyleSheet("border-image: url(:/image/general/buttoncancel.png); image: none; background-image: none; color: rgb(0, 237, 240);background-color: rgba(0,0,0,0)");
}

/*新ui修改*/
void OpenFileDlg::SetNightModel()
{
	setStyleSheet("QWidget#widget{border-image: url(:/image/night/selectprotect.png);}");

	ui->tableWidget_programs->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:rgba(0,0,0,0);padding-left: 1px;border: 1px solid #00d1d7;font: 75 15pt \"微软雅黑\";}");
	ui->tableWidget_programs->setStyleSheet("background-color: rgba(255, 255, 255, 0);gridline-color: rgb(0, 209, 215);color: rgb(0, 237, 240); border: 1px solid #00d1d7;}");

	ui->pushButton_ok->setStyleSheet("border-image: url(:/image/general/buttonok.png);color: rgb(255, 255, 255);");
	ui->pushButton_cancel->setStyleSheet("border-image: url(:/image/general/buttoncancel.png);color: rgb(0, 237, 240);");
}
