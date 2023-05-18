#ifndef UPHSTATISTICS_H
#define UPHSTATISTICS_H
#include "maindefine.h"
#include <QWidget>

namespace Ui {
class UphStatistics;
}

class UphStatistics : public QWidget
{
    Q_OBJECT

public:
	static UphStatistics* GetInstance(QWidget* pParant = NULL);
	static void ReleaseInstance();
    explicit UphStatistics(QWidget *parent = 0);
    ~UphStatistics();
	//
	bool updateUi();
	void CurrentChanged(QString ChangedDate, bool bDay = true);
	void dayOrnightClicked(bool bDay = true);
private:
	void updateDaystoChanged();//���µ�ǰ�ж���������ݿ�����ʾ
	bool getData();//��ȡѡ���������
	void updateshow();
    Ui::UphStatistics *ui;
	static UphStatistics* m_pSelfInstance;
	QStringList m_pListDay;
	QStringList m_pListNight;
	int m_iDay;
	int m_iAllDayVal[DATE_DATA_ALL];
	QString m_strDate;
};

#endif // UPHSTATISTICS_H
