#ifndef SHOWPICTUREONLABEL_H
#define SHOWPICTUREONLABEL_H

#include "dllexport.h"
#include "uidefine.h"

#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>

class CoordinateLabel : public QLabel
{
    Q_OBJECT
public:
    CoordinateLabel(QWidget *parent = 0);
    ~CoordinateLabel();

    void showPicPos(const QPoint &pos);  // 图像上显示点位
    void showPicPosF(const QPointF &pos);  // 图像上显示点位

    void setCoordinateOrigin(const int x, const int y, const double angle);
    void setCoordinateOrigin(const double x, const double y, const double angle);
    void setCoordinateDest(const int x, const int y);
    void setScaled(double dX, double dY);

private:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    //void paintEvent( QPaintEvent * e);

    QPointF updateCoordinateData(QPointF point);
signals:
    void signalMousePosF(const QPointF &pos);  // 发送鼠标位置(高精度)
    void signalMousePos(const QPoint &pos);    // 发送鼠标位置
private:
    QPoint m_point;
    QPointF m_pointF;

    QPoint m_pointOrigin;
    QPointF m_pointOriginF;
    QPointF m_pointDest;
    double m_dAngleOrigin; //保证角度在0~90度之间
    double m_dScaledX;
    double m_dScaledY;
    QPoint m_pointTest;
};

//封装一个专用于显示图片的类
class RT_API ShowPicture : public QWidget
{
    Q_OBJECT

public:
    ShowPicture(QWidget *parent = 0);
    ~ShowPicture();

    void showPictureInLabel(QString file_path);//只显示一张图片
	void showPictureInLabel(unsigned char *pixelsR, unsigned char *pixelsG, unsigned char *pixelsB,long width, long height);//显示以二进制的图片

    bool savePicture(QString file_path = QString());//提供保存图片接口
	void clearPicture();
    void hiddenButton(bool isHidden);//隐藏所有的按钮
    void adjustPictureSize();
	long GetWidgetWinID();

protected:
	bool eventFilter(QObject *target, QEvent *event);
private slots:
    void slotReducePicture();		// 缩小图片
    void slotEnlargePicture();		// 放大图片
    void slotTurnLeftPicture();		// 左旋转图片
    void slotTurnRightPicture();	// 右旋转图片
    void slotAdjustPicture();		// 还原自适应大小
	void slotSavePicture();			// 保存图片

	void slotHScrollBarRangeChange(int min, int max);
	void slotVScrollBarRangeChange(int min, int max);

protected:                        // 子类使用，从私有属性改为保护属性
    CoordinateLabel *m_imageLabel;

    qreal m_scaleX;      //X拉伸的值
    qreal m_scaleY;      //Y拉伸的值
    int   m_rotate;      //旋转的角度

private:
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
	void scaleImage(long x, long y, double factor);
	void dragImage(QPoint mousePos);

	QPushButton *m_reduceButton;
	QPushButton *m_enlargedButton;
	QPushButton *m_turnLeftButton;
	QPushButton *m_turnRightButton;
	QPushButton *m_adjustButton;
	QPushButton *m_saveButton;

    QScrollArea *m_scrollArea;
    QImage* m_imgScaled;
	int m_CurWidth;
	int m_CurHeight;
    QMatrix m_matrix;//旋转

	bool m_bMouseButtonPress;
	QPoint m_MousePos;

	const static int maxWidth = 6144;	// 图片放大最大宽度
	const static int maxHeight = 6144;	// 
	const static int minWidth = 64;		// 图片放大最小宽度
	const static int minHeight = 64;	// 

	double m_ScaleX1;           // 滚轮滚动时鼠标相对 m_imageLabel 的坐标
	double m_ScaleY1;           // 
	double m_ScaleX2;           // 滚轮滚动时鼠标相对 m_scrollArea->viewport() 的坐标
	double m_ScaleY2;           // 
};

class RT_API ShowPictureEx: public ShowPicture
{
    Q_OBJECT

public:
    ShowPictureEx(QWidget *parent = 0);
    ~ShowPictureEx();

    void showPicPos(const QPoint &pos);  // 图像上显示点位
    void showPicPosF(const QPointF &pos);  // 图像上显示点位

signals:
    void signalMousePosF(const QPointF &pos); // 发送鼠标所在图像坐标(高精度)
    void signalMousePos(const QPoint &pos);   // 发送鼠标所在图像坐标

private slots:
    void slotMousePosF(const QPointF &pos);		// 鼠标位置
    void slotMousePos(const QPoint &pos);		// 鼠标位置
    void slotCoordinatePos(const int index, const QPointF point, const double dAngle);
};


//封装一个专用于显示图片几行几列的容器
class RT_API ShowPictureWidget : public QWidget
{
    Q_OBJECT

public:
    //ShowPictureWidget(int row,int column,QWidget *parent = 0);
    ShowPictureWidget(QWidget *parent = 0);
    ~ShowPictureWidget();
    void setShowPicture(int row,int column);
    ShowPicture* getSpecifyWidget(int row,int column);
    void hiddenSpecifyWidget(int row,int column,bool isHidden);
	void resizeWindowsSize();

private:
    QGridLayout *mainLayout;
    QList<ShowPicture*> m_allWidget;
    int m_row;
    int m_column;
};

#endif // SHOWPICTUREONLABEL_H
