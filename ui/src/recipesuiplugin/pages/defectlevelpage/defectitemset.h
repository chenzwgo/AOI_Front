#ifndef DEFECTITEM_SET_H
#define DEFECTITEM_SET_H

#include <QWidget>
#include "defect/idefectqualification.h"
#include "defectvalue.h"
#include "QCheckBox"
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>



class ScriptLexer;
class QVBoxLayout;
class QTreeWidget;
class QTreeWidgetItem;

class DefectItemSet : public QWidget
{
    Q_OBJECT

public:
    explicit DefectItemSet(int iStation,string strChannel,vector<defectlevel>* VecDefect, int iRegion = 1,QWidget *parent = 0);
    ~DefectItemSet();
	void UiCancel();//ȡ��������ui��ԭ����
	vector<defectlevel>* getUiVale();

public slots:
	void slotActAdd();    //���
	void slotActDelete(); //ɾ��
	void slotActCopy();   //����
	void slotActPaste();  //ճ��
	void slotActExec();  // ִ��
	void slotActFullExp();  // ����ָ��
	void slotActLevelExec();  // ִ��
	void popMenu(const QPoint& pot);
	void slotitemDoubleClicked(QTreeWidgetItem* item, int column);
	void slotitemSelectionChanged();
private:
	void InitUi();
	bool execSingle(QString strExpr, itemValue& value);

	vector<QCheckBox*>  m_VecCheckBox;
    vector<defectvalue*>   m_VecWigetItem;
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
    defectvalue* m_ptypeitem;
	IDefectQualification *m_pIDefectQualification;
	string m_strDefectName;
	QVBoxLayout *m_pverticalLayout;
	QTreeWidget *m_ptreeWidget;
	QTreeWidgetItem *m_pCurrItem;
	int m_iRegion;
	string m_strChannel;
	int m_iStation;
	vector<string> m_defectNameVec;
};

#endif // DEFECTITEM_SET_H
