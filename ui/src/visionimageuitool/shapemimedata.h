#ifndef SHAPEMIMEDATA_H
#define SHAPEMIMEDATA_H

#include <QMimeData>

// 剪贴板类
class QGraphicsItem;
class ShapeMimeData : public QMimeData
{
    Q_OBJECT
public:
    ShapeMimeData( QList<QGraphicsItem * > items);
    //ShapeMimeData中的内容都是图形元素的副本，ShapeMimeData析构函数中需要释放。
    ~ShapeMimeData();

    QList<QGraphicsItem *> items() const ;

private:  
    QList<QGraphicsItem * > m_items;
};

#endif // SHAPEMIMEDATA_H
