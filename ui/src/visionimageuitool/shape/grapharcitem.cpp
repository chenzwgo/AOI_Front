#include "shape/grapharcitem.h"
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "pointalgorithm.h"

GraphicsArcItem::GraphicsArcItem(QGraphicsItem *parent)
    :GraphicsShapeItem(parent)
    ,m_drawFinished(false)
    ,m_radius(0)
    ,m_sweepLen(0)
{
    addControls();
}

GraphicsArcItem::GraphicsArcItem( const GraphicsArcItem &other )
{
    *this = other;
    m_arcPoints   =other.m_arcPoints   ;
    m_sweepLen    =other.m_sweepLen    ;    // 弧度
    m_radius      =other.m_radius      ;    // 半径
    m_center      =other.m_center      ;    // 圆心
    m_drawFinished=other.m_drawFinished;
}

void GraphicsArcItem::addControls()
{
    m_controls.addControls(ArcP1);
    m_controls.addControls(ArcP2);
    m_arcPoints.append(QPointF(0,0));
    m_arcPoints.append(QPointF(0,0));
}

void GraphicsArcItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}


void GraphicsArcItem::updateControls()
{
    for (int i=0; i<m_controls.count(); i++)
    {
        m_controls.updateControl(ArcP1+i, m_arcPoints.at(i));
    }
}

void GraphicsArcItem::resizeShape( NControlPos pos, const QPointF &point )
{
    QPointF pointSp = simplifyPoint(point);
    QPointF localP = mapFromScene(pointSp);
    if (m_controls.contains(pos))
    {
        QVector<QPointF> points = m_arcPoints;
        points[pos-ArcP1] = localP;
        if (points[0] == points[1]
        || points[0] == points[points.size()-1])
        {
            return;
        }

        // 判断3点成线
        if (points.size() == 3)
        {
            QLineF lineP02(points[0], points[2]);
            QLineF lineP01(points[0], points[1]);
            if(qAbs(lineP01.angle() - lineP02.angle())<0.1)
            {
                return;
            }
        }

        m_arcPoints = points;
    }

    GraphicsShapeItem::resizeShape(pos,point);
}

bool GraphicsArcItem::isDrawFinished()
{
    return m_drawFinished;
}

void GraphicsArcItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
    if (m_drawFinished)
    {
        return GraphicsShapeItem::mouseReleaseEvent(event);
    }
    
    if (m_controls.count() == ArcP3-ArcP1+1)
    {
        m_drawFinished = true;
    }
    else
    {
        int index = ArcP1+m_controls.count();
        m_controls.addControls(index);
        m_arcPoints.append(event->pos()+QPointF(5,10));
        m_controls.setPressedControl(index);
        updateControls();
    }
    QAbstractGraphicsShapeItem::mouseReleaseEvent(event);
}

bool GraphicsArcItem::isShapeValid()
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

QPainterPath GraphicsArcItem::getArcPath( const QPointF &center, qreal radius, qreal startAngle, qreal sweepLength )
{
    QRectF rect(0,0,radius*2,radius*2);
    rect.moveCenter(center);
    QPainterPath path;                
    path.arcMoveTo(rect, startAngle);
    path.arcTo(rect, startAngle, sweepLength);
    return path;
}

QPainterPath GraphicsArcItem::getShapePath()
{
    QPainterPath path;
    if (m_arcPoints.size()==2)
    {
        path.moveTo(m_arcPoints[0]);
        path.lineTo(m_arcPoints[1]);
    }
    else
    {
        qreal radius = -1;
        QPointF center = PMethod::tCircle(m_arcPoints[0],m_arcPoints[1], m_arcPoints[2], radius);
        if (center.isNull())
        {
            return path;
        }
        
        QLineF line1(center, m_arcPoints[0]);
        QLineF line2(center, m_arcPoints[2]);
        // 判断为正向弧还是逆向弧
        QLineF lineP02(m_arcPoints[0], m_arcPoints[2]);
        QLineF lineP01(m_arcPoints[0], m_arcPoints[1]);

        qreal sweepLen = lineP02.angleTo(lineP01)<=180 ? line2.angleTo(line1) : line1.angleTo(line2);
        qreal startAngle = lineP02.angleTo(lineP01)<=180 ? line2.angle() : line1.angle();
        path = getArcPath(center, radius, startAngle, sweepLen);
        m_radius = radius;
        m_sweepLen = sweepLen;
    }
    return path;
}

QPainterPath GraphicsArcItem::getOutSidePath()
{
    return GraphicsShapeItem::getOutSidePath();
}

QLineF GraphicsArcItem::getArrowLine( qreal &width )
{
    QLineF arrowLine;
    if (m_arcPoints.size() == 3)
    {
        qreal radius = -1;
        QPointF center = PMethod::tCircle(m_arcPoints[0],m_arcPoints[1], m_arcPoints[2], radius);
        if (center.isNull())
        {
            return arrowLine;
        }

        // 判断为正向弧还是逆向弧
        QLineF lineP02(m_arcPoints[0], m_arcPoints[2]);
        QLineF lineP01(m_arcPoints[0], m_arcPoints[1]);

        // 画箭头
        if (lineP02.angleTo(lineP01)<=180)
        {
            QLineF lineEnd(center,m_arcPoints[2]);
            QLineF lineArrow = lineEnd.normalVector();
            lineArrow.translate(lineEnd.dx(),lineEnd.dy());
            arrowLine = QLineF(lineArrow.p2(),lineArrow.p1());
        }
        else
        {
            QLineF lineEnd(m_arcPoints[2],center);
            QLineF lineArrow = lineEnd.normalVector();
            arrowLine = QLineF(lineArrow.p2(),lineArrow.p1());
        }
    }
    return arrowLine;
}

QVariant GraphicsArcItem::shapeData( DataType type )
{
    QVariant var;
    if (m_arcPoints.size()<3)
    {
        return var;
    }
    
    switch (type)
    {
    case D_BeginPoint:
        var = mapToScene(m_arcPoints[0]);
        break;
    case D_EndPoint:
        var = mapToScene(m_arcPoints[2]);
        break;
    case D_Angle:
        var = m_sweepLen;
        break;
    case D_Radius:
        var = m_radius;
        break;
    case D_Center:
        var = mapToScene(m_center);
        break;
    case D_Length:
        var = m_radius*PI*m_sweepLen/180.0;
        break;
    default:
        break;
    }

    return var;
}

GraphicsShapeItem * GraphicsArcItem::copy()
{
    return new GraphicsArcItem(*this);
}

bool GraphicsArcItem::setShapeData( const QVariant &data )
{
    QList<QVariant> points = data.toList();
    if (points.size() != 3)
    {
        return false;
    }

    QPointF localP1 = mapFromScene(points.at(0).toPointF());
    QPointF localP2 = mapFromScene(points.at(1).toPointF());
    QPointF localP3 = mapFromScene(points.at(2).toPointF());
    m_arcPoints.clear();
    m_arcPoints<<localP1<<localP2<<localP3;
    m_controls.clearCtrls();
    m_controls.addControls(ArcP1);
    m_controls.addControls(ArcP2);
    m_controls.addControls(ArcP3);
    updateShape();
    return true;
}

QVariant GraphicsArcItem::shapeData()
{
    QList<QVariant> points;
    for (int i=0; i<m_arcPoints.size(); i++)
    {
        points<<mapToScene(m_arcPoints.at(i));
    }
    return points;
}
