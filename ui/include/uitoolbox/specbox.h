#ifndef SPECSETTING_H
#define SPECSETTING_H

#include <QtGui/QtGui>
#include <vector>
#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include "dllexport.h"
using namespace std;
#define UI_SAFE_DELETE(_X_) if (_X_) {delete (_X_); (_X_) = NULL; }
struct specUiItem
{
	int    row;
	QLabel *name;
	QLineEdit  *minEdit;
	QLineEdit  *maxEdit;
	QPushButton *changeBtn;
	specUiItem()
	{
		name = NULL;
		minEdit  = NULL;
		maxEdit  = NULL;
		changeBtn = NULL;
	}
	~specUiItem()
	{
		UI_SAFE_DELETE(name);
		UI_SAFE_DELETE(minEdit);
		UI_SAFE_DELETE(maxEdit);
		UI_SAFE_DELETE(changeBtn);
	}
};
struct specInfoItem
{
	QString name;
	double  minValue;
	double  maxValue;
	specUiItem* specItemInfo;
	specInfoItem()
	{
		specItemInfo = NULL;
	}
	~specInfoItem()
	{
		UI_SAFE_DELETE(specItemInfo);
	}
};
class RT_API SpecBox : public QGroupBox
{
	Q_OBJECT

public:
	SpecBox(QWidget *parent = 0);
	SpecBox(QString title,QString specConfigFilePath ,bool bModifyEnable = false,QWidget *parent = 0);

	~SpecBox();

	void setSpecFilePath(QString specConfigFilePath);   //设置xml路径
	double getMinSpecByName(QString name); 
	double getMaxSpecByName(QString name); 

	bool updateUi();  //更新数据到UI，会重新读取配置文件

	void setModifyEnable(bool state);  //是否可以更改
public slots:
	bool changeSpec(); //
private:
	bool initUi();
	bool LoadSpecConfig(QString strFile);
	bool ReadSpecInformation(QXmlStreamReader& reader);//add by huanghucheng
	
	vector<specInfoItem> m_vecSpecInfo;
	QGridLayout *m_gridLayout;
	QString      m_specConfigFilePath;
	bool         m_bModifyEnable;
};

#endif // SPECSETTING_H
