#ifndef ABSTRACTPAGEWIDGET_H
#define ABSTRACTPAGEWIDGET_H
#include <QWidget>
#include <QString>
#include <QVariant>

class  AbstractPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractPageWidget(QWidget *parent = 0);
    virtual ~AbstractPageWidget(){}
    virtual void  onEnable(bool enable);
    virtual void  onSave();
    virtual void  onCancel();
    virtual void  onUpdate();
    bool setUIItemToolTip(const QString& itemObjName,QString Tip);
    bool setUIItemValue(const QString& itemObjName,QVariant value);
    bool enableUIItem(const QString& itemObjName,bool bEnable);
    QVariant getUItemValue(const QString& itemObjName,QVariant defaultValue);
    bool setUIObjectValue( QObject* itemObj,QVariant value);
    QVariant getUIObjectValue( QObject* itemObj,QVariant defaultValue);
    virtual void updateProductName(const QString& strProductName);
	QWidget* getUIItem(const QString& itemObjName);
protected:
        QString m_strProductName;
private:
    QObject* FindChildObject( QObject* pObj = NULL, QString strObjName="" );

};


#endif
