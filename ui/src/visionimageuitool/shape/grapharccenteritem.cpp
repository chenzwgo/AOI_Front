#include "shape/grapharccenteritem.h"
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "pointalgorithm.h"

#define CtlCount (ArcCP2-ArcCP1+1)   //控制点个数

GraphicsArcCenterItem::GraphicsArcCenterItem(QGraphicsItem *parent)
    :GraphicsShapeItem(parent)
    ,m_drawFinished(false)
    ,m_radius(0)
    ,m_sweepLen(0)
{
    addControls();
}

GraphicsArcCenterItem::GraphicsArcCenterItem( const GraphicsArcCenterItem &other )
{
    *this = other;
    m_arcPoints   =other.m_arcPoints   ;
    m_drawFinished=other.m_drawFinished;
    m_sweepLen    =other.m_sweepLen    ;    // 弧度
    m_radius      =other.m_radius      ;    // 半径
}



void GraphicsArcCenterItem::addControls()
{
    m_controls.addControls(ArcCP1);
    m_controls.addControls(ArcCenter);
    m_arcPoints.append(QPointF(0,0));
    m_arcPoints.append(QPointF(0,0));
}

void GraphicsArcCenterItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
}

void GraphicsArcCenterItem::updateControls()
{
    for (int i=0; i<m_controls.count(); i++)
    {
        m_controls.updateControl(ArcCP1+i, m_arcPoints.at(i));
    }
}

void GraphicsArcCenterItem::resizeShape( NControlPos pos, const QPointF &point )
{
    if (!m_controls.contains(pos))
    {
        return;
    }

    QPointF pointSp = simplifyPoint(point);
    QPointF localP = mapFromScene(pointSp);

    QVector<QPointF> points = m_arcPoints;
    points[pos-ArcCP1] = localP;
    
    if (m_arcPoints.size() == 2)
    {
        if (points[0] != points[1])
        {
            m_arcPoints = points;
            GraphicsShapeItem::resizeShape(pos,point);
        }
        return;
    }
    
    switch (pos)
    {
    case ArcCRotate:
        {
            QLineF line1(points[1],m_arcPoints[CtlCount-2]);
            QLineF line2(points[1],points[CtlCount-2]);
            qreal angle = line2.angleTo(line1);
            setTransformOriginPoint(points[1]);
            setRotation(rotation()+angle);
            break;
        }
    case ArcCMid:
        {
            if (points[1] == points[2])
            {
                return;
            }
            qreal radius = PMethod::distanceP2P(points[1], points[2]);
            QLineF lineBegin = QLineF(points[1],points[0]);
            lineBegin.setLength(radius);
            points[0] = lineBegin.p2();
            QLineF lineEnd = QLineF(points[1],points[CtlCount-1]);
            lineEnd.setLength(radius);
            points[CtlCount-1] = lineEnd.p2();
            break;
        }
    case ArcCP1:
        {
            if (points[0] == points[CtlCount-1])
            {
                return;
            }
            qreal radius = PMethod::distanceP2P(points[1], points[2]);
            QLineF lineEnd = QLineF(points[1],points[0]);
            lineEnd.setLength(radius);
            points[0] = lineEnd.p2();
            break;
        }
    case ArcCP2:
        {
            if (points[0] == points[CtlCount-1])
            {
                return;
            }

            qreal radius = PMethod::distanceP2P(points[1], points[2]);
            QLineF lineEnd = QLineF(points[1],points[CtlCount-1]);
            lineEnd.setLength(radius);
            points[CtlCount-1] = lineEnd.p2();
            break;
        }
    case ArcCenter:
        {
            qreal radius = PMethod::distanceP2P(points[1], points[2]);
            QLineF lineEnd = QLineF(points[2],m_arcPoints[1]);
            lineEnd.setLength(radius);
            points[1] = lineEnd.p2();
            break;
        }
    default:
        break;
    }

    // 得到弧度
    qreal sweepLength = getSweepLen(points[1],points[0],points[CtlCount-1]);
    if(sweepLength<0.1 || sweepLength>359.9) return;

    // 得到圆弧中间点
    points[2] = PMethod::arcMidPoint(points[1],points[0],points[CtlCount-1],sweepLength>180);
    points[CtlCount-2] = QLineF(points[1],points[2]).pointAt(4.0/5);

    m_arcPoints = points;
    GraphicsShapeItem::resizeShape(pos,point);
}

qreal GraphicsArcCenterItem::getSweepLen( const QPointF &center, const QPointF &p1, const QPointF &p2 )
{
    QLineF line1 = QLineF(center,p1);
    QLineF line2 = QLineF(center,p2);
    return line2.angleTo(line1);
}

bool GraphicsArcCenterItem::isDrawFinished()
{
    return m_drawFinished;
}

void GraphicsArcCenterItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
    if (m_drawFinished)
    {
        return GraphicsShapeItem::mouseReleaseEvent(event);
    }
    
    m_controls.addControls(ArcCMid);
    m_controls.addControls(ArcCRotate);
    m_controls.addControls(ArcCP2);
    m_controls.setPressedControl(ArcCP2);

    QPointF pCenter = QPointF(0,0);
    QPointF p1 = event->pos();
    qreal r = PMethod::distanceP2P(pCenter,p1);
    QPointF p2 = QPointF(r,0);
    qreal angle = QLineF(pCenter, p1).angle();
    QPointF pMid = PMethod::arcMidPoint(pCenter, p1, p2, angle>180);
    QPointF pRolate = QLineF(pCenter,pMid).pointAt(4.0/5);
    m_arcPoints[ArcCP1-ArcCP1] = p1;
    m_arcPoints[ArcCenter-ArcCP1] = pCenter;
    m_arcPoints.append(pMid);
    m_arcPoints.append(pRolate);
    m_arcPoints.append(p2);
    
    GraphicsShapeItem::updateShape(); // 修改了图形点位,需要调用刷新
    m_drawFinished = true;
    QAbstractGraphicsShapeItem::mouseReleaseEvent(event);
}

bool GraphicsArcCenterItem::isShapeValid()
{
    if (m_controls.count()<2)
    {
        return false;
    }

    if (m_controls.count()==CtlCount)
    {
        NControlPos posBegin = m_controls.ctrlPosByIndex(1);
        NControlPos posEnd = m_controls.ctrlPosByIndex(CtlCount-1);
        if (m_controls.controlsPos(posBegin) == m_controls.controlsPos(posEnd))
        {
            return false;
        }
    }
    return true;
}

QPainterPath GraphicsArcCenterItem::getArcPath( const QPointF &center, qreal radius, qreal startAngle, qreal sweepLength )
{
    QRectF rect(0,0,radius*2,radius*2);
    rect.moveCenter(center);
    QPainterPath path;
    path.arcMoveTo(rect, startAngle);
    path.arcTo(rect, startAngle,sweepLength);
    return path;
}

QPainterPath GraphicsArcCenterItem::getShapePath()       // 获取去形状轨迹
{
    QPainterPath path;
    if (m_arcPoints.size() == 2)
    {
        path.moveTo(m_arcPoints[1]);
        path.lineTo(m_arcPoints[0]);
    }
    else
    {
        QLineF line1 = QLineF(m_arcPoints[1],m_arcPoints[0]);
        QLineF line2 = QLineF(m_arcPoints[1],m_arcPoints[CtlCount-1]);
        qreal startAngle = line2.angle();
        qreal sweepLength = line2.angleTo(line1);

        qreal radius = line1.length();
        path = getArcPath(m_arcPoints[1], radius, startAngle, sweepLength);
        m_sweepLen = sweepLength;
        m_radius = radius;
    }
    
    return path;
}
QPainterPath GraphicsArcCenterItem::getOutSidePath()     // 获取外框轨迹
{
    QPainterPath path;
    if (m_arcPoints.size() == CtlCount)
    {
        path = getShapePath();
        path.lineTo(m_arcPoints[1]);
        path.closeSubpath();
    }
    return path;
}

QLineF GraphicsArcCenterItem::getArrowLine( qreal &width )
{
    QLineF arrowLine;
    if (m_arcPoints.size() == CtlCount)
    {
        // 画箭头
        QLineF lineEnd(m_arcPoints[1],m_arcPoints[ArcCP2-ArcCP1]);
        QLineF lineArrow = lineEnd.normalVector();
        lineArrow.translate(lineEnd.dx(),lineEnd.dy());

        arrowLine = QLineF(lineArrow.p2(),lineArrow.p1());
    }
    return arrowLine;
}

QVariant GraphicsArcCenterItem::shapeData( DataType type )
{
    QVariant var;
    if (m_arcPoints.size() < ArcCP2-ArcCP1)
    {
        return var;
    }

    switch (type)
    {
    case D_BeginPoint:
        var = mapToScene(m_arcPoints[ArcCP1-ArcCP1]);
        break;
    case D_EndPoint:
        var = mapToScene(m_arcPoints[ArcCP2-ArcCP1]);
        break;
    case D_Angle:
        var = m_sweepLen;
        break;
    case D_Radius:
        var = m_radius;
        break;
    case D_Center:
        var = mapToScene(m_arcPoints[ArcCenter-ArcCP1]);
        break;
    case D_Length:
        var = m_radius*PI*m_sweepLen/180.0;
        break;
    default:
        break;
    }

    return var;
}

GraphicsShapeItem * GraphicsArcCenterItem::copy()
{
    return new GraphicsArcCenterItem(*this);
}

bool GraphicsArcCenterItem::setShapeData( const QVariant &data )
{
    QList<QVariant> points = data.toList();
    if (points.size() != 4)
    {
        return false;
    }

    setRotation(points.at(3).toReal());
    QPointF localP1 = mapFromScene(points.at(0).toPointF());
    QPointF center = mapFromScene(points.at(1).toPointF());
    QPointF localP2 = mapFromScene(points.at(2).toPointF());

    // 得到弧度
    qreal sweepLength = getSweepLen(center,localP1,localP2);

    QPointF pMid = PMethod::arcMidPoint(center, localP1, localP2, sweepLength>180);
    QPointF pRolate = QLineF(center,pMid).pointAt(4.0/5);
    m_arcPoints.clear();
    m_arcPoints << localP1 << center <<pMid<<pRolate<<localP2;
    m_controls.clearCtrls();
    m_controls.addControls(ArcCP1);
    m_controls.addControls(ArcCenter);
    m_controls.addControls(ArcCMid);
    m_controls.addControls(ArcCRotate);
    m_controls.addControls(ArcCP2);
    updateShape();
    return true;
}

QVariant GraphicsArcCenterItem::shapeData()
{
    if (m_arcPoints.size()<5)
    {
        return QVariant();
    }
    
    QList<QVariant> points;
    points<<mapToScene(m_arcPoints[0])
        <<mapToScene(m_arcPoints[1])
        <<mapToScene(m_arcPoints[4]);

    points<<rotation();
    return points;
}
