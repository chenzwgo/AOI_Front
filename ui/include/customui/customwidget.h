/****************************************************************************
** Copyright (C) 2016 modularize for motion platform by ZhangLi and HuangXiong.  All rights reserved.
**
** This file is written to respond to custom widgets click and update events, developer only
** need to drag custom widget to ui, and new the class instance.Then, when click the custom widget,
** the class will do all things.
** 
** note: when use, you must new CustomWidget after the parent ui setup.And new CustomWidget(QObject*),
** this param can not be NULL.
****************************************************************************/

#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#ifdef UI_CUSTOMWIDGET
#define RT_API Q_DECL_EXPORT
#else
#define RT_API Q_DECL_IMPORT
#endif

#include <QObject>

class MotionCtlDlg;
class RT_API CustomWidget : public QObject
{
    Q_OBJECT

public:

    // 必须在parent的UI对象构造后构造
    explicit CustomWidget(QObject *parent);
    ~CustomWidget();

    // 更新UI,提供外部定时器调用
    void updateAllWidget();

public slots:	
    //气缸
    void slot_Cylinder_Open();
    void slot_Cylinder_Update();

    //轴
    void slot_Axis_Enable();
    void slot_Axis_Stop();
    void slot_Axis_ClearAlarm();
    void slot_Axis_Update();

    //真空
    void slot_Vacuum_Open();
    void slot_Vacuum_Update();

    //IO
    void slot_ioInput_Update();
    void slot_ioOutPut_Open();
    void slot_ioOutPut_Update();

    //工站运动
    void slot_station_move();
    
    // 跑点
    void slot_run_point();

    //流程执行
    void slot_runFlow();

    // jog运动
    void slot_updateMovStatus();
    void slot_jogMoveP();
    void slot_jogMoveN();
    void slot_jogStop();

private:
    void addCustomWidget(QObject *parent);
    void updateWidget(QObject *pWidget);
    // 将控件加入customwidget统一处理
    void initWidgetConnect(QObject *pWidget);
    QObjectList m_objList;
    MotionCtlDlg *m_pStationMoveDlg;

};

#endif // CUSTOMWIDGET_H
