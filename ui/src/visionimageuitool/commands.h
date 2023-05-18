#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "controlsdef.h"
#include <QGraphicsScene>

class MoveCommand : public QUndoCommand
{
public:
    enum { Id = 1234 };

    MoveCommand(QList<QGraphicsItem*> diagramItems, const QList<QPointF> &oldPos,
                QUndoCommand *parent = 0);

    void undo();
    void redo();
    //bool mergeWith(const QUndoCommand *command);
    int id() const { return Id; }

private:
    QList<QGraphicsItem*> m_diagramItems;
    QList<QPointF> m_oldPos;
    QList<QPointF> m_newPos;
};

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(QList<QGraphicsItem*> diagramItems, QUndoCommand *parent = 0);

    void undo();
    void redo();

private:
    QList<QGraphicsItem*> m_diagramItems;
    QGraphicsScene *m_graphicsScene;
};

class AddCommand : public QUndoCommand
{
public:
    AddCommand(QList<QGraphicsItem*> diagramItems, QUndoCommand *parent = 0);

    void undo();
    void redo();

private:
    QList<QGraphicsItem*> m_diagramItems;
    QGraphicsScene *m_graphicsScene;
    QList<QPointF> m_initialPosition;
};

class ModifyCommand : public QUndoCommand
{
public:
    ModifyCommand(QVariant oldData, QGraphicsItem* item, QUndoCommand *parent = 0);
    void undo();
    void redo();

private:
    QVariant m_oldData;
    QVariant m_newData;
    QGraphicsItem* m_item;
    QGraphicsScene *m_graphicsScene;
};

QString createCommandString(QGraphicsItem *item, const QPointF &point);

#endif
