#ifndef _I_MENUBAR_H
#define _I_MENUBAR_H

#include <QWidget>
#include <QString>
#include <QList>

#define PLUGIN_IID_NAME_IMENUBAR    "com.aooxoo.Plugin.IMenuBar/1.0"
class IMenuBar
{   
public:
    enum
    {
        IMENUBAR_ID = 0x66
    };
    virtual ~IMenuBar(){}

    virtual void EnableMenuItem(const QString& strPluginName) = 0;
    virtual void DisableMenuItem(const QString& strPluginName) = 0;
    virtual void EnableAllMenuItem() = 0;
    virtual void DisableAllMenuItem() = 0;
};
QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(IMenuBar,PLUGIN_IID_NAME_IMENUBAR);
QT_END_NAMESPACE
#endif // _I_MENUBAR_H
