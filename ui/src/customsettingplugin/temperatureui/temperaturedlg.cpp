#include "temperaturedlg.h"
#include "ui_temperaturedlg.h"
#include "motion/imotion.h"
#include "callsdk.h"
#include "rtdefine.h"

TemperatureDlg::TemperatureDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TemperatureDlg)
{
    ui->setupUi(this);
}

TemperatureDlg::~TemperatureDlg()
{
    delete ui;
}

void TemperatureDlg::showEvent(QShowEvent *event)
{
	on_pushButton_refresh_clicked();
}

void TemperatureDlg::on_pushButton_refresh_clicked()
{
	iMotion *pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
	RETURN_IF(NULL == pMotion);
	static ICard* pCard = pMotion->GetCardInstance("card");
	if (pCard)
	{
		IIO *pIo = pCard->GetIO(0);
		if (pIo)
		{
			QVector<QDoubleSpinBox*> spinBoxList;
			spinBoxList << ui->doubleSpinBox_temperature1 << ui->doubleSpinBox_temperature2 << ui->doubleSpinBox_temperature3 \
				<< ui->doubleSpinBox_temperature4;
			for (int i = 0; i < 4; i++)
			{
				double temperature = 0;
				pIo->getAdc(i, temperature);
				spinBoxList[i]->setValue(temperature);
			}
		}
	}
}