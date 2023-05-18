#ifndef DEFECTCUSTOMDELEGATE_H
#define DEFECTCUSTOMDELEGATE_H

#include <QItemDelegate>
class DefectCustomDelegate : public QItemDelegate
{
    //Q_OBJECT

public:
    enum CUSTOM_WIDGET_TYPE
    {
        TYPE_DOUBLE_SPIN = 0,
        TYPE_SPIN,
        TYPE_COMBO_BOX
    };
    DefectCustomDelegate(QObject *parent = 0);

    void setCurrentWidget(CUSTOM_WIDGET_TYPE type, QString strText = "");
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QString             m_editorText;
    CUSTOM_WIDGET_TYPE  m_type;
};

#endif // DEFECTCUSTOMDELEGATE_H