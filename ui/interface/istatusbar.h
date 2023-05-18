#ifndef _I_STATUSBAR_H
#define _I_STATUSBAR_H

#include <QWidget>
#include <QString>
#include <QList>
class StatusItem
{
public:
    QString strType;        //!< item ���  text icon widget
    QString strName;        //!<  item ������
    QString	strcontent;     //!<  ��ʾ����
    QWidget *pWidget;
    QString strAlign;       //!<  ��ʾʱ�Ķ��뷽ʽ

    StatusItem():
        strType(""),
        strName(""),
        strcontent(""),
        pWidget(NULL),
        strAlign("left")
    {

    }

    StatusItem (const StatusItem& info)
    {
        strType = info.strType;
        strName = info.strName;
        strcontent = info.strcontent;
        pWidget = info.pWidget;
        strAlign = info.strAlign;
    }

    bool operator == (StatusItem& info)
    {
        if (info.strType == strType && strName == info.strName)
        {
            return true;
        }
        return false;
    }
    
};
#define  PLUGIN_IID_NAME_ISTATUSBAR   "com.bj.Plugin.IStatusBar/1.0"
class IStatusBar
{   
public:
    enum
    {
        ISTATUSBAR_ID = 0x55
    };
    virtual ~IStatusBar(){}

    virtual unsigned int GetStatusItemCount() = 0;
    virtual void AddStatusItem(StatusItem* pItem) = 0;
    virtual void RemoveStatusItem(const QString& strItemName) = 0;
    virtual void AddStatusItem(const QString& strItemName,QWidget* pWidget) = 0;
    virtual void AddStretchToStatusbar() = 0;
    virtual void addStatusLine(const QString &strItemName) = 0;
};
QT_BEGIN_NAMESPACE
    Q_DECLARE_INTERFACE(IStatusBar,PLUGIN_IID_NAME_ISTATUSBAR);
QT_END_NAMESPACE
#endif // _I_STATUSBAR_H
