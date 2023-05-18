#ifndef PRODUCTPARAMUI_H
#define PRODUCTPARAMUI_H

#include <QWidget>
#include <QTreeWidget>
#include <QStringList>
#include <QStackedWidget>
#include <QPushButton>


class  AbstractPageWidget;
class ProductParamUI : public QWidget
{
    Q_OBJECT
public:
    explicit ProductParamUI(QWidget *parent = nullptr);
    void initUi();
	void updateProductItem(QString strProductName);
signals:

private slots:
    void on_itemChanged(QTreeWidgetItem* pItem,int i);
    void on_currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
    void on_mdfBtnClicked();
    void on_savBtnClicked();
    void on_cancelBtnClicked();
private:
    void initParamTree();
    void initPageWidget();
    void readParamFile();
    void showStepPage(QString strStep);
    void showProductPage(QString strStep,QString strProctName);

private:
    QTreeWidget*  m_pParamTree;
    QStackedWidget*    m_pPageStackedWidget;
    QVector<QString> m_productStep;
    QMap<QString,QVector<QString> > m_ParamsMap;
    QMap<QString,AbstractPageWidget*> m_stepPages;
    QMap<QString,AbstractPageWidget*> m_productPages;
    QMap<QString,QString> m_stepDesMap;
    AbstractPageWidget* m_pCurrentPage;
    QWidget* m_pBtnBoxWidget;
    QPushButton* m_pMdyBtn ;
    QPushButton* m_pSaveBtn ;
    QPushButton* m_pCancelBtn;
};

#endif // PRODUCTPARAMUI_H
