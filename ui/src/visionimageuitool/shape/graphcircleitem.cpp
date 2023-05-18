#include "shape/graphcircleitem.h"
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "pointalgorithm.h"

GraphicsCircleItem::GraphicsCircleItem(QGraphicsItem *parent)
    :GraphicsShapeItem(parent)
    ,m_drawFinished(false)
    ,m_radius(0)
{
    addControls();
}

GraphicsCircleItem::GraphicsCircleItem( const GraphicsCircleItem &other )
{
    *this = other;
    m_circlePoints = other.m_circlePoints;
    m_cirRect      = other.m_cirRect     ;
    m_drawFinished = other.m_drawFinished;
    m_radius       = other.m_radius      ;
    m_center       = other.m_center      ;
}

void GraphicsCircleItem::addControls()
{
    m_controls.addControls(CircleP1);
    m_controls.addControls(CircleP2);
    m_circlePoints.append(QPointF(0,0));
    m_circlePoints.append(QPointF(0,0));
}

void GraphicsCircleItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

void GraphicsCircleItem::updateControls()
{
    for (int i=0; i<m_controls.count(); i++)
    {
        m_controls.updateControl(CircleP1+i, m_circlePoints.at(i));
    }
}

void GraphicsCircleItem::resizeShape( NControlPos pos, const QPointF &point )
{
    QPointF pointSp = simplifyPoint(point);
    QPointF localP = mapFromScene(pointSp);
    if (m_controls.contains(pos))
    {
        QVector<QPointF> points = m_circlePoints;
        points[pos-CircleP1] = localP;
        if (points.size() == 3)
        {
            qreal radius = -1;
            QPointF center = PMethod::tCircle(points[0],points[1], points[2], radius);
            if (center.isNull())
            {
                return;
            }
        }
        
        m_circlePoints = points;
    }

    GraphicsShapeItem::resizeShape(pos,point);
}

bool GraphicsCircleItem::isDrawFinished()
{
    return m_drawFinished;
}

void GraphicsCircleItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
    if (m_drawFinished)
    {
        return GraphicsShapeItem::mouseReleaseEvent(event);
    }
    
    if (m_controls.count() == CircleP3-CircleP1+1)
    {
        m_drawFinished = true;
    }
    else
    {
        int index = CircleP1+m_controls.count();
        m_controls.addControls(index);
        m_circlePoints.append(event->pos());
        m_controls.setPressedControl(index);
    }
    QAbstractGraphicsShapeItem::mouseReleaseEvent(event);
}

bool GraphicsCircleItem::isShapeValid()
{
    if (m_controls.count()<2)
    {
        return false;
    }

    if (m_controls.count()==3)
    {
        NControlPos posBegin = m_controls.ctrlPosByIndex(0);
        NControlPos posEnd = m_controls.ctrlPosByIndex(1);
        if (m_controls.controlsPos(posBegin) == m_controls.controlsPos(posEnd))
        {
            return false;
        }
    }
    return true;
}

QPainterPath GraphicsCircleItem::getShapePath()
{
    QPainterPath path;

    QVector<QPointF> points = m_circlePoints;
    if (points.size() == 3)
    {
        qreal radius = -1;
        QPointF center = PMethod::tCircle(points[0],points[1], points[2], radius);
        m_cirRect = QRectF(0,0,radius*2,radius*2);
        m_cirRect.moveCenter(center);
        m_center = center;
        m_radius = radius;

        path.addEllipse(m_cirRect);
    }
    else if (points.size() == 2)
    {
        QPointF center = (points[0]+points[1])/2;
        qreal diameter = PMethod::distanceP2P(points[0],points[1]);
        m_cirRect = QRectF(0,0,diameter,diameter);
        m_cirRect.moveCenter(center);
        path.moveTo(m_circlePoints[0]);
        path.lineTo(m_circlePoints[1]);
    }

    return path;
}

QPainterPath GraphicsCircleItem::getOutSidePath()
{
    QPainterPath path;
    path.addRect(m_cirRect);
    return path;
}

QVariant GraphicsCircleItem::shapeData( DataType type )
{
    QVariant var;
    if (m_circlePoints.size() < 3)
    {
        return var;
    }

    switch (type)
    {
    case D_BeginPoint:
        var = mapToScene(m_circlePoints[0]);
        break;
    case D_EndPoint:
        var = mapToScene(m_circlePoints[2]);
        break;
    case D_Angle:
        var = 360;
        break;
    case D_Center:
        var = mapToScene(m_center);
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

GraphicsShapeItem * GraphicsCircleItem::copy()
{
    return new GraphicsCircleItem(*this);
}

bool GraphicsCircleItem::setShapeData( const QVariant &data )
{
    QList<QVariant> points = data.toList();
    if (points.size() != 3)
    {
        return false;
    }

    QPointF localP1 = mapFromScene(points.at(0).toPointF());
    QPointF localP2 = mapFromScene(points.at(1).toPointF());
    QPointF localP3 = mapFromScene(points.at(2).toPointF());
    m_circlePoints.clear();
    m_circlePoints<<localP1<<localP2<<localP3;
    m_controls.clearCtrls();
    m_controls.addControls(CircleP1);
    m_controls.addControls(CircleP2);
    m_controls.addControls(CircleP3);
    updateShape();
    return true;
}

QVariant GraphicsCircleItem::shapeData()
{
    QList<QVariant> points;
    for (int i=0; i<m_circlePoints.size(); i++)
    {
        points<<mapToScene(m_circlePoints.at(i));
    }
    return points;
}