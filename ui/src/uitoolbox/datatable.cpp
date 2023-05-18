#include "uitoolbox/datatable.h"
#include <QFile>
#include "uicommon/commonutils.h"
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
DataTable::DataTable(QWidget *parent) 
			  : QWidget(parent)
			  ,m_tableWidget(new QTableWidget())
			  ,m_maxRowCount(20)
						 
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(m_tableWidget);
    layout->setMargin(0);
	this->setLayout(layout);
	initTitle();
	connect(this, SIGNAL( addDataItemSignal(QStringList  , bool ,QDateTime  ) ),
		this, SLOT( addDataItemSlot(QStringList  , bool  ,QDateTime ) )
		);
}

DataTable::DataTable( QString filePath,unsigned int maxCount /*= 10*/,QWidget *parent /*= 0*/ )
				: QWidget(parent)
				,m_xmlFilePath(filePath)
				,m_maxRowCount(maxCount)
				,m_tableWidget(new QTableWidget(this))
				,m_bSaveFile(false)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(m_tableWidget);
    layout->setMargin(0);
	this->setLayout(layout);
	initTitle();
	connect(this, SIGNAL( addDataItemSignal(QStringList  , bool ,QDateTime  ) ),
		this, SLOT( addDataItemSlot(QStringList  , bool  ,QDateTime ) )
		);
}

DataTable::~DataTable()
{

}

bool DataTable::initTitle()
{
	if (!loadXmlConfig(m_xmlFilePath))
	{
		qDebug("ERROR:加载或解析xml文件失败，%s",m_xmlFilePath.toStdString().c_str());
		return false;
	}
	QStringList headers;
	headers.append(QString("Time"));
	headers += m_dataFormat.itemNames;
	headers.append(QString("NG/PASS"));
	//先设置大小，才能设置标题名字。。。
	m_tableWidget->setColumnCount(headers.size());
	m_tableWidget->setHorizontalHeaderLabels(headers);
	m_tableWidget->verticalHeader()->setVisible(false);
	m_tableWidget->horizontalHeader()->setStretchLastSection(true);
	m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    return true;
}
void DataTable::setXmlConfigPath( QString filePath )
{
    if (m_tableWidget == NULL)
    {
        return;
    }
    m_xmlFilePath = filePath;
	m_tableWidget->setColumnCount(0);
	m_tableWidget->setRowCount(0);
	initTitle();
}

void DataTable::setMaxRowCount( unsigned int maxCount )
{
    if (m_tableWidget == NULL)
    {
        return;
    }
    while (m_tableWidget->rowCount() > maxCount)
	{
		m_tableWidget->removeRow(m_tableWidget->rowCount()-1);
	}
	m_maxRowCount = maxCount;
}

bool DataTable::loadXmlConfig( QString strfile )
{
	QString strConfigFile = strfile;

	if (!QFile::exists (strfile))
	{
		strConfigFile = CommonUtils::getPluginLibraryPath () + "/plugins/robotui/" + strfile;
	}

	qDebug ("%s: %s: %s: called config file is %s", _TIME_, __FILE__, __FUNCTION__,qPrintable(strConfigFile));
	// Open the file
	QFile cfgFIle (strConfigFile);
	if (!QFile::exists (strConfigFile))
	{
		qDebug ("%s: %s: File %s does not exist", __FILE__, __FUNCTION__, qPrintable (strConfigFile));
		return false;
	}

	if (!cfgFIle.open (QFile::ReadOnly | QFile::Text))
	{
		qDebug ("%s: %s: Error open file:%s", __FILE__, __FUNCTION__, qPrintable (strConfigFile));
		strfile = "";
		return false;
	}

	QXmlStreamReader reader;
	reader.setDevice (&cfgFIle);

	// In read file, all content should be saved in memory
	//bool bRes = readFile (reader);

	qDebug ("%s: %s: %s: called", _TIME_, __FILE__, __FUNCTION__);
	QIODevice *pDevice = reader.device ();

	if (NULL == pDevice)
	{
		qDebug ("%s: %s: Stream reader not bind with device", __FILE__, __FUNCTION__);
		return false;
	}
	bool bRes = true;
	while (!reader.atEnd ())
	{
		if (reader.error ())
		{            
			qDebug() << "Error: " << qPrintable (reader.errorString ()) <<
				" at line " << reader.lineNumber () <<
				", column " << reader.columnNumber ();
			bRes = false;
			break;
		}
		bRes = readConfigInfo(reader);//解析位置信息
	}

	cfgFIle.close ();
	//m_strLoadFileName = strfile;
	return bRes;
}

bool DataTable::readConfigInfo( QXmlStreamReader& reader )
{
//	m_vecSpecInfo.clear();

	bool bRes = true;
	while (!reader.atEnd ())
	{

		if (reader.error ())
		{
			qDebug() << "Error: " << qPrintable (reader.errorString ()) <<
				" at line " << reader.lineNumber () <<
				", column " << reader.columnNumber ();
			bRes = false;
			break;
		}
		if (reader.isEndElement ())
		{
			reader.readNext ();
			if (reader.error ())
			{}
			break;
		}
		if (reader.isStartElement ())
		{
			if ("dataformat" !=  reader.name())
			{
				qDebug("ERROR:xml文件格式错误，不是dataformat文件：%s",qPrintable(m_xmlFilePath));
			}
			else {
				readDataFormat(reader);
			}
			
		}
		reader.readNext ();
	}

	return bRes;
}

QStringList DataTable::getDataFormat()
{
	return m_dataFormat.itemNames;
}

void DataTable::skipUnkownElement( QXmlStreamReader& reader )
{
	reader.readNext();
	while (!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}
		if (reader.isStartElement())
		{
			skipUnkownElement(reader);
		}
		else
		{
			reader.readNext();
		}
	}
}

void DataTable::readItemNames( QXmlStreamReader &reader )
{
	reader.readNext ();
	while (!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}
		if (reader.isStartElement())
		{
			if ("item" == reader.name())
			{
				m_dataFormat.itemNames.append(reader.attributes().value("name").toString());
				reader.readNext();
			}
			else
			{
				skipUnkownElement(reader);
			}
		}
	//5	else 
		{ 
			reader.readNext();
		}
	}
}

void DataTable::readSaveOption( QXmlStreamReader &reader )
{
	reader.readNext ();
	while (!reader.atEnd())
	{
		if (reader.isEndElement())
		{
			reader.readNext();
			break;
		}
		if (reader.isStartElement())
		{
			if ("enable" == reader.name())
			{
				QString strValue = reader.readElementText();
				QStringList trueStrs = QString("true,1").split(",",QString::SkipEmptyParts);
				if (trueStrs.contains(strValue,Qt::CaseInsensitive))
				{
					m_dataFormat.saveOption.enable = true;
				}
				else
				{
					m_dataFormat.saveOption.enable = FALSE;
				}
			}
			else if ("filedirpath" == reader.name())
			{
				m_dataFormat.saveOption.fileDirPath = reader.readElementText();
			}
			else if ("overdue" == reader.name())
			{
				bool bOk;
				m_dataFormat.saveOption.overdue = reader.readElementText().toInt(&bOk,10);
				if (!bOk)
				{
					m_dataFormat.saveOption.overdue = 30; 
					qDebug("读取文件保存时间参数错误，设为默认值30。");
				} 
			}
			else
			{
				skipUnkownElement(reader);
			}	
		}
		reader.readNext();
	}
}

void DataTable::readDataFormat( QXmlStreamReader &reader )
{
	reader.readNext();
	while (!reader.atEnd ())
	{
		if (reader.isEndElement ())
		{
			reader.readNext ();
			if (reader.error ())
			{}
			break;
		}
		if (reader.isStartElement ())
		{
			if ("displaytimeformat" == reader.name ())
			{
				m_dataFormat.displayTimeFormat = reader.readElementText();
			}
			else if ("dataitems" == reader.name ())
			{
				readItemNames(reader);
			}
			else if ("savafile" == reader.name ())
			{
				readSaveOption(reader);
			}
			else{ skipUnkownElement(reader);
			}
		}
		reader.readNext ();
	}
}

void DataTable::addDataItem(QStringList dataStrList, bool bPass ,QDateTime dateTime )
{
	emit addDataItemSignal(dataStrList,bPass,dateTime);
}

void DataTable::addDataItem( DataItem dataItem )
{
	addDataItem(dataItem.dataStrList,dataItem.bPass,dataItem.dateTime);
}

bool DataTable::saveItemTofile( QStringList strList )
{
	QString strCsvFile; 
	QDateTime nowDateTime = QDateTime::currentDateTime();



	if (!CommonUtils::CreateDir(m_dataFormat.saveOption.fileDirPath))
	{
		qDebug()<<"ERROR: CreateDir"<< m_dataFormat.saveOption.fileDirPath << "failed!";
		return false;
	}
	//删除过期文件
	static QDateTime lastDelDate = nowDateTime;
	if (lastDelDate.secsTo(nowDateTime) >= 60*60*24 )
	{
		QDir dir(m_dataFormat.saveOption.fileDirPath);
		dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
		QFileInfoList fileList = dir.entryInfoList();
		foreach (QFileInfo fi, fileList)
		{
			if (nowDateTime.daysTo(fi.lastModified()) >= m_dataFormat.saveOption.overdue ) 
			{
				fi.dir().remove(fi.fileName());
				qDebug() << "..............删除过期文件: " << fi.fileName() << "最后修改时间："<< fi.lastModified();
			}
		}
		lastDelDate = nowDateTime;
	}
	
	strCsvFile = m_dataFormat.saveOption.fileDirPath + "/" + nowDateTime.toString("yyyy-MM-dd") + ".csv";

	QFile csvFile(strCsvFile);
	if (!csvFile.open(QIODevice::Append)) 
	{
		qDebug() << "Error: Open " << strCsvFile << "failed!";
		return false;
	}
	csvFile.write(strList.join(",").toStdString().c_str());
	csvFile.close();
	return true;
}

void DataTable::setSaveFile( bool bSave )
{
	m_bSaveFile = bSave;
}

void DataTable::addDataItemSlot( QStringList dataStrList , bool bPass /*= TRUE*/,QDateTime dateTime /*= QDateTime::currentDateTime()*/ )
{
	if (m_tableWidget == NULL)
	{
        return;
	}
    if (m_tableWidget->rowCount() >= m_maxRowCount)
	{
		m_tableWidget->removeRow(m_tableWidget->rowCount()-1);
	}
	int row = 0;  
	m_tableWidget->insertRow(row);
	m_tableWidget->setItem(row,0,new QTableWidgetItem(dateTime.toString(m_dataFormat.displayTimeFormat)));
	int col = 1;
	for (; col <= dataStrList.size(); col++)
	{
		m_tableWidget->setItem(row,col,new QTableWidgetItem(dataStrList.at(col-1)));
	}
	QLabel* label = NULL;
	if (bPass)
	{
		label = new QLabel("<h3><font color=Green>PASS<font></h3>");
	}
	else
	{
		label = new QLabel("<h3><font color=Red>NG<font></h3>");
	}
	m_tableWidget->setCellWidget(row,col,label);
    m_tableWidget->scrollToTop();
	if (m_bSaveFile)
	{
		QStringList strSavelist;
		strSavelist.append(dateTime.toString(m_dataFormat.displayTimeFormat));
		strSavelist += dataStrList;
		if(bPass == true)  strSavelist += "PASS\n"; else strSavelist += "NG\n" ;
		saveItemTofile(strSavelist);
	}
}


