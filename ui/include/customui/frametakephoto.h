#ifndef FRAMETAKEPHOTO_H
#define FRAMETAKEPHOTO_H

#include <QFrame>

namespace Ui {
class FrameTakePhoto;
}

class FrameTakePhoto : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QString sceneName READ sceneName WRITE setSceneName)
    Q_PROPERTY(int timeOut READ timeOut WRITE setTimeOut)

signals:
    void sigTakePhoto();

public:
    explicit FrameTakePhoto(QWidget *parent = 0);
    ~FrameTakePhoto();

    void updateUiData(double x, double y, double a);

    QString sceneName() { return m_sceneName; }
    void setSceneName(const QString text) { m_sceneName = text; }

    int timeOut() { return m_timeOut; }
    void setTimeOut(const int timeout) { m_timeOut = timeout; }

private:
    QString m_sceneName;
    int     m_timeOut;
    Ui::FrameTakePhoto *ui;
};

#endif // FRAMETAKEPHOTO_H
