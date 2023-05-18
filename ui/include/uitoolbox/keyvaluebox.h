#ifndef SPECIFICBOX_H
#define SPECIFICBOX_H

#include <QtGui>
#include "dllexport.h"
#include <QTableWidget>
#include <QLabel>
#include "uidefine.h"
#include <vector>
#include <QSizePolicy>
#include <QGroupBox>
#include <QGridLayout>
using namespace std;
    

#ifndef UI_SAFE_DELETE
#define UI_SAFE_DELETE(_X_) if (_X_) {delete (_X_); (_X_) = NULL; }
#endif


class RT_API BoxItem:public QWidget
{
public:
    QLabel*	  keyLabel;
    QLabel*	  valueLabel;
    BoxItem(QString key,QString value,QWidget *parent = 0):QWidget(parent)
    {
        keyLabel	= new QLabel(key);
        keyLabel->setAlignment(Qt::AlignLeft);
        valueLabel  = new QLabel(value);
        valueLabel->setAlignment(Qt::AlignRight);
    }
    ~BoxItem()
    {
        UI_SAFE_DELETE(keyLabel);
        UI_SAFE_DELETE(valueLabel);
    }
};
/************************************************************************/
/*
    1.用来显示（key,value）这种格式数据的GroupBox
    2.void addItem(key,value)	   按key名字加入每条数据。
    3.void delItem(key,value)	   按key名字删除数据。
    4.void setItemText(key,value)  按key名字修改数据。
    5.void setNoTitleAndBorder()   去掉GroupBox的边框和titile。
*/
/************************************************************************/
class RT_API KeyValueBox : public QGroupBox
{
    Q_OBJECT
public:
    enum S_Layout
    {
        Horizontal = 0,		 //水平排列
        Vertical			//垂直排列
    };
    explicit KeyValueBox(QWidget *parent = 0);
    /************************************************************************/
    /* title : GroupBox名字
       layout: Items排列方式
               layout = KeyValueBox::Horizontal, 每一条Item水平排列。 
                ________   ________    ________   ________ 
               |________| |________|  |________| |________|

               layout = KeyValueBox::Vertical,   每一条Item垂直排列。
                ________ 
               |________|
                ________ 
               |________|
                ________ 
               |________|
                ________ 
               |________|
    /************************************************************************/
    explicit KeyValueBox(const QString  title, S_Layout layout,QWidget *parent = 0);
    void addItem(const QString  key,const QString value);
    void delItem(const QString  key);
    bool setItemText(const QString key,const QString value);
    //去掉边框和title
    void setNoTitleAndBorder(){
        this->setStyleSheet("QGroupBox{border:none;}");
        this->setTitle("");
    }
    ~KeyValueBox();
signals:
    void setTextSignal(QLabel* label,QString text);
    void addItemSignal(QString key,QString  value);
    void delItemSignal(QString key);
private:
    bool        m_bHorizontal;
    QGridLayout* m_GridLayout;
    void setLayout(QLayout* layout);
    vector<BoxItem*> m_vecBoxItem;
private slots:
    void setTextSlot(QLabel* label,QString text);
    void addItemSlot(QString key,QString value);
    void delItemSlot(QString key);
};

#endif // SPECIFICBOX_H
