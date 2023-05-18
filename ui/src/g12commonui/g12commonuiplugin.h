#ifndef G12_COMMON_UIPLUGIN_H
#define G12_COMMON_UIPLUGIN_H

#include <QObject>
#include "iuiplugin.h"
#include "inotify.h"

class CDefectRecord;
class G12CommoneUIPlugin : public QObject, IUIPlugin, INotify
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IUIPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_INOTIFY)
    Q_INTERFACES(IPlugin IUIPlugin INotify)
public:
    G12CommoneUIPlugin();
    virtual ~G12CommoneUIPlugin();
    virtual bool init();
    virtual bool uninit();
    virtual bool showWindow(QWidget *parent, QRect &rect,bool bPopFlag = false);
    virtual bool hideWindow();
    virtual bool queryInterface(const int iid_interface, void **ppInterface);

    virtual void fitSize(const QRect& rect);

    virtual bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);

    virtual void SetLanguage(QString strlanguage);

private:
    CDefectRecord *m_pRecordView;
};

#endif // G12_COMMON_UIPLUGIN_H
