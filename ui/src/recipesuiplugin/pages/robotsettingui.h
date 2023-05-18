#ifndef ROBOTSETTINGUI_H
#define ROBOTSETTINGUI_H

#include <QWidget>
#include "abstractpagewidget.h"

namespace Ui {
class RobotSettingUi;
}
class ICom;
class RobotSettingUi : public AbstractPageWidget
{
    Q_OBJECT

public:
    explicit RobotSettingUi(QWidget *parent = 0);
    ~RobotSettingUi();
	void  onEnable(bool enable);
	void  onSave();
	void  onCancel();
	void  onUpdate();
	virtual void updateProductName(const QString& strProductName);

private slots:
    void on_pushButton_RI_clicked();

    void on_pushButton_NG_clicked();

    void on_pushButton_OK_clicked();

	void on_pushButton_send_clicked();

	void on_pushButton_DisConnect_clicked();

	void on_pushButton_ReConnect_clicked();

// ≈Á¬Î µœ÷
	void on_pushButton_opencom_clicked();

	void on_pushButton_trigpenma_clicked();

	void on_pushButton_sendcontent_clicked();

	void toPenmaMsg(QStringList & msgList, char penmsg[4][64]);

private:
    Ui::RobotSettingUi *ui;
	ICom* m_pICom;
	int m_testSn;
	
};

#endif // ROBOTSETTINGUI_H
