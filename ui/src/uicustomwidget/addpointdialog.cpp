#include "addpointdialog.h"
#include "ui_addpointdialog.h"
#include "station/istation.h"
#include <QMessageBox>
#include <QCloseEvent>

AddPointDialog::AddPointDialog(IStation* station,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPointDialog),
	m_pCurrentAxisStation(station)
{
    ui->setupUi(this);
	this->installEventFilter(this);
	int index = GetNextIndex();
	ui->spinBox_pointIndex->setValue(index);
	ui->lineEdit_pointName->setText("point_"+QString::number(index));
}

AddPointDialog::~AddPointDialog()
{
    delete ui;
}

QString AddPointDialog::getPointName()
{
    return ui->lineEdit_pointName->text();
}

int AddPointDialog::getPointIndex()
{
    return ui->spinBox_pointIndex->value();
}

void AddPointDialog::on_buttonBox_accepted()
{
    int pointIndex = getPointIndex();
    QString pointName = getPointName();
    if (IsSameNameAndIndex(pointIndex,pointName))
    {
        accept();
    }
}

bool AddPointDialog::IsSameNameAndIndex(int pointIndex, QString point )
{
	if (!m_pCurrentAxisStation)
	{
		QMessageBox::warning(this,"warn", QString::fromLocal8Bit("当前工站为空指针!"));
		return true;
	}
    if (point.isEmpty())
    {
        QMessageBox::warning(this,"warn","The point's name should not be empty!");
        return false;
    }
    if (point.length() >= NAME_LEN)
    {
        QMessageBox::warning(this,"warn","The point's name is too long.");
        return false;
    }
	const map<int,StPoint>* allPoint = m_pCurrentAxisStation->GetAllPoint();
	if (NULL == allPoint) return false;

	map<int,StPoint>::const_iterator iter = allPoint->begin();
	while(iter!=allPoint->end())
	{
		StPoint stPoint = iter->second;
		if (iter->first == pointIndex)
		{
			QMessageBox::warning(this,"warn", QString::fromLocal8Bit("点索引重复!"));
			return false;
		}
		if (point == stPoint.name)
		{
			QMessageBox::warning(this,"warn", QString::fromLocal8Bit("点名称重复!"));
			return false;
		}
		++iter;
	}
	return true;
}

int AddPointDialog::GetNextIndex()
{
	if (!m_pCurrentAxisStation)
	{
		QMessageBox::warning(this,"warn", QString::fromLocal8Bit("当前工站为空指针!"));
		return true;
	}

	const map<int,StPoint>* allPoint = m_pCurrentAxisStation->GetAllPoint();
	if (NULL == allPoint) return false;

	int index = 0;
	map<int,StPoint>::const_iterator iter = allPoint->begin();
	while(iter!=allPoint->end())
	{
		StPoint stPoint = iter->second;
		if (iter->first >= index)
		{
			index = iter->first + 1;
		}
		++iter;
	}
	return index;
}
