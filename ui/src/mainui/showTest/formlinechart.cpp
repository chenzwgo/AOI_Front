#include "formlinechart.h"
#include "ui_formlinechart.h"
#include <QFileDialog>
#include "iniFile.h"
#include "commonapi.h"
#include "g12common.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
FormLineChart* FormLineChart::m_pSelfInstance = NULL;

FormLineChart* FormLineChart::GetInstance(QWidget* pParant /*= NULL*/)
{
	if (NULL == m_pSelfInstance)
	{
		m_pSelfInstance = new FormLineChart(pParant);
	}
	return m_pSelfInstance;
}

void FormLineChart::ReleaseInstance()
{
	if (NULL != m_pSelfInstance)
	{
		delete m_pSelfInstance;
		m_pSelfInstance = NULL;
	}
}
FormLineChart::FormLineChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormLineChart)
{
	m_pchartView =NULL ;
	m_stUPHCfg = NULL;
	memset(m_dVal, 0.0, sizeof(double)*DATE_DATA_ALL);
	m_iDay = 0;
    ui->setupUi(this);
	//setWindowFlags(Qt::Dialog);// set move and close crl;
	//updateDaystoChanged();
	//JTime curTime = JTime::CurrentLocalDateTime();

	m_strDate = getCurrentProducsTimeStrPath();
}

FormLineChart::~FormLineChart()
{
    //delete ui;
}

bool FormLineChart::updateUi()
{
	getData();//更新数据//
	updateshow();;
	return true;
}

void FormLineChart::CurrentChanged(QString ChangedDate, bool bDay )
{
	m_strDate = ChangedDate;
	m_iDay = bDay ? 0 : 1;
	updateUi();
}
void FormLineChart::dayOrnightClicked(bool bDay )
{
	m_iDay = bDay ? 0 : 1;
	updateUi();
}

bool FormLineChart::getData()
{
	memset(m_dVal, 0.0, sizeof(double)*DATE_DATA_ALL);
	QString strDate = m_strDate;
	CIniFile iniFile;

	// 路径不存在，创建路径
	QString strDir = getSaveResultFilePath(); + "/ProductResult";
	QDir dir;
	if (!dir.exists(strDir))
	{
		dir.mkdir(strDir);
		return false;
	}
	QString strSavePath = strDir + "/" + strDate;

	QDir dir2(strSavePath);
	if (!dir2.exists(strSavePath))
	{
		return false;
	}

	QString csvFileName = strSavePath + "/" + strDate + "_UPH.ini";

	if (!QFile::exists(csvFileName))
	{
		return false;
	}
	iniFile.SetPath(csvFileName.toStdString());
	iniFile.ReadFile();
	QString keyname = UPHINFO;
	double dToCl = iniFile.GetValueI(keyname.toStdString(), "TotalCnt");
	double dOkCl = iniFile.GetValueI(keyname.toStdString(), "OKCnt") + iniFile.GetValueI(keyname.toStdString(), "ReCheck");
	dToCl = dToCl > 0 ? dToCl : 1;
	m_dVal[0] = dOkCl/dToCl;
	for (int i = 1; i < DATE_DATA_ALL; i++)
	{
		QString key = keyname +"_"+ QString::number(i);
		dToCl = iniFile.GetValueI(key.toStdString(), "TotalCnt");
		dOkCl = iniFile.GetValueI(key.toStdString(), "OKCnt") + iniFile.GetValueI(key.toStdString(), "ReCheck");
		dToCl = dToCl > 0 ? dToCl : 1;
		m_dVal[i] = dOkCl / dToCl;
	}
	return true;
}

void FormLineChart::updateDaystoChanged()
{
	/*
	QApplication::applicationDirPath();
	QString strDir = QApplication::applicationDirPath() + "/ProductResult";
	QDir *NewQir = new QDir(strDir);
	NewQir->setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
	if (!NewQir->exists(strDir))
	{
		return;
	}
	QStringList dateNames = NewQir->entryList();
	int imax = 0;
	QString lastDate;
	int ival[3] = { 10000,100,1 };
	int index = 0;
	int lastDate_index = 0;
	foreach(QString date, dateNames)
	{
		QStringList number = date.split("-");
		int idata = 0;
		int dateMax = number.count()>3 ? 3 : number.count();

		for (int i = 0; i < number.count(); i++) //因为文件获取的数据排序是乱的，所以需要把最新的日期找出来做默认显示
		{
			idata += number.at(i).toInt() * ival[i];
		}
		if (imax <= idata) //
		{
			imax = idata;
			lastDate = date;
			lastDate_index = index;
		}
		ui->comboBox_day->addItem(date);
		index++;
	}
	JTime curTime = JTime::CurrentLocalDateTime();
	QString datePath = QString("%1-%2-%3").arg(curTime.year()).arg(curTime.month()).arg(curTime.day());
	dateNames.clear();
	if (lastDate != datePath) //当前目录尚未创建
	{
		ui->comboBox_day->addItem(datePath);
		ui->comboBox_day->setCurrentIndex(ui->comboBox_day->count() - 1);
	}
	else {
		ui->comboBox_day->setCurrentIndex(lastDate_index);
	}
	delete NewQir;
	NewQir = NULL;
	*/
}
void FormLineChart::updateshow()
{
	double dval[DATE_TIMES] = {0};
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	int itimestart = 8;
	if (pFile)
	{
		itimestart = pFile->getValue(WORK_MODE_AM_STRAT_TIME).toInt(8);//
	}
	for (int i = 0; i < DATE_TIMES; i++)
	{
		double ival = 0;
		int itime = 1;
		if (0==m_iDay)
		{
			itime = i + itimestart;//点数开始 
		}
		else {

			itime = i + itimestart + DATE_TIMES;//
			itime = (itime > DATE_TIMES * 2) ? (itime - DATE_TIMES * 2) : itime;

		}
		ival = m_dVal[itime] * 1000;

		dval[i] = ival / 10.0;
	}
	QString str = m_strDate + QString::fromLocal8Bit(szDay[m_iDay]);
	ui->widget_ui->updateUI_Yield(str, dval, m_iDay, DATE_TIMES);
}