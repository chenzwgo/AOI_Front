#ifndef GRAPH_POLY_ITEM_H
#define GRAPH_POLY_ITEM_H

#include "shape/graphshapeitem.h"

class GraphicsPolyItem : public GraphicsShapeItem
{
public:
    GraphicsPolyItem(QGraphicsItem * parent=0 );
    GraphicsPolyItem(const GraphicsPolyItem &other );
    virtual int type () const {return EnShape_Polygon;}
    virtual void resizeShape(NControlPos pos, const QPointF &point);
    virtual void addControls();    // ���ӿ��Ƶ�
    virtual void updateControls(); // ���¿��Ƶ�
    virtual void removeCtrl(NControlPos pos); // �Ƴ����Ƶ�
    virtual bool isDrawFinished(); // �Ƿ�������
    virtual bool isShapeValid();   // �Ƿ���Чͼ��
    virtual QVariant shapeData(DataType type);  // ����ͼ������
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

private:
    QPolygonF m_poly;
    bool m_drawFinished;
};

#endif // GRAPHICS
