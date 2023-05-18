#ifndef STATIONMOVE_H
#define STATIONMOVE_H

#include <QPushButton>

class StationMove : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QString staionName READ staionName WRITE setStationName)
    Q_PROPERTY(QString pointMask READ pointMask WRITE setPointMask)

public:
    StationMove(QWidget *parent = 0);

    void setStationName(const QString &stationName) { m_stationName = stationName; setToolTip(stationName);}
    QString staionName() const { return m_stationName; }

    void setPointMask(const QString &pointMask) { m_pointMask = pointMask;}
    QString pointMask() const { return m_pointMask; }

private:
    QString m_stationName;
    QString m_pointMask;
};

#endif
