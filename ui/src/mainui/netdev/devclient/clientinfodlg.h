#ifndef CLIENTINFODLG_H
#define CLIENTINFODLG_H

#include <QDialog>
#include "socketcommon.h"
namespace Ui {
class ClientInfoDlg;
}

class ClientInfoDlg : public QDialog
{
    Q_OBJECT
public:
    enum OperatorModel{Create, Alter};
    explicit ClientInfoDlg(OperatorModel enModel, QWidget *parent = 0);
    ~ClientInfoDlg();

    clientCfg GetUiData();
    void SetUiData(const clientCfg& stCfg);
private slots:
    void on_pushButton_ok_clicked();
private:
    Ui::ClientInfoDlg   *ui;
    const OperatorModel  m_enModel;
    clientCfg        m_stSrcCfg;
};

#endif // CLIENTINFODLG_H
