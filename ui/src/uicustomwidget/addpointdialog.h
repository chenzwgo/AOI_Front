#ifndef ADDPOINTDIALOG_H
#define ADDPOINTDIALOG_H

#include <QDialog>

namespace Ui {
class AddPointDialog;
}
class IStation;
class AddPointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPointDialog(IStation* station,QWidget *parent = 0);
    ~AddPointDialog();
    QString getPointName();
    int getPointIndex();
private slots:
    void on_buttonBox_accepted();
private:
    Ui::AddPointDialog *ui;
	bool IsSameNameAndIndex(int pointIndex, QString point );
	int GetNextIndex();
	IStation* m_pCurrentAxisStation;
};

#endif // ADDPOINTDIALOG_H
