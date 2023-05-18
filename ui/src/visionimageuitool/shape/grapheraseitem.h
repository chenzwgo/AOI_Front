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
    virtual void addControls();    // ���ӿ��Ƶ�
    virtual void updateControls(); // ���¿��Ƶ�
    virtual void removeCtrl(NControlPos pos); // �Ƴ����Ƶ�
    virtual bool isDrawFinished(); // �Ƿ�������
    virtual bool isShapeValid();   // �Ƿ���Чͼ��
    virtual QVariant shapeData(DataType type);  // ����ͼ������
    virtual GraphicsShapeItem *copy();          // ����
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    // ���˶���λ�����ӿ�
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

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
