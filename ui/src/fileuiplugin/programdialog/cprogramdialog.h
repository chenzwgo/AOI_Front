#ifndef CPROGRAMDIALOG_H
#define CPROGRAMDIALOG_H

#include <QDialog>
#include <QMap>
#include "fileuimanager.h"
#include "ui_newfiledlg.h"

namespace Ui {
class CProgramDialog;
}

class INotifyService;

class CProgramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CProgramDialog(QWidget *parent = 0);
    ~CProgramDialog();

    void  initProgramList(QMap<QString,StProgramInfo> mapProInfo, QString currentPro);
    bool  isModify();
    QMap<QString,StProgramInfo>  getMapProInfo();
protected slots:
    void showEvent(QShowEvent *e);
private  slots:
    void onCurrentRowChanged( int currentRow);
    //void onAddPro();
    void onDelPro();
    //void onBackupPro();
    void onRenamePro();
    //void onOK();
    //void onCanal();

private:
    void  initProList();
    void updateProInfo(QString strProName);
    bool qCopyDirectory(const QString& fromDir, const QString& toDir, bool bCoverIfFileExists);
    void writeProgramDesc( const QString &programName, const QString &programDesc );
private:
    Ui::CProgramDialog *ui;
    QDialog* m_RenameDlg;
    Ui::newfiledlg* m_RenameUI;
    QString m_strCurrentPro;
    QMap<QString,StProgramInfo> m_mapProInfo;
    INotifyService* m_pNotifyService; //发UI信号句柄
    bool      m_bIsModify;  //界面是否有改动
};

#endif // CPROGRAMDIALOG_H
