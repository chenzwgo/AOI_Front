#ifndef GRAPH_RECT_ITEM_H
#define GRAPH_RECT_ITEM_H

#include "shape/graphshapeitem.h"

class GraphicsRectItem : public GraphicsShapeItem
{
public:
    GraphicsRectItem(QGraphicsItem * parent=0 );
    GraphicsRectItem(const GraphicsRectItem &other );
    virtual int type () const {return EnShape_Rect;}
    virtual void resizeShape(NControlPos pos, const QPointF &point);
    virtual void addControls();    // 增加控制点
    virtual void updateControls(); // 更新控制点
    virtual QVariant shapeData(DataType type);  // 返回图形数据
    virtual GraphicsShapeItem *copy();          // 复制

    // 与运动点位交互接口
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

private:
    virtual QPainterPath getShapePath();       // 获取去形状轨迹
    virtual QPainterPath getOutSidePath();     // 获取外框轨迹
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    // 矩形
    QRectF rect() const {return m_rect;}
    // 矩形旋转
    void rotateRect(const QPointF &point);

private:
    QRectF m_rect;
};

#endif // GRAPHICS
