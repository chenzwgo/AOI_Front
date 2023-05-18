#ifndef FILTER_H
#define FILTER_H

#include <QWidget>
#include <QDialog>
#include <QTableWidget>
#include <map>
#include "defectitemset.h"
#include "defect/idefectqualification.h"
#include "../abstractpagewidget.h"
namespace Ui {
class defectlevelpage;
}
class ExtendStackWidget;
class FilterSet;
class DefectQualification;
class VarExpressionWidget;
class QTableWidgetItem;

class defectlevelpage : public AbstractPageWidget
{
    Q_OBJECT
public:
    explicit defectlevelpage(int iStation,QString Channel,QWidget *parent = 0);
	~defectlevelpage();
    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();
protected:
    virtual void showEvent(QShowEvent *event);
private slots:
	void on_pushButton_save_clicked();  // 
	void on_pushButton_modify_clicked();  // 
private:
	void setUisetEnabled(bool bEnabled);
	void getenableCfg();

	void InitUi();
    Ui::defectlevelpage *ui;
	ExtendStackWidget *m_pExtendStackWidget;
	vector<QWidget*> m_vecFilterSet;
    //bool defectTypeEnableArr[DEFECT_TYPE_SIZE];
    vector<bool> m_defectTypeEnableVec;
	map<string, bool> m_mapDefectEnable;
	VarExpressionWidget* m_pVarExpressionWidget;
    QString m_strChannel;
	bool m_bUIInit;
	int m_iStation;
	vector<string> m_defectNameVec;
	vector<string> m_defectEnNameVec;
	vector<pair<string,bool>> m_defectEnableVec;
};

class VarExpressionWidget :public QWidget
{
	Q_OBJECT
public:
	explicit VarExpressionWidget(int iStation, QString Channel, QWidget *parent = 0);
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
	QString m_strChannel;
	int m_iStation;

};
#endif // FILTER_H
