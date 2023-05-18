#include "customitemdelegate.h"
#include "linecameramanagerui.h"
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

QWidget * CCustomItemDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    Q_UNUSED(option);
    CParamWidgetCreate item;
    QWidget* pItemWidget = item.CreateEditorWidget(parent, index.data(Qt::UserRole).toInt());

    return pItemWidget;
}

void CCustomItemDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    Date_Type_Atrribute iAtrribute = (Date_Type_Atrribute)index.data(Qt::UserRole+1).toInt();
    switch(iAtrribute)
    {
    case VS_ATT_LINEEDIT:
        {
            QString value = index.model()->data(index, Qt::EditRole).toString();
            QLineEdit *edit = static_cast<QLineEdit*>(editor);
            edit->setText(value);
        }
        break;
    case VS_ATT_COMBOBOX:
        {
            QString value = index.model()->data(index, Qt::EditRole).toString();
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            int i = comboBox->findText(value);
            comboBox->setCurrentIndex(i);
        }
        break;
    case VS_ATT_SPINBOX:
        {
            int iValue = index.model()->data(index, Qt::EditRole).toInt();
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            spinBox->setValue(iValue);
        }
    }
}

void CCustomItemDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    Date_Type_Atrribute iAtrribute = (Date_Type_Atrribute)index.data(Qt::UserRole+1).toInt();
    switch(iAtrribute)
    {
    case VS_ATT_LINEEDIT:
        {
            QLineEdit *edit = static_cast<QLineEdit*>(editor);
            QString strValue = edit->text();
            if (!strValue.isEmpty())
            {
                model->setData(index, strValue);
            }
        }
        break;
    case VS_ATT_COMBOBOX:
        {
            QComboBox *comboBox = static_cast<QComboBox*>(editor);
            QString strText = comboBox->currentText();
            if (!strText.isEmpty())
            {
                model->setData(index, strText);
            }
        }
        break;
    case VS_ATT_SPINBOX:
        {
            QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
            int iValue = spinBox->value();
            model->setData(index, iValue);
        }
    }
}

void CCustomItemDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
