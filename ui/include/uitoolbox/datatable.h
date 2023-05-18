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
	bool         enable;      //�Ƿ񱣴�
	QString      fileDirPath; //�ļ�·��
	unsigned int overdue;	  //��Чʱ��
};
struct DataFormat
{
	QString			displayTimeFormat;    //ʱ���ʽ
	QStringList     itemNames;			  //items
	SaveConfig      saveOption;
};

/************************************************************************/
/*             ÿһ�е����ݸ�ʽ                                         */
/************************************************************************/
struct DataItem
{
	QDateTime	dateTime;    
	QStringList dataStrList;
	bool		bPass; 
	bool		bSave;
};
/************************************************************************/
/* add by huanghucheng   ������ʾ�б��� 

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

	//���������ļ�·��
	void setXmlConfigPath(QString filePath);
	//���������� ���ַ�ʽ
	void addDataItem(DataItem dataItem);  //����DataItem�ṹ��
    void addDataItem(QStringList dataStrList , bool bPass = true,QDateTime dateTime = QDateTime::currentDateTime()); //ֱ�Ӵ�����,Ĭ��PASS,��ǰʱ�䣬��Ҫ����
	//�������������
	void setMaxRowCount(unsigned int maxCount);
	void setSaveFile(bool bSave);
	//���Ⱪ¶items' name list
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

	//����������
	bool saveItemTofile(QStringList strList);
};

#endif // DATALISTWIDGET_H
