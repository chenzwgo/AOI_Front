#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QThread>
#include <QMutex>

class CSaveImageThread : public QThread
{
public:
    CSaveImageThread(QObject *parent = 0);
    ~CSaveImageThread();

    void insertImageData(QString strPath, QImage image);
    void stopRunning();
    void run();
private:
    QList<QImage> m_saveImageList;
    QList<QString> m_savePathList;
    QMutex m_lock;
    bool   m_bIsRunning;
};

// 重载视图类
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
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
    void saveImage(const QString strPath);
    unsigned char *getImageLabelPtr();
    void UpdateImage();
signals:
    void sigScaleChanged(qreal);

public slots:
    void slot_zoomIn();
    void slot_zoomOut();

protected:
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void scaleView(qreal scaleFactor);
    void showEvent(QShowEvent *event);
    QPixmap createGrid(qreal ratio = 0);  // 创建网格线,ratio为放大系数

    void sceneScaleChanged(qreal radio);

    void ThreadSaveImage(void* param);
private:
    QPixmap m_backgroundPixmap; // 背景图像
    bool m_backgroundGrid;   // 背景网格
    bool m_drawCross;   // 十字中心
    double m_dRadius;   // 中心区域圆及外接矩形大小
    QPixmap m_grid;          // 网格线
    QImage* m_pImage;
    CSaveImageThread *m_pThreadSaveImage;
};

#endif
