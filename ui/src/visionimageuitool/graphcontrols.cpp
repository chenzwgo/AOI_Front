#include "graphcontrols.h"
#include <QDebug>

void GraphicsControls::addControls( NControlPos pos)
{
    if (m_bIsEnable)
    {
        m_ctlRects[pos] = QRectF(0, 0, m_iCtrlWidth, m_iCtrlWidth);
    }
}

void GraphicsControls::setControlWidth( qreal width/*=ControlWidth*/ )
{
    m_iCtrlWidth = width;
    foreach (NControlPos pos, m_ctlRects.keys())
    {
        QPointF center = m_ctlRects[pos].center();
        m_ctlRects[pos].setSize(QSizeF(m_iCtrlWidth,m_iCtrlWidth));
        m_ctlRects[pos].moveCenter(center);
    }
}

void GraphicsControls::updateControl( NControlPos pos, const QPointF &point )
{
    if (m_ctlRects.contains(pos))
    {
        updateControlsLastPos();
        m_ctlRects[pos].moveCenter(point);
    }
}

void GraphicsControls::updateControls( const QMap<NControlPos, QPointF> &points )
{
    foreach (NControlPos pos, points.keys())
    {
        updateControlsLastPos();
        m_ctlRects[pos].moveCenter(points[pos]);
    }
}

NControlPos GraphicsControls::hitControls( const QPointF &point )
{
    for (int i = m_ctlRects.keys().size()-1; i >= 0 ; --i)
    {
        NControlPos pos = m_ctlRects.keys().at(i);
        if (m_ctlRects[pos].contains(point))
        {
            return pos;
        }
    }
    return None;
}

QRectF GraphicsControls::controls( NControlPos pos ) const
{
    if (m_ctlRects.contains(pos))
    {
        return m_ctlRects[pos];
    }

    return QRectF();
}

bool GraphicsControls::contains( NControlPos pos )
{
    return m_ctlRects.contains(pos);
}

void GraphicsControls::setParent( QGraphicsItem *pItem )
{
    m_pItem = pItem;
}

QPointF GraphicsControls::controlsPos( NControlPos pos ) const
{
    if (m_pItem != NULL)
    {
        return m_pItem->mapToScene(m_ctlRects[pos].center());
    }
    
    return QPointF();
}

QPointF GraphicsControls::controlsLastPos( NControlPos pos ) const
{
    if (m_ctlLastPos.contains(pos))
    {
        return m_ctlLastPos[pos];
    }

    return QPointF();
}

void GraphicsControls::updateControlsLastPos()
{
    if (m_pItem != NULL)
    {
        foreach (NControlPos pos, m_ctlRects.keys())
        {
            m_ctlLastPos[pos] = m_pItem->mapToScene(m_ctlRects[pos].center());
        }
    }
}

NControlPos GraphicsControls::ctrlPosByIndex( int index )
{
    if (index >= 0 && index < m_ctlRects.size())
    {
        QMap<NControlPos, QRectF>::iterator iter = m_ctlRects.begin();
        return (iter+index).key();
    }
    return None;
}

void GraphicsControls::removeCtrl( NControlPos pos )
{
    if (m_ctlRects.contains(pos))
    {
        m_ctlRects.remove(pos);
        m_ctlLastPos.remove(pos);
    }
    
}

void GraphicsControls::clearCtrls()
{
    m_ctlRects.clear();
    m_ctlLastPos.clear();
}


void GraphicsControls::setCtrlEnable(bool enable)
{
    m_bIsEnable = enable;
}

qreal GraphicsControls::getControlWidth()
{
    return m_iCtrlWidth;
}












