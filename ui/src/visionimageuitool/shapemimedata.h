#ifndef SHAPEMIMEDATA_H
#define SHAPEMIMEDATA_H

#include <QMimeData>

// ��������
class QGraphicsItem;
class ShapeMimeData : public QMimeData
{
    Q_OBJECT
public:
    ShapeMimeData( QList<QGraphicsItem * > items);
    //ShapeMimeData�е����ݶ���ͼ��Ԫ�صĸ�����ShapeMimeData������������Ҫ�ͷš�
    ~ShapeMimeData();

    QList<QGraphicsItem *> items() const ;

private:  
    QList<QGraphicsItem * > m_items;
};

#endif // SHAPEMIMEDATA_H
