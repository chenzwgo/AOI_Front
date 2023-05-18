#ifndef GRAPH_CONTROLS_H
#define GRAPH_CONTROLS_H

#include "controlsdef.h"
#include <QRectF>
#include <QDebug>

class GraphicsControls
{
public:
    GraphicsControls(): m_curPos(None),m_pItem(NULL), m_iCtrlWidth(ControlMaxWidth), m_bIsEnable(true){};
    void setParent(QGraphicsItem *pItem);
    void addControls(NControlPos pos);     // ���ӿ��Ƶ�
    void setControlWidth(qreal width=ControlMaxWidth);                  // ���ÿ��Ƶ������α䳤
    qreal getControlWidth();                  // ���ÿ��Ƶ������α䳤
    void updateControls(const QMap<NControlPos, QPointF> &points); // �������¿��Ƶ�
    void updateControl(NControlPos pos, const QPointF &point);     // ���¿��Ƶ�
    NControlPos hitControls(const QPointF &point);                 // ����Ŀ��Ƶ�λ��
    QRectF controls(NControlPos pos) const;                        // �õ����Ƶ����
    QPointF controlsPos(NControlPos pos) const;                     // �õ����Ƶ��ڳ����е�����
    QPointF controlsLastPos(NControlPos pos) const;                // �õ����Ƶ���һ���ڳ����е�����
    void updateControlsLastPos();                                  // ���¿��Ƶ���һ���ڳ����е�����
    void setPressedControl(NControlPos pos) { m_curPos = pos;}     // ���õ�ǰ��ѹ���Ƶ�
    NControlPos pressedControl() {return m_curPos;}                // ��ȡ��ǰ��ѹ���Ƶ�
    bool contains(NControlPos pos);                                // ����λ��pos
    int count() { return m_ctlRects.size(); }                      // ���ؿ��Ƶ����
    NControlPos ctrlPosByIndex(int index);                         // ͨ�������õ����Ƶ�ö��λ��
    void removeCtrl(NControlPos pos);                              // �Ƴ����Ƶ�
    void clearCtrls();                                             // ��տ��Ƶ�
    void setCtrlEnable(bool enable);
private:
    QMap<NControlPos, QRectF> m_ctlRects;     // ���Ƶ����
    QMap<NControlPos, QPointF> m_ctlLastPos;  // ���Ƶ����
    NControlPos m_curPos;
    QGraphicsItem *m_pItem;
    int         m_iCtrlWidth;                 //���Ƶ��С
    bool        m_bIsEnable;                  //�Ƿ�ʹ�ÿ��Ƶ�
};

#endif // GRAPHICS
