#ifndef GRAPH_CIRCLE_ITEM_H
#define GRAPH_CIRCLE_ITEM_H

#include "shape/graphshapeitem.h"

class GraphicsCircleItem: public GraphicsShapeItem
{
public:
    GraphicsCircleItem(QGraphicsItem * parent=0 );
    GraphicsCircleItem(const GraphicsCircleItem &other);

    virtual int type () const {return EnShape_Circle;}
    virtual void resizeShape(NControlPos pos, const QPointF &point);
    virtual void addControls();    // 增加控制点
    virtual void updateControls(); // 更新控制点
    virtual bool isDrawFinished(); // 是否绘制完成
    virtual bool isShapeValid();   // 是否有效图形
    virtual QVariant shapeData(DataType type);  // 图形数据
    virtual GraphicsShapeItem *copy();          // 复制

    // 与运动点位交互接口
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    virtual QPainterPath getShapePath();             // 获取图形
    virtual QPainterPath getOutSidePath();           // 获取外边框
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QVector<QPointF> m_circlePoints;
    QRectF m_cirRect;   // 圆外框
    bool m_drawFinished;
    qreal m_radius;    // 半径
    QPointF m_center;    // 圆心
};

#endif // GRAPHICS
