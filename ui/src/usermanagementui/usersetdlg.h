#ifndef USERSETDLG_H
#define USERSETDLG_H

#include <QDialog>
#include <QMenu>
#include <QAction>

namespace Ui {
class UserSetDlg;
}

class QTableWidgetItem;
class QAbstractButton;
class UserManagerCfg;
class UserSetDlg : public QDialog
{
    Q_OBJECT

public:
    static UserSetDlg* GetInstance(QWidget* pParent = NULL);
    static void               ReleaseInstance();
protected:
    virtual void keyPressEvent( QKeyEvent *e );
    virtual void closeEvent(QCloseEvent *event);
private:
    explicit UserSetDlg(QWidget *parent = 0);
    ~UserSetDlg();

    bool Init();
    bool InitTableValue(QString strModels);
    QString GetCurModel(QAbstractButton* pQAbstractButton = NULL);
    bool SaveUserInfo(QString strModel);
private slots:
    void on_tableWidget_itemChanged ( QTableWidgetItem * item );
    void on_tableWidget_customContextMenuRequested ( const QPoint & pos );
    void on_buttonGroup_buttonClicked ( QAbstractButton * button );
    void on_btnSet_clicked();
    void on_btnBack_clicked();
    void SlotAddUser();
    void SlotDelUser();
private:
    Ui::UserSetDlg *ui;
    static UserSetDlg* m_pSelfInstance;
    bool m_bChang;
    UserManagerCfg*          m_pUserManagerCfg;
    QAbstractButton*         m_pCurQAbstractButton;
    QMenu*                   m_pMenu;
    QAction*                 m_pAddAction;
    QAction*                 m_pDelAction;
};

#endif // USERSETDLG_H
