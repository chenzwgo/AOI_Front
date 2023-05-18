#ifndef GRAPH_CONTROLS_H
#define GRAPH_CONTROLS_H

#include "controlsdef.h"
#include <QRectF>
#include <QDebug>

class GraphicsControls
{
public:
    GraphicsControls(): m_curPos(None),m_pItem(NULL), m_iCtrlWidth(ControlMaxWidth), m_bIsEnable(true){};
    void setParent(QGraphicsItem *pItem);
    void addControls(NControlPos pos);     // 增加控制点
    void setControlWidth(qreal width=ControlMaxWidth);                  // 设置控制点正方形变长
    qreal getControlWidth();                  // 设置控制点正方形变长
    void updateControls(const QMap<NControlPos, QPointF> &points); // 批量更新控制点
    void updateControl(NControlPos pos, const QPointF &point);     // 更新控制点
    NControlPos hitControls(const QPointF &point);                 // 点击的控制点位置
    QRectF controls(NControlPos pos) const;                        // 得到控制点矩形
    QPointF controlsPos(NControlPos pos) const;                     // 得到控制点在场景中的坐标
    QPointF controlsLastPos(NControlPos pos) const;                // 得到控制点上一次在场景中的坐标
    void updateControlsLastPos();                                  // 更新控制点上一次在场景中的坐标
    void setPressedControl(NControlPos pos) { m_curPos = pos;}     // 设置当前下压控制点
    NControlPos pressedControl() {return m_curPos;}                // 获取当前下压控制点
    bool contains(NControlPos pos);                                // 包含位置pos
    int count() { return m_ctlRects.size(); }                      // 返回控制点个数
    NControlPos ctrlPosByIndex(int index);                         // 通过索引得到控制点枚举位置
    void removeCtrl(NControlPos pos);                              // 移除控制点
    void clearCtrls();                                             // 清空控制点
    void setCtrlEnable(bool enable);
private:
    QMap<NControlPos, QRectF> m_ctlRects;     // 控制点矩形
    QMap<NControlPos, QPointF> m_ctlLastPos;  // 控制点矩形
    NControlPos m_curPos;
    QGraphicsItem *m_pItem;
    int         m_iCtrlWidth;                 //控制点大小
    bool        m_bIsEnable;                  //是否使用控制点
};

#endif // GRAPHICS
