#include "mainframe.h"
#include "ui_mainframe.h"
#include "uicommon/commonutils.h"
#include "homeui/homeview.h"
#include <QMessageBox>
#include "inotifyservice.h"

MainFrame* MainFrame::m_pInstance = NULL;
MainFrame::MainFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainFrame),
	m_pHomeView(NULL)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
	m_pHomeView = HomeView::GetInstance();
	connect(ui->widget_toolbar, SIGNAL(sigToolButtonClicked(int)), this, SLOT(slotToolBarClicked(int)));
	connect(ui->widget_toolbar, SIGNAL(sigChangeModel(int)), this, SLOT(slotModelChange(int)));
	CommonUtils::getIUIPlugin("netdevui");
}

MainFrame::~MainFrame()
{
	HomeView::DestroyInstance();
	m_pHomeView = NULL;
    delete ui;
}

MainFrame* MainFrame::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new MainFrame();
	}
	return m_pInstance;
}

void MainFrame::DestroyInstance()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void MainFrame::slotToolBarClicked(int iToolBtnIndex)
{
	QWidget* pWidgetShow = NULL;
	int iLeft = 0, iRight = 0, iTop = 0, iButtom = 0;
	switch (iToolBtnIndex)
	{
	case ToolBarBarUi::IDT_TOOLBTN_HOME:
		{
			pWidgetShow = m_pHomeView;
		}
        break;
    case ToolBarBarUi::IDT_TOOLBTN_ANALYSIS:
    {
        pWidgetShow = CommonUtils::GetPluginMainWidget("setupuiplugin");
        iLeft = 17, iRight = 17, iTop = 30, iButtom = 17;
    }
    break;
	case ToolBarBarUi::IDT_TOOLBTN_DEBUD:
		{
			pWidgetShow = CommonUtils::GetPluginMainWidget("customsettingplugin");
			iLeft = 17, iRight = 17, iTop = 30, iButtom = 17;
		}
		break;
	case ToolBarBarUi::IDT_TOOLBTN_LOGIN:
		{
		pWidgetShow = CommonUtils::GetPluginMainWidget("usermanagementui");
		iLeft = 17, iRight = 17, iTop = 30, iButtom = 17;
		}
		break;
	case ToolBarBarUi::IDT_TOOLBTN_SETTING:
		{
		pWidgetShow = CommonUtils::GetPluginMainWidget("recipesuiplugin");
		iLeft = 17, iRight = 17, iTop = 30, iButtom = 17;
		}
		break;
	default:
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("未定义的工具按钮！"));
		return;
	}
	if (NULL == pWidgetShow)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("获取窗口失败！"));
		return;
	}
	/*if (-1 == ui->stackedWidget->indexOf(pWidgetShow))
	{
		ui->stackedWidget->addWidget(pWidgetShow);
	}
	ui->stackedWidget->setCurrentWidget(pWidgetShow);*/
	if (m_mapPages.end() == m_mapPages.find(pWidgetShow))
	{
		QWidget* pContainWidget = new QWidget;
		QGridLayout* pQGridLayout = new QGridLayout;
		pQGridLayout->setContentsMargins(iLeft, iTop, iRight, iButtom);
		pQGridLayout->addWidget(pWidgetShow); 
		pContainWidget->setLayout(pQGridLayout);
		ui->stackedWidget->addWidget(pContainWidget);
		m_mapPages[pWidgetShow] = pContainWidget;
	}
	ui->stackedWidget->setCurrentWidget(m_mapPages[pWidgetShow]);
}

void MainFrame::slotModelChange(int iModel)
{
	if (NULL == m_pHomeView)
	{
		return;
	}
	if (ToolBarBarUi::IMODEL_DAY == iModel)
	{
		m_pHomeView->SlotDayMode();
	}
	else if(ToolBarBarUi::IMODEL_NIGHT == iModel)
	{
		//m_pHomeView->SlotNightMode();
		m_pHomeView->SlotDayMode();
	}

	INotifyService *pINotifyService = CommonUtils::getNotifyService();
	if (pINotifyService != NULL)
	{
		CommonUtils::getIUIPlugin("fileuiplugin");
        pINotifyService->post("fileuiplugin", "styleMode_change", (OS_LPARAM)iModel, 0);
	}
}

bool MainFrame::init()
{
	if (NULL != m_pHomeView)
	{
		m_pHomeView->init();
		/*
		if (!m_bInit)
		{
		IUIPlugin* pD = CommonUtils::getIUIPlugin("usermanagementui");//
		if (NULL != pD)
		{
		pD->init();
		}
		m_bInit = true;
		}
		*/
		//GetSDKInstance("smcamera", ISMCameraServer::ISMCAMERASERVER_IID);
	}
	slotToolBarClicked(ToolBarBarUi::IDT_TOOLBTN_HOME);
	return true;
}

bool MainFrame::deinit()
{
	if (NULL != m_pHomeView)
	{
		m_pHomeView->deinit();
	}
	return true;
}

bool MainFrame::showWindow(QWidget* parent, QRect& rect, bool bPopFlag /*= false*/)
{
	if (parent != NULL)
	{
		this->setParent(parent);
		parent->setWindowFlags(Qt::FramelessWindowHint);

	}
	if (bPopFlag)
	{
		QWidget::setWindowFlags(Qt::Dialog);
		if (parent != NULL)
		{
			QPoint point(parent->geometry().center() - QPoint(this->width() / 2,
				this->height() / 2));
			move(point);
		}
	}
	else
	{
		resize(rect.width(), rect.height());
		move(0, 0);
		this->resize(rect.width(), rect.height());
	}
	if (NULL == m_pHomeView)
	{
		return false;
	}
	m_pHomeView->showWindow(parent, rect, bPopFlag);
	show();
	return true;
}

bool MainFrame::hideWindow()
{
	hide();
	if (NULL == m_pHomeView)
	{
		return false;
	}
	return m_pHomeView->hideWindow();
	return true;
}

void MainFrame::fitSize(const QRect& rect)
{
	resize(rect.width(), rect.height());
	if (NULL == m_pHomeView)
	{
		return;
	}
	m_pHomeView->fitSize(rect);
}

bool MainFrame::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
	if ("logininfo" == event && 0 != wparam)
    {
		QString strLoginInfo = *(QString*)wparam;
		ui->widget_toolbar->SetLoginInfo(strLoginInfo);
		return true;
	}
	if (NULL == m_pHomeView)
	{
		return false;
	}
	return m_pHomeView->notify(event, wparam, lparam);
}
void MainFrame::SetLanguage(QString strlanguage)
{
	if (NULL == m_pHomeView)
	{
		return;
	}
	m_pHomeView->SetLanguage(strlanguage);
}
