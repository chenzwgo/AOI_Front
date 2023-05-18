#ifndef MOVING_H
#define MOVING_H

#include <QDialog>
#include "errcode_def.h"
#include "motor_common.h"
#include <QTimer>

#ifdef UI_CUSTOMWIDGET
#define RT_API Q_DECL_EXPORT
#else
#define RT_API Q_DECL_IMPORT
#endif

namespace Ui {
	class Moving;
}

class IStation;

class RT_API  MovingDlg : public QDialog
{
	Q_OBJECT

public:
	MovingDlg(const QString &stationName, QWidget *parent = 0);
	~MovingDlg();
  
    // �ܵ�,mask:���룬����ѡ���ƶ��ļ����ᣬ bAbs���Ƿ�Ϊ�����˶�
	ERROR_CODE MovePoint(const QString &pointName, short mask = Mask_ALL, bool bAbs = true);
	ERROR_CODE MovePoint(int pointIndex, int short = Mask_ALL, bool bAbs = true);
    ERROR_CODE MoveHome(short mask = Mask_ALL);

protected:
	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);

private slots:
    void on_toolButton_pause_clicked();

    void on_toolButton_continue_clicked();

    void on_toolButton_stop_clicked();

	void on_timeout();

private:
	enum {E_Type_POINT, E_Type_HOME, E_TYPE_OFFSET};
	Ui::Moving *ui;
	IStation *m_pStation;  //��վ
	QTimer *m_pTimer;      //��ʱ��
    int   m_type;        //�Ƿ��ǻ�ԭ
    StPos    m_pointPos;   //������
    short m_axisMask;     //������
};

#endif // MOVING_H
