#include "graphscene.h"
#include "shape/graphlineitem.h"
#include "shape/graphrectitem.h"
#include "shape/graphrectangle1item.h"
#include "shape/graphpolyitem.h"
#include "shape/grapharcitem.h"
#include "shape/grapharccenteritem.h"
#include "shape/graphcircleitem.h"
#include "shape/graphcirclecenteritem.h"
#include "shape/grapheraseitem.h"
#include <QtGui>
#include "rtdefine.h"
#include "pointalgorithm.h"
#include "commands.h"
#include <QGraphicsView>
#include <QGraphicsTextItem>
#define SHAPE_ITEM_INDEX_DATA_KEY   0

GraphScene::GraphScene( QObject *parent /*= 0*/ )
    : QGraphicsScene(parent),
    m_drawShape(EnShape_None),
    m_operateType(OT_Select),
    m_curItem(NULL),
    m_bItemMove(false),
    m_undoStack(NULL),
    m_dLineWidth(0.001),
    m_color(Qt::red)
{
}

void GraphScene::mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent )
{
    if (!sceneRect().contains(mouseEvent->scenePos()))
    {
        return;
    }

    emit sigCoordinate(mouseEvent->scenePos());
    emit sigUpdateGray(mouseEvent->scenePos().toPoint());
    update();

    // 绘图模式屏蔽item鼠标事件
    if (!m_curItem && m_operateType == OT_Draw)
    {
        return;
    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphScene::mousePressEvent( QGraphicsSceneMouseEvent *mouseEvent )
{
    if (m_curItem)
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
        return;
    }

    if (mouseEvent->button() == Qt::MiddleButton)
    {
        m_operateType = OT_Move;
        return;
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        m_operateType = OT_Select;
        return;
    }
    
    if (m_operateType == OT_Draw)
    {
        m_lastPos = mouseEvent->scenePos();
        clearSelection();
        //GraphicsShapeItem *curItem = dynamic_cast<GraphicsShapeItem*>(itemAt(m_lastPos));
        //if (NULL != curItem)
        //{
        //    m_operateType = OT_Select;
        //    QGraphicsScene::mousePressEvent(mouseEvent);
        //    return;
        //}
        if (m_drawShape == DiagramTextItem::Type)
        {
            DiagramTextItem *pItem = new DiagramTextItem();
            QFont font = pItem->font();
            font.setPointSize(m_FontSize);
            pItem->setFont(font);
            pItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            pItem->setZValue(1000.0);
            connect(pItem, SIGNAL(lostFocus(DiagramTextItem*)),
                this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(pItem, SIGNAL(selectedChange(QGraphicsItem*)),
                this, SIGNAL(itemSelected(QGraphicsItem*)));
            pItem->setBoundingRegionGranularity(0);
            pItem->setDefaultTextColor(m_color);
            pItem->setFlag(QGraphicsItem::ItemIsMovable, true);
            pItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
            pItem->setFlag(QGraphicsItem::ItemIsFocusable, true);
            m_curItem = pItem;
            addItem(m_curItem);
            m_curItem->setSelected(true);
            m_curItem->setPos(m_lastPos);
        }
        else
        {
            m_curItem = createShape(m_drawShape); // 创建实例
            if (m_curItem)
            {
                GraphicsShapeItem *pShapeItem = dynamic_cast<GraphicsShapeItem*>(m_curItem);
                pShapeItem->setLineWidth(m_dLineWidth);
                pShapeItem->setLineColor(m_color);
                addItem(m_curItem);
                m_curItem->setSelected(true);
                m_curItem->grabMouse();
                m_curItem->setPos(m_lastPos);
            }
        }
    }
    else if (m_operateType == OT_Select)
    {
        m_lastPos = mouseEvent->scenePos();
        QTransform deviceTransform;
        m_bItemMove = (NULL != itemAt(m_lastPos, deviceTransform));
        views().first()->setDragMode(QGraphicsView::RubberBandDrag);
    }

    update();
    QGraphicsScene::mousePressEvent(mouseEvent);

    // 记录修改前数据
    if (selectedItems().size() == 1)
    {
        GraphicsShapeItem *pShape = dynamic_cast<GraphicsShapeItem*>(selectedItems().first());
        if (pShape)
        {
            m_oldData = pShape->shapeData();
        }
    }
    //QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *mouseEvent )
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    bool bNew = false;
    if (m_operateType == OT_Draw && m_curItem)
    {
        ReleaseDraw();
        bNew = true;
    }
    else if (m_operateType == OT_Select)
    {
        // item有移动，发信号放入移动队列,用于撤销操作
        if (m_bItemMove && m_lastPos != mouseEvent->scenePos())
        {
            QList<QGraphicsItem *> itemList = selectedItems();
            if (itemList.isEmpty()) return;

            GraphicsShapeItem* firstShape = dynamic_cast<GraphicsShapeItem*>(itemList.first());
            if (itemList.size()==1 && firstShape && firstShape->isResized())
            {
                // 加入撤销队列
                QUndoCommand *addCommand = new ModifyCommand(m_oldData, firstShape);
                m_undoStack->push(addCommand);
            }
            else
            {
                QList<QPointF> posList;
                foreach (QGraphicsItem *pItem, itemList)
                {
                    QPointF oldPos = pItem->scenePos()-mouseEvent->scenePos()+m_lastPos;
                    posList.append(oldPos);
                }

                // 加入撤销队列
                QUndoCommand *addCommand = new MoveCommand(itemList, posList);  
                m_undoStack->push(addCommand);
            }
        }

        views().first()->setDragMode(QGraphicsView::NoDrag);
    }

    joinedPolyLine();  // 多线段头尾磁性连接
    joinedShape();     // 各图形磁性连接
    update();
    //抛出鼠标移动释放事件
    if (bNew || m_bItemMove && m_lastPos != mouseEvent->scenePos())
    {
        emit sigItemMoveFinished();
    }
}
void GraphScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    if (mouseEvent->button() == Qt::RightButton)
    {
        //获取像素坐标并抛出去
        emit sigItemRBtDoubleCliecked(mouseEvent->scenePos());
    }
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GraphScene::setDrawType( ShapeType shape )
{
    m_drawShape = shape;
}

void GraphScene::setOperateType( OPERATE_TYPE type )
{
    // 模式切换,如果有图形正在绘制,删除掉
    if (m_operateType == OT_Select && NULL != m_curItem)
    {
        removeItem(m_curItem);
        m_curItem = NULL;
    }
    m_operateType = type;
}

void GraphScene::joinLine( GraphicsShapeItem* pSelectShape, GraphicsShapeItem* pOrgLine, bool bBeginPos )
{
    RETURN_IF(pSelectShape==NULL || pOrgLine==NULL);
    // 选中的线端点
    NControlPos beginPos = pSelectShape->ctrlPos(0);
    NControlPos endPos = pSelectShape->ctrlPos(pSelectShape->ctlsCount()-1);
    NControlPos curPos = bBeginPos ? beginPos : endPos;

    // 要吸附的线端点
    NControlPos orgBeginPos = pOrgLine->ctrlPos(0);
    NControlPos orgEndPos = pOrgLine->ctrlPos(pOrgLine->ctlsCount()-1);

    QPointF lastP = pSelectShape->lastPointAt(curPos);
    QPointF curP = pSelectShape->pointAt(curPos);

    QPointF p1 = pSelectShape->pointAt(beginPos);
    QPointF p2 = pSelectShape->pointAt(endPos);
    QPointF orgP1 = pOrgLine->pointAt(orgBeginPos);
    QPointF orgP2 = pOrgLine->pointAt(orgEndPos);
    QLineF orgLine(orgP1, orgP2);

    QPointF pNearest = PMethod::verticalPointToLine(curP,orgLine); // 线段上离点p最近的点
    if (curP == pNearest) return;
    if (PMethod::distanceP2P(pNearest,orgP1) <= AdSorbDistance)
    {
        pNearest = orgP1;
    }
    else if (PMethod::distanceP2P(pNearest,orgP2) <= AdSorbDistance)
    {
        pNearest = orgP2;
    }

    if (PMethod::distanceP2P(curP,pNearest) > PMethod::distanceP2P(lastP, pNearest)) return; // 远离

    // 判断是移动还是改变
    if (pSelectShape->isResized())
    {
        pSelectShape->resizeShape(curPos, pNearest); // 改变线头
    }
    else
    {
        QPointF offset = pNearest-curP;
        pSelectShape->moveBy(offset.x(), offset.y());
    }
}

void GraphScene::joinedShape()
{
    GraphicsShapeItem *pShape = selectShape();
    RETURN_IF(NULL == pShape);

    QPointF p1 = pShape->pointAt(pShape->ctrlPos(0));
    QPointF p2 = pShape->pointAt(pShape->ctrlPos(pShape->ctlsCount()-1));

    // 线头连接
    GraphicsShapeItem *pShapeOrg = shapeAt(p1,pShape);
    if(pShapeOrg)
    {
        if (pShapeOrg->type() == EnShape_Line)
        {
            joinLine(pShape,pShapeOrg, true);
        }
        else
        {
            joinShape(pShape, pShapeOrg,true);
        }
        
    }

    // 线尾连接
    pShapeOrg = shapeAt(p2,pShape);
    if(pShapeOrg)
    {
        if (pShapeOrg->type() == EnShape_Line)
        {
            joinLine(pShape,pShapeOrg, false);
        }
        else
        {
            joinShape(pShape, pShapeOrg,false);
        }
    }
}

GraphicsShapeItem* GraphScene::selectShape()
{
    QList<QGraphicsItem*> selectItems = selectedItems();
    foreach (QGraphicsItem* item, selectItems)
    {
        return dynamic_cast<GraphicsShapeItem*>(item);
    }
    return NULL;
}

GraphicsShapeItem* GraphScene::shapeAt( const QPointF &point, GraphicsShapeItem *exceptShape/*=NULL*/ )
{
    QList<QGraphicsItem*> graphItems = items(point);
    foreach (QGraphicsItem *item, graphItems)
    {
        if (item != exceptShape)
        {
            return dynamic_cast<GraphicsShapeItem*>(item);
        }
    }

    return NULL;
}

void GraphScene::joinShape( GraphicsShapeItem* pSelectShape,GraphicsShapeItem* pOrgShape, bool bBeginPos )
{
    RETURN_IF(pSelectShape==NULL || pOrgShape==NULL);
    NControlPos beginPos = pSelectShape->ctrlPos(0);
    NControlPos endPos = pSelectShape->ctrlPos(pSelectShape->ctlsCount()-1);
    NControlPos curPos = bBeginPos ? beginPos : endPos;

    QPointF lastP = pSelectShape->lastPointAt(curPos);
    QPointF curP = pSelectShape->pointAt(curPos);

    QPointF orgP1 = pOrgShape->pointAt(pOrgShape->ctrlPos(0));
    QPointF orgP2 = pOrgShape->pointAt(pOrgShape->ctrlPos(pOrgShape->ctlsCount()-1));
    RETURN_IF(orgP1==curP || orgP2==curP);

    QPointF modifyP;
    if (PMethod::distanceP2P(curP,orgP1) <= AdSorbDistance)
    {
        modifyP = orgP1;
    }
    else if (PMethod::distanceP2P(curP,orgP2) <= AdSorbDistance)
    {
        modifyP = orgP2;
    }
    else
    {
        return;
    }

    if (PMethod::distanceP2P(curP,modifyP) > PMethod::distanceP2P(lastP, modifyP)) return; // 远离

    // 判断是移动还是改变
    if (pSelectShape->isResized())
    {
        pSelectShape->resizeShape(curPos, modifyP); // 改变线头
    }
    else
    {
        QPointF offset = modifyP - curP;
        pSelectShape->moveBy(offset.x(), offset.y());
    }
}

void GraphScene::joinedPolyLine()
{
    GraphicsShapeItem *pShape = selectShape();
    RETURN_IF(NULL == pShape || pShape->type() != EnShape_Polygon);
    RETURN_IF(!pShape->isResized());

    NControlPos beginPos = pShape->ctrlPos(0);
    NControlPos endPos = pShape->ctrlPos(pShape->ctlsCount()-1);
    QPointF lastP1 = pShape->lastPointAt(beginPos);
    QPointF lastP2 = pShape->lastPointAt(endPos);
    QPointF p1 = pShape->pointAt(beginPos);
    QPointF p2 = pShape->pointAt(endPos);

    RETURN_IF(p1==p2);
    qreal lastDis = PMethod::distanceP2P(lastP1,lastP2);
    qreal curDis = PMethod::distanceP2P(p1,p2);
    if (curDis<AdSorbDistance && curDis<=lastDis)
    {
        pShape->resizeShape(endPos, p1); // 改变线头
    }
}

GraphicsShapeItem * GraphScene::createShape( ShapeType shape)
{
    switch (shape)
    {
    case EnShape_Line:
        {
            return new GraphicsLineItem();
        }
    case EnShape_Rect:
        {
            return new GraphicsRectItem();
        }
    case EnShape_Rect1:
        {
            return new GraphRectangle1Item();
        }
    case EnShape_Polygon:
        {
            return new GraphicsPolyItem();
        }
    case EnShape_Arc:
        {
            return new GraphicsArcItem();
        }
    case EnShape_Arc_Center:
        {
            return new GraphicsArcCenterItem();
        }
    case EnShape_Circle:
        {
            return new GraphicsCircleItem();
        }
    case EnShape_Circle_Center:
        {
            return new GraphicsCircleCenterItem();
        }
    case EnShape_Erase_Path:
        {
            return new GraphicsEraseItem();
        }
    default:
        break;
    }

    return NULL;
}

bool GraphScene::focusNext( bool next )
{
    QList<QGraphicsItem *> pSelectItem = selectedItems();
    if (pSelectItem.isEmpty())
    {
        if (!items().isEmpty())
        {
            clearSelection();
            items().first()->setSelected(true);
        }
    }
    else
    {
        QGraphicsItem *pItem = pSelectItem.first();
        QList<QGraphicsItem *> listItems = items();
        int index = listItems.indexOf(pItem);

        if (next)
        {
            index++;
            if (index == listItems.count())
            {
                index = 0;
            }
        }
        else
        {
            index--;
            if (index == -1)
            {
                index = listItems.count()-1;
            }
        }

        clearSelection();
        listItems.at(index)->setSelected(true);
    }
    return true;
}

void GraphScene::setUndoStack( QUndoStack *stack )
{
    m_undoStack = stack;
}

bool GraphScene::addShapeItem(const QVariant &param, bool bEnableMove/* = true*/, int index/* = -1*/)
{
    clearSelection();
    m_operateType = OT_Draw;
    QList<QVariant> listArrary = param.toList();
    m_drawShape = (ShapeType)listArrary.at(0).toInt();
    m_curItem = createShape(m_drawShape); // 创建实例
    if (m_curItem)
    {
        m_curItem->setData(SHAPE_ITEM_INDEX_DATA_KEY, index);
        GraphicsShapeItem *pShapeItem = dynamic_cast<GraphicsShapeItem*>(m_curItem);
        pShapeItem->setMoveEnable(bEnableMove);  //设置是否可移动
        pShapeItem->setLineWidth(m_dLineWidth);  //设置线宽
        pShapeItem->setLineColor(m_color);       //设置显示颜色
        addItem(m_curItem);
        m_curItem->setSelected(true);
        pShapeItem->setShapeData(param);
        m_lastPos = m_curItem->pos();
        m_curItem = NULL;
        m_operateType = OT_Select;
    }
    return true;
}

QList<QGraphicsItem *> GraphScene::queryShapeItem()
{
    return items();
}

bool GraphScene::removeShapeItem( int index, bool clear /*= false*/ )
{
    QList<QGraphicsItem *> list = items();
    if (index >= list.size())
    {
        return false;
    }
    if (clear)
    {
        for (QList<QGraphicsItem *>::iterator it = list.begin(); it != list.end(); it++)
        {
            removeItem(*it);
        }
        m_curItem = NULL;
    }
    else
    {
        QGraphicsItem *pItem = list.at(index);
        if (pItem)
        {
            m_curItem = NULL;
            removeItem(pItem);
        }
    }
    return true;
}

void GraphScene::ReleaseDraw()
{
    if (NULL == m_curItem)
    {
        m_operateType = OT_Select;
        return;
    }
    GraphicsShapeItem *pShapeItem = dynamic_cast<GraphicsShapeItem*>(m_curItem);
    bool bIsUserType = false;
    if (m_curItem->type() >= m_curItem->UserType)
    {
        bIsUserType = true;
    }

    if (bIsUserType && !pShapeItem->isShapeValid()) // 是否有效
    {
        removeItem(m_curItem);
        m_curItem = NULL;
        m_operateType = OT_Select;
        return;
    }

    if (!bIsUserType || pShapeItem->isDrawFinished()) // 绘制完成
    {
        m_curItem->ungrabMouse();
        m_curItem->setSelected(true);
        // 加入撤销队列
        QList<QGraphicsItem*> itemList;
        itemList.push_back(m_curItem);
        QUndoCommand *addCommand = new AddCommand(itemList);  
        m_undoStack->push(addCommand);

        m_operateType = OT_Select;
        m_curItem = NULL;
    }
}

void GraphScene::setLineWidth( double dWidth /*= -1*/ )
{
    m_dLineWidth = dWidth;
}

void GraphScene::setCtrlWidth( int iWidth )
{
    foreach (QGraphicsItem *pItem, items())
    {
        if (pItem->type() >= pItem->UserType)
        {
            GraphicsShapeItem *pShapeItem = dynamic_cast<GraphicsShapeItem*>(pItem);
            pShapeItem->setControlWidth(iWidth);
            pShapeItem->updateControls();
        }
    }
}

void GraphScene::setShapeColor(QColor color)
{
    m_color = color;
}

void GraphScene::setFontSize(int iWidth /*= 12*/)
{
    m_FontSize = iWidth;
}


void GraphScene::setItemSelect(int index)
{
    clearSelection();
    m_curItem = NULL;
    QList<QGraphicsItem *> listItems = items();
    for (int i = 0; i < listItems.size();i++)
    {
        QGraphicsItem *pItem = listItems[i];
        if (NULL != pItem && index == pItem->data(SHAPE_ITEM_INDEX_DATA_KEY))
        {
            pItem->setSelected(true);
            m_curItem = pItem;
            break;
        }
    }
}

int GraphScene::getItemSelect()
{
    int index = -1;
    QList<QGraphicsItem *> listItems = selectedItems();
    if (!listItems.isEmpty())
    {
        index = listItems.first()->data(SHAPE_ITEM_INDEX_DATA_KEY).toInt();
    }
    return index;
}

DiagramTextItem::DiagramTextItem()
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}
//! [0]

//! [1]
QVariant DiagramTextItem::itemChange(GraphicsItemChange change,
    const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);
    return value;
}
//! [1]

//! [2]
void DiagramTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
//! [2]

//! [5]
void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}