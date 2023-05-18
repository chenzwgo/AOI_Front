#include "recipesparamui.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include "pages/abstractpagewidget.h"
#include "g12common.h"
#include "stringcommon.h"


#define UserRole_type                       Qt::UserRole + 100
#define UserRole_name                       Qt::UserRole + 101

RecipesParamUI::RecipesParamUI(QWidget *parent) : QWidget(parent)
{
    m_pParamTree = new QTreeWidget(this);
    m_pProductWidget = new AbstractPageWidget(this);
    QSplitter* pSplitter = new QSplitter(Qt::Horizontal);
    pSplitter->setHandleWidth(1);//设置分割线宽度为1
    pSplitter->setChildrenCollapsible(false);

    QHBoxLayout *h=new QHBoxLayout();
    h->setMargin(0);
    h->setSpacing(0);
    h->addWidget(pSplitter);
    setLayout(h);

    pSplitter->addWidget(m_pParamTree);

    pSplitter->addWidget(m_pProductWidget);
    pSplitter->setStretchFactor(0,0);
    pSplitter->setStretchFactor(1,1);
    m_pParamTree->setHeaderLabels(QStringList()<<"Section");
    m_pParamTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents );

    initUi();

    connect(m_pParamTree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_itemChanged(QTreeWidgetItem*,int)));
}

void RecipesParamUI::initUi()
{
    readParamFile();
    initParamTree();
}


void RecipesParamUI::initPageWidget()
{

}

void RecipesParamUI::readParamFile()
{
    string programs = getProgramNameList();
    vector<string> vect = SString::split(programs,",");
    for (int index = 0;index <vect.size();index++)
    {
        m_products.append(QString::fromStdString(vect.at(index)));
    }
}


void RecipesParamUI::initParamTree()
{
    for( int index =0 ;index < m_products.size();index++)
    {
        QTreeWidgetItem *StpItem = new QTreeWidgetItem();
        StpItem->setFirstColumnSpanned(true);
        StpItem->setText(0, m_products.at(index));
        StpItem->setData(0,UserRole_type,m_products.at(index));
        m_pParamTree->insertTopLevelItem( index, StpItem );
    }
}

void RecipesParamUI::on_itemChanged(QTreeWidgetItem *pItem, int i)
{
    if(pItem == NULL)
    {return;}
    QString name = pItem->data(0,UserRole_type).toString();
    if(name.length() >0)
    {
        showProductPage(name);
    }

}

void RecipesParamUI::showProductPage(QString strProctName)
{
    m_pProductWidget->onUpdate();
}
