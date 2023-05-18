/****************************************************************************
** Copyright (C) 2016 modularize for motion platform by HuangXiong.  All rights reserved.
**
** This class is written to show cylinder operation interface .Support One-way solenoid valve 
** and Two-way solenoid valve just by the method setTwoWay.
** The class only show ui and emit suitable signal,it's not do the actual work.
** 
** note: when use, you must set the correct IO names in request .
****************************************************************************/

#ifndef QCYLINDER_H
#define QCYLINDER_H

#include <QWidget>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class QCylinder : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(bool isTwoWay READ isTwoWay WRITE setTwoWay)
    Q_PROPERTY(bool existLimitP READ existLimitP WRITE setExistLimitP)
    Q_PROPERTY(bool existLimitN READ existLimitN WRITE setExistLimitN)
    Q_PROPERTY(QSize minIconSize READ minIconSize WRITE setMinIconSize)
    Q_PROPERTY(QSize maxIconSize READ maxIconSize WRITE setMaxIconSize)
    Q_PROPERTY(QString ioNameOpen READ ioNameOpen WRITE setIoNameOpen)
    Q_PROPERTY(QString ioNameClose READ ioNameClose WRITE setIoNameClose)
    Q_PROPERTY(QString ioNameLimitP READ ioNameLimitP WRITE setIoNameLimitP)
    Q_PROPERTY(QString ioNameLimitN READ ioNameLimitN WRITE setIoNameLimitN)
    Q_PROPERTY(QString slotPreProcess READ slotPreProcess WRITE setSlotPreProcess)

public:
    explicit QCylinder(QWidget *parent = 0);
    ~QCylinder();

    // 设置IO
    void setIo(const QString &ioNameOpen, const QString &ioNameClose="", const QString &ioNameLimitP="",const QString &ioNameLimitN="");
    // 更新UI
    void update();
    // 预处理
    bool preProcess();

    void setText(const QString &text);
    QString text() const;

    void setTwoWay(const bool twoWay) { m_isTwoWay = twoWay; } // 是否双向电磁阀
    bool isTwoWay() const { return m_isTwoWay; }

    void setExistLimitP(const bool exist);
    bool existLimitP() const { return m_existLimitP; }

    void setExistLimitN(const bool exist);
    bool existLimitN() const { return m_existLimitN; }

    void setMinIconSize(const QSize &size) { label_plimit->setMinimumSize(size); label_nlimit->setMinimumSize(size);}
    QSize minIconSize() const { return label_plimit->minimumSize(); }

    void setMaxIconSize(const QSize &size) { label_plimit->setMaximumSize(size); label_nlimit->setMaximumSize(size);}
    QSize maxIconSize() const { return label_plimit->maximumSize(); }

    bool isChecked();
    void setChecked(const bool checked);

    bool limitPOn() {return m_limitPOn;}
    void setLimitPOn(const bool on);

    bool limitNOn() {return m_limitNOn;}
    void setLimitNOn(const bool on);

    QString ioNameOpen() {return m_ioNameOpen;}
    void setIoNameOpen(const QString text) {m_ioNameOpen = text; pushButton->setToolTip(m_ioNameOpen+"\n"+m_ioNameClose);}

    QString ioNameClose() {return m_ioNameClose;}
    void setIoNameClose(const QString text) {m_ioNameClose = text; pushButton->setToolTip(m_ioNameOpen+"\n"+m_ioNameClose);}

    QString ioNameLimitP() {return m_ioNameLimitP;}
    void setIoNameLimitP(const QString text) {m_ioNameLimitP = text; label_plimit->setToolTip(text);}

    QString ioNameLimitN() {return m_ioNameLimitN;}
    void setIoNameLimitN(const QString text) {m_ioNameLimitN = text; label_nlimit->setToolTip(text);}

    QString slotPreProcess() {return m_slotPreProcess;}
    void setSlotPreProcess(const QString text) {m_slotPreProcess = text;}

signals:
    void clicked();       // 点击
    void sigUpdate();     // 更新
    void sigPreProcess(bool&); // 预处理

private:
    void initUI();

protected:
    void showEvent(QShowEvent *);

private:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label_nlimit;
    QLabel *label_plimit;
    QPushButton *pushButton;

    QPixmap m_pixmaps[3]; // 开关图标 index=0 是off图标 index=1 是on图标

    bool m_existLimitP;
    bool m_existLimitN;
    bool m_limitPOn;
    bool m_limitNOn;
    bool m_isTwoWay;
    QString m_ioNameOpen;
    QString m_ioNameClose;
    QString m_ioNameLimitN;
    QString m_ioNameLimitP;
    QString m_slotPreProcess;
};

#endif //QCYLINDER_H

