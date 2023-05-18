/****************************************************************************
** Copyright (C) 2016 modularize for motion platform by HuangXiong.  All rights reserved.
**
** This class is written to show one axis's status, such as is home, is limit, is alarm, is running.
** And show button enable, stop, home, clear alarm.
** The class only show ui and emit suitable signal,it's not do the actual work.
** 
** note: when use, you must set the correct axisname and IO names in request .
****************************************************************************/

#ifndef QAXISSTATUS_H
#define QAXISSTATUS_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Ui {
class frame_axisstatus;
}

class QAxisStatus : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QString axisName READ axisName WRITE setAxisName)
    Q_PROPERTY(QString axisDesc READ axisDesc WRITE setAxisDesc)
    Q_PROPERTY(bool showDesc READ showDesc WRITE setShowDesc)
    Q_PROPERTY(bool showTitle READ showTitle WRITE setShowTitle)
    Q_PROPERTY(bool limitPEnabled READ limitPEnabled WRITE setLimitPEnabled)
    Q_PROPERTY(bool limitNEnabled READ limitNEnabled WRITE setLimitNEnabled)
    Q_PROPERTY(bool homeEnabled READ homeEnabled WRITE setHomeEnabled)
    Q_PROPERTY(bool powerOnEnabled READ powerOnEnabled WRITE setPowerOnEnabled)
    Q_PROPERTY(bool alarmEnabled READ alarmEnabled WRITE setAlarmEnabled)
    Q_PROPERTY(bool runingEnabled READ runingEnabled WRITE setRuningEnabled)

    Q_PROPERTY(bool limitPVisable READ limitPVisable WRITE setLimitPVisable)
    Q_PROPERTY(bool limitNVisable READ limitNVisable WRITE setLimitNVisable)
    Q_PROPERTY(bool homeVisable READ homeVisable WRITE setHomeVisable)
    Q_PROPERTY(bool powerOnVisable READ powerOnVisable WRITE setPowerOnVisable)
    Q_PROPERTY(bool alarmVisable READ alarmVisable WRITE setAlarmVisable)
    Q_PROPERTY(bool runingVisable READ runingVisable WRITE setRuningVisable)

public:
    QAxisStatus(QWidget *parent = 0);

signals:
    void sigAxisOn();
    void sigClearAlarm();
    void sigStop();
    void sigUpdate();

public:

    // 设置IO
    void setIo(QString axisName, QString ioLimitP="", QString ioLimitN="", QString ioHome="", QString ioPowerOn="", QString ioAlarm="", QString ioEnable="", QString ioClear="", QString desc="");
    // 更新UI
    void update();

    QString ioNameLimitP() { return m_ioNameLimitP; }
    QString ioNameLimitN() { return m_ioNameLimitN; }
    QString ioNameHome() { return m_ioNameHome; }
    QString ioNameAlarm() { return m_ioNameAlarm; }
    QString ioNamePowerOn() { return m_ioNamePowerOn; }
    QString ioNameEnable() { return m_ioNameEnable; }
    QString ioNameClearAlarm() { return m_ioNameClear; }

    void setLimitPOn(bool bOn);
    void setLimitNOn(bool bOn);
    void setHomeOnOn(bool bOn);
    void setAlarmOn(bool bOn);
    void setPowerOn(bool bOn);
    void setRuning(bool bOn);

    //********** UI可设置属性 ************
    QString axisName() {return m_axisName;}
    void setAxisName(const QString text);

    QString axisDesc() {return m_axisDesc;}
    void setAxisDesc(const QString text);

    bool showDesc()  {return m_bShowDesc;}
    void setShowDesc(bool bShow);

    bool showTitle()  {return m_bShowTitle;}
    void setShowTitle(bool bShow);

    bool limitPEnabled();
    void setLimitPEnabled(bool bEnable);

    bool limitNEnabled();
    void setLimitNEnabled(bool bEnable);

    bool homeEnabled();
    void setHomeEnabled(bool bEnable);

    bool powerOnEnabled();
    void setPowerOnEnabled(bool bEnable);

    bool alarmEnabled();
    void setAlarmEnabled(bool bEnable);

    bool runingEnabled();
    void setRuningEnabled(bool bEnable);


    bool limitPVisable();
    void setLimitPVisable(bool bVisable);

    bool limitNVisable();
    void setLimitNVisable(bool bVisable);

    bool homeVisable();
    void setHomeVisable(bool bVisable);

    bool powerOnVisable();
    void setPowerOnVisable(bool bVisable);

    bool alarmVisable();
    void setAlarmVisable(bool bVisable);

    bool runingVisable();
    void setRuningVisable(bool bVisable);

private:
    Ui::frame_axisstatus *m_ui;

    QString m_axisName;
    QString m_axisDesc;
    QString m_ioNameLimitP;
    QString m_ioNameLimitN;
    QString m_ioNameHome;
    QString m_ioNamePowerOn;
    QString m_ioNameAlarm;
    QString m_ioNameEnable;
    QString m_ioNameClear;

    bool m_bShowDesc;
    bool m_bShowTitle;
};

#endif // QAXISSTATUS_H

