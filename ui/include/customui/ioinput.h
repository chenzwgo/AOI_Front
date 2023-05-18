/****************************************************************************
** Copyright (C) 2016 modularize for motion platform by HuangXiong.  All rights reserved.
**
** This class is written to show IO input operation interface.
** The class only show IO input status by different icons.
** 
** note: when use, you must set the correct IO name in request .
****************************************************************************/

#ifndef IOINPUT_H
#define IOINPUT_H

#include <QLabel>

class IoInput : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(bool state READ state WRITE setState)
    Q_PROPERTY(QString ioName READ ioName WRITE setIoName)
    Q_PROPERTY(QSize minIconSize READ minIconSize WRITE setMinIconSize)
    Q_PROPERTY(QSize maxIconSize READ maxIconSize WRITE setMaxIconSize)

public:
    IoInput(QWidget *parent = 0);

    bool state() const { return m_bState; }
    void setState(bool bOn);

    void setIoName(const QString &ioName) { m_ioName = ioName; setToolTip(ioName); }
    QString ioName() const { return m_ioName; }

    void update() { emit sigUpdate(); }

    void setMinIconSize(const QSize &size) { this->setMinimumSize(size); this->setMinimumSize(size);}
    QSize minIconSize() const { return this->minimumSize(); }

    void setMaxIconSize(const QSize &size) { this->setMaximumSize(size); this->setMaximumSize(size);}
    QSize maxIconSize() const { return this->maximumSize(); }

signals:
    void sigUpdate();

private:
    bool m_bState;
    QString m_ioName;
};

#endif
