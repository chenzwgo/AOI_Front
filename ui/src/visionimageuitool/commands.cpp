#include <QtGui>

#include "commands.h"
#include "shape/graphlineitem.h"
#include "shape/graphrectitem.h"
#include "shape/graphpolyitem.h"
#include "shape/grapharcitem.h"
#include "shape/grapharccenteritem.h"
#include "shape/graphcircleitem.h"
#include "shape/graphcirclecenteritem.h"

MoveCommand::MoveCommand(QList<QGraphicsItem*> diagramItems, const QList<QPointF> &oldPos,
                 QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_diagramItems = diagramItems;
    foreach (QGraphicsItem *pItem, diagramItems)
    {
        m_newPos.push_back(pItem->pos());
    }
    m_oldPos = oldPos;
}

//bool MoveCommand::mergeWith(const QUndoCommand *command)
//{
//    const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
//    QList<QGraphicsItem *> items = moveCommand->m_diagramItems;
//
//    if (m_diagramItems != items)
//    return false;
//
//    QList<QPointF> newPos;
//    foreach (QGraphicsItem *pItem, items)
//    {
//        newPos.push_back(pItem->pos());
//    }
//
//    m_newPos = newPos;
//    setText(QObject::tr("Move %1")
//        .arg(createCommandString(m_diagramItems.first(), m_newPos.first())));
//
//    return true;
//}

void MoveCommand::undo()
{
    for (int i=0; i<m_diagramItems.count() && i< m_oldPos.count(); i++)
    {
        m_diagramItems.at(i)->setPos(m_oldPos.at(i));
    }

    m_diagramItems.first()->scene()->update();
    setText(QObject::tr("Move %1")
        .arg(createCommandString(m_diagramItems.first(), m_newPos.first())));
}

void MoveCommand::redo()
{
    for (int i=0; i<m_diagramItems.count() && i< m_newPos.count(); i++)
    {
        m_diagramItems.at(i)->setPos(m_newPos.at(i));
    }

    setText(QObject::tr("Move %1")
        .arg(createCommandString(m_diagramItems.first(), m_newPos.first())));
}

DeleteCommand::DeleteCommand(QList<QGraphicsItem*> diagramItems, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_diagramItems = diagramItems;
    m_graphicsScene = diagramItems.first()->scene();
    setText(QObject::tr("Delete %1")
        .arg(createCommandString(m_diagramItems.first(), m_diagramItems.first()->pos())));
}

void DeleteCommand::undo()
{
    foreach (QGraphicsItem *pItem, m_diagramItems)
    {
        m_graphicsScene->addItem(pItem);
    }
    m_graphicsScene->update();
}

void DeleteCommand::redo()
{
    foreach (QGraphicsItem *pItem, m_diagramItems)
    {
        m_graphicsScene->removeItem(pItem);
    }
}

AddCommand::AddCommand(QList<QGraphicsItem*> diagramItems, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_diagramItems = diagramItems;
    m_graphicsScene = diagramItems.first()->scene();
    foreach(QGraphicsItem *pItem, diagramItems)
    {
        m_initialPosition.push_back(pItem->scenePos());
    }
    setText(QObject::tr("Add %1")
        .arg(createCommandString(m_diagramItems.first(), m_initialPosition.first())));
}

void AddCommand::undo()
{
    foreach(QGraphicsItem *pItem, m_diagramItems)
    {
        m_graphicsScene->removeItem(pItem);
    }
    
    m_graphicsScene->update();
}

void AddCommand::redo()
{
    for(int i=0; i<m_diagramItems.count() && i<m_initialPosition.count(); i++)
    {
        if (m_diagramItems.at(i)->scene() == NULL)
        {
            m_graphicsScene->addItem(m_diagramItems.at(i));
            m_diagramItems.at(i)->setPos(m_initialPosition.at(i));
        }
    }

    m_graphicsScene->update();
}

QString createCommandString(QGraphicsItem *item, const QPointF &pos)
{
    StShapeType strType;
    return QObject::tr("%1 at (%2, %3)")
        .arg(strType.typeMap[item->type()])
        .arg(pos.x()).arg(pos.y());
}

ModifyCommand::ModifyCommand( QVariant oldData, QGraphicsItem* item, QUndoCommand *parent /*= 0*/ )
{
    m_item = item;
    GraphicsShapeItem *pItem = dynamic_cast<GraphicsShapeItem*>(item);
    if (NULL != pItem)
    {
        m_newData = pItem->shapeData();
    }
    m_oldData = oldData;
    m_graphicsScene = pItem->scene();
    setText(QObject::tr("Modify %1")
        .arg(createCommandString(m_item, m_item->pos())));
}

void ModifyCommand::undo()
{
    GraphicsShapeItem *pItem = dynamic_cast<GraphicsShapeItem*>(m_item);
    pItem->setShapeData(m_oldData);
}

void ModifyCommand::redo()
{
    GraphicsShapeItem *pItem = dynamic_cast<GraphicsShapeItem*>(m_item);
    pItem->setShapeData(m_newData);
}
