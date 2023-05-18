#include "defectcustomdelegate.h"

#include <QDoubleSpinBox>
#include <QComboBox>

DefectCustomDelegate::DefectCustomDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void DefectCustomDelegate::setCurrentWidget(CUSTOM_WIDGET_TYPE type, QString strText)
{
    m_editorText = strText;
    m_type = type;
}

QWidget *DefectCustomDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QWidget *editor;
    switch (m_type)
    {
    case DefectCustomDelegate::TYPE_DOUBLE_SPIN:
    {
        QDoubleSpinBox *spinBox = new QDoubleSpinBox(parent);
        spinBox->setDecimals(3);
        spinBox->setMinimum(0);
        spinBox->setMaximum(999999);
        editor = spinBox;
    }
    break;
    case DefectCustomDelegate::TYPE_SPIN:
    {
        QSpinBox *spinBox = new QSpinBox(parent);
        spinBox->setMinimum(0);
        spinBox->setMaximum(999999);
        editor = spinBox;
    }
    break;
    case DefectCustomDelegate::TYPE_COMBO_BOX:
    {
        QStringList list = m_editorText.split(",");
        QComboBox *pComBox = new QComboBox(parent);
        pComBox->addItems(list);
        editor = pComBox;
    }
    break;
    default:
        break;
    }
    return editor;
}

void DefectCustomDelegate::setEditorData(QWidget *editor,
    const QModelIndex &index) const
{
    switch (m_type)
    {
    case DefectCustomDelegate::TYPE_DOUBLE_SPIN:
    {
        double value = index.model()->data(index, Qt::EditRole).toDouble();
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->setValue(value);
    }
    break;
    case DefectCustomDelegate::TYPE_SPIN:
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }
    break;
    case DefectCustomDelegate::TYPE_COMBO_BOX:
    {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentText(value);
    }
    break;
    default:
        break;
    }
}

void DefectCustomDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
    const QModelIndex &index) const
{
    QVariant value;
    switch (m_type)
    {
    case DefectCustomDelegate::TYPE_DOUBLE_SPIN:
    {
        QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
        spinBox->interpretText();
        double dValue = spinBox->value();
        value = dValue;
    }
    break;
    case DefectCustomDelegate::TYPE_SPIN:
    {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int dValue = spinBox->value();
        value = dValue;
    }
    break;
    case DefectCustomDelegate::TYPE_COMBO_BOX:
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        value = comboBox->currentText();
    }
    break;
    default:
        break;
    }
    model->setData(index, value, Qt::EditRole);
}

void DefectCustomDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}