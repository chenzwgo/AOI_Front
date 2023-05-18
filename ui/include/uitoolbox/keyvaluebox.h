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
    1.������ʾ��key,value�����ָ�ʽ���ݵ�GroupBox
    2.void addItem(key,value)	   ��key���ּ���ÿ�����ݡ�
    3.void delItem(key,value)	   ��key����ɾ�����ݡ�
    4.void setItemText(key,value)  ��key�����޸����ݡ�
    5.void setNoTitleAndBorder()   ȥ��GroupBox�ı߿��titile��
*/
/************************************************************************/
class RT_API KeyValueBox : public QGroupBox
{
    Q_OBJECT
public:
    enum S_Layout
    {
        Horizontal = 0,		 //ˮƽ����
        Vertical			//��ֱ����
    };
    explicit KeyValueBox(QWidget *parent = 0);
    /************************************************************************/
    /* title : GroupBox����
       layout: Items���з�ʽ
               layout = KeyValueBox::Horizontal, ÿһ��Itemˮƽ���С� 
                ________   ________    ________   ________ 
               |________| |________|  |________| |________|

               layout = KeyValueBox::Vertical,   ÿһ��Item��ֱ���С�
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
    //ȥ���߿��title
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
