#include "shape/graphrectitem.h"
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "pointalgorithm.h"

GraphicsRectItem::GraphicsRectItem(QGraphicsItem *parent)
    :GraphicsShapeItem(parent)
{
    m_rect = QRectF(0,0,0,0);
    addControls();
}

GraphicsRectItem::GraphicsRectItem( const GraphicsRectItem &other )
{
    m_rect = other.m_rect;
    *this = other;
}

void GraphicsRectItem::addControls()
{
    m_controls.addControls(LeftTop);
    m_controls.addControls(RightBottom);
    m_controls.addControls(LeftBottom);
    m_controls.addControls(RightTop);
    m_controls.addControls(Left);
    m_controls.addControls(Right);
    m_controls.addControls(Top);
    m_controls.addControls(Bottom);
    m_controls.addControls(Center);
}

void GraphicsRectItem::updateControls()
{
    // 设置控制点大小
    qreal len = qMin(m_rect.width(), m_rect.height());
    qreal width = qMin(len/3,qreal(ControlMaxWidth));
    if (width<m_controls.getControlWidth()) width = m_controls.getControlWidth();
    m_controls.setControlWidth(width);

    // 更新控制点位置
    m_controls.updateControl(LeftTop,rect().topLeft());
    m_controls.updateControl(RightBottom,rect().bottomRight());
    m_controls.updateControl(LeftBottom,rect().bottomLeft());
    m_controls.updateControl(RightTop,rect().topRight());
    m_controls.updateControl(Left,(rect().topLeft()+rect().bottomLeft())/2);
    m_controls.updateControl(Right,(rect().topRight()+rect().bottomRight())/2);
    m_controls.updateControl(Top,(rect().topLeft()+rect().topRight())/2);
    m_controls.updateControl(Bottom,(rect().bottomLeft()+rect().bottomRight())/2);
    m_controls.updateControl(Center,QLineF(rect().center(), (rect().topRight()+rect().bottomRight())/2).pointAt(3.0/4));
}

void GraphicsRectItem::resizeShape( NControlPos pos, const QPointF &point )
{
    QPointF pointSp = simplifyPoint(point);
    QPointF localP = mapFromScene(pointSp);
    switch (pos)
    {
    case LeftTop:
        {
            if (localP.x()<m_rect.right() && localP.y()<m_rect.bottom())
            {
                m_rect.setTopLeft(localP);
            }
            break;
        }
    case RightBottom:
        {
            if (localP.x()>m_rect.left() && localP.y()>m_rect.top())
            {
                m_rect.setBottomRight(localP);
            }
            break;
        }
    case RightTop:
        {
            if (localP.x()>m_rect.left() && localP.y()<m_rect.bottom())
            {
                m_rect.setTopRight(localP);
            }
            break;
        }
    case LeftBottom:
        {
            if (localP.x()<m_rect.right() && localP.y()>m_rect.top())
            {
                m_rect.setBottomLeft(localP);
            }
            break;
        }
    case Top:
        {
            if (localP.y()<m_rect.bottom())
            {
                m_rect.setTop(localP.y());
            }
            break;
        }
    case Left:
        {
            if (localP.x()<m_rect.right())
            {
                m_rect.setLeft(localP.x());
            }
            break;
        }
    case Bottom:
        {
            if (localP.y()>m_rect.top())
            {
                m_rect.setBottom(localP.y());
            }
            break;
        }
    case Right:
        {
            if (localP.x()>m_rect.left())
            {
                m_rect.setRight(localP.x());
            }
            break;
        }
    case Center:
        {
            rotateRect(localP);
            break;
        }
    default:
        break;
    }

    GraphicsShapeItem::resizeShape(pos,point);
}

void GraphicsRectItem::rotateRect( const QPointF &point )
{
    QLineF tmpLine(mapToScene(m_rect.center()), mapToScene(point));
    qreal angle = 360-tmpLine.angle();
    setTransformOriginPoint(m_rect.center());
    setRotation(angle);
}

QPainterPath GraphicsRectItem::getShapePath()
{
    QPainterPath path;
    path.addRect(m_rect);
    //添加箭头
    path.moveTo(m_rect.center());
    QPointF lineEnd = QLineF(rect().center(), (rect().bottomLeft()+rect().bottomRight())/2).pointAt(2.0);
    path.lineTo(lineEnd);
    QLineF line(m_rect.center(), lineEnd);
    QPolygonF polygon;
    polygon = PMethod::getArrow(line, line.length() / 5);
    path.addPolygon(polygon);
    return path;
}

QPainterPath GraphicsRectItem::getOutSidePath()
{
    QPainterPath path;
    QRectF rect;
    qreal widthOffset = m_rect.width() > 10 ? 1 : m_rect.width()/10;
    qreal heightOffset = m_rect.height() > 10 ? 1 : m_rect.height()/10;
    rect.setSize(QSizeF(m_rect.width()+widthOffset, m_rect.height()+heightOffset));
    rect.moveCenter(m_rect.center());
    path.addRect(rect);
    return path;
}

void GraphicsRectItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

QVariant GraphicsRectItem::shapeData( DataType type )
{
    QVariant var;
    switch (type)
    {
    case D_BeginPoint:
        var = mapToScene(m_rect.topLeft());
        break;
    case D_EndPoint:
        var = mapToScene(m_rect.bottomRight());
        break;
    case D_Angle:
        var = rotation();
        break;
    case D_Radius:
        break;
    case D_Center:
        var = mapToScene(m_rect.center());
        break;
    case D_Width:
        var = m_rect.width();
        break;
    case D_Height:
        var = m_rect.height();
        break;
    default:
        break;
    }

    return var;
}

GraphicsShapeItem * GraphicsRectItem::copy()
{
    return new GraphicsRectItem(*this);
}

bool GraphicsRectItem::setShapeData( const QVariant &data )
{
    QList<QVariant> dataList = data.toList();
    if (dataList.size() < 5)
    {
        return false;
    }
    
    QPointF pointCenter = dataList.at(1).toPointF(); //旋转后的中心点坐标
    double  width = dataList.at(2).toFloat();
    double  height = dataList.at(3).toFloat();
    double  angle = dataList.at(4).toFloat();

    if (dataList.size() < 6)
    {
        //创建后没初始化时
        QRectF rect = QRectF(pointCenter.x()-width/2, pointCenter.y()-height/2, width, height);
        setTransformOriginPoint(rect.center());
        this->setRotation(360-angle);
        m_rect = rect;
    }
    else
    {
        //UI调整时
        setRotation(360-angle);
        QPointF localP1 = mapFromScene(dataList.at(5).toPointF());
        QPointF localP2 = mapFromScene(dataList.at(6).toPointF());
        m_rect.setTopLeft(localP1);
        m_rect.setBottomRight(localP2);
    }
    
    addControls();
    updateShape();
    return true;
}

QVariant GraphicsRectItem::shapeData()
{
    QList<QVariant> dataList;
    dataList<<type()<<mapToScene(m_rect.center())<<m_rect.width()<< m_rect.height()<<360-rotation()<<mapToScene(m_rect.topLeft())<<mapToScene(m_rect.bottomRight());
    return dataList;
}
