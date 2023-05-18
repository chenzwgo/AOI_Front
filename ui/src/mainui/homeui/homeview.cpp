#include "homeview.h"
#include "ui_homeview.h"
/*新ui修改*/
#include "QTextCursor"

#include <QHBoxLayout>
#include <QDebug>

#include "commonutils.h"
#include "workflow/workflow.h"
#include "workflow/workflowfun.h"
#include "rtdefine.h"
#include "sendmessage.h"
#include "QEvent"
#include "iodefine.h"
#include "msgbox.h"
#include "iuiplugin.h"
#include "customwidget.h"
#include "mainflowaction/ilogReport.h"
#include "mainflow/imainflowmgr.h"
#include "callsdk.h"
#include "displaylogwidget.h"
#include "mainflowaction/ilogReport.h"
#include "uicommon/change.h"
#include "../signalcheck.h"
#include "itoolbar.h"
#include "uicommon/commonutils.h"
#include "iuiplugin.h"
#include "any.h"
#include "sendmessage.h"
#include "defecttableview.h"
#include "../showdefect/diagramtextitem.h"
#include "../showdefect/graphwidget.h"
#include "g12globalenv.h"
#include "filevariable/ifilevariable.h"
#include "commonapi.h"
#include "expressionparser/Features.h"
#include "expressionparser/scriptLexer.h"
#include "../defectcfg/defectcfg.h"
#include "inotify.h"
#include "inotifyservice.h"
#include "workflow/InvokeArithmetic.h"
#include "showTest/uphstatistics.h"
#include "showTest/formlinechart.h"
#include "filter/filter.h"
#include "stringcommon.h"
#include "./g12commonui/idefectrecord.h"
#include "defect/idefectqualimanage.h"
#include "iniFile.h"
#include "config/dvconfig.h"
#include "defectlevelpiechart.h"
#include "uphbarchart.h"
#include "trace.h"
#include "./vscameratools/cameramanager/icamerabuffermanager.h"
#include "thresholdui.h"
#include "ProductOutUnion.h"
#include <QFileDialog>
#include <QPushButton>

#include "netdev/devserver/socketPC1.h"
#include "netdev/devserver/socketserver.h"
#include "commonapi.h"
#include "workflow/frontpc.h"
#include "workflow/backpc.h"
#include "ProductMark.h"
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"
#include "workflow/frontpc.h"
#include "robotcrl.h"
#include "outputDataSave.h"
/*新ui修改*/
#define IO_ON_IMG  ":image/general/IoOn.png"
#define IO_OFF_IMG ":image/general/IoOff.png"

#define LOG_PATH		".\\worklog"
#define COLOR_NONE    "none"
#define COLOR_BLUE     "rgb(179, 202, 255)"
#define COLOR_RED     "rgb(255, 128, 128)"
#define COLOR_YELLOW  "rgb(255, 255, 128)"
#define COLOR_GREEN   "rgb(128, 255, 128)"


string COLOR_G12_OK_NG_RE[] = { "rgb(85, 170, 255)","rgb(230, 39, 14)" ,"rgb(85, 170, 255)","rgb(255, 85, 0)" };

string vShowData[] = { "A","C","A","B" };
string vShowDataOK[] = { "OK","NG","OK","RI" };


#define LABLE_ALARM_SHEET "background-color: rgb(255, 0, 0);border-radius: 12px;"
#define LABLE_NOMORML_SHEET "background-color: rgb(176, 216, 155);border-radius: 12px;"

#define COLOR_ACTIVE  0X00afd99a
#define COLOR_WAIT    0X00fdfdbf
HomeView* HomeView::m_pInstance=NULL;

HomeView::HomeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeView),
	m_pTimer(NULL),
    m_bCountProduceTime(false),
	m_iProduceTime(0),
    m_scene(NULL),
    m_pTimerDiskCheck(-1),
    m_fileVariable(NULL),
    m_currentView(NULL),
	m_pIDefectQualification(NULL),
    m_CurrentProductSn(0),
	m_LastProductSn(0),
	m_bFristCheckSpec(true),
	m_iSequenceNumber(0),
	m_bSaveLog(true),
	m_strLogPath(""),
	m_scene1Index(0),
	m_scene2Index(1),
	m_iScenseIndex(0),
	m_iDayClass(1),
    m_iChooseDay(1),
    m_binit(false),
	m_strLine("A")
{  
    ui->setupUi(this);

	initStationUiCfg();

	ui->radioButton_secondcheck->hide();
	ui->radioButton_firstcheck->hide();
	
	ui->label_checkStandar->hide();
	ui->pushButton_defect_day->hide();
	ui->pushButton_defect_pices->hide();
	//ui->toolButton_stopBeeAlarm->hide();
	ui->toolButton_heatmap_2->hide();
	//ui->label_cam3->hide();
	//ui->label_cam4->hide();
	//ui->label_Cam2Title->hide();
	//ui->label_Cam2Title_2->hide();

	setObjectName("HomeView");
	QString strDay = QDateTime::currentDateTime().toString("yyyyMMdd");
	QString strPath = QString("log_%1.txt").arg(strDay);
	QString strLogPath = QString(LOG_PATH) + "\\" + strPath;
	m_pFile = new QFile(strLogPath);
	m_curCamIndex = 0;
	memset(m_currentChannel, 0, sizeof(m_currentChannel));
	memset(m_ResFlag, 0, sizeof(m_ResFlag));

	ui->textEdit_runLog->SetMaxDisplaycount(1000);
	m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(timeOut()));
    m_runTime = QDateTime::currentDateTime();
    connect(this,SIGNAL(sigupdateSysStateBtn(int)),this,SLOT(slotupdateSysStateBtn(int)));
    connect(this,SIGNAL(sigupdateSysStateBtn(int)),this,SLOT(updateWorkStatus(int)));
    connect(SENDMSG_INSTANCE, SIGNAL(signalMessage(const QString&, int)), this, SLOT(updateRunLog(const QString &, int)));
	connect(SENDMSG_INSTANCE, SIGNAL(signalProductInfo(CImageProduct* )), this, SLOT(slotProductInfo(CImageProduct* )),Qt::BlockingQueuedConnection);
	connect(SENDMSG_INSTANCE, SIGNAL(signalShowOkNGEnd(unsigned long long , enResFlag,string)), this, SLOT(slotUpdateShowEnd(unsigned long long , enResFlag,string)), Qt::BlockingQueuedConnection);
     //connect(ui->widget_heatmap, SIGNAL(sigdefectWarning(QString)), ui->label_warning_status, SIGNAL(sigShowLableMsg(QString)));
	connect(SENDMSG_INSTANCE, SIGNAL(signalCtToUi(double)), this, SLOT(slotCTime(double)));
	int iTotalImageLen = 12800;
	int         iWidth = 8192;
	IFileVariable* pAcquisitionFile = getAcquisitionFileVariable();
	if (pAcquisitionFile != NULL)
	{
		iTotalImageLen = pAcquisitionFile->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT).toInt();          // 总行数
		iTotalImageLen = iTotalImageLen > 0 ? iTotalImageLen : 12800;
		iWidth = pAcquisitionFile->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEWIDTH).toInt();
		iWidth = iWidth > 0 ? iWidth : 8192;
	}
	ui->widget_heatmap->initHeatMap(iWidth, iTotalImageLen,true);//初始化 大小缺陷类型
	ui->page_heatmap_2->initHeatMap(iWidth, iTotalImageLen,false);//初始化 大小缺陷类型
	connect(CSendMessage::GetInstance(), SIGNAL(signalUpdateUiStus(QString, int)), this, SLOT(slotUpdateImage(QString, int)));

    memset(m_pIamgeSrc,0,sizeof(m_pIamgeSrc));

	/*新ui修改*/
    connect(ui->widget_defectTableView, SIGNAL(sigSelectCamera(QString,int)), this, SLOT(slotCamera_show(QString,int)));
    connect(ui->widget_defectTableView, SIGNAL(sigSelectDefect(const QRectF,int)), this, SLOT(slotUpdateSelectPos(const QRectF,int)));


	//m_pIDefectQualification = (IDefectQualification*)GetSDKInstance("defectqualification", IDefectQualification::IDD_DEFECT_QULIFICATION);
	connect(SENDMSG_INSTANCE, SIGNAL(signalInitOtherUi()), this, SLOT(slotsIniOtherUI()));

	IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
	if (NULL != p)
	{
		m_pIDefectQualification = p->getDefectQualificationInstance(0);
	}

	m_Pix[0] = QPixmap(QString::fromUtf8(IO_OFF_IMG));
    m_Pix[1] = QPixmap(QString::fromUtf8(IO_ON_IMG));
	IBufferManagerFactory* pIBufferManagerFactory = (IBufferManagerFactory*)GetSDKInstance("cameramanager", IBufferManagerFactory::IID_CAMERA_BUFFER_MANAGER_FACTORY);

	m_iImageChoose[0] = 2;
	m_iImageChoose[1] = 2;
	QString strCamNames = getCurrentProgramCamNameList().c_str();
    m_CamNameList = strCamNames.split(",");

    QString strChannelNameList = getCurrentProgramRecipesList().c_str();
    if (strChannelNameList.isEmpty())
    {
        strChannelNameList = "SEQUENCE_A,SEQUENCE_B,SEQUENCE_C,SEQUENCE_D,SEQUENCE_E,SEQUENCE_F,SEQUENCE_G,SEQUENCE_H";
    }
    m_ChannelNameList = strChannelNameList.split(",");

    m_pContextMenu = new QMenu(this);
    m_pActInsert = new QAction(QString::fromLocal8Bit("导入缺陷库"), this);
	m_pActSaveImage = new QAction(QString::fromLocal8Bit("保存图片"), this);
    m_pActThreshold =   new QAction(QString::fromLocal8Bit("图片阈值化"), this);
    connect(m_pActInsert, SIGNAL(triggered()), this, SLOT(slotInsert()));
    connect(m_pActSaveImage, SIGNAL(triggered()), this, SLOT(slotSaveImage()));
    connect(m_pActThreshold, SIGNAL(triggered()), this, SLOT(slotThreshold()));
    m_pActShowRoi = new QAction(QString::fromLocal8Bit("Roi线显示"), this);
	m_pActFreezeImage = new QAction(QString::fromLocal8Bit("冻结图片"), this);
    m_pActShowRoi->setCheckable(true);
    m_pActShowRoi->setChecked(true);
	m_pActFreezeImage->setCheckable(true);
	m_pActFreezeImage->setChecked(false);

    m_pContextMenu->addAction(m_pActInsert);
	m_pContextMenu->addAction(m_pActSaveImage);
    m_pContextMenu->addAction(m_pActShowRoi);
    m_pContextMenu->addAction(m_pActThreshold);
	m_pContextMenu->addAction(m_pActFreezeImage);
    /*新ui修改*/
	/**隐藏控件**/
	JTime curTime = JTime::CurrentLocalDateTime();
	m_strMarkCurday = getCurrentProducsTimeStrPath();

	addDateDataChoose();
	// 读取sceneindex配置
	QSettings setting("./plugins/mainui/setting.ini", QSettings::IniFormat);
	m_scene1Index = setting.value("sceneindex/scene1", 0).toInt();
	m_scene2Index = setting.value("sceneindex/scene2", 1).toInt();
	connect(SENDMSG_INSTANCE, SIGNAL(signalLoginModelChange(int)), this, SLOT(slotsLoginModelChange(int)));

	// 磁盘检测定时器,1分钟检测一次
	m_pTimerDiskCheck = startTimer(60000);
	//
	on_toolButton_prodcuctInfo_clicked();

	//

	IFileVariable* pFile = getStationCfgVariable();

	if (pFile != NULL)
	{
		m_strLine = pFile->getValue("ImageStation", "Line").toCString();
	}
}
void  HomeView::test(OS_LPARAM wparam, OS_LPARAM lparam)
{
	int i = (int)wparam;
	QString str = (char*)lparam;
}
void HomeView::initStationUiCfg()
{

    // 最多五个工位，8个通道
    QString btnName[SEQUE_NUMBER]={"A","B","C","D","E","F","G","H"};
    m_ChannelbtnLayout = ui->HLayout_station0;
    m_BtnGroup = new QButtonGroup();
	int stationCnt = getImageStationCnt();
    for (int index = 0; index < stationCnt ; ++index)
    {
        vector<QToolButton* > imageBtnVec;
        for(int j = 0; j < SEQUE_NUMBER;j++)
        {
            QToolButton* pPBtn = new QToolButton();
            pPBtn->setText(QString("%1%2").arg(index).arg(btnName[j]));
            connect(pPBtn, SIGNAL(clicked()), this, SLOT(on_channelBtn_clicked()));
			pPBtn->setCheckable(true);
            imageBtnVec.push_back(pPBtn);
            m_ChannelbtnLayout->addWidget(pPBtn);
            m_BtnGroup->addButton(pPBtn);
        }
        btnVecMap[index] = imageBtnVec;
    }

    //隐藏所有按钮
    map<int, vector<QToolButton* > >::iterator it = btnVecMap.begin();
    while (it != btnVecMap.end())
    {
        for (int i = 0; i < it->second.size(); ++i)
        {
            it->second.at(i)->hide();
        }
        it++;
    }


    for (int index = 0; index < stationCnt; ++index)
    {
        QWidget* wdg = new QWidget();
        QHBoxLayout *layout = new QHBoxLayout;
        DiagramScene* scene = new DiagramScene((enScene)index);
        diagramSceneVec.push_back(scene);
        GraphWidget* view = new GraphWidget(scene);
        update_dect(index);//更新底色框大小
        layout->addWidget(view);
        layout->setContentsMargins(0, 0, 0, 0);
        wdg->setLayout(layout);
        wdg->setContentsMargins(0, 0, 0, 0);
        connect(scene, SIGNAL(sigCoordinate(QPointF)), this, SLOT(slotUpdatePos(QPointF)));
        connect(scene, SIGNAL(sigRightButtonClicked(QPointF)), this, SLOT(slotRightButtonClicked(QPointF)));

		graphWidgetVec.push_back(view);
		
		ui->tabWidget_defect->addWidget(wdg);
        updateSueqence(index);

        int cnt = ui->tabWidget_defect->count();
    }
    ui->tabWidget_defect->setContentsMargins(0, 0, 0, 0);
    ui->tabWidget_defect->setCurrentIndex(0);
    if (graphWidgetVec.size() > 0)
    {
        m_currentView = graphWidgetVec.at(0);
    }
}


HomeView* HomeView::GetInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new HomeView();
    }
    return m_pInstance;
}
void HomeView::DestroyInstance()
{	
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}
HomeView::~HomeView()
{
	unInit();
}

void HomeView::unInit()
{
	m_pTimer->stop();
	delete m_pTimer;
	delete ui;

	if (m_pFile)
	{
		m_pFile->close();
	}
    SAFE_DELETE(m_pFile);
}
int itest = 0;
void HomeView::timeOut()
{
	//光源
	ui->label_light_1->setPixmap(m_Pix[1]);
	ui->label_light_2->setPixmap(m_Pix[1]);
	//ui->label_light_3->setPixmap(m_Pix[1]);
	//
	UiStatusStrut*  pUiStatus = WorkFlow::getInstance()->GetUiStatus();

	if (NULL != pUiStatus)
	{
		ui->label_card->setPixmap(m_Pix[pUiStatus->bCradOnline]);
		ui->label_cam1->setPixmap(m_Pix[pUiStatus->bCameraOnline[0]]);
		ui->label_cam2->setPixmap(m_Pix[pUiStatus->bCameraOnline[1]]);
		ui->label_cam3->setPixmap(m_Pix[pUiStatus->bCameraOnline[2]]);
		ui->label_cam4->setPixmap(m_Pix[pUiStatus->bCameraOnline[3]]);
		ui->label_AOI->setPixmap(m_Pix[pUiStatus->bAoiIsConnected?1:0]);
	}
	ui->label_clean->setPixmap(m_Pix[SignalCheck::readIo(DI_CLEAN_RUN)]);
	ui->label_shang->setPixmap(m_Pix[SignalCheck::readIo(DI_SHANG_RUN)]);
	ui->label_xia->setPixmap(m_Pix[SignalCheck::readIo(DI_XIA_RUN)]);
	ui->label_clear_mode->setPixmap(m_Pix[CBackPCtoFront::getInstance()->isClearMode()|| SignalCheck::readIo(DI_CLEAR_MODE)]);
}
void HomeView::updateWorkStatus(int state)
{
	RETURN_IF (state == m_preState);
    // 是否统计生产时间
    m_bCountProduceTime = ((state== MAIN_WORK_WORK) || (state== MAIN_WORK_PAUSE));
	m_preState = (E_WORK_STATE)state;
	QBrush clrColor(QColor(255, 128, 128));//默认红色
	// COLOR_BLUE     "rgb(179, 202, 255)"
	// COLOR_YELLOW  "rgb(255, 255, 128)"
	// COLOR_GREEN   "rgb(128, 255, 128)"
	switch (m_preState)
	{
	case MAIN_WORK_INIT:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("初始化"));
		SignalCheck::OpenLight(DO_YELLOW_LIGHT);
		break;
	case MAIN_WORK_WAIT_RESET:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("等待复位"));
		clrColor.setColor(QColor(255, 255, 128));
		SignalCheck::OpenLight(DO_YELLOW_LIGHT);
		break;
	case MAIN_WORK_RESETING:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("复位中"));
		clrColor.setColor(QColor(128, 255, 128));
		SignalCheck::OpenLight(DO_YELLOW_LIGHT);
		
		break;
	case MAIN_WORK_RESET_FAILED:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("复位失败"));
		
		SignalCheck::OpenLight(DO_RED_LIGHT);
		break;
	case MAIN_WORK_ESTOP:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("急停"));
		
		SignalCheck::OpenLight(DO_RED_LIGHT);
		break;
	case MAIN_WORK_STOP:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("停止"));
		clrColor.setColor(QColor(255, 255, 128));
		SignalCheck::OpenLight(DO_YELLOW_LIGHT);
		break;
	case MAIN_WORK_PAUSE:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("暂停"));
		clrColor.setColor(QColor(255, 255, 128));
        if (!m_bCountProduceTime) m_bCountProduceTime=false;
		SignalCheck::OpenLight(DO_YELLOW_LIGHT);
		break;
	case MAIN_WORK_READY:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("就绪"));
		clrColor.setColor(QColor(179, 202, 255));
		SignalCheck::OpenLight(DO_YELLOW_LIGHT);
		break;
	case MAIN_WORK_WORK:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("工作中"));
		clrColor.setColor(QColor(128, 255, 128));
        m_startTime = QDateTime::currentDateTime();
		SignalCheck::OpenLight(DO_GREED_LIGHT);
		break;
	case MAIN_WORK_POWEROFF:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("断电"));

		SignalCheck::OpenLight(DO_RED_LIGHT);
		break;
	case MAIN_WORK_SAFEDOOR:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("安全门被打开"));
	
		SignalCheck::OpenLight(DO_RED_LIGHT);
		break;
	case MAIN_WORK_ERROR:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("出现异常"));

		SignalCheck::OpenLight(DO_RED_LIGHT);
		break;
	case MAIN_WORK_CARD_UNCONNECT:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("运动卡未连接"));

		SignalCheck::OpenLight(DO_YELLOW_LIGHT);
		break;
	case MAIN_WORK_AXISALARM:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("轴报警"));

		SignalCheck::OpenLight(DO_RED_LIGHT);
		break;
	default:
		ui->lable_system_statu->setText(QString::fromLocal8Bit("未知"));

		break;
	}
	setBackColor(ui->frame_sys_status, clrColor);
}
void HomeView::slotsLoginModelChange(int iModel)
{
	bool bSet = (iModel >0) ? true: false;
	
	ui->toolBar_on->setEnabled(bSet);
	ui->toolBar_pause->setEnabled(bSet);
	ui->toolBar_stop->setEnabled(bSet);
	ui->toolBar_reset->setEnabled(bSet);
	//bSet = (iModel >= 2) ? true : false;
	ui->toolButton_change_product->setVisible(bSet);
	ui->toolButton_reCheck->setVisible(bSet);
	ui->toolButton_clear_data->setVisible(bSet);
	ui->comboBox_moduls->setVisible(bSet);
    ui->toolButton_stopBeeAlarm->setVisible(bSet);
	//ui->pushButton_level->setVisible(bSet);
	//ui->label_2->setVisible(bSet);
	//ui->spinBox_set_sn_number->setVisible(bSet);

}
void HomeView::updateRunLog( const QString &runLog, int type )
{
    //SaveLog(runLog, type, true);
    //JTime curTime = JTime::CurrentLocalDateTime();
	ui->textEdit_runLog->Addlogmsg(runLog, (UILogLevel)type);
	if (m_bSaveLog)
	{
		
		QString path = m_strLogPath > 0 ? (m_strLogPath): ("./log");
		QDir dir2(path);
		if (!dir2.exists(path))
		{
			dir2.mkdir(path);
		}
		//QString strSavePath = path + "/ui_log"+ QString("%1-%2-%3").arg(curTime.year()).arg(curTime.month()).arg(curTime.day());
		QString str = (1==getCurrentWorktimes())?"A_":"B_";
		JTime curTime = JTime::CurrentLocalDateTime();
		str += QString::number(curTime.hour());
		QString strSavePath = path + "/ui_log" + getCurrentProducsTimeStrPath()+ str;//日期+班次+当前小时
	    
		LOG_INFO_F_SIMP(strSavePath.toStdString().data(), runLog.toLocal8Bit());
	}
	//ui->textEdit_runLog->Addlogmsg(runLog, (UILogLevel)type);
}

void HomeView::setBackColor(QWidget *widget, const QBrush &abrush)
{
	QPalette palette;
	palette.setBrush(widget->backgroundRole(), abrush);
	widget->setPalette(palette);
	widget->setAutoFillBackground(true);
	//QString styleSheet = widget->styleSheet() + "; background:" + rgbColor;

	//widget->setStyleSheet(styleSheet);

}
void HomeView::slotUpdateUPH(bool bCurrentDate)
{
	stUPHCfg UPHCfgs[UPHCFG_SIZES];
	if (bCurrentDate)
	{
		memcpy(UPHCfgs, m_UPHCfgs, sizeof(stUPHCfg) * UPHCFG_SIZES);
	}
	else {
		memcpy(UPHCfgs, m_UPHCfgsHistory, sizeof(stUPHCfg) * UPHCFG_SIZES);
	}
	ui->spinBox_total->setValue(UPHCfgs[m_iDayClass].iTotalCnt);
	if (UPHCfgs[m_iDayClass].iTotalCnt >0)
	{
		ui->spinBox_ok_radio->setValue((UPHCfgs[m_iDayClass].iOKCnt + UPHCfgs[m_iDayClass].iReCheck) * 100 / UPHCfgs[m_iDayClass].iTotalCnt);
	}

	ui->spinBox_ok_count->setValue(UPHCfgs[m_iDayClass].iOKCnt + UPHCfgs[m_iDayClass].iReCheck);
	ui->spinBox_ng_count->setValue(UPHCfgs[m_iDayClass].iNGCnt);

	ui->spinBox_defectnumber->setValue(UPHCfgs[m_iDayClass].iDefectAvgCnt);
	ui->doubleSpinBox_defectAera->setValue(UPHCfgs[m_iDayClass].dSmirchAvgAera);
	ui->spinBox_smirch->setValue(UPHCfgs[m_iDayClass].iSmirchAvgCnt);
	ui->spinBox_hair->setValue(UPHCfgs[m_iDayClass].iHairAvgCnt);
	ui->spinBox_dirtpits->setValue(UPHCfgs[m_iDayClass].iDirtpitsAvgCnt);
	//
	/*新ui修改*/
	// UPH 更新太快。。
	if (ui->widget_uph->isVisible())
	{
		ui->widget_uph->updateUI(UPHCfgs);
	}	
}

void HomeView::slotUpdefectLevel(int dvdefectCntArr[MAX_DEFECT_SIZE])
{
    ui->widget_defectlevel->updateUI(dvdefectCntArr,false);
}

void HomeView::slotUpdefectHeatMap(double dposx, double dposy)
{
	//ui->widget_heatmap->addPoint(dposx, dposy);
}

void HomeView::slotUpdateDVDefectCnt()
{
    //ui->widget_dvdefecttype->updateUI(&m_stDvToplimitCfg,&m_resSummary);
}

void HomeView::slotStartWork()
{
	m_StartWorkTime = QDateTime::currentDateTime();
}


void HomeView::SlotDisplayImage( int iRow, int iCol, void* pImage )
{
	if (pImage == NULL || !isVisible())
	{
		return;
	}
}
void HomeView::slotCTime( double dCTime )
{
	ui->label_deal_ct->setText(QString::number(dCTime,'f',3));
}

void HomeView::showEvent( QShowEvent * event )
{
    QWidget::showEvent(event);
    if (m_pTimer)
    {
        m_pTimer->start(500);
    }
	IFileVariable *pFile = getDVCfgFileVariable();
	if (NULL != pFile)
	{
		m_strLogPath =  pFile->getValue(G12_DATA_UI_LOG_PATH).toCString().c_str();
		int iFlags = pFile->getValue(G12_DATA_UI_LOG_FLAGS).toInt();
		m_bSaveLog = iFlags & 0x04;
	}


    int stationCnt = getImageStationCnt();
    for (int index = 0; index < stationCnt; ++index)
    {
        updateSueqence(index);//更新通道显示个数
    }
    ShowFlag();
}

static const char* g_ModeFlag[2] = { "流水线模式","离线模式" };
static const char* g_HFlag[3] = { "算法关闭","算法开启" };
static const char* g_timeoutFlag[2] = { "超时关闭","超时开启" };
void HomeView::ShowFlag()
{
    //IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
    IFileVariable* pFileVariable = getCameraSetCfgFileVariable();
    int iModel = 0;
    const char *szKey = NULL;
    if (NULL != pFileVariable)
    {
        //iModel = pFileVariable->getValue(PRODUCT_LINE_RUN_MODEL, 0).toInt();
        iModel = pFileVariable->getValue(IMAGE_PARAM_RUN_MODE, 0).toInt();

    }
    ui->lable_system_mode->setText(QString::fromLocal8Bit(g_ModeFlag[iModel]));
    //

    IFileVariable *pFile = getCameraSetCfgFileVariable();
    int hFlag = 0;
    int timeoutFlag = 0;
    if (pFile)
    {
        if (pFile->getValue(IMAGE_PARAM_DEFECT_DETECT_ENABLE).toBool() || pFile->getValue(IMAGE_PARAM_H_SY_DETECT_ENABLE).toBool())
        {
            hFlag = 1;
        }
        if (pFile->getValue(IMAGE_PARAM_H_TIMEOUT_ENABLE).toBool())
        {
            timeoutFlag = 1;
        }
    }

    ui->label_h_status->setText(QString::fromLocal8Bit(g_HFlag[hFlag]));
    ui->label_timeout_status->setText(QString::fromLocal8Bit(g_timeoutFlag[timeoutFlag]));

}

void HomeView::on_toolButton_clear_log_clicked()
{
	itest++;
	if (QMessageBox::No == QMessageBox::warning(NULL, "warn", QString::fromLocal8Bit("请确认是否清空运行信息？"), QMessageBox::Yes | QMessageBox::No))
		return;
    ui->textEdit_runLog->ClearLog();
}

void HomeView::hideEvent( QHideEvent * event )
{
    if (m_pTimer)
    {
        m_pTimer->stop();
    }
}

void HomeView::on_toolButton_clear_data_clicked()
{
	//
	if (MAIN_WORK_WORK == WorkFlow::getInstance()->GetWorkState()){
		QMessageBox::warning(NULL, "Warning", QString::fromLocal8Bit("请先停止后，再清空生产数据！"));
		return;
	}
	if (1 < CProductUnion::getInstance()->getProductCnt()) {
		QMessageBox::warning(NULL, "Warning", QString::fromLocal8Bit("请等待产品检测完后，再清空生产数据！"));
		return;
	}
	//
	if (QMessageBox::No == QMessageBox::warning(NULL, "warn", QString::fromLocal8Bit("请确认是否清空生产数据？"), QMessageBox::Yes | QMessageBox::No))
		return;

	for (int i = 0; i < UPHCFG_SIZES; i++)
	{
		m_UPHCfgs[i].clear();
	}
	slotUpdateUPH();
	writeUPHFile();
	//int isn = ui->spinBox_set_sn_number->value()-1;
	int isn = 0;
	m_CurrentProductSn = 0;
	m_LastProductSn    = 0;
	ui->label_table_show_res->setText("SN:OK");
	setBackColor(ui->label_table_show_res, QBrush(QColor(71, 164, 233)));
	CProductUnion::getInstance()->writeCurrentDayPrSn(isn);
	RobotCrl::GetInstance()->send_sn_to_xialiaoji(isn);
}
bool HomeView::init()
{
    if(m_binit)
    {
        return true;
    }
	Utils::getPCSocketproxy();
    ILogReport *pLogReport = (ILogReport*)GetSDKInstance("mainflowaction", ILogReport::IDD_LOG_REPORT);
    if (pLogReport)
    {
        pLogReport->addReproter(&HomeView::uilog);
    }


	IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p2)
	{
		m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
		m_defectEnNameVec = p2->getDefectListInstance(0)->getDefectEnNameList();
	}

    WorkFlow::getInstance();
	readUPHFile();
    
	initToolBtns();
	readDvToplimitCfg();
	//
	stDefectFeature Defect;
	Defect.iIndex = 1;
	Defect.strChannelName = "SEQUENCE_A";
	Defect.iDefectLevel = 1;
	CSendMessage::GetInstance()->sendOneDefecUI(Defect,0);
	Defect.strChannelName = "SEQUENCE_D";
	Defect.iIndex++;
	CSendMessage::GetInstance()->sendOneDefecUI(Defect, 1);

    m_binit = true;
	//
	QString str = ui->label_ProductTitle->text();
	updateRunLog(str);
    return true;
}

bool HomeView::deinit()
{
    ILogReport *pLogReport = (ILogReport*)GetSDKInstance("mainflowaction", ILogReport::IDD_LOG_REPORT);
    if (pLogReport)
    {
        pLogReport->removeReproter(&HomeView::uilog);
    }
    WorkFlow::releaseInstance();


    for (int j = 0; j < IMAGE_STATION_MAX_CNT; j++)
    {
        for (int i = 0; i < SEQUE_NUMBER; i++)
        {
            if (NULL != m_pIamgeSrc[j][i])
            {
                if (NULL != m_pIamgeSrc[j][i]->imageAddr)
                {
                    delete[]m_pIamgeSrc[j][i]->imageAddr;
                    m_pIamgeSrc[j][i]->imageAddr = NULL;
                }
                delete m_pIamgeSrc[j][i];
                m_pIamgeSrc[j][i] = NULL;
            }
        }
    }
    return true;
}

bool HomeView::showWindow( QWidget* parent, QRect& rect,bool bPopFlag /*= false*/ )
{
    /*新ui修改*/
	/*if (parent != NULL)
	{
		this->setParent(parent);

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
		move(rect.x(), rect.y());
		this->resize(rect.width(), rect.height());
    }*/
    updateProductDes();
	/*新ui修改*/
    //show();
    return true;
}

bool HomeView::hideWindow()
{
	/*新ui修改*/
	//hide();
    return true;
}

void HomeView::fitSize( const QRect& rect )
{
     /*新ui修改*/
	//resize(rect.width(), rect.height());
}

bool HomeView::notify( QString event, OS_LPARAM wparam, OS_LPARAM lparam )
{
    /*新ui修改*/
    if (event == "start")
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
	else if (event == "pause")
	{
		SignalCheck::SetSignal(SYS_PAUSE);
	}
	else if (event == "stop")
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("收到 stop event"));
		SignalCheck::SetSignal(SYS_STOP);
	}
	else if (event == "reset")
	{
		SignalCheck::SetSignal(SYS_RESET);
	}
    else if (event == "send_product_data_result")
    {
		unsigned int iPsn = (unsigned int)wparam;
		string strResData = *(string*)lparam;
		QStringList strInPara = QString::fromStdString(strResData).split('|', QString::SkipEmptyParts);

		stProductDataRes RevData;
		RevData.sn = to_string(iPsn);
		if (strInPara.size()>=2)
		{
			RevData.ResultFlag = (enResFlag)strInPara.at(0).toInt();
			RevData.iNgOrRiDefectType = strInPara.at(1).toInt();
			if (strInPara.size() > 2)
			{
				RevData.str2Dcode = strInPara.at(2).toStdString();
			}
		}
		enResFlag result = RevData.ResultFlag;
		QString resultStr = "";
		if (result == enResFlag::res_OK || result == enResFlag::res_OK2)
		{
			resultStr = "OK";
		}
		else if (result == enResFlag::res_Ng)
		{
			resultStr = "NG";
		}
		else if (result == enResFlag::res_OK3)
		{
			resultStr = "RI";
		}
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("收到 send_product_result 信号 - %1,%2,data:%3").arg(iPsn).arg(resultStr).arg(QString::fromStdString(strResData)));
		CFrontPCtoBack::getInstance()->pushBackSynResult(iPsn, (int)result, RevData.iNgOrRiDefectType, RevData.str2Dcode);
    }
    else if (event == "send_product_sn")
    {
        unsigned int iPsn = (unsigned int )wparam;
        CProductUnion::getInstance()->writeCurrentDayPrSn((unsigned int)iPsn);
		
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("收到 send_product_sn 信号 - %1").arg(iPsn));
    }
	else if (event == "query_sys_status")
	{
		int iStatus = WorkFlow::getInstance()->GetWorkState();
		int* pStatus = (int*)(wparam);
		*pStatus = iStatus;
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("收到 query_sys_status 信号"));
	}
	else if (event == "query_product_sn")
	{
		unsigned int iProductSN = CProductUnion::getInstance()->readCurrentDayPrSn();;
		unsigned int* pProductSN = (unsigned int*)(wparam);
		*pProductSN = iProductSN;
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("收到 query_product_sn 信号"));
	}
	else if (event == NOTIFY_RESET_UPDATE_DECT)
	{
		int stationCnt = getImageStationCnt();
		for (int index = 0; index < stationCnt; ++index)
		{
			update_dect(index);
		}
	}
	else if (event == NOTIFY_PROGRAM_OPEN || event == NOTIFY_PROGRAM_NEW)
	{
		UpdateProduct();
	}
	else if (event == NOTIFY_PROGRAM_SAVE)
	{
		//SaveProduct();//delete 2021/7/7  相机对应配置原已保存好的-切换型号需要重启动软。所以删除
	}
	else if (event == "offline_test")
	{
		//WorkFlowFun::callFunSetCamerCfg(WorkFlowFun::getInstance(), "");
		//WorkFlowFun::callFunTriggerGrabImage(WorkFlowFun::getInstance(), "");
		if (MAIN_WORK_WORK ==  WorkFlow::getInstance()->GetWorkState() ||
			MAIN_WORK_RESETING == WorkFlow::getInstance()->GetWorkState())
		{
			QMessageBox::warning(NULL, "Warning", QString::fromLocal8Bit("请先停止后，再使用离线"));
			return true;
		}
		CInvokeArithmetic::getInstance()->startGrapLocalImage();

	}
	else if (event == "update_model")
	{
		CInvokeArithmetic::getInstance()->resetProcess();
	}
	else if(event == "ctr_move")
	{
		if (MAIN_WORK_WORK == WorkFlow::getInstance()->GetWorkState() ||
			MAIN_WORK_RESETING == WorkFlow::getInstance()->GetWorkState())
		{
			QMessageBox::warning(NULL, "Warning", QString::fromLocal8Bit("请先停止后再操作"));
			return true;
		}
		QString str = *(QString*)wparam;
		if (str == NOTIFY_MOVE_RIGHT)
		{
			WorkFlowFun::getInstance()->Crlmove(1);
		}
		else if (str == NOTIFY_MOVE_STOP)
		{
			WorkFlowFun::getInstance()->Crlmove(0);
		}
		else if (str == NOTIFY_MOVE_LEFT)
		{
			WorkFlowFun::getInstance()->Crlmove(-1);
		}
	}
	else if (event == "offline_aoto_test"&& NULL != wparam)
	{
		//启动线程自动测试
		int imax = *(int*)wparam;
		CInvokeArithmetic::getInstance()->setAotoTestFlag(true);
		CInvokeArithmetic::getInstance()->startAotoTest(imax);
		
	}
    else if(event == "set_product_sn")
    {
        unsigned int iPsn = (unsigned int )wparam;
        CProductUnion::getInstance()->writeCurrentDayPrSn((unsigned int)iPsn);
		RobotCrl::GetInstance()->send_sn_to_xialiaoji(iPsn);
        CFrontPCtoBack::getInstance()->SendProductSN(iPsn);
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("收到 set_product_sn 信号 - %1").arg(iPsn));
    }
    else if(event == "send_alarm")
    {
        // lucas  process of alarm ;
		SignalCheck::OpenLight(DO_RED_LIGHT);     // 写输出IO
		SignalCheck::writeIo("do_alarm", true);   // 写输出IO
    }
    return true;
}
void HomeView::SetLanguage(QString strlanguage)
{
}
void HomeView::uilog( string msg, int level )
{
    m_pInstance->ui->textEdit_runLog->Addlogmsg(QString::fromUtf8(msg.c_str()), (UILogLevel)level);
}

void HomeView::updateSysStateBtn( int esState )
{
    emit sigupdateSysStateBtn((int) esState);
}

void HomeView::setToolBtnPress( QString strBtnName /*=""*/ )
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

void HomeView::timerEvent(QTimerEvent *event)
{
    if (NULL == m_fileVariable)
    {
        m_fileVariable = getCameraSetCfgFileVariable();
    }
    if (m_pTimerDiskCheck == event->timerId())
    {
        if (NULL != m_fileVariable)
        {
            QString  strPath = "D:\\IMAGE";
            strPath = m_fileVariable->getValue(IMAGE_SAVE_PATH, "D:\\IMAGE").toCString().c_str();
            strPath = strPath.left(2);
            quint64 size = getDiskFreeSpace(strPath.utf16());

			// 磁盘空间不足10G，报警
            if (10 > size)
            {
                CSendMessage::GetInstance()->Send(QString(QString::fromLocal8Bit("磁盘空间不足,剩余空间(%1M)")).arg(size), UILOG_POP);
            }
        }
    }
}

void HomeView::slotupdateSysStateBtn(int esState)
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
    case MAIN_WORK_CARD_UNCONNECT:
    case MAIN_WORK_ERROR:            // 出现异常
        break;
    default:
        break;
    }
}
void HomeView::slotUpdatePos( QPointF pos )
{
    int iGrayValue = 0;
    if (NULL != m_currentView)
    {
        iGrayValue = m_currentView->getImageGray(QPoint(pos.x(), pos.y()));
    }
    QString str = tr("X=") + QString::number((int)pos.x()) + tr(",Y=") + QString::number((int)pos.y()) + tr(",G=") + QString::number(iGrayValue);
    ui->label_curPos->setText(str);
}

void HomeView::slotUpdateImage( QString str,int iType )
{
    switch(iType)
    {
    case update_number:
        ui->label_cur_number->setText(str);
        break;
    case update_getimage:
        ui->label_getimage_sn->setText(str);
        break;
    case update_getiamgetime:
        ui->label_getimagetime->setText(str);
        break;
    case update_deal:
        ui->label_deal_sn->setText(str);
        break;
    case update_dealtime:
        ui->label_deal_time->setText(str);
        break;
    case update_reset:
        str = "0";
        ui->label_cur_number->setText(str);
        ui->label_getimage_sn->setText(str);
        ui->label_getimagetime->setText(str);
        ui->label_deal_sn->setText(str);
        ui->label_deal_time->setText(str);
        break;
    default:

        break;
    }
   // update();
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (NULL != pFile)
	{
		pFile->setValue(CAMERAPARAM_CURPRONUMBER,str.toInt());
	}
}

void HomeView::slotUpdateOK(stProductSummary *resData)
{
	int iOK = 0;
	enResFlag ResultFlag = resData->ResultFlag;
	QBrush clrColor(QColor(71, 164, 233));
	if (ResultFlag == res_OK2)
	{
		iOK = 2;
	}
	else if (ResultFlag == res_OK)
	{
		iOK = 0;
	}
	else if ( ResultFlag == res_Ng)
	{
		iOK = 1;
		clrColor.setColor(QColor(230, 39, 14));
	}
    //else if (ResultFlag == res_recheck || ResultFlag == res_error || ResultFlag == res_OK3)
	else
    {
        iOK = 3;
		clrColor.setColor(QColor(255, 85, 0));
    }
    updateOutTipIO(iOK);
	//QString showMsg = QString::fromLocal8Bit(vShowData[iOK].c_str()) + " "+ QString::fromLocal8Bit(vShowDataOK[iOK].c_str());
	//QString showMsg = QString::fromLocal8Bit(vShowDataOK[iOK].c_str());

	ui->label_show_ok_sn->setText("SN:" + QString::fromStdString(resData->sn));
	//ui->label_show_ok->setText(showMsg);
	ui->label_show_ok->setText(QString::fromStdString(resData->str2Dcode));
	setBackColor(ui->label_show_ok, clrColor);
}
void HomeView::updateOutTipIO(int index)
{
	bool bOpenG = false;
	bool bOpenY = false;
	bool bOpenR = false;
	switch (index)
	{
	case 0:
	case 2:
		bOpenG = true;
		break;
	case 1:
		bOpenR = true;
		break;
	case 3:
		bOpenY = true;
		break;
	} 
	SignalCheck::writeIo("do_out_green", bOpenG);
	Sleep(150);
	SignalCheck::writeIo("do_out_yellow", bOpenY);
	Sleep(150);
	SignalCheck::writeIo("do_out_red", bOpenR);
	Sleep(150);
}

void HomeView::updateProductDes()
{
    QString strPorgramDesc;
    QString strProgramDir = getCurrentProgramPath().c_str();
    QFile file(strProgramDir + "/" + getCurrentProgramName().c_str() + ".txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        strPorgramDesc = in.readAll();
    }
    if (strPorgramDesc.isEmpty())
    {
        strPorgramDesc = getCurrentProgramName().c_str();
    }
    ui->label_show_type->setText(strPorgramDesc);
}

void HomeView::slotUpdateTableOK(stProductSummary resData)
{
	int iType = 0;
	QString strData = "";
	QBrush clrColor(QColor(71, 164, 233));
	if (resData.ResultFlag == res_OK2)
	{
		iType = 2; // 2 
	}
	else if (resData.ResultFlag == res_OK  )
	{
		iType = 0; //0 OK
	}
	else if ( resData.ResultFlag == res_Ng)
	{
		iType = 1; //1 NG
		clrColor.setColor(QColor(230, 39, 14));
	}
    //else if (resData.ResultFlag == res_recheck || resData.ResultFlag == res_error || resData.ResultFlag == res_OK3 )
	else
    {
        iType = 3; //重测
		clrColor.setColor(QColor(255, 85, 0));
    }
	//QString showMsg = "SN:" + QString::fromStdString(resData.sn) + "  " + QString::fromLocal8Bit(vShowData[iType].c_str()) + "  " + QString::fromLocal8Bit(vShowDataOK[iType].c_str());
	QString showMsg = "SN:" + QString::fromStdString(resData.sn) + "  " + QString::fromLocal8Bit(vShowDataOK[iType].c_str());
	//
	JTime curTime = JTime::CurrentLocalDateTime();
	QString strTime = QString("%1:%2:%3").arg(curTime.hour()).arg(curTime.minute()).arg(curTime.second());
	//
	strData = strTime + "," + QString::fromStdString(resData.sn) + "," + QString::fromLocal8Bit(vShowData[iType].c_str()) + ",";// sn+res;


	ui->label_table_show_res->setText(showMsg);
	
	setBackColor(ui->label_table_show_res, clrColor);
	QString  strPath = "D:\\IMAGE";

    IFileVariable *fileVariable = getCameraSetCfgFileVariable();
	strPath = fileVariable->getValue(IMAGE_SAVE_PATH, "D:\\IMAGE").toCString().c_str();
	
	QString filePathName = QString("%1\\%2").arg(strPath).arg(getCurrentProducsTimeStrPath());
	QDir dir(filePathName);
	if (!dir.exists(filePathName))
	{
		dir.mkdir(filePathName);
	}
	filePathName += QString("\\grab");
	if (!dir.exists(filePathName))
	{
		dir.mkdir(filePathName);
	}

	filePathName = filePathName + "\\" + resData.sn.c_str();
	filePathName += ".png";
	QPixmap pResultGrab = grab(QRect(ui->widget_right->mapTo(this, QPoint(0, 0)), ui->widget_right->size()));
	pResultGrab.save(filePathName, "png");
	//
	double dArea = resData.dvdefectSummaryAreaArr[en_dv_Smirch] + resData.dvdefectSummaryAreaArr[en_dv_hair] + resData.dvdefectSummaryAreaArr[en_dv_dirtpits];

	strData += QString("%1,%2,").arg(resData.iDefectNumber).arg(QString::number(dArea, 'f', 4));
	strData += QString("%1,%2,%3,%4").arg(resData.dvdefectSummaryArr[en_dv_Smirch]).arg(resData.dvdefectSummaryArr[en_dv_hair]).arg(resData.dvdefectSummaryArr[en_dv_dirtpits]).arg(resData.iCT);
	saveEndResData(strData);
	//
    //update();// 优化页面卡顿，屏蔽update();
}

void HomeView::slotRightButtonClicked(QPointF pos)
{
	QPoint postmp = cursor().pos();
    m_pContextMenu->exec(postmp);
}

void HomeView::slotInsert()
{

}

void HomeView::slotSaveImage()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		"./logimage/untitled.jpg",
		tr("Images (*.png *.bmp *.jpg)"));
    m_currentView->saveImage(fileName);
}

void HomeView::slotThreshold()
{
    ImageSrcStrut *pImageSrcStrut = NULL;
    if(m_iImageChoose[0] < SEQUE_NUMBER && m_iImageChoose[0] >=0)
    {
        pImageSrcStrut = m_pIamgeSrc[0][m_iImageChoose[0]];
    }
    if(pImageSrcStrut == NULL)
        return ;
    Thresholdui ThresholdWidget(this);
	ThresholdWidget.setImageData(pImageSrcStrut);
    ThresholdWidget.exec();
	return;
}

void HomeView::on_pushButton_update_choose_clicked(int change)
{
    static int iCkeck[8] = {0,0,0,0,0,0,0,0};
    memset(iCkeck,0,sizeof(iCkeck));
	iCkeck[change] = 1;
    CSendMessage::GetInstance()->sendChooseShow(iCkeck,ui->tabWidget_defect->currentIndex());
}
//
void HomeView::setImageRoi(bool bShow)
{

}
//
void HomeView::slotProductInfo(CImageProduct* pProduct)
{
	if (pProduct != NULL)
	{	
		slotReShowProductInfo(pProduct);
		pProduct->setUpdateUIFinish(true);
		CInvokeArithmetic::getInstance()->setAotoTestFlag();
	}
}
void HomeView::slotReShowProductInfo(CImageProduct* pProduct)
{
	CImageProduct* pCurrentProduct = pProduct;
	m_CurrentProductSn = pProduct->getProductSn();
	if (pCurrentProduct == NULL || !CProductUnion::getInstance()->isProductExist(pCurrentProduct))
	{
		return;
	}
	QTime m_startTimeUI = QTime::currentTime();
	//清除图像上的缺陷框
	if (!m_pActFreezeImage->isChecked())
	{
		CSendMessage::GetInstance()->Send("update- ui start ");
		CSendMessage::GetInstance()->sendClearTable();
	}
	//显示图片
	if (!m_pActFreezeImage->isChecked())
	{
        for(int index = 0;index < getImageStationCnt();index++)
        {

            vector<string> ChannelVec;
            getEnabelChannle(ChannelVec, SString::IntToStr(index));  //默认first
            for (int i = 0; i < ChannelVec.size() && m_resSummary.ResultFlag != res_error; i++)
            {
                QString ChannelName = QString::fromStdString(ChannelVec.at(i));
                QString Channel = ChannelName.mid(0, 10);

                // 拷贝原图
                if (i < SEQUE_NUMBER)
                {
                    if (NULL == m_pIamgeSrc[index][i])
                    {
                        m_pIamgeSrc[index][i] = new ImageSrcStrut;
                    }
                    if (m_pIamgeSrc[index][i]->iImageWide != pCurrentProduct->getProductWidth(index)
                        || m_pIamgeSrc[index][i]->iImageHight != pCurrentProduct->getProductHeight(index))
                    {
                        if (NULL != m_pIamgeSrc[index][i]->imageAddr)
                        {
                            delete[] m_pIamgeSrc[index][i]->imageAddr;
                            m_pIamgeSrc[index][i]->imageAddr = NULL;
                        }
                        m_pIamgeSrc[index][i]->iImageWide = pCurrentProduct->getProductWidth(index);
                        m_pIamgeSrc[index][i]->iImageHight = pCurrentProduct->getProductHeight(index);
                    }
                    if (NULL == m_pIamgeSrc[index][i]->imageAddr)
                    {
                        m_pIamgeSrc[index][i]->imageAddr = new unsigned char[m_pIamgeSrc[index][i]->iImageWide*m_pIamgeSrc[index][i]->iImageHight];
                    }
                    //m_pIamgeSrc[j]->iCam = iCameraIndex;
                    m_pIamgeSrc[index][i]->strChannel = Channel.toStdString();
                    //m_pIamgeSrc[i]->iCam = i;
                    m_pIamgeSrc[index][i]->iCam = 0;
                    if (pCurrentProduct->getImageBuffer(ChannelName,index))
                    {
                        unsigned char *img_data = pCurrentProduct->getImageBuffer(ChannelName,index)->rawData.img_data;
                        IFileVariable *pCameraSetCfgFile = getCameraSetCfgFileVariable();
                        if (pCameraSetCfgFile == NULL)
                        {
                            return;
                        }
                        bool bEnableStatic = pCameraSetCfgFile->getValue(IMAGE_PARAM_STATIC_ENABLE, false).toBool();
                        if (bEnableStatic)
                        {
                            img_data = pCurrentProduct->getImageBuffer(ChannelName,index)->staticData.img_data;
                        }
                        memcpy(m_pIamgeSrc[index][i]->imageAddr, img_data, m_pIamgeSrc[index][i]->iImageWide*m_pIamgeSrc[index][i]->iImageHight);
                    }
                    //end
                    CSendMessage::GetInstance()->sendImageDefecTable(m_pIamgeSrc[index][i], index); // for fix根据DV 结果适配界面
			}
		}

        }
    }

	m_resSummary.init();
	m_resSummary = pCurrentProduct->m_stProductSummary;

	// 缺陷显示


	int iIndex = 0;
	double dMaxArea = 0.0;
	double heatPosX = 0.0;
	double heatPosY = 0.0;
	int iNgDefectType = 0;
	stProductSummary showRes;
	string strSn = to_string(pCurrentProduct->getProductSn());;
	m_resSummary.sn = strSn;

    /****************/
	if (!m_pActFreezeImage->isChecked())
	{
	  //	CSendMessage::GetInstance()->sendAddCutROIUI(pCurrentProduct->m_cutRoiInfo[m_scene1Index], 0);
	  //	CSendMessage::GetInstance()->sendAddCutROIUI(pCurrentProduct->m_cutRoiInfo[m_scene2Index], 1);
	  
	// m_HomeViewCurPolygon[m_scene1Index][0] = pCurrentProduct->m_curPolygon[m_scene1Index][0];
	// m_HomeViewCurPolygon[m_scene1Index][1] = pCurrentProduct->m_curPolygon[m_scene1Index][1];
	// m_HomeViewCurPolygon[m_scene2Index][0] = pCurrentProduct->m_curPolygon[m_scene2Index][0];
	// m_HomeViewCurPolygon[m_scene2Index][1] = pCurrentProduct->m_curPolygon[m_scene2Index][1];
	}

    CSendMessage::GetInstance()->Send("data process -begin");

    //ui->widget_heatmap->clearUI();//清除当片热力图数据
	//QStringList strMsgToPenma;
	//int iSn = QString::fromStdString(strSn).toInt();
	//
	//CPrMarkUnion::getInstance()->SnToPenmaData(iSn, strMsgToPenma);
	//

    for (int i = 0; i < pCurrentProduct->pDefectStrutVec->size(); i++)
    {
        stDefectFeature *pDefectRes = pCurrentProduct->pDefectStrutVec->at(i);

        stDefectFeature mark(*pDefectRes);
        mark.iIndex = iIndex;	
        		//更新缺陷列表
		if (!m_pActFreezeImage->isChecked())
		{
            if(mark.iStation < diagramSceneVec.size() && mark.iStation >=0)
            {
                if(diagramSceneVec.at(mark.iStation) != NULL)
                {
					CSendMessage::GetInstance()->sendOneDefecUI(mark, mark.iStation);
                }
            }
		}
        iIndex++;
		//CPrMarkUnion::getInstance()->DefectToPenmaData(pDefectRes, m_strLine,strMsgToPenma);
		//
        //ui->widget_heatmap->addData(mark.iDvDefectType, mark.posx, mark.posy,true);//当片热力图数据增加同时写入文件保存
        //ui->page_heatmap_2->addData(mark.iDvDefectType, mark.posx, mark.posy);     //当班次热力图点增加-不写文件
    }
	//ui->widget_heatmap->updateUI();// 更新缺陷热力图
	/*新ui修改*/
	ui->widget_defectTableView->dataSort();


	//if (res_OK3 == m_resSummary.ResultFlag || res_Ng == m_resSummary.ResultFlag)//
	//{
	//	m_ResFlag[m_resSummary.ResultFlag]++;
	//	// 判断 连续N个  RI NG 报警
	//	RiorNgAlarm();
	//	//
	//}
    //else
    //{
	//	m_ResFlag[res_OK3] = 0;
	//	m_ResFlag[res_Ng] = 0;
	//}

	
	if (m_CurrentProductSn != m_LastProductSn) // 同一个产品重测时，不进行计数
	{
		QTime m_endTimeUI = QTime::currentTime();
		int iCT = 0;
		int iImageCT = pCurrentProduct->grapImageElapsedTime();
		int iProcessCT = pCurrentProduct->processElapsedTime();
		int iUiCT = m_startTimeUI.msecsTo(m_endTimeUI);
		iCT = iImageCT > iProcessCT ? iImageCT : iProcessCT;
		iCT = iCT > iUiCT ? iCT : iUiCT;
		//slotCTime((double)iCT / 1000);
		m_resSummary.iCT = iCT;
		slotUpdateImage(QString::number((double)iImageCT / 1000), update_getiamgetime);
		slotUpdateImage(QString::number((double)iProcessCT / 1000), update_dealtime);
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("采图时间：%1ms 流程时间：%2ms  界面时间: %3ms  ").arg(iImageCT).arg(iProcessCT).arg(iUiCT));
		m_LastProductSn = m_CurrentProductSn;

		updateProductUPH(); // 更新产品的UPH 信息
        //updateDefectLevel(); // 更新当前产品的缺陷等级信息 饼状图
        //ui->widget_regioncnt->updateUI(&m_resSummary);
		slotUpdateTableOK(m_resSummary);
		//COutUnion::getInstance()->setResultFlag(atoi(m_resSummary.sn.c_str()),m_resSummary.ResultFlag);
		stProductDataRes dataFlag;
		dataFlag.sn = m_resSummary.sn;
		dataFlag.str2Dcode = m_resSummary.str2Dcode;
		dataFlag.iNgOrRiDefectType = m_resSummary.iProductNgDefectType;
		dataFlag.ResultFlag = m_resSummary.ResultFlag;
		COutUnion::getInstance()->setResultDataFlag(atoi(m_resSummary.sn.c_str()),&dataFlag);
		
	}
	//test
	//m_resSummary.ResultFlag = res_Ng;
	//m_resSummary.iProductNgDefectType = rand()%5;
	//m_resSummary.iProductNgDefectType++;
	if (0 == ui->comboBox_moduls->currentIndex())
	{
		outputDataSave::GetInstance()->saveFoxTrackPadData(m_resSummary);
	}
	else {
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("测试模式--保存数据"),UILOG_WARN);
	}
	
	  
	// 尺寸测量数据更新
    //ui->widget_image_layout->setValues(pProduct->m_glassSize);

    //CFrontPCtoBack::getInstance()->sendProductInfo(pProduct->getProductSn(), m_resSummary.ResultFlag);
	CSendMessage::GetInstance()->Send("update ui -end");
	//slotUpdateShowEnd(resData);
}
void HomeView::RiorNgAlarm()
{
	IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
	bool ri_enable = false;
	bool ng_enable = false;
	int ri_max = 0;
	int ng_max = 0;

	if (NULL != pFileVariable)
	{
		ri_enable = pFileVariable->getValue(PRODUCT_WORK_ALARM_RI_ENABLE).toBool();
		ri_max = pFileVariable->getValue(PRODUCT_WORK_ALARM_RI_MAX).toInt();
		ng_enable = pFileVariable->getValue(PRODUCT_WORK_ALARM_NG_ENABLE).toBool();
		ng_max = pFileVariable->getValue(PRODUCT_WORK_ALARM_NG_MAX).toInt();
	}
	//
	if ((ri_enable &&(ri_max <= m_ResFlag[res_OK3])) || (ng_enable && (ng_max <= m_ResFlag[res_Ng])))
	{
		//通知报警
		SignalCheck::OpenLight(DO_RED_LIGHT);     // 写输出IO
		SignalCheck::writeIo("do_alarm", true);   // 写输出IO
	}


}


//#define CSV_FILE_SN_RES_HEADER "时间,SN,结果,不良类别,缺陷个数,脏污面积,脏污个数,毛发个数,尘点个数\n"
#define CSV_FILE_SN_RES_HEADER "时间,SN,结果,缺陷个数,脏污面积,脏污个数,毛发个数,尘点个数,CT\n"
void HomeView::saveEndResData(const QString &strData)
{
	if (strData.isEmpty())
	{
		return;
	}

	QString strDir = getSaveResultFilePath();

	//JTime curTime = JTime::CurrentLocalDateTime();
	QString datePth = m_strMarkCurday;//getCurrentLocalDateTimeStrPath();

	QString strSavePath = QString("%1/%2").arg(strDir).arg(datePth);
	QDir dir2(strSavePath);
	if (!dir2.exists(strSavePath))
	{
		dir2.mkdir(strSavePath);

	}
	QString csvFileName = strSavePath + "/" + datePth + "_finalReslut.csv";

	//
	QString addData="";
	if (!QFile::exists(csvFileName))
	{
		 addData = QString::fromLocal8Bit(CSV_FILE_SN_RES_HEADER);
	}
	//
	QFile csvFile(csvFileName);
	
	if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		return ;
	}
	 addData += strData + "\n";

	csvFile.write(addData.toLocal8Bit());
	csvFile.close();
}
void HomeView::saveDefectResult(QString strData)
{
    IFileVariable *pUserSettingFile = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
    if (NULL != pUserSettingFile)
    {
        bool bEnable = pUserSettingFile->getValue(LOG_SAVE_DEFECT_RESULT_ENABLE, false).toBool();
        if (bEnable)
        {
            // 路径不存在，创建路径
			QString strDir = getSaveResultFilePath();

            //JTime curTime = JTime::CurrentLocalDateTime();
			QString strDatePth = m_strMarkCurday;//getCurrentLocalDateTimeStrPath();
            QString strSavePath = QString("%1/%2").arg(strDir).arg(strDatePth);
            QDir dir2(strSavePath);
            if (!dir2.exists(strSavePath))
            {
                dir2.mkdir(strSavePath);
            }

            QString csvFileName = strSavePath + "/" + QString::number(m_CurrentProductSn) + ".csv";
            QFile csvFile(csvFileName);
            bool bIsOpen = false;
            bIsOpen = csvFile.open(QIODevice::WriteOnly | QIODevice::Text);
            if (bIsOpen)
            {
                csvFile.write(strData.toLocal8Bit());
                csvFile.close();
            }
            //QString strSavePathAll = strDatePth;
            QString csvFileNameAll = strSavePath + "/" + strDatePth + ".csv";
            
            QFile csvFileAll(csvFileNameAll);
            bool bIsOpenAll = false;
            bIsOpenAll = csvFileAll.open(QIODevice::Append | QIODevice::Text);

            if (bIsOpenAll)
            {
                strData = QString::number(m_CurrentProductSn) + " - " + strData;
                csvFileAll.write(strData.toLocal8Bit());
                csvFileAll.close();
            }

        }
    }
}



void HomeView::slotUpdateShowEnd(unsigned long long sn, enResFlag RestFlag,string str2Dcode)
{
	
	int iOK = 0;
	enResFlag ResultFlag = RestFlag;
	QBrush clrColor(QColor(71, 164, 233));
	if (ResultFlag == res_OK2|| ResultFlag == res_OK)
	{
		iOK = 0;
	}
	else if (ResultFlag == res_Ng)
	{
		iOK = 1;
		clrColor.setColor(QColor(230, 39, 14));
	}
	else
	{
		iOK = 3;
		clrColor.setColor(QColor(255, 85, 0));
	}
	QString showMsg = QString::fromStdString(str2Dcode);
	if (showMsg.length()>12)
	{
		showMsg = showMsg.mid(4,7);

	}
	ui->label_show_ok_sn->setText("SN:" + QString::number(sn));
	ui->label_show_ok->setText(showMsg);
	setBackColor(ui->label_show_ok, clrColor);
    //update(); // 优化页面卡顿，屏蔽update();
}



void HomeView::UpdateDispImage(int indexChannel, int iScene)
{
    if (indexChannel < 0 || indexChannel >= SEQUE_NUMBER || iScene >= IMAGE_STATION_MAX_CNT)
	{
		return;
	}
	int itype = 0;
	ImageSrcStrut *pImageSrc;
    pImageSrc = m_pIamgeSrc[iScene][indexChannel];
    int iCameraIndex = 0;
    int iType = 0;
    string strChannelNames = getCurrentProgramRecipesList();
    std::vector<std::string> ChannelNameList = SString::split(strChannelNames, ",");
	GraphWidget* view = graphWidgetVec.at(iScene);
	ui->tabWidget_defect->setCurrentIndex(iScene);
	if(NULL != pImageSrc && NULL != pImageSrc->imageAddr)
	{
		view->showPictureInLabel(pImageSrc->imageAddr, NULL, NULL, pImageSrc->iImageWide, pImageSrc->iImageHight);
	}
    SENDMSG_INSTANCE->sendShowChoosePic(indexChannel);
    
	m_curCamIndex = iCameraIndex;
	m_currentChannel[iCameraIndex] = indexChannel;
	on_pushButton_update_choose_clicked(indexChannel);

}
void HomeView::showCutRoi(bool bChannel_A)
{
	// roi  正面 abc efg
	if (!m_pActShowRoi->isChecked())
	{
		return;
	}
}
void HomeView::readUPHFile(QString date)
{
	CIniFile iniFile;

	QString strDir = getSaveResultFilePath();

	//JTime curTime = JTime::CurrentLocalDateTime();
	QString strCurrDatPath = m_strMarkCurday;//getCurrentLocalDateTimeStrPath();
	QString strSavePath = QString("%1/%2").arg(strDir).arg(strCurrDatPath);

	if (!date.isEmpty())
	{
		strSavePath = strDir +"/"+ date;
		for (int i = 0; i < UPHCFG_SIZES; i++)
		{
			m_UPHCfgsHistory[i].clear();
		}
	}
	
	QDir dir2(strSavePath);
	if (!dir2.exists(strSavePath))
	{
		dir2.mkdir(strSavePath);
		return;
	}

	QString csvFileName = strSavePath + "/" + strCurrDatPath + "_UPH.ini";
	if (!date.isEmpty())
	{
		csvFileName = strSavePath + "/" + date + "_UPH.ini";
	}
	if (!QFile::exists(csvFileName))
	{
		return;
	}
	iniFile.SetPath(csvFileName.toStdString());
	iniFile.ReadFile();
	string firstKeyName = UPHINFO;
	stUPHCfg tmpUPH[UPHCFG_SIZES];

	//total
	string keyname = firstKeyName;
	tmpUPH[0].iTotalCnt=iniFile.GetValueI(keyname, "TotalCnt");
	tmpUPH[0].iOKCnt=iniFile.GetValueI(keyname, "OKCnt");
	tmpUPH[0].iNGCnt=iniFile.GetValueI(keyname, "NGCnt");
	tmpUPH[0].iReCheck=iniFile.GetValueI(keyname, "ReCheck");
	tmpUPH[0].iReClean=iniFile.GetValueI(keyname, "ReClean");
	tmpUPH[0].iAiError = iniFile.GetValueI(keyname, "AiError");
	//
	tmpUPH[0].iDefectAvgCnt = iniFile.GetValueI(keyname, "DefectAvgCnt");
	tmpUPH[0].iSmirchAvgCnt = iniFile.GetValueI(keyname, "SmirchAvgCnt");
	tmpUPH[0].iHairAvgCnt   = iniFile.GetValueI(keyname, "HairAvgCnt");
	tmpUPH[0].iDirtpitsAvgCnt = iniFile.GetValueI(keyname, "DirtpitsAvgCnt"); 
	tmpUPH[0].dSmirchAvgAera = iniFile.GetValueI(keyname, "SmirchAvgAera");
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
        tmpUPH[0].NgDefectType[i] = iniFile.GetValueI(keyname, m_defectEnNameVec[i]);
	}
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
		tmpUPH[0].RiDefectType[i] = iniFile.GetValueI(keyname, "Ri_"+m_defectEnNameVec[i]);
	}
	//day
	keyname = firstKeyName+"_DAY";
	tmpUPH[1].iTotalCnt = iniFile.GetValueI(keyname, "TotalCnt");
	tmpUPH[1].iOKCnt = iniFile.GetValueI(keyname, "OKCnt");
	tmpUPH[1].iNGCnt = iniFile.GetValueI(keyname, "NGCnt");
	tmpUPH[1].iReCheck = iniFile.GetValueI(keyname, "ReCheck");
	tmpUPH[1].iReClean = iniFile.GetValueI(keyname, "ReClean");
	tmpUPH[1].iAiError = iniFile.GetValueI(keyname, "AiError");
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
        tmpUPH[1].NgDefectType[i] = iniFile.GetValueI(keyname, m_defectEnNameVec[i]);
	}
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
		tmpUPH[1].RiDefectType[i] = iniFile.GetValueI(keyname, "Ri_"+m_defectEnNameVec[i]);
	}
	//night
	keyname = firstKeyName + "_NIGHT";
	tmpUPH[2].iTotalCnt = iniFile.GetValueI(keyname, "TotalCnt");
	tmpUPH[2].iOKCnt = iniFile.GetValueI(keyname, "OKCnt");
	tmpUPH[2].iNGCnt = iniFile.GetValueI(keyname, "NGCnt");
	tmpUPH[2].iReCheck = iniFile.GetValueI(keyname, "ReCheck");
	tmpUPH[2].iReClean = iniFile.GetValueI(keyname, "ReClean");
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
        tmpUPH[2].NgDefectType[i] = iniFile.GetValueI(keyname, m_defectEnNameVec[i]);
	}
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
		tmpUPH[2].RiDefectType[i] = iniFile.GetValueI(keyname, "Ri_" + m_defectEnNameVec[i]);
	}
	//time 
	for (int i = 3; i < UPHCFG_SIZES; i++)//index first by 3
	{
		keyname = firstKeyName + "_" + QString::number(i-2).toStdString(); //time 1--24
		tmpUPH[i].iTotalCnt = iniFile.GetValueI(keyname, "TotalCnt");
		tmpUPH[i].iOKCnt = iniFile.GetValueI(keyname, "OKCnt");
		tmpUPH[i].iNGCnt = iniFile.GetValueI(keyname, "NGCnt");
		tmpUPH[i].iReCheck = iniFile.GetValueI(keyname, "ReCheck");
		tmpUPH[i].iReClean = iniFile.GetValueI(keyname, "ReClean");
		tmpUPH[i].iAiError = iniFile.GetValueI(keyname, "AiError");
	}
	//
	if (date.isEmpty()){
		memcpy(m_UPHCfgs, tmpUPH, sizeof(stUPHCfg) * UPHCFG_SIZES);
	}
	else { memcpy(m_UPHCfgsHistory, tmpUPH, sizeof(stUPHCfg) * UPHCFG_SIZES); }
	return;
}
void HomeView::addDateDataChoose()
{
	QString strDir = getSaveResultFilePath();;
	QDir *NewQir = new QDir(strDir);
	NewQir->setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
	if (!NewQir->exists(strDir))
	{
		return;
	}
	QStringList dateNames = NewQir->entryList();
	if (dateNames.size() ==0)
	{
		return;
	}
	int index = 0;
	int i = 0;
	ui->comboBox_date_data->blockSignals(true); //增加日期时不触发槽
	foreach (QString date, dateNames)
	{
		ui->comboBox_date_data->addItem(date);
		if (m_strMarkCurday == date)
		{
			index = i;
		}
		i++;
	}
	//
	//
	//JTime curTime = JTime::CurrentLocalDateTime();
	QString lastDate = dateNames.at(dateNames.size() - 1);
	//QString datePath = getCurrentLocalDateTimeStrPath();

	if (lastDate != m_strMarkCurday ) //当前目录尚未创建
	{
		ui->comboBox_date_data->addItem(m_strMarkCurday);
		ui->comboBox_date_data->blockSignals(false);//恢复正常 
		ui->comboBox_date_data->setCurrentIndex(dateNames.size());
	}
	else {
		ui->comboBox_date_data->blockSignals(false);//恢复正常
		ui->comboBox_date_data->setCurrentIndex(index);
	}
	delete NewQir;
	NewQir = NULL;
}
void HomeView::writeUPHFile()
{
	CIniFile iniFile;

	// 路径不存在，创建路径
	QString strDir = getSaveResultFilePath();

	//JTime curTime = JTime::CurrentLocalDateTime();

	//if (8 > curTime.hour())
	//{
	//	m_strMarkCurday = QString("%1").arg(getCurrentLocalDateTimeStrPath());
	//}
	QString strSavePath = QString("%1/%2").arg(strDir).arg(m_strMarkCurday);

	QDir dir2(strSavePath);
	if (!dir2.exists(strSavePath))
	{
		dir2.mkdir(strSavePath);

	}

	QString csvFileName = strSavePath + "/" + QString("%1").arg(m_strMarkCurday) + "_UPH.ini";

	iniFile.Initialize(csvFileName.toStdString());
	iniFile.ReadFile();
	
	string firstKeyName = UPHINFO;
	//total
	string keyname = firstKeyName;
	iniFile.SetValueI(keyname, "TotalCnt", m_UPHCfgs[0].iTotalCnt);
	iniFile.SetValueI(keyname, "OKCnt", m_UPHCfgs[0].iOKCnt);
	iniFile.SetValueI(keyname, "NGCnt", m_UPHCfgs[0].iNGCnt);
	iniFile.SetValueI(keyname, "ReCheck", m_UPHCfgs[0].iReCheck);
	iniFile.SetValueI(keyname, "ReClean", m_UPHCfgs[0].iReClean);
	iniFile.SetValueI(keyname, "AiError", m_UPHCfgs[0].iAiError);

	iniFile.SetValueI(keyname, "DefectAvgCnt", m_UPHCfgs[0].iDefectAvgCnt  );
	iniFile.SetValueI(keyname, "SmirchAvgCnt", m_UPHCfgs[0].iSmirchAvgCnt  );
	iniFile.SetValueI(keyname, "HairAvgCnt", m_UPHCfgs[0].iHairAvgCnt );
	iniFile.SetValueI(keyname, "DirtpitsAvgCnt", m_UPHCfgs[0].iDirtpitsAvgCnt);


	//if (m_resSummary.iProductNgDefectType >0 && m_resSummary.iProductNgDefectType <DEFECT_TYPE_SIZE)
	//{
	//	int ngdefectCnt = iniFile.GetValueI(keyname, szdefectTypeEn[m_resSummary.iProductNgDefectType], 0) + 1;
	//	iniFile.SetValueI(keyname, szdefectTypeEn[m_resSummary.iProductNgDefectType], ngdefectCnt);
	//}
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
        iniFile.SetValueI(keyname, m_defectEnNameVec[i], m_UPHCfgs[0].NgDefectType[i]);
	}
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
		iniFile.SetValueI(keyname, "Ri_" + m_defectEnNameVec[i], m_UPHCfgs[0].RiDefectType[i]);
	}
	//day
	keyname = firstKeyName + "_DAY";
	iniFile.SetValueI(keyname, "TotalCnt", m_UPHCfgs[1].iTotalCnt);
	iniFile.SetValueI(keyname, "OKCnt", m_UPHCfgs[1].iOKCnt);
	iniFile.SetValueI(keyname, "NGCnt", m_UPHCfgs[1].iNGCnt);
	iniFile.SetValueI(keyname, "ReCheck", m_UPHCfgs[1].iReCheck);
	iniFile.SetValueI(keyname, "ReClean", m_UPHCfgs[1].iReClean);
	iniFile.SetValueI(keyname, "AiError", m_UPHCfgs[1].iAiError);
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
        iniFile.SetValueI(keyname, m_defectEnNameVec[i], m_UPHCfgs[1].NgDefectType[i]);
	}
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
		iniFile.SetValueI(keyname, "Ri_" + m_defectEnNameVec[i], m_UPHCfgs[1].RiDefectType[i]);
	}
	//night
	keyname = firstKeyName+"_NIGHT";
	iniFile.SetValueI(keyname, "TotalCnt", m_UPHCfgs[2].iTotalCnt);
	iniFile.SetValueI(keyname, "OKCnt", m_UPHCfgs[2].iOKCnt);
	iniFile.SetValueI(keyname, "NGCnt", m_UPHCfgs[2].iNGCnt);
	iniFile.SetValueI(keyname, "ReCheck", m_UPHCfgs[2].iReCheck);
	iniFile.SetValueI(keyname, "ReClean", m_UPHCfgs[2].iReClean);
	iniFile.SetValueI(keyname, "AiError", m_UPHCfgs[2].iAiError);
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
        iniFile.SetValueI(keyname, m_defectEnNameVec[i], m_UPHCfgs[2].NgDefectType[i]);
	}
	for (int i = 1; i < m_defectEnNameVec.size(); i++)
	{
		iniFile.SetValueI(keyname, "Ri_" + m_defectEnNameVec[i], m_UPHCfgs[2].RiDefectType[i]);
	}
	// time  1--24

	for (int i = 3; i < UPHCFG_SIZES; i++)
	{
		keyname = firstKeyName + "_" + QString::number(i - 2).toStdString();
		iniFile.SetValueI(keyname, "TotalCnt", m_UPHCfgs[i].iTotalCnt);
		iniFile.SetValueI(keyname, "OKCnt",    m_UPHCfgs[i].iOKCnt);
		iniFile.SetValueI(keyname, "NGCnt",    m_UPHCfgs[i].iNGCnt);
		iniFile.SetValueI(keyname, "ReCheck",  m_UPHCfgs[i].iReCheck);
		iniFile.SetValueI(keyname, "ReClean",  m_UPHCfgs[i].iReClean);
		iniFile.SetValueI(keyname, "AiError", m_UPHCfgs[i].iAiError);
	}


	iniFile.WriteFile();
    return;
}

void HomeView::readDvToplimitCfg()
{
	IFileVariable *pFile = getQualificationCfgFileVariable();
	if (NULL != pFile)
	{
		for (int i = 0; i < DVTOPLIMITCFG_SIZE; i++)
		{
			QString strI = QString::number(i);
			m_stDvToplimitCfg[i].idvSmirchCnt = pFile->getValue(QUALIFICATION_TOPLIMITCFG_SMIRCHCNT + strI.toStdString(), 999).toInt();
			m_stDvToplimitCfg[i].idvHairCnt = pFile->getValue(QUALIFICATION_TOPLIMITCFG_HAIRCNT + strI.toStdString(), 999).toInt();
			m_stDvToplimitCfg[i].idvDirtpitsCnt = pFile->getValue(QUALIFICATION_TOPLIMITCFG_DIRTPITSCNT + strI.toStdString(), 999).toInt();
			m_stDvToplimitCfg[i].ddvSmirchArea = pFile->getValue(QUALIFICATION_TOPLIMITCFG_SMIRCHAREA + strI.toStdString(), 999.0).toDouble();
			m_stDvToplimitCfg[i].ddvHairArea = pFile->getValue(QUALIFICATION_TOPLIMITCFG_HAIRAREA + strI.toStdString(), 999.0).toDouble();
			m_stDvToplimitCfg[i].ddvDirtArea = pFile->getValue(QUALIFICATION_TOPLIMITCFG_DIRTAREA + strI.toStdString(), 999.0).toDouble();

			pFile->bindValue(QUALIFICATION_TOPLIMITCFG_SMIRCHCNT + strI.toStdString(), &m_stDvToplimitCfg[i].idvSmirchCnt);
			pFile->bindValue(QUALIFICATION_TOPLIMITCFG_HAIRCNT + strI.toStdString(), &m_stDvToplimitCfg[i].idvHairCnt);
			pFile->bindValue(QUALIFICATION_TOPLIMITCFG_DIRTPITSCNT + strI.toStdString(), &m_stDvToplimitCfg[i].idvDirtpitsCnt);
			pFile->bindValue(QUALIFICATION_TOPLIMITCFG_SMIRCHAREA + strI.toStdString(), &m_stDvToplimitCfg[i].ddvSmirchArea);
			pFile->bindValue(QUALIFICATION_TOPLIMITCFG_HAIRAREA + strI.toStdString(), &m_stDvToplimitCfg[i].ddvHairArea);
			pFile->bindValue(QUALIFICATION_TOPLIMITCFG_DIRTAREA + strI.toStdString(), &m_stDvToplimitCfg[i].ddvDirtArea);
		}
	}
}

void HomeView::initToolBtns()
{
	connect(ui->toolButton_Log, SIGNAL(clicked()), this, SLOT(on_ToolBtns_clicked()));
	connect(ui->toolButton_uph, SIGNAL(clicked()), this, SLOT(on_ToolBtns_clicked()));
	connect(ui->toolButton_level, SIGNAL(clicked()), this, SLOT(on_ToolBtns_clicked()));
    connect(ui->toolButton_heatmap, SIGNAL(clicked()), this, SLOT(on_ToolBtns_clicked()));
    connect(ui->toolButton_dvdefectcnt, SIGNAL(clicked()), this, SLOT(on_ToolBtns_clicked()));
	/*新ui修改*/
	connect(ui->toolButton_defecttableview, SIGNAL(clicked()), this, SLOT(on_ToolBtns_clicked()));
    connect(ui->toolButton_imagelayout, SIGNAL(clicked()), this, SLOT(on_ToolBtns_clicked()));
	ui->stackedWidget->setCurrentWidget(ui->page_defectTableView);
}

void HomeView::update_dect(int stationIndex)
{
	if (stationIndex >= diagramSceneVec.size())
    {
        return;
    }
	IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramPath(), SString::IntToStr(stationIndex));
	if (pFileVariable != NULL)
	{
		int iresolutionX = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONX, 8192).toInt();
		int iresolutionY = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONY, 12800).toInt();
		double oldW = 0;
		double oldH = 0;
//		diagramSceneVec.at(stationIndex)->getNewDefet(oldW, oldH);
//		if(oldW != iresolutionY || oldH != iresolutionY)
//		{
//			diagramSceneVec.at(stationIndex)->setNewDefet(iresolutionX, iresolutionY);
//		}
	}
}

void HomeView::updateSueqence(int stationIndex)
{
    if(stationIndex <0 || stationIndex >= getImageStationCnt())
        return ;
    CImageStation *pCImageStation  = CImageStationManager::GetInstance()->getImageStation(stationIndex);
    if(pCImageStation == NULL)
        return;
    int channelCnt = pCImageStation->getEnableChannelCnt();
	vector<QToolButton* > toolBtnVec = btnVecMap[stationIndex];
	if (toolBtnVec.size() == 0)
	{
		return;
	}
	if (channelCnt > toolBtnVec.size())
	{
		return;
	}
    for (int i = 0; i < toolBtnVec.size(); ++i)
	{
        toolBtnVec.at(i)->setVisible(i<channelCnt);
	}
}


void HomeView::on_toolButton_change_product_clicked()
{
	if (MAIN_WORK_WORK == WorkFlow::getInstance()->GetWorkState())
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("请停止工作后,进行产品切换!"));
		return;
	}

	INotifyService *pINotifyService = CommonUtils::getNotifyService();
	if (pINotifyService == NULL)
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("切换产品插件加载失败!"));
		ui->label_show_type->setText("Error!");
		return;
	}
	CommonUtils::getIUIPlugin("fileuiplugin");
	pINotifyService->send("fileuiplugin", "file_open", 0,0);

    updateProductDes();
}

void HomeView::on_tabWidget_defect_currentChanged(int index)
{
	if (graphWidgetVec.size() > 0 && index < graphWidgetVec.size())
	{
		m_currentView = graphWidgetVec.at(index);
	}
	return;
}


 void HomeView::UpdateProduct()
 {
    //ILineCameraFactoryManage *pCameraManager = (ILineCameraFactoryManage*)GetSDKInstance("cameramanager", 
    //                                                           ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE);
    //RETURN_IF(NULL == pCameraManager);
    //pCameraManager->ReInit();
    //INotifyService* pNotifyService = CommonUtils::getNotifyService();
    //pNotifyService->post("linecamerasettingui", "update_cam_ui", 0, 0);
	////切换产品时更新缺陷定义配置
	//CSendMessage::GetInstance()->sendChangeProduct();
    //WorkFlowFun::getInstance()->Reinit();
    ////重新初始化算法参数
    //CInvokeArithmetic::getInstance()->resetProcess();
    //CInvokeArithmetic::getInstance()->init();

     updateProductDes();
 }

 void HomeView::SaveProduct()
 {
     ILineCameraFactoryManage *pCameraManager = (ILineCameraFactoryManage*)GetSDKInstance("cameramanager",
         ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE);
     RETURN_IF(NULL == pCameraManager);
     pCameraManager->SaveCameraInParam();
 }

 void HomeView::slotsIniOtherUI()
 {
	 CommonUtils::getIUIPlugin("customsettingplugin");//
	 CommonUtils::getIUIPlugin("recipesuiplugin");//
 }
 void HomeView::slotsUpdateUI(gHistogram* pHistogram, int iSize,int ial)
 {
	 //ui->widget_DefectChart->updateUI(pHistogram, iSize, ial);//删除缺陷柱状图显示//
 }

 void HomeView::getNgDefectType(stProductSummary* presSummary)
 {
	 presSummary->iProductNgDefectType = 0;
	 if (presSummary->ResultFlag != res_Ng) //
	 {
		 return;
	 }
	 int iLevel = 7;
	 for (int i = 7; i > 0; i--)//D
	 {
		 // 根据缺陷类型 等级相同优先级别 崩边，凹凸点，水波纹，划伤
		 int iChips = presSummary->defectSingleLevelCntArr[en_Chips][iLevel];
		 int iDent = presSummary->defectSingleLevelCntArr[en_Dent][iLevel];
		 int iRipples = presSummary->defectSingleLevelCntArr[en_Ripples][iLevel];
		 int iImpress = presSummary->defectSingleLevelCntArr[en_Impress][iLevel];

		 int iScratches = presSummary->defectSingleLevelCntArr[en_Scratches][iLevel];
		 int iFalseScratches = presSummary->defectSingleLevelCntArr[en_FalseScratches][iLevel];
		 int iSmirch = presSummary->defectSingleLevelCntArr[en_Smirch][iLevel];
		 int iFalseSmirch = presSummary->defectSingleLevelCntArr[en_FalseSmirch][iLevel];

		 if (iChips > 0){
			 presSummary->iProductNgDefectType = en_Chips;
			 return;
		 }
		 else if (iDent > 0){
			 presSummary->iProductNgDefectType = en_Dent;
			 return;
		 }
		 else if (iRipples > 0){
			 presSummary->iProductNgDefectType = en_Ripples;
			 return;
		 }
		 else if (iImpress > 0){
			 presSummary->iProductNgDefectType = en_Impress;
			 return;
		 }
		 else if (iScratches > 0 || iFalseScratches > 0 || iSmirch > 0 || iFalseSmirch > 0) {
			 return;	
		 }
	 }

 }

 void HomeView::updateProductUPH()
 {
	 JTime curTime = JTime::CurrentLocalDateTime();
	 int iHour = curTime.hour();
	 IFileVariable *pFile = getCameraSetCfgFileVariable();
	 int iSartTime = 8;
	 if (pFile)
	 {
		 iSartTime = pFile->getValue(WORK_MODE_AM_STRAT_TIME).toInt(8);;//
	 }
	 
	 //分白夜班次
	 int iType = DAY_BY_WORK_INDEX;
	 if ((iSartTime > iHour) || (iHour >= (iSartTime + DATE_TIMES)))//小于 白班开始时间或大于白班交班时间
	 {
		 iType = NIGHT_BY_WORK_INDEX;
	 }
	 m_iDayClass = iType;
	 //记录 1 点到 24点数据
	 int iIndex = iHour + DAY_BY_WORK_TIMES; //确定填充对应24小時数据索引 (0.1.2） 已用// 如 1 点则记录在第3位

	 if (0 == iHour) //24点为最后一个数据
	 {
		 iIndex = DAY_BY_WORK_TIMES + DATE_TIMES * 2;
	 }

	 enResFlag rstFlag = m_resSummary.ResultFlag;
	 m_UPHCfgs[iType].iTotalCnt  += 1;
	 m_UPHCfgs[iIndex].iTotalCnt += 1;
	 if ( rstFlag == res_OK2) //  B
	 {
		 m_UPHCfgs[iType].iReCheck +=1;    
		 m_UPHCfgs[iIndex].iReCheck += 1; 
	 }
	 else if (rstFlag == res_OK)         // A
	 {
		 m_UPHCfgs[iType].iOKCnt +=1;
		 m_UPHCfgs[iIndex].iOKCnt += 1;
	 }
	 else if (rstFlag == res_Ng)         // D
	 {
		 m_UPHCfgs[iType].iNGCnt += 1;
		 m_UPHCfgs[iType].NgDefectType[m_resSummary.iProductNgDefectType] += 1;//NG类型
		 m_UPHCfgs[iIndex].iNGCnt += 1;
		 //NG type  add 2020/05/13
		 m_UPHCfgs[0].NgDefectType[m_resSummary.iProductNgDefectType] += 1;
		
	 }
	 else // C rstFlag == res_error || rstFlag == res_recheck || rstFlag == res_OK3 || res_unknow
	 {
		 m_UPHCfgs[iType].iReClean += 1;  //  c
		 m_UPHCfgs[iIndex].iReClean += 1;  //  c
		 if (rstFlag == res_recheck)
		 {
			 m_UPHCfgs[iType].iAiError++;
		 }
		 else
		 {
			 m_UPHCfgs[iType].RiDefectType[m_resSummary.iProductRiDefectType] ++;//Ri类型
		 }

	 }
	 //总数 
	 m_UPHCfgs[0].iTotalCnt = m_UPHCfgs[DAY_BY_WORK_INDEX].iTotalCnt + m_UPHCfgs[NIGHT_BY_WORK_INDEX].iTotalCnt;
	 m_UPHCfgs[0].iReCheck =  m_UPHCfgs[DAY_BY_WORK_INDEX].iReCheck +  m_UPHCfgs[NIGHT_BY_WORK_INDEX].iReCheck;
	 m_UPHCfgs[0].iOKCnt =    m_UPHCfgs[DAY_BY_WORK_INDEX].iOKCnt +    m_UPHCfgs[NIGHT_BY_WORK_INDEX].iOKCnt;
	 m_UPHCfgs[0].iNGCnt =    m_UPHCfgs[DAY_BY_WORK_INDEX].iNGCnt +    m_UPHCfgs[NIGHT_BY_WORK_INDEX].iNGCnt;
	 m_UPHCfgs[0].iReClean =  m_UPHCfgs[DAY_BY_WORK_INDEX].iReClean +  m_UPHCfgs[NIGHT_BY_WORK_INDEX].iReClean;
	 //

	 QString str = ui->comboBox_date_data->currentText();

	 if (str == m_strMarkCurday) //选择显示,不是当天的不更新
	 {
		 slotUpdateUPH();
	 }
     writeUPHFile();
	 //add 20200708 be
	QString strCurday = getCurrentLocalDateTimeStrPath();//现在时间
	
	if (strCurday != m_strMarkCurday && curTime.hour() >= iSartTime) //新的一天  数据 
	{
	 for (int i = 0; i < UPHCFG_SIZES; i++)
	 {
		 m_UPHCfgs[i].clear();
	 }
	 m_strMarkCurday = strCurday;
	 //增加 新的一天数据选 择
	 ui->comboBox_date_data->addItem(strCurday);
	 ui->comboBox_date_data->setCurrentIndex(ui->comboBox_date_data->count()-1);
	 //更新日期
	 ui->page_productUph->CurrentChanged(strCurday);
	 ui->page_yieldRate->CurrentChanged(strCurday);

	}
	 //add 20200708 end
 }

 void HomeView::updateDefectLevel()
 {
     slotUpdefectLevel(m_resSummary.dvdefectSummaryArr);
	 QString str = ui->comboBox_date_data->currentText();
	 if (str == m_strMarkCurday)
	 {
		 int iOK = m_UPHCfgs[m_iChooseDay].iOKCnt + m_UPHCfgs[m_iChooseDay].iReCheck;  
		 int iArror = m_UPHCfgs[m_iChooseDay].iAiError;
		 ui->page_NgTypeChart->updateUI((int*)m_UPHCfgs[m_iChooseDay].NgDefectType, (int*)m_UPHCfgs[m_iChooseDay].RiDefectType,true, iOK, iArror);
	 }
 }



 void HomeView::updateDVDefectCnt()
 {
	 slotUpdateDVDefectCnt();
 }

 void HomeView::UpdateFacilityStatus()
 {

 }

 void HomeView::on_toolButton_reCheck_clicked()
 {
	 if (QMessageBox::No == QMessageBox::warning(NULL, "warn", QString::fromLocal8Bit("请确认是否重测？"), QMessageBox::Yes | QMessageBox::No))
		 return;
	 //if (NULL == pCurrentProduct)
	 //{
		// //error 
		// return;
	 //}

	 //if (!CProductUnion::getInstance()->isProductExist(pCurrentProduct))
	 //{
		// return;
	 //}

	 if (0 != CInvokeArithmetic::getInstance()->reProcessSpecialProduct(m_CurrentProductSn))
	 {
		 //error 
	 }
	 ;
 }

 void HomeView::on_pushButton_defect_day_clicked()
 {
	 //ui->widget_defectlevel->updateUI((int*)m_UPHCfgs[0].NgDefectType);
 }
 void HomeView::on_pushButton_defect_pices_clicked()
 {
	 // 当前一片的数据//
	 ui->widget_defectlevel->updateUI(m_resSummary.defectSummaryArr,false);//test m_UPHCfgs[2]
 }
 void HomeView::on_toolButton_prodcuctInfo_clicked()
 {
	 //QString strdate = getCurrentLocalDateTimeStrPath();
	 QString str = ui->comboBox_date_data->currentText();
	 bool res = (str == m_strMarkCurday) ? true : false;
	 slotUpdateUPH(res);
     ui->stackedWidget_productInfo->setCurrentWidget(ui->page_productInfo);
 }
 void HomeView::on_toolButton_UPH_clicked()
 {
	
	 //UphStatistics::GetInstance(this)->show();
     //改为stackedWidget切换模式
     ui->page_productUph->updateUi();
     ui->stackedWidget_productInfo->setCurrentWidget(ui->page_productUph);
 }
 void HomeView::on_toolButton_yield_clicked()
 {
	 //FormLineChart::GetInstance(this)->updateUi();
	 //FormLineChart::GetInstance(this)->show();
     //改为stackedWidget切换模式
     ui->page_yieldRate->updateUi();
     ui->stackedWidget_productInfo->setCurrentWidget(ui->page_yieldRate);
 }
 void HomeView::on_toolButton_ngchart_clicked()
 {

	 QString str = ui->comboBox_date_data->currentText();
	 updateDayOrNightNgChart(str == m_strMarkCurday);
	 ui->stackedWidget_productInfo->setCurrentWidget(ui->page_NgTypeChart);
 }
 void HomeView::on_toolButton_heatmap_2_clicked()
 {
	 QString str = ui->comboBox_date_data->currentText();
	 ui->page_heatmap_2->updateUI(str, m_iChooseDay);
	 ui->stackedWidget_productInfo->setCurrentWidget(ui->page_heatmap_2);
 }
 void HomeView::slotCamera_show(QString strName,int iSence)
 {
     int iSenc = iSence;
     int iChannel = 0;
    vector<string> vec ;
    string strSceneName = SString::IntToStr(iSence);
    getEnabelChannle(vec,strSceneName);
	string str = "";
	str = strSceneName;
    for(int i = 0;i <vec.size();i++)
    {
        if(vec.at(i) + str == strName.toStdString())
        {
            iChannel = i;
            break;
        }
    }
    UpdateDispImage(iChannel, iSenc);
 }

 void HomeView::slotUpdateSelectPos(const QRectF pos, int iSence)
 {
    if(iSence >=0 && iSence < graphWidgetVec.size())
    {
        GraphWidget *pGraphWidget = graphWidgetVec.at(iSence);
        if(pGraphWidget != NULL)
        {
            pGraphWidget->slotUpdateSelectPos(pos);
        }
    }
 }

 void HomeView::on_toolButton_change_class_clicked()
 {
	 return;//当前先自动转班
	 /**
	 IFileVariable *pFile = getCameraSetCfgFileVariable();
	 int iSartTime = 8;
	 if (pFile)
	 {
		 iSartTime = pFile->getValue(WORK_MODE_AM_STRAT_TIME).toInt(8);//
	 }
	 JTime curTime = JTime::CurrentLocalDateTime();

	 QString strTime = getCurrentLocalDateTimeStrPath();

	 QString msg = QString::fromLocal8Bit("请确认是否转班？");
	 if (QMessageBox::No == QMessageBox::warning(NULL, QString::fromLocal8Bit("转班"), msg, QMessageBox::Yes | QMessageBox::No))
		 return;

	 if (strTime == m_strMarkCurday)//同一天
	 {
		 if (0 == m_iDayClass)//白班
		 {
			 if (curTime.hour() < iSartTime + 10) //两班倒的情况下 提前2小时转夜班 报错 
			 {
				 QMessageBox::warning(this, QString::fromLocal8Bit("失败"), strTime+QString::fromLocal8Bit("两班倒的情况下 提前超2小时转夜班报错"), QMessageBox::Ok);
				 return;
			 }
			 else {  //转夜班
				 m_iDayClass = 1;
				 //
			 }

		 }
		 else {
			 QMessageBox::warning(this, QString::fromLocal8Bit("失败"), strTime+QString::fromLocal8Bit("已经转到夜班！不能再次转班！"), QMessageBox::Ok);
			 return;
		 }
	 }
	 else {  //时间已更新//说明是新一天。

		 if (0 == m_iDayClass)//白班--
		 {
			 if (curTime.hour() < iSartTime) //- 若是没转夜班，也没生产。直接到第二天凌晨后才转班生产//但不到白天时间 
			 {
				 //转前一天的夜班
				 m_iDayClass = 1;


				 //若N天后的凌晨？

			 }
			 else if (curTime.hour() > (iSartTime+ DATE_TIMES)) ////- 若是没转夜班，也没生产。直接到第N天下午夜班时间开始 N大于等于二
			 {
				 //
				 //转夜第N天夜班
				 m_iDayClass = 1;
			 }
			 else {
				 //转夜第N天白班
				 m_iDayClass = 0;
			 }
			
			 
		 }
		 else {  //夜班




		 }

	 }
	 */
 }

 void HomeView::on_ToolBtns_clicked()
 {
	 QToolButton * pBtn = qobject_cast<QToolButton*>(sender());
	 if (pBtn == NULL)
	 {
		 return ;
	 }
	 QWidget* pWidget = NULL;
	 if (ui->toolButton_Log == pBtn)
	 {
		 pWidget = ui->page_log;
	 }
	 else if (ui->toolButton_uph == pBtn)
	 {
		 pWidget = ui->page_uph;
		 slotUpdateUPH();
	 }
	 else if (ui->toolButton_level == pBtn)
	 {
		 pWidget = ui->page_defectlevel;
		 on_pushButton_defect_pices_clicked();
		 //slotUpdefectLevel(m_resSummary.dvdefectSummaryArr);
	 }
	 else if (ui->toolButton_heatmap == pBtn)
	 {
		 pWidget = ui->page_heatmap;
		 slotUpdefectHeatMap(0,0);
     }
     else if (ui->toolButton_dvdefectcnt == pBtn)
     {
		 pWidget = ui->page_dvdefecttype;
         slotUpdateDVDefectCnt();
     }
	 /*新ui修改*/
	else if (ui->toolButton_defecttableview == pBtn)
	{
		pWidget = ui->page_defectTableView;
		//slotUpdateDVDefectCnt();
	}
    else if (ui->toolButton_imagelayout == pBtn)
    {
		pWidget = ui->page_imageview;
    }

	ui->stackedWidget->setCurrentWidget(pWidget);
		 
 }

 void HomeView::on_radioButton_firstcheck_clicked()
 {
	 m_bFristCheckSpec = true;
 }

 void HomeView::on_radioButton_secondcheck_clicked()
 {
	 m_bFristCheckSpec = false;
 }

/*新ui修改*/
void HomeView::on_buttonGroup_mainFlow_buttonClicked(QAbstractButton * button)
{
	if (ui->toolBar_on == button)
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
	else if(ui->toolBar_pause == button)
	{
		SignalCheck::SetSignal(SYS_PAUSE);
		CInvokeArithmetic::getInstance()->setStopAotoTest();//停止自动测试
	}
	else if (ui->toolBar_stop == button)
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("收到界面控件 stop "));
		SignalCheck::SetSignal(SYS_STOP);
		CInvokeArithmetic::getInstance()->setStopAotoTest();//停止自动测试
	}
	else if (ui->toolBar_reset == button)
	{
		SignalCheck::SetSignal(SYS_RESET);
		CInvokeArithmetic::getInstance()->setStopAotoTest();//停止自动测试
	}
}

void HomeView::on_pushButton_page_cnt_clicked()
{
	ui->stackedWidget_dvdefecttype->setCurrentIndex(0);
	ui->pushButton_page_area->setChecked(false);
}

void HomeView::on_pushButton_page_area_clicked()
{
	ui->stackedWidget_dvdefecttype->setCurrentIndex(1);
	ui->pushButton_page_cnt->setChecked(false);
}

/*新ui修改*/
void HomeView::SlotDayMode()
{
    //HomeView
    QString strStyleHomeView = "QWidget#widget_main{background-color: rgb(225, 227, 234);}";
    setStyleSheet(strStyleHomeView);

    /*****************************左侧************************************************************************************/
    //状态机样式表
	QString strStyleMainflow = "QToolButton#toolBar_on{border-image: url(:/image/general/start.png);background-color: rgbas(0,0,0,0);}\
    QToolButton#toolBar_pause{border-image: url(:/image/general/pause.png);background-color: rgbas(0,0,0,0);}\
    QToolButton#toolBar_stop{border-image: url(:/image/general/stop.png);background-color: rgbas(0,0,0,0);}\
    QToolButton#toolBar_reset{border-image: url(:/image/general/reset.png);background-color: rgbas(0,0,0,0);}\
    QToolButton:checked#toolBar_on{border-image: url(:/image/general/start_ing.png);background-color: rgbas(0,0,0,0);}\
    QToolButton:checked#toolBar_pause{border-image: url(:/image/general/pause_ing.png);background-color: rgbas(0,0,0,0);}\
    QToolButton:checked#toolBar_stop{border-image: url(:/image/general/stop_ing.png);background-color: rgbas(0,0,0,0);}\
    QToolButton:checked#toolBar_reset{border-image: url(:/image/general/reset_ing.png);background-color: rgbas(0,0,0,0);}";

    //生产信息
    QString strStyleProduct = "QFrame#frame_productInfo{background-color: rgba(255, 255, 255, 0);}\
                               QLabel#label_PreProductTitle{background-color: rgb(34, 163, 169);color: rgb(214, 197, 15);}\
                               QLabel#label_ProductTitle{background-color: rgb(34, 163, 169);color: rgba(255,255,255);}\
                               QLabel#label_show_type{color: rgb(26, 188, 155);background-color: rgb(255, 255, 255);}\
                               QWidget#widget_productOperator{background-color: rgb(255, 255, 255);}\
                               QToolButton#toolButton_change_product\
                               ,QToolButton#toolButton_stopBeeAlarm\
                               ,QToolButton#toolButton_reCheck{border-image: url(:/image/day/pushbutton1.png);color: rgb(255, 255, 255);}\
		                      QToolButton:!enabled#toolButton_change_product\
                               ,QToolButton:!enabled#toolButton_stopBeeAlarm\
		                      ,QToolButton:!enabled#toolButton_change_class\
		                      ,QToolButton:!enabled#toolButton_reCheck{ color: rgb(190,190,190); }\
                               QLabel#label_checkStandar{color: rgb(102, 102, 102);}\
                               QRadioButton#radioButton_firstcheck{color: rgb(102, 102, 102);}\
                               QRadioButton#radioButton_secondcheck{color: rgb(102, 102, 102);}";
    QString strStyleProduct1 = "QFrame#frame_deviceInfo{background-color: rgb(255, 255, 255);}\
                               QFrame#frame_curNumer\
                               ,QFrame#frame_curphoto\
                               ,QFrame#frame_procssimage\
                               ,QFrame#frame_pohtotime\
                               ,QFrame#frame_processtime\
                               ,QFrame#frame_ct{border-image: url(:/image/general/rect1.png);}\
                               QLabel{color: rgb(102, 102, 102);}\
                               QLabel#label_curNumer{border-image: url(:/image/day/curnumbers.png);}\
                               QLabel#label_curphoto{border-image: url(:/image/day/curphoto.png);}\
                               QLabel#label_procssimage{border-image: url(:/image/day/procssimage.png);}\
                               QLabel#label_pohtotime{border-image: url(:/image/day/pohtotime.png);}\
                               QLabel#label_processtime{border-image: url(:/image/day/processtime.png);}\
                               QLabel#label_ct{border-image: url(:/image/day/ct.png);}\
                               QLabel#label_cardTitle\
                               ,QLabel#label_L1Title\
                               ,QLabel#label_L2Title\
                               ,QLabel#label_Cam1Title\
                               ,QLabel#label_Cam2Title{color: rgb(102, 102, 102);}";
    ui->widget_left->setStyleSheet(strStyleMainflow + strStyleProduct + strStyleProduct1);

    /*****************************中间************************************************************************************/
    //运行状态
	QString strStyleRunStatus = "QLabel#label_PreRunStatus{color: rgb(214, 197, 15);}\
                                QLabel#lable_system_statu{color: rgba(255,255,255);}\
								QLabel#lable_system_mode{color: rgba(255,255,255);}\
								QLabel#label_h_status{color: rgba(255,255,255);}\
								QLabel#label_timeout_status{color: rgba(255,255,255);}";
    //图像显示
    QString strStyleImage = "QWidget#widget_imageInfo{background-color: rgb(255, 255, 255);}\
                             QToolButton{background-color:rgb(225, 227, 234);border-radius:4px;font: 9pt \"微软雅黑\";color: rgb(102, 102, 102);}\
                             QToolButton:checked{background-color:rgb(34, 163, 169);border-radius:4px;font: 9pt \"微软雅黑\";color: rgb(255, 255, 255);}\
                             QLabel#label_curPos{border-radius:4px;color: rgb(255, 255, 255);background-color:rgb(33, 186, 195);}";
	ui->widget_mid->setStyleSheet(strStyleRunStatus + strStyleImage);


    /*****************************右侧************************************************************************************/
    QString strStyleRight = "QWidget#widget_right{background-color: rgba(255, 255, 255, 0);}\
        QWidget#widget_logToolBar{background-color: rgb(255, 255, 255);}\
        QToolButton#toolButton_Log{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/log.png);qproperty-iconSize:46px 46px;}\
        QToolButton#toolButton_uph{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/shuji.png);qproperty-iconSize:46px 46px;}\
        QToolButton#toolButton_level{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/hong.png);qproperty-iconSize:46px 46px;}\
        QToolButton#toolButton_heatmap{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/huang.png);qproperty-iconSize:46px 46px;}\
        QToolButton#toolButton_dvdefectcnt{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/day/sn.png);qproperty-iconSize:46px 46px;}\
		QToolButton#toolButton_defecttableview{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/defecttable.png);qproperty-iconSize:46px 46px;}\
		QToolButton#toolButton_imagelayout{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/previewimage.png);qproperty-iconSize:46px 46px;}\
        QStackedWidget#stackedWidget{background-color: rgb(255, 255, 255);}\
        QPlainTextEdit#textEdit_runLog{color: rgb(102, 102, 102);background-color: rgba(255, 255, 255, 0);}\
        QToolButton#toolButton_clear_log\
        ,QToolButton#toolButton_clear_data\
        ,QToolButton#pushButton_defect_day\
        ,QToolButton#pushButton_defect_pices\
        , QToolButton#toolButton_prodcuctInfo\
        , QToolButton#toolButton_UPH\
        , QToolButton#toolButton_yield \
        , QToolButton#toolButton_ngchart \
        , QToolButton#toolButton_heatmap_2 \
        ,QToolButton#pushButton_day\
        ,QToolButton#pushButton_night\
        , QToolButton#pushButton_page_cnt\
        , QToolButton#pushButton_heatmap\
        , QToolButton#pushButton_regioncnt\
        , QToolButton#pushButton_page_area{border-image: url(:/image/day/pushbutton2.png);background-color: rgba(255, 255, 255, 0);color: rgb(255, 255, 255);}\
        QToolButton:checked#pushButton_day\
        ,QToolButton:checked#pushButton_night\
        ,QToolButton:checked#toolButton_prodcuctInfo\
        ,QToolButton:checked#toolButton_UPH\
        ,QToolButton:checked#toolButton_yield\
        , QToolButton:checked#toolButton_ngchart \
        , QToolButton:checked#toolButton_heatmap_2 \
        ,QToolButton:checked#pushButton_page_cnt\
		,QToolButton:checked#pushButton_heatmap\
		,QToolButton:checked#pushButton_regioncnt\
        ,QToolButton:checked#pushButton_page_area{color:rgb(255, 255, 127);}\
        QLabel#label_table_show_res{color: rgb(255, 255, 255);border-radius:6px;}\
        QLabel#label_show_ok{color: rgb(255, 255, 255);border-radius:4px;}\
        QLabel#label_show_ok_sn{background-color: rgb(71, 164, 233);color: rgb(255, 255, 255);border-radius:4px;}";
    ui->widget_right->setStyleSheet(strStyleRight);

    //时间下拉框
    QString strStyleTimeComBox = "QComboBox\
    {\
        height: 50px;\
        background: rgb(20,162,182,0);\
        color: rgba(255, 255, 255);\
        border-image: url(:/image/day/pushbutton2.png);\
    }\
    QComboBox:hover\
    {\
        background:rgb(20,162,182,0);\
    }\
    QComboBox::drop-down\
    {\
        width: 58px;\
        border:none;\
    }\
    QComboBox::down-arrow\
    {\
        image: url(:/images/arrow_retract_icon.png);\
    }\
    QComboBox::down-arrow:on\
    {\
        image: url(:/images/arrow_open_icon.png);\
    }\
    QComboBox QAbstractItemView\
    {\
        border: none;\
        outline:none;\
        background-color:rgb(20,162,182);\
        selection-background - color:rgb(20,162,182);\
    }";
    ui->comboBox_date_data->setStyleSheet(strStyleTimeComBox);
	ui->comboBox_moduls->setStyleSheet(strStyleTimeComBox);

	ui->page_cnt->updateTextColor(QColor(102, 102, 102));
	ui->page_area->updateTextColor(QColor(102, 102, 102));
	ui->widget_uph->updateTextColor(QColor(102, 102, 102));
	ui->widget_defectTableView->updateTextColor(QColor(102,102,102));
}

/*新ui修改*/
void HomeView::SlotNightMode()
{
    //HomeView
    QString strStyleHomeView = "QWidget#widget_main{background-color: rgb(12, 15, 46);}";
    setStyleSheet(strStyleHomeView);
    /*****************************左侧*********************************************************************/
    //状态机样式表
    QString strStyleMainflow = "QToolButton#toolBar_on{border-image: url(:/image/general/start.png);background-color: rgbas(0,0,0,0);}\
                               QToolButton#toolBar_pause{border-image: url(:/image/general/pause.png);background-color: rgbas(0,0,0,0);}\
                               QToolButton#toolBar_stop{border-image: url(:/image/general/stop.png);background-color: rgbas(0,0,0,0);}\
                               QToolButton#toolBar_reset{border-image: url(:/image/general/reset.png);background-color: rgbas(0,0,0,0);}\
                               QToolButton:checked#toolBar_on{border-image: url(:/image/general/start_ing.png);background-color: rgbas(0,0,0,0);}\
                               QToolButton:checked#toolBar_pause{border-image: url(:/image/general/pause_ing.png);background-color: rgbas(0,0,0,0);}\
                               QToolButton:checked#toolBar_stop{border-image: url(:/image/general/stop_ing.png);background-color: rgbas(0,0,0,0);}\
                               QToolButton:checked#toolBar_reset{border-image: url(:/image/general/reset_ing.png);background-color: rgbas(0,0,0,0);}";
    //生产信息
    QString strStyleProduct = "QFrame#frame_productInfo{background-color: rgba(255, 255, 255, 0);}\
                              QLabel#label_PreProductTitle{background-color: rgb(30, 159, 210);color: rgb(214, 197, 15);}\
                              QLabel#label_ProductTitle{background-color: rgb(30, 159, 210);color: rgba(255,255,255);}\
                              QLabel#label_show_type{background-color: rgb(0, 125, 204);color: rgb(255, 255, 255);}\
                              QWidget#widget_productOperator{background-color: rgb(14, 71, 147);}\
                              QToolButton#toolButton_change_product \
                              ,QToolButton#toolButton_change_class\
                              ,QToolButton#toolButton_stopBeeAlarm \
                              ,QToolButton#toolButton_reCheck{border-image: url(:/image/night/pushbutton1.png);color: rgb(222, 222, 222);}\
                              QToolButton:!enabled#toolButton_change_product \
                              QToolButton:!enabled#toolButton_stopBeeAlarm \
		                      ,QToolButton:!enabled#toolButton_change_class\
		                      ,QToolButton:!enabled#toolButton_reCheck{ color: rgb(140,140,140); }\
                              QLabel#label_checkStandar{color: rgb(222, 222, 222);}\
                              QRadioButton#radioButton_firstcheck{color: rgb(222, 222, 222);}\
                              QRadioButton#radioButton_secondcheck{color: rgb(222, 222, 222);}";
    QString strStyleProduct1 = "QFrame#frame_deviceInfo{background-color: rgb(14, 71, 147);}\
                               QFrame#frame_curNumer\
                               ,QFrame#frame_curphoto\
                               ,QFrame#frame_procssimage\
                               ,QFrame#frame_pohtotime\
                               ,QFrame#frame_processtime\
                               ,QFrame#frame_ct{border-image: url(:/image/general/rect1.png);}\
                               QLabel{color: rgb(102, 102, 102);}\
                               QLabel#label_curNumer{border-image: url(:/image/night/curnumbers.png);}\
                               QLabel#label_curphoto{border-image: url(:/image/night/curphoto.png);}\
                               QLabel#label_procssimage{border-image: url(:/image/night/procssimage.png);}\
                               QLabel#label_pohtotime{border-image: url(:/image/night/pohtotime.png);}\
                               QLabel#label_processtime{border-image: url(:/image/night/processtime.png);}\
                               QLabel#label_ct{border-image: url(:/image/night/ct.png);}\
                               QLabel#label_cardTitle\
                               ,QLabel#label_L1Title\
                               ,QLabel#label_L2Title\
                               ,QLabel#label_Cam1Title\
                               ,QLabel#label_Cam2Title{color: rgb(255, 255, 255);}";
    ui->widget_left->setStyleSheet(strStyleMainflow + strStyleProduct + strStyleProduct1);

    /*****************************中间************************************************************************************/
    //运行状态
    QString strStyleRunStatus = "QLabel#label_PreRunStatus{color: rgb(214, 197, 15);}\
                                QLabel#lable_system_statu{color: rgba(255,255,255);}\
								QLabel#lable_system_mode{color: rgba(255,255,255);}\
								QLabel#label_h_status{color: rgba(255,255,255);}\
								QLabel#label_timeout_status{color: rgba(255,255,255);}";
    //图像显示
    QString strStyleImage = "QWidget#widget_imageInfo{background-color: rgb(255, 255, 255);}\
                            QToolButton{background-color:rgb(225, 227, 234);border-radius:4px;font: 9pt \"微软雅黑\";color: rgb(102, 102, 102);}\
                            QToolButton:checked{background-color:rgb(34, 163, 169);border-radius:4px;font: 9pt \"微软雅黑\";color: rgb(255, 255, 255);}\
                            QLabel#label_curPos{border-radius:4px;color: rgb(255, 255, 255);background-color:rgb(24, 126, 167);}";
    ui->widget_mid->setStyleSheet(strStyleRunStatus + strStyleImage);
	//m_view->setBackgroundBrush(QBrush(QColor(0, 0, 0)));
	//m_sceneSecond->setBackgroundBrush(QBrush(QColor(0, 0, 0)));
	/*****************************右侧************************************************************************************/
    QString strStyleRight = "QWidget#widget_right{background-color: rgba(255, 255, 255, 0);}\
                            QWidget#widget_logToolBar{background-color: rgb(0, 125, 204);}\
                            QToolButton#toolButton_Log{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/log.png);qproperty-iconSize:46px 46px;}\
                            QToolButton#toolButton_uph{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/shuji.png);qproperty-iconSize:46px 46px;}\
                            QToolButton#toolButton_level{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/hong.png);qproperty-iconSize:46px 46px;}\
                            QToolButton#toolButton_heatmap{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/huang.png);qproperty-iconSize:46px 46px;}\
						    QToolButton#toolButton_dvdefectcnt{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/night/sn.png);qproperty-iconSize:46px 46px;}\
						    QToolButton#toolButton_defecttableview{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/defecttable.png);qproperty-iconSize:46px 46px;}\
						    QToolButton#toolButton_imagelayout{background-color: rgba(255, 255, 255, 0);qproperty-icon:url(:/image/general/previewimage.png);qproperty-iconSize:46px 46px;}\
                            QStackedWidget#stackedWidget{background-color: rgb(14, 71, 147);}\
                            QPlainTextEdit#textEdit_runLog{color: rgb(222, 222, 222);background-color: rgba(255, 255, 255, 0);}\
                            QToolButton#toolButton_clear_log\
                            ,QToolButton#toolButton_clear_data\
                            ,QToolButton#pushButton_level\
                            ,QToolButton#pushButton_Test\
                            , QToolButton#pushButton_day\
                            , QToolButton#pushButton_night\
                            , QToolButton#toolButton_prodcuctInfo\
                            , QToolButton#toolButton_UPH\
                            , QToolButton#toolButton_yield \
                            , QToolButton#toolButton_ngchart \
                            , QToolButton#toolButton_heatmap_2 \
                            ,QToolButton#pushButton_page_cnt\
							,QToolButton#pushButton_heatmap\
							,QToolButton#pushButton_regioncnt\
                            ,QToolButton#pushButton_page_area{border-image: url(:/image/night/pushbutton2.png);background-color: rgba(255, 255, 255, 0);color: rgb(222, 222, 222);}\
                            QToolButton:checked#pushButton_day\
                            ,QToolButton:checked#pushButton_night\
                            ,QToolButton:checked#toolButton_prodcuctInfo\
                            ,QToolButton:checked#toolButton_UPH\
                            ,QToolButton:checked#toolButton_yield\
                            , QToolButton:checked#toolButton_ngchart \
                            , QToolButton:checked#toolButton_heatmap_2 \
                            ,QToolButton:checked#pushButton_page_cnt\
							,QToolButton:checked#pushButton_heatmap\
							,QToolButton:checked#pushButton_regioncnt\
                            ,QToolButton:checked#pushButton_page_area{color:rgb(255, 255, 127);}\
                            QLabel#label_table_show_res{border-radius:6px;}\
                            QLabel#label_show_ok{background-color: rgb(34, 163, 169);color: rgb(222, 222, 222);border-radius:4px;}\
                            QLabel#label_show_ok_sn{background-color: rgb(0, 125, 204);color: rgb(222, 222, 222);border-radius:4px;}";
    ui->widget_right->setStyleSheet(strStyleRight);

    //时间下拉框
    QString strStyleTimeComBox = "QComboBox\
    {\
        height: 50px;\
        background: rgb(20,162,182,0);\
        color: rgba(222, 222, 222);\
        border-image: url(:/image/night/pushbutton2.png);\
    }\
    QComboBox:hover\
    {\
        background:rgb(20,162,182,0);\
    }\
    QComboBox::drop-down\
    {\
        width: 58px;\
        border:none;\
    }\
    QComboBox::down-arrow\
    {\
        image: url(:/images/arrow_retract_icon.png);\
    }\
    QComboBox::down-arrow:on\
    {\
        image: url(:/images/arrow_open_icon.png);\
    }\
    QComboBox QAbstractItemView\
    {\
        border: none;\
        outline:none;\
        background-color:rgb(20,162,182);\
        selection-background - color:rgb(20,162,182);\
    }";
    ui->comboBox_date_data->setStyleSheet(strStyleTimeComBox);

	ui->page_cnt->updateTextColor(QColor(222, 222, 222));
	ui->page_area->updateTextColor(QColor(222, 222, 222));
	ui->widget_uph->updateTextColor(QColor(222, 222, 222));
	ui->widget_defectTableView->updateTextColor(QColor(222, 222, 222));
}
void HomeView::on_comboBox_date_data_currentIndexChanged(const QString &arg1)
{
	QString str = arg1;

	readUPHFile(str);
	slotUpdateUPH(str == m_strMarkCurday);
	
	updateDayOrNightNgChart(str == m_strMarkCurday);
	
	ui->page_yieldRate->CurrentChanged(str);
	
	ui->page_productUph->CurrentChanged(str);
	
	ui->page_heatmap_2->updateUI(str, m_iChooseDay);
}
void HomeView::on_pushButton_day_clicked()
{
	ui->page_productUph->dayOrnightClicked();
	ui->page_yieldRate->dayOrnightClicked();
	ui->widget_uph->dayOrnightClicked();
	//QString strdate = getCurrentLocalDateTimeStrPath();
	QString str = ui->comboBox_date_data->currentText();
	bool res = (str == m_strMarkCurday) ? true : false;
	slotUpdateUPH(res);
	m_iChooseDay = 1;
	updateDayOrNightNgChart(str == m_strMarkCurday);
	ui->page_heatmap_2->updateUI(str, m_iChooseDay);
}

void HomeView::on_pushButton_heatmap_clicked()
{
	ui->stackedWidget_heatmap->setCurrentIndex(0);
	ui->pushButton_regioncnt->setChecked(false);
}

void HomeView::on_pushButton_regioncnt_clicked()
{
    ui->stackedWidget_heatmap->setCurrentIndex(1);
    ui->pushButton_heatmap->setChecked(false);
}

void HomeView::on_pushButton_shang_start_clicked()
{
     bool bOn = SignalCheck::isShangRunning();
	 if (bOn)
	 {
		 SignalCheck::setShangStop();
		 return;
	 }
	 int iStatus = MAIN_WORK_WORK;
	 int iRet = WorkFlowFun::getInstance()->queryBackPcStatus(iStatus);
	 if (iRet == 0)	//另一台机处于工作状态才允许启动上料机
	 {
		 if (iStatus == MAIN_WORK_WORK)
		 {
			 CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("另一条线已启动，启动上料机"), UILOG_WARN);
			 SignalCheck::setShangOn();
		 }
		 else
		 {
			 CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("另一条线启动才能启动上料机"), UILOG_ERR);
		 }
	 }
}

void HomeView::on_pushButton_clean_start_clicked()
{
    if (SignalCheck::isCleanRunning())
    {
        //SignalCheck::setCleanOn(false);
        SignalCheck::setCleanPause(true);
    }
    else
    {
        SignalCheck::setCleanOn(true);
        SignalCheck::setCleanPause(false);
        Sleep(1800);
        if (!SignalCheck::isCleanRunning())
        {
            QMessageBox::warning(NULL, "warning", QString::fromLocal8Bit("清洗机启动失败，请确认清洗机电源是否开启或另一条线是否启动清洗机！"));
            return;
        }
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("启动清洗机成功！"));
    }
    return;
}

void HomeView::on_pushButton_xia_start_clicked()
{
    
	SignalCheck::setXiaStop();
    
}

void HomeView::on_pushButton_clearmode_clicked()
{
	if (MAIN_WORK_WORK == WorkFlow::getInstance()->GetWorkState())
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("工作中，禁止启动清料!......."));
		return;
	}

	if (ui->pushButton_clearmode->text().toUtf8() == QString::fromLocal8Bit("开始清料"))
	{
		
		if (!SignalCheck::readIo(DI_XIA_RUN))
		{
			QMessageBox::warning(NULL, "Warning", QString::fromLocal8Bit("下料机未启动,不能使用清料模式！"));

			return;
		}
		ui->pushButton_clearmode->setText(QString::fromLocal8Bit("结束清料"));
		CBackPCtoFront::getInstance()->startClearMode();
		CBackPCtoFront::getInstance()->setSysToFront(SignalType::Stop);  //清料开启时需要停止前端
	}
	else
	{
		ui->pushButton_clearmode->setText(QString::fromLocal8Bit("开始清料"));
		CBackPCtoFront::getInstance()->stopClearMode();
	}
	

	//CBackPCtoFront::getInstance()->startClearMode();
	//WorkFlowFun::getInstance()->Crlmove(1);
	
}
void HomeView::on_channelBtn_clicked()
{
	QToolButton* pBtn = qobject_cast<QToolButton*>(sender());
    if(pBtn == NULL)
        return ;
    int iStationIndex = -1;
    int iChannelIndex = -1;
	int j = 0;
	map<int, vector<QToolButton* > >::iterator it = btnVecMap.begin();
	while (it != btnVecMap.end())
	{
		for (int i = 0; i < it->second.size(); ++i)
		{
			QToolButton* ptmp = it->second.at(i);
			if (ptmp == pBtn)
			{
				iStationIndex = j;
				iChannelIndex = i;
                //ptmp->setCheckable(true);
				ptmp->setChecked(true);
				
			}
			else
			{
                //ptmp->setCheckable(false);
				ptmp->setChecked(false);
			}		
		}
		it++;
		j++;
	}
	if (iStationIndex == -1 || iChannelIndex == -1)
	{
		return;
	}
    UpdateDispImage(iChannelIndex,iStationIndex);
    m_iImageChoose[iStationIndex] = iChannelIndex;
    }

    void HomeView::on_toolButton_stopBeeAlarm_clicked()
    {
		SignalCheck::writeIo(DO_ELEC_LOCK, true);
    }

void HomeView::on_pushButton_night_clicked()
{
	ui->page_productUph->dayOrnightClicked(false);
	ui->page_yieldRate->dayOrnightClicked(false);
	ui->widget_uph->dayOrnightClicked(false);
	//QString strdate = getCurrentLocalDateTimeStrPath();
	QString str = ui->comboBox_date_data->currentText();
	slotUpdateUPH(str == m_strMarkCurday);
	m_iChooseDay = 2;
	updateDayOrNightNgChart(str == m_strMarkCurday);
	ui->page_heatmap_2->updateUI(str, m_iChooseDay);
}

void  HomeView::updateDayOrNightNgChart(bool bToday)
{
	if (bToday)
	{
		int iOK = m_UPHCfgs[m_iChooseDay].iOKCnt + m_UPHCfgs[m_iChooseDay].iReCheck;
		int iArror = m_UPHCfgs[m_iChooseDay].iAiError;
		ui->page_NgTypeChart->updateUI((int*)m_UPHCfgs[m_iChooseDay].NgDefectType, (int*)m_UPHCfgs[m_iChooseDay].RiDefectType, true, iOK, iArror);
	}
	else {
		int iOK = m_UPHCfgsHistory[m_iChooseDay].iOKCnt + m_UPHCfgsHistory[m_iChooseDay].iReCheck;
		int iArror = m_UPHCfgsHistory[m_iChooseDay].iAiError;
		ui->page_NgTypeChart->updateUI((int*)m_UPHCfgsHistory[m_iChooseDay].NgDefectType, (int*)m_UPHCfgsHistory[m_iChooseDay].RiDefectType, true, iOK, iArror);
	}
}


