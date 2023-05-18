#include "shape/graphcirclecenteritem.h"
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "pointalgorithm.h"

GraphicsCircleCenterItem::GraphicsCircleCenterItem(QGraphicsItem *parent)
    :GraphicsShapeItem(parent)
    ,m_radius(0)
{
    addControls();
}

GraphicsCircleCenterItem::GraphicsCircleCenterItem( const GraphicsCircleCenterItem &other )
{
    *this = other;
    m_circlePoints = other.m_circlePoints;
    m_cirRect = other.m_cirRect;
    m_radius = other.m_radius;
}

void GraphicsCircleCenterItem::addControls()
{
    m_controls.addControls(CircleCenter);
    m_controls.addControls(CircleP);
    m_circlePoints.append(QPointF(0,0));
    m_circlePoints.append(QPointF(0,0));
}

void GraphicsCircleCenterItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

void GraphicsCircleCenterItem::updateControls()
{
    for (int i=0; i<m_controls.count(); i++)
    {
        m_controls.updateControl(CircleCenter+i, m_circlePoints.at(i));
    }
}

void GraphicsCircleCenterItem::resizeShape( NControlPos pos, const QPointF &point )
{
    QPointF pointSp = simplifyPoint(point);
    QPointF localP = mapFromScene(pointSp);
    if (m_controls.contains(pos))
    {
        m_circlePoints[pos-CircleCenter] = localP;
    }

    GraphicsShapeItem::resizeShape(pos,point);
}

QPainterPath GraphicsCircleCenterItem::getShapePath()
{
    m_radius = PMethod::distanceP2P(m_circlePoints[0],m_circlePoints[1]);
    m_cirRect = QRectF(0,0,m_radius*2,m_radius*2);
    m_cirRect.moveCenter(m_circlePoints[0]);
    QPainterPath path;
    path.addEllipse(m_cirRect);
    return path;
}

QPainterPath GraphicsCircleCenterItem::getOutSidePath()
{
    return getShapePath();
}

QVariant GraphicsCircleCenterItem::shapeData( DataType type )
{
    QVariant var;
    switch (type)
    {
    case D_BeginPoint:
        var = mapToScene(m_circlePoints[0]);
        break;
    case D_EndPoint:
        var = mapToScene(m_circlePoints[1]);
        break;
    case D_Angle:
        var = 360;
        break;
    case D_Center:
        var = mapToScene(m_circlePoints[0]);
        break;
    case D_Radius:
        var = m_radius;
        break;
    case D_Length:
        var = m_radius*PI*2;
        break;
    default:
        break;
    }

    return var;
}

GraphicsShapeItem * GraphicsCircleCenterItem::copy()
{
    return new GraphicsCircleCenterItem(*this);
}


bool GraphicsCircleCenterItem::setShapeData( const QVariant &data )
{
    QList<QVariant> points = data.toList();
    if (points.size() < 3)
    {
        return false;
    }

    m_circlePoints.clear();
    if (points.size() > 3)
    {
        QPointF localP1 = mapFromScene(points.at(2).toPointF());
        QPointF localP2 = mapFromScene(points.at(3).toPointF());
        m_circlePoints<<localP1<<localP2;

    }
    else
    {
        QPointF localP1 = mapFromScene(points.at(2).toPointF());
        m_radius = points.at(1).toFloat();
        QPointF localP2(localP1.x()+m_radius, localP1.y());
        m_circlePoints<<localP1<<localP2;
    }
    m_controls.clearCtrls();
    m_controls.addControls(CircleCenter);
    m_controls.addControls(CircleP);
    updateShape();
    return true;
}

QVariant GraphicsCircleCenterItem::shapeData()
{
    QList<QVariant> points;
    points <<type()<< m_radius;
    for (int i=0; i<m_circlePoints.size(); i++)
    {
        points<<mapToScene(m_circlePoints.at(i));
    }
    return points;
}