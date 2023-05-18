#include "glasssizeshow.h"
#include "ui_glasssizeshow.h"

GlassSizeShow::GlassSizeShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::glasssizeshow)
{
    ui->setupUi(this);
}

GlassSizeShow::~GlassSizeShow()
{
    delete ui;
}

void GlassSizeShow::setValues(map<string, double> values)
{
	ui->label_A->setText(QString::number(values["A"], 'f', 3));
	ui->label_B->setText(QString::number(values["B"], 'f', 3));
	ui->label_C->setText(QString::number(values["C"], 'f', 3));
	ui->label_D->setText(QString::number(values["D"], 'f', 3));
	ui->label_E->setText(QString::number(values["E"], 'f', 3));
	ui->label_F->setText(QString::number(values["F"], 'f', 3));
	ui->label_G->setText(QString::number(values["G"], 'f', 3));
	ui->label_H->setText(QString::number(values["H"], 'f', 3));
	ui->label_I->setText(QString::number(values["I"], 'f', 3));
	ui->label_J->setText(QString::number(values["J"], 'f', 3));
}
