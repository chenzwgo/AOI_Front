#ifndef DVDEFECTTYPEBARCHART_H
#define DVDEFECTTYPEBARCHART_H
#include <QWidget>
#include <QChartView>
#include <QList>
#include <QStringList>
#include "maindefine.h"
QT_CHARTS_USE_NAMESPACE

class QHBoxLayout;
#define  SMIRCH_TYPE_SIZE  3 

class DvdefecttypeChartWidget:public QWidget
{
public:
    explicit DvdefecttypeChartWidget(QWidget * parent = NULL);
    ~DvdefecttypeChartWidget();
    void clearUI();
    void updateUI(stDvToplimitCfg* pstDvToplimitCfg,stProductSummary* pstProductSummary, int ipage = 0);
	/*新ui修改*/
	void updateTextColor(const QColor& qClolor);
	void updateTextColor(QChartView *pQChartView, const QColor& qClolor);
private:
	void updateUI_S(int index ,stDvToplimitCfg* pstDvToplimitCfg, double dArea, int iCnt);
private:
	QHBoxLayout* m_pHLayout;
	QChartView *m_chartView[SMIRCH_TYPE_SIZE];
	double m_iAxisXMax;
	double m_iAxisYMax;
	QList<int> m_listSmirchCnt[SMIRCH_TYPE_SIZE];
	/*新ui修改*/
	QColor     m_textColor;
	QStringList m_DefectNameList;
	int m_iPageIndex;
};

#endif  //DVDEFECTTYPEBARCHART_H
