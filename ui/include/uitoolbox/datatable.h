#ifndef DATALISTWIDGET_H
#define DATALISTWIDGET_H
//add by huanghucheng
#include <QList>
#include <QWidget>
#include <QLayout>
#include <QTableWidget>
#include <QXmlStreamReader>
#include "dllexport.h"
#include <QDateTime>

struct SaveConfig
{
	bool         enable;      //是否保存
	QString      fileDirPath; //文件路径
	unsigned int overdue;	  //有效时间
};
struct DataFormat
{
	QString			displayTimeFormat;    //时间格式
	QStringList     itemNames;			  //items
	SaveConfig      saveOption;
};

/************************************************************************/
/*             每一行的数据格式                                         */
/************************************************************************/
struct DataItem
{
	QDateTime	dateTime;    
	QStringList dataStrList;
	bool		bPass; 
	bool		bSave;
};
/************************************************************************/
/* add by huanghucheng   数据显示列表类 

QHBoxLayout *dataTablelayout = new QHBoxLayout(this);
DataTable* dataTable = new DataTable("E:\\code\\runtime\\bin\\win\\debug\\config\\datasavewidget.xml");
dataTablelayout->addWidget(dataTable);
dataTable->addDataItem(QString("1,1,2,3").split(","));
/************************************************************************/
class RT_API DataTable : public QWidget
{
	Q_OBJECT
public:
    DataTable(QWidget *parent = 0);
	DataTable(QString filePath,unsigned int maxCount = 20,QWidget *parent = 0);
    ~DataTable();

	//设置配置文件路径
	void setXmlConfigPath(QString filePath);
	//加入行数据 两种方式
	void addDataItem(DataItem dataItem);  //利用DataItem结构体
    void addDataItem(QStringList dataStrList , bool bPass = true,QDateTime dateTime = QDateTime::currentDateTime()); //直接传数据,默认PASS,当前时间，需要保存
	//设置最大行数量
	void setMaxRowCount(unsigned int maxCount);
	void setSaveFile(bool bSave);
	//对外暴露items' name list
	QStringList getDataFormat();
signals:
    void addDataItemSignal(QStringList dataStrList , bool bPass = true,QDateTime dateTime = QDateTime::currentDateTime());
public slots:
	void addDataItemSlot(QStringList dataStrList , bool bPass ,QDateTime dateTime );
private: 
	 QString		m_xmlFilePath;
	 unsigned int   m_maxRowCount;
	 QTableWidget*    m_tableWidget; 
	 DataFormat     m_dataFormat;
	 bool m_bSaveFile;
private:
	bool initTitle();
	bool loadXmlConfig(QString strFile);
	bool readConfigInfo(QXmlStreamReader& reader);

	void readDataFormat( QXmlStreamReader &reader );

	void readItemNames( QXmlStreamReader &reader );
	void readSaveOption(QXmlStreamReader &reader);
	void skipUnkownElement(QXmlStreamReader& reader);

	//保存行数据
	bool saveItemTofile(QStringList strList);
};

#endif // DATALISTWIDGET_H
