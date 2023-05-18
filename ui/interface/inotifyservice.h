#ifndef INOTIFYSERVICE_H
#define INOTIFYSERVICE_H

#include <QWidget>
#include "OS_DEFINE.h"
#define PLUGIN_IID_NAME_INOTIFYSERVICE    "com.aooxoo.Plugin.INotifyService/1.0"
class INotifyService
{
public:
    enum
    {
        IID_NOTIFYSERVICE = 0x001
    };
    virtual ~INotifyService(){}
    virtual bool subscribe(QString strPluginName, QString event) = 0;
    virtual bool unsubscribe(QString strPluginName, QString event) = 0;
    virtual bool send(QString strPluginName, QString event, const OS_LPARAM wparam, const OS_LPARAM lparam) = 0;
    virtual bool post(QString strPluginName, QString event, OS_LPARAM wparam, OS_LPARAM lparam) = 0;
    virtual bool push(QString event, OS_LPARAM wparam, OS_LPARAM lparam) = 0;
    virtual bool broadcast(QString event, OS_LPARAM wparam, OS_LPARAM lparam) = 0;
};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(INotifyService, PLUGIN_IID_NAME_INOTIFYSERVICE);
QT_END_NAMESPACE

#endif // INOTIFYSERVICE_H
