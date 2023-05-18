#ifndef FILTER_H
#define FILTER_H

#include <QWidget>
#include <QDialog>
#include <QTableWidget>
#include <map>
#include "filterset.h"
#include "defect/idefectqualification.h"
namespace Ui {
class filter;
}
class ExtendStackWidget;
class FilterSet;
class DefectQualification;
class VarExpressionWidget;
class QTableWidgetItem;
class filter : public QDialog
{
    Q_OBJECT

public:
	explicit filter(QDialog *parent = 0);
	~filter();
private slots:
	void on_pushButton_save_clicked();  // 
	void on_pushButton_modify_clicked();  // 
private:
	void setUisetEnabled(bool bEnabled);
	void getenableCfg();

	void InitUi();
    Ui::filter *ui;
	ExtendStackWidget *m_pExtendStackWidget;
	vector<QWidget*> m_vecFilterSet;
    //bool defectTypeEnableArr[DEFECT_TYPE_SIZE];
    vector<pair<string, bool>> m_defectTypeEnableVec;
	map<string, bool> m_mapDefectEnable;
	VarExpressionWidget* m_pVarExpressionWidget;
	vector<string> m_defectNameVec;
    vector<string> m_defectEnNameVec;
};

class VarExpressionWidget :public QWidget
{
	Q_OBJECT
public:
	explicit VarExpressionWidget(QWidget *parent = 0);
	~VarExpressionWidget();
	void initTableWidget();
public:
	void UiCancel();
	void getUiVale();
private slots:
	void openEditor(QTableWidgetItem* item);
	void closeEditor();
private:
	QTableWidget * m_pExpressionTableWidget;
	QTableWidgetItem* m_pitem;
	QString m_itemName;
};
#endif // FILTER_H
