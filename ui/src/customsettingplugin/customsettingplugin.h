#ifndef CUSTOMSETTINGUIPLUGIN_H
#define CUSTOMSETTINGUIPLUGIN_H

#include <QObject>
#include "iuiplugin.h"
#include "inotify.h"

class CustomUi;
class DebugView;
class CustomSettingUIPlugin: public QObject, IUIPlugin, INotify
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IUIPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_INOTIFY)
    Q_INTERFACES(IPlugin IUIPlugin INotify)
public:
    CustomSettingUIPlugin();
    virtual ~CustomSettingUIPlugin();
    virtual bool init();
    virtual bool uninit();
    virtual bool showWindow(QWidget *parent, QRect &rect,bool bPopFlag = false);
    virtual bool hideWindow();
    virtual bool queryInterface(const int iid_interface, void **ppInterface);

    virtual void fitSize(const QRect& rect);

    virtual bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);

    virtual void SetLanguage(QString strlanguage);

private:
    CustomUi *m_pCustomUIWidget;
	DebugView *m_pDebugView;
};

#endif // ABOUTUIPLUGIN_H
