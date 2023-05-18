#ifndef GRAPH_CIRCLECENTER_ITEM_H
#define GRAPH_CIRCLECENTER_ITEM_H

#include "shape/graphshapeitem.h"

class GraphicsCircleCenterItem: public GraphicsShapeItem
{
public:
    GraphicsCircleCenterItem(QGraphicsItem * parent=0 );
    GraphicsCircleCenterItem(const GraphicsCircleCenterItem &other);

    virtual int type () const {return EnShape_Circle_Center;}
    virtual void resizeShape(NControlPos pos, const QPointF &point);
    virtual void addControls();    // 增加控制点
    virtual void updateControls(); // 更新控制点
    virtual QVariant shapeData(DataType type);  // 图形数据
    virtual GraphicsShapeItem *copy();          // 复制

    // 与运动点位交互接口
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

private:
    virtual QPainterPath getShapePath();             // 获取图形
    virtual QPainterPath getOutSidePath();           // 获取外边框
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QVector<QPointF> m_circlePoints;
    QRectF m_cirRect;   // 圆外框
    qreal m_radius;    // 半径
};

#endif // GRAPHICS
