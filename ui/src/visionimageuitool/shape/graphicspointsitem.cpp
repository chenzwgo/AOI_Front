#include "graphicspointsitem.h"

#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsView>
GraphicsPointsItem::GraphicsPointsItem(QGraphicsItem *parent)
    :GraphicsShapeItem(parent)
    , m_drawFinished(false)
{
    setMoveEnable(false);
    addControls();
}

GraphicsPointsItem::GraphicsPointsItem(const GraphicsPointsItem &other)
{
    *this = other;
    m_poly = other.m_poly;
    m_drawFinished = other.m_drawFinished;
}

void GraphicsPointsItem::resizeShape(NControlPos pos, const QPointF &point)
{
    //QPointF pointSp = simplifyPoint(point);
    //QPointF localP = mapFromScene(pointSp);
    //if (m_controls.contains(pos))
    //{
    //    m_poly[pos - Poly] = localP;
    //}

    //GraphicsShapeItem::resizeShape(pos, point);
}

void GraphicsPointsItem::addControls()
{
    //m_controls.addControls(Poly);
    //m_controls.addControls(Poly + 1);
    //m_poly.append(QPointF(0, 0));
    //m_poly.append(QPointF(0, 0));
}

void GraphicsPointsItem::updateControls()
{
    //for (int i = 0; i < m_controls.count(); i++)
    //{
    //    m_controls.updateControl(Poly + i, m_poly.at(i));
    //}
}

void GraphicsPointsItem::removeCtrl(NControlPos pos)
{
    //if (pos < Poly + m_poly.size() && pos >= Poly)
    //{
    //    m_poly.remove(pos - Poly);
    //}

    //GraphicsShapeItem::removeCtrl(pos);
}

bool GraphicsPointsItem::isDrawFinished()
{
    return m_drawFinished;
}

bool GraphicsPointsItem::isShapeValid()
{
    if (m_controls.count() < 2)
    {
        return false;
    }

    if (m_controls.count() == 3)
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

QVariant GraphicsPointsItem::shapeData(DataType type)
{
    QVariant var;
    switch (type)
    {
    case D_BeginPoint:
        var = mapToScene(m_poly[0]);
        break;
    case D_EndPoint:
        var = mapToScene(m_poly[m_poly.size() - 1]);
        break;
    case D_Angle:
        break;
    case D_Radius:
        break;
    case D_Center:
        break;
    case D_Length:
    {
        var = m_poly.size();
        break;
    }
    default:
        break;
    }

    return var;
}

QVariant GraphicsPointsItem::shapeData()
{
    QList<QVariant> points;
    points << EnShape_Points;
    points << m_dLineWidth;
    for (int i = 0; i < m_poly.size(); i++)
    {
        points << mapToScene(m_poly.at(i));
    }
    return points;
}

void GraphicsPointsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_drawFinished = false;
}

void GraphicsPointsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_drawFinished = true;
    // 图形有变化,更新
    GraphicsShapeItem::updateShape();
    return GraphicsShapeItem::mouseReleaseEvent(event);
}

void GraphicsPointsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_drawFinished)
    {
        int index = Poly + m_controls.count();
        m_poly.append(event->pos());
        m_controls.setPressedControl(index);
    }
    GraphicsShapeItem::updateShape();
    GraphicsShapeItem::mouseReleaseEvent(event);
}

QPainterPath GraphicsPointsItem::getShapePath()
{
    QPainterPath path;
    path.moveTo(m_poly.at(0));

    return path;
}

QPainterPath GraphicsPointsItem::getOutSidePath()
{
    return GraphicsShapeItem::getOutSidePath();
}

void GraphicsPointsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}

GraphicsShapeItem * GraphicsPointsItem::copy()
{
    return new GraphicsPointsItem(*this);
}

void GraphicsPointsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen = painter->pen();
    QColor color(m_color);
    color.setAlpha(128);
    pen.setColor(color);  //设置笔颜色
                          //pen.setColor(Qt::green);  //设置笔颜色
    pen.setWidth(m_dLineWidth);   //设置线宽
    painter->setPen(pen);
    //pen.setStyle(Qt::DashDotLine);
    painter->setOpacity(0.5); //设置透明
    painter->drawPoints(m_poly);
}

bool GraphicsPointsItem::setShapeData(const QVariant &data)
{
    QList<QVariant> points = data.toList();
    if (points.size() < 2)
    {
        return false;
    }

    m_poly.clear();
    m_controls.clearCtrls();
    for (int i = 1; i < points.size(); i++)
    {
        QPointF localP = mapFromScene(points.at(i).toPointF());
        m_poly.append(localP);
        m_controls.addControls(Poly + i);
    }
    updateShape();
    return true;
}

