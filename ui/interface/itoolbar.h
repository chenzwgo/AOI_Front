#ifndef _I_TOOLBAR_H
#define _I_TOOLBAR_H

#include<QString>
#define PLUGIN_IID_NAME_ITOOLBAR        "com.bj.Plugin.IToolBar/1.0"
class IToolBar                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
{   
public:
    enum
    {
        ITOOLBAR_ID = 0x77
    };
    virtual ~IToolBar(){}

    virtual void EnableTooltem(const QString& strPluginName) =0;
    virtual void DisableToolItem(const QString& strPluginName) =0;
    virtual void EnableAllToolItem() =0;
    virtual void DisableAllToolItem() =0;
    virtual void SetTooltemChecked(const QString& strObjectName) = 0;
    virtual void SetTooltemUnChecked(const QString& strObjectName) = 0;
};
QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(IToolBar,
        PLUGIN_IID_NAME_ITOOLBAR);
QT_END_NAMESPACE
#endif // _I_TOOLBAR_H
