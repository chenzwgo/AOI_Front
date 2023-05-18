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
    virtual void addControls();    // ���ӿ��Ƶ�
    virtual void updateControls(); // ���¿��Ƶ�
    virtual bool isDrawFinished(); // �Ƿ�������
    virtual bool isShapeValid();   // �Ƿ���Чͼ��
    virtual QVariant shapeData(DataType type);  // ͼ������
    virtual GraphicsShapeItem *copy();          // ����

    // ���˶���λ�����ӿ�
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    virtual QPainterPath getShapePath();             // ��ȡͼ��
    virtual QPainterPath getOutSidePath();           // ��ȡ��߿�
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QVector<QPointF> m_circlePoints;
    QRectF m_cirRect;   // Բ���
    bool m_drawFinished;
    qreal m_radius;    // �뾶
    QPointF m_center;    // Բ��
};

#endif // GRAPHICS
