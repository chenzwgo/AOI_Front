#ifndef IUIPLUGIN_H
#define IUIPLUGIN_H
#include <QWidget>
#include "iplugin.h"
#define PLUGIN_IID_NAME_IUIPLUGIN    "com.bj.Plugin.IUIPlugin/1.0"
class IUIPlugin :public IPlugin
{
public:
    enum
    {
        GET_MAIN_WIDGET_OBJECT=0x110,
		GET_DEBUG_VIEW_WIDGET_OBJECT
    };
    virtual ~IUIPlugin(){}

    virtual bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false) = 0;
    virtual bool hideWindow() = 0;
    virtual void fitSize(const QRect& rect) = 0;
    virtual void SetLanguage(QString strlanguage) = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IUIPlugin, PLUGIN_IID_NAME_IUIPLUGIN);
QT_END_NAMESPACE

#endif // IUIPLUGIN_H
