#ifndef RUNFLOWDLG_H
#define RUNFLOWDLG_H

#include <QDialog>
#include "rtdefine.h"
#include "errcode_def.h"
#include <QTimer>

#ifdef UI_CUSTOMWIDGET
#define RT_API Q_DECL_EXPORT
#else
#define RT_API Q_DECL_IMPORT
#endif


namespace Ui {
class runFlowDlg;
}

class IMainFlow;
class RT_API RunFlowDlg : public QDialog
{
    Q_OBJECT

public:
    explicit RunFlowDlg(QWidget *parent = 0);
    ~RunFlowDlg();

    ERROR_CODE runFlow(const QString &group, const QString &flowName, const QString &beginStep="", const QString &endStep="");

protected:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent *);

private slots:
    void on_toolButton_pause_clicked();

    void on_toolButton_continue_clicked();

    void on_toolButton_stop_clicked();

    void on_timeout();

private:
    Ui::runFlowDlg *ui;
    IMainFlow *m_pMainFow;  //流程
    QTimer *m_pTimer;      //定时器
    QString m_flowname;
};

#endif // RUNFLOWDLG_H
