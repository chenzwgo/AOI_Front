#ifndef DISPLAYLOGWIDGET_H
#define DISPLAYLOGWIDGET_H
#include "dllexport.h"
#include "uidefine.h"

#include <QWidget>
#include <QPlainTextEdit>


class RT_API DisplayLogWidget:public QPlainTextEdit
{
    Q_OBJECT
public:
    DisplayLogWidget(QWidget* parent=NULL);
    ~DisplayLogWidget();

    void ClearLog();
    void SetMaxDisplaycount(int maxcount=500);
    void Addlogmsg(QString msg,UILogLevel error = UILOG_INFO ,bool bsavefile = false, bool bshowTime = true);
signals:
    void addmsgsig(QString,int,bool,bool);
private slots:
    void slotAddmsg(QString msg,int errorlevel,bool bsavefile,bool bshowTime);
};

#endif // DISPLAYLOGWIDGET_H
