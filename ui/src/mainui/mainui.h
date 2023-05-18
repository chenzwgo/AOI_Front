#ifndef _RECORDVIEWUI_PLUGIN_H_
#define _RECORDVIEWUI_PLUGIN_H_

#include <QObject>
#include "iuiplugin.h"
#include "inotify.h"

/*新ui修改*/
class MainFrame;

class Mainui: public QObject, IUIPlugin, INotify
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IUIPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_INOTIFY)
    Q_INTERFACES(IPlugin IUIPlugin INotify)

public:
    Mainui();
    virtual ~Mainui();

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
	MainFrame *m_pMainFrame;
	
	bool m_bInit;
};

#endif // _RECORDVIEWUI_PLUGIN_H_

