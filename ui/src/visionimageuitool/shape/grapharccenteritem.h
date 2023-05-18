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
    virtual QPainterPath getShapePath();       // ��ȡȥ��״�켣
    virtual QPainterPath getOutSidePath();     // ��ȡ���켣
    virtual QLineF getArrowLine(qreal &width); // ��ȡ����ͷ�õ���
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    QPainterPath getArcPath(const QPointF &center, qreal radius, qreal startAngle, qreal sweepLength);
    qreal getSweepLen(const QPointF &center, const QPointF &p1, const QPointF &p2);

private:
    QVector<QPointF> m_arcPoints;
    bool m_drawFinished;
    qreal m_sweepLen;  // ����
    qreal m_radius;    // �뾶
};

#endif // GRAPHICS
