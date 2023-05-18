#ifndef _DEVCLIENT_UI_PLUGIN_H_
#define _DEVCLIENT_UI_PLUGIN_H_

#include <QObject>
#include "iuiplugin.h"
#include "inotify.h"

/*新ui修改*/
class Socketlistui;
class CSocketClientListCfg;

class Devclientui: public QObject, IUIPlugin, INotify
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IUIPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_INOTIFY)
    Q_INTERFACES(IPlugin IUIPlugin INotify)

public:
    Devclientui();
    virtual ~Devclientui();

    virtual bool init();
    virtual bool uninit();
    virtual bool showWindow(QWidget *parent, QRect &rect,bool bPopFlag = false);
    virtual bool hideWindow();
    virtual bool queryInterface(const int iid_interface, void **ppInterface);

    virtual void fitSize(const QRect& rect);

    virtual bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    virtual void SetLanguage(QString strlanguage);

private:
    /*新ui修改*/
    Socketlistui *m_pSocketlistui;
    CSocketClientListCfg* m_CSocketClientListCfg;
	
	bool m_bInit;
};

#endif // _DEVCLIENT_UI_PLUGIN_H_

