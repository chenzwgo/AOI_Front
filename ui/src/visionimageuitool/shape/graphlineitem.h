#ifndef GRAPH_LINE_ITEM_H
#define GRAPH_LINE_ITEM_H

#include "shape/graphshapeitem.h"

class GraphicsLineItem : public GraphicsShapeItem
{
public:
    GraphicsLineItem(QGraphicsItem * parent=0 );
    GraphicsLineItem(const GraphicsLineItem &other);
    virtual int type () const {return EnShape_Line;}
    virtual void resizeShape(NControlPos pos, const QPointF &point);
    virtual void addControls();    // 增加控制点
    virtual void updateControls(); // 更新控制点
    virtual QVariant shapeData(DataType type);  // 图形数据
    virtual GraphicsShapeItem *copy();          // 复制

    // 与运动点位交互接口
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

private:
    virtual QPainterPath getShapePath();       // 获取去形状轨迹
    virtual QPainterPath getOutSidePath();     // 获取外框轨迹
    virtual QLineF getArrowLine(qreal &width); // 获取画箭头用的线
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    // 返回线
    QLineF line() const {return m_line;}

private:
    QLineF m_line;
};

#endif // GRAPHICS
