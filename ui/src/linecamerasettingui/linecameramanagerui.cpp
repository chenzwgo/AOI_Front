#include "linecameramanagerui.h"
#include "ui_linecameramanagerui.h"
#include "customitemdelegate.h"
#include "uicommon/change.h"
#include "rtdefine.h"
#include "iuiplugin.h"
#include "uicommon/commonutils.h"
#include "inotifyservice.h"
#include "uicommon/curvegraphwidget.h"
#include "motion/imotion.h"
#include "card/icardcontinterp.h"
#include "g12globalenv.h"
#include "g12common.h"
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QFileDialog>
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "g12globalenv.h"
#include "card/icard.h"
#include "g12utils.h"
//#include "calib/calibratio.h"
//#include "imagecalibprocess.h"
#include "clinecameralist.h"
#include "maindefine.h"

#define PLUGIN_PATH "plugins/linecamerasettinguiplugin"
#define PLUGIN_NAME "linecamerasettinguiplugin"

CLineCameraManagerUI::CLineCameraManagerUI(QWidget *parent) 
    : QWidget(parent)
    , ui(new Ui::CLineCameraManagerUI)
    , m_pCameraManager(NULL)
    , m_pCurrentCamera(NULL)
    , m_currentCameraName("")
    , m_pThreadDisplay(NULL)
    , m_pThreadSnap(NULL)
    , m_pTriggerThread(NULL)
    , m_pThreadInit(NULL)
    , m_pLightCtrlWidget(NULL)
    , m_pCurGraph(NULL)
    , m_pCalibWidget(NULL)
    , m_pCalibRatioWidget(NULL)
    , m_recvChannel(0)
    , m_bInitRunning(false)
    , m_bIsGGCard(false)
	, m_pCardConInterp(NULL)
{
    ui->setupUi(this);
    m_ImageList.clear();
    m_lock.Init();
    m_Counterlock.Init();
    m_imageLock.Init();
    //setAttribute(Qt::WA_DeleteOnClose, true);

    qRegisterMetaType<QVariant>("QVariant");

    int index = 0;

    m_pShowImageWidget = ui->C1;
    m_pShowImageWidget->showStatusBar(XTV_IMAGE_SHOW::VS_STATUS_SHOW_ALL);
    m_pShowImageWidget->showToolBar(XTV_IMAGE_SHOW::VS_TOOLBAR_IMAGEZOOM);

    ui->checkBox_C1->setStyleSheet(QStringLiteral("background-color: rgb(0,0,255);"));
    ui->checkBox_C2->setStyleSheet(QStringLiteral("background-color: rgb(0,0,0);"));
    ui->checkBox_C3->setStyleSheet(QStringLiteral("background-color: rgb(0,255,0);"));
    ui->checkBox_C4->setStyleSheet(QStringLiteral("background-color: rgb(0,255,255);"));
    ui->checkBox_C5->setStyleSheet(QStringLiteral("background-color: rgb(255,0,255);"));
    ui->checkBox_C6->setStyleSheet(QStringLiteral("background-color: rgb(255,255,0);"));
    ui->checkBox_C7->setStyleSheet(QStringLiteral("background-color: rgb(192,192,192);"));
    ui->checkBox_C8->setStyleSheet(QStringLiteral("background-color: rgb(128,128,128);"));

    //treeWidget_Job->setVisible(false);
    //pushButton_saveJob->setVisible(false);
    //pushButton_cancleJob->setVisible(false);
    //connect(this, SIGNAL(signalUpDateUiImage(QVariant)), this, SLOT(SlotUpDateUiImage(QVariant)));
    //connect(this, SIGNAL(signalUpdateStatus(QString, int)), this, SLOT(slotUpdateStatus(QString, int)));
    connect(this, SIGNAL(signalUpDateUiImage(QVariant)), this, SLOT(SlotUpDateUiImage(QVariant)));
    connect(this, SIGNAL(signalUpdateStatus(QString, int)), this, SLOT(slotUpdateStatus(QString, int)));
    //connect(ui->C1, SIGNAL(sigItemRBtDoubleCliecked(QPointF &)),this, SLOT(slotItemRBtDoubleCliecked(QPointF &)));
    connect(this,SIGNAL(signalShowImage()),this,SLOT(slotShowImage()));
    connect(this, SIGNAL(signalUpdateUI()), this, SLOT(slotUpdateUI()));

    m_realEvent.Init(true, false);
    m_displayEvent.Init(false, true);
    m_arriveEvent.Init(false, false);
    m_pThreadDisplay = new JThreadTP<CLineCameraManagerUI,void*,void>;
    m_pThreadSnap = new JThreadTP<CLineCameraManagerUI,void*,void>;
    m_pTriggerThread = new JThreadTP<CLineCameraManagerUI, void*, void>;
    m_pThreadInit = new JThreadTP<CLineCameraManagerUI, void*, void>;
    m_bRunning = true;
    m_pThreadDisplay->StartThread(this, &CLineCameraManagerUI::ThreadDisplay, this);

    m_waitDlg.setWindowTitle(QString::fromLocal8Bit("图像采集"));
    m_waitDlg.addButton(QMessageBox::Cancel);
    //m_waitDlg.setModal(false);

    connect(this, SIGNAL(signalCancelWait()), this, SLOT(slotWaitCancel()));
    connect(this, SIGNAL(signalFinished()), this, SLOT(slotWaitFinished()));
    connect(m_waitDlg.button(QMessageBox::Cancel), SIGNAL(clicked()), this, SLOT(slotWaitCancel()));
    connect(&m_waitDlg, SIGNAL(finished (int)), this, SLOT(slotWaitCancel(int)));
}


CLineCameraManagerUI::~CLineCameraManagerUI()
{
    m_bRunning = false;
    if (NULL != m_pThreadDisplay)
    {
        while(m_pThreadDisplay->IsRunning())
        {
            Sleep(50);
        }
        delete m_pThreadDisplay;
        m_pThreadDisplay = NULL;
    }
    if (NULL != m_pTriggerThread)
    {
        while (m_pTriggerThread->IsRunning())
        {
            Sleep(50);
        }
        delete m_pTriggerThread;
        m_pTriggerThread = NULL;
    }
    if (NULL != m_pThreadInit)
    {
        while (m_pThreadInit->IsRunning())
        {
            Sleep(50);
        }
        delete m_pThreadInit;
        m_pThreadInit = NULL;
    }
    if (NULL != m_pThreadSnap)
    {
        while (m_pThreadSnap->IsRunning())
        {
            Sleep(50);
        }
        delete m_pThreadSnap;
        m_pThreadSnap = NULL;
    }
    delete ui;
}

void CLineCameraManagerUI::on_treeWidget_itemDoubleClicked( QTreeWidgetItem * item, int column )
{
    switch(column)
    {
    case 0:
        item->setFlags(item->flags()&~(Qt::ItemIsEditable));
        break;
    case 1:
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        break;
    }
}

void CLineCameraManagerUI::on_pushButton_save_clicked( bool checked /*= false */ )
{
    if (bIsModify())
    {
        SaveParam();
    }
}

void CLineCameraManagerUI::on_pushButton_get_image_clicked()
{
    m_displayEvent.ResetEvent();
    QMessageBox info;
    int iTimeOut = 30000;
    if (NULL == m_pCurrentCamera)
    {
        return;
    }
    m_waitDlg.setText(QString::fromLocal8Bit("取图中..."));
    //m_waitDlg.setStandardButtons(0);
    m_waitDlg.show();
    if (GrapOneImage())
    {
        m_waitDlg.setText(QString::fromLocal8Bit("取图完成,显示中..."));
        emit signalShowImage();
        m_waitTID = startTimer(100);    //定时检测显示状态
        m_waitTimeOutID = startTimer(30000); // 超时处理
    }
    else
    {
        UpdateCamStatus(QString::fromLocal8Bit("取像失败"));
        m_waitDlg.hide();
    }
}

void CLineCameraManagerUI::on_pushButton_singlePhoto_clicked()
{
    if (NULL == m_pCurrentCamera)
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选择要执行的相机或配置有效相机！"));
        return;
    }
    if (m_pThreadSnap->IsRunning())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("采集未完成！"));
        return;
    }
    SetUIEnable(false);
    m_waitDlg.setText(QString::fromLocal8Bit("采集中..."));
    m_waitDlg.addButton(QMessageBox::Cancel);
    m_waitDlg.show();
    //更新行频率
    LINE_CAM_PARAM info;
    m_pCurrentCamera->GetCameraInParam(info);
    info.height = info.snapheight;
    m_pCurrentCamera->SetCameraInParam(info);
    m_pThreadSnap->StartThread(this, &CLineCameraManagerUI::ThreadSnap, this);

}

void CLineCameraManagerUI::on_pushButton_realTimeDisplay_clicked( bool checked )
{
    if (NULL == m_pCurrentCamera)
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选择要执行的相机或配置有效相机！"));
        ui->pushButton_realTimeDisplay->setChecked(!checked);
        return;
    }
    //pushButton_add_camera->setEnabled(!checked);
    ui->pushButton_singlePhoto->setEnabled(!checked);
    //treeWidget_DisplayJob->setEnabled(!checked);
    //pushButton_alterAttribute->setEnabled(!checked);

    if (checked)
	{
		//ui->pushButton_save->setEnabled(false);
        m_pCurGraph->clearValue();
        SetUIEnable(false);
        ui->pushButton_realTimeDisplay->setEnabled(true);

        //pushButton_realTimeDisplay->setIcon(m_real_off_icon);
        m_realEvent.SetEvent();
        m_pCurrentCamera->SetTriggerFunction(CLineCameraManagerUI::CallbackGrapFunction, this); //防止回调被修改
        m_bTriggerRunning = true;
		ClearLightCrlNumber();
        m_recvChannel = 0;
        int iCount = m_channelCount > 0 ? m_channelCount : 1;
        if (!m_pCurrentCamera->Continuations(true, iCount))
        {
            m_bTriggerRunning = false;
            ui->pushButton_realTimeDisplay->setChecked(false);
            SetUIEnable(true);
            m_realEvent.ResetEvent();
            m_pCurrentCamera->AbortAcq();
            m_pCurrentCamera->Continuations(false);
            m_pCurrentCamera->Reset();
        }
		else
		{
			//外部触发时， 开线程持续触发高速IO
			if (m_pCameraParam.bExtTrigger)
			{
                startTrigger(-1);
				//m_pThreadInit->StartThread(this, &CLineCameraManagerUI::ThreadTriger, this);
			}
		}
    }
    else
    {
        //pushButton_realTimeDisplay->setIcon(m_real_on_icon);
        stopTrigger();

        m_bTriggerRunning = false;
        SetUIEnable(true);
        m_realEvent.ResetEvent();
        m_pCurrentCamera->Continuations(false);
        m_pCurrentCamera->Reset();
    }
}

void CLineCameraManagerUI::on_pushButton_Refresh_clicked()
{
    on_pushButton_realTimeDisplay_clicked(false);
    CLineCameraList dlg;
    if (0 == dlg.exec())
    {
        if (!m_pCameraParam.strCameraName.empty())
        {
            UpdateCurrentCamera(m_pCameraParam.strCameraName);
            InitCam();
        }
        if (!m_bInitRunning)
        {
            slotUpdateUI();
        }
        //InitUI();
    }
}

void CLineCameraManagerUI::on_comboBox_Camera_Select_currentIndexChanged( int index )
{
    if (bIsModify())
    {
        if (QMessageBox::Save == QMessageBox::information(this, QString::fromLocal8Bit("保存参数"), QString::fromLocal8Bit("检测到参数有修改， 是否保存"), QMessageBox::Save | QMessageBox::Cancel))
        {
            SaveParam();
        }
    }
    if (m_ValidCamList.size() > index)
    {
        UpdateCurrentCamera(m_ValidCamList[index]);
    }
    InitUI();
}

void CLineCameraManagerUI::on_pushButton_browse_clicked()
{
    QFileDialog fileDialog;//创建一个QFileDialog对象，构造函数中的参数可以有所添加。
    fileDialog.setWindowTitle(tr("select configuration file"));//设置文件保存对话框的标题
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);//设置文件对话框为保存模式
    fileDialog.setFileMode(QFileDialog::AnyFile);//设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件
    fileDialog.setViewMode(QFileDialog::Detail);//文件以详细的形式显示，显示文件名，大小，创建日期等信息；
    //还有另一种形式QFileDialog::List，这个只是把文件的文件名以列表的形式显示出来
    //fileDialog->setGeometry(10,30,300,200);//设置文件对话框的显示位置
    fileDialog.setDirectory(".");//设置文件对话框打开时初始打开的位置
    //fileDialog.setFilter(tr("Image Files(*.ccf)"));//设置文件类型过滤器
    if(fileDialog.exec() == QDialog::Accepted)
    {
        //注意使用的是QFileDialog::Accepted或者QDialog::Accepted,不是QFileDialog::Accept
        QString file_path = fileDialog.selectedFiles()[0];//得到用户选择的文件名
        ui->lineEdit_config_path->setText(file_path);
    }
}

void CLineCameraManagerUI::on_spinBox_min_col_valueChanged()
{
    double min = ui->spinBox_min_col->value();
    double max = ui->spinBox_max_col->value();
    if (min < max)
    {
        m_pCurGraph->setXAxisLower(min);
    }
}

void CLineCameraManagerUI::on_spinBox_max_col_valueChanged()
{
    double min = ui->spinBox_min_col->value();
    double max = ui->spinBox_max_col->value();
    if (min < max)
    {
        m_pCurGraph->setXAxisUpper(max);
    }
}

void CLineCameraManagerUI::on_spinBox_min_gray_valueChanged()
{
    double min = ui->spinBox_min_gray->value();
    double max = ui->spinBox_max_gray->value();
    if (min < max)
    {
        m_pCurGraph->setYAxisLower(min);
    }
}

void CLineCameraManagerUI::on_spinBox_max_gray_valueChanged()
{
    double min = ui->spinBox_min_gray->value();
    double max = ui->spinBox_max_gray->value();
    if (min < max)
    {
        m_pCurGraph->setYAxisUpper(max);
    }
}

void CLineCameraManagerUI::on_spinBox_min_limit_valueChanged()
{
    double min = ui->spinBox_min_limit->value();
    double max = ui->spinBox_max_limit->value();
    if (min < max)
    {
        m_pCurGraph->setRangeLower(min);
    }
    m_pCurGraph->setRangeLower(ui->spinBox_min_limit->value());
}

void CLineCameraManagerUI::on_spinBox_max_limit_valueChanged()
{
    double min = ui->spinBox_min_limit->value();
    double max = ui->spinBox_max_limit->value();
    if (min < max)
    {
        m_pCurGraph->setRangeUpper(max);
    }
}

void CLineCameraManagerUI::InitUI()
{
    //初始化Item
    int index = 0;

    bool bChannelOn[SEQUE_NUMBER] = {0};
    int iChannel = 0;
    vector<QString> vecChannelNames;
    QStringList strNameList;

    QString strList = getCurrentProgramRecipesList().c_str();
    if (strList.isEmpty())
    {
        strList = "SEQUENCE_A,SEQUENCE_B,SEQUENCE_C,SEQUENCE_D,SEQUENCE_E,SEQUENCE_F,SEQUENCE_G,SEQUENCE_H";
    }
    strNameList = strList.split(",");
    
    if (NULL == m_pCurrentCamera)
    {
        //for (int i = 0; i < m_pCameraParam.channel ;i++)
        //{
        //    if (i < strNameList.size())
        //    {
        //        vecChannelNames.push_back(strNameList[i]);
        //    }
        //    //string strName = "SEQUENCE_";
        //    //strName += 'A' + i;
        //    //vecChannelNames.push_back(strName);
        //}
    }
    else
    {
        //获取配方更新， 同步UI
        //int index = ui->comboBox_Camera_Select->currentIndex();
        //if (index < 0)
        //{
        //    index = 0;
        //}
        //TBM
		//根据相机名称获取组索引
        int index = 0;
        string          m_strCamName[CAMERA_NUMBER];
        bool bFound = false;
        for (int i = 0; i < CAMERA_NUMBER; i++)
        {
            getCamNameList(i, m_strCamName);
            for (int j = 0; j < CAMERA_NUMBER; j++)
            {
                if (m_pCameraParam.strCameraName == m_strCamName[j])
                {
                    index = i;
                    bFound = true;
                    break;
                }
            }
            if (bFound)
            {
                break;
            }
        }
        for (int i = 0; i < SEQUE_NUMBER ;i++)
        {
            if (i < strNameList.size())
            {
                QString strSeqName = strNameList[i];
                int ires = 0;
                int iType;
                int iCamIndex = 0;
                ires = getSqueLightTypeAndCameraIndex(strSeqName.toStdString(), iType, iCamIndex);
                RETURN_IF(-1 == ires, ERR_BUSSINESS_CARD_INSTANCE);
                if (iCamIndex > 0 && iCamIndex == index)
                {
                    iChannel++;
                    vecChannelNames.push_back(strSeqName);
                    //string strName = "SEQUENCE_";
                    //strName += 'A' + i;
                    //vecChannelNames.push_back(strName);
                }
            }
        }
        m_channelCount = iChannel;
    }

    QTreeWidgetItem *pItem = NULL;
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::fromLocal8Bit(m_pCameraParam.strCamDes.c_str()));
        pItem->setToolTip(1, QString::fromLocal8Bit(m_pCameraParam.strCameraName.c_str()));
    }

    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, m_pCameraParam.strCameraID.c_str());
    }
    //pItem = ui->treeWidget->topLevelItem(index++);
    //if (NULL != pItem)
    //{
    //    pItem->setText(1, QString::number(m_pCameraParam.channel));
    //}
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(m_pCameraParam.width));
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(m_pCameraParam.snapheight));
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(m_pCameraParam.realheight));
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(m_pCameraParam.exposure));
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(m_pCameraParam.gain));
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(m_pCameraParam.grabFreq));
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(m_pCameraParam.blockSize));
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        if (m_pCameraParam.bExtTrigger)
        {
            pItem->setText(1, "true");
        }
        else
        {
            pItem->setText(1, "false");
        }

        //QTreeWidgetItem *pItemChild = NULL;
        //pItemChild = pItem->child(0);
        //if (NULL != pItemChild)
        //{
        //    pItemChild->setText(1, "Internal");
        //}
        //pItemChild = pItem->child(1);
        //if (NULL != pItemChild)
        //{
        //    pItemChild->setText(1, "Rising edge");
        //}
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        if (m_pCameraParam.bIsInvert)
        {
            pItem->setText(1, "true");
        }
        else
        {
            pItem->setText(1, "false");
        }
    }
    //pItem = ui->treeWidget->topLevelItem(index++);
    //if (NULL != pItem)
    //{
    //    pItem->setText(1, QString::number(m_pCameraParam.channel));
    //}
    ui->lineEdit_config_path->setText(m_pCameraParam.strCamConfigPath.c_str());
    //UpdateImageTable();
    //for (int in = 0; in < m_pCameraParam.channel; in++)
    //{
    //    if (ui->tabWidget->count() >  in+1)
    //    {
    //        ui->tabWidget->setTabText(in+1, vecChannelNames[in]);
    //    }
    //    m_CheckBox[in]->setText(vecChannelNames[in]);
    //}
}

void CLineCameraManagerUI::InitCam()
{
    do 
    {
        vector<string> vecName;
        m_pCameraManager->GetCameraNameList(vecName);
        if (vecName.size() > 0)
        {
            bool bInit = false;
            if (NULL != m_pCameraManager)
            {
                ui->comboBox_Camera_Select->clear();
                m_ValidCamList.clear();
                for (int i = 0; i < vecName.size(); i++)
                {
                    LINE_CAM_PARAM param;
                    m_pCameraManager->GetCameraInParam(vecName[i], param);
                    if (param.bEable)
                    {
                        if (!bInit)
                        {
                            bInit = true;
                            m_pCameraParam.strCameraName = vecName[i];
                        }
                        m_ValidCamList.push_back(param.strCameraName);
                    }
                }
            }
        }
    } while (0);
}

void CLineCameraManagerUI::SaveParam()
{
	bool bUpdateCamList = false;
    //初始化Item
    SetUIEnable(false);
    int index = 0;
    QTreeWidgetItem *pItem = NULL;
    QString strValue;

    GetCameraInParam(m_pCameraParam);
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        string camDes = strValue.toLocal8Bit().data();
        if (m_pCameraParam.strCamDes != camDes)
        {
			bUpdateCamList = true;
            m_pCameraParam.strCamDes = camDes;
        }
        //重新初始化 ui->comboBox_CameraName;
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        m_pCameraParam.strCameraID = strValue.toStdString();
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        m_pCameraParam.width = strValue.toULong();
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        m_pCameraParam.snapheight = strValue.toULong();
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        m_pCameraParam.realheight = strValue.toULong();
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        m_pCameraParam.exposure = strValue.toULong();
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        m_pCameraParam.gain = strValue.toULong();
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        m_pCameraParam.grabFreq = strValue.toULong();
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        m_pCameraParam.blockSize = strValue.toULong();
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        m_pCameraParam.bExtTrigger = true;
        if (strValue == "false")
        {
            m_pCameraParam.bExtTrigger = false;
        }

    //    QTreeWidgetItem *pItemChild = NULL;
    //    pItemChild = pItem->child(0);
    //    if (NULL != pItemChild)
    //    {
    //        strValue = pItem->text(1);
    //        if (strValue == "Internal")
    //        {
    //            m_pCameraParam.tgType = LINE_TG_INTERNAL;
    //        }
    //        else
    //        {
				//m_pCameraParam.tgType = LINE_TG_EXTERNAL;
    //        }
    //    }
    //    pItemChild = pItem->child(1);
    //    if (NULL != pItemChild)
    //    {
    //        strValue = pItem->text(1);
    //        if (strValue == "Rising edge")
    //        {
    //            m_pCameraParam.tgDetection = TG_RISING_EDGE;
    //        }
    //        else
    //        {

    //        }
    //    }
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        strValue = pItem->text(1);
        m_pCameraParam.bIsInvert = false;
        if (strValue == "true")
        {
            m_pCameraParam.bIsInvert = true;
        }
    }
    //pItem = ui->treeWidget->topLevelItem(index++);
    //if (NULL != pItem)
    //{
    //    strValue = pItem->text(1);
    //    m_pCameraParam.channel = strValue.toInt();
    //}

    if (0 == m_pCameraParam.blockSize 
        || m_pCameraParam.snapheight < m_pCameraParam.blockSize
        || 0 != m_pCameraParam.snapheight%m_pCameraParam.blockSize)
    {
        QMessageBox::information(this, QString::fromLocal8Bit("参数错误"), QString::fromLocal8Bit("单帧图像无法正常分块，请确认单帧行数能否整除块大小"));
        SetUIEnable(true);
        return;
    }

    m_pCameraParam.strCamConfigPath = ui->lineEdit_config_path->text().toLocal8Bit();
    if (!m_pCameraManager->SetCameraInParam(m_pCameraParam))
    {
        if (NULL != m_pCurrentCamera)
        {
            m_pCurrentCamera->GetCameraInParam(m_pCameraParam);
            InitUI();
        }
    }
    //UpdateImageTable();
    SetUIEnable(true);

    //同步更新其他相机参数， 保持各相机同步
    vector<string> vecName;
    LINE_CAM_PARAM info;
    m_pCameraManager->GetCameraNameList(vecName);
    for (int i = 0; i < vecName.size(); i++)
    {
		if (vecName[i] == m_pCameraParam.strCameraName)
		{
			m_pCameraManager->GetCameraInParam(vecName[i], info);
			info.width = m_pCameraParam.width;
			info.snapheight = m_pCameraParam.snapheight;
			info.blockSize = m_pCameraParam.blockSize;
			info.exposure = m_pCameraParam.exposure;
			info.gain = m_pCameraParam.gain;
			m_pCameraManager->SetCameraInParam(info);
		}

    }
	if (bUpdateCamList)
	{
		InitCamListUI();  //描述有修改， 则更新列表
	}
}

void CLineCameraManagerUI::ThreadDisplay( void* param)
{
    CLineCameraManagerUI* pDlg = (CLineCameraManagerUI*)param;
    while(m_bRunning)
    {
        if (JEVENT_WAIT_TIME_OUT == JEvent::WaitForSingleObject(&m_realEvent, 2000))
        {
            continue;
		}
        if (!m_bRunning)
        {
            break;
        }
        if (JEVENT_WAIT_TIME_OUT == JEvent::WaitForSingleObject(&m_arriveEvent, 1))
        {
            continue;
        }
        if (m_bRunning && JEVENT_WAIT_TIME_OUT != JEvent::WaitForSingleObject(&m_displayEvent, 10))
        {
            m_imageLock.Lock();
            if (m_ImageList.size() > 0)
            {
                m_ImageMemory = m_ImageList.front();
                m_ImageList.clear();
            }
            m_imageLock.Unlock();
            m_displayEvent.ResetEvent();
            //pDlg->GrapOneImage();
            emit pDlg->signalShowImage();
            //Sleep(10);
        }
        //pDlg->GrapOneImage();
        //pDlg->signalShowImage();
    }
}

void CLineCameraManagerUI::ThreadSnap( void* )
{
    if (NULL != m_pCurrentCamera)
    {
        if (!m_pCurrentCamera->IsOpen())
        {
            m_pCurrentCamera->Open();
        }
        if (!m_pCurrentCamera->SingleAcq())
        {
            m_errorCode = L_UI_GET_IMAGE_FAILED;
            emit signalCancelWait();
            return ;
        }

        //等待图像采集完成
        int iWaitTime = 0;
        do 
        {
            if (m_pCurrentCamera->IsSnapFinished())
            {
                break;
            }
            iWaitTime += 100;
            Sleep(100);
        } while (iWaitTime < 60000);

        if (GrapOneImage())
        {
            emit signalShowImage();
        }
        else
        {
            m_errorCode = L_UI_GET_IMAGE_FAILED;
            emit signalCancelWait();
            return ;
        }
    }
    emit signalFinished();
}

void CLineCameraManagerUI::ThreadInitCam( void* param)
{
    CLineCameraManagerUI* pDlg = (CLineCameraManagerUI*)param;
    while (m_bInitRunning &&m_bRunning)
    {
        //获取相机管理插件实例
        m_pCameraManager = (ILineCameraFactoryManage*)GetSDKInstance("cameramanager", ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE);
        if (NULL == m_pCameraManager)
        {
            Sleep(1000);
            continue;
        }
        if (m_pCameraManager->IsInitFinished())
        {
            break;
        }
        Sleep(100);
    }
    if (NULL != pDlg)
    {
        pDlg->InitCam();
    }
    //等待初始化完成
    while (NULL == m_pCurrentCamera && m_bInitRunning &&m_bRunning)
    {
        vector<string>::iterator itCamName = m_ValidCamList.begin();
        for (; itCamName != m_ValidCamList.end(); ++itCamName)
        {
            m_pCameraParam.strCameraName = *itCamName;
            m_pCurrentCamera = m_pCameraManager->GetCameraInstance(m_pCameraParam.strCameraName);
            if (NULL != m_pCurrentCamera)
            {
                break;
            }
        }
        Sleep(100);
    }
    m_bInitRunning = false;
    emit signalUpdateUI();
}

void CLineCameraManagerUI::ThreadTriger(void *param)
{
    int iTime = (int)param;
    while (m_bTriggerRunning)
    {
        int i = 0;
        if (iTime < 0 || i < iTime)
        {
			RETURN_IF(NULL == m_pCardConInterp);
            m_pCardConInterp->SetComparePluse(2, 0, 100);
            Sleep(1);
            i++;
        }
    }
}
//
//void CLineCameraManagerUI::ThreadTriger( void* param )
//{
//	iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion", iMotionCfg::IID_MOTION_CONFIG);
//	RETURN_IF(NULL == pMotionCfg);
//
//#ifdef CARD_GG
//	while (m_bTriggerRunning)
//	{
//		m_pCardConInterp->SetComparePluse(2, 0, 100);
//		Sleep(1);
//	}
//#else
//	m_pCardConInterp->SetComparePluse(0, 0, 0);
//#endif
//}

bool CLineCameraManagerUI::GrapOneImage()
{
    if (NULL == m_pCurrentCamera)
    {
        //QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请关联要执行的相机！"));
        return false;
    }
    int width = 0;
    int height = 0;
    int iSize = 0;
    unsigned char *pImageBuffer = NULL;

    try
    {
        if (!m_pCurrentCamera->GetImageSize(width, height))
        {
            return false;
        }
        iSize = width * height;

        CISAPUGuard<JMutex> locker(m_lock);
        pImageBuffer = (unsigned char *)m_pCurrentCamera->GetImage();
        if (NULL == pImageBuffer)
        {
            return false;
        }
        if (NULL != m_ImageMemory.pixelR && (width != m_ImageMemory.width || height != m_ImageMemory.height))
        {
            delete m_ImageMemory.pixelR;
            m_ImageMemory.pixelR = NULL;
        }
        m_ImageMemory.width  = width;
        m_ImageMemory.height = height;
        if (m_ImageMemory.pixelR == NULL)
        {
            m_ImageMemory.pixelR = new unsigned char[iSize];
        }
        memcpy(m_ImageMemory.pixelR, pImageBuffer, sizeof(unsigned char) * iSize);
        if (m_pCurrentCamera->IsImplemented())
        {
            if (NULL == m_ImageMemory.pixelG)
            {
                m_ImageMemory.pixelG = new unsigned char[iSize];
            }
            if (NULL == m_ImageMemory.pixelB)
            {
                m_ImageMemory.pixelB = new unsigned char[iSize];
            }
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

void CLineCameraManagerUI::timerEvent( QTimerEvent *event )
{
    if (event->timerId() == m_statusTID)
    {
        checkCameraStatus();
    }
    else if (event->timerId() == m_waitTID)
    {
        if (m_displayEvent.IsSiangaled())
        {
            m_waitDlg.hide();
            killTimer(m_waitTID);
            killTimer(m_waitTimeOutID);
        }
    }
    else if (event->timerId() == m_waitTimeOutID)
    {
        m_waitDlg.hide();
        UpdateCamStatus(QString::fromLocal8Bit("显示超时"));
        killTimer(m_waitTID);
        killTimer(m_waitTimeOutID);
    }
}

void CLineCameraManagerUI::checkCameraStatus()
{
    if (NULL != m_pCurrentCamera)
    {
        char *szBuf = NULL;
        int iSize = 0;
        int iErrorCode = m_pCurrentCamera->GetLastError(szBuf, &iSize);
        if (0 < iErrorCode)
        {
            if (iSize > 0)
            {
                szBuf = new char[iSize+1];
                m_pCurrentCamera->GetLastError(szBuf, &iSize);
                UpdateCamStatus(szBuf);
            }
            else if (!m_pCurrentCamera->IsOpen())
            {
                UpdateCamStatus(QString::fromLocal8Bit("相机离线"));
            }
            else
            {
                UpdateCamStatus(QString::fromLocal8Bit("相机状态异常，错误码为%1").arg(iErrorCode));
            }
        }
        else
        {
            UpdateCamStatus("");
        }
    }
    else
    {
        UpdateCamStatus(QString::fromLocal8Bit("当前无相机"));
    }
}

void CLineCameraManagerUI::UpdateCurrentCamera( string strCamerName )
{
    if (NULL != m_pCameraManager)
    {
        m_pCameraParam.strCameraName = strCamerName;
        if (NULL != m_pCurrentCamera)
        {
            m_pCurrentCamera->SetTriggerFunction(NULL, NULL);
        }
        m_pCurrentCamera = m_pCameraManager->GetCameraInstance(strCamerName);
        GetCameraInParam(m_pCameraParam);
        if (NULL == m_pCurrentCamera)
        {
            UpdateCamStatus(QString::fromLocal8Bit("获取相机实例失败！"));
        }
        else
        {
            m_pCurrentCamera->SetTriggerFunction(CLineCameraManagerUI::CallbackGrapFunction, this);
        }
        //UpdateImageTable();
        checkCameraStatus();
    }
}

//void CLineCameraManagerUI::UpdateImageTable()
//{
    //LINE_CAM_PARAM param;
    //param.strCameraName = m_pCameraParam.strCameraName;
    //GetCameraInParam(param);
    //int iCount = ui->tabWidget->count()-1;
    //if (iCount < param.channel)
    //{
    //    ui->tabWidget->addTab(m_pShowImageWidget, QString(""));
    //    m_CheckBox[index]->setVisible(true);
    //}
    //else if(iCount > param.channel)
    //{
    //    for (int index = iCount-1; index >= param.channel; index--)
    //    {
    //        ui->tabWidget->removeTab(index+1);
    //        m_CheckBox[index]->setVisible(false);
    //    }
    //}
//}

bool CLineCameraManagerUI::bIsModify()
{
    bool bRet = true;
    int index = 0;
    QString strValue;
    QTreeWidgetItem *pItem = NULL;

    do 
    {
        pItem = ui->treeWidget->topLevelItem(index++);
        if (NULL != pItem)
        {
            strValue = pItem->text(1);
            BREAK_IF(0 != strValue.compare(QString::fromLocal8Bit(m_pCameraParam.strCamDes.c_str())));
        }
        pItem = ui->treeWidget->topLevelItem(index++);
        if (NULL != pItem)
        {
            strValue = pItem->text(1);
            BREAK_IF(0 != strValue.compare(m_pCameraParam.strCameraID.c_str()));
        }
        pItem = ui->treeWidget->topLevelItem(index++);
        if (NULL != pItem)
        {
            strValue = pItem->text(1);
            BREAK_IF(m_pCameraParam.width != strValue.toULong());
        }
        pItem = ui->treeWidget->topLevelItem(index++);
        if (NULL != pItem)
        {
            strValue = pItem->text(1);
            BREAK_IF(m_pCameraParam.snapheight != strValue.toULong());
        }
        pItem = ui->treeWidget->topLevelItem(index++);
        if (NULL != pItem)
        {
            strValue = pItem->text(1);
            BREAK_IF(m_pCameraParam.realheight != strValue.toULong());
        }
        pItem = ui->treeWidget->topLevelItem(index++);
        if (NULL != pItem)
        {
            strValue = pItem->text(1);
            BREAK_IF(m_pCameraParam.exposure != strValue.toULong());
        }
        pItem = ui->treeWidget->topLevelItem(index++);
        if (NULL != pItem)
        {
            strValue = pItem->text(1);
            BREAK_IF(m_pCameraParam.gain != strValue.toULong());
        }
        pItem = ui->treeWidget->topLevelItem(index++);
        if (NULL != pItem)
        {
            strValue = pItem->text(1);
            BREAK_IF(m_pCameraParam.grabFreq != strValue.toULong());
        }
        pItem = ui->treeWidget->topLevelItem(index++);
        if (NULL != pItem)
        {
            strValue = pItem->text(1);
            BREAK_IF(m_pCameraParam.blockSize != strValue.toULong());
        }
        strValue = ui->lineEdit_config_path->text();
        BREAK_IF(0 != strValue.compare(m_pCameraParam.strCamConfigPath.c_str()));
        //以上检测项相同
        bRet = false;
    } while (0);
    return bRet;
}

bool CLineCameraManagerUI::SetUIEnable( bool bEnable )
{
    //ui->tabWidget->setEnabled(bEnable);
    ui->lineEdit_config_path->setEnabled(bEnable);
    ui->pushButton_browse->setEnabled(bEnable);
    ui->pushButton_singlePhoto->setEnabled(bEnable);
    ui->pushButton_get_image->setEnabled(bEnable);
    ui->pushButton_realTimeDisplay->setEnabled(bEnable);
    ui->tabWidget_otherPage1->setEnabled(bEnable);
    return true;
}

void CLineCameraManagerUI::SnapFinished()
{
    INotifyService* pNotifyService = CommonUtils::getNotifyService();
    if (pNotifyService == NULL)
    {
        return;
    }
    if (!pNotifyService->broadcast("line_snap_finished",(OS_LPARAM)0,(OS_LPARAM)0))
    {
        return;
    }
}

void CLineCameraManagerUI::slotShowImage()
{
    stImageInfo imageInfo;

    m_lock.Lock();
    if (NULL != m_pShowImageWidget)
    {
        m_pShowImageWidget->showPictureInLabel(m_ImageMemory.pixelR, m_ImageMemory.pixelG, m_ImageMemory.pixelB, m_ImageMemory.width, m_ImageMemory.height);
    }
    if (NULL != m_pCurGraph)
    {
        QVector<double> keyList;
        QVector<double> valueList;
        double max = 0;
        double min = 255;
        int iValue = 0;
        for (int iLine = 0; iLine < m_ImageMemory.height && iLine < SEQUE_NUMBER; iLine++)
        {
            if (m_bShowRawVidio[iLine])
            {
                keyList.clear();
                valueList.clear();
                for (int j = 0; j < m_ImageMemory.width; j++)
                {
                    iValue = m_ImageMemory.pixelR[j + iLine*m_ImageMemory.width];
                    keyList << j;
                    if (iValue > 255)
                    {
                        iValue = 255;
                    }
                    valueList << iValue;
                    if (max < iValue)
                    {
                        max = iValue;
                    }
                    if (min > iValue)
                    {
                        min = iValue;
                    }
                }
                ui->label_up->setText(QString::fromLocal8Bit("最大阈值:%1").arg(max));
                ui->label_down->setText(QString::fromLocal8Bit("最小阈值:%1").arg(min));
                m_pCurGraph->setValues(keyList, valueList, iLine);
            }
        }
    }

    m_lock.Unlock();
	static int index = 0;
    QString strInfo = ".";
    if (index%2)
    {
        strInfo = "..";
    }
    UpdateCamStatus(strInfo);
	index++;
    //通知更新完成
    m_displayEvent.SetEvent();
}

void CLineCameraManagerUI::slotUpdateUI()
{
    InitCamListUI();
    //CreateTreeData();
    InitUI();
    checkCameraStatus();
    SetUIEnable(true);
    m_statusTID = startTimer(10000);
}

void CLineCameraManagerUI::slotWaitCancel()
{
    if (m_pCurrentCamera)
    {
        m_pCurrentCamera->AbortAcq();
    }
    m_waitDlg.hide();
    UpdateCamStatus(QString::fromLocal8Bit("取像异常"));
    SetUIEnable(true);
}

void CLineCameraManagerUI::slotWaitCancel( int iR )
{
    if (m_pCurrentCamera)
    {
        m_pCurrentCamera->AbortAcq();
    }
    UpdateCamStatus(QString::fromLocal8Bit("取像取消"));
    SetUIEnable(true);
}

void CLineCameraManagerUI::slotWaitFinished()
{
    SetUIEnable(true);
    SnapFinished();
    m_waitDlg.hide();
}

void CLineCameraManagerUI::on_checkBox_C1_clicked( bool checked )
{
    m_bShowRawVidio[0] = checked;
    m_pCurGraph->clearValue();
}

void CLineCameraManagerUI::on_checkBox_C2_clicked( bool checked )
{
    m_bShowRawVidio[1] = checked;
    m_pCurGraph->clearValue();
}

void CLineCameraManagerUI::on_checkBox_C3_clicked( bool checked )
{
    m_bShowRawVidio[2] = checked;
    m_pCurGraph->clearValue();
}

void CLineCameraManagerUI::on_checkBox_C4_clicked( bool checked )
{
    m_bShowRawVidio[3] = checked;
    m_pCurGraph->clearValue();
}

void CLineCameraManagerUI::on_checkBox_C5_clicked( bool checked )
{
    m_bShowRawVidio[4] = checked;
    m_pCurGraph->clearValue();
}

void CLineCameraManagerUI::on_checkBox_C6_clicked( bool checked )
{
    m_bShowRawVidio[5] = checked;
    m_pCurGraph->clearValue();
}

void CLineCameraManagerUI::on_checkBox_C7_clicked( bool checked )
{
    m_bShowRawVidio[6] = checked;
    m_pCurGraph->clearValue();
}

void CLineCameraManagerUI::on_checkBox_C8_clicked( bool checked )
{
    m_bShowRawVidio[7] = checked;
    m_pCurGraph->clearValue();
}

bool CLineCameraManagerUI::init()
{
    bool bRet = true;
    SetUIEnable(false);
    int Res = 0;

    QWidget *pWorkWidget = NULL;
    IUIPlugin *pUiPligin = CommonUtils::getIUIPlugin("customsettingplugin");
    if (NULL != pUiPligin)
    {
        pUiPligin->queryInterface(IUIPlugin::GET_MAIN_WIDGET_OBJECT+1,(void **)&m_pLightCtrlWidget);
    }
    if (NULL != m_pLightCtrlWidget)
    {
        QLayout *pLayout = ui->tab_light->layout();
        pLayout->addWidget(m_pLightCtrlWidget);
        //m_pLightCtrlWidget->setParent(ui->widget_other);
    }

    //m_pMotion = (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
    //RETURN_V_IF(NULL == m_pMotion, false);
    //m_pCard = (ICard *)m_pMotion->GetCardInstance(CONTROLCARD_NAME);
    //RETURN_V_IF(NULL == m_pCard, false);
    //m_pCardConInterp = dynamic_cast<ICardContInterp*>(m_pCard);
    initCardConInterpObj();

    m_bInitRunning = true;
    SetUIEnable(false);
    Res = m_pThreadInit->StartThread(this, &CLineCameraManagerUI::ThreadInitCam, this);
    UpdateCamStatus(QString::fromLocal8Bit("初始化相机中..."));

    if (NULL == m_pCurGraph)
    {
        m_pCurGraph = new CurveGraph();
        //m_pCurGraph->setWindowFlags(Qt::Dialog);

        ui->scrollArea_chart->setWidget(m_pCurGraph);
        m_pCurGraph->setXAxisUpper(8192);
        m_pCurGraph->setXAxisLower(0);
        m_pCurGraph->setYAxisUpper(255);
        m_pCurGraph->setYAxisLower(0);
        m_pCurGraph->setRangeUpper(255);
        m_pCurGraph->setRangeLower(0);


        //QVector<double> keyList;
        //QVector<double> valueList[8];
        //for (int j = 0 ; j < 8192; j++)
        //{
        //    keyList << j;
        //    int ii = 20;
        //    for (int i = 0; i < 8; i++)
        //    {
        //        valueList[i] << (ii + j)%255;
        //        ii += 10;
        //    }
        //}
        //for (int i = 0; i < 8; i++)
        //{
        //    m_pCurGraph->setValues(keyList, valueList[i], i);
        //}
    }
    CreateTreeData();

    return bRet;
}

bool CLineCameraManagerUI::deinit()
{
    if (NULL != m_pLightCtrlWidget)
    {
        m_pLightCtrlWidget->setParent(NULL);
    }
    if (NULL != m_pCurrentCamera)
    {
        if (ui->pushButton_realTimeDisplay->isChecked())
        {
            on_pushButton_realTimeDisplay_clicked(false);
        }
        m_pCurrentCamera->SetTriggerFunction(NULL, NULL);
    }
    if (NULL != m_pCurrentCamera && m_pCurrentCamera->IsOpen())
    {
        m_pCurrentCamera->Close();
    }
    m_bRunning = false;
    m_displayEvent.SetEvent();
    m_realEvent.SetEvent();
    if (NULL != m_pThreadDisplay)
    {
        while(m_pThreadDisplay->IsRunning())
        {
            Sleep(50);
        }
        delete m_pThreadDisplay;
        m_pThreadDisplay = NULL;
    }
    return true;
}

bool CLineCameraManagerUI::showWindow( QWidget* parent, QRect& rect,bool bPopFlag /*= false*/ )
{
    if (parent != NULL)
    {
        this->setParent(parent);

    }
    if (bPopFlag)
    {
        QWidget::setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
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
    }
    show();
    if (!ui->pushButton_realTimeDisplay->isChecked())
    {
        ui->pushButton_realTimeDisplay->setChecked(true);
        on_pushButton_realTimeDisplay_clicked(true);
    }
    return true;
}

bool CLineCameraManagerUI::hideWindow()
{
    if (ui->pushButton_realTimeDisplay->isChecked())
    {
        ui->pushButton_realTimeDisplay->setChecked(false);
        on_pushButton_realTimeDisplay_clicked(false);
    }
    hide();
    return true;
}

void CLineCameraManagerUI::fitSize( const QRect& rect )
{
    resize(rect.width(), rect.height());
}
bool CLineCameraManagerUI::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    qDebug("%s:%d(%s) event is %s, wparam is %d, lparam is %d",
        __FILE__, __LINE__, __FUNCTION__, qPrintable(event), wparam, lparam);
    if (event == "showimage" && 0 != wparam)
    {
        QString strCam = *(QString *)wparam;
        if (0 == strCam.compare(m_pCameraParam.strCameraName.c_str()))
        {
            emit ui->pushButton_get_image->clicked();
        }
    }
    else if(event == "update_cam_ui")
    {
        UpdateUI();
    }
    else if (event == "APP_CLOSE")
    {
        m_waitDlg.hide();
        m_waitDlg.done(0);
    }

    return true;
}

void CLineCameraManagerUI::SetLanguage( QString strlanguage )
{
    QString strLangFilePath = Change::GetUITranslatorPath(PLUGIN_NAME);
    qDebug("%s:%d(%s) LangFile is \"%s\" ",
        __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));


    if (m_translator.load(strLangFilePath))
    {
        qApp->installTranslator(&m_translator);

        ui->retranslateUi(this);
    }
    else
    {
        qWarning("%s:%d(%s) translator load LangFile\"%s\" failed, or is not exist",
            __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));
    }
}

void CLineCameraManagerUI::CallbackGrapFunction( void * obj, unsigned char *buffer, int width, int height)
{
    if (NULL != obj && NULL != buffer)
    {
        CLineCameraManagerUI * pCamera = (CLineCameraManagerUI *)obj;
        if (NULL != pCamera)
        {
            //CISAPUGuard<JMutex> locker(pCamera->m_imageLock);
			pCamera->m_imageLock.Lock();
			if (pCamera->m_ImageList.size() < 16) //buffer满了则直接扔掉
			{
				stImageInfo image;
				image.width = width;
				image.height = height;
				image.pixelR = new unsigned char[width * height];
				memcpy(image.pixelR, buffer, sizeof(unsigned char) * width * height);
				pCamera->m_ImageList.push_back(image);
			}
			pCamera->m_imageLock.Unlock();
			if (pCamera->m_realEvent.IsSiangaled())
			{
				pCamera->m_arriveEvent.SetEvent(); //通知取图像
			}
        }
    }
}

void CLineCameraManagerUI::UpdateUI()
{
    if (NULL != m_pCameraManager)
    {
        GetCameraInParam(m_pCameraParam);
        InitUI();
    }
}

void CLineCameraManagerUI::CreateTreeData()
{
    //设置委托
    CCustomItemDelegate* pCustomTypeDelegate = new CCustomItemDelegate(ui->treeWidget);
    pCustomTypeDelegate->SetParentWidget(ui->treeWidget);
    ui->treeWidget->setItemDelegate(pCustomTypeDelegate);

    //初始化Item
    int index = 0;
    QTreeWidgetItem *pItem = NULL;
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, "name1");
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_NAME);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_LINEEDIT);
    }

    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, "SerialNumber");
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_ID);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_COMBOBOX);
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(8192));
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_WIDTH);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_SPINBOX);
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(100));
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_SNAP_HEIGHT);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_SPINBOX);
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(100));
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_REAL_HEIGHT);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_SPINBOX);
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(20));
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_EXPOSURE);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_SPINBOX);
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(0));
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_GAIN);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_SPINBOX);
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(0));
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_FREQ);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_SPINBOX);
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, QString::number(0));
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_BLOCK_SIZE);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_SPINBOX);
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, "false");
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_TRIGEREXT);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_COMBOBOX);

        //QTreeWidgetItem *pItemChild = NULL;
        //pItemChild = pItem->child(0);
        //if (NULL != pItemChild)
        //{
        //    pItemChild->setText(1, "Internal");
        //    pItemChild->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_TRIGER_TYPE);
        //    pItemChild->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_COMBOBOX);
        //}
        //pItemChild = pItem->child(1);
        //if (NULL != pItemChild)
        //{
        //    pItemChild->setText(1, "Rising edge");
        //    pItemChild->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_TRIGERPOLARITY);
        //    pItemChild->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_COMBOBOX);
        //}
        //pItem->setExpanded(true);
    }
    pItem = ui->treeWidget->topLevelItem(index++);
    if (NULL != pItem)
    {
        pItem->setText(1, "false");
        pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_IS_INVERT);
        pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_COMBOBOX);
    }
    //pItem = ui->treeWidget->topLevelItem(index++);
    //if (NULL != pItem)
    //{
    //    pItem->setText(1, QString::number(3));
    //    pItem->setData(1, Qt::UserRole, CParamWidgetCreate::VS_CAMERA_CHANNEL);
    //    pItem->setData(1, Qt::UserRole+1, CCustomItemDelegate::VS_ATT_SPINBOX);
    //}
}

void CLineCameraManagerUI::InitCamListUI()
{
	ui->comboBox_Camera_Select->blockSignals(true);
    ui->comboBox_Camera_Select->clear();
	int index = -1;
	string invalidName = "";
	int invalidIndex = -1;
    for (int i = 0; i < m_ValidCamList.size(); i++)
    {
        LINE_CAM_PARAM param;
        m_pCameraManager->GetCameraInParam(m_ValidCamList[i], param);
        ui->comboBox_Camera_Select->addItem(QString::fromLocal8Bit(param.strCamDes.c_str()));
		if (param.strCameraName == m_pCameraParam.strCameraName)
		{
			index = i;
		    invalidName = m_pCameraParam.strCameraName;
		}
    }
	if (index < 0 && m_ValidCamList.size() >= 0)
	{
		index = 0;
        LINE_CAM_PARAM param;
        m_pCameraManager->GetCameraInParam(m_ValidCamList[index], param);
		invalidName = param.strCameraName;
	}
	ui->comboBox_Camera_Select->setCurrentIndex(index);
	ui->comboBox_Camera_Select->blockSignals(false);
    UpdateCurrentCamera(invalidName);
}

bool CLineCameraManagerUI::OpenHighSpeedIo(bool bOpen, int iChannel)
{
    bool bRet = false;
    short encoder = 0;   //
    short source = 1;   //电压
    if (m_pCardConInterp)
    {
        if (bOpen){
            encoder = 1 << iChannel;
        } 
        else{
            encoder = 0;
        }
        bRet = m_pCardConInterp->setCompareOutput(encoder, source);
    }
    return bRet;
}

void CLineCameraManagerUI::showEvent( QShowEvent * event )
{
    UpdateUI();
    QWidget::showEvent(event);
}

void CLineCameraManagerUI::hideEvent(QHideEvent * event)
{
    if (ui->pushButton_realTimeDisplay->isChecked())
    {
        ui->pushButton_realTimeDisplay->setChecked(false);
        on_pushButton_realTimeDisplay_clicked(false);
    }
    QWidget::hideEvent(event);
}

void CLineCameraManagerUI::GetCameraInParam( LINE_CAM_PARAM &info )
{
    m_pCameraManager->GetCameraInParam(info.strCameraName, info);
}

void CLineCameraManagerUI::ClearLightCrlNumber()
{
   // lightcontrol::GetInstance()->ClearNumber();
}

void CLineCameraManagerUI::UpdateCamStatus(QString strInfo)
{
    ui->label_status->setText(strInfo);
}
void CLineCameraManagerUI::on_pushButton_test_clicked(bool checked)
{
    int i = 1000;

    startTrigger(i);
//#ifdef CARD_GG
//    do
//    {
//        m_pCardConInterp->SetComparePluse(2, 0, 100);
//        Sleep(1);
//        i--;
//    } while (i > 0);
//#else
//	m_pCardConInterp->SetComparePluse(0, 1, i);
//#endif

    short sss = 0;
    long test[2] = { 0 };
    bool res = false;
	RETURN_IF(NULL == m_pCardConInterp);
    res = m_pCardConInterp->GetCompareStatus(sss, test);

    QString strTile = res ? QString::fromLocal8Bit("成功") : QString::fromLocal8Bit("错误");
    QString strLog = res ? (QString::number(test[0]) + "  " + QString::number(test[1])) : "error";
    QMessageBox::about(this, strTile, strLog);
}

void CLineCameraManagerUI::on_pushButton_calib_clicked(bool checked)
{
    //TBM
    //if (NULL == m_pCalibWidget)
    //{
    //    m_pCalibWidget = new ImageCalibProcess;
    //    m_pCalibWidget->setParent(this);
    //    m_pCalibWidget->setWindowFlags(Qt::Dialog);
    //}
    //m_pCalibWidget->show();
}

void CLineCameraManagerUI::on_pushButton_signal_clicked(bool checked)
{
    int i = 1000;
    startTrigger(1);
//#ifdef CARD_GG
//	m_pCardConInterp->SetComparePluse(2, 0, 100);
//#else
//    m_pCardConInterp->SetComparePluse(0, 1, 1);
//#endif
}

void CLineCameraManagerUI::on_pushButton_calib_pluse_ratio_clicked(bool checked)
{
    //TBM
    //if (NULL == m_pCalibRatioWidget)
    //{
    //    m_pCalibRatioWidget = new CalibRatio;
    //    m_pCalibRatioWidget->setParent(this);
    //    m_pCalibRatioWidget->setWindowFlags(Qt::Dialog);
    //}
    //m_pCalibRatioWidget->show();
}

QWidget * CParamWidgetCreate::CreateEditorWidget(QWidget *pParent, int type)
{
    QWidget *pWidget = NULL;
    switch(type)
    {
    case VS_CAMERA_NAME:
        {
            QLineEdit *comboBoxName = new QLineEdit(pParent);
            //comboBoxName->setFocusPolicy(Qt::StrongFocus);
            //comboBoxName->setEditable(true);
            //comboBoxName->addItems(QStringList()<< "Rising edge" << "Trailing edge");
            pWidget = (QWidget *)comboBoxName;
        }
        break;
    case VS_CAMERA_ID:
        {
            QComboBox *comboBoxCameraID = new QComboBox(pParent);
            comboBoxCameraID->setFocusPolicy(Qt::StrongFocus);
            ILineCameraFactoryManage *pCamFactoryManage = (ILineCameraFactoryManage *)GetSDKInstance("cameramanager", ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE);
            map<string, ILineCamera *> mapCamera;
            if (NULL != pCamFactoryManage)
            {
                pCamFactoryManage->GetValidCameraList(mapCamera);
                map<string, ILineCamera *>::iterator itC = mapCamera.begin();
                for (; itC != mapCamera.end(); ++itC)
                {
                    comboBoxCameraID->addItem(itC->first.c_str());
                }
            }
            //QString strCamDesc = ui->treeWidget->topLevelItem(1)->text(1);
            //comboBoxCameraID->addItems(QStringList()<< "Rising edge" << "Trailing edge");
            pWidget = (QWidget *)comboBoxCameraID;
        }
        break;
    //case VS_CAMERA_CHANNEL:
    //    {
    //        QSpinBox *spinBoxChannel = new QSpinBox(pParent);
    //        spinBoxChannel->setFocusPolicy(Qt::StrongFocus);
    //        spinBoxChannel->setMaximum(8);
    //        spinBoxChannel->setMinimum(1);
    //        spinBoxChannel->setSingleStep(1);
    //        pWidget = (QWidget *)spinBoxChannel;
    //    }
    //    break;
    case VS_CAMERA_WIDTH:
        {
            QSpinBox *spinBoxWidth = new QSpinBox(pParent);
            spinBoxWidth->setFocusPolicy(Qt::StrongFocus);
            spinBoxWidth->setMaximum(999999);
            spinBoxWidth->setMinimum(1);
            spinBoxWidth->setSingleStep(1);
            pWidget = (QWidget *)spinBoxWidth;
        }
        break;
    case VS_CAMERA_SNAP_HEIGHT:
    case VS_CAMERA_REAL_HEIGHT:
        {
            QSpinBox *spinBoxHeight = new QSpinBox(pParent);
            spinBoxHeight->setFocusPolicy(Qt::StrongFocus);
            spinBoxHeight->setMaximum(999999);
            spinBoxHeight->setMinimum(1);
            spinBoxHeight->setSingleStep(1);
            pWidget = (QWidget *)spinBoxHeight;
        }
        break;
    case VS_CAMERA_EXPOSURE:
        {
            QSpinBox *spinBoxExp = new QSpinBox(pParent);
            spinBoxExp->setFocusPolicy(Qt::StrongFocus);
            spinBoxExp->setMaximum(999999);
            spinBoxExp->setMinimum(1);
            spinBoxExp->setSingleStep(1);
            pWidget = (QWidget *)spinBoxExp;
        }
        break;
    case VS_CAMERA_GAIN:
        {
            QSpinBox *spinBoxGain = new QSpinBox(pParent);
            spinBoxGain->setFocusPolicy(Qt::StrongFocus);
            spinBoxGain->setMaximum(999999);
            spinBoxGain->setMinimum(0);
            spinBoxGain->setSingleStep(1);
            pWidget = (QWidget *)spinBoxGain;
        }
        break;
    case VS_CAMERA_FREQ:
    case VS_CAMERA_BLOCK_SIZE:
        {
            QSpinBox *spinBoxFreq = new QSpinBox(pParent);
            spinBoxFreq->setFocusPolicy(Qt::StrongFocus);
            spinBoxFreq->setMaximum(99999);
            spinBoxFreq->setMinimum(1);
            spinBoxFreq->setSingleStep(1);
            pWidget = (QWidget *)spinBoxFreq;
        }
        break;
    case VS_CAMERA_TRIGEREXT:
    case VS_CAMERA_IS_INVERT:
        {
            QComboBox *comboBoxExt = new QComboBox(pParent);
            comboBoxExt->addItems(QStringList()<< "false" << "true");
            comboBoxExt->setFocusPolicy(Qt::StrongFocus);
            pWidget = (QWidget *)comboBoxExt;
        }
        break;
    case VS_CAMERA_TRIGER_TYPE:
        {
            QComboBox *comboBoxTType = new QComboBox(pParent);
            comboBoxTType->addItems(QStringList()<< "Internal" << "External" << "Encoder");
            comboBoxTType->setFocusPolicy(Qt::StrongFocus);
            pWidget = (QWidget *)comboBoxTType;
        }
        break;
    case VS_CAMERA_TRIGERPOLARITY:
        {
            QComboBox *comboBoxPolarity = new QComboBox(pParent);
            comboBoxPolarity->addItems(QStringList()<< "Rising edge" << "Trailing edge");
            comboBoxPolarity->setFocusPolicy(Qt::StrongFocus);
            pWidget = (QWidget *)comboBoxPolarity;
        }
        break;
    }
    return pWidget;
}

bool CLineCameraManagerUI::initCardConInterpObj()
{
    bool bRet = false;
    do
    {
        if (NULL == m_pCardConInterp)
        {
            m_pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
            BREAK_IF(NULL == m_pMotion);
            m_pCard = (ICard *)m_pMotion->GetCardInstance("card");
            BREAK_IF(NULL == m_pCard);
            m_pCardConInterp = dynamic_cast<ICardContInterp*>(m_pCard);
            BREAK_IF(NULL == m_pCardConInterp);
            iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion", iMotionCfg::IID_MOTION_CONFIG);
            BREAK_IF(NULL == pMotionCfg);
            StCardInfo cardInfo;
            pMotionCfg->getCardInfo("card", cardInfo);
            m_bIsGGCard = false;
            if (0 == strcmp(cardInfo.cardType, "cardgg"))
            {
                m_bIsGGCard = true;
            }
        }
        bRet = true;
    } while (0);
    return bRet;
}

bool CLineCameraManagerUI::startTrigger(int iNumbs/* = -1*/)
{
    bool bRet = false;
    RETURN_V_IF(!initCardConInterpObj(), false);
    RETURN_V_IF(NULL == m_pCardConInterp, false);
    if (m_bIsGGCard)
    {
        if (NULL == m_pTriggerThread)
        {
            m_pTriggerThread = new JThreadTP<CLineCameraManagerUI, void *, void>;
        }
        if (!m_pTriggerThread->IsRunning())
        {
            m_bTriggerRunning = true;
            m_pTriggerThread->StartThread(this, &CLineCameraManagerUI::ThreadTriger, (void *)iNumbs);
        }
        bRet = true;
    }
    else
    {
        if (iNumbs < 0)
        {
            bRet = m_pCardConInterp->SetComparePluse(0, 0, 0);
        }
        else
        {
            bRet = m_pCardConInterp->SetComparePluse(0, 1, 1);
        }
    }
    return bRet;
}

bool CLineCameraManagerUI::stopTrigger()
{
    if (m_bIsGGCard)
    {
        m_bTriggerRunning = false;
    }

    RETURN_V_IF(!initCardConInterpObj(), false);
    RETURN_V_IF(NULL == m_pCardConInterp, false);
    return m_pCardConInterp->stopComparePos(0);
}

