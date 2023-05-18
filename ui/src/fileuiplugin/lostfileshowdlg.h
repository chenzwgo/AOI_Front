#ifndef LOSTFILESHOWDLG_H
#define LOSTFILESHOWDLG_H

#include <QDialog>

namespace Ui {
class LostFileShowDlg;
}

class QCloseEvent;
class LostFileShowDlg : public QDialog
{
    Q_OBJECT

public:
	enum 
	{
		Operator_UseDefault = 0,
		Operator_Stop,
		Operator_Ignore,
		Operator_End
	};
    explicit LostFileShowDlg(QWidget *parent = 0);
    ~LostFileShowDlg();

	void setLostFile(const QStringList& lostFileList);
protected:
	void closeEvent(QCloseEvent * event);
private slots:
	void on_pushButton_useDefault_clicked();
	void on_pushButton_stop_clicked();
	void on_pushButton_ignore_clicked();
private:
    Ui::LostFileShowDlg *ui;
};

#endif // LOSTFILESHOWDLG_H
