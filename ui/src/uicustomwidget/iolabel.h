#ifndef IOLABEL_H
#define IOLABEL_H

#include <QLabel>

class IOLabel : public QLabel
{
    Q_OBJECT
public:
    explicit IOLabel(QWidget* parent = 0);
    ~IOLabel();

    void setState(bool bOn);

signals:
    void signalSetState(bool bOn);
public slots:
    void slotSetState(bool bOn);
private:
    int m_bState;
};

#endif // IOLABEL_H
