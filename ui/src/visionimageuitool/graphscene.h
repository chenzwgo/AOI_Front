#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QUndoCommand>
#include "shape/graphshapeitem.h"
#include "controlsdef.h"

// ���س�����
class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphScene(QObject *parent = 0);
    void setOperateType(OPERATE_TYPE type); // ���ò�������
    void setDrawType(ShapeType shape);       // ����Ҫ���Ƶ�ͼ��
    OPERATE_TYPE operateType() {return m_operateType;}
    ShapeType drawShape() {return m_drawShape;}
    void joinedShape();  // ͼ�ζϵ��������
    void joinedPolyLine(); // ���߶αպ�
    bool focusNext(bool next);
    void setUndoStack(QUndoStack *stack);   // ����������ջ

    bool addShapeItem(const QVariant &param, bool bEnableMove = true, int index = -1);          //���ͼ��ʵ��
    QList<QGraphicsItem *> queryShapeItem();                            //��ȡ����ͼ��ʵ��
    bool removeShapeItem(int index, bool clear = false);                //ɾ��ͼ��ʵ��, indexΪͼ�������� clearΪtrueʱ����б�

    void setRoiMaxCount(int iCount = -1);           //������������ROI������ Ĭ��-1Ϊ������
    void setLineWidth(double iWidth = 0.001);           //�����߿�
    void setCtrlWidth(int iWidth);           //�����߿�
    void setShapeColor(QColor color);           //�����߿�
    void setFontSize(int iWidth = 12);           //���������С

    void setItemSelect(int index);               //����ָ��itemѡ��
    int  getItemSelect();                        //��ȡѡ��item
signals:
    void sigCoordinate(QPointF); // ����
    void sigUpdateGray(QPoint); // �Ҷ�ֵ
    void sig_items_moved(QList<QGraphicsItem*>, QList<QPointF>); // ͼ��Ԫ���ƶ�
    void sigItemMoveFinished();
    void sigItemRBtDoubleCliecked(QPointF &);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * mouseEvent );

private:
    // ���ŵ���������ͷ����
    void joinLine(GraphicsShapeItem* pSelectLine, GraphicsShapeItem* pOrgLine, bool bBeginPos);

    GraphicsShapeItem* selectShape();  // ѡ�е�ͼ��
    // ���ǵ�point��ͼ��(exceptShape����)
    GraphicsShapeItem* shapeAt(const QPointF &point, GraphicsShapeItem *exceptShape=NULL);
    // ���ŵ���������ͷ����
    void joinShape(GraphicsShapeItem* pSelectShape,GraphicsShapeItem* pOrgShape, bool bBeginPos);

    // ����ͼ��ʵ��
    GraphicsShapeItem *createShape(ShapeType shape);

    void ReleaseDraw();
private:
    ShapeType m_drawShape;  // ��ͼ����
    OPERATE_TYPE m_operateType;  //��������
    QGraphicsItem *m_curItem;  // ��ǰͼ��
    QPointF m_lastPos;             // ����(�����Ƿ���Ч����)
    bool m_bItemMove;              // �Ƿ���Item�ƶ�
    QUndoStack *m_undoStack;   // ����������ջ
    QVariant m_oldData;        // �޸�ǰitem data
    double     m_dLineWidth;   //�߿�
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
