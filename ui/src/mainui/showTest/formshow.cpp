#include "formshow.h"
#include "ui_formshow.h"
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"
#include "callsdk.h"
FormShow::FormShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormShow)
{
	m_pchartView = NULL;
	m_pHLayout = NULL;
	m_DrilldownChart = NULL;

	m_pHLayout = new QHBoxLayout(this);
	m_pHLayout->setMargin(0);
	setLayout(m_pHLayout);
    ui->setupUi(this);

	IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p2)
	{
		m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
	}
}

FormShow::~FormShow()
{
    delete ui;
}
#include <QtCore/QTime>
void FormShow::updateData(QString strDate, int *idata, QStringList daytimelist, int ileng)
{
	
	if (NULL == idata)
	{
		return;
	}
	if (NULL != m_pchartView)
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
	yearSeries->setName(strDate);
	int index = 0;
	for (int i = 0; i < daytimelist.count(); i++)
	{
		QPieSeries *pie  = new QPieSeries();
		pie->setName(QString::fromStdString(m_defectNameVec[i]));
		pie->append(new DrilldownSlice(idata[i], daytimelist.at(i), yearSeries));
		DrilldownSlice* pDrilldownSlice = new DrilldownSlice(pie->sum(), daytimelist.at(i), pie);
		yearSeries->append(pDrilldownSlice);
	}

	m_DrilldownChart->changeSeries(yearSeries);
	//![2]

	//![3]
	m_pchartView = new QChartView(m_DrilldownChart);
	m_pchartView->setRenderHint(QPainter::Antialiasing);
	m_pchartView->setBackgroundBrush(QColor(199, 201, 229));
	m_pHLayout->addWidget(m_pchartView);
	//
}
