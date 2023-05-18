
#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H
#include "maindefine.h"
#include <QGraphicsTextItem>
#include <QPen>
#include<QObject>
QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPaintDevice;
QT_END_NAMESPACE

//! [0]
#include <QLabel>
#include <QWidget>
#include <QString>
class myLabel : public QLabel
{
    Q_OBJECT
public:
    myLabel(const QString & text, int index,QWidget * parent = 0);
    ~myLabel(){}
signals:
    void signalsToDefect();
public slots:
    void slotSetCorlor(int index);
protected:
    void leaveEvent(QEvent *); //鼠标进到区域显示颜色
    void enterEvent(QEvent *); //鼠标离开改颜色
public:
    void mousePressEvent ( QMouseEvent * event ) ;
    //void mouseReleaseEvent( QMouseEvent * event) ;
    void slotSetLabel(int iPres);
private:
    int m_index;
};
class myLabel;
class DiagramTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };

    DiagramTextItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    void init(QString strMark,int iMark,int iw,int ih);

signals:
    //void lostFocus(DiagramTextItem *item);
    void selected(int isele);
    void sigSetLabel(int index);
public slots:    
    void slotsPush();
    
protected:
    //QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    //void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
     
    //QRectF boundingRect();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    myLabel* m_pLabel;
    int m_iPress;
 
};
#include <QColor>
#include <QGraphicsItem>

//class QFontMetrics;
class Chip :/* public QGraphicsItem,*/public QGraphicsObject
{
    Q_OBJECT
public:
    Chip(stDefectFeature inStru);
	~Chip(){};
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
public slots:
      void slotsSelet(int indxe);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void setColorType(int iType);
    QColor color;
    QList<QPointF> stuff;
    int m_iPress;
    QString m_Mark;
    bool m_bshow;
    int  m_iwd;
    int  m_ihg;
};
#endif
