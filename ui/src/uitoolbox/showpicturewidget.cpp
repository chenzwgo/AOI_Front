#include <QPushButton>
#include <QHBoxLayout>
#include <QFile>
#include <QScrollArea>
#include <QLabel>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QScrollBar>

#include "uitoolbox/showpicturewidget.h"

ShowPicture::ShowPicture(QWidget *parent)
    : QWidget(parent)
	, m_imgScaled(NULL)
	, m_bMouseButtonPress(false)
	, m_CurWidth(-1)
	, m_CurHeight(-1)
    , m_rotate(0)
    , m_scaleX(1)
    , m_scaleY(1)
{
    QHBoxLayout *m_layButton = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    m_reduceButton = new QPushButton(QString::fromLocal8Bit("缩小"));
    m_enlargedButton = new QPushButton(QString::fromLocal8Bit("放大"));
    m_turnLeftButton = new QPushButton(QString::fromLocal8Bit("左旋转"));
    m_turnRightButton = new QPushButton(QString::fromLocal8Bit("右旋转"));
    m_adjustButton = new QPushButton(QString::fromLocal8Bit("自适应"));
	m_saveButton = new QPushButton(QString::fromLocal8Bit("保存"));
    //初始化所有的按钮不可用
    m_reduceButton->setEnabled(false);
    m_enlargedButton->setEnabled(false);
    m_turnLeftButton->setEnabled(false);
    m_turnRightButton->setEnabled(false);
    m_adjustButton->setEnabled(false);
	m_saveButton->setEnabled(false);
	//hiddenButton(true);

    connect(m_enlargedButton,SIGNAL(clicked()),this,SLOT(slotEnlargePicture()));
    connect(m_reduceButton,SIGNAL(clicked()),this,SLOT(slotReducePicture()));
    connect(m_turnLeftButton,SIGNAL(clicked()),this,SLOT(slotTurnLeftPicture()));
    connect(m_turnRightButton,SIGNAL(clicked()),this,SLOT(slotTurnRightPicture()));
    connect(m_adjustButton,SIGNAL(clicked()),this,SLOT(slotAdjustPicture()));
	connect(m_saveButton,SIGNAL(clicked()),this,SLOT(slotSavePicture()));

    m_layButton->addWidget(m_enlargedButton);
    m_layButton->addWidget(m_reduceButton);
    m_layButton->addWidget(m_turnLeftButton);
    m_layButton->addWidget(m_turnRightButton);
    m_layButton->addWidget(m_adjustButton);
	m_layButton->addWidget(m_saveButton);

	m_imageLabel = new CoordinateLabel;
	m_imageLabel->setAlignment(Qt::AlignCenter);
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(1);
    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidget(m_imageLabel);
	mainLayout->addWidget(m_scrollArea);
    mainLayout->addLayout(m_layButton);
	mainLayout->setMargin(0);
    setLayout(mainLayout);

	m_scrollArea->viewport()->installEventFilter(this);
	m_imageLabel->installEventFilter(this);

	connect(m_scrollArea->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(slotHScrollBarRangeChange(int,int)));
	connect(m_scrollArea->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(slotVScrollBarRangeChange(int,int)));
}

ShowPicture::~ShowPicture()
{

    if(NULL != m_imgScaled)
    {
        delete m_imgScaled;
		m_imgScaled = NULL;
    }

}

//还原自适应大小
void ShowPicture::slotAdjustPicture()
{
    if(NULL == m_imgScaled)
        return;

	QSize imgSize(m_CurWidth, m_CurHeight);
	QSize scrollSize = m_scrollArea->size() - QSize(2,2);
	QSize adjustSize;

	if (imgSize.width()*scrollSize.height() < scrollSize.width()*imgSize.height())
	{
		adjustSize.setHeight(scrollSize.height());
		adjustSize.setWidth(scrollSize.height()*imgSize.width()/imgSize.height());
	}
	else
	{
		adjustSize.setHeight(scrollSize.width()*imgSize.height()/imgSize.width());
		adjustSize.setWidth(scrollSize.width());
	}

	m_CurWidth = adjustSize.width();
	m_CurHeight = adjustSize.height();

	m_scaleX = (qreal)m_CurWidth/m_imgScaled->size().width();
    m_scaleY = (qreal)m_CurHeight/m_imgScaled->size().height();
    QPixmap saveImage;
	saveImage.convertFromImage(m_imgScaled->transformed(m_matrix).scaled(m_CurWidth,m_CurHeight,Qt::KeepAspectRatio));
    m_imageLabel->setPixmap(saveImage);

	m_enlargedButton->setEnabled(true);
	m_reduceButton->setEnabled(true);
}

void ShowPicture::slotEnlargePicture()
{
	if(NULL == m_imgScaled)
		return;

	QSize imgSize = m_imgScaled->size();
	if(imgSize.width() > maxWidth || imgSize.height() > maxHeight)
	{
		m_enlargedButton->setEnabled(false);
		m_reduceButton->setEnabled(true);
		return;
	}

	QSize scrollSize = m_scrollArea->viewport()->size();
    scaleImage(scrollSize.width()/2, scrollSize.height()/2, 0.2);
}

void ShowPicture::slotReducePicture()
{
	if(NULL == m_imgScaled)
		return;

	QSize imgSize = m_imgScaled->size();
	if(imgSize.width() < minWidth || imgSize.height() < minHeight)
	{
		m_enlargedButton->setEnabled(true);
		m_reduceButton->setEnabled(false);
		return;
	}

	QSize scrollSize = m_scrollArea->viewport()->size();
	scaleImage(scrollSize.width()/2, scrollSize.height()/2, -0.2);
}

void ShowPicture::scaleImage(long x, long y, double factor)
{
	if (NULL == m_imgScaled)
		return;

	QSize oldSize(m_CurWidth, m_CurHeight);
 	QSize viewSize = m_scrollArea->viewport()->size();
 	QPoint viewPoint = m_scrollArea->viewport()->mapToGlobal(QPoint(0,0)) - m_imageLabel->mapToGlobal(QPoint(0,0));
	QSize newSize = oldSize * (1+factor);
    if ((newSize.width() < oldSize.width() || newSize.height() < oldSize.height())
        && (newSize.width() <= minWidth || newSize.height() < minHeight))
    {
        return;
    }

	bool bEnlarge = true;
	bool bReduce = true;
	if (newSize.width() > newSize.height())    //设置最大放大高度
	{
		if (newSize.width() > maxWidth)
		{
			factor = 1.0 * maxWidth / oldSize.width() - 1;
			newSize.setHeight((1+factor) * oldSize.height());
			newSize.setWidth(maxWidth);
			bEnlarge = false;
		}
		else if (newSize.width() < minWidth)
		{
			factor = 1.0 * minWidth / oldSize.width() - 1;
			newSize.setHeight((1+factor) * oldSize.height());
			newSize.setWidth(minWidth);
			bReduce = false;
		}
	}
	else
	{
		if (newSize.height() > maxHeight)
		{
			factor = 1.0 * maxHeight / oldSize.height() - 1;
			newSize.setHeight(maxHeight);
			newSize.setWidth((1+factor) * oldSize.width());
			bEnlarge = false;
		}
		else if (newSize.height() < minHeight)
		{
			factor = 1.0 * minHeight / oldSize.height() - 1;
			newSize.setHeight(minHeight);
			newSize.setWidth((1+factor) * oldSize.width());
			bReduce = false;
		}
	}

	m_enlargedButton->setEnabled(bEnlarge);
	m_reduceButton->setEnabled(bReduce);

	m_ScaleX1 = 1.0 * x / m_imageLabel->width();//oldSize.width();
	m_ScaleY1 = 1.0 * y / m_imageLabel->height();//oldSize.height();
	m_ScaleX2 = 1.0 * (x - viewPoint.x()) / newSize.width();
	m_ScaleY2 = 1.0 * (y - viewPoint.y()) / newSize.height();

	m_CurWidth = newSize.width();
	m_CurHeight = newSize.height();

    m_scaleX = (qreal)m_CurWidth/m_imgScaled->size().width();
    m_scaleY = (qreal)m_CurHeight/m_imgScaled->size().height();


	QImage saveImage = m_imgScaled->transformed(m_matrix).scaled(newSize,Qt::KeepAspectRatio);
    m_imageLabel->setPixmap(QPixmap::fromImage(saveImage));
    m_imageLabel->setScaled(m_scaleX, m_scaleY);
}

void ShowPicture::dragImage(QPoint mousePos)
{
	QSize widgetSize = m_imageLabel->size();
	QScrollBar *hBar = m_scrollArea->horizontalScrollBar();
	QScrollBar *vBar = m_scrollArea->verticalScrollBar();
	int hMaximum = hBar->maximum() - hBar->minimum() + hBar->pageStep();
	int vMaximun = vBar->maximum() - vBar->minimum() + vBar->pageStep();

	QPoint diffPoint = m_MousePos - mousePos;
	double diffScrollX = 1.0 * diffPoint.x() / widgetSize.width() * hMaximum;
	double diffScrollY = 1.0 * diffPoint.y() / widgetSize.height() * vMaximun;
	hBar->setSliderPosition(hBar->value() + diffScrollX);
	vBar->setSliderPosition(vBar->value() + diffScrollY);

}

void ShowPicture::slotTurnLeftPicture()
{
    if(NULL == m_imgScaled)
        return;

	QMatrix matrix;
	matrix.rotate(270);
	m_matrix.rotate(270);
    m_rotate = (m_rotate+270)%360;
	QPixmap newPixmap = m_imageLabel->pixmap()->transformed(matrix);
	m_imageLabel->setPixmap(newPixmap);
}

void ShowPicture::slotTurnRightPicture()
{
    if(NULL == m_imgScaled)
        return;

    QMatrix matrix;
	matrix.rotate(90);
	m_matrix.rotate(90);
    m_rotate = (m_rotate+90)%360;
	QPixmap newPixmap = m_imageLabel->pixmap()->transformed(matrix);
    m_imageLabel->setPixmap(newPixmap);
}

void ShowPicture::showPictureInLabel(QString file_path)
{
	m_matrix.reset();
	if(NULL != m_imgScaled)
	{
		delete m_imgScaled;
		m_imgScaled = NULL;
	}
	m_imageLabel->setPixmap(QPixmap());

	if(!QFile::exists(file_path))
	{
		return;
	}

	QImage* img=new QImage;
    if(!img->load(file_path) ) //加载图像
    {
        //如果加载失败，则提示并返回
        QMessageBox::information(this,QString::fromLocal8Bit("打开图像失败"),QString::fromLocal8Bit("打开图像失败!"));
        delete img;
        return;
    }

    m_imgScaled = img;
	m_CurWidth = img->width();
	m_CurHeight = img->height();

    //显示图片//第一次以原始大小显示
    m_imageLabel->setPixmap(QPixmap::fromImage(*m_imgScaled));

    //使放大，缩小按钮可用
    m_reduceButton->setEnabled(true);
    m_enlargedButton->setEnabled(true);
    m_turnLeftButton->setEnabled(true);
    m_turnRightButton->setEnabled(true);
    m_adjustButton->setEnabled(true);
	m_saveButton->setEnabled(true);
}

void ShowPicture::showPictureInLabel(unsigned char *pixelsR, unsigned char *pixelsG, unsigned char *pixelsB, long width, long height)
{
	if (pixelsR == NULL || width <= 0 || height <= 0)
	{
		if(NULL != m_imgScaled)
		{
			delete m_imgScaled;
			m_imgScaled = NULL;
		}
		m_imageLabel->setPixmap(QPixmap());
		m_CurWidth = -1;
		m_CurHeight = -1;
		return;
	}

	if(m_imgScaled != NULL && (m_imgScaled->width() != width || m_imgScaled->height() != height))
	{
		delete m_imgScaled;
		m_imgScaled = NULL;
	}

	if (pixelsG == NULL || pixelsB == NULL)
	{
		if (m_imgScaled != NULL && m_imgScaled->format() != QImage::Format_Indexed8)
		{
			delete m_imgScaled;
			m_imgScaled = NULL;
		}

		if (m_imgScaled == NULL)
		{
			m_imgScaled = new QImage(width, height, QImage::Format_Indexed8);
			QVector<QRgb> colors;
			for (int i=0; i<256; ++i)
			{
				colors.push_back(qRgb(i,i,i));
			}
			m_imgScaled->setColorTable(colors);
		}

        uchar* pPixData = m_imgScaled->bits();
		for (long y=0; y<height; ++y)
        {
            uchar* pPixX = pPixData + y * m_imgScaled->bytesPerLine();
			for (long x=0; x<width; ++x)
			{	
                pPixX[x] = *pixelsR;
				pixelsR++;
			}
		}
	}
	else
	{
		if (m_imgScaled != NULL && m_imgScaled->format() != QImage::Format_RGB32)
		{
			delete m_imgScaled;
			m_imgScaled = NULL;
		}

		if (m_imgScaled == NULL)
		{
			m_imgScaled = new QImage(width, height, QImage::Format_RGB32);
		}

        uchar* pPixData = m_imgScaled->bits();
		for (long y=0; y<height; ++y)
        {
            uchar* pPixX = pPixData + y * m_imgScaled->bytesPerLine();
			for (long x=0; x<width; ++x)
            {	
                ((uint *)pPixX)[x] = uint(255 << 24) | qRgb(*pixelsR, *pixelsG, *pixelsB);
				pixelsR++;
				pixelsG++;
				pixelsB++;
			}
		}
	}
	if (m_CurWidth < 0)
	{
		m_CurWidth = width;
	}
	if (m_CurHeight < 0)
	{
		m_CurHeight = height;
	}

	//显示图片//第一次以原始大小显示
	m_imageLabel->setPixmap(QPixmap::fromImage(*m_imgScaled).transformed(m_matrix).scaled(m_CurWidth, m_CurHeight, Qt::KeepAspectRatio));

	//使放大，缩小按钮可用
	m_reduceButton->setEnabled(true);
	m_enlargedButton->setEnabled(true);
	m_turnLeftButton->setEnabled(true);
	m_turnRightButton->setEnabled(true);
	m_adjustButton->setEnabled(true);
	m_saveButton->setEnabled(true);
}

//提供保存图片接口
void ShowPicture::slotSavePicture()
{
	savePicture();
}
//提供保存图片接口
bool ShowPicture::savePicture(QString file_path)
{
    if(m_imgScaled == NULL)
        return false;

    if(!file_path.isEmpty())
    {
        if(!QFile::exists(file_path))
        {
            QMessageBox::information(this,"ERROR",file_path + "is not exist!");
            return false;
        }

    }
    else
    {
        QString strFilter;
        file_path = QFileDialog::getSaveFileName(this, "Save As", ".", tr("BMP (*.bmp);;JPEG (*.jpg)"), &strFilter);
        if (!file_path.isEmpty())
        {
            if (strFilter == "BMP (*.bmp)" && file_path.indexOf(".bmp") == -1)
            {
                file_path += ".bmp";
            }
            else if (strFilter == "JPEG (*.jpg)" && file_path.indexOf(".jpg") == -1)
            {
                file_path += ".jpg";
            } 
        }
        else
        {
            return true;
        }
    }
	m_imgScaled->save(file_path);
    return true;
}

void ShowPicture::clearPicture()
{
	if(NULL != m_imgScaled)
	{
		delete m_imgScaled;
		m_imgScaled = NULL;
	}

	//显示图片//第一次以原始大小显示
	m_imageLabel->setPixmap(QPixmap());

	m_reduceButton->setEnabled(false);
	m_enlargedButton->setEnabled(false);
	m_turnLeftButton->setEnabled(false);
	m_turnRightButton->setEnabled(false);
	m_adjustButton->setEnabled(false);
	m_saveButton->setEnabled(false);
}

void ShowPicture::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ShowPicture::hiddenButton(bool isHidden)
{
    if(isHidden)
    {
        m_reduceButton->setHidden(true);
        m_enlargedButton->setHidden(true);
        m_turnLeftButton->setHidden(true);
        m_turnRightButton->setHidden(true);
        m_adjustButton->setHidden(true);
		m_saveButton->setHidden(true);

    }
    else
    {
        m_reduceButton->setHidden(false);
        m_enlargedButton->setHidden(false);
        m_turnLeftButton->setHidden(false);
        m_turnRightButton->setHidden(false);
        m_adjustButton->setHidden(false);
		m_saveButton->setHidden(false);
    }
}

void ShowPicture::adjustPictureSize()
{
    slotAdjustPicture();
}

long ShowPicture::GetWidgetWinID()
{
	return (long )m_imageLabel->winId();
}

bool ShowPicture::eventFilter( QObject *target, QEvent *event )
{
	if (target == m_scrollArea->viewport())
	{
		if (event->type() == QEvent::Wheel)
		{
			QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
			if (wheelEvent != NULL)
			{
				QPoint oldMousePos = wheelEvent->globalPos() - m_imageLabel->mapToGlobal(QPoint(0,0));
				double nDelta = wheelEvent->delta() / 360.0;
				scaleImage(oldMousePos.x(), oldMousePos.y(), nDelta);
				return true;
			}
		}
		else if (event->type() == QEvent::MouseButtonPress)
		{      
			QMouseEvent* mouseEvent = static_cast<QMouseEvent* >(event);
			if(mouseEvent != NULL && (mouseEvent->button() & Qt::LeftButton) )
			{
				setCursor(Qt::OpenHandCursor);
				m_MousePos = mouseEvent->globalPos();
				m_bMouseButtonPress = true;
			}    
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{      
			QMouseEvent* mouseEvent = static_cast<QMouseEvent* >(event);
			if(mouseEvent != NULL && (mouseEvent->button() & Qt::LeftButton) )
			{
				setCursor(Qt::ArrowCursor);
				m_bMouseButtonPress = false;
			}    
		}
		else if (event->type() == QEvent::MouseMove)
		{      
			QMouseEvent* mouseEvent = static_cast<QMouseEvent* >(event);
			if(mouseEvent != NULL && m_bMouseButtonPress)
			{
				QPoint gPos = mouseEvent->globalPos();
				dragImage(gPos);
				m_MousePos = gPos;
			}    
		}
	}

	return QWidget::eventFilter(target,event);
}

void ShowPicture::slotHScrollBarRangeChange( int min, int max )
{
	QScrollBar *hBar = m_scrollArea->horizontalScrollBar();
	int hMaximum = hBar->maximum() - hBar->minimum() + hBar->pageStep();
	if (hBar->isVisible())
	{
		hBar->setValue(hBar->minimum() + (int)((m_ScaleX1 - m_ScaleX2)*hMaximum));
	}
}

void ShowPicture::slotVScrollBarRangeChange( int min, int max )
{
	QScrollBar *vBar = m_scrollArea->verticalScrollBar();
	int vMaximun = vBar->maximum() - vBar->minimum() + vBar->pageStep();
	if (vBar->isVisible())
	{
		vBar->setValue(vBar->minimum() + (int)((m_ScaleY1 - m_ScaleY2)*vMaximun));
	}
}

ShowPictureWidget::ShowPictureWidget(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QGridLayout();
    mainLayout->setMargin(0);

    setLayout(mainLayout);
}

ShowPictureWidget::~ShowPictureWidget()
{

}

void ShowPictureWidget::setShowPicture(int row, int column)
{
    m_row = row;
    m_column = column;

    if(m_allWidget.count() > 0)
    {
        for(int i = 0; i < m_allWidget.count(); i++)
        {
            mainLayout->removeWidget(m_allWidget[i]);
            delete m_allWidget[i];
            m_allWidget[i] = NULL;
        }
    }

    m_allWidget.clear();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            ShowPicture *addWidget = new ShowPicture;
            mainLayout->addWidget(addWidget,i,j);
            m_allWidget.append(addWidget);

        }
    }
}

ShowPicture *ShowPictureWidget::getSpecifyWidget(int row,int column)
{
    if(row > m_row-1 || column > m_column-1)
    {
        return NULL;
    }
    int index = row * m_column + column;
    return m_allWidget[index];
}
void ShowPictureWidget::hiddenSpecifyWidget(int row,int column,bool isHidden)
{
    if(row > m_row-1 || column > m_column-1)
    {
        QMessageBox::information(this,"ERROR","get widget fail!");
        return ;
    }

    int index = row * (mainLayout->columnCount()) + column + 1;
    m_allWidget[index]->setHidden(isHidden);
    return;
}

void ShowPictureWidget::resizeWindowsSize()
{
	for (int i = 0; i < m_allWidget.size(); ++i) 
	{
		if(m_allWidget.at(i) != NULL)
		{
			m_allWidget.at(i)->adjustPictureSize();			
		}
	}
}



void CoordinateLabel::mouseDoubleClickEvent( QMouseEvent *e )
{
    if(e->button() == Qt::RightButton)
    {
        QPointF pointF = e->pos();
        QPoint  point = e->pos();
        emit signalMousePosF(pointF);
        emit signalMousePos(point);
    }
}

CoordinateLabel::CoordinateLabel( QWidget *parent /*= 0*/ ) : m_dAngleOrigin(0)
{
    m_point.setX(0);
    m_point.setY(0);
    m_pointOrigin.setX(0);
    m_pointOrigin.setY(0);
    m_dScaledX = 0;
    m_dScaledY = 0;
    m_pointDest = QPoint(0,0);
    setCursor(QPixmap("plugins/uitoolbox/mouse.png"));
}

CoordinateLabel::~CoordinateLabel()
{

}

//void CoordinateLabel::paintEvent( QPaintEvent * e )
//{
//    QLabel::paintEvent(e);
//    QPainter painter(this);
//    QPen pen(Qt::blue, 3);
//    painter.setPen(pen);
//    if (m_point.x() > 0 || m_point.y())
//    {
//        QRect rect(m_point.x() -5, m_point.y()-5, 10, 10);
//        painter.drawRect(rect);
//    }
//    if (m_pointOriginF.x() > 0 || m_pointOriginF.y() > 0 || m_dAngleOrigin > 0)
//    {
//        QPointF pOrigin = updateCoordinateData(m_pointOriginF);
//        QSize widSize = size();
//        QPointF xStartPoint,yStartPoint;
//        QPointF xEndPoint, yEndPoint;
//
//        double dXStartX = 0.0;
//        double dXStartY = 0.0;
//        double dYStartX = 0.0;
//        double dYStartY = 0.0;
//        double dXEndX = widSize.width();
//        double dXEndY = 0.0;
//        double dYEndX = 0.0;
//        double dYEndY = widSize.height();
//
//        if (m_dAngleOrigin > 0 && m_dAngleOrigin < 90)
//        {
//            double dDeg = (m_dAngleOrigin * 3.14)/180;
//            dYStartX = (0 - pOrigin.y())/tan(dDeg+1.57) + pOrigin.x();
//            dYEndX = (dYEndY - pOrigin.y())/tan(dDeg+1.57) + pOrigin.x();
//
//            //dYStartX = m_dXOrigin;
//            //dYEndX = m_dXOrigin;
//
//            dXStartY = (0 - pOrigin.x())*tan(dDeg) + pOrigin.y();
//            dXEndY = (dXEndX - pOrigin.x())*tan(dDeg) + pOrigin.y();
//        }
//        else
//        {
//            dXStartY = pOrigin.y();
//            dYStartX = pOrigin.x();
//            dXEndY = pOrigin.y();
//            dYEndX = pOrigin.x();
//        }
//
//        //计算起点与终点
//        xStartPoint.setX(dXStartX);
//        xStartPoint.setY(dXStartY);
//        yStartPoint.setY(dYStartY);
//        yStartPoint.setX(dYStartX);
//
//        xEndPoint.setX(dXEndX);
//        xEndPoint.setY(dXEndY);
//        yEndPoint.setX(dYEndX);
//        yEndPoint.setY(dYEndY);
//
//        painter.drawLine(xStartPoint, xEndPoint);
//        painter.drawLine(yStartPoint, yEndPoint);
//    }
//    if (0 < m_pointDest.x() || 0 < m_pointDest.y())
//    {
//        QPointF destPoint = updateCoordinateData(m_pointDest);
//        painter.drawRect(destPoint.x() - 5, destPoint.y() - 5, 10, 10);
//        painter.drawPoint(destPoint);
//    }
//}

void CoordinateLabel::showPicPos( const QPoint &pos )
{
    m_point = pos;
    update();
}

void CoordinateLabel::showPicPosF( const QPointF &pos )
{
    m_pointF = pos;
    update();
}

void CoordinateLabel::setCoordinateOrigin( const double x, const double y, const double angle )
{
    m_pointOriginF.setX(x);
    m_pointOriginF.setY(y);
    m_dAngleOrigin = angle;
    update();
}

void CoordinateLabel::setCoordinateOrigin( const int x, const int y, const double angle )
{
    m_pointOrigin.setX(x);
    m_pointOrigin.setY(y);
    m_dAngleOrigin = angle;
    update();
}

void CoordinateLabel::setCoordinateDest( const int x, const int y )
{
    m_pointDest.setX(x);
    m_pointDest.setY(y);
    update();
}


void CoordinateLabel::setScaled( double dX, double dY )
{
    m_dScaledX = dX;
    m_dScaledY = dY;
    update();
}

QPointF CoordinateLabel::updateCoordinateData(QPointF point)
{
    QPointF pointDest;
    QMatrix matrix;
    matrix.scale(m_dScaledX, m_dScaledY);
    QPointF labelPos = matrix.map(point);
    int xoffset = (contentsRect().width()-pixmap()->rect().width())/2;
    int yoffset = (contentsRect().height()-pixmap()->rect().height())/2;
    pointDest = labelPos+QPointF(xoffset, yoffset);
    return pointDest;
}

void CoordinateLabel::mouseMoveEvent( QMouseEvent *e )
{
    m_point.setX(e->pos().x());
    m_point.setY(e->pos().y());
    QLabel::mouseMoveEvent( e );
    update();
}


ShowPictureEx::ShowPictureEx( QWidget *parent /*= 0*/ )
{
    connect(m_imageLabel, SIGNAL(signalMousePosF(const QPointF &)), this, SLOT(slotMousePosF(const QPointF &)));
    connect(m_imageLabel, SIGNAL(signalMousePos(const QPoint &)), this, SLOT(slotMousePos(const QPoint &)));
}

ShowPictureEx::~ShowPictureEx()
{

}

void ShowPictureEx::slotMousePosF(const QPointF &e)
{
    if (NULL == m_imageLabel->pixmap())
    {
        return;
    }
    int xoffset = (m_imageLabel->contentsRect().width()-m_imageLabel->pixmap()->rect().width())/2;
    int yoffset = (m_imageLabel->contentsRect().height()-m_imageLabel->pixmap()->rect().height())/2;
    QPointF mousePos = e-QPointF(xoffset, yoffset);

    QMatrix matrix;
    matrix.scale(m_scaleX, m_scaleY);
    QPointF pos = matrix.inverted().map(mousePos);
    emit signalMousePosF(pos);
}

void ShowPictureEx::slotMousePos(const QPoint &e)
{
    if (NULL == m_imageLabel->pixmap())
    {
        return;
    }
    int xoffset = (m_imageLabel->contentsRect().width()-m_imageLabel->pixmap()->rect().width())/2;
    int yoffset = (m_imageLabel->contentsRect().height()-m_imageLabel->pixmap()->rect().height())/2;
    QPoint mousePos = e-QPoint(xoffset, yoffset);

    QMatrix matrix;
    matrix.scale(m_scaleX, m_scaleY);
    QPoint pos = matrix.inverted().map(mousePos);

    emit signalMousePos(pos);
}

void ShowPictureEx::showPicPos( const QPoint &pos )
{
    QMatrix matrix;
    matrix.scale(m_scaleX, m_scaleY);
    QPoint labelPos = matrix.map(pos);

    qDebug() << "showPic : " << m_imageLabel->contentsRect();
    qDebug() << "showPic : " << m_imageLabel->pixmap()->rect();
    int xoffset = (m_imageLabel->contentsRect().width()-m_imageLabel->pixmap()->rect().width())/2;
    int yoffset = (m_imageLabel->contentsRect().height()-m_imageLabel->pixmap()->rect().height())/2;
    qDebug() << "showPic : " << xoffset;
    qDebug() << "showPic : " << yoffset;
    QPoint showPos = labelPos+QPoint(xoffset, yoffset);

    m_imageLabel->showPicPos(showPos);
}

void ShowPictureEx::showPicPosF( const QPointF &pos )
{
    QMatrix matrix;
    matrix.scale(m_scaleX, m_scaleY);
    QPointF labelPos = matrix.map(pos);

    int xoffset = (m_imageLabel->contentsRect().width()-m_imageLabel->pixmap()->rect().width())/2;
    int yoffset = (m_imageLabel->contentsRect().height()-m_imageLabel->pixmap()->rect().height())/2;
    QPointF showPos = labelPos+QPointF(xoffset, yoffset);

    m_imageLabel->showPicPosF(showPos);
}

void ShowPictureEx::slotCoordinatePos(const int index, const QPointF point, const double dAngle )
{
    if (0 == index)
    {
        m_imageLabel->setCoordinateOrigin(point.x(), point.y(), dAngle);
    }
    else
    {
        m_imageLabel->setCoordinateDest(point.x(), point.y());
    }
}
