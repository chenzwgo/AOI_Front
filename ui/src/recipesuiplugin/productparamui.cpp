#include "productparamui.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QPushButton>
#include "g12common.h"
#include "stringcommon.h"

#include "pages/acquisitionpage.h"
#include "pages/qualificationpage.h"
#include "pages/staticshadingcorrectionpage.h"
#include "pages/measuresizedlg.h"
#include "pages/otherpage.h" 

#include "pages/robotsettingui.h"

#define UserRole_type                       Qt::UserRole + 100
#define UserRole_name                       Qt::UserRole + 101

ProductParamUI::ProductParamUI(QWidget *parent) : QWidget(parent)
    ,m_pParamTree(NULL)
    ,m_pPageStackedWidget(NULL)
    ,m_pCurrentPage(NULL)
    ,m_pBtnBoxWidget(NULL)
    ,m_pMdyBtn(NULL)
    ,m_pSaveBtn(NULL)
    ,m_pCancelBtn(NULL)
{
    m_pParamTree = new QTreeWidget(this);
    m_pPageStackedWidget = new QStackedWidget(this);
    QWidget* m_pMainWidget = new QWidget(this);
    QSplitter* pSplitter = new QSplitter(Qt::Horizontal);
    m_pMdyBtn = new QPushButton(QString::fromLocal8Bit("修改"));
    m_pSaveBtn = new QPushButton(QString::fromLocal8Bit("保存"));
    m_pCancelBtn = new QPushButton(QString::fromLocal8Bit("取消"));
    pSplitter->setHandleWidth(1);//设置分割线宽度为1
    pSplitter->setChildrenCollapsible(false);

    QHBoxLayout *h=new QHBoxLayout();
    QVBoxLayout *h2=new QVBoxLayout();
    QHBoxLayout *v1 = new QHBoxLayout();
    v1->addStretch();
    v1->addWidget(m_pMdyBtn);
    v1->addWidget(m_pSaveBtn);
    v1->addWidget(m_pCancelBtn);
    v1->addStretch();
    h->setMargin(0);
    h->setSpacing(0);
    h->addWidget(pSplitter);
    setLayout(h);

    m_pMdyBtn->setFixedSize(40,40);
    m_pSaveBtn->setFixedSize(40,40);
    m_pCancelBtn->setFixedSize(40,40);

    m_pMainWidget->setLayout(h2);

    QWidget* m_pBtnBoxWidget = new QWidget();
    m_pBtnBoxWidget->setLayout(v1);
    h2->setMargin(0);
    h2->setSpacing(0);
    h2->addWidget(m_pPageStackedWidget);
    h2->addWidget(m_pBtnBoxWidget);
    

    pSplitter->addWidget(m_pParamTree);
    pSplitter->addWidget(m_pMainWidget);
    pSplitter->setStretchFactor(0,0);
    pSplitter->setStretchFactor(1,1);
    m_pParamTree->setHeaderLabels(QStringList()<<QString::fromLocal8Bit("参数列表"));
    m_pParamTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents );

    initUi();

    connect(m_pParamTree,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_itemChanged(QTreeWidgetItem*,int)));
    connect(m_pParamTree,SIGNAL(currentItemChanged(QTreeWidgetItem* , QTreeWidgetItem*)),this,SLOT(on_currentItemChanged(QTreeWidgetItem* , QTreeWidgetItem*)));
    connect(m_pMdyBtn,SIGNAL(clicked()),this,SLOT(on_mdfBtnClicked()));
    connect(m_pSaveBtn,SIGNAL(clicked()),this,SLOT(on_savBtnClicked()));
    connect(m_pCancelBtn,SIGNAL(clicked()),this,SLOT(on_cancelBtnClicked()));

    m_pMdyBtn->setVisible(false);
    //m_pMdyBtn->setEnabled(true);
    //m_pSaveBtn->setEnabled(false);
    //m_pCancelBtn->setEnabled(false);

}

void ProductParamUI::initUi()
{
    readParamFile();
    initParamTree();
}

void ProductParamUI::updateProductItem(QString strProductName)
{
	m_pParamTree->blockSignals(true);
	int iTopItemCnt = m_pParamTree->topLevelItemCount();
	for (int index = 0; index < iTopItemCnt; index++)
	{
		QTreeWidgetItem * pTopItem = m_pParamTree->topLevelItem(index);
		int iChildCnt = pTopItem->childCount();
		for (int j = 0; j < iChildCnt;j++)
		{
			QTreeWidgetItem *pChildItem = pTopItem->child(j);
			QString strText = pChildItem->text(0);
			if(strText == strProductName)
			{ 		
				QFont ft = pChildItem->font(0); 
				ft.setBold(true);
				pChildItem->setFont(0, ft);
			}
			else
			{
				QFont ft = pChildItem->font(0);
				ft.setBold(false);
				pChildItem->setFont(0, ft);
			}
		}		
	}
	m_pParamTree->blockSignals(false);
}

void ProductParamUI::initPageWidget()
{
    for( int index =0 ;index < m_productStep.size();index++)
    {
        m_pPageStackedWidget->addWidget(new QWidget());
    }
    m_pPageStackedWidget->setCurrentIndex(0);
}

void ProductParamUI::readParamFile()
{
    m_stepDesMap.clear();
    m_productStep.append("Acquisition");
    m_productStep.append("Static Shading Correction");
    m_productStep.append("Qualification");
	m_productStep.append("MeasureSize");
	m_productStep.append("OtherPage");
    m_productStep.append("RobotSettingUi");

    m_stepDesMap.insert("Acquisition",QString::fromLocal8Bit("采集设置"));
	m_stepDesMap.insert("Static Shading Correction", QString::fromLocal8Bit("静态校准"));
    m_stepDesMap.insert("Qualification",QString::fromLocal8Bit("缺陷验证"));
	m_stepDesMap.insert("MeasureSize", QString::fromLocal8Bit("尺寸测量"));
	m_stepDesMap.insert("OtherPage", QString::fromLocal8Bit("其它通讯"));
    m_stepDesMap.insert("RobotSettingUi", QString::fromLocal8Bit("下料机机械手配置"));

    AcquisitionPage* pAcquisitionPage = new AcquisitionPage();
    QualificationPage* pQualificationPage = new QualificationPage();
	StaticShadingCorrectionpage *pStaticShadingCorrectionpage = new StaticShadingCorrectionpage();
    MeasureSizeDlg* pMeasureSizeDlg = new MeasureSizeDlg();
	OtherPage* pOtherPage = new OtherPage();
    RobotSettingUi* pRobotSettingUi = new RobotSettingUi();

    m_stepPages.insert("Acquisition", pAcquisitionPage);
    m_stepPages.insert("Static Shading Correction", pStaticShadingCorrectionpage);
    m_stepPages.insert("Qualification", pQualificationPage);
    m_stepPages.insert("MeasureSize", pMeasureSizeDlg);
	m_stepPages.insert("OtherPage", pOtherPage);
    m_stepPages.insert("RobotSettingUi", pRobotSettingUi);
	
    m_productPages.insert("Acquisition", pAcquisitionPage);
	m_productPages.insert("Static Shading Correction", pStaticShadingCorrectionpage);
    m_productPages.insert("Qualification", pQualificationPage);
    m_productPages.insert("MeasureSize", pMeasureSizeDlg);
	m_productPages.insert("OtherPage", pOtherPage);
    m_productPages.insert("RobotSettingUi", pRobotSettingUi);
}


void ProductParamUI::initParamTree()
{
	m_pParamTree->blockSignals(true);
    for( int index =0 ;index < m_productStep.size();index++)
    {
        QTreeWidgetItem *StpItem = new QTreeWidgetItem();
        StpItem->setFirstColumnSpanned(true);
        QString strStepDes  = m_productStep.at(index);
        if (m_stepDesMap.contains(m_productStep.at(index)))
        {
            strStepDes = m_stepDesMap.value(m_productStep.at(index));
        }
        StpItem->setText(0, strStepDes);
        StpItem->setData(0,UserRole_type,"stepRoot");        
        StpItem->setData(0,UserRole_name,m_productStep.at(index));
        m_pParamTree->insertTopLevelItem( index, StpItem );

        QVector<QString> proslist = m_ParamsMap.value(m_productStep.at(index));
        string programs = getProgramNameList();
        vector<string> vect = SString::split(programs,",");
        for (int index = 0;index <vect.size();index++)
        {
            proslist.append(QString::fromStdString(vect.at(index)));
        }
        for( int iproductIndex =0 ; iproductIndex < proslist.size();iproductIndex++)
        {
            QTreeWidgetItem * proItem = new QTreeWidgetItem(StpItem);
            proItem->setText(0, proslist.at(iproductIndex));
            proItem->setData(0,UserRole_type,"product");
            proItem->setData(0,UserRole_name,proslist.at(iproductIndex));
            StpItem->addChild(proItem);
        }
    }
	m_pParamTree->blockSignals(false);
}

void ProductParamUI::on_itemChanged(QTreeWidgetItem *pItem, int i)
{
    if(pItem == NULL)
    {return;}
    QString type = pItem->data(0,UserRole_type).toString();
    QString name = pItem->data(0,UserRole_name).toString();
    if(type == "stepRoot")
    {
        showStepPage(name);
    }
    else if(type == "product")
    {
        if (pItem->parent() == NULL)
        {
            return;
        }
        QString Steptype = pItem->parent()->data(0,UserRole_name).toString();
        showProductPage(Steptype,name);
    }
}

void ProductParamUI::on_currentItemChanged( QTreeWidgetItem * current, QTreeWidgetItem * previous )
{
    if(current == NULL)
    {return;}
    QString type = current->data(0,UserRole_type).toString();
    QString name = current->data(0,UserRole_name).toString();
    if(type == "stepRoot")
    {
        showStepPage(name);
    }
    else if(type == "product")
    {
        if (current->parent() == NULL)
        {
            return;
        }
        QString Steptype = current->parent()->data(0,UserRole_name).toString();
        showProductPage(Steptype,name);
    }
}

void ProductParamUI::on_mdfBtnClicked()
{
    if (m_pCurrentPage == NULL)
    {
        return;
    }
    //m_pCurrentPage->onEnable(true);

    //m_pMdyBtn->setEnabled(false);
   // m_pSaveBtn->setEnabled(true);
    //m_pCancelBtn->setEnabled(true);

}

void ProductParamUI::on_savBtnClicked()
{
    if (m_pCurrentPage == NULL)
    {
        return;
    }
   // m_pCurrentPage->onEnable(false);
    m_pCurrentPage->onSave();
    //m_pMdyBtn->setEnabled(true);
    //m_pSaveBtn->setEnabled(false);
    //m_pCancelBtn->setEnabled(false);
}

void ProductParamUI::on_cancelBtnClicked()
{
    if (m_pCurrentPage == NULL)
    {
        return;
    }
    //m_pCurrentPage->onEnable(false);
    m_pCurrentPage->onCancel();
    //m_pMdyBtn->setEnabled(true);
    //m_pSaveBtn->setEnabled(false);
    //m_pCancelBtn->setEnabled(false);
}

void ProductParamUI::showStepPage(QString strStep)
{
    AbstractPageWidget* pPage = m_stepPages.value(strStep);
    if (pPage == NULL)
    {
        return ;
    }
    int index = m_pPageStackedWidget->indexOf(pPage);
    if(index <0 )
    {
        m_pPageStackedWidget->addWidget(pPage);
    }
    pPage->onUpdate();
    m_pPageStackedWidget->setCurrentWidget(pPage);
    m_pCurrentPage = pPage;

}

void ProductParamUI::showProductPage(QString strStep,QString strProctName)
{
    AbstractPageWidget* pPage = m_stepPages.value(strStep);

    int index = m_pPageStackedWidget->indexOf(pPage);
    if(index <0 )
    {
        m_pPageStackedWidget->addWidget(pPage);
    }
    pPage->updateProductName(strProctName);
    pPage->onUpdate();
    
    m_pPageStackedWidget->setCurrentWidget(pPage);
    m_pCurrentPage = pPage;
}
