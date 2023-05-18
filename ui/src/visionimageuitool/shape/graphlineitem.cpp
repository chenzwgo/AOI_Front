#include "shape/graphlineitem.h"
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "pointalgorithm.h"

GraphicsLineItem::GraphicsLineItem(QGraphicsItem *parent)
    :GraphicsShapeItem(parent)
{
    m_line = QLineF(0,0,0,0);
    addControls();
}

GraphicsLineItem::GraphicsLineItem( const GraphicsLineItem &other )
{
    *this = other;
    m_line = other.m_line;
}

void GraphicsLineItem::addControls()
{
    m_controls.addControls(LineP1);
    m_controls.addControls(LineP2);
}

void GraphicsLineItem::updateControls()
{
    // 设置控制点大小
    qreal lineLen = line().length();
    qreal width = qMin(lineLen/3,qreal(ControlMaxWidth));
    if (width<m_controls.getControlWidth()) width = m_controls.getControlWidth();
    m_controls.setControlWidth(width);

    // 更新控制点位置
    m_controls.updateControl(LineP1,line().p1());
    m_controls.updateControl(LineP2,line().p2());
}

void GraphicsLineItem::resizeShape( NControlPos pos, const QPointF &point )
{
    QPointF pointSp = simplifyPoint(point);
    QPointF localP = mapFromScene(pointSp);
    switch (pos)
    {
    case LineP1:
        {
            m_line.setP1(localP);
            break;
        }
    case LineP2:
        {
            m_line.setP2(localP);
            break;
        }
    default:
        break;
    }

    GraphicsShapeItem::resizeShape(pos,point);
}

QPainterPath GraphicsLineItem::getOutSidePath()
{
    qreal lineLen = line().length();
    qreal width = qMin(lineLen/6,qreal(2));
    if (width<0.1) width = 0.1;

    QPainterPath path;
    QPainterPathStroker ps;
    ps.setWidth(width);
    path = ps.createStroke(getShapePath());
    return path;
}

QLineF GraphicsLineItem::getArrowLine( qreal &width )
{
    qreal lineLen = line().length();
    width = qMin(lineLen/4, qreal(4));
    return line();
}

QPainterPath GraphicsLineItem::getShapePath()
{
    QPainterPath path;
    path.moveTo(line().p1());
    path.lineTo(line().p2());
    return path;
}

void GraphicsLineItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

QVariant GraphicsLineItem::shapeData( DataType type )
{
    QVariant var;
    switch (type)
    {
    case D_BeginPoint:
        var = mapToScene(line().p1());
        break;
    case D_EndPoint:
        var = mapToScene(line().p2());
        break;
    case D_Angle:
        var = line().angle();
        break;
    case D_Length:
        var = line().length();
        break;
    default:
        break;
    }

    return var;
}

GraphicsShapeItem * GraphicsLineItem::copy()
{
    return new GraphicsLineItem(*this);
}

bool GraphicsLineItem::setShapeData( const QVariant &data )
{
    QList<QVariant> points = data.toList();
    if (points.size() != 2)
    {
        return false;
    }
    
    QPointF localP1 = mapFromScene(points.at(0).toPointF());
    QPointF localP2 = mapFromScene(points.at(1).toPointF());
    m_line.setLine(localP1.x(),localP1.y(),localP2.x(), localP2.y());
    addControls();
    updateShape();
    return true;
}

QVariant GraphicsLineItem::shapeData()
{
    QList<QVariant> points;
    points<<mapToScene(m_line.p1())<<mapToScene(m_line.p2());
    return points;
}
