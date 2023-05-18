#ifndef INOTIFY_H
#define INOTIFY_H

#include <QString>
#include "OS_DEFINE.h"
#define PLUGIN_IID_NAME_INOTIFY    "com.bj.Plugin.INotify/1.0"
class INotify
{
public:

    virtual ~INotify(){}
    virtual bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam) = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(INotify, PLUGIN_IID_NAME_INOTIFY);
QT_END_NAMESPACE

#endif // INOTIFY_H
