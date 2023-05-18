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
	//�Թ�Դ����һ���Ƚ�ֹ��ʹ��Ƶ������//�л���Ʒ���ԭʱ���в���//
	bool ResetStroboflash();//
    ~DebugView();

public slots:
	void slotTimerEvent();

    //***�������ǰ��Ҫ��Ԥ����,��������ʾ, \
    //***����ۺ���������<����::������>���� \
    //***���Զ���ؼ���slotPreProcess��,�磺\
    //***DebugView::slotTestCylinder��
    void slotTestCylinder(bool &);  // ����Ԥ����
    void slotTestOutPut(bool &);    // IO���Ԥ����
    void slotTestVacuum(bool &);    // ���Ԥ����
    void slotTestRunPoint(bool &);  // �ܵ�Ԥ����
    void slotTestPreProcess(bool &);  // Ԥ����

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
