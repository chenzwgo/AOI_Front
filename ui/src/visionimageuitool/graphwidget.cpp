#include "graphwidget.h"
#include "graphscene.h"
#include <QtGui>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
    , m_backgroundGrid(false)
    , m_drawCross(false)
    , m_dRadius(0)
    , m_pImage(NULL)
{
    GraphScene *scene = new GraphScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    //setMinimumSize(400, 400);
    this->setMouseTracking(true);
    m_grid = createGrid(1);

    m_pThreadSaveImage = new CSaveImageThread;
    if (NULL != m_pThreadSaveImage)
    {
        m_pThreadSaveImage->start();
    }
}

GraphWidget::~GraphWidget()
{
    if (NULL != m_pThreadSaveImage)
    {
        if (m_pThreadSaveImage->isRunning())
        {
            m_pThreadSaveImage->stopRunning();
        }
        while (m_pThreadSaveImage->isRunning())
        {
            Sleep(50);
        }
        delete m_pThreadSaveImage;
        m_pThreadSaveImage = NULL;
    }
    if (NULL != m_pImage)
    {
        delete m_pImage;
        m_pImage = NULL;
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, event->delta() / 240.0));
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    // Shadow
    QRectF sceneRect = this->sceneRect();
    if (m_backgroundPixmap.isNull())
    {
        if (m_backgroundGrid)
        {
            painter->setBrush(m_grid);
        }
        else
        {
            painter->setBrush(Qt::lightGray);
        }
        painter->drawRect(sceneRect);
    }
    else
    {
        // back ground image
        painter->drawPixmap(sceneRect, m_backgroundPixmap, QRect(0.0, 0.0, sceneRect.width(), sceneRect.height()));
    }
    if (m_drawCross)
    {
        QPen newPen;
        QPen oldPen = painter->pen();
        newPen.setColor(QColor(Qt::red));
        painter->setPen(newPen);
        QPointF top(sceneRect.width()/2,0);
        QPointF left(0,sceneRect.height()/2);
        QPointF bottom(sceneRect.width()/2,sceneRect.height());
        QPointF right(sceneRect.width(),sceneRect.height()/2);
        QPointF center(sceneRect.center());
        double  dRadius = sceneRect.height()/40;
        if (m_dRadius > 0)
        {
            dRadius = m_dRadius;
        }
        QRectF rect((sceneRect.width()/2-dRadius),sceneRect.height()/2 - dRadius, 2*dRadius,2*dRadius);
        painter->drawRect(rect);
        painter->drawEllipse(center, dRadius, dRadius);
        painter->drawLine(top, bottom);
        painter->drawLine(left, right);
        painter->setPen(oldPen);
    }
    QGraphicsView::drawBackground(painter, rect);
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    double dMinScale = 0.01;
    double dMaxScale = 120;
    QTransform t = transform();
    if (factor<dMinScale)
    {
        t.setMatrix(dMinScale,t.m12(),t.m13(),t.m21(), dMinScale,t.m23(), t.m31(), t.m32(),t.m33());
        setTransform(t);
    }
    else if (factor > dMaxScale)
    {
        t.setMatrix(dMaxScale,t.m12(),t.m13(),t.m21(), dMaxScale,t.m23(), t.m31(), t.m32(),t.m33());
        setTransform(t);
    }
    else
    {
        scale(scaleFactor, scaleFactor);
    }

    qreal radio = transform().m11();
    // 创建背景网格
    m_grid = createGrid(radio);

    // 更新显示窗口显示比例
    sceneScaleChanged(radio);
}

void GraphWidget::slot_zoomIn()
{
    qDebug()<<transform();
    scaleView(qreal(1.2));
}

void GraphWidget::slot_zoomOut()
{
    qDebug()<<transform();
    scaleView(1 / qreal(1.2));
}

void GraphWidget::setBackgroundImage( const QString &imageFile )
{
    QPixmapCache::clear();
    m_backgroundPixmap.load(imageFile);
    scene()->setSceneRect(m_backgroundPixmap.rect());
    m_backgroundPixmap.scaled(m_backgroundPixmap.width(), m_backgroundPixmap.height());
    // 更新显示窗口显示比例
    sceneScaleChanged(transform().m11());
}
void GraphWidget::showPictureInLabel(unsigned char *pixelsR, unsigned char *pixelsG, unsigned char *pixelsB, long width, long height)
{
    QPixmapCache::clear();
    if (pixelsG == NULL || pixelsB == NULL)
    {
        if (m_pImage != NULL
            && (m_pImage->format() != QImage::Format_Indexed8
            || m_pImage->width() != width
            || m_pImage->height() != height))
        {
            delete m_pImage;
            m_pImage = NULL;
        }

        if (m_pImage == NULL)
        {
            m_pImage = new QImage(width, height, QImage::Format_Indexed8);
            QVector<QRgb> colors;
            for (int i=0; i<256; ++i)
            {
                colors.push_back(qRgb(i,i,i));
            }
            m_pImage->setColorTable(colors);
        }

        uchar* pPixData = m_pImage->bits();
        if (NULL != pPixData)
        {
            int iByteLength = m_pImage->bytesPerLine();
            for (long y = 0; y < height; ++y)
            {            
			    memcpy(pPixData + iByteLength*y, pixelsR + width * y, width);
            }
        }
        m_backgroundPixmap = QPixmap::fromImage(*m_pImage).scaled(width, height, Qt::KeepAspectRatio);
    }
    else
    {
        if (m_pImage != NULL 
            && (m_pImage->format() != QImage::Format_RGB32
            || m_pImage->width() != width
            || m_pImage->height() != height))
        {
            delete m_pImage;
            m_pImage = NULL;
        }

        if (m_pImage == NULL)
        {
            m_pImage = new QImage(width, height, QImage::Format_RGB32);
        }
        uchar* pPixData = m_pImage->bits();
        for (long y=0; y<height; ++y)
        {
            uchar* pPixX = pPixData + y * m_pImage->bytesPerLine();
            for (long x=0; x<width; ++x)
            {	
                ((uint *)pPixX)[x] = uint(255 << 24) | qRgb(*pixelsR, *pixelsG, *pixelsB);
                pixelsR++;
                pixelsG++;
                pixelsB++;
            }
        }
        //m_backgroundPixmap = QPixmap::fromImage(*m_pImage).scaled(width, height, Qt::KeepAspectRatio);
        m_backgroundPixmap = QPixmap::fromImage(*m_pImage).scaled(width, height);
    }
    //显示图片//第一次以原始大小显示
    scene()->update();
    scene()->setSceneRect(m_backgroundPixmap.rect());
    // 更新显示窗口显示比例
    sceneScaleChanged(transform().m11());
}

void GraphWidget::showEvent( QShowEvent *event )
{
    if (m_backgroundPixmap.isNull())
    {
        scene()->setSceneRect(0,0,width()*0.99,height()*0.99);
        scene()->setSceneRect(0,0,width(),height());
    }

    fitInView(sceneRect(), Qt::KeepAspectRatio);
    // 更新显示窗口显示比例
    sceneScaleChanged(transform().m11());
    return QGraphicsView::showEvent(event);
}

void GraphWidget::clearBackgroundImage()
{
    if (!m_backgroundPixmap.isNull())
    {
        m_backgroundPixmap = QPixmap();
        scene()->setSceneRect(0,0,width()*0.99,height()*0.99);
        scene()->setSceneRect(0,0,width(),height());
    }
}

void GraphWidget::showBackGroundGrid(bool bShow)
{
    m_backgroundGrid = bShow;
    if (!m_backgroundPixmap.isNull())
    {
        scene()->setSceneRect(0,0,width()*0.99,height()*0.99);
        scene()->setSceneRect(0,0,width(),height());
    }
}
void GraphWidget::showBackGroundCross( bool bShow , double radius/* = 0.0*/)
{
    m_drawCross = bShow;
    m_dRadius = radius;
    scene()->update();
}

QPixmap GraphWidget::createGrid( qreal ratio)
{
    QPixmap pixMap;
    if (ratio<4)
    {
        pixMap = QPixmap(":/image/background64X64.png");
    }
    else if (ratio>=4 && ratio <8)
    {
        pixMap = QPixmap(":/image/background32X32.png");
    }
    else if (ratio>=8 && ratio <16)
    {
        pixMap = QPixmap(":/image/background16X16.png");
    }
    else if (ratio>=16 && ratio <32)
    {
        pixMap = QPixmap(":/image/background8X8.png");
    }
    else if (ratio>=32 && ratio <64)
    {
        pixMap = QPixmap(":/image/background4X4.png");
    }
    else
    {
        pixMap = QPixmap(":/image/background2X2.png");
    }

    return pixMap;
}

int GraphWidget::getImageGray( const QPoint &point )
{
    QRgb rgb = m_backgroundPixmap.toImage().pixel(point);
    return qGray(rgb);
}

void GraphWidget::saveImage( const QString strPath )
{
    if (NULL != m_pImage)
    {
        QImage image(m_pImage->size(), QImage::Format_RGB32);
        QPainter painter(&image);
        painter.drawImage(m_pImage->rect(), *m_pImage);
        painter.setRenderHint(QPainter::Antialiasing, true);
        scene()->render(&painter);

        m_pThreadSaveImage->insertImageData(strPath, image);
    }
}

unsigned char * GraphWidget::getImageLabelPtr()
{
    return m_pImage->bits();
}

void GraphWidget::UpdateImage()
{
    m_backgroundPixmap = QPixmap::fromImage(*m_pImage).scaled(m_pImage->width(), m_pImage->height());
    scene()->update();
}

void GraphWidget::sceneScaleChanged(qreal radio)
{
    // 发送到状态栏
    emit sigScaleChanged(radio);

    //更新控制点大小
    GraphScene *pScene = dynamic_cast<GraphScene*>(scene());
    int iwidth = 24;
    //计算控制点大小
    //获取窗口的最小边大小/50 再转换为图像像素
    QSize widSize = size();
    iwidth = (width() > height() ? height() : width())/50;
    if (radio != 0)
    {
        iwidth /= radio;
    }
    pScene->setCtrlWidth(iwidth);
}

CSaveImageThread::CSaveImageThread(QObject *parent /*= 0*/)
{

}

CSaveImageThread::~CSaveImageThread()
{

}

void CSaveImageThread::insertImageData(QString strPath, QImage image)
{
    m_lock.lock();
    m_saveImageList.push_back(image);
    m_savePathList.push_back(strPath);
    m_lock.unlock();
}

void CSaveImageThread::stopRunning()
{
    m_bIsRunning = false;
}

void CSaveImageThread::run()
{
    QImage image;
    QString strPath;
    int iListSize = 0;
    m_bIsRunning = true;
    while (m_bIsRunning)
    {
        m_lock.lock();
        iListSize = m_saveImageList.size();
        if (iListSize > 0)
        {
            image = m_saveImageList.front();
            m_saveImageList.pop_front();
            strPath = m_savePathList.front();
            m_savePathList.pop_front();
        }
        m_lock.unlock();
        if (iListSize > 0)
        {
            image.save(strPath);
        }
        else
        {
            msleep(500);
        }
    };
}
