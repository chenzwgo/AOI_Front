#ifndef RecipesUIPlugin_H
#define RecipesUIPlugin_H

#include <QObject>
#include "iuiplugin.h"
#include "inotify.h"

class RecipesUI;
class RecipesUIPlugin: public QObject, IUIPlugin, INotify
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_IUIPLUGIN)
    Q_PLUGIN_METADATA(IID PLUGIN_IID_NAME_INOTIFY)
    Q_INTERFACES(IPlugin IUIPlugin INotify)
public:
    RecipesUIPlugin();
    virtual ~RecipesUIPlugin();
    virtual bool init();
    virtual bool uninit();
    virtual bool showWindow(QWidget *parent, QRect &rect,bool bPopFlag = false);
    virtual bool hideWindow();
    virtual bool queryInterface(const int iid_interface, void **ppInterface);

    virtual void fitSize(const QRect& rect);

    virtual bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);

    virtual void SetLanguage(QString strlanguage);



private:
    RecipesUI *m_pRecipesUI;
};

#endif // RecipesUIPlugin_H
