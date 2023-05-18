#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include "diagramscene.h"

// ������ͼ��
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
    GraphWidget(DiagramScene *scene,QWidget *parent = 0);
    ~GraphWidget();
    // ���ñ���ͼƬ
    void setBackgroundImage(const QString &imageFile);
    void showPictureInLabel(unsigned char *pixelsR, unsigned char *pixelsG, unsigned char *pixelsB, long width, long height);
    // �������ͼƬ
    void clearBackgroundImage();

    // ��ʾ��������
    void showBackGroundGrid( bool bShow);
    void showBackGroundCross( bool bShow, double radius = 0.0);
    
    int  getImageGray(const QPoint &point);

	void saveImage(const QString strFilePath);
    void setMoveAble(bool bIsAble);
    void initScene(DiagramScene *);
signals:
    void sigScaleChanged(qreal);

public slots:
    void slot_zoomIn();
    void slot_zoomOut();
    void slotUpdateSize(const QRectF &rect);
    void slotOperatorType(int iOperatorType);
    void slotUpdateSelectPos(const QRectF pos);
protected:
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void scaleView(qreal scaleFactor);
    void showEvent(QShowEvent *event);
    QPixmap createGrid(qreal ratio = 0);  // ����������,ratioΪ�Ŵ�ϵ��
    void sceneScaleChanged(qreal radio); 
    void drawForeground(QPainter *painter, const QRectF &rect);
private:
    QPixmap m_backgroundPixmap; // ����ͼ��
    bool m_backgroundGrid;   // ��������
    bool m_drawCross;   // ʮ������
    double m_dRadius;   // ��������Բ����Ӿ��δ�С
    QPixmap m_grid;          // ������
    QImage* m_pImage;
    double  m_dMinRatio;
    DiagramScene *m_scene;
    QRectF  m_defectPos;
    bool    m_bIsMoveable;

};

#endif
