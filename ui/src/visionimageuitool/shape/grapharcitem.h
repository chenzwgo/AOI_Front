#ifndef GRAPH_ARC_ITEM_H
#define GRAPH_ARC_ITEM_H

#include "shape/graphshapeitem.h"

class GraphicsArcItem: public GraphicsShapeItem
{
public:
    GraphicsArcItem(QGraphicsItem * parent=0 );
    GraphicsArcItem(const GraphicsArcItem &other);

    virtual int type () const {return EnShape_Arc;}
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

private:
    QVector<QPointF> m_arcPoints;
    qreal m_sweepLen;  // 弧度
    qreal m_radius;    // 半径
    QPointF m_center;    // 圆心
    bool m_drawFinished;
};

#endif // GRAPHICS
