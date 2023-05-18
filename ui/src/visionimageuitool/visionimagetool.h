#ifndef _VISIONIMAGETOOL_PLUGIN_H_
#define _VISIONIMAGETOOL_PLUGIN_H_

#include <QObject>
#include "iuiplugin.h"
#include "inotify.h"

class CVisionimagetoolDlg;
class CVisionimagetool: public QObject, IUIPlugin, INotify
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IUIPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_INOTIFY)
    Q_INTERFACES(IPlugin IUIPlugin INotify)

public:
    CVisionimagetool();
    virtual ~CVisionimagetool();

    virtual bool init();
    virtual bool uninit();
    virtual bool showWindow(QWidget *parent, QRect &rect,bool bPopFlag = false);
    virtual bool hideWindow();
    virtual bool queryInterface(const int iid_interface, void **ppInterface);

    virtual void fitSize(const QRect& rect);

    virtual bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    virtual void SetLanguage(QString strlanguage);

private:
    CVisionimagetoolDlg *m_pVisionimagetoolDlg;
};

#endif // _VISIONIMAGETOOL_PLUGIN_H_

