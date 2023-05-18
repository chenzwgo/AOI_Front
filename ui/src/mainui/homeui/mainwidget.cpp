#include "mainwidget.h"

MainWidget::MainWidget(QWidget* parent):QWidget(parent)
{
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainSplitter = new QSplitter(Qt::Horizontal,this);
	m_pRightSpltter = new QSplitter(Qt::Vertical,this);

    m_pTopWidget = new QWidget(this);
    m_pleftWidget = new QWidget(this);
	m_pRightWidget = new  QGroupBox(this);
    m_pRightTopWidget = new QTabWidget(this);
    m_pRightBottomWidget = new QTabWidget(this);
    m_pleftLayout = new QVBoxLayout(this);
	m_pRightLayout = new QVBoxLayout(this);
    m_pTopLayout = new QHBoxLayout(this);
    m_pTopLeftLayout =new QHBoxLayout(this);
    m_pTopRightLayout = new QHBoxLayout(this);
    createMainArea();
    createTopArea();
    createLeftArea();
	creteRightArea();
    this->setContentsMargins(0,0,0,0);
}

MainWidget::~MainWidget()
{

}

void MainWidget::addWidgetToTopLeftArea(QWidget *pWidget)
{
    if(pWidget == NULL)
    {
        return;
    }
    m_pTopLeftLayout->addWidget(pWidget);      
}

void MainWidget::addWidgetToTopRightArea(QWidget *pWidget)
{
    if(pWidget == NULL)
    {
        return;
    }
    m_pTopRightLayout->addWidget(pWidget);
}

void MainWidget::addWidgetToLeftArea(QWidget *pWidget)
{
    if(pWidget == NULL)
    {
        return;
    }
    m_pleftLayout->addWidget(pWidget);
}

void MainWidget::addWidgetToRightTopArea(QWidget *pWidget, QString widgetname)
{
    if(pWidget == NULL)
    {
        return;
    }
    int index = m_pRightTopWidget->indexOf(pWidget);
    if(index != -1)
    {
       return;
    }
    m_pRightTopWidget->addTab(pWidget,widgetname);
}

void MainWidget::addWidgetToRightBottomArea(QWidget *pWidget, QString widgetname)
{
    if(pWidget == NULL)
    {
        return;
    }
    int index = m_pRightBottomWidget->indexOf(pWidget);
    if(index != -1)
    {
       return;
    }
    m_pRightBottomWidget->addTab(pWidget,widgetname);
}

void MainWidget::createMainArea()
{
    this->setLayout(m_pMainLayout);
	m_pMainLayout->addLayout(m_pTopLayout);
    m_pMainLayout->addWidget(m_pMainSplitter);
    m_pMainLayout->setMargin(0);

    m_pMainSplitter->setContentsMargins(0,0,0,0);
    m_pMainSplitter->addWidget(m_pleftWidget);
	m_pMainSplitter->addWidget(m_pRightWidget);

	//设置左右大小比例 1:5
	m_pleftWidget->resize(m_pMainSplitter->size()/5);
	m_pRightWidget->resize(m_pMainSplitter->size());
	QList<int>  sizes;
	sizes.append(m_pleftWidget->size().rwidth());
	sizes.append(m_pRightWidget->size().rwidth());
	m_pMainSplitter->setSizes(sizes);

}

void MainWidget::createTopArea()
{
    if(m_pTopWidget == NULL)
    {
        return;
    }
    m_pTopWidget->setLayout(m_pTopLayout);
    m_pTopLayout->addLayout(m_pTopLeftLayout);
    m_pTopLayout->addStretch();
    m_pTopLayout->addLayout(m_pTopRightLayout);
	//m_pTopLayout->setSizeConstraint(QLayout::SetFixedSize);
    m_pTopLayout->setContentsMargins(0,0,0,0);
}

void MainWidget::createLeftArea()
{
    if(m_pleftWidget == NULL)
    {
        return;
    }
	m_pleftLayout->setContentsMargins(0,0,0,0);
    m_pleftWidget->setLayout(m_pleftLayout);
}

void MainWidget::createRighttopArea()
{
    if(m_pRightTopWidget == NULL)
    {
        return;
    }
}

void MainWidget::createRightBottomArea()
{
    if(m_pRightBottomWidget == NULL)
    {
        return;
    }
}

void MainWidget::creteRightArea()
{
	createRighttopArea();
	createRightBottomArea();
	if (m_pRightWidget == NULL)
	{
		return;
	}

	m_pRightSpltter->addWidget(m_pRightTopWidget);
	m_pRightSpltter->addWidget(m_pRightBottomWidget);
	m_pRightLayout->addWidget(m_pRightSpltter);
	m_pRightWidget->setLayout(m_pRightLayout);
    m_pRightLayout->setMargin(0);
}

