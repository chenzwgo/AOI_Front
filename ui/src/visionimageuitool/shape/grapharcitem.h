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

private:
    QVector<QPointF> m_arcPoints;
    qreal m_sweepLen;  // ����
    qreal m_radius;    // �뾶
    QPointF m_center;    // Բ��
    bool m_drawFinished;
};

#endif // GRAPHICS
