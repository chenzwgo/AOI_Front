#ifndef COMPETENCESETDLG_H
#define COMPETENCESETDLG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "usermanagercfg.h"

namespace Ui {
class CompetenceSetDlg;
}

class UserManagerCfg;
class QAbstractButton;
class CompetenceSetDlg : public QDialog
{
    Q_OBJECT
public:
    static CompetenceSetDlg* GetInstance(QWidget* pParent = NULL);
    static void               ReleaseInstance();
protected:
    virtual void keyPressEvent( QKeyEvent *e );
    virtual void closeEvent(QCloseEvent *event);
private:
    explicit CompetenceSetDlg(QWidget *parent = 0);
    ~CompetenceSetDlg();

    bool Init();
    bool InitTree(int iModel);
	bool InitTreeItem(int iModel, QTreeWidgetItem* pTreeItem, STWidgetInfo* pInfo, bool bTopItem=false);
    void SetTreeWidgetStyle();
    int GetCurModel(QAbstractButton* pQAbstractButton = NULL);
    bool SaveComp(int iModel);
	bool SaveWidgetInfo(int iModel, QTreeWidgetItem* pTreeWidget, STWidgetInfo* pInfo);
private slots:
    void on_treeWidget_itemChanged(QTreeWidgetItem * item, int column);
    void on_buttonGroup_buttonClicked ( QAbstractButton * button );
    void on_btnSet_clicked();
    void on_btnBack_clicked();
private:
    Ui::CompetenceSetDlg *ui;
    static CompetenceSetDlg* m_pSelfInstance;
    UserManagerCfg*          m_pUserManagerCfg;
    bool                     m_bChang;
    QAbstractButton*         m_pCurQAbstractButton;

};

#endif // COMPETENCESETDLG_H
