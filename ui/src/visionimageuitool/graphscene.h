#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QUndoCommand>
#include "shape/graphshapeitem.h"
#include "controlsdef.h"

// 重载场景类
class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphScene(QObject *parent = 0);
    void setOperateType(OPERATE_TYPE type); // 设置操作类型
    void setDrawType(ShapeType shape);       // 设置要绘制的图案
    OPERATE_TYPE operateType() {return m_operateType;}
    ShapeType drawShape() {return m_drawShape;}
    void joinedShape();  // 图形断点磁性连接
    void joinedPolyLine(); // 多线段闭合
    bool focusNext(bool next);
    void setUndoStack(QUndoStack *stack);   // 撤销重做堆栈

    bool addShapeItem(const QVariant &param, bool bEnableMove = true, int index = -1);          //添加图形实例
    QList<QGraphicsItem *> queryShapeItem();                            //获取所有图形实例
    bool removeShapeItem(int index, bool clear = false);                //删除图形实例, index为图形索引， clear为true时清空列表

    void setRoiMaxCount(int iCount = -1);           //设置允许的最大ROI个数， 默认-1为不限制
    void setLineWidth(double iWidth = 0.001);           //设置线宽
    void setCtrlWidth(int iWidth);           //设置线宽
    void setShapeColor(QColor color);           //设置线宽
    void setFontSize(int iWidth = 12);           //设置字体大小

    void setItemSelect(int index);               //设置指定item选中
    int  getItemSelect();                        //获取选中item
signals:
    void sigCoordinate(QPointF); // 坐标
    void sigUpdateGray(QPoint); // 灰度值
    void sig_items_moved(QList<QGraphicsItem*>, QList<QPointF>); // 图形元素移动
    void sigItemMoveFinished();
    void sigItemRBtDoubleCliecked(QPointF &);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );

private:
    // 挨着的两条线线头连接
    void joinLine(GraphicsShapeItem* pSelectLine, GraphicsShapeItem* pOrgLine, bool bBeginPos);

    GraphicsShapeItem* selectShape();  // 选中的图形
    // 覆盖点point的图形(exceptShape除外)
    GraphicsShapeItem* shapeAt(const QPointF &point, GraphicsShapeItem *exceptShape=NULL);
    // 挨着的两条线线头连接
    void joinShape(GraphicsShapeItem* pSelectShape,GraphicsShapeItem* pOrgShape, bool bBeginPos);

    // 创建图形实例
    GraphicsShapeItem *createShape(ShapeType shape);

    void ReleaseDraw();
private:
    ShapeType m_drawShape;  // 绘图类型
    OPERATE_TYPE m_operateType;  //操作类型
    QGraphicsItem *m_curItem;  // 当前图形
    QPointF m_lastPos;             // 最后点(用于是否有效绘制)
    bool m_bItemMove;              // 是否有Item移动
    QUndoStack *m_undoStack;   // 撤销重做堆栈
    QVariant m_oldData;        // 修改前item data
    double     m_dLineWidth;   //线宽
    QColor     m_color;
    int        m_FontSize;
};

class DiagramTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    //enum { Type = UserType + 3 };

    DiagramTextItem();

    int type() const
    {
        return Type;
    }

signals:
    void lostFocus(DiagramTextItem *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};
#endif
