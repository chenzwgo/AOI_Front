#ifndef DEFECTCHART_H
#define DEFECTCHART_H
#include <QWidget>
#include <QChartView>
#include "maindefine.h"
QT_CHARTS_USE_NAMESPACE

class QHBoxLayout;

class DefectChartWidget:public QWidget
{
public:

    explicit DefectChartWidget(QWidget * parent = NULL);
    ~DefectChartWidget();
    void clearUI();
	void updateUI(gHistogram* pHistogram, int iSize,int ihistogramTotalCnt);
	/*新ui修改*/
	void updateTextColor(const QColor& qClolor);
public slots:
   
private:
	QHBoxLayout* m_pHLayout;
	QChartView *m_chartView;
	/*新ui修改*/
	QColor     m_textColor;

};

#endif
