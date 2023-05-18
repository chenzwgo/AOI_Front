#ifndef MOTIONCTLDLG_H
#define MOTIONCTLDLG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <vector>
#include <QTimer>
#include <QKeyEvent>
#include "motor_common.h"
#include "customui/customwidget.h"

#define STATION_RECHECK   RECHECK_STATION
#define STATION_PRESS     PRESSURE_STATION
#define STATION_PLATE     LOAD_MATERIAL_STATION

#ifdef UI_CUSTOMWIDGET
#define RT_API Q_DECL_EXPORT
#else
#define RT_API Q_DECL_IMPORT
#endif

namespace Ui {
class MotionCtlDlg;
}

typedef struct _axisMove
{
	QDoubleSpinBox *lineEditOffset;
	int axis;
	int dir;   //运动是否反向,1正，-1反
	_axisMove()
	{
		lineEditOffset = NULL;
		axis = 0;
		dir = 0;
	}

	_axisMove(QDoubleSpinBox *lineEdit, int nAxis, int axisDir)
	{
		lineEditOffset = lineEdit;
		axis = nAxis;
		if (axisDir)
		{
			dir = -1;
		}
		else
		{
			dir = 1;
		}
	}
}StAxisMove;

class IStation;
class QLabel;

class RT_API MotionCtlDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MotionCtlDlg(QWidget *parent = 0);
	MotionCtlDlg(const QString &stationName, QWidget *parent = 0);
    ~MotionCtlDlg();

	bool setStation( const QString &stationName, const QString &pointMask="");

private:
	void initUI();
	void initAxisUI(int axiscount);
    void initMoveJogBtn();
	void readAxisDir();
	void initAxisMap();  // 用于简化移动函数
    void refreshAxisStatu();
    void updatePointUI();
	void refreshCurrentPos( void );
    void axisEnable(int axis); // 轴使能
    void clearAlarm(int axis); // 清除轴报警
    void axisHome(int axis);   // 轴回原
    void updateStationSpeed(); // 更新工站速度

protected:
	void showEvent(QShowEvent *);
	void hideEvent(QHideEvent *);
    void keyPressEvent(QKeyEvent *e);

private slots:
	void slotMoveAxis( void );
    void slot_resize(void);
    void slot_timeout();
    void on_pushButton_station_stop_clicked();
    void on_pushButton_station_home_clicked();
    void on_pushButton_movePoint_clicked();
    void on_pushButton_AddPoint_clicked();
    void on_pushbutton_deletepoint_clicked();
    void on_pushButton_teachPoint_clicked();

    // 使能
    void on_pushButton_axis_on_1_clicked();
    void on_pushButton_axis_on_2_clicked();
    void on_pushButton_axis_on_3_clicked();
    void on_pushButton_axis_on_4_clicked();
    void on_pushButton_axis_on_5_clicked();
    void on_pushButton_axis_on_6_clicked();
    // 清除报警
    void on_pushButton_axis_clear_alarm_1_clicked();
    void on_pushButton_axis_clear_alarm_2_clicked();
    void on_pushButton_axis_clear_alarm_3_clicked();
    void on_pushButton_axis_clear_alarm_4_clicked();
    void on_pushButton_axis_clear_alarm_5_clicked();
    void on_pushButton_axis_clear_alarm_6_clicked();
    // 回原
    void on_pushButton_home_1_clicked();
    void on_pushButton_home_2_clicked();
    void on_pushButton_home_3_clicked();
    void on_pushButton_home_4_clicked();
    void on_pushButton_home_5_clicked();
    void on_pushButton_home_6_clicked();
    // 速度
    void on_lowSpeed_clicked(bool);
    void on_highSpeed_clicked(bool);
    void on_defineSpeed_toggled(bool);

    // 点位表格
    void on_tableWidgetpoint_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    // 设置速度
    void on_pushButton_setspeed_clicked();
    // 展开与收缩
    void on_pushButton_expand_clicked();

    void slotAlwaysMove();  // 连续运动
    void slotAlwaysMoveFinish();  // 连续运动结束
    void slotChangedistance();

   void slotModPoint(int row,int cloun);//修改点信息

private:
    Ui::MotionCtlDlg *ui;

    CustomWidget *m_pCustomWidget;
	IStation *m_pStation;
    StStationInfo m_stationInfo; // 工站信息
	bool m_axisDir[6]; // 轴方向
	map<QPushButton*, StAxisMove> m_mapAxisMove; // 轴移动
    QTimer m_timer;   // 定时器刷轴状态
    QStringList m_pointNameList; //添加点名称信息.all point
    QList<StPoint> m_pointInfoList;
    bool m_isReadFile;//记录当前是否是读文件操作。
};

#endif // MOTIONCTLDLG_H
