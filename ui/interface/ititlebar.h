#ifndef _I_TITLEBAR_H
#define _I_TITLEBAR_H

#include <QWidget>
#include <QString>
#define PLUGIN_IID_NAME_ITITLEBAR   "com.bj.Plugin.ITitleBar/1.0"
class ITitleBar
{   
public:
    enum
    {
        ITITLEBAR_ID = 0x88
    };
    virtual ~ITitleBar(){}

    virtual void setTitleText(const QString strPluginName) = 0;    //!<  设置titleText
    virtual void setTitleIcon(const QString strIconPath) = 0;      //!<  设置titleIcon
    virtual void showMaxRestoreBTN(bool bShow) = 0;            //!<  显示最大按钮
    virtual void showMinimizeBTN(bool bShow) = 0;              //!<  显示最小化按钮
    virtual void showCloseBTN(bool bShow) = 0;                 //!<  显示关闭按钮
    virtual void showIcon(bool bShow) = 0;                     //!<  显示图标
};
QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(ITitleBar, PLUGIN_IID_NAME_ITITLEBAR );
QT_END_NAMESPACE
#endif // _I_TITLEBAR_H
