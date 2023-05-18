#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QWidget>
#define PLUGIN_IID_NAME_IPLUGIN    "com.bj.Plugin.IPlugin/1.0"
class IPlugin
{
public:
    virtual ~IPlugin(){}
    virtual bool init() = 0;
    virtual bool uninit() = 0;
    virtual bool queryInterface(const int iid_interface, void **ppInterface) = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IPlugin, PLUGIN_IID_NAME_IPLUGIN);
QT_END_NAMESPACE

#endif // IPLUGIN_H
