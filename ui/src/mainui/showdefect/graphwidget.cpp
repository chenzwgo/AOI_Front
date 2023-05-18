#include "graphwidget.h"
#include "diagramscene.h"
#include <QtGui>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
    , m_backgroundGrid(false)
    , m_drawCross(false)
    , m_dRadius(0)
    , m_pImage(NULL)
    , m_dMinRatio(0.01)
    , m_bIsMoveable(true)
{
    //setMinimumSize(400, 600);
    this->setMouseTracking(true);
    m_grid = createGrid(1);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    //this->setSceneRect(0,0,8196,8196);//setScenRect();
    //this->setFixedSize(800,700);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//setStyleSheet("padding:0px;border:0px");
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

GraphWidget::GraphWidget(DiagramScene *scene,QWidget *parent)
    : QGraphicsView(parent)
    , m_backgroundGrid(false)
    , m_drawCross(false)
    , m_dRadius(0)
    , m_pImage(NULL)
    , m_dMinRatio(0.01)
    , m_bIsMoveable(true)
{
    initScene(scene);
    //setMinimumSize(400, 600);
    this->setMouseTracking(true);
    m_grid = createGrid(1);
    setTransformationAnchor (QGraphicsView::AnchorUnderMouse);
    //this->setSceneRect(0,0,8196,8196);//setScenRect();
    //this->setFixedSize(800,700);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//setStyleSheet("padding:0px;border:0px");
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

GraphWidget::~GraphWidget()
{
    if (NULL != m_pImage)
    {
        delete m_pImage;
        m_pImage = NULL;
    }
    if (NULL != m_scene)
    {
        delete m_scene;
        m_scene = NULL;
    }
}

void GraphWidget::initScene(DiagramScene *scene)
{
    if (NULL == scene)
    {
        return;
    }
    m_scene = NULL;
    m_scene = scene;
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(m_scene);
    connect(m_scene, SIGNAL(sigUpdteSize(const QRectF &)), this, SLOT(slotUpdateSize(const QRectF &)));
    connect(m_scene, SIGNAL(sigOperatypeChang(int)), this, SLOT(slotOperatorType(int)));
    connect(m_scene, SIGNAL(sigLeftButtonClicked(QRectF)), this, SLOT(slotUpdateSelectPos(QRectF)));
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    if (m_bIsMoveable)
    {
        scaleView(pow((double)2, event->delta() / 240.0));
    }
    else
    {
        fitInView(m_backgroundPixmap.rect(), Qt::KeepAspectRatio);
    }
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
  
    // Shadow
    //QRectF sceneRect = this->sceneRect();
   double iW, iH;
    m_scene->getNewDefet(iW, iH);
    QRectF sceneRect(0,0,iW,iH);

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
        double  dRadius = sceneRect.height() / 40;
        if (m_dRadius > 0)
        {
            dRadius = m_dRadius;
        }
        QRectF rect((sceneRect.width() / 2 - dRadius), sceneRect.height() / 2 - dRadius, 2 * dRadius, 2 * dRadius);
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
    double dScale = 0.1;
    QTransform t = transform();
    if (factor<m_dMinRatio)
    {
        //t.setMatrix(0.1,t.m12(),t.m13(),t.m21(), 0.1,t.m23(), t.m31(), t.m32(),t.m33());
        //setTransform(t);
    }
    else if (factor > 120)
    {
        t.setMatrix(120,t.m12(),t.m13(),t.m21(), 120,t.m23(), t.m31(), t.m32(),t.m33());
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
    //sceneScaleChanged(radio);
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
		memcpy(pPixData, pixelsR, width * height);

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
    if (!m_bIsMoveable)
    {
        fitInView(m_backgroundPixmap.rect(), Qt::KeepAspectRatio);
    }
}

void GraphWidget::showEvent( QShowEvent *event )
{
    double iW, iH;
    m_scene->getNewDefet(iW, iH);
    QRectF sceneRect(0,0,iW,iH);
    scene()->setSceneRect(sceneRect);
    /*if (m_backgroundPixmap.isNull())
    {
        scene()->setSceneRect(0,0,width()*0.99,height()*0.99);
        scene()->setSceneRect(0,0,width(),height());
    }*/
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

void GraphWidget::saveImage(const QString strFilePath)
{
	//m_backgroundPixmap.save(strFilePath);
	if (m_pImage == NULL)
	{
		return;
	}
	QImage image(m_pImage->size(), QImage::Format_RGB32);
	QPainter painter(&image);
	painter.drawImage(m_pImage->rect(), *m_pImage);
	painter.setRenderHint(QPainter::Antialiasing, true);
	scene()->render(&painter);
	image.save(strFilePath);
}

void GraphWidget::setMoveAble(bool bIsAble)
{
    m_bIsMoveable = bIsAble;
}

void GraphWidget::sceneScaleChanged(qreal radio)
{
    // 发送到状态栏
    emit sigScaleChanged(radio);

    //更新控制点大小
    //DiagramScene *pScene = dynamic_cast<DiagramScene*>(scene());
    int iwidth = 24;
    //计算控制点大小
    //获取窗口的最小边大小/50 再转换为图像像素
    QSize widSize = size();
    iwidth = (width() > height() ? height() : width())/50;
    if (radio != 0)
    {
        iwidth /= radio;
    }
    //pScene->setCtrlWidth(iwidth);
}

void GraphWidget::drawForeground(QPainter *painter, const QRectF &rect)
{
    if (m_defectPos.x() > 0 || m_defectPos.y() > 0)
    {
        double iW, iH;
        m_scene->getNewDefet(iW, iH);
        const QRectF sceneR = sceneRect();
//            (0, 0, iW, iH);
        //QRectF viewRect = QRectF(QPointF(), size());
        QSizeF viewRect = size();
        QPen newPen;
        QPen oldPen = painter->pen();
        newPen.setColor(QColor(Qt::red));
        double  dRadius = sceneR.height() / 40;
        if (m_dRadius > 0)
        {
            dRadius = m_dRadius;
        }
        newPen.setWidth(viewRect.height() > viewRect.width() ? viewRect.width() / (10000) : viewRect.height() / (10000));
        painter->setPen(newPen);
        QPointF pos(m_defectPos.center());
        QPointF top(pos.x(), 0);
        QPointF left(0, pos.y());
        QPointF bottom(pos.x(), sceneR.height());
        QPointF right(sceneR.width(), pos.y());
        //QRectF rectF((pos.x() - dRadius), pos.y() - dRadius, 2 * dRadius, 2 * dRadius);
        //painter->drawRect(rectF);
        //painter->drawEllipse(pos, dRadius, dRadius);
        QPointF pointTmp;
        pointTmp.setX(bottom.x());
        pointTmp.setY(m_defectPos.y());
        painter->drawLine(top, pointTmp);
        pointTmp.setX(bottom.x());
        pointTmp.setY(m_defectPos.y()+m_defectPos.height());
        painter->drawLine(pointTmp, bottom);

        pointTmp.setX(m_defectPos.x());
        pointTmp.setY(left.y());
        painter->drawLine(left, pointTmp);
        pointTmp.setX(m_defectPos.x() + +m_defectPos.width());
        pointTmp.setY(left.y());
        painter->drawLine(pointTmp, right);
        painter->drawRect(m_defectPos);
        painter->setPen(oldPen);
    }

    QGraphicsView::drawForeground(painter, rect);
}

void GraphWidget::slotUpdateSize(const QRectF &rect)
{
    double dW = rect.width();
    double dH = rect.height();
    double dMinSenceW = 600; //最小宽度；
    m_dMinRatio = dMinSenceW/dW;

    scene()->update();
    scene()->setSceneRect(rect);
    setTransform(QTransform());

    scaleView(m_dMinRatio);
    // 更新显示窗口显示比例
    sceneScaleChanged(transform().m11());
}

void GraphWidget::slotOperatorType(int iOperatorType)
{
    switch(iOperatorType)
    {
    case OT_Select:
        {
            // 多加这句,解决鼠标形状不更新的bug
            setDragMode(QGraphicsView::ScrollHandDrag);
            setDragMode(QGraphicsView::NoDrag);
            setCursor(Qt::ArrowCursor);
            break;
        }
    case OT_Move:
        {
            setDragMode(QGraphicsView::ScrollHandDrag);
            break;
        }
    case OT_None:
        {
            setDragMode(QGraphicsView::NoDrag);
            break;
        }
    default:
        break;
    }
}

void GraphWidget::slotUpdateSelectPos(const QRectF rectSelect)
{
    m_defectPos = rectSelect;
    if (m_scene->getOperateType() != OT_Move && rectSelect.x()>0)
    {
        centerOn(rectSelect.center());
    }
    scene()->update();
}

