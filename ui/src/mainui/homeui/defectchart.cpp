#include "defectchart.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QBarCategoryAxis>
#include <QHBoxLayout>
#include <QGraphicsLayout>
#include "sendmessage.h"
QT_CHARTS_USE_NAMESPACE


DefectChartWidget::DefectChartWidget(QWidget *parent):QWidget(parent)
	,m_chartView(NULL)
{
	m_pHLayout = new QHBoxLayout(this);
	m_pHLayout->setMargin(0);
	setLayout(m_pHLayout);
	/*新ui修改*/
	//setStyleSheet("background - color: rgb(199, 201, 229);");
	
}

DefectChartWidget::~DefectChartWidget()
{

}

void DefectChartWidget::clearUI()
{

}

void DefectChartWidget::updateUI(gHistogram* pHistogram, int iSize, int ihistogramTotalCnt)
{
	if (m_chartView != NULL)
	{
		//m_pHLayout->removeWidget(m_chartView);
		delete m_chartView;
		/*新ui修改*/
		m_chartView = NULL;
	}
	else if (0 == iSize || NULL == pHistogram)
	{
		return;
	}
	// 获取Y最大值
	//int iyValMax = 0;
	//for (int i = 0; i < iSize; i++)
	//{
	//	iyValMax = (pHistogram[i].pixel_num>iyValMax)? pHistogram[i].pixel_num: iyValMax;
	//}
	//if (iyValMax <= 0)
	//{
	//	return;
	//}
	//![1]
	QString strShow = QString::fromLocal8Bit("缺陷像素乘以个数总值：");
	strShow += QString::number(ihistogramTotalCnt);
	QBarSet *set0 = new QBarSet(strShow);

	// add  y val 
	int imaxNuber = 0;
	int index = 0;
	for (int i = 0; i < iSize; i++)  
	{
		if (pHistogram[i].pixel_num >= imaxNuber)
		{
			imaxNuber = pHistogram[i].pixel_num;
			index = i;
		}
		set0->append(pHistogram[i].pixel_num);
	}
	QBarSeries *series = new QBarSeries();
	series->append(set0);
	series->setLabelsPosition(QAbstractBarSeries::LabelsInsideBase); // 设置数据系列标签的位置于数据柱内测上方
	series->setLabelsVisible(true); // 设置显示数据系列标签
	QChart *chart = new QChart();
	chart->addSeries(series);
	QString strS = QString::fromLocal8Bit("灰度值分布-");
	strS += QString::fromLocal8Bit("最高数据-灰度值") + QString::number(pHistogram[index].gray_value);
	strS += QString::fromLocal8Bit("  个数-") + QString::number(pHistogram[index].pixel_num);
	chart->setTitle(strS);
	chart->setAnimationOptions(QChart::SeriesAnimations);

	//!add y val 
	QStringList categories;
	for (int i = 0; i < iSize; i++)
	{
		categories.push_back(QString::number(pHistogram[i].gray_value));
	}
	QBarCategoryAxis *axis = new QBarCategoryAxis();
	axis->append(categories);
	axis->setTitleFont(QFont("Arial", 9));
	axis->setLabelsFont(QFont("Arial", 9));
	chart->createDefaultAxes();
	chart->setAxisX(axis, series);

	//![5]
	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->setBackgroundRoundness(0);
	chart->layout()->setContentsMargins(0, 0, 0, 0);
	/*新ui修改*/
	//chart->setPlotAreaBackgroundBrush(QBrush (QColor(199, 201, 229)));
	
	chart->setBackgroundVisible(false);
	//![5]

	//![6]
	m_chartView = new QChartView(chart);
	m_chartView->setRenderHint(QPainter::Antialiasing);
	/*新ui修改*/
	//m_chartView->setBackgroundBrush(QColor(199, 201, 229));
	m_chartView->setStyleSheet("background-color: rgba(199, 201, 229, 0);");
	
	m_pHLayout->addWidget(m_chartView);
	/*新ui修改*/
	updateTextColor(m_textColor);
}
/*新ui修改*/
void DefectChartWidget::updateTextColor(const QColor& textColor)
{
	if (m_textColor != textColor)
	{
		m_textColor = textColor;
	}

	if (NULL == m_chartView)
	{
		return;
	}
	QChart* pChart = m_chartView->chart();

	//标题
	if (NULL == pChart)
	{
		return;
	}
	pChart->setTitleBrush(QBrush(textColor));

	//legend
	QLegend* pLegend = pChart->legend();
	if (NULL != pLegend)
	{
		pLegend->setLabelColor(textColor);
	}

	//axisX
	QAbstractAxis* pAxisX = pChart->axisX();
	if (NULL != pAxisX)
	{
		pAxisX->setLabelsColor(textColor);
	}

	//axisY
	QAbstractAxis* pAxisY = pChart->axisY();
	if (NULL != pAxisY)
	{
		pAxisY->setLabelsColor(textColor);
	}

	//
	QList<QAbstractAxis *> tempList = m_chartView->chart()->axes();
	for (int i = 0; i < tempList.size(); ++i)
	{
		tempList[i]->setLabelsColor(textColor);
	}
}
