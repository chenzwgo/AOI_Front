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
    virtual void addControls();    // ���ӿ��Ƶ�
    virtual void updateControls(); // ���¿��Ƶ�
    virtual QVariant shapeData(DataType type);  // ����ͼ������
    virtual GraphicsShapeItem *copy();          // ����

    // ���˶���λ�����ӿ�
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

private:
    virtual QPainterPath getShapePath();       // ��ȡȥ��״�켣
    virtual QPainterPath getOutSidePath();     // ��ȡ���켣
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    // ����
    QRectF rect() const {return m_rect;}
    // ������ת
    void rotateRect(const QPointF &point);

private:
    QRectF m_rect;
};

#endif // GRAPHICS
