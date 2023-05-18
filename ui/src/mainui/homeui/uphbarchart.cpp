#include "uphbarchart.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QBarCategoryAxis>
#include <QHBoxLayout>
#include <QGraphicsLayout>
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "g12common.h"
QT_CHARTS_USE_NAMESPACE


UphBarChartWidget::UphBarChartWidget(QWidget *parent):QWidget(parent)
	,m_chartView(NULL)
{
	m_pHLayout = new QHBoxLayout(this);
	m_pHLayout->setMargin(0);

    m_pHLayout->setContentsMargins(0,0,0,0);
	setLayout(m_pHLayout);
	/*��ui�޸�*/
	//setStyleSheet("background - color: rgb(199, 201, 229);");
	m_iDay = DAY_BY_WORK_INDEX;
}

UphBarChartWidget::~UphBarChartWidget()
{

}

void UphBarChartWidget::clearUI()
{

}
void UphBarChartWidget::dayOrnightClicked(bool bDay)
{
	m_iDay = bDay?DAY_BY_WORK_INDEX: NIGHT_BY_WORK_INDEX;
}
void UphBarChartWidget::updateUI(stUPHCfg* pstUPHCfg)
{
	if (m_chartView != NULL)
	{
	    m_pHLayout->removeWidget(m_chartView);
		delete m_chartView;
		/*��ui�޸�*/
		m_chartView = NULL;
	}

	//![1]
	QString strLable = m_iDay == DAY_BY_WORK_INDEX? QString::fromLocal8Bit("�װ�ϼ�") : QString::fromLocal8Bit("ҹ��ϼ�");
	QBarSet *set0 = new QBarSet(strLable);
	//QBarSet *set1 = new QBarSet(QString::fromLocal8Bit("��һ�μ��"));
	//QBarSet *set2 = new QBarSet(QString::fromLocal8Bit("�ڶ��μ��"));
	//����     OK    NG      ��ϴ
	*set0 << pstUPHCfg[m_iDay].iTotalCnt << pstUPHCfg[m_iDay].iOKCnt + pstUPHCfg[m_iDay].iReCheck << pstUPHCfg[m_iDay].iNGCnt << pstUPHCfg[m_iDay].iReClean;
	//*set1 << pstUPHCfg[1].iTotalCnt << pstUPHCfg[1].iOKCnt + pstUPHCfg[1].iReCheck << pstUPHCfg[1].iNGCnt<< pstUPHCfg[1].iReClean;
	//*set2 << pstUPHCfg[2].iTotalCnt << pstUPHCfg[2].iOKCnt + pstUPHCfg[2].iReCheck << pstUPHCfg[2].iNGCnt<< pstUPHCfg[2].iReClean;
	//![1]

	//![2]
	QBarSeries *series = new QBarSeries();
	series->append(set0);
	//series->append(set1);
	//series->append(set2);
	series->setLabelsPosition(QAbstractBarSeries::LabelsInsideBase); // ��������ϵ�б�ǩ��λ�����������ڲ��Ϸ�
	series->setLabelsVisible(true); // ������ʾ����ϵ�б�ǩ

									//![2]
									//![3]
	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->setTitle(QString::fromLocal8Bit("����ͳ����Ϣ"));
	chart->setAnimationOptions(QChart::SeriesAnimations);

	double percentageOK = 0;
	double percentageNG =0;
	double percentageRecheck = 0;
	double percentageReClean =0;
	//![4]
	if (pstUPHCfg[m_iDay].iTotalCnt != 0)
	{
		 percentageOK = (pstUPHCfg[m_iDay].iOKCnt + pstUPHCfg[m_iDay].iReCheck )* 1000 / pstUPHCfg[m_iDay].iTotalCnt;
		 percentageNG = pstUPHCfg[m_iDay].iNGCnt * 1000 / pstUPHCfg[m_iDay].iTotalCnt;
		 //percentageRecheck = pstUPHCfg[0].iReCheck * 1000 / pstUPHCfg[0].iTotalCnt;
		 percentageReClean = pstUPHCfg[m_iDay].iReClean * 1000 / pstUPHCfg[m_iDay].iTotalCnt;
	}


	QStringList categories;
	categories << QString::fromLocal8Bit("����")
		//<< QString::fromLocal8Bit("A %1%").arg(percentageOK/10)
		//<< QString::fromLocal8Bit("D %1%").arg(percentageNG/10)
		//<< QString::fromLocal8Bit("B %1%").arg(percentageRecheck/10)
		//<< QString::fromLocal8Bit("C %1%").arg(percentageReClean/10);
		<< QString::fromLocal8Bit("OK %1%").arg(percentageOK / 10)
		<< QString::fromLocal8Bit("NG %1%").arg(percentageNG / 10)
	    << QString::fromLocal8Bit("RI %1%").arg(percentageReClean / 10);
		//<< QString::fromLocal8Bit("RI");
	QBarCategoryAxis *axis = new QBarCategoryAxis();
	axis->append(categories);
	axis->setTitleFont(QFont("Arial", 9));
	axis->setLabelsFont(QFont("Arial", 9));

	chart->createDefaultAxes();   //����Ĭ�ϵ�����ϵ����������ó��������в��˵�
	//chart->axisX()->setRange(0, 22);//����ͼ��������ķ�Χ�����Բ����ã��Զ����ڵ�
	//chart->axisY()->setRange(0, 20);//����ͼ��������ķ�Χ�����Բ����ã��Զ����ڵ�
	chart->setAxisX(axis, series);

	//![5]
	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->setBackgroundRoundness(0);
	chart->layout()->setContentsMargins(0, 0, 0, 0);
	//

	//
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
void UphBarChartWidget::updateTextColor(const QColor& textColor)
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
void UphBarChartWidget::updateUI_UPH_Day(QString strDate, int *iVal, int iDayOrNight, int ileng)
{
	if (m_chartView != NULL)
	{
		m_pHLayout->removeWidget(m_chartView);
		delete m_chartView;
		/*��ui�޸�*/
		m_chartView = NULL;
	}

	//![1]
	QBarSet *set0 = new QBarSet(QString::fromLocal8Bit(" uph "));
	//ƽ��ֵ  �װ�/ҹ�� ��Сʱ�ֲ� 

	for (int i = 0; i < ileng; i++)
	{
		*set0 << iVal[i];
	}
	QBarSeries *series = new QBarSeries();
	series->append(set0);
	series->setLabelsPosition(QAbstractBarSeries::LabelsInsideBase); // ��������ϵ�б�ǩ��λ�����������ڲ��Ϸ�
	
	series->setLabelsVisible(true); // ������ʾ����ϵ�б�ǩ
	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->setTitle(QString::fromLocal8Bit("uph�ֲ�"));
	chart->setAnimationOptions(QChart::SeriesAnimations);

	//![4]
	QStringList categories;
	//
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	int iSartTime = 8;
	if (pFile)
	{
		iSartTime = pFile->getValue(WORK_MODE_AM_STRAT_TIME).toInt(8);;//
	}
	//
	int istart = (0 == iDayOrNight)?iSartTime: (iSartTime+ DATE_TIMES);
	for (int i = istart; i < ileng+ istart; i++)
	{
		int index = (i >= DATE_TIMES * 2 )?( i - DATE_TIMES * 2 ): i;
		categories << g_timeFlag[index] + QString::fromLocal8Bit("��");//iDayOrNight
	}

	QBarCategoryAxis *axis = new QBarCategoryAxis();
	axis->append(categories);
	axis->setTitleFont(QFont("Arial", 9));
	axis->setLabelsFont(QFont("Arial", 9));

	chart->createDefaultAxes();   //����Ĭ�ϵ�����ϵ����������ó��������в��˵�
	//chart->axisX()->setRange(0, 22);//����ͼ��������ķ�Χ�����Բ����ã��Զ����ڵ�
	//chart->axisY()->setRange(0, 100);//����ͼ��������ķ�Χ�����Բ����ã��Զ����ڵ�
	chart->setAxisX(axis, series);

	//![5]
	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->setBackgroundRoundness(0);
	chart->layout()->setContentsMargins(0, 0, 0, 0);
	//

	//
	/*��ui�޸�*/
	//chart->setPlotAreaBackgroundBrush(QBrush (QColor(199, 201, 229)));

	chart->setBackgroundVisible(false);
	//![5]

	//![6]
	m_chartView = new QChartView(chart);
	m_chartView->setRenderHint(QPainter::Antialiasing);


	m_chartView->setStyleSheet("background-color: rgba(199, 201, 229, 0);");

	m_pHLayout->addWidget(m_chartView);

	updateTextColor(m_textColor);
}
void UphBarChartWidget::updateUI_Yield(QString strDate, double *dVal, int iDayOrNight, int ileng)
{
	if (m_chartView != NULL)
	{
		m_pHLayout->removeWidget(m_chartView);
		delete m_chartView;
		/*��ui�޸�*/
		m_chartView = NULL;
	}

	//![1]
	QBarSet *set0 = new QBarSet(  QString::fromLocal8Bit(" ���� "));
	//ƽ��ֵ  �װ�/ҹ�� ��Сʱ�ֲ� 
	
	for (int i = 0; i < ileng; i++)
	{
		*set0 << (int)dVal[i];// s����ͼ̫С�޷���ȫ��ʾС����
	}
	QBarSeries *series = new QBarSeries();
	series->append(set0);
	series->setLabelsPosition(QAbstractBarSeries::LabelsCenter); // ��������ϵ�б�ǩ��λ�����������ڲ��Ϸ�

	
	series->setLabelsVisible(true); // ������ʾ����ϵ�б�ǩ
	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->setTitle(QString::fromLocal8Bit("���ʷֲ�"));
	chart->setAnimationOptions(QChart::SeriesAnimations);

	//![4]
	QStringList categories;
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	int iSartTime = 8;
	if (pFile)
	{
		iSartTime = pFile->getValue(WORK_MODE_AM_STRAT_TIME).toInt(8);;//
	}
	//
	int istart = (0 == iDayOrNight) ? iSartTime : (iSartTime + DATE_TIMES);
	for (int i = istart; i < ileng+ istart; i++)
	{
		int index = (i >= DATE_TIMES * 2 )? (i - DATE_TIMES * 2) : i;
		categories << g_timeFlag[index] + QString::fromLocal8Bit("��");//iDayOrNight
	}
	

	//

	QBarCategoryAxis *axis = new QBarCategoryAxis();
	axis->append(categories);
	axis->setTitleFont(QFont("Arial", 9));
	axis->setLabelsFont(QFont("Arial", 9));

	chart->createDefaultAxes();   //����Ĭ�ϵ�����ϵ����������ó��������в��˵�
								  //chart->axisX()->setRange(0, 22);//����ͼ��������ķ�Χ�����Բ����ã��Զ����ڵ�
	chart->axisY()->setRange(0, 100);//����ͼ��������ķ�Χ�����Բ����ã��Զ����ڵ�
	chart->setAxisX(axis, series);

	//![5]
	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->setBackgroundRoundness(0);
	chart->layout()->setContentsMargins(0, 0, 0, 0);
	//

	//
	/*��ui�޸�*/
	//chart->setPlotAreaBackgroundBrush(QBrush (QColor(199, 201, 229)));

	chart->setBackgroundVisible(false);
	//![5]

	//![6]
	m_chartView = new QChartView(chart);
	m_chartView->setRenderHint(QPainter::Antialiasing);


	m_chartView->setStyleSheet("background-color: rgba(199, 201, 229, 0);");

	m_pHLayout->addWidget(m_chartView);

	updateTextColor(m_textColor);
}