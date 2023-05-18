#ifndef NETLIGHTCOMMUNICATIONDLG_H
#define NETLIGHTCOMMUNICATIONDLG_H

#include <QWidget>
#include "abstractpagewidget.h"
namespace Ui {
class NetLightCommunicationDlg;
}

class lightcontrol_new;
class NetLightCommunicationDlg : public AbstractPageWidget
{
    Q_OBJECT

public:
    explicit NetLightCommunicationDlg(QWidget *parent = 0);
    ~NetLightCommunicationDlg();
	void  onEnable(bool enable);
	void  onSave();
	void  onCancel();
	void  onUpdate();
	virtual void updateProductName(const QString& strProductName);
public slots:
	void on_pushBtnIp_ok_clicked();
	void on_pushBtnIp_cancel_clicked();
	void on_pushButton_getCtrType_crl_clicked();
	void on_pushButton_send_st_clicked();
	void on_pushButton_send_order_clicked();
private:
    Ui::NetLightCommunicationDlg *ui;
	QString m_strProductName;
	lightcontrol_new*  m_plightcontrol_new;
};

#endif // NETLIGHTCOMMUNICATIONDLG_H
