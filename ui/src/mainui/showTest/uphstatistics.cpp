#include "uphstatistics.h"
#include "ui_uphstatistics.h"
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QFileDialog>
#include "commonapi.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "g12common.h"

UphStatistics* UphStatistics::m_pSelfInstance = NULL;

UphStatistics* UphStatistics::GetInstance(QWidget* pParant /*= NULL*/)
{
	if (NULL == m_pSelfInstance)
	{
		m_pSelfInstance = new UphStatistics(pParant);
	}
	return m_pSelfInstance;
}

void UphStatistics::ReleaseInstance()
{
	if (NULL != m_pSelfInstance)
	{
		delete m_pSelfInstance;
		m_pSelfInstance = NULL;
	}
}
UphStatistics::UphStatistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UphStatistics)
{
	ui->setupUi(this);
	setWindowFlags(Qt::Dialog);
	m_iDay = 0;
	memset(m_iAllDayVal, 0, DATE_DATA_ALL*sizeof(int));
	//for (int i = 0; i < DATE_TIMES; i++)
	//{
	//	m_pListDay.push_back(g_timeFlag[i] + QString::fromLocal8Bit("点 "));
	//	m_pListNight.push_back(g_timeFlag[i+DATE_TIMES] + QString::fromLocal8Bit("点 "));
	//}

	
	//JTime curTime = JTime::CurrentLocalDateTime();

	m_strDate = getCurrentProducsTimeStrPath();
	updateUi();
   
}

UphStatistics::~UphStatistics()
{
   // delete ui;
}
void UphStatistics::updateDaystoChanged()
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
		int dateMax = number.count()>3?3: number.count();
		
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
		ui->comboBox_day->setCurrentIndex(ui->comboBox_day->count()-1);
	}
	else {
		ui->comboBox_day->setCurrentIndex(lastDate_index);
	}
	delete NewQir;
	NewQir = NULL;
	*/
}

void UphStatistics::CurrentChanged(QString ChangedDate, bool bDay )
{
	m_strDate = ChangedDate;
	m_iDay = bDay ? 0 : 1;
	updateUi();
}
void UphStatistics:: dayOrnightClicked(bool bDay )
{
	m_iDay = bDay ? 0 : 1;
	updateUi();
}

#include "iniFile.h"
bool UphStatistics::getData()
{
	memset(m_iAllDayVal, 0, DATE_DATA_ALL * sizeof(int));//清原来数据
	QString strDate = m_strDate;
	CIniFile iniFile;
	// 路径不存在，创建路径
	QString strDir = getSaveResultFilePath();
	QDir dir;
	if (!dir.exists(strDir))
	{
		dir.mkdir(strDir);
		return false;
	}
	QString strSavePath = strDir +"/"+strDate;

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
	m_iAllDayVal[0] = iniFile.GetValueI(keyname.toStdString(), "TotalCnt");
	for (int i = 1 ;i <DATE_DATA_ALL; i++)
	{
		QString key = keyname + "_"+ QString::number(i);
		m_iAllDayVal[i] = iniFile.GetValueI(key.toStdString(), "TotalCnt");
	}
	return true;
}
void UphStatistics::updateshow()
{
	int ival[DATE_TIMES] = {0};
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	int iSartTime = 8;
	if (pFile)
	{
		iSartTime = pFile->getValue(WORK_MODE_AM_STRAT_TIME).toInt(8);;//
	}
	for (int i = 0; i < DATE_TIMES; i++)
	{
		if (0 == m_iDay)// 转班点 DAY_BY_TIME	 8   //转班时间
		{
			ival[i] = m_iAllDayVal[iSartTime +i];// 
		}
		else {

			int itime = iSartTime + DATE_TIMES + i; //转晚班点//比如8点对点转 则 8+12 = 20

			if (itime > DATE_TIMES * 2)//大于24点
			{
				itime -= DATE_TIMES * 2;
			}
			ival[i] = m_iAllDayVal[itime];// 
		}
		//
	}
	QString str = m_strDate + QString::fromLocal8Bit(szDay[m_iDay]);
	ui->widget_uph->updateUI_UPH_Day(str, ival, m_iDay, DATE_TIMES);
	
}
bool UphStatistics::updateUi()
{
	getData();//更新数据//
	updateshow();;
	return true;
}