
#ifndef DEFECTLEVELPIECHART_H
#define DEFECTLEVELPIECHART_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QPieSeries>
#include <QChartView>
#include <QHBoxLayout>
#include "maindefine.h"

QT_CHARTS_USE_NAMESPACE

class MainSlice : public QPieSlice
{
    Q_OBJECT
public:
    MainSlice(QPieSeries *breakdownSeries, QObject *parent = 0);

    QPieSeries *breakdownSeries() const;

    void setName(QString name);
    QString name() const;

public Q_SLOTS:
    void updateLabel();

private:
    QPieSeries *m_breakdownSeries;
    QString m_name;
};

class DonutBreakdownChart : public QChart
{
public:
    DonutBreakdownChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    void addBreakdownSeries(QPieSeries *series, QColor color);
	void removeAllBreakdownSeries();

private:
    void recalculateAngles();
    void updateLegendMarkers();

private:
    QPieSeries *m_mainSeries;
};
//

QT_CHARTS_BEGIN_NAMESPACE
class QAbstractSeries;
class QPieSlice;
QT_CHARTS_END_NAMESPACE
class DrilldownChart : public QChart
{
	Q_OBJECT
public:
	explicit DrilldownChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
	~DrilldownChart();
	void changeSeries(QAbstractSeries *series);
	public Q_SLOTS:
	void handleSliceClicked(QPieSlice *slice);

private:
	QAbstractSeries *m_currentSeries;
};


QT_CHARTS_BEGIN_NAMESPACE
class QAbstractSeries;
QT_CHARTS_END_NAMESPACE
class DrilldownSlice : public QPieSlice
{
	Q_OBJECT

public:
	DrilldownSlice(qreal value, QString prefix, QAbstractSeries *drilldownSeries);
	virtual ~DrilldownSlice();
	QAbstractSeries *drilldownSeries() const;

	public Q_SLOTS:
	void updateLabel();
	void showHighlight(bool show);

private:
	QAbstractSeries *m_drilldownSeries;
	QString m_prefix;
};
//
class Defectlevelpiechart:public QWidget
{
public:
    Defectlevelpiechart(QWidget* parent = NULL);
    ~Defectlevelpiechart();
    void clearUI();
    void updateUI(int dvdefectNgArr[MAX_DEFECT_SIZE], int dvdefectRiArr[MAX_DEFECT_SIZE], bool bDateData = true,int OkCnt = 1,int RiCnt=0);
private:
	DonutBreakdownChart * m_pdonutBreakdown;
	QChartView *m_pchartView;
	QHBoxLayout* m_pHLayout;
    QPieSeries * seriesArray[MAX_DEFECT_SIZE];
    //vector<QPieSeries *> m_seriesVec;
    //bool m_defectTypeEnableVec[DEFECT_TYPE_SIZE];
    vector<bool> m_defectTypeEnableVec;
    //QColor colors[DEFECT_TYPE_SIZE];
    vector<QColor> m_colorsVec;
    //int m_data[DEFECT_TYPE_SIZE] ;
    vector<int> m_dataVec;
	bool m_defectTypeEnablebChange;
	DrilldownChart *m_DrilldownChart;
	vector<string> m_defectNameVec;
	vector<pair<string, bool>> m_defectEnableVec;
	vector<string> m_defectEnNameVec;
};

#endif // DONUTBREAKDOWNCHART_H
