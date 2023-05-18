#include "defectheatmap.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QBarCategoryAxis>
#include <QHBoxLayout>
#include <QScatterSeries>
#include <QGraphicsLayout>
#include <QPointF>
#include <QScatterSeries>
#include <QValueAxis>
#include <QSplineSeries>
#include <QCategoryAxis>

QT_CHARTS_USE_NAMESPACE


DefectheatmapWidget::DefectheatmapWidget(QWidget *parent):QWidget(parent)
	,m_chartView(NULL)
	, m_bInit(false)
{
	m_pHLayout = new QHBoxLayout(this);
	m_pHLayout->setMargin(0);
	setLayout(m_pHLayout);

	setStyleSheet("background - color: rgb(199, 201, 229);");

}

DefectheatmapWidget::~DefectheatmapWidget()
{

}

void DefectheatmapWidget::initHeatmap(int RangeX, int RangeY, vector<STPoint>& vecPos)
{
	if (m_bInit)
	{
		return;
	}
	m_chartView = new QChartView();
	QChart *pQChart = m_chartView->chart();
	m_chartView->setRenderHint(QPainter::Antialiasing);
	m_chartView->setBackgroundBrush(QColor(199, 201, 229));
	m_chartView->setStyleSheet("background - color: rgb(199, 201, 229);");
	m_chartView->setRenderHint(QPainter::Antialiasing);

	m_scatter = new QScatterSeries();
	m_scatter->setName("defect");
	m_scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	m_scatter->setColor(Qt::red);
	m_scatter->setMarkerSize(5.0);
	
	for (int i = 0; i < vecPos.size();i++)
	{
		*m_scatter << QPointF(vecPos.at(i).x, vecPos.at(i).y);
	}

	pQChart->addSeries(m_scatter);

	QValueAxis *axisX = new QValueAxis;
	axisX->setRange(0, RangeX);    //设置范围
	axisX->setLabelFormat("%u");   //设置刻度的格式
	axisX->setGridLineVisible(true);   //网格线可见
	axisX->setTickCount(RangeX/1024);       //设置多少格
	//axisX->setMinorTickCount(1);   //设置每格小刻度线的数目
	pQChart->setAxisX(axisX, m_scatter);       //为chart设置X轴,并把轴附加到series线上


	QValueAxis *axisY = new QValueAxis;
	axisY->setReverse(true);
	axisY->setRange(0, RangeY);
	axisY->setLabelFormat("%u");
	axisY->setGridLineVisible(true);
	axisY->setTickCount(RangeY/512);
	//axisY->setMinorTickCount(1);
	pQChart->setAxisY(axisY, m_scatter);
	
	//pQChart->addAxis(axisX, Qt::AlignTop);
	//pQChart->addAxis(axisY, Qt::AlignLeft);

	pQChart->setTitle(QString::fromLocal8Bit("缺陷分布热力图"));
	pQChart->setAnimationOptions(QChart::SeriesAnimations);
	pQChart->legend()->setVisible(false);
	pQChart->legend()->setAlignment(Qt::AlignBottom);
	pQChart->setBackgroundRoundness(0);
	pQChart->layout()->setContentsMargins(0, 0, 0, 0);
	pQChart->setPlotAreaBackgroundBrush(QBrush(QColor(199, 201, 229)));
	pQChart->setBackgroundVisible(false);

	
	m_pHLayout->addWidget(m_chartView);
	m_bInit = true;
}

void DefectheatmapWidget::clearUI()
{
	m_scatter->clear();
}

void DefectheatmapWidget::updateUI(double dposx, double dposy)
{
	if (dposx == 0.0 && dposy == 0.0 )
	{
		return;
	}
	*m_scatter << QPointF(dposx, dposy);
}
