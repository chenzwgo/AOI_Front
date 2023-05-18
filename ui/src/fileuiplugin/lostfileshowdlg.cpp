#include "lostfileshowdlg.h"
#include "ui_lostfileshowdlg.h"
#include <QCloseEvent>

LostFileShowDlg::LostFileShowDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LostFileShowDlg)
{
    ui->setupUi(this);
	setResult(Operator_End);
}

LostFileShowDlg::~LostFileShowDlg()
{
    delete ui;
}

void LostFileShowDlg::setLostFile(const QStringList& lostFileList)
{
	foreach(QString strLostFile, lostFileList)
	{
		ui->textEdit_lostFile->append(strLostFile);
	}
}

void LostFileShowDlg::on_pushButton_useDefault_clicked()
{
	done(Operator_UseDefault);
	//exit(Operator_UseDefault);
}

void LostFileShowDlg::on_pushButton_stop_clicked()
{
	done(Operator_Stop);
	//exit(Operator_Stop);
}

void LostFileShowDlg::on_pushButton_ignore_clicked()
{
	done(Operator_Ignore);
	//exit(Operator_Ignore);
}

void LostFileShowDlg::closeEvent(QCloseEvent * event)
{
	event->ignore();
}
