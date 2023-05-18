#ifndef TEMPERATUREDLG_H
#define TEMPERATUREDLG_H

#include <QDialog>

namespace Ui {
class TemperatureDlg;
}

class TemperatureDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TemperatureDlg(QWidget *parent = 0);
    ~TemperatureDlg();

protected:
	void showEvent(QShowEvent *event);

protected slots:
	void on_pushButton_refresh_clicked();

private:
    Ui::TemperatureDlg *ui;
};

#endif // TEMPERATUREDLG_H
