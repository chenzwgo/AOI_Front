#ifndef GRAPH_ARC_CENTER_ITEM_H
#define GRAPH_ARC_CENTER_ITEM_H

#include "shape/graphshapeitem.h"

class GraphicsArcCenterItem: public GraphicsShapeItem
{
public:
    GraphicsArcCenterItem(QGraphicsItem * parent=0 );
    GraphicsArcCenterItem(const GraphicsArcCenterItem &other);

    virtual int type () const {return EnShape_Arc_Center;}
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
    virtual QPainterPath getShapePath();       // 获取去形状轨迹
    virtual QPainterPath getOutSidePath();     // 获取外框轨迹
    virtual QLineF getArrowLine(qreal &width); // 获取画箭头用的线
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    QPainterPath getArcPath(const QPointF &center, qreal radius, qreal startAngle, qreal sweepLength);
    qreal getSweepLen(const QPointF &center, const QPointF &p1, const QPointF &p2);

private:
    QVector<QPointF> m_arcPoints;
    bool m_drawFinished;
    qreal m_sweepLen;  // 弧度
    qreal m_radius;    // 半径
};

#endif // GRAPHICS
