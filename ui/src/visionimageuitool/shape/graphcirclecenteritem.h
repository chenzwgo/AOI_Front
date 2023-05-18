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
    virtual void addControls();    // ���ӿ��Ƶ�
    virtual void updateControls(); // ���¿��Ƶ�
    virtual QVariant shapeData(DataType type);  // ͼ������
    virtual GraphicsShapeItem *copy();          // ����

    // ���˶���λ�����ӿ�
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

private:
    virtual QPainterPath getShapePath();             // ��ȡͼ��
    virtual QPainterPath getOutSidePath();           // ��ȡ��߿�
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    QVector<QPointF> m_circlePoints;
    QRectF m_cirRect;   // Բ���
    qreal m_radius;    // �뾶
};

#endif // GRAPHICS
