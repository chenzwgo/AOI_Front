#ifndef DEFECTCFG_H
#define DEFECTCFG_H

#include <QWidget>
#include "maindefine.h"
namespace Ui {
class defectcfg;
}

class defectcfg : public QWidget
{
    Q_OBJECT

public:
	static defectcfg* GetInstance(QWidget* pParant = NULL);
	static void ReleaseInstance();

private:
    explicit defectcfg(QWidget *parent = 0);
    ~defectcfg();
	void setEnable(bool bEn);
	void updateUi();

public slots:
    void on_pushButton_save_clicked();
    void on_pushButton_canle_clicked();
	void slotChangePruduct();
private:
    Ui::defectcfg *ui;
	static defectcfg* m_pSelfInstance;

};

#endif // DEFECTCFG_H
