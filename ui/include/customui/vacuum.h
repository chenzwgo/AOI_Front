/****************************************************************************
** Copyright (C) 2016 modularize for motion platform by HuangXiong.  All rights reserved.
**
** This class is written to show vacuum operation interface .
** The class only show ui and emit suitable signal,it's not do the actual work.
** 
** note: when use, you must set the correct IO names in request .
****************************************************************************/

#ifndef VACUUM_H
#define VACUUM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class Vacuum : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    //Q_PROPERTY(bool blowVacuum READ blowVacuum WRITE setBlowVacuum)
    Q_PROPERTY(bool existArrive READ existArrive WRITE setExistArrive)
    Q_PROPERTY(QString ioNameVacuumArrive READ ioNameVacuumArrive WRITE setIoNameVacuumArrive)
    Q_PROPERTY(QString ioNameAbsorbVacuum READ ioNameAbsorbVacuum WRITE setIoNameAbsorbVacuum)
    Q_PROPERTY(QString ioNameBlowVacuum READ ioNameBlowVacuum WRITE setIoNameBlowVacuum)
    Q_PROPERTY(QSize minIconSize READ minIconSize WRITE setMinIconSize)
    Q_PROPERTY(QSize maxIconSize READ maxIconSize WRITE setMaxIconSize)
    Q_PROPERTY(QString slotPreProcess READ slotPreProcess WRITE setSlotPreProcess)

public:
    Vacuum(QWidget *parent = 0);

    // 设置IO
    void setIo(const QString &ioNameAbsorbVacuum, const QString &ioNameVacuumArrive="", const QString &ioNameBlowVacuum="");
    // 更新UI
    void update();
    // 预处理
    bool preProcess();

    void setText(const QString &text);
    QString text() const;

    //void setBlowVacuum(bool);
    //bool blowVacuum() const;

    void setExistArrive(bool);
    bool existArrive() const;

    void setAbsorbVacuumChecked(bool);
    bool isAbsorbVacuumChecked() const;

    void setVacuumArrived(bool);
    bool isVacuumArrived() const;

    void setMinIconSize(const QSize &size) { m_labelVacuumArrive->setMinimumSize(size); m_labelVacuumArrive->setMinimumSize(size);}
    QSize minIconSize() const { return m_labelVacuumArrive->minimumSize(); }

    void setMaxIconSize(const QSize &size) { m_labelVacuumArrive->setMaximumSize(size); m_labelVacuumArrive->setMaximumSize(size);}
    QSize maxIconSize() const { return m_labelVacuumArrive->maximumSize(); }

    QString ioNameVacuumArrive() {return m_ioNameVacuumArrive;}
    void setIoNameVacuumArrive(const QString text) {m_ioNameVacuumArrive = text; m_labelVacuumArrive->setToolTip(text);}

    QString ioNameAbsorbVacuum() {return m_ioNameAbsorbVacuum;}
    void setIoNameAbsorbVacuum(const QString text) { m_ioNameAbsorbVacuum = text;
                                                     QString toolTip = m_ioNameAbsorbVacuum;
                                                     if(!m_ioNameBlowVacuum.isEmpty())
                                                     {
                                                         toolTip += ","+m_ioNameBlowVacuum;
                                                     }
                                                     m_pushbuttonAbsorb->setToolTip(toolTip); }

    QString ioNameBlowVacuum() {return m_ioNameBlowVacuum;}
    void setIoNameBlowVacuum(const QString text) { m_ioNameBlowVacuum = text;
                                                   QString toolTip = m_ioNameAbsorbVacuum;
                                                   if(!m_ioNameBlowVacuum.isEmpty())
                                                   {
                                                       toolTip += ","+m_ioNameBlowVacuum;
                                                   }
                                                   m_pushbuttonAbsorb->setToolTip(toolTip); }

    QString slotPreProcess() {return m_slotPreProcess;}
    void setSlotPreProcess(const QString text) {m_slotPreProcess = text;}

protected:
    void showEvent(QShowEvent *);
	
signals:
    void sigAbsorbVacuum();
    void sigUpdate();
    void sigPreProcess(bool&); // 预处理

private:
    void initUi();

private:
    QPushButton *m_pushbuttonAbsorb;  //吸真空
    QLabel      *m_labelVacuumArrive; //真空到达

    QString m_ioNameVacuumArrive;
    QString m_ioNameAbsorbVacuum;
    QString m_ioNameBlowVacuum;

    bool m_existVacuumArrived; // 是否有到位信号
    bool m_bVacuumArrived;
    QPixmap m_pixmaps[3];    // 开关图标 index=0 是off图标 index=1 是on图标
	QString m_slotPreProcess;
};

#endif
