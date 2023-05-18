#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include "diagramscene.h"

// 重载视图类
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
    GraphWidget(DiagramScene *scene,QWidget *parent = 0);
    ~GraphWidget();
    // 设置背景图片
    void setBackgroundImage(const QString &imageFile);
    void showPictureInLabel(unsigned char *pixelsR, unsigned char *pixelsG, unsigned char *pixelsB, long width, long height);
    // 清除背景图片
    void clearBackgroundImage();

    // 显示背景网格
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
    QPixmap createGrid(qreal ratio = 0);  // 创建网格线,ratio为放大系数
    void sceneScaleChanged(qreal radio); 
    void drawForeground(QPainter *painter, const QRectF &rect);
private:
    QPixmap m_backgroundPixmap; // 背景图像
    bool m_backgroundGrid;   // 背景网格
    bool m_drawCross;   // 十字中心
    double m_dRadius;   // 中心区域圆及外接矩形大小
    QPixmap m_grid;          // 网格线
    QImage* m_pImage;
    double  m_dMinRatio;
    DiagramScene *m_scene;
    QRectF  m_defectPos;
    bool    m_bIsMoveable;

};

#endif
