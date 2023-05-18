#ifndef AXISJOG_H
#define AXISJOG_H

#include <QWidget>

namespace Ui {
class AxisJog;
}

class AxisJog : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(bool moving READ moving WRITE setMoving)
    Q_PROPERTY(QString slotPreProcess READ slotPreProcess WRITE setSlotPreProcess)

public:
    explicit AxisJog(QWidget *parent = 0);
    ~AxisJog();

    void setName(const QString &name) { m_name = name; setToolTip(name);}
    QString name() const { return m_name; }

    void setMoving(bool bMove);
    bool moving() { return m_bMoving; }

    void update() { emit sigUpdate(); }
    // 预处理
    bool preProcess();

    QString slotPreProcess() {return m_slotPreProcess;}
    void setSlotPreProcess(const QString text) {m_slotPreProcess = text;}
signals:
    void sigUpdate(); //更新运行状态
    void sigStop();  //停止
    void sigMoveP(); //正转
    void sigMoveN(); //反转
    void sigPreProcess(bool&); // 预处理
	
protected:
    void showEvent(QShowEvent *);

private:
    Ui::AxisJog *ui;
    bool m_bMoving;
    QString m_name;
    QString m_slotPreProcess;
};

#endif // AXISJOG_H
