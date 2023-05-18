
#include "defectlevelpiechart.h"
#include <QtCharts/QPieSlice>
#include <QtCharts/QPieLegendMarker>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QHBoxLayout>
#include <QGraphicsLayout>
#include "filevariable/ifilevariable.h"
#include "callsdk.h"
#include "g12common.h"
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"

QT_CHARTS_USE_NAMESPACE

//![1]
MainSlice::MainSlice(QPieSeries *breakdownSeries, QObject *parent)
    : QPieSlice(parent),
      m_breakdownSeries(breakdownSeries)
{
    connect(this, &MainSlice::percentageChanged, this, &MainSlice::updateLabel);
}
//![1]

QPieSeries *MainSlice::breakdownSeries() const
{
    return m_breakdownSeries;
}

void MainSlice::setName(QString name)
{
    m_name = name;
}

QString MainSlice::name() const
{
    return m_name;
}

//![2]
void MainSlice::updateLabel()
{
    this->setLabel(QString("%1 %2 (%3%)").arg(m_name).arg(value()).arg(percentage() * 100, 0, 'f', 2));
}

//![1]
DonutBreakdownChart::DonutBreakdownChart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    // create the series for main center pie
    m_mainSeries = new QPieSeries();
    m_mainSeries->setPieSize(0.7);
    QChart::addSeries(m_mainSeries);
}
//![1]

//![2]
void DonutBreakdownChart::addBreakdownSeries(QPieSeries *breakdownSeries, QColor color)
{
    QFont font("Arial", 8);

    // add breakdown series as a slice to center pie
    MainSlice *mainSlice = new MainSlice(breakdownSeries);
    mainSlice->setName(breakdownSeries->name());
    mainSlice->setValue(breakdownSeries->sum());
    m_mainSeries->append(mainSlice);

    // customize the slice
    mainSlice->setBrush(color);
    mainSlice->setLabelVisible();
    mainSlice->setLabelColor(Qt::white);
    mainSlice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
    mainSlice->setLabelFont(font);

    // position and customize the breakdown series
    breakdownSeries->setPieSize(0.8);
    breakdownSeries->setHoleSize(0.7);
    breakdownSeries->setLabelsVisible();
    const auto slices = breakdownSeries->slices();
    for (QPieSlice *slice : slices) {
        color = color.lighter(115);
        slice->setBrush(color);
        slice->setLabelFont(font);
    }

    // add the series to the chart
    QChart::addSeries(breakdownSeries);

    // recalculate breakdown donut segments
    recalculateAngles();

    // update customize legend markers
    updateLegendMarkers();
}

void DonutBreakdownChart::removeAllBreakdownSeries()
{
	QList<QPieSlice *> slices  = m_mainSeries->slices();
	for each (QPieSlice * var in slices)
	{
		if (var != NULL)
		{
			delete var;
		}
	}
	m_mainSeries->clear();
	QChart::removeAllSeries();
}

//![2]

//![3]
void DonutBreakdownChart::recalculateAngles()
{
    qreal angle = 0;
    const auto slices = m_mainSeries->slices();
    for (QPieSlice *slice : slices) {
        QPieSeries *breakdownSeries = qobject_cast<MainSlice *>(slice)->breakdownSeries();
        breakdownSeries->setPieStartAngle(angle);
        angle += slice->percentage() * 360.0; // full pie is 360.0
        breakdownSeries->setPieEndAngle(angle);
    }
}
//![3]

//![4]
void DonutBreakdownChart::updateLegendMarkers()
{
    // go through all markers
    const auto allseries = series();
    for (QAbstractSeries *series : allseries) {
        const auto markers = legend()->markers(series);
        for (QLegendMarker *marker : markers) {
            QPieLegendMarker *pieMarker = qobject_cast<QPieLegendMarker *>(marker);
            if (series == m_mainSeries) {
                // hide markers from main series
                pieMarker->setVisible(false);
            } else {
                // modify markers from breakdown series
                pieMarker->setLabel(QString("%1 %2 (%3%)")
                                    .arg(pieMarker->slice()->label())
									.arg(pieMarker->slice()->value())
                                    .arg(pieMarker->slice()->percentage() * 100, 0, 'f', 2));
				;
                pieMarker->setFont(QFont("Arial", 8));
            }
        }
    }
}


Defectlevelpiechart::Defectlevelpiechart(QWidget *parent):QWidget(parent)
, m_pdonutBreakdown(NULL)
, m_pchartView(NULL)
, m_pHLayout(NULL)
, m_DrilldownChart(NULL)
{
	IDefectListManage* p = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p)
	{
		m_defectNameVec = p->getDefectListInstance(0)->getDefectCnNameList();
		m_defectEnableVec = p->getDefectListInstance(0)->getDefectEnableList();
		m_defectEnNameVec = p->getDefectListInstance(0)->getDefectEnNameList();
	}

	m_defectTypeEnablebChange = false;
	m_pHLayout = new QHBoxLayout(this);
	m_pHLayout->setMargin(0);
    setLayout(m_pHLayout);

    for (int i = 0; i < m_defectNameVec.size();i++)
	{
		seriesArray[i] = NULL;
	}

	for (int i = 0; i < m_defectNameVec.size(); i += 5)
	{
		m_colorsVec.push_back(Qt::green);
		m_colorsVec.push_back(Qt::magenta);
		m_colorsVec.push_back(Qt::darkGray);
		m_colorsVec.push_back(Qt::black);
		m_colorsVec.push_back(Qt::lightGray);
	}
	
	setStyleSheet("background - color: rgb(199, 201, 229);");


}

Defectlevelpiechart::~Defectlevelpiechart()
{
	if (m_pchartView != NULL)
	{
		m_pHLayout->removeWidget(m_pchartView);
		delete m_pchartView;
		m_pchartView = NULL;
	}
	if (m_pHLayout)
	{
		delete m_pHLayout;
		m_pHLayout = NULL;
	}
}

void Defectlevelpiechart::clearUI()
{

}

void Defectlevelpiechart::updateUI(int dvdefectNgArr[MAX_DEFECT_SIZE], int dvdefectRiArr[MAX_DEFECT_SIZE], bool bDateData, int OkCnt, int RiCnt)
{
	if (m_pchartView != NULL)
	{
		m_pHLayout->removeWidget(m_pchartView);
		delete m_pchartView;
    }
	m_DrilldownChart = new DrilldownChart();
	m_DrilldownChart->setTheme(QChart::ChartThemeLight);
	m_DrilldownChart->setAnimationOptions(QChart::AllAnimations);
	m_DrilldownChart->legend()->setVisible(true);
	m_DrilldownChart->legend()->setAlignment(Qt::AlignRight);
	
	QPieSeries *yearSeries = new QPieSeries();

	int iTatolCnt = 0;
	int index = 0;
    for (int i = 1; i < m_defectNameVec.size(); i++)
	{
        if (m_defectEnableVec[i].second&&dvdefectNgArr[i]>0)
		{
			QString strName = "NG"+QString::fromStdString(m_defectNameVec[i]);
			//int iLeng = strName.length();
			//for (int i = 0;i<7- iLeng;i++)
			//{
			//	strName+= QString::fromStdString(" ");
			//}
			seriesArray[index] = new QPieSeries();
			seriesArray[index]->setName(strName);
			seriesArray[index]->append(new DrilldownSlice(dvdefectNgArr[i], strName, yearSeries));
            DrilldownSlice* pDrilldownSlice = new DrilldownSlice(seriesArray[index]->sum(), strName, seriesArray[index]);
            pDrilldownSlice->setBrush(m_colorsVec[i]);
			yearSeries->append(pDrilldownSlice);
			index++;
            iTatolCnt += dvdefectNgArr[i];
		}
	}
	for (int i = 1; i < m_defectNameVec.size(); i++)
	{
		if (m_defectEnableVec[i].second&&dvdefectRiArr[i] > 0)
		{
			QString strName =QString::fromStdString(m_defectNameVec[i]);
			//int iLeng = strName.length();
			//for (int i = 0; i < 7 - iLeng; i++)
			//{
			//	strName += QString::fromStdString(" ");
			//}
			seriesArray[index] = new QPieSeries();
			seriesArray[index]->setName(strName);
			seriesArray[index]->append(new DrilldownSlice(dvdefectRiArr[i], strName, yearSeries));
			DrilldownSlice* pDrilldownSlice = new DrilldownSlice(seriesArray[index]->sum(), strName, seriesArray[index]);
			pDrilldownSlice->setBrush(m_colorsVec[i]);
			yearSeries->append(pDrilldownSlice);
			index++;
			iTatolCnt += dvdefectRiArr[i];
		}
	}
	if (bDateData && m_defectNameVec.size() != 0)
	{
		// ri
		QString strName = QString::fromLocal8Bit("其他RI   ");
		seriesArray[index] = new QPieSeries();
		seriesArray[index]->setName(strName);
		seriesArray[index]->append(new DrilldownSlice(RiCnt, strName, yearSeries));
		DrilldownSlice* pDrilldownSlice = new DrilldownSlice(seriesArray[index]->sum(), strName, seriesArray[index]);
		pDrilldownSlice->setBrush(m_colorsVec[1]);
		yearSeries->append(pDrilldownSlice);
		//OK
		index++;
		strName = QString::fromLocal8Bit("OK     ");
		seriesArray[index] = new QPieSeries();
		seriesArray[index]->setName(strName);
		seriesArray[index]->append(new DrilldownSlice(OkCnt, strName, yearSeries));
		//
        pDrilldownSlice = new DrilldownSlice(seriesArray[index]->sum(), strName, seriesArray[index]);
        pDrilldownSlice->setBrush(m_colorsVec[0]);
		yearSeries->append(pDrilldownSlice);
		
		//
		//yearSeries->setName(QString::fromLocal8Bit("当前班次产品缺陷分布 OK ：") + QString::number(OkCnt) + QString::fromLocal8Bit(", NG ：") + QString::number(iTatolCnt - OkCnt));
		yearSeries->setName(QString::fromLocal8Bit("当前班次产品缺陷分布"));
	}
	else {
		yearSeries->setName(QString::fromLocal8Bit("当片缺陷分布 总数：") + QString::number(iTatolCnt));
	}

	m_DrilldownChart->changeSeries(yearSeries);

	m_pchartView = new QChartView(m_DrilldownChart);
	m_pchartView->setRenderHint(QPainter::Antialiasing);
	m_pchartView->setBackgroundBrush(QColor(199, 201, 229));
	m_pHLayout->addWidget(m_pchartView);
}

DrilldownChart::DrilldownChart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
	: QChart(QChart::ChartTypeCartesian, parent, wFlags),
	m_currentSeries(0)
{

}
DrilldownChart::~DrilldownChart()
{

}

void DrilldownChart::changeSeries(QAbstractSeries *series)
{
	// NOTE: if the series is owned by the chart it will be deleted
	// here the "window" owns the series...
	if (m_currentSeries)
		removeSeries(m_currentSeries);
	m_currentSeries = series;
	addSeries(series);
	setTitle(series->name());
}

void DrilldownChart::handleSliceClicked(QPieSlice *slice)
{
	DrilldownSlice *drilldownSlice = static_cast<DrilldownSlice *>(slice);
	changeSeries(drilldownSlice->drilldownSeries());
}
//-----///


DrilldownSlice::DrilldownSlice(qreal value, QString prefix, QAbstractSeries *drilldownSeries)
	: m_drilldownSeries(drilldownSeries),
	m_prefix(prefix)
{
	setValue(value);
	updateLabel();
	setLabelFont(QFont("Arial", 8));
	connect(this, &DrilldownSlice::percentageChanged, this, &DrilldownSlice::updateLabel);
	connect(this, &DrilldownSlice::hovered, this, &DrilldownSlice::showHighlight);
}
DrilldownSlice::~DrilldownSlice()
{

}

QAbstractSeries *DrilldownSlice::drilldownSeries() const
{
	return m_drilldownSeries;
}

void DrilldownSlice::updateLabel()
{
	QString label = m_prefix;
	label += " ";
	label += QString::number(this->value());
	label += ",";
	label += QString::number(this->percentage() * 100, 'f', 1);
	label += "%";
	setLabel(label);
}

void DrilldownSlice::showHighlight(bool show)
{
	setLabelVisible(show);
	setExploded(show);
}
//