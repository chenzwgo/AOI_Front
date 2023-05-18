#ifndef CLOCK_H
#define CLOCK_H

#include <QWidget>
#include "QTimer"
#include "QTime"
#include "QPainter"
#include "QPen"
#include "QPalette "

namespace Ui {
class Clock;
}

class Clock : public QWidget
{
    Q_OBJECT

public:
    explicit Clock(QWidget *parent = 0);
    ~Clock();
protected:
	virtual void paintEvent(QPaintEvent *event);
private:
    Ui::Clock *ui;
	QTimer *m_pTimer;

private:
	virtual void draw(QPainter *painter);
	void gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color);
};

#endif // CLOCK_H
