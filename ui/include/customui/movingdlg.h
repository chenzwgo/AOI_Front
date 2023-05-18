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
  
    // 跑点,mask:掩码，用于选择移动哪几个轴， bAbs：是否为绝对运动
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
	IStation *m_pStation;  //工站
	QTimer *m_pTimer;      //定时器
    int   m_type;        //是否是回原
    StPos    m_pointPos;   //点坐标
    short m_axisMask;     //轴掩码
};

#endif // MOVING_H
