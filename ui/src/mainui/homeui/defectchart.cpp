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
	/*��ui�޸�*/
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
		/*��ui�޸�*/
		m_chartView = NULL;
	}
	else if (0 == iSize || NULL == pHistogram)
	{
		return;
	}
	// ��ȡY���ֵ
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
	QString strShow = QString::fromLocal8Bit("ȱ�����س��Ը�����ֵ��");
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
	series->setLabelsPosition(QAbstractBarSeries::LabelsInsideBase); // ��������ϵ�б�ǩ��λ�����������ڲ��Ϸ�
	series->setLabelsVisible(true); // ������ʾ����ϵ�б�ǩ
	QChart *chart = new QChart();
	chart->addSeries(series);
	QString strS = QString::fromLocal8Bit("�Ҷ�ֵ�ֲ�-");
	strS += QString::fromLocal8Bit("�������-�Ҷ�ֵ") + QString::number(pHistogram[index].gray_value);
	strS += QString::fromLocal8Bit("  ����-") + QString::number(pHistogram[index].pixel_num);
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
	/*��ui�޸�*/
	//chart->setPlotAreaBackgroundBrush(QBrush (QColor(199, 201, 229)));
	
	chart->setBackgroundVisible(false);
	//![5]

	//![6]
	m_chartView = new QChartView(chart);
	m_chartView->setRenderHint(QPainter::Antialiasing);
	/*��ui�޸�*/
	//m_chartView->setBackgroundBrush(QColor(199, 201, 229));
	m_chartView->setStyleSheet("background-color: rgba(199, 201, 229, 0);");
	
	m_pHLayout->addWidget(m_chartView);
	/*��ui�޸�*/
	updateTextColor(m_textColor);
}
/*��ui�޸�*/
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
