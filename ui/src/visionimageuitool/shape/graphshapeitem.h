#ifndef GRAPH_SHAPE_ITEM_H
#define GRAPH_SHAPE_ITEM_H

#include <QAbstractGraphicsShapeItem>
#include <QGraphicsSceneMouseEvent>
#include "controlsdef.h"
#include "graphcontrols.h"

class GraphicsShapeItem : public QAbstractGraphicsShapeItem
{
public:
    enum DataType{ D_BeginPoint=0, D_EndPoint, D_Center, D_Radius, D_Length, D_Angle, D_Width, D_Height };

    GraphicsShapeItem(QGraphicsItem * parent=0 );
    virtual int type () const = 0;              // �������ʵ��
    virtual void resizeShape(NControlPos pos, const QPointF &point); // �������ʵ��(pointΪ�����е�)
    QPointF pointAt(NControlPos pos);           // ��ȡ����Ϣ(������)
    QPointF lastPointAt(NControlPos pos);       // ��ȡ��һ�εĵ���Ϣ(������)
    bool isResized() const {return m_bResized;} // �Ƿ�ı���״(ֻ���ƶ�����false,����ֱ�ߴ�������)
    void moveOffset(qreal dx, qreal dy);        // ƫ��
    int ctlsCount();                            // ���Ƶ����
    NControlPos ctrlPos(int index);             // ͨ�������õ����Ƶ�λ��
    virtual void removeCtrl(NControlPos pos);   // �Ƴ����Ƶ�(���߶�ʹ��)
    virtual bool isDrawFinished();              // �ж��Ƿ���ƽ���
    virtual bool isShapeValid();                // ����ͼ���Ƿ���Ч(����ֱ�߳���Ϊ0Ϊ��Ч)
    bool onControls(const QPointF &point);      // ���Ƿ����ڿ��Ƶ㷶Χ��
    virtual void showArrow(bool show){m_bShowArrow = show;}// ��ʾ��ͷ
    virtual QVariant shapeData(DataType type);  // ����ͼ������
    virtual GraphicsShapeItem *copy() = 0;      // ����
    virtual void updateControls() = 0; // ���¿��Ƶ�     // <�������ʵ��>

    // ���˶���λ�����ӿ�
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

    virtual void setLineWidth(double iWidth);
    virtual void setLineColor(QColor color);

    GraphicsShapeItem& operator=(const GraphicsShapeItem& other);

    void setControlWidth(qreal width = ControlMaxWidth);                  // ���ÿ��Ƶ��С
    void setMoveEnable(bool bEnable);                  // ����Item�Ƿ���ƶ�
protected:
    virtual QPainterPath getShapePath() = 0;         // ��ȡͼ�� <�������ʵ��>
    virtual QPainterPath getOutSidePath();           // ��ȡ��߿�(����shape()��,�����ͣ��ʽ�ж�)
    virtual QLineF getArrowLine(qreal &width);       // ��ȡ����ͷ�õ��� <�������ʵ��>
    virtual void addControls() = 0;  // ���ӿ��Ƶ�       // <�������ʵ��>
    virtual QPainterPath createCtlsPath();

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);  // �Ҽ��˵�    �������ʵ��
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QPainterPath shape() const;

    virtual void updateShape();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent * event );
    virtual QRectF boundingRect() const;

    // ��λ��(ȥ����ͷ)
    QPointF simplifyPoint(const QPointF &point);

protected:
    GraphicsControls m_controls;
    QPolygonF m_arrowHead;
    QPainterPath m_shapePath;    // ͼ����ʽ,paint()��ʹ��
    QPainterPath m_outsidePath;  // ���켣(����shape��)
    QPainterPath m_cltsRectPath; // ���Ƶ����
    QMap<DataType,QVariant> m_shapeData; // ͼ������
    double  m_dLineWidth; //�߿�
    QColor  m_color;

private:
    void updateCursor();  // ���������״
    void updateCursor(NControlPos pos);

private:
    bool m_bResized; // �Ǹı仹���ƶ�
    bool m_bShowArrow; //��ʾ��ͷ
    bool m_bIsEnableMove;
};

#endif // GRAPHICS
