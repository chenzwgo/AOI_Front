#ifndef RECIPESPARAMUI_H
#define RECIPESPARAMUI_H

#include <QWidget>
#include <QTreeWidget>
#include <QStringList>
#include <QStackedWidget>


class  AbstractPageWidget;
class RecipesParamUI : public QWidget
{
    Q_OBJECT
public:
    explicit RecipesParamUI(QWidget *parent = nullptr);
    void initUi();
signals:

private slots:
    void on_itemChanged(QTreeWidgetItem* pItem,int i);
private:
    void initParamTree();
    void initPageWidget();
    void readParamFile();
    void showProductPage(QString strProctName);

private:
    QTreeWidget*  m_pParamTree;
    AbstractPageWidget*    m_pProductWidget;
    QVector<QString> m_products;
};

#endif // RECIPESPARAMUI_H
