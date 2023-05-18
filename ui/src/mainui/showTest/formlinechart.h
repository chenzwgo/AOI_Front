#ifndef FORMLINECHART_H
#define FORMLINECHART_H
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QChartView>
#include <QHBoxLayout>
QT_CHARTS_USE_NAMESPACE
#include <QWidget>
#include "maindefine.h"
namespace Ui {
class FormLineChart;
}

class FormLineChart : public QWidget
{
    Q_OBJECT

public:
	static FormLineChart* GetInstance(QWidget* pParant = NULL);
	static void ReleaseInstance();
    explicit FormLineChart(QWidget *parent = 0);
	bool updateUi();
    ~FormLineChart();
	void CurrentChanged(QString ChangedDate, bool bDay = true);
	void dayOrnightClicked(bool bDay = true);

private:
	void updateDaystoChanged();//更新当前有多少天的数据可以显示
	bool getData();
	void updateshow();
	static FormLineChart* m_pSelfInstance;
    Ui::FormLineChart *ui;
	QChartView  *m_pchartView;
	stUPHCfg* m_stUPHCfg;
	int m_iDay;
	double m_dVal[DATE_DATA_ALL];
	QString m_strDate;
};

#endif // FORMLINECHART_H
