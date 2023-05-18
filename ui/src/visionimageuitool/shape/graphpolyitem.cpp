#include "shape/graphpolyitem.h"
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "pointalgorithm.h"

GraphicsPolyItem::GraphicsPolyItem(QGraphicsItem *parent)
    :GraphicsShapeItem(parent)
    ,m_drawFinished(false)
{
    addControls();
}

GraphicsPolyItem::GraphicsPolyItem( const GraphicsPolyItem &other )
{
    *this = other;
    m_poly = other.m_poly;
    m_drawFinished = other.m_drawFinished;
}

void GraphicsPolyItem::addControls()
{
    m_controls.addControls(Poly);
    m_controls.addControls(Poly+1);
    m_poly.append(QPointF(0,0));
    m_poly.append(QPointF(0,0));
}

void GraphicsPolyItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

void GraphicsPolyItem::updateControls()
{
    for (int i=0; i<m_controls.count(); i++)
    {
        m_controls.updateControl(Poly+i, m_poly.at(i));
    }
}

void GraphicsPolyItem::resizeShape( NControlPos pos, const QPointF &point )
{
    QPointF pointSp = simplifyPoint(point);
    QPointF localP = mapFromScene(pointSp);
    if (m_controls.contains(pos))
    {
        m_poly[pos-Poly] = localP;
    }

    GraphicsShapeItem::resizeShape(pos,point);
}

void GraphicsPolyItem::removeCtrl( NControlPos pos )
{
    if (pos<Poly+m_poly.size() && pos >= Poly)
    {
        m_poly.remove(pos-Poly);
    }

    GraphicsShapeItem::removeCtrl(pos);
}

bool GraphicsPolyItem::isDrawFinished()
{
    return m_drawFinished;
}

void GraphicsPolyItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
    if (m_drawFinished)
    {
        return GraphicsShapeItem::mouseReleaseEvent(event);
    }
    
    if (event->button() == Qt::RightButton)
    {
        m_drawFinished = true;
        removeCtrl(Poly+m_controls.count()-1);
        // 图形有变化,更新
        GraphicsShapeItem::updateShape();
    }
    else
    {
        int index = Poly+m_controls.count();
        m_controls.addControls(index);
        m_poly.append(event->pos());
        m_controls.setPressedControl(index);
    }
    QAbstractGraphicsShapeItem::mouseReleaseEvent(event);
}

bool GraphicsPolyItem::isShapeValid()
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

QPainterPath GraphicsPolyItem::getShapePath()
{
    QPainterPath path;
    path.moveTo(m_poly.at(0));
    for (int i=1; i<m_poly.size(); i++)
    {
        path.lineTo(m_poly.at(i));
    }

    return path;
}

QPainterPath GraphicsPolyItem::getOutSidePath()
{
    return GraphicsShapeItem::getOutSidePath();
}

QLineF GraphicsPolyItem::getArrowLine( qreal &width )
{
    if (m_poly.size() > 1)
    {
        return QLineF(m_poly[m_poly.size()-2], m_poly[m_poly.size()-1]);
    }
    return QLineF();
}

QVariant GraphicsPolyItem::shapeData( DataType type )
{
    QVariant var;
    switch (type)
    {
    case D_BeginPoint:
        var = mapToScene(m_poly[0]);
        break;
    case D_EndPoint:
        var = mapToScene(m_poly[m_poly.size()-1]);
        break;
    case D_Angle:
        break;
    case D_Radius:
        break;
    case D_Center:
        break;
    case D_Length:
        {
            double len = 0.0;
            for (int i=0; i<m_poly.size()-1; i++)
            {
                len += QLineF(m_poly[i], m_poly[i+1]).length();
            }
            var = len;
            break;
        }
    default:
        break;
    }

    return var;
}

GraphicsShapeItem * GraphicsPolyItem::copy()
{
    return new GraphicsPolyItem(*this);
}

bool GraphicsPolyItem::setShapeData( const QVariant &data )
{
    QList<QVariant> points = data.toList();
    if (points.size() < 3)
    {
        return false;
    }

    m_poly.clear();
    m_controls.clearCtrls();
    for (int i=1; i<points.size(); i++)
    {
        QPointF localP = mapFromScene(points.at(i).toPointF());
        m_poly.append(localP);
        m_controls.addControls(Poly+i);
    }
    updateShape();
    return true;
}

QVariant GraphicsPolyItem::shapeData()
{
    QList<QVariant> points;
    points << EnShape_Polygon;
    for (int i=0; i<m_poly.size(); i++)
    {
        points<<mapToScene(m_poly.at(i));
    }
    return points;
}
