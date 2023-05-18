#ifndef CUSTOMITEMDELEGATE_H
#define CUSTOMITEMDELEGATE_H

#include <QItemDelegate>
#include <QTreeWidget>
#include <QWidget>

class CCustomItemDelegate : public QItemDelegate
{
public:
    CCustomItemDelegate(QObject *parent = 0) : QItemDelegate(parent), m_pParentWidget(NULL) { }

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void SetParentWidget(QTreeWidget* pParentWidget){m_pParentWidget = pParentWidget;}
private:
    QTreeWidget    *m_pParentWidget;
public:
    enum Date_Type_Atrribute
    {
        VS_ATT_LINEEDIT = 0,        //QLineEdit
        VS_ATT_COMBOBOX,            //QComboBox
        VS_ATT_SPINBOX,             //QSpinBox
        VS_ATT_DOUBLE_SPINBOX,      //QDoubleSpinBox
        VS_ATT_CHECK_BOX,           //QCheckBox
    };
};

#endif // CUSTOMITEMDELEGATE_H
