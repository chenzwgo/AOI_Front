#include "regioncntbarchart.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QBarCategoryAxis>
#include <QHBoxLayout>
#include <QGraphicsLayout>

QT_CHARTS_USE_NAMESPACE


RegionCntBarChartWidget::RegionCntBarChartWidget(QWidget *parent):QWidget(parent)
	,m_chartView(NULL)
{
	m_pHLayout = new QHBoxLayout(this);
	m_pHLayout->setMargin(0);

    m_pHLayout->setContentsMargins(0,0,0,0);
	setLayout(m_pHLayout);
	/*��ui�޸�*/
	//setStyleSheet("background - color: rgb(199, 201, 229);");

}

RegionCntBarChartWidget::~RegionCntBarChartWidget()
{

}

void RegionCntBarChartWidget::clearUI()
{

}

void RegionCntBarChartWidget::updateUI(stProductSummary* pstProductSummary)
{
	if (m_chartView != NULL)
	{
	    m_pHLayout->removeWidget(m_chartView);
		delete m_chartView;
		/*��ui�޸�*/
		m_chartView = NULL;
	}

	//![1]
	QBarSet *set0 = new QBarSet(QString::fromLocal8Bit("�ϼ�"));
	for (int i = 1; i < DEFECT_REGION_SIZE; i++)
	{
		*set0 << pstProductSummary->dvdefectRegionCnt[i];//
	}


	//![2]
	QBarSeries *series = new QBarSeries();
	series->append(set0);
	series->setLabelsPosition(QAbstractBarSeries::LabelsInsideBase); // ��������ϵ�б�ǩ��λ�����������ڲ��Ϸ�
	series->setLabelsVisible(true); // ������ʾ����ϵ�б�ǩ

									//![2]
									//![3]
	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->setTitle(QString::fromLocal8Bit("��Ƭ����ȱ�ݸ���ͳ��"));
	chart->setAnimationOptions(QChart::SeriesAnimations);
	//![4]

	QStringList categories;
	for (int i = 1; i < DEFECT_REGION_SIZE; i++)
	{
		categories << QString::fromLocal8Bit("%1").arg(i);//
	}

	QBarCategoryAxis *axis = new QBarCategoryAxis();
	axis->append(categories);
	axis->setTitleFont(QFont("Arial", 9));
	axis->setLabelsFont(QFont("Arial", 9));

	chart->createDefaultAxes();   //����Ĭ�ϵ�����ϵ����������ó��������в��˵�
	chart->setAxisX(axis, series);

	//![5]
	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->setBackgroundRoundness(0);
	chart->layout()->setContentsMargins(0, 0, 0, 0);
	//

	chart->setBackgroundVisible(false);
	//![5]

	//![6]
	m_chartView = new QChartView(chart);
	m_chartView->setRenderHint(QPainter::Antialiasing);

	/*��ui�޸�*/
	//m_chartView->setBackgroundBrush(QColor(199, 201, 229));
	m_chartView->setStyleSheet("background-color: rgba(199, 201, 229, 0);");
	//m_chartView->setStyleSheet(R"(QGraphicsView{ background-image:url(:/image/show.png);})");
	m_pHLayout->addWidget(m_chartView);
	/*��ui�޸�*/
	updateTextColor(m_textColor);
}
/*��ui�޸�*/
void RegionCntBarChartWidget::updateTextColor(const QColor& textColor)
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

	//����
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
