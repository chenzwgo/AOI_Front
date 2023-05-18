#include "defectvalue.h"
#include "ui_defectvalue.h"

#include "expressionparser/scriptLexer.h"

defectvalue::defectvalue(itemValue* pitemValue, QDialog *parent) :
	QDialog(parent),
	m_pItemValue(pitemValue),
    ui(new Ui::defectvalue)
{
    ui->setupUi(this);
}

defectvalue::~defectvalue()
{
    delete ui;
}

void defectvalue::on_pushButton_ok_clicked()
{
	;

	m_pItemValue->dArea = ui->doubleSpinBox_Area->value();
	m_pItemValue->dScore = ui->doubleSpinBox_Score->value();
	m_pItemValue->dContrast = ui->doubleSpinBox_Contrast->value();
	m_pItemValue->dWidth = ui->doubleSpinBox_Width->value();
	m_pItemValue->dHeight = ui->doubleSpinBox_Height->value();
	m_pItemValue->dSkeletonLength = ui->doubleSpinBox_SkeletonLength->value();
	m_pItemValue->dPosX = ui->doubleSpinBox_PosX->value();
	m_pItemValue->dPosY = ui->doubleSpinBox_PosY->value();
	m_pItemValue->iSvm = ui->spinBox_Svm->value();


	QDialog::accept();
}

void defectvalue::on_pushButton_cancel_clicked()
{
	QDialog::reject();
}

