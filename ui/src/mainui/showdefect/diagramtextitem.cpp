
#include <QtGui>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMoveEvent>
#include <QGraphicsProxyWidget>
#include "diagramtextitem.h"
#include "diagramscene.h"
#include "sendmessage.h"


#define SET_COLORS         "background-color: rgba(255, 165, 39,80);font: 6pt \"Arial\""
#define SET_PRES_COLORS    "background-color: rgba(0, 85, 255,80);font: 6pt \"Arial\""  //color: rgba(255, 255, 255,60);
myLabel::myLabel( const QString & text,int index, QWidget * parent )
    :QLabel(parent)
{
    setText(text);
    setAlignment(Qt::AlignCenter);
    //setStyleSheet(SET_COLORS);
    m_index = index;
    connect(CSendMessage::GetInstance(),SIGNAL(signalPressTabView(int)),this,SLOT(slotSetCorlor(int)));
}
void myLabel::mousePressEvent(QMouseEvent *event)
{
    emit signalsToDefect();
    
}

//void myLabel::mouseReleaseEvent( QMouseEvent * event )
//{
//}
void myLabel::slotSetLabel(int iPres)
{
    if (m_index == iPres)
    {
        //setStyleSheet(SET_PRES_COLORS);
        return;
    }
    //setStyleSheet(SET_COLORS);
}

void myLabel::slotSetCorlor( int index )
{
    slotSetLabel(index);
}
void myLabel::enterEvent(QEvent *)  
{  
    //setStyleSheet(SET_PRES_COLORS);
}  

void myLabel::leaveEvent(QEvent *)  
{  
    //setStyleSheet(SET_COLORS);
} 


//! [0]

DiagramTextItem::DiagramTextItem(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsTextItem(parent)
{
    //setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    m_pLabel = NULL;
    m_iPress = 0; 
}
//! [0]

//! [1]
//QVariant DiagramTextItem::itemChange(GraphicsItemChange change,
//                     const QVariant &value)
//{
//    if (change == QGraphicsItem::ItemSelectedHasChanged)
//        emit selectedChange(this);
//    return value;
//}
////! [1]
//
////! [2]
//void DiagramTextItem::focusOutEvent(QFocusEvent *event)
//{
//    setTextInteractionFlags(Qt::NoTextInteraction);
//    emit lostFocus(this);
//    QGraphicsTextItem::focusOutEvent(event);
//}
//! [2]

//! [5]
void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void DiagramTextItem::init(QString strMark,int iMark,int iw,int ih)
{
    m_pLabel = new myLabel(strMark,iMark);
    m_iPress = iMark;
    m_pLabel->setFixedSize(iw,ih);
    QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this); // the proxy's parent is pMyItem
    pMyProxy->setWidget(m_pLabel); //
    connect(m_pLabel, SIGNAL(signalsToDefect()), this, SLOT(slotsPush()));
}
//void DiagramTextItem::init(QString strMark,int iw,int ih)
//{
//    QPushButton* pLabel = new QPushButton(strMark);
//    pLabel->setFixedSize(iw,ih);
//    //pLabel->setAlignment(Qt::AlignCenter);
//    pLabel->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(255, 170, 255);font: 6pt \"Arial\"");
//    QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this); // the proxy's parent is pMyItem
//    pMyProxy->setWidget(pLabel); //
//    connect(pLabel, SIGNAL(Clicked()), this, SLOT(slotsPush()));
//}
void DiagramTextItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    QGraphicsTextItem::mousePressEvent(event);
}

//QRectF DiagramTextItem::boundingRect()
//{
//    qreal penWidth = 1;
//
//    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
//        20 + penWidth, 20 + penWidth);
//}
void DiagramTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    QGraphicsTextItem::paint(painter, option, widget);
}

void DiagramTextItem::slotsPush()
{
    CSendMessage::GetInstance()->sendPressUI(m_iPress);
}

/****************************************///

 #include <QtGui>

 Chip::Chip(stDefectFeature inStru)
 {
     //QColor color(0,0,0);
     //this->color = color;
     //setZValue((x + y) % 2);
     m_iPress = inStru.iIndex;
     //m_Mark = QString::fromStdString(inStru.strOriginate);
     m_iwd = inStru.dWide;
     m_ihg = inStru.dHigh;
     //setColorType(0);
     //setFlags(ItemIsSelectable /*| ItemIsMovable*/);
     setFlags(flags() | ItemIsSelectable/*| ItemIsMovable*/);
     //setAcceptsHoverEvents(true);
     m_bshow = false;
     connect(CSendMessage::GetInstance(),SIGNAL(signalPressTabView(int)),this,SLOT(slotsSelet(int)));
 }

 QRectF Chip::boundingRect() const
 {
      return QRectF(0, 0, m_iwd, m_ihg);
 }

 QPainterPath Chip::shape() const
 {
     QPainterPath path;
      path.addRect(0, 0, m_iwd, m_ihg);
     return path;
 }

 void Chip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
 {
    
     Q_UNUSED(widget);
 
  /*
      QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
      if (option->state & QStyle::State_MouseOver)
          fillColor = fillColor.light(125);

      painter->fillRect(QRectF(0, 0, m_iwd, m_ihg), fillColor);
     // painter->drawText(m_iwd*3/16, m_ihg*2/3, m_Mark);
      return;
     */
     
     painter->setPen(QPen(Qt::red, 2));
     painter->drawLine(0, 0, m_iwd, 0);
     painter->drawLine(m_iwd, 0, m_iwd, m_ihg);
     painter->drawLine(m_iwd, m_ihg, 0, m_ihg);
     painter->drawLine(0, m_ihg, 0, 0);
     return;
 }

 void Chip::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     setFlags(flags() ^ ItemIsSelectable /*| ItemIsMovable*/);
     if (event->button() == Qt::LeftButton)
     {
         CSendMessage::GetInstance()->sendPressUI(m_iPress);
     }
     QGraphicsItem::mousePressEvent(event);
    // update(); // ÓÅ»¯Ò³Ãæ¿¨¶Ù£¬ÆÁ±Îupdate();
 }

 void Chip::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
 {
     if (event->modifiers() & Qt::ShiftModifier) {
         stuff << event->pos();
         //update(); // ÓÅ»¯Ò³Ãæ¿¨¶Ù£¬ÆÁ±Îupdate();
         return;
     }
     QGraphicsItem::mouseMoveEvent(event);
 }

 void Chip::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     setFlags(flags() | ItemIsSelectable /*| ItemIsMovable*/);
     QGraphicsItem::mouseReleaseEvent(event);
    // update(); // ÓÅ»¯Ò³Ãæ¿¨¶Ù£¬ÆÁ±Îupdate();
 }

 void Chip::setColorType( int iType )
 {
   //int RGBUnit[] = {0x55,0x66};
   //int size = sizeof(RGBUnit)/sizeof(int);
   //if (iType >= size)
   //{
   //    return;
   //}
   int iR = 255 - iType*5;
   int iG = iType*5 - 255;
   int iB = iType*5 - 2*255;
   if (iR > 0)
   {
      iG =0;
      iB =255-iR;
   }  
   else if ( iG >= 0  && 255 >= iG)
   {
      iR = 0;
      iB = 255-iG;
   }
   else
   {
       iB = iB>255?255:iB;
       iR = 255-iB;
       iG = 255-iB;
       
   }
   
   QColor colorset(iR,iG,iB);
   this->color = colorset;
 }

 void Chip::slotsSelet( int indxe )
 {
     this->setSelected(indxe==m_iPress);
 }

//! [5]
