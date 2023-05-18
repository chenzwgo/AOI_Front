#ifndef ILAYOUTSERVICE_H
#define ILAYOUTSERVICE_H

#include <QWidget>
#define PLUGIN_IID_NAME_ILAYOUTSERVICE    "com.bj.Plugin.ILayoutService/1.0"
class ILayoutService
{
public:
    enum
    {
        IID_LAYOUTSERVICE = 0x002
    };
    virtual ~ILayoutService(){}
    virtual bool showPlugin(const QString strClsid) = 0;
    virtual QString getActivePluginClsid() = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ILayoutService,PLUGIN_IID_NAME_ILAYOUTSERVICE);
QT_END_NAMESPACE

#endif // ILAYOUTSERVICE_H
