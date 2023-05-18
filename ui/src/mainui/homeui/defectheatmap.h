#ifndef DEFECTHEATMAP_H
#define DEFECTHEATMAP_H
#include <QWidget>
#include <QChartView>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include "maindefine.h"
#include "common_data.h"
QT_CHARTS_USE_NAMESPACE

class QHBoxLayout;
;
class DefectheatmapWidget:public QWidget
{
public:

    explicit DefectheatmapWidget(QWidget * parent = NULL);
    ~DefectheatmapWidget();
	void initHeatmap(int RangeX, int RangeY, vector<STPoint>& vecPos);
    void clearUI();
    void updateUI(double dposx, double dposy);
private:
	bool m_bInit;
	QHBoxLayout* m_pHLayout;
	QChartView *m_chartView;
	QScatterSeries* m_scatter;

};

#endif
