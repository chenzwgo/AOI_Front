#include "dvdefecttypebarchart.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QBarCategoryAxis>
#include <QHBoxLayout>
#include <QGraphicsLayout>
#include <QtCharts/QLineSeries>
#include <QValueAxis>
#include <QStringList>

QT_CHARTS_USE_NAMESPACE


DvdefecttypeChartWidget::DvdefecttypeChartWidget(QWidget *parent):QWidget(parent)
{
	m_pHLayout = new QHBoxLayout(this);
	m_pHLayout->setMargin(0);
	setLayout(m_pHLayout);
	for (int i = 0; i < SMIRCH_TYPE_SIZE;i++)
	{
		m_chartView[i] = 0;
	}

	m_DefectNameList << QString::fromLocal8Bit("脏污") << QString::fromLocal8Bit("毛发") << QString::fromLocal8Bit("尘点");
    /*新ui修改*/
	//setStyleSheet("background - color: rgb(199, 201, 229);");

}

DvdefecttypeChartWidget::~DvdefecttypeChartWidget()
{

}

void DvdefecttypeChartWidget::clearUI()
{

}

void DvdefecttypeChartWidget::updateUI(stDvToplimitCfg* pstDvToplimitCfg, stProductSummary* pstProductSummary, int ipage )
{
	m_iPageIndex = ipage;
    updateUI_S(0, pstDvToplimitCfg, pstProductSummary->dvdefectSummaryAreaArr[en_dv_Smirch], pstProductSummary->dvdefectSummaryAreaArr[en_dv_Smirch]);
    updateUI_S(1, pstDvToplimitCfg, pstProductSummary->dvdefectSummaryAreaArr[en_dv_hair], pstProductSummary->dvdefectSummaryAreaArr[en_dv_hair]);
    updateUI_S(2, pstDvToplimitCfg, pstProductSummary->dvdefectSummaryAreaArr[en_dv_dirtpits], pstProductSummary->dvdefectSummaryAreaArr[en_dv_dirtpits]);
// 
// 	updateUI_S(0, pstDvToplimitCfg, 0.2, 70);
// 	updateUI_S(1, pstDvToplimitCfg, 0.3, 80);
// 	updateUI_S(2, pstDvToplimitCfg, 0.4, 100);
}

/*新ui修改*/
void DvdefecttypeChartWidget::updateTextColor(QChartView *pQChartView,const QColor& textColor)
{
	if (m_textColor != textColor)
	{
		m_textColor = textColor;
	}

	if (NULL == pQChartView)
	{
		return;
	}
	QChart* pChart = pQChartView->chart();

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
	QList<QAbstractAxis *> tempList = pQChartView->chart()->axes();
	for (int i = 0; i < tempList.size(); ++i)
	{
		tempList[i]->setLabelsColor(textColor);
	}
}

void DvdefecttypeChartWidget::updateTextColor(const QColor& qClolor)
{
	for (int i = 0; i < SMIRCH_TYPE_SIZE;i++ )
	{
		updateTextColor(m_chartView[i],qClolor);
	}
}

void DvdefecttypeChartWidget::updateUI_S(int index, stDvToplimitCfg* pstDvToplimitCfg, double dArea, int iCnt)
{

	if (m_chartView[index] != NULL)
	{
		delete m_chartView[index];
		m_chartView[index] = NULL;
	}
	QString strTypeName = m_DefectNameList[index];
	//![1]
	QBarSet *set0 = new QBarSet(strTypeName);

	if (m_iPageIndex ==0  )
	{
		*set0 << iCnt ;
	}
	else if (m_iPageIndex == 1)
	{
		*set0 << dArea;
	}
	
	set0->setColor(QColor(255, 0, 0, 150));
	set0->setLabelColor(QColor(0, 0, 0));
	set0->setLabelFont(QFont("微软雅黑"));


	//![2]
	QBarSeries *series = new QBarSeries();
	series->append(set0);
	series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd); // 设置数据系列标签的位置于数据柱内测上方
	series->setLabelsVisible(true); // 设置显示数据系列标签
	series->setLabelsAngle(30);//设置标签的角度
	
	QLineSeries *lineseries = new QLineSeries();
	lineseries->setName(strTypeName + QString::fromLocal8Bit("趋势图"));
	m_iAxisXMax = 1;
	m_iAxisYMax = 0;

	//if (m_iAxisXMax < m_listSmirchCnt[index].size())
	//{
	//	m_iAxisXMax = m_listSmirchCnt[index].size();
	//}

	////

	QLineSeries *lineseries_info = new QLineSeries();
	lineseries_info->setName(QString::fromLocal8Bit("干净线"));
	lineseries_info->setColor(Qt::green);
	lineseries_info->setPointLabelsClipping(false);
	QPen pen_info = lineseries_info->pen();
	pen_info.setWidth(3);
	lineseries_info->setPen(pen_info);


	QLineSeries *lineseries_warning = new QLineSeries();
	lineseries_warning->setName(QString::fromLocal8Bit("中等线"));
	lineseries_warning->setColor(QColor(255, 165, 0));
	lineseries_warning->setPointLabelsClipping(false);
	QPen pen_warning = lineseries_warning->pen();
	pen_warning.setWidth(3);
	lineseries_warning->setPen(pen_warning);


	QLineSeries *lineseries_error = new QLineSeries();
	lineseries_error->setName(QString::fromLocal8Bit("严重图"));
	lineseries_error->setColor(Qt::red);
	lineseries_error->setPointLabelsClipping(false);
	QPen pen_error = lineseries_error->pen();
	pen_error.setWidth(3);
	lineseries_error->setPen(pen_error);

	double iFirstLine = 0;
	double iSecondLine = 0;
	double iThirdLine = 0;
	if (m_iPageIndex == 0)
	{
		if (index == 0)
		{
			//if (m_iAxisYMax < pstDvToplimitCfg[0].idvSmirchCnt)
			//{
			//	m_iAxisYMax = pstDvToplimitCfg[0].idvSmirchCnt;
			//}

			iFirstLine = pstDvToplimitCfg[2].idvSmirchCnt;
			iSecondLine = pstDvToplimitCfg[1].idvSmirchCnt;
			iThirdLine = pstDvToplimitCfg[0].idvSmirchCnt;
		}
		else if (index == 1)
		{
			iFirstLine = pstDvToplimitCfg[2].idvHairCnt;
			iSecondLine = pstDvToplimitCfg[1].idvHairCnt;
			iThirdLine = pstDvToplimitCfg[0].idvHairCnt;

		}
		else if (index == 2)
		{

			iFirstLine = pstDvToplimitCfg[2].idvDirtpitsCnt;
			iSecondLine = pstDvToplimitCfg[1].idvDirtpitsCnt;
			iThirdLine = pstDvToplimitCfg[0].idvDirtpitsCnt;
		}



		if (iCnt >0 && iCnt< iFirstLine)
		{
			set0->setColor(QColor(Qt::green));
		}
		else if (iCnt >= iFirstLine && iCnt <= iSecondLine)
		{
			set0->setColor(QColor(255, 165, 0));
		}
		else if (iCnt > iSecondLine)
		{
			set0->setColor(QColor(Qt::red));
		}

		
	}
	else if (m_iPageIndex == 1)
	{
		if (index == 0)
		{
			//if (m_iAxisYMax < pstDvToplimitCfg[0].ddvSmirchArea)
			//{
			//	m_iAxisYMax = pstDvToplimitCfg[0].ddvSmirchArea;
			//}

			iFirstLine = pstDvToplimitCfg[2].ddvSmirchArea;
			iSecondLine = pstDvToplimitCfg[1].ddvSmirchArea;
			iThirdLine = pstDvToplimitCfg[0].ddvSmirchArea;
		}
		else if (index == 1)
		{
			iFirstLine = pstDvToplimitCfg[2].ddvHairArea;
			iSecondLine = pstDvToplimitCfg[1].ddvHairArea;
			iThirdLine = pstDvToplimitCfg[0].ddvHairArea;

		}
		else if (index == 2)
		{

			iFirstLine = pstDvToplimitCfg[2].ddvDirtArea;
			iSecondLine = pstDvToplimitCfg[1].ddvDirtArea;
			iThirdLine = pstDvToplimitCfg[0].ddvDirtArea;
		}

		if (dArea > 0 && dArea < iFirstLine)
		{
			set0->setColor(QColor(Qt::green));
		}
		else if (dArea >= iFirstLine && dArea <= iSecondLine)
		{
			set0->setColor(QColor(255, 165, 0));
		}
		else if (dArea > iSecondLine)
		{
			set0->setColor(QColor(Qt::red));
		}

	}

	

	if (m_iAxisYMax <= iThirdLine)
	{
		m_iAxisYMax = iThirdLine;
	}

	m_iAxisYMax = m_iAxisYMax * 2;
	m_iAxisXMax = m_iAxisXMax * 2;

	QString strToolTip = QString::fromLocal8Bit("可接受范围： %1～%2 \n 警告范围： %3～%4 \n 严重范围：%5～%6").arg(0).arg(iFirstLine).arg(iFirstLine).arg(iSecondLine).arg(iSecondLine).arg(iThirdLine);

	lineseries_info->append(QPointF(-1, iFirstLine));
	lineseries_info->append(QPointF(m_iAxisXMax, iFirstLine));

	lineseries_warning->append(QPointF(-1, iSecondLine));
	lineseries_warning->append(QPointF(m_iAxisXMax, iSecondLine));

	lineseries_error->append(QPointF(-1, iThirdLine));
	lineseries_error->append(QPointF(m_iAxisXMax, iThirdLine));

	QChart *chart = new QChart();
	chart->addSeries(series);
	//chart->addSeries(lineseries);
	QString strTitletmp = m_iPageIndex == 0 ? QString::fromLocal8Bit("个数") : QString::fromLocal8Bit("面积");
	QString strTitle = strTypeName + strTitletmp ;
	chart->setTitle(strTitle);
	chart->setAnimationOptions(QChart::SeriesAnimations);

	chart->addSeries(lineseries_info);
	chart->addSeries(lineseries_warning);
	chart->addSeries(lineseries_error);
	chart->setToolTip(strToolTip);


	QStringList categories;
	categories << strTypeName ;
	QBarCategoryAxis *axisX = new QBarCategoryAxis();
	axisX->append(categories);
	axisX->setTitleFont(QFont("Arial", 8));
	axisX->setLabelsFont(QFont("Arial", 8));
	//chart->createDefaultAxes();

	//QValueAxis *axisX = new QValueAxis();

	chart->setAxisX(axisX, lineseries_info);
	chart->setAxisX(axisX, lineseries_warning);
	chart->setAxisX(axisX, lineseries_error);
	//chart->setAxisX(axisX, lineseries);
	chart->setAxisX(axisX, series);
	//axisX->setRange(0, m_iAxisXMax);

	QValueAxis *axisY = new QValueAxis();
	chart->setAxisY(axisY, lineseries);
	chart->setAxisY(axisY, lineseries_info);
	chart->setAxisY(axisY, lineseries_warning);
	chart->setAxisY(axisY, lineseries_error);
	chart->setAxisY(axisY, series);
	axisY->setRange(0, m_iAxisYMax);
	axisY->setTickCount(10);


	//![5]
	chart->legend()->setVisible(false);
	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->setBackgroundRoundness(0);
	chart->layout()->setContentsMargins(0, 0, 0, 0);

	/*新ui修改*/
	//chart->setPlotAreaBackgroundBrush(QBrush (QColor(199, 201, 229)));

	chart->setBackgroundVisible(false);
	//![5]

	//![6]
	
	
	QChartView * pQChartView = new QChartView(chart);
	m_chartView[index] = pQChartView;
	pQChartView->setRenderHint(QPainter::Antialiasing);
	
	/*新ui修改*/
	//m_chartView->setBackgroundBrush(QColor(199, 201, 229));
	pQChartView->setStyleSheet("background-color: rgba(199, 201, 229, 0);");

	m_pHLayout->addWidget(pQChartView);
	m_pHLayout->setStretch(index, 0);


	/*新ui修改*/
	updateTextColor(pQChartView,m_textColor);
}
