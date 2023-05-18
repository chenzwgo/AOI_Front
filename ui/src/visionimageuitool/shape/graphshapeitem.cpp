#include "shape/graphshapeitem.h"
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "pointalgorithm.h"

GraphicsShapeItem::GraphicsShapeItem(QGraphicsItem *parent)
    :QAbstractGraphicsShapeItem(parent)
    ,m_bResized(false)
    ,m_bShowArrow(true)
    ,m_dLineWidth(0.001)
    ,m_color(Qt::red)
    , m_bIsEnableMove(true)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
    m_controls.setParent(this);
}

QPainterPath GraphicsShapeItem::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path += m_outsidePath;
    path += m_cltsRectPath;
    return path;
}

QRectF GraphicsShapeItem::boundingRect() const
{
    return shape().boundingRect();
}

void GraphicsShapeItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    QPen pen = painter->pen();
    pen.setColor(m_color);
    pen.setWidth(m_dLineWidth);
    painter->setPen(pen);
    painter->drawPath(m_shapePath);
    if (!m_arrowHead.isEmpty() && m_bShowArrow) // 画箭头
    {
        pen.setColor(Qt::blue);
        painter->setPen(pen);
        painter->drawPolyline(m_arrowHead);
    }

    // 选中时绘制
    if (option->state & QStyle::State_Selected
        || option->state & QStyle::State_MouseOver)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::magenta);
        painter->setOpacity(0.5);
        for (int i = 0; i < m_controls.count(); i++)
        {
            NControlPos pos = m_controls.ctrlPosByIndex(i);
            if (pos == Center || pos >= LineP1)
            {
                painter->drawEllipse(m_controls.controls(pos));
            }
            else
            {
                painter->drawRect(m_controls.controls(pos));
            }
        }

        painter->setBrush(QColor("blue"));

        // 绘制选中线
        pen.setColor(Qt::blue);
        pen.setStyle(Qt::DashDotLine);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(m_outsidePath);

        //painter->drawPath(shape());
    }
}


void GraphicsShapeItem::resizeShape( NControlPos pos, const QPointF &point )
{
    m_bResized = true;
    updateShape();
}


void GraphicsShapeItem::updateShape()
{
    // 此句用来注明需要时UI要进行刷新
    prepareGeometryChange(); 

    // 更新控制点位置
    updateControls();

    m_shapePath = getShapePath();
    m_outsidePath = getOutSidePath();
    qreal width = ARROW_SIZE;
    QLineF arrowLine = getArrowLine(width);
    if (!arrowLine.isNull())
    {
        m_arrowHead = PMethod::getArrow(arrowLine, width);
    }
}

void GraphicsShapeItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    NControlPos pos = m_controls.hitControls(event->pos());
    m_controls.setPressedControl(pos);
    updateCursor();
    if (pos != None)
    {
        scene()->clearSelection();
    }
    
    QAbstractGraphicsShapeItem::mousePressEvent(event);
}

void GraphicsShapeItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
    m_controls.setPressedControl(None);
    QAbstractGraphicsShapeItem::mouseReleaseEvent(event);
}

void GraphicsShapeItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
    if (m_bIsEnableMove)
    {
        if (None != m_controls.pressedControl())
        {
            resizeShape(m_controls.pressedControl(), event->scenePos());
        }
        else
        {
            m_bResized = false;
            m_controls.updateControlsLastPos();
            QAbstractGraphicsShapeItem::mouseMoveEvent(event);
        }
    }
}

void GraphicsShapeItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
    updateCursor(m_controls.hitControls(event->pos()));
    QAbstractGraphicsShapeItem::hoverEnterEvent(event);
}

void GraphicsShapeItem::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
    NControlPos pos = m_controls.hitControls(event->pos());
    updateCursor(pos);
    if (pos != None)
    {
        QPointF point = m_controls.controlsPos(pos);
        setToolTip(QString("%1, %2").arg(point.x(),0,'f',2).arg(point.y(),0,'f',2));
    }
    
    QAbstractGraphicsShapeItem::hoverMoveEvent(event);
}

void GraphicsShapeItem::updateCursor( NControlPos pos )
{
    Qt::CursorShape cursorShape = Qt::SizeAllCursor;
    // 图形选转,鼠标样式也要跟着变化
    QVector<Qt::CursorShape> cursorShapeList;
    cursorShapeList<<Qt::SizeFDiagCursor<<Qt::SizeVerCursor<<Qt::SizeBDiagCursor<<Qt::SizeHorCursor;
    int t = (int)(rotation()+45/2)%360/45;
    QString pixmapfile;
    switch (pos)
    {
    case Top:             // 上下
    case Bottom:
        cursorShape = cursorShapeList.at((t+1)%4);
        break;
    case Left:            // 左右
    case Right:
        cursorShape = cursorShapeList.at((t+3)%4);
        break;
    case LeftBottom:      // 左下角
    case RightTop:        // 右上角
        cursorShape = cursorShapeList.at((t+2)%4);
        break;
    case LeftTop:         // 左上角
    case RightBottom:     // 右下角
        cursorShape = cursorShapeList.at(t%4);
        break;
    case Center:          // 中间
    case ArcCRotate:
        cursorShape = Qt::BitmapCursor;
        pixmapfile = ":/image/icons/rotation.png";
        break;
    case None:
        cursorShape = Qt::SizeAllCursor;
        break;
    default:
        cursorShape = Qt::SizeFDiagCursor;
        break;
    }

    if (cursor().shape() != cursorShape)
    {
        if (cursorShape == Qt::BitmapCursor)
        {
            setCursor(QPixmap(pixmapfile));
        }
        else
        {
            setCursor(cursorShape);
        }
    }
}

void GraphicsShapeItem::updateCursor()
{
    updateCursor(m_controls.pressedControl());
}

QPointF GraphicsShapeItem::pointAt( NControlPos pos )
{
    if (m_controls.contains(pos))
    {
        return m_controls.controlsPos(pos);
    }
    return QPointF();
}


QPointF GraphicsShapeItem::lastPointAt( NControlPos pos )
{
    if (m_controls.contains(pos))
    {
        return m_controls.controlsLastPos(pos);
    }
    return QPointF();
}

void GraphicsShapeItem::moveOffset( qreal dx, qreal dy )
{
    m_bResized = false;
    m_controls.updateControlsLastPos();
    QAbstractGraphicsShapeItem::moveBy(dx,dy);
}

int GraphicsShapeItem::ctlsCount()
{
    return m_controls.count();
}

NControlPos GraphicsShapeItem::ctrlPos( int index )
{
    return m_controls.ctrlPosByIndex(index);
}

void GraphicsShapeItem::removeCtrl( NControlPos pos )
{
    prepareGeometryChange();
    return m_controls.removeCtrl(pos);
}

bool GraphicsShapeItem::isDrawFinished()
{
    return true;
}

bool GraphicsShapeItem::isShapeValid()
{
    if (m_controls.count()<2)
    {
        return false;
    }
    
    NControlPos posBegin = m_controls.ctrlPosByIndex(0);
    NControlPos posEnd = m_controls.ctrlPosByIndex(m_controls.count()-1);
    if (m_controls.controlsPos(posBegin) == m_controls.controlsPos(posEnd))
    {
        return false;
    }

    return true;
}

bool GraphicsShapeItem::onControls( const QPointF &point )
{
    return m_controls.hitControls(point);
}

QPainterPath GraphicsShapeItem::createCtlsPath()
{
    QPainterPath path;
    for (int i=0; i<m_controls.count(); i++)
    {
        QPainterPath tmpPath;
        NControlPos pos = m_controls.ctrlPosByIndex(i);
        if (pos == Center || pos >= LineP1)
        {
            tmpPath.addEllipse(m_controls.controls(pos));
        }
        else
        {
            tmpPath.addRect(m_controls.controls(pos));
        }
        path += tmpPath;
    }

    return path;
}

QLineF GraphicsShapeItem::getArrowLine( qreal &width )
{
    return QLineF();
}

void GraphicsShapeItem::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
{
    
}

QVariant GraphicsShapeItem::shapeData( DataType type )
{
    return QVariant();
}

QPainterPath GraphicsShapeItem::getOutSidePath()
{
    QPainterPath path;
    QPainterPathStroker ps;
    ps.setWidth(2);
    path = ps.createStroke(getShapePath());
    return path;
}

QPointF GraphicsShapeItem::simplifyPoint( const QPointF &point )
{
    qreal x,y;
    qreal decimalX = point.x()-(int)point.x();
    qreal decimalY = point.y()-(int)point.y();
    if (decimalX<0.1) x = (int)point.x();
    else if (decimalX>0.9) x = (int)point.x()+1;
    else x = point.x();

    if (decimalY<0.1) y = (int)point.y();
    else if (decimalY>0.9) y = (int)point.y()+1;
    else y = point.y();

    return QPointF(x,y);
}

GraphicsShapeItem& GraphicsShapeItem::operator=( const GraphicsShapeItem& other )
{
    m_controls = other.m_controls;
    m_bResized = other.m_bResized;
    m_bShowArrow = other.m_bShowArrow;
    m_arrowHead    = other.m_arrowHead   ;
    m_shapePath    = other.m_shapePath   ;    // 图形样式,paint()中使用
    m_outsidePath  = other.m_outsidePath ;  // 外框轨迹(用在shape中)
    m_cltsRectPath = other.m_cltsRectPath;  // 控制点矩形
    setPos(other.pos());
    setTransform(other.transform());
    setTransformOriginPoint(other.transformOriginPoint());
    setRotation(other.rotation());
    return *this;
}

bool GraphicsShapeItem::setShapeData( const QVariant &data )
{
    return true;
}

QVariant GraphicsShapeItem::shapeData()
{
    return QVariant();
}

void GraphicsShapeItem::setLineWidth( double dWidth )
{
    m_dLineWidth = dWidth;
}


void GraphicsShapeItem::setLineColor(QColor color)
{
    m_color = color;
}

void GraphicsShapeItem::setControlWidth(qreal width/*=ControlMaxWidth*/)
{
    m_controls.setControlWidth(width);
}

void GraphicsShapeItem::setMoveEnable(bool bEnable)
{
    m_bIsEnableMove = bEnable;
    m_controls.setCtrlEnable(bEnable);
    m_controls.clearCtrls();
    setFlag(QGraphicsItem::ItemIsMovable, bEnable);
}
