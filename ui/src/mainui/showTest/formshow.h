#ifndef FORMSHOW_H
#define FORMSHOW_H

#include <QWidget>
#include "maindefine.h"
#include "homeui/defectlevelpiechart.h"

namespace Ui {
class FormShow;
}

class FormShow : public QWidget
{
    Q_OBJECT

public:
	void updateData(QString strDate,int *idata,QStringList daytimelist,int ileng);
    explicit FormShow(QWidget *parent = 0);
    ~FormShow();

private:
	
    Ui::FormShow *ui;
	QChartView *m_pchartView;
	QHBoxLayout* m_pHLayout;
	DrilldownChart *m_DrilldownChart;
	vector<string> m_defectNameVec;

};

#endif // FORMSHOW_H
