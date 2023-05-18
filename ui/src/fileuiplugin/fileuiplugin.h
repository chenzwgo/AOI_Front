#ifndef FILEUI_PLUGIN_H
#define FILEUI_PLUGIN_H

#include <QObject>
#include "iuiplugin.h"
#include "inotify.h"
class  FileUiManager;


class FileUiPlugin: public QObject, IUIPlugin, INotify
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IUIPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_INOTIFY)
    Q_INTERFACES(IPlugin IUIPlugin INotify)
public:
    FileUiPlugin();
    virtual ~FileUiPlugin();
    virtual bool init();
    virtual bool uninit();
    virtual bool showWindow(QWidget *parent, QRect &rect,bool bPopFlag = false);
    virtual bool hideWindow();
    virtual bool queryInterface(const int iid_interface, void **ppInterface);

    virtual void fitSize(const QRect& rect);

    virtual bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);

    virtual void SetLanguage(QString strlanguage);

private:
    FileUiManager *m_pUIService;
};

#endif // ABOUTUIPLUGIN_H
