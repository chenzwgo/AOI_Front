#ifndef PCCOMMUNICATIONDLG_H
#define PCCOMMUNICATIONDLG_H

#include <QWidget>
#include "abstractpagewidget.h"
namespace Ui {
class PcCommunicationDlg;
}

class PcCommunicationDlg : public AbstractPageWidget
{
    Q_OBJECT

public:
    explicit PcCommunicationDlg(QWidget *parent = 0);
    ~PcCommunicationDlg();
	void  onEnable(bool enable);
	void  onSave();
	void  onCancel();
	void  onUpdate();
	virtual void updateProductName(const QString& strProductName);
public slots:

    void on_pushButton_backpc_OK_clicked();
	void on_pushButton_backpc_Cancel_clicked();

	void on_pushButton_show_netdevui_clicked();
	void on_pushButton_syn_sn_clicked();
	void on_pushButton_query_sn_clicked();
	void on_pushButton_query_status_clicked();
private:
    Ui::PcCommunicationDlg *ui;
	QString m_strProductName;
};

#endif // PCCOMMUNICATIONDLG_H
