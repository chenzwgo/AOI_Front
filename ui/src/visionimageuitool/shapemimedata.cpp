#include "shapemimedata.h"
#include "shape/graphshapeitem.h"
#include <QGraphicsItem>

ShapeMimeData::ShapeMimeData( QList<QGraphicsItem * > items )
{
    foreach (QGraphicsItem *item , items )
    {
        GraphicsShapeItem *sp = qgraphicsitem_cast<GraphicsShapeItem*>(item);
        if (sp)
        {
            m_items.append(sp->copy());
        }
    }
}

ShapeMimeData::~ShapeMimeData()
{
    foreach (QGraphicsItem *item , m_items )
    {
        delete item;
    }
    m_items.clear();
}

QList<QGraphicsItem *> ShapeMimeData::items() const
{
    return m_items;
}