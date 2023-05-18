#ifndef UPHBARCHART_H
#define UPHBARCHART_H
#include <QWidget>
#include <QChartView>
#include "maindefine.h"
QT_CHARTS_USE_NAMESPACE

class QHBoxLayout;

class UphBarChartWidget:public QWidget
{
public:

    explicit UphBarChartWidget(QWidget * parent = NULL);
    ~UphBarChartWidget();
    void clearUI();
	void updateUI(stUPHCfg* pstUPHCfg);
	void updateUI_Yield(QString strDate,double *dVal, int iDayOrNight,int ileng);
	void updateUI_UPH_Day(QString strDate, int *iVal, int iDayOrNight, int ileng);
	/*新ui修改*/
	void updateTextColor(const QColor& qClolor);
	void dayOrnightClicked(bool bDay = true);
private:
	QHBoxLayout* m_pHLayout;
	QChartView *m_chartView;
	int m_iDay;
	/*新ui修改*/
	QColor     m_textColor;

};

#endif
