#ifndef REGION_CNT_BARCHART_H
#define REGION_CNT_BARCHART_H
#include <QWidget>
#include <QChartView>
#include "maindefine.h"
QT_CHARTS_USE_NAMESPACE

class QHBoxLayout;

class RegionCntBarChartWidget:public QWidget
{
public:

    explicit RegionCntBarChartWidget(QWidget * parent = NULL);
    ~RegionCntBarChartWidget();
    void clearUI();
	void updateUI(stProductSummary* pstProductSummary);
	/*��ui�޸�*/
	void updateTextColor(const QColor& qClolor);
	
private:
	QHBoxLayout* m_pHLayout;
	QChartView *m_chartView;
	/*��ui�޸�*/
	QColor     m_textColor;

};

#endif
