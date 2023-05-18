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
	int dir;   //�˶��Ƿ���,1����-1��
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
	void initAxisMap();  // ���ڼ��ƶ�����
    void refreshAxisStatu();
    void updatePointUI();
	void refreshCurrentPos( void );
    void axisEnable(int axis); // ��ʹ��
    void clearAlarm(int axis); // ����ᱨ��
    void axisHome(int axis);   // ���ԭ
    void updateStationSpeed(); // ���¹�վ�ٶ�

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

    // ʹ��
    void on_pushButton_axis_on_1_clicked();
    void on_pushButton_axis_on_2_clicked();
    void on_pushButton_axis_on_3_clicked();
    void on_pushButton_axis_on_4_clicked();
    void on_pushButton_axis_on_5_clicked();
    void on_pushButton_axis_on_6_clicked();
    // �������
    void on_pushButton_axis_clear_alarm_1_clicked();
    void on_pushButton_axis_clear_alarm_2_clicked();
    void on_pushButton_axis_clear_alarm_3_clicked();
    void on_pushButton_axis_clear_alarm_4_clicked();
    void on_pushButton_axis_clear_alarm_5_clicked();
    void on_pushButton_axis_clear_alarm_6_clicked();
    // ��ԭ
    void on_pushButton_home_1_clicked();
    void on_pushButton_home_2_clicked();
    void on_pushButton_home_3_clicked();
    void on_pushButton_home_4_clicked();
    void on_pushButton_home_5_clicked();
    void on_pushButton_home_6_clicked();
    // �ٶ�
    void on_lowSpeed_clicked(bool);
    void on_highSpeed_clicked(bool);
    void on_defineSpeed_toggled(bool);

    // ��λ���
    void on_tableWidgetpoint_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    // �����ٶ�
    void on_pushButton_setspeed_clicked();
    // չ��������
    void on_pushButton_expand_clicked();

    void slotAlwaysMove();  // �����˶�
    void slotAlwaysMoveFinish();  // �����˶�����
    void slotChangedistance();

   void slotModPoint(int row,int cloun);//�޸ĵ���Ϣ

private:
    Ui::MotionCtlDlg *ui;

    CustomWidget *m_pCustomWidget;
	IStation *m_pStation;
    StStationInfo m_stationInfo; // ��վ��Ϣ
	bool m_axisDir[6]; // �᷽��
	map<QPushButton*, StAxisMove> m_mapAxisMove; // ���ƶ�
    QTimer m_timer;   // ��ʱ��ˢ��״̬
    QStringList m_pointNameList; //��ӵ�������Ϣ.all point
    QList<StPoint> m_pointInfoList;
    bool m_isReadFile;//��¼��ǰ�Ƿ��Ƕ��ļ�������
};

#endif // MOTIONCTLDLG_H
