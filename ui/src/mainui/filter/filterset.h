#ifndef FILTERSET_H
#define FILTERSET_H

#include <QWidget>
#include "defect/idefectqualification.h"
#include "typeitem.h"
#include "QCheckBox"
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>



class ScriptLexer;
class QVBoxLayout;
class QTreeWidget;
class QTreeWidgetItem;

class FilterSet : public QWidget
{
    Q_OBJECT

public:
    explicit FilterSet(vector<defectlevel>* VecDefect, int iRegion = 1,QWidget *parent = 0);
    ~FilterSet();
	void UiCancel();//取消操作。ui还原数据
	vector<defectlevel>* getUiVale();

public slots:
	void slotActAdd();    //添加
	void slotActDelete(); //删除
	void slotActCopy();   //复制
	void slotActPaste();  //粘贴
	void slotActExec();  // 执行
	void slotActFullExp();  // 完整指令
	void slotActLevelExec();  // 执行
	void popMenu(const QPoint& pot);
	void slotitemDoubleClicked(QTreeWidgetItem* item, int column);
	void slotitemSelectionChanged();
private:
	void InitUi();
	bool execSingle(QString strExpr, itemValue& value);

	vector<QCheckBox*>  m_VecCheckBox;
	vector<typeitem*>   m_VecWigetItem;
	vector<defectlevel>* m_VecCurrData;
	QMenu*   m_pContextMenu;
	QAction* m_pActAdd;
	QAction* m_pActDelete;
	QAction* m_pActCopy;
	QAction* m_pActPaste;
	QAction* m_pActExec;	
	QAction* m_pActFullExp;
	QAction* m_pActLevelExec;
	ScriptLexer* m_pScriptLevel;
	typeitem* m_ptypeitem;
	IDefectQualification *m_pIDefectQualification;
	string m_strDefectName;
	QVBoxLayout *m_pverticalLayout;
	QTreeWidget *m_ptreeWidget;
	QTreeWidgetItem *m_pCurrItem;
	int m_iRegion;
};

#endif // FILTERSET_H
