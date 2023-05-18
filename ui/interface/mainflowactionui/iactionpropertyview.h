#ifndef IACTIONPROPERTYVIEW_H
#define IACTIONPROPERTYVIEW_H
#include "mainflowaction/workflowactiondefine.h"

#include <QStringList>

class IActionPropertyView
{
public:
    virtual bool init() = 0;   // 界面初始化
    virtual bool updateView(WORK_ACTION* data) = 0;   // 数据更新到界面
    virtual bool updateData(WORK_ACTION* data) = 0;   // 从界面读取数据到内存
    virtual void onModifyBegin() = 0;                 // 主框架通知修改按钮被点击
    virtual void onModifyConfirm() = 0;               // 主框架通知确认按钮被点击
    virtual void onModifyCancel() = 0;                // 主框架通知取消按钮被点击
    virtual QStringList getHeaderLabels() = 0 ;   // 界面初始化
};

#endif // IACTIONPROPERTYVIEW_H

