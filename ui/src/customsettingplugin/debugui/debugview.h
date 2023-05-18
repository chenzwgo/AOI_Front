#ifndef DEBUGVIEW_H
#define DEBUGVIEW_H

#include <QFrame>
#include <jthreadt.h>
#include <QMap>
#include "motion/imotion.h"
#include "card/icardcontinterp.h"
#include "station/istation.h"
#include "proxymanager.h"
#include <jthreadt.h>
namespace Ui {
class DebugView;
}

class QTimer;
class QShowEvent;
class QHideEvent;
class CustomWidget;
class IlluminationControlDlg;
class IllumChannelWdgt;

class DebugView : public QFrame
{
    Q_OBJECT
private:
public:
    explicit DebugView(QFrame *parent = 0);
	void SetLightInstance(IlluminationControlDlg* pInt);
    bool ClearLightNumber();
	//对光源进行一次先禁止再使能频闪操作//切换产品后回原时进行操作//
	bool ResetStroboflash();//
    ~DebugView();

public slots:
	void slotTimerEvent();

    //***如果动作前需要做预处理,就如下所示, \
    //***定义槽函数，并把<类名::函数名>配置 \
    //***到自定义控件的slotPreProcess项,如：\
    //***DebugView::slotTestCylinder。
    void slotTestCylinder(bool &);  // 气缸预处理
    void slotTestOutPut(bool &);    // IO输出预处理
    void slotTestVacuum(bool &);    // 真空预处理
    void slotTestRunPoint(bool &);  // 跑点预处理
    void slotTestPreProcess(bool &);  // 预处理

    void on_pushButton_svae_set_clicked();
	void on_pushButton_clear_clicked();

	//void on_checkBox_A_clicked(bool checked);
	//void on_checkBox_B_clicked(bool checked);
	//void on_checkBox_C_clicked(bool checked);
	//void on_checkBox_D_clicked(bool checked);

    //void on_checkBox_B_R_clicked(bool checked);
    //void on_checkBox_C_R_clicked(bool checked);
    //void on_checkBox_D_R_clicked(bool checked);

    void on_pushButton_test_clicked(bool checked);
    
	void on_pushButton_calib_clicked(bool checked);
    void on_pushButton_signal_clicked(bool checked);
    void on_pushButton_calib_pluse_ratio_clicked(bool checked);
protected:
	virtual void showEvent(QShowEvent *event);
	virtual void hideEvent(QHideEvent *event);

private:
	void initUi();
	bool intCom();
	void LightTypeSet(QString strLightType);

    Ui::DebugView *ui;
	CustomWidget *m_pCustomWidget;
	IlluminationControlDlg *m_pLight;
	QTimer *m_pTimer;
	iMotion*   m_pMotion;

    IStation*  m_pTarget ;

	
};

#endif // DEBUGVIEW_H
