#ifndef FLICKERLABEL_H
#define FLICKERLABEL_H
#include <QTimer>
#include <QLabel>

class flickerLabel : public QLabel
{
        Q_OBJECT
public:
    flickerLabel(QWidget* parent);
    ~flickerLabel();
public :signals:
    void sigShowLableMsg(QString );
    void sigClearLableMsg();
public slots:
    void slotShowLableMsg(QString strmsg);
    void slotClearLableMsg();
    void slotTimeout();
private:
    QTimer *m_pTimer;
    QString m_msg;
    int m_istep;
};

#endif // FLICKERLABEL_H
