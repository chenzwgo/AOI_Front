#ifndef GRAPH_ERASE_ITEM_H
#define GRAPH_ERASE_ITEM_H

#include "shape/graphshapeitem.h"

class GraphicsEraseItem : public GraphicsShapeItem
{
public:
    GraphicsEraseItem(QGraphicsItem * parent=0 );
    GraphicsEraseItem(const GraphicsEraseItem &other );
    virtual int type () const {return EnShape_Polygon;}
    virtual void resizeShape(NControlPos pos, const QPointF &point);
    virtual void addControls();    // 增加控制点
    virtual void updateControls(); // 更新控制点
    virtual void removeCtrl(NControlPos pos); // 移除控制点
    virtual bool isDrawFinished(); // 是否绘制完成
    virtual bool isShapeValid();   // 是否有效图形
    virtual QVariant shapeData(DataType type);  // 返回图形数据
    virtual GraphicsShapeItem *copy();          // 复制
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    // 与运动点位交互接口
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    virtual QPainterPath getShapePath();       // 获取去形状轨迹
    virtual QPainterPath getOutSidePath();     // 获取外框轨迹
    virtual QLineF getArrowLine(qreal &width); // 获取画箭头用的线

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QPolygonF m_poly;
    bool m_drawFinished;
};

#endif // GRAPHICS
