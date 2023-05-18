#include "toolbarbarui.h"
#include "ui_toolbarbarui.h"
#include "g12common.h"
#include "filevariable/ifilevariable.h"
#include <QDateTime>
#include <QMessageBox>
#include "inotify.h"
#include "inotifyservice.h"
#include "uicommon/commonutils.h"
#include "sendmessage.h"
ToolBarBarUi::ToolBarBarUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBarBarUi),
	m_bFirstShow(true)
{
    ui->setupUi(this);


	IFileVariable *pFileVar = getGlobalFileVariable();
	m_strDayBenginTime = pFileVar->getValue("dayAndNight", "dayBenginTime", "07:00:00").toCString().c_str();
	m_strNightBenginTime = pFileVar->getValue("dayAndNight", "nightBenginTime", "19:00:00").toCString().c_str();
	QString strText1 = QString::fromLocal8Bit(pFileVar->getValue("toolBar", "text1", "").toCString().c_str());
	QString strText2 = QString::fromLocal8Bit(pFileVar->getValue("toolBar", "text2", "").toCString().c_str());
	ui->label_text1->setText(strText1);
	ui->label_text2->setText(strText2);

    m_bPressed = false;
    m_pTimer = new QTimer;
    connect(m_pTimer, SIGNAL(timeout()), SLOT(SlotTimer()));
	//
	ui->toolBar_Products->hide();//配置界面
	ui->toolBar_interface->hide();//调试界面
	ui->toolBar_SetUp->hide();
}

ToolBarBarUi::~ToolBarBarUi()
{
    m_pTimer->stop();
    delete m_pTimer;
    m_pTimer = NULL;
    delete ui;
}


void ToolBarBarUi::SetLoginInfo(const QString& strLoginInfo)
{
	QStringList infoList = strLoginInfo.split(':');
	QString strShowInfo = QString::fromLocal8Bit("错误的登录信息");
	int iLoginModel = 0;
	do 
	{
		if (infoList.size() < 2)
		{
			ui->toolBar_Products->hide();//配置界面
			ui->toolBar_interface->hide();//调试界面
			break;
		}
		iLoginModel = infoList[0].toInt();
		switch (iLoginModel)
		{
		case 0:
		{
			strShowInfo = QString::fromLocal8Bit("未登录");
			ui->toolBar_Products->hide();//配置界面
			ui->toolBar_interface->hide();//调试界面
		}
		break;
		case 1:
		{
			strShowInfo = QString::fromLocal8Bit("生产模式");
			ui->toolBar_Products->hide();//配置界面
			ui->toolBar_interface->hide();//调试界面
		}
		break;
		case 2:
		{
			strShowInfo = QString::fromLocal8Bit("调试模式");
			ui->toolBar_Products->show();//配置界面
			ui->toolBar_interface->show();//调试界面
		}
		break;
		case 3:
		{
			strShowInfo = QString::fromLocal8Bit("工程模式");
			ui->toolBar_Products->show();//配置界面
			ui->toolBar_interface->show();//调试界面
		}
		break;
		case 4:
		{
			strShowInfo = QString::fromLocal8Bit("超级用户");
			ui->toolBar_Products->show();//配置界面
			ui->toolBar_interface->show();//调试界面
		}
		break;
		default:
			break;
		}
	} while (0);
	SENDMSG_INSTANCE->SendLoginModelChange(iLoginModel);
	ui->label_loginInfo->setText(strShowInfo);
}

void ToolBarBarUi::mousePressEvent(QMouseEvent *e)
{
    // 鼠标左键按下事件
    if (e->button() == Qt::LeftButton)
    {
        // 记录鼠标左键状态
        m_bPressed = true;
        //记录鼠标在屏幕中的位置
        m_ptPress = e->globalPos();
    }
}

void ToolBarBarUi::mouseMoveEvent(QMouseEvent *e)
{
    if(m_bPressed)
    {
        //将父窗体移动到父窗体原来的位置加上鼠标移动的位置：event->globalPos()-m_start
        this->window()->move(this->window()->geometry().topLeft() +
            e->globalPos() - m_ptPress);
        //将鼠标在屏幕中的位置替换为新的位置
        m_ptPress = e->globalPos();
    }
}

void ToolBarBarUi::mouseReleaseEvent(QMouseEvent *e)
{
    // 鼠标左键释放
    if (e->button() == Qt::LeftButton)
    {
        // 记录鼠标状态
        m_bPressed = false;
    }
}

void ToolBarBarUi::mouseDoubleClickEvent(QMouseEvent *e)
{
    this->window()->move(0,0);
    this->window()->isMaximized() ? this->window()->showNormal() : this->window()->showMaximized();
}

void ToolBarBarUi::on_toolButton_close_clicked()
{
	if (QMessageBox::Yes != QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否退出软件？"), QMessageBox::Yes, QMessageBox::No))
	{
		return;
	}

	INotifyService *pINotifyService = CommonUtils::getNotifyService();
	if (pINotifyService != NULL)
	{
		pINotifyService->broadcast("APP_CLOSE", 0, 0);
	}

	if (CommonUtils::getMainWindow() != NULL)
	{
		emit CommonUtils::getMainWindow()->close();
	}

}

void ToolBarBarUi::on_toolButton_min_clicked()
{
    this->window()->showMinimized();
}

void ToolBarBarUi::on_buttonGroup_buttonClicked(QAbstractButton * button)
{
	IDToolButtonClicked iDSend = IDT_TOOLBTN_HOME;
	if (ui->toolBar_a_mainui == button)
	{
		iDSend = IDT_TOOLBTN_HOME;
	}
	else if(ui->toolBar_SetUp == button)
	{
		iDSend = IDT_TOOLBTN_ANALYSIS;
	}
	else if (ui->toolBar_interface == button)
	{
		iDSend = IDT_TOOLBTN_DEBUD;
	}
	else if (ui->toolBar_Products == button)
	{
		iDSend = IDT_TOOLBTN_SETTING;
	}
	else if (ui->toolButton_login == button)
	{
		iDSend = IDT_TOOLBTN_LOGIN;
	}
	emit sigToolButtonClicked(iDSend);
}

void ToolBarBarUi::on_toolButton_help_clicked()
{
	emit sigToolButtonClicked(IDT_TOOLBTN_HELP);
}

void ToolBarBarUi::showEvent( QShowEvent * e )
{
	if (m_bFirstShow)
	{
		m_bFirstShow = false;
		QDateTime curDateTime = QDateTime::currentDateTime();
		QDateTime dayBeginTime = QDateTime::currentDateTime();
		dayBeginTime.setTime(QTime::fromString(m_strDayBenginTime, "hh:mm:ss"));
		QDateTime nightBeginTime = QDateTime::currentDateTime();
		nightBeginTime.setTime(QTime::fromString(m_strNightBenginTime, "hh:mm:ss"));
		if (dayBeginTime < curDateTime && curDateTime <= nightBeginTime)
		{
			emit sigChangeModel(IMODEL_DAY);
		}
		else
		{
			emit sigChangeModel(IMODEL_NIGHT);
		}
	}
    if (!m_pTimer->isActive())
    {
        m_pTimer->start(1000);
    }
    QWidget::showEvent(e);
}

void ToolBarBarUi::hideEvent( QHideEvent * e )
{
    //m_pTimer->stop();
    QWidget::hideEvent(e);
}

void ToolBarBarUi::SlotTimer()
{
    QString strDateTime = QDateTime::currentDateTime().toString("dddd;yyyy/MM/dd;hh:mm:ss");
    QStringList dataList = strDateTime.split(';');
    ui->label_ddd->setText(dataList[0]);
    ui->label_dddd->setText(dataList[1]);
    ui->label_time->setText(dataList[2]);
	if (m_strDayBenginTime == dataList[2] )
	{
		emit sigChangeModel(IMODEL_DAY);
	}
	else if(m_strNightBenginTime == dataList[2])
	{
		emit sigChangeModel(IMODEL_NIGHT);
	}
}


