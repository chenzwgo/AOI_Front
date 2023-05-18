#ifndef TYPEITEM_H
#define TYPEITEM_H

#include <QDialog>
#include "defect/idefectqualification.h"
namespace Ui {
class typeitem;
}

struct itemValue
{	
	itemValue()
	{
		dArea = 0.0;
		dScore = 0.0;
		dContrast = 0.0;
		dWidth = 0.0;
		dHeight = 0.0;
		dSkeletonLength = 0.0;
		dPosX = 0.0;
		dPosY = 0.0;
		iSvm = 0;
	}

	double dArea;
	double dScore;
	double dContrast ;
	double dWidth ;
	double dHeight ;
	double dSkeletonLength ;
	double dPosX;
	double dPosY ;
	int    iSvm;
};

class ScriptLexer;
class typeitem : public QDialog
{
    Q_OBJECT

public:
    explicit typeitem(itemValue* pitemValue, QDialog *parent = 0);
    ~typeitem();

private:
    Ui::typeitem *ui;
	itemValue* m_pItemValue;
private slots:
	void on_pushButton_ok_clicked();
	void on_pushButton_cancel_clicked();
};

#endif // TYPEITEM_H
