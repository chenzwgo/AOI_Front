#ifndef _USERMANAGEMENTUI_PLUGIN_H_
#define _USERMANAGEMENTUI_PLUGIN_H_

#include <QObject>
#include "iuiplugin.h"
#include "inotify.h"

class CUsermanagementuiDlg;
class CUsermanagementui: public QObject, IUIPlugin, INotify
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IUIPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_INOTIFY)
    Q_INTERFACES(IPlugin IUIPlugin INotify)

public:
    CUsermanagementui();
    virtual ~CUsermanagementui();

    virtual bool init();
    virtual bool uninit();
    virtual bool showWindow(QWidget *parent, QRect &rect,bool bPopFlag = false);
    virtual bool hideWindow();
    virtual bool queryInterface(const int iid_interface, void **ppInterface);

    virtual void fitSize(const QRect& rect);

    virtual bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    virtual void SetLanguage(QString strlanguage);


private:
    CUsermanagementuiDlg *m_pUsermanagementuiDlg;
};

#endif // _USERMANAGEMENTUI_PLUGIN_H_

