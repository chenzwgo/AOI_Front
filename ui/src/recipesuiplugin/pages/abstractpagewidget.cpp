#include "abstractpagewidget.h"
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QTextEdit>

AbstractPageWidget::AbstractPageWidget(QWidget *parent):QWidget(parent)
{

}

void AbstractPageWidget::onEnable(bool enable)
{
    return;
}

void AbstractPageWidget::onSave()
{
    return;
}

void AbstractPageWidget::onCancel()
{
    return;
}

void AbstractPageWidget::onUpdate()
{
    return;
}


bool AbstractPageWidget::setUIItemToolTip( const QString& itemObjName,QString Tip )
{
    QObject* pWidget = FindChildObject(this,itemObjName);
    if (pWidget == NULL)
    {
        return false;
    }
    QWidget* pWidget2 = qobject_cast<QWidget*>(pWidget);
    if (pWidget2 == NULL)
    {
        return false;
    }
    pWidget2->setToolTip(Tip);
    return true;
}

bool AbstractPageWidget::setUIItemValue( const QString& itemObjName,QVariant value )
{
    QObject* pWidget = FindChildObject(this,itemObjName);
    if (pWidget == NULL)
    {
        return false;
    }

    return setUIObjectValue(pWidget,value);
}

bool AbstractPageWidget::enableUIItem(const QString &itemObjName, bool bEnable)
{
    QObject* itemObj = FindChildObject(this,itemObjName);
    if (itemObj == NULL)
    {
        return false;
    }
    QWidget* pWidget = qobject_cast<QWidget*>(itemObj);
    if (pWidget == NULL)
    {
        return false;
    }
    pWidget->setEnabled(bEnable);
    return true;
}

QVariant AbstractPageWidget::getUItemValue( const QString& itemObjName,QVariant defaultValue )
{
    QObject* pWidget = FindChildObject(this,itemObjName);
    if (pWidget == NULL)
    {
        return defaultValue;
    }
    return getUIObjectValue(pWidget,defaultValue);

}

bool AbstractPageWidget::setUIObjectValue(  QObject* itemObj,QVariant value )
{
    QWidget* pWidget = qobject_cast<QWidget*>(itemObj);
    if (pWidget == NULL)
    {
        return false;
    }
    QString strClassName = pWidget->metaObject()->className();
    if (strClassName == "QLineEdit"){
        QLineEdit *pLineEdit = qobject_cast<QLineEdit*>(pWidget);
        pLineEdit->setText(value.toString());
    } 
    else if (strClassName == "QComboBox"){
        QComboBox *pComboBox = qobject_cast<QComboBox*>(pWidget);
        int index = pComboBox->findText(value.toString());
        if(-1 != index)
        {
            //this->setToolTip("");
            pComboBox->setCurrentIndex(index);
        }
		else
		{
			index = value.toInt();
			if (index >= 0 && index < pComboBox->count())
			{
				pComboBox->setCurrentIndex(index);
			}
		}    
    }
    else if (strClassName == "QCheckBox"){
        QCheckBox *pCheckBox = qobject_cast<QCheckBox*>(pWidget);
        pCheckBox->setChecked(value.toInt() !=0);
    }
    else if (strClassName == "QSpinBox"){
        QSpinBox *pSpinBox = qobject_cast<QSpinBox*>(pWidget);
        pSpinBox->setValue(value.toInt());
    }
    else if (strClassName == "QDoubleSpinBox"){
        QDoubleSpinBox *pSpinBox = qobject_cast<QDoubleSpinBox*>(pWidget);
        pSpinBox->setValue(value.toDouble());
    }
    else if (strClassName == "QRadioButton"){      
        QRadioButton *pRadioButton = qobject_cast<QRadioButton*>(pWidget);
        pRadioButton->setChecked(value.toInt() !=0);
    }
    else if (strClassName == "QLabel"){      
        QLabel *pLable = qobject_cast<QLabel*>(pWidget);
        pLable->setText(value.toString());
    }
    else if (strClassName == "QRadioButton"){
        QRadioButton *pRadioButton = qobject_cast<QRadioButton*>(pWidget);
        pRadioButton->setChecked(value.toInt() !=0);
    }
    else if (strClassName == "QGroupBox") {
        QGroupBox *pGroupButton = qobject_cast<QGroupBox*>(pWidget);
        pGroupButton->setChecked(value.toInt() != 0);
    }
	else if (strClassName == "QTextEdit") {
		QTextEdit *pTextEdit = qobject_cast<QTextEdit*>(pWidget);
		pTextEdit->setText(value.toString());
	}
    else{
        return false;
    }
    return true;
}

QVariant AbstractPageWidget::getUIObjectValue( QObject* itemObj,QVariant defaultValue )
{
    QWidget* pWidget = qobject_cast<QWidget*>(itemObj);
    if (pWidget == NULL)
    {
        return false;
    }
    QString strClassName = pWidget->metaObject()->className();
    if (strClassName == "QLineEdit"){
        QLineEdit *pLineEdit = qobject_cast<QLineEdit*>(pWidget);
        return pLineEdit->text();
    } 
    else if (strClassName == "QComboBox"){
        QComboBox *pComboBox = qobject_cast<QComboBox*>(pWidget);
        return pComboBox->currentText();
    }
    else if (strClassName == "QCheckBox"){
        QCheckBox *pCheckBox = qobject_cast<QCheckBox*>(pWidget);
        return pCheckBox->isChecked() ? 1:0;
    }
    else if (strClassName == "QSpinBox"){
        QSpinBox *pSpinBox = qobject_cast<QSpinBox*>(pWidget);
        return pSpinBox->value();
    }
    else if (strClassName == "QDoubleSpinBox"){
        QDoubleSpinBox *pSpinBox = qobject_cast<QDoubleSpinBox*>(pWidget);
        return pSpinBox->value();
    }
    else if (strClassName == "QRadioButton"){      
        QRadioButton *pRadioButton = qobject_cast<QRadioButton*>(pWidget);
        return pRadioButton->isChecked() ? 1:0;
    }
    else if (strClassName == "QLable"){      
        QLabel *pLable = qobject_cast<QLabel*>(pWidget);
        return pLable->text();
    }
    else if (strClassName == "QRadioButton"){
        QRadioButton *pRadioButton = qobject_cast<QRadioButton*>(pWidget);
        return pRadioButton->isChecked() ? 1:0;
    }
    else if (strClassName == "QGroupBox") {
        QGroupBox *pGroupButton = qobject_cast<QGroupBox*>(pWidget);
        return pGroupButton->isChecked() ? 1 : 0;
    }
    else{
        return false;
    }
}

void AbstractPageWidget::updateProductName( const QString& strProductName )
{
    m_strProductName = strProductName;
}

QWidget* AbstractPageWidget::getUIItem(const QString& itemObjName)
{
	return qobject_cast<QWidget*>(FindChildObject(this, itemObjName));
}

QObject* AbstractPageWidget::FindChildObject( QObject* pObj, QString strObjName )
{
    if (pObj == NULL || strObjName.length() == 0)
    {
        return NULL;
    }

    QObject* pChildWidget = pObj->findChild<QObject*>(strObjName);
    if (pChildWidget != NULL)
    {
        return pChildWidget;
    }
    else
    {
        QObjectList childlist = pObj->children();
        for (int i=0; i< childlist.size(); i++)
        {
            QString strName = childlist.at(i)->objectName();
            pChildWidget = FindChildObject(childlist.at(i), strObjName);
            if (pChildWidget != NULL)
            {
                return pChildWidget;
            }
            else
            {
                continue;
            }
        }
    }
    return NULL;
}
