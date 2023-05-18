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
    virtual void addControls();    // ���ӿ��Ƶ�
    virtual void updateControls(); // ���¿��Ƶ�
    virtual QVariant shapeData(DataType type);  // ͼ������
    virtual GraphicsShapeItem *copy();          // ����

    // ���˶���λ�����ӿ�
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

private:
    virtual QPainterPath getShapePath();       // ��ȡȥ��״�켣
    virtual QPainterPath getOutSidePath();     // ��ȡ���켣
    virtual QLineF getArrowLine(qreal &width); // ��ȡ����ͷ�õ���
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    // ������
    QLineF line() const {return m_line;}

private:
    QLineF m_line;
};

#endif // GRAPHICS
