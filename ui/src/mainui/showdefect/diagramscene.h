#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramtextitem.h"
#include "maindefine.h"
#include <string>
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class GraphicsShapeItem;
class Chip;
QT_END_NAMESPACE

//! [0]
enum OPERATE_TYPE{
    OT_None=0,      // ��
    OT_Draw,        // ��ͼ
    OT_Move,        // ƽ��
    OT_Select,      // ѡ��
    OT_ZoomRect,    // �ֲ��Ŵ�
};
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    DiagramScene(enScene iScene = Scene_First,QObject *parent = 0);
    //enum Mode { InsertItem, InsertLine, InsertText, MoveItem };
    //��������̶���С����
    void setNewDefet(double width,double height);//
    void getNewDefet(double& width,double& height);
    ~DiagramScene(){};
    //ȱ��λ������ʾ
    void Addsetdecet(MarkStrut mark);

    void setCtrlWidth( int iWidth );
	//

    //
    void AddPolygon(QPolygon polygon);

    OPERATE_TYPE getOperateType();
public slots:
    void editorLostFocus(DiagramTextItem *item);
    void slotSetNewDefet(double width,double height);
    void slotAddsetdecet(MarkStrut *mark);
    void slotAddVecsetdecet(QVector<MarkStrut*>* vecmark);
    //
     void slotAddMapVecsetdecet(QMap<QString, QVector<MarkStrut *> *> * mapMark ,int iScne );
     void slotChooseShow(int *showChoose ,int iScene );
    //
	 void slotAddDefectRes(stDefectFeature Defect,int iScne);
     void slotAddRoiInfo(QRect roi, int iScne);
	 void slotClearDefect();
signals:
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);
    void sigCoordinate(QPointF); // ����
    void sigUpdteSize(const QRectF &rect); // 
    void sigOperatypeChang(int iOperatorType);
    void sigRightButtonClicked(QPointF);
    void sigLeftButtonClicked(QRectF);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    bool ShowChooseMark(QMap<QString, QVector<MarkStrut *> *>* pMap);
	// add 20180613 begin
    void AddsetdecetBySequence(stDefectFeature Defect);
	void setDefect(stDefectFeature defect);
    void addROIRect(QRect roi);
	void ClearDefect();
	void ShowChoose();
    //add 20180613 end

    bool isItemChange(int type);
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    QFont myFont;
    DiagramTextItem *textItem;
    Chip *mChip;
    int m_iW;
    int m_iH;
    GraphicsShapeItem *m_curItem;  // ��ǰͼ��
    OPERATE_TYPE m_operateType;
    QList<Chip*> m_ListchipItem;
    QMap<QString, QVector<MarkStrut *> *>* M_qMapMark;
    vector<string> m_strSequence;//ʱ������
    int m_iCheckShow[8];        //�����ʾʱ��
    enScene m_iScne;
	//add 20180613 begin
	//QList<stDefectFeature> mListA_Defec;
	//QList<stDefectFeature> mListB_Defec;
	//QList<stDefectFeature> mListC_Defec;
	//QList<stDefectFeature> mListD_Defec;
	//
	//QList<stDefectFeature> mListE_Defec;
	//QList<stDefectFeature> mListF_Defec;
	//QList<stDefectFeature> mListG_Defec;
	//QList<stDefectFeature> mListH_Defec;

	//
	vector<QList<stDefectFeature>> m_vListDefec;
	//
	//add 20180613end
	bool m_bUnion;//union�汾
};
//! [0]

#endif
