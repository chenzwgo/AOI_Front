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
	/*��ui�޸�*/
	void updateTextColor(const QColor& qClolor);
public slots:
   
private:
	QHBoxLayout* m_pHLayout;
	QChartView *m_chartView;
	/*��ui�޸�*/
	QColor     m_textColor;

};

#endif
