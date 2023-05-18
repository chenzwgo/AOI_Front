#include "keyfeatureswidget.h"
#include "ui_keyfeatureswidget.h"

KeyFeaturesWidget::KeyFeaturesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyFeaturesWidget)
{
    ui->setupUi(this);
}

KeyFeaturesWidget::~KeyFeaturesWidget()
{
    delete ui;
}
