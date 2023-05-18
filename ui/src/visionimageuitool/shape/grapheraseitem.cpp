#include "shape/grapheraseitem.h"
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "pointalgorithm.h"

GraphicsEraseItem::GraphicsEraseItem(QGraphicsItem *parent)
    :GraphicsShapeItem(parent)
    ,m_drawFinished(false)
{
    addControls();
}

GraphicsEraseItem::GraphicsEraseItem( const GraphicsEraseItem &other )
{
    *this = other;
    m_poly = other.m_poly;
    m_drawFinished = other.m_drawFinished;
}

void GraphicsEraseItem::addControls()
{
    m_controls.addControls(Poly);
    m_controls.addControls(Poly+1);
    m_poly.append(QPointF(0,0));
    m_poly.append(QPointF(0,0));
}

void GraphicsEraseItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

void GraphicsEraseItem::updateControls()
{
    for (int i=0; i<m_controls.count(); i++)
    {
        m_controls.updateControl(Poly+i, m_poly.at(i));
    }
}

void GraphicsEraseItem::resizeShape( NControlPos pos, const QPointF &point )
{
    QPointF pointSp = simplifyPoint(point);
    QPointF localP = mapFromScene(pointSp);
    if (m_controls.contains(pos))
    {
        m_poly[pos-Poly] = localP;
    }

    GraphicsShapeItem::resizeShape(pos,point);
}

void GraphicsEraseItem::removeCtrl( NControlPos pos )
{
    if (pos<Poly+m_poly.size() && pos >= Poly)
    {
        m_poly.remove(pos-Poly);
    }

    GraphicsShapeItem::removeCtrl(pos);
}

bool GraphicsEraseItem::isDrawFinished()
{
    return m_drawFinished;
}

void GraphicsEraseItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
    if (!m_drawFinished)
    {
        int index = Poly+m_controls.count();
        m_poly.append(event->pos());
        m_controls.setPressedControl(index);
    }
    GraphicsShapeItem::updateShape();
    GraphicsShapeItem::mouseReleaseEvent(event);
}

void GraphicsEraseItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    m_drawFinished = false;
}

void GraphicsEraseItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
    m_drawFinished = true;
    // 图形有变化,更新
    GraphicsShapeItem::updateShape();
    return GraphicsShapeItem::mouseReleaseEvent(event);
}

void GraphicsEraseItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    QPen pen = painter->pen();
    QColor color(m_color); 
    color.setAlpha(128);
    pen.setColor(color);  //设置笔颜色
    //pen.setColor(Qt::green);  //设置笔颜色
    pen.setWidth(m_dLineWidth);   //设置线宽
    pen.setJoinStyle(Qt::RoundJoin); //设置拐角平滑
    pen.setCapStyle(Qt::RoundCap); //设置笔端平滑
    painter->setPen(pen);
    //pen.setStyle(Qt::DashDotLine);
    painter->setRenderHint(QPainter::Antialiasing, true); //消除锯齿
    painter->setOpacity(0.5); //设置透明
    painter->drawPath(m_shapePath);
}

bool GraphicsEraseItem::isShapeValid()
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

QPainterPath GraphicsEraseItem::getShapePath()
{
    QPainterPath path;
    path.moveTo(m_poly.at(0));
    for (int i=1; i<m_poly.size(); i++)
    {
        path.lineTo(m_poly.at(i));
    }

    return path;
}

QPainterPath GraphicsEraseItem::getOutSidePath()
{
    return GraphicsShapeItem::getOutSidePath();
}

QLineF GraphicsEraseItem::getArrowLine( qreal &width )
{
    if (m_poly.size() > 1)
    {
        return QLineF(m_poly[m_poly.size()-2], m_poly[m_poly.size()-1]);
    }
    return QLineF();
}

QVariant GraphicsEraseItem::shapeData( DataType type )
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

GraphicsShapeItem * GraphicsEraseItem::copy()
{
    return new GraphicsEraseItem(*this);
}

bool GraphicsEraseItem::setShapeData( const QVariant &data )
{
    QList<QVariant> points = data.toList();
    if (points.size() < 3)
    {
        return false;
    }

    m_poly.clear();
    m_controls.clearCtrls();
    m_dLineWidth = points.at(1).toFloat();
    for (int i=2; i<points.size(); i++)
    {
        QPointF localP = mapFromScene(points.at(i).toPointF());
        m_poly.append(localP);
        m_controls.addControls(Poly+i);
    }
    updateShape();
    return true;
}

QVariant GraphicsEraseItem::shapeData()
{
    QList<QVariant> points;
    points << EnShape_Erase_Path;
    points << m_dLineWidth;
    for (int i=0; i<m_poly.size(); i++)
    {
        points<<mapToScene(m_poly.at(i));
    }
    return points;
}


