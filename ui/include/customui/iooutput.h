/****************************************************************************
** Copyright (C) 2016 modularize for motion platform by HuangXiong.  All rights reserved.
**
** This class is written to show IO output operation interface.
** The class only show ui and emit open IO signal,it's not do the actual work.
** 
** note: when use, you must set the correct IO name in request .
****************************************************************************/

#ifndef IOOUTPUT_H
#define IOOUTPUT_H

#include <QPushButton>

class IoOutPut : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool state READ state WRITE setState)
    Q_PROPERTY(QString ioName READ ioName WRITE setIoName)
    Q_PROPERTY(QString slotPreProcess READ slotPreProcess WRITE setSlotPreProcess)

public:
    IoOutPut(QWidget *parent = 0);

    bool state() const { return isChecked(); }
    void setState(bool bOn) { setChecked(bOn); }

    void setIoName(const QString &ioName) { m_ioName = ioName; setToolTip(ioName);}
    QString ioName() const { return m_ioName; }

    QString slotPreProcess() {return m_slotPreProcess;}
    void setSlotPreProcess(const QString text) {m_slotPreProcess = text;}

    void update() { emit sigUpdate(); }
    // 预处理
    bool preProcess();

protected:
    void showEvent(QShowEvent *);

signals:
    void sigUpdate();
    void sigPreProcess(bool&); // 预处理

private:
    bool m_bState;
    QString m_ioName;
    QString m_slotPreProcess;
};

#endif
