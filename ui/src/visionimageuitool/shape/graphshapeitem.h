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
    virtual int type () const = 0;              // 子类必须实现
    virtual void resizeShape(NControlPos pos, const QPointF &point); // 子类必须实现(point为场景中点)
    QPointF pointAt(NControlPos pos);           // 获取点信息(场景中)
    QPointF lastPointAt(NControlPos pos);       // 获取上一次的点信息(场景中)
    bool isResized() const {return m_bResized;} // 是否改变形状(只是移动返回false,用于直线磁性吸附)
    void moveOffset(qreal dx, qreal dy);        // 偏移
    int ctlsCount();                            // 控制点个数
    NControlPos ctrlPos(int index);             // 通过索引得到控制点位置
    virtual void removeCtrl(NControlPos pos);   // 移除控制点(多线段使用)
    virtual bool isDrawFinished();              // 判断是否绘制结束
    virtual bool isShapeValid();                // 绘制图形是否有效(比如直线长度为0为无效)
    bool onControls(const QPointF &point);      // 点是否落在控制点范围内
    virtual void showArrow(bool show){m_bShowArrow = show;}// 显示箭头
    virtual QVariant shapeData(DataType type);  // 返回图形数据
    virtual GraphicsShapeItem *copy() = 0;      // 复制
    virtual void updateControls() = 0; // 更新控制点     // <子类必须实现>

    // 与运动点位交互接口
    virtual bool setShapeData(const QVariant &data);
    virtual QVariant shapeData();

    virtual void setLineWidth(double iWidth);
    virtual void setLineColor(QColor color);

    GraphicsShapeItem& operator=(const GraphicsShapeItem& other);

    void setControlWidth(qreal width = ControlMaxWidth);                  // 设置控制点大小
    void setMoveEnable(bool bEnable);                  // 设置Item是否可移动
protected:
    virtual QPainterPath getShapePath() = 0;         // 获取图形 <子类必须实现>
    virtual QPainterPath getOutSidePath();           // 获取外边框(用于shape()中,鼠标悬停样式判断)
    virtual QLineF getArrowLine(qreal &width);       // 获取画箭头用的线 <子类必须实现>
    virtual void addControls() = 0;  // 增加控制点       // <子类必须实现>
    virtual QPainterPath createCtlsPath();

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);  // 右键菜单    子类必须实现
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QPainterPath shape() const;

    virtual void updateShape();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent * event );
    virtual QRectF boundingRect() const;

    // 点位简化(去掉零头)
    QPointF simplifyPoint(const QPointF &point);

protected:
    GraphicsControls m_controls;
    QPolygonF m_arrowHead;
    QPainterPath m_shapePath;    // 图形样式,paint()中使用
    QPainterPath m_outsidePath;  // 外框轨迹(用在shape中)
    QPainterPath m_cltsRectPath; // 控制点矩形
    QMap<DataType,QVariant> m_shapeData; // 图形数据
    double  m_dLineWidth; //线宽
    QColor  m_color;

private:
    void updateCursor();  // 更新鼠标形状
    void updateCursor(NControlPos pos);

private:
    bool m_bResized; // 是改变还是移动
    bool m_bShowArrow; //显示箭头
    bool m_bIsEnableMove;
};

#endif // GRAPHICS
