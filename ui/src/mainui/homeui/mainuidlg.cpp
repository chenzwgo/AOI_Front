#include "mainuidlg.h"
#include "displaylogwidget.h"
#include "ui_mainuidlg.h"
#include "mainflowaction/ilogReport.h"
#include "../workflow/workflow.h"
#include "callsdk.h"
#include "uicommon/change.h"
#include "../signalcheck.h"
#include "itoolbar.h"
#include "uicommon/commonutils.h"
#include "iuiplugin.h"
#include "any.h"
#include "homeui/homeview.h"

CMainuiDlg* CMainuiDlg::m_psInstance = NULL;


CMainuiDlg::CMainuiDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainuiDlg)
{
    m_HomeView = new HomeView(this);
    //m_pExtendStackWidget = new ExtendStackWidget(this);
    //ExtendStackWidget* m_pExtendStackWidget2 = new ExtendStackWidget(m_pExtendStackWidget);
    //m_pExtendStackWidget->addWidget(" Main ",m_HomeView);
   // m_pExtendStackWidget2->addWidget("Local Defects",new QWidget());
    //m_pExtendStackWidget2->addWidget("Measurements",new QWidget());
    //m_pExtendStackWidget->addWidget("Sheet Overview",m_pExtendStackWidget2);
    //m_pExtendStackWidget->addWidget("Statistics",new QWidget());
    connect(this,SIGNAL(sigupdateSysStateBtn(int)),this,SLOT(slotupdateSysStateBtn(int)));
    
    
}

CMainuiDlg::~CMainuiDlg()
{
    deinit();
    delete ui;
}

CMainuiDlg* CMainuiDlg::GetInstance()
{
	if (NULL == m_psInstance)
	{
		m_psInstance = new CMainuiDlg();
	}
	return m_psInstance;
}
void CMainuiDlg::DestroyInstance()
{	
	if (NULL != m_psInstance)
	{
		delete m_psInstance;
		m_psInstance = NULL;
	}
}


void CMainuiDlg::uilog( string msg, int level )
{
    m_psInstance->ui->plainTextEditlog->Addlogmsg(QString::fromUtf8(msg.c_str()), (UILogLevel)level);
}

void CMainuiDlg::updateRunLog( const QString &runLog, int type /*= 0*/ )
{
    ui->plainTextEditlog->Addlogmsg(runLog, (UILogLevel)type);
}



bool CMainuiDlg::init()
{
    ILogReport *pLogReport = (ILogReport*)GetSDKInstance("mainflowaction", ILogReport::IDD_LOG_REPORT);
    if (pLogReport)
    {
        pLogReport->addReproter(&CMainuiDlg::uilog);
    }
    WorkFlow::getInstance();

    QWidget *pWorkWidget = NULL;
    IUIPlugin *pUiPligin = CommonUtils::getIUIPlugin("visionactionui");
    if (NULL != pUiPligin)
    {
        pUiPligin->queryInterface(IUIPlugin::GET_MAIN_WIDGET_OBJECT + 2,(void **)&pWorkWidget);
        if (NULL != pWorkWidget)
        {
            ui->showPictureLayout->addWidget(pWorkWidget);
        }
    }

    return true;
}

bool CMainuiDlg::deinit()
{
    ILogReport *pLogReport = (ILogReport*)GetSDKInstance("mainflowaction", ILogReport::IDD_LOG_REPORT);
    if (pLogReport)
    {
        pLogReport->removeReproter(&CMainuiDlg::uilog);
    }
    WorkFlow::releaseInstance();
    return true;
}

bool CMainuiDlg::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (parent != NULL)
    {
        this->setParent(parent);

    }
    if (bPopFlag)
    {
        QWidget::setWindowFlags(Qt::Dialog);
        if(parent != NULL)
        {
            QPoint point (parent->geometry().center() - QPoint (this->width() / 2,
                                                                this->height() / 2));
            move(point);
        }
    }
    else
    {
        resize(rect.width(), rect.height());
        move(rect.x(), rect.y());
        m_HomeView->resize(rect.width(), rect.height());
    }
    show();

    return true;
}

bool CMainuiDlg::hideWindow()
{
    hide();
    return true;
}


void CMainuiDlg::fitSize(const QRect& rect)
{
    resize(rect.width(), rect.height());
}

bool CMainuiDlg::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    if(event == "start")
    {
        if (SYS_PAUSE == WorkFlow::getInstance()->GetWorkState())
        {
            SignalCheck::SetSignal(SYS_CONTINUE);
        }
        else
        {
            SignalCheck::SetSignal(SYS_START);
        }
    }
    else if(event == "pause")
    {
        SignalCheck::SetSignal(SYS_PAUSE);
    }
    else if(event == "stop")
    {
        SignalCheck::SetSignal(SYS_STOP);
    }
    else if(event == "reset")
    {
        SignalCheck::SetSignal(SYS_RESET);
    }
    else if (event == "open_data_dir")
    {
         // exploreDir ("E:\\WorkData");
    }
    else if (event == "open_picture_dir")
    {
         // exploreDir ("D:\\Apple_AAA\\Pic");
    }
    else
    {

    }

    return true;
}

void CMainuiDlg::SetLanguage(QString strlanguage)
{
    QString strLangFilePath = Change::GetUITranslatorPath("CMainuiDlg");
    qDebug("%s:%d(%s) LangFile is \"%s\" ",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));


    if (m_translator.load(strLangFilePath))
    {
        qApp->installTranslator(&m_translator);
        //retranslateUi(this);
    }
    else
    {
        qWarning("%s:%d(%s) translator load LangFile\"%s\" failed, or is not exist",
                 __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));
    }
}

void CMainuiDlg::updateSysStateBtn( int esState )
{
    emit sigupdateSysStateBtn((int) esState);
}

void CMainuiDlg::slotupdateSysStateBtn( int esState )
{
    E_WORK_STATE state = (E_WORK_STATE)esState;

    switch (state)
    {
    case MAIN_WORK_INIT:       // 初始状态
        setToolBtnPress("");
        break;
    case MAIN_WORK_WAIT_RESET: // 等待复位
    case MAIN_WORK_RESET_FAILED:
        setToolBtnPress("");
        break;
    case MAIN_WORK_RESETING:   // 复位中
        setToolBtnPress("toolBar_reset");
        break;
    case MAIN_WORK_ESTOP:      // 急停
        setToolBtnPress("");
        break;
    case MAIN_WORK_SAFEDOOR:   // 安全门被打开
        setToolBtnPress("toolBar_pause");
        break;
    case MAIN_WORK_POWEROFF:   // 掉电	
        setToolBtnPress("");
        break;
    case MAIN_WORK_READY:      // 准备好
        setToolBtnPress("");
        break;
    case MAIN_WORK_WORK:       // 工作中
        setToolBtnPress("toolBar_on");
        break;
    case MAIN_WORK_PAUSE:      // 暂停
        setToolBtnPress("toolBar_pause");
        break;
    case MAIN_WORK_STOP:       // 停止(不断电)
        setToolBtnPress("toolBar_stop");
        break;
    case MAIN_WORK_ROBOT_UNCONNECT:  // Robot断开连接
    case MAIN_WORK_ERROR:            // 出现异常
    default:
        break;
    }
}

void CMainuiDlg::setToolBtnPress( QString strBtnName )
{
    IToolBar* pToolbar = NULL;
    IUIPlugin* pUIPlugin = CommonUtils::getIUIPlugin("toolbarplugin");
    if (pUIPlugin == NULL)
    {
        return ;
    }

    QObject *pObject = NULL;
    pUIPlugin->queryInterface(IToolBar::ITOOLBAR_ID,(void**)&pObject);
    if (pObject == NULL)
    {
        return ;
    }

    pToolbar = qobject_cast<IToolBar* >(pObject);
    if (pToolbar == NULL)
    {
        return ;
    }

    QStringList btnList ;
    btnList << "toolBar_on" << "toolBar_pause" << "toolBar_stop" << "toolBar_reset";
    foreach(QString toolButton,btnList)
    {
        if (toolButton == strBtnName)
        {
            pToolbar->SetTooltemChecked(strBtnName);
        }
        else
        {
            pToolbar->SetTooltemUnChecked(toolButton);
        }
    }
    return ;
}

