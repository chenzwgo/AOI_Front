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

// ������ͼ��
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
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
    QPixmap createGrid(qreal ratio = 0);  // ����������,ratioΪ�Ŵ�ϵ��

    void sceneScaleChanged(qreal radio);

    void ThreadSaveImage(void* param);
private:
    QPixmap m_backgroundPixmap; // ����ͼ��
    bool m_backgroundGrid;   // ��������
    bool m_drawCross;   // ʮ������
    double m_dRadius;   // ��������Բ����Ӿ��δ�С
    QPixmap m_grid;          // ������
    QImage* m_pImage;
    CSaveImageThread *m_pThreadSaveImage;
};

#endif
