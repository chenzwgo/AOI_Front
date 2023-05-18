#include "customui/motionctldlg.h"
#include "ui_motionctldlg.h"
#include "proxymanager.h"

#include "motion/imotion.h"
#include "station/istation.h"
#include <QStringList>
#include <QTimer>
#include <QGridLayout>
#include <QMessageBox>
#include <QSettings>
#include "customui/movingdlg.h"
#include "addpointdialog.h"
#include "rtdefine.h"

#define PLATE1_MARK       "1"
#define PLATE2_MARK       "2"
#define PLATE3_MARK       "3"
#define PLATE4_MARK       "4"

#define STATIONAXISDIR_PATH "plugins/motion/stationaxisdir.ini"

MotionCtlDlg::MotionCtlDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotionCtlDlg),
	m_pStation(NULL),
    m_pCustomWidget(NULL)
{
    ui->setupUi(this);
	initUI();
}

MotionCtlDlg::MotionCtlDlg( const QString &stationName, QWidget *parent /*= 0*/ ) :
    QDialog(parent),
	ui(new Ui::MotionCtlDlg),
	m_pStation(NULL),
    m_pCustomWidget(NULL)
{
	ui->setupUi(this);
	initUI();
	setStation(stationName);
}

MotionCtlDlg::~MotionCtlDlg()
{
    delete ui;
}

void MotionCtlDlg::initUI()
{
	this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::Dialog);
    ui->groupBox_point->setVisible(false);

    // 自定义控件
    m_pCustomWidget = new CustomWidget(this);

	connect(ui->pushButtonXAdd, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonXSub, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonYAdd, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonYSub, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonZAdd, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonZSub, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonUAdd, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonUSub, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonVAdd, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonVSub, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonWAdd, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );
	connect(ui->pushButtonWSub, SIGNAL(clicked()), this, SLOT(slotMoveAxis()) );

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slot_timeout()) );

    connect(ui->tableWidgetpoint,SIGNAL(cellChanged(int,int)),this,SLOT(slotModPoint(int,int)));

    initMoveJogBtn();
}

void MotionCtlDlg::initAxisUI( int axiscount )
{
	//w轴
	ui->label_W->setVisible(axiscount>=6);
	ui->label_title_w_2->setVisible(axiscount>=6);
	ui->label_cur_w->setVisible(axiscount>=6);
    ui->doubleSpinBox_w->setVisible(axiscount>=6);
	ui->pushButtonWSub->setVisible(axiscount>=6);
	ui->pushButtonWAdd->setVisible(axiscount>=6);

	//v轴
	ui->label_V->setVisible(axiscount>=5);
	ui->label_title_v_2->setVisible(axiscount>=5);
	ui->label_cur_v->setVisible(axiscount>=5);
    ui->doubleSpinBox_v->setVisible(axiscount>=5);
	ui->pushButtonVAdd->setVisible(axiscount>=5);
	ui->pushButtonVSub->setVisible(axiscount>=5);

	//u轴
	ui->label_U->setVisible(axiscount>=4);
	ui->label_title_u_2->setVisible(axiscount>=4);
	ui->label_cur_u->setVisible(axiscount>=4);
    ui->doubleSpinBox_u->setVisible(axiscount>=4);
	ui->pushButtonUSub->setVisible(axiscount>=4);
	ui->pushButtonUAdd->setVisible(axiscount>=4);

	//z轴
	ui->label_Z->setVisible(axiscount>=3);
	ui->label_title_z_2->setVisible(axiscount>=3);
	ui->label_cur_z->setVisible(axiscount>=3);
    ui->doubleSpinBox_z->setVisible(axiscount>=3);
	ui->pushButtonZAdd->setVisible(axiscount>=3);
	ui->pushButtonZSub->setVisible(axiscount>=3);

	ui->label_Y->setVisible(axiscount>=2);
	ui->label_title_y_2->setVisible(axiscount>=2);
	ui->label_cur_y->setVisible(axiscount>=2);
    ui->doubleSpinBox_y->setVisible(axiscount>=2);
	ui->pushButtonYAdd->setVisible(axiscount>=2);
	ui->pushButtonYSub->setVisible(axiscount>=2);

    // 轴状态显示
    ui->label_axisName_1->setVisible(axiscount>=1);
    ui->label_axisName_2->setVisible(axiscount>=2);
    ui->label_axisName_3->setVisible(axiscount>=3);
    ui->label_axisName_4->setVisible(axiscount>=4);
    ui->label_axisName_5->setVisible(axiscount>=5);
    ui->label_axisName_6->setVisible(axiscount>=6);

    ui->label_status_n_1->setVisible(axiscount>=1);
    ui->label_status_n_2->setVisible(axiscount>=2);
    ui->label_status_n_3->setVisible(axiscount>=3);
    ui->label_status_n_4->setVisible(axiscount>=4);
    ui->label_status_n_5->setVisible(axiscount>=5);
    ui->label_status_n_6->setVisible(axiscount>=6);

    ui->label_status_p_1->setVisible(axiscount>=1);
    ui->label_status_p_2->setVisible(axiscount>=2);
    ui->label_status_p_3->setVisible(axiscount>=3);
    ui->label_status_p_4->setVisible(axiscount>=4);
    ui->label_status_p_5->setVisible(axiscount>=5);
    ui->label_status_p_6->setVisible(axiscount>=6);

    ui->label_status_home_1->setVisible(axiscount>=1);
    ui->label_status_home_2->setVisible(axiscount>=2);
    ui->label_status_home_3->setVisible(axiscount>=3);
    ui->label_status_home_4->setVisible(axiscount>=4);
    ui->label_status_home_5->setVisible(axiscount>=5);
    ui->label_status_home_6->setVisible(axiscount>=6);

    ui->label_status_alarm_1->setVisible(axiscount>=1);
    ui->label_status_alarm_2->setVisible(axiscount>=2);
    ui->label_status_alarm_3->setVisible(axiscount>=3);
    ui->label_status_alarm_4->setVisible(axiscount>=4);
    ui->label_status_alarm_5->setVisible(axiscount>=5);
    ui->label_status_alarm_6->setVisible(axiscount>=6);

    ui->label_status_run_1->setVisible(axiscount>=1);
    ui->label_status_run_2->setVisible(axiscount>=2);
    ui->label_status_run_3->setVisible(axiscount>=3);
    ui->label_status_run_4->setVisible(axiscount>=4);
    ui->label_status_run_5->setVisible(axiscount>=5);
    ui->label_status_run_6->setVisible(axiscount>=6);

    ui->label_status_on_1->setVisible(axiscount>=1);
    ui->label_status_on_2->setVisible(axiscount>=2);
    ui->label_status_on_3->setVisible(axiscount>=3);
    ui->label_status_on_4->setVisible(axiscount>=4);
    ui->label_status_on_5->setVisible(axiscount>=5);
    ui->label_status_on_6->setVisible(axiscount>=6);

    ui->pushButton_axis_on_1->setVisible(axiscount>=1);
    ui->pushButton_axis_on_2->setVisible(axiscount>=2);
    ui->pushButton_axis_on_3->setVisible(axiscount>=3);
    ui->pushButton_axis_on_4->setVisible(axiscount>=4);
    ui->pushButton_axis_on_5->setVisible(axiscount>=5);
    ui->pushButton_axis_on_6->setVisible(axiscount>=6);

    ui->pushButton_axis_clear_alarm_1->setVisible(axiscount>=1);
    ui->pushButton_axis_clear_alarm_2->setVisible(axiscount>=2);
    ui->pushButton_axis_clear_alarm_3->setVisible(axiscount>=3);
    ui->pushButton_axis_clear_alarm_4->setVisible(axiscount>=4);
    ui->pushButton_axis_clear_alarm_5->setVisible(axiscount>=5);
    ui->pushButton_axis_clear_alarm_6->setVisible(axiscount>=6);

    ui->pushButton_home_1->setVisible(axiscount>=1);
    ui->pushButton_home_2->setVisible(axiscount>=2);
    ui->pushButton_home_3->setVisible(axiscount>=3);
    ui->pushButton_home_4->setVisible(axiscount>=4);
    ui->pushButton_home_5->setVisible(axiscount>=5);
    ui->pushButton_home_6->setVisible(axiscount>=6);

}

bool MotionCtlDlg::setStation( const QString &station, const QString &pointMask )
{
	memset(m_axisDir, 0, sizeof(m_axisDir));

	iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion",iMotionCfg::IID_MOTION_CONFIG);
	if(!pMotionCfg)
	{
		return false;
	}

	if(SYS_ERR_OK != pMotionCfg->getStationInfo(station.toLatin1().data(), m_stationInfo))
	{
		return false;
	}

    ui->label_stationName->setText(QString::fromUtf8(m_stationInfo.desc));
    ui->label_stationName->setToolTip(QString::fromUtf8(m_stationInfo.name));
    //m_axisCount = stationInfo.axiscnt;
	initAxisUI(m_stationInfo.axiscnt);

	iMotion *pMotion = (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
	if(!pMotion)
	{
		return false;
	}

	m_pStation = pMotion->GetStationInstance(station.toLatin1().data());
	if(!m_pStation)
	{
		return false;
	}

	readAxisDir();
	initAxisMap();

    updateStationSpeed();
    updatePointUI();
	
	double ddistance = 0.1;
	ui->radioButtonsmall->setChecked(true);
	ui->doubleSpinBox_x->setValue(ddistance);
	ui->doubleSpinBox_y->setValue(ddistance);
	ui->doubleSpinBox_z->setValue(ddistance);
	ui->doubleSpinBox_u->setValue(ddistance);
	ui->doubleSpinBox_v->setValue(ddistance);
	ui->doubleSpinBox_w->setValue(ddistance);

    ui->tableWidgetpoint->resizeColumnsToContents();

    // 轴信息设置,tooltip描述显示
    StAxisParam axisParam;
    if(SYS_ERR_OK == pMotionCfg->getAxisInfo(m_stationInfo.axisname[0], axisParam))
    {

        ui->pushButtonXAdd->setToolTip(QString::fromUtf8(axisParam.AxisDesc));
        ui->pushButtonXSub->setToolTip(QString::fromUtf8(axisParam.AxisDesc));

        pMotionCfg->getAxisInfo(m_stationInfo.axisname[1], axisParam);
        ui->pushButtonYAdd->setToolTip(QString::fromUtf8(axisParam.AxisDesc));
        ui->pushButtonYSub->setToolTip(QString::fromUtf8(axisParam.AxisDesc));

        pMotionCfg->getAxisInfo(m_stationInfo.axisname[2], axisParam);
        ui->pushButtonZAdd->setToolTip(QString::fromUtf8(axisParam.AxisDesc));
        ui->pushButtonZSub->setToolTip(QString::fromUtf8(axisParam.AxisDesc));

        pMotionCfg->getAxisInfo(m_stationInfo.axisname[3], axisParam);
        ui->pushButtonUAdd->setToolTip(QString::fromUtf8(axisParam.AxisDesc));
        ui->pushButtonUSub->setToolTip(QString::fromUtf8(axisParam.AxisDesc));

        pMotionCfg->getAxisInfo(m_stationInfo.axisname[4], axisParam);
        ui->pushButtonVAdd->setToolTip(QString::fromUtf8(axisParam.AxisDesc));
        ui->pushButtonVSub->setToolTip(QString::fromUtf8(axisParam.AxisDesc));

        pMotionCfg->getAxisInfo(m_stationInfo.axisname[5], axisParam);
        ui->pushButtonWAdd->setToolTip(QString::fromUtf8(axisParam.AxisDesc));
        ui->pushButtonWSub->setToolTip(QString::fromUtf8(axisParam.AxisDesc));
    }

    QTimer::singleShot(1,this, SLOT(slot_resize()));
	return true;
}

void MotionCtlDlg::slotMoveAxis( void )
{
    RETURN_IF(NULL == m_pStation);
    if(STATION_IDLE != m_pStation->GetStatus())
    {
        //QMessageBox::warning(this, "warn", "工站正在运动中,请先停止.");
        return;
    }

	QPushButton *pBtn = (QPushButton*)sender();
    if (m_mapAxisMove.end() == m_mapAxisMove.find(pBtn))
    {
        return;
    }

	double offset = m_mapAxisMove[pBtn].lineEditOffset->text().toDouble();
	offset = offset*m_mapAxisMove[pBtn].dir;
	
	int nRet = m_pStation->AxisMoveOffset(m_mapAxisMove[pBtn].axis,offset);
}

void MotionCtlDlg::refreshCurrentPos( void )
{
	if ( m_pStation )
	{
		StPos pos;
		if( SYS_ERR_OK == m_pStation->GetCurrentPos(pos))
		{
			ui->label_cur_x->setText( QString::number( pos.x, 'f', 3 ) );
			ui->label_cur_y->setText( QString::number( pos.y, 'f', 3 ) );
			ui->label_cur_z->setText( QString::number( pos.z, 'f', 3 ) );
			ui->label_cur_u->setText( QString::number( pos.u, 'f', 3 ) );
			ui->label_cur_v->setText( QString::number( pos.v, 'f', 3 ) );
			ui->label_cur_w->setText( QString::number( pos.w, 'f', 3 ) );
		} 
		else
		{
            ui->label_cur_x->setText( "" );
            ui->label_cur_y->setText( "" );
            ui->label_cur_z->setText( "" );
            ui->label_cur_u->setText( "" );
            ui->label_cur_v->setText( "" );
            ui->label_cur_w->setText( "" );
		}
	}
}

void MotionCtlDlg::readAxisDir()
{
    QSettings sett(STATIONAXISDIR_PATH,QSettings::IniFormat);
    sett.beginGroup(m_stationInfo.name);
    bool bDirRevX = sett.value("ReverseX",false).toBool();
    bool bDirRevY = sett.value("ReverseY",false).toBool();
    bool bDirRevZ = sett.value("ReverseZ",false).toBool();
    bool bDirRevU = sett.value("ReverseU",false).toBool();
    bool bDirRevV = sett.value("ReverseV",false).toBool();
    bool bDirRevW = sett.value("ReverseW",false).toBool();

    QString iconTypeX = sett.value("iconTypeX","left_right").toString();
    QString iconTypeY = sett.value("iconTypeY","front_back").toString();
    QString iconTypeZ = sett.value("iconTypeZ","up_down").toString();
    QString iconTypeU = sett.value("iconTypeU","clockwise_anticlockwise").toString();
    QString iconTypeV = sett.value("iconTypeV","clockwise_anticlockwise").toString();
    QString iconTypeW = sett.value("iconTypeW","clockwise_anticlockwise").toString();

    QString iconTipX = sett.value("iconTipX","X").toString();
    QString iconTipY = sett.value("iconTipY","Y").toString();
    QString iconTipZ = sett.value("iconTipZ","Z").toString();
    QString iconTipU = sett.value("iconTipU","U").toString();
    QString iconTipV = sett.value("iconTipV","V").toString();
    QString iconTipW = sett.value("iconTipW","W").toString();
    sett.endGroup();

    bool axisDir[6] = {bDirRevX,bDirRevY,bDirRevZ,bDirRevU,bDirRevV,bDirRevW};
    memcpy(m_axisDir, axisDir, sizeof(axisDir));

    StAxisIcon stAxisIconXAdd(iconTypeX.split('_').at(0), iconTipX, !bDirRevX);
    StAxisIcon stAxisIconXSub(iconTypeX.split('_').at(1), iconTipX, bDirRevX);
    StAxisIcon stAxisIconYAdd(iconTypeY.split('_').at(0), iconTipY, !bDirRevY);
    StAxisIcon stAxisIconYSub(iconTypeY.split('_').at(1), iconTipY, bDirRevY);
    StAxisIcon stAxisIconZAdd(iconTypeZ.split('_').at(0), iconTipZ, !bDirRevZ);
    StAxisIcon stAxisIconZSub(iconTypeZ.split('_').at(1), iconTipZ, bDirRevZ);
    StAxisIcon stAxisIconUAdd(iconTypeU.split('_').at(0), iconTipU, !bDirRevU);
    StAxisIcon stAxisIconUSub(iconTypeU.split('_').at(1), iconTipU, bDirRevU);
    StAxisIcon stAxisIconVAdd(iconTypeV.split('_').at(0), iconTipV, !bDirRevV);
    StAxisIcon stAxisIconVSub(iconTypeV.split('_').at(1), iconTipV, bDirRevV);
    StAxisIcon stAxisIconWAdd(iconTypeW.split('_').at(0), iconTipW, !bDirRevW);
    StAxisIcon stAxisIconWSub(iconTypeW.split('_').at(1), iconTipW, bDirRevW);
    
    ui->pushButtonXAdd->setIconInfo(stAxisIconXAdd);
    ui->pushButtonXSub->setIconInfo(stAxisIconXSub);
    ui->pushButtonYAdd->setIconInfo(stAxisIconYAdd);
    ui->pushButtonYSub->setIconInfo(stAxisIconYSub);
    ui->pushButtonZAdd->setIconInfo(stAxisIconZAdd);
    ui->pushButtonZSub->setIconInfo(stAxisIconZSub);
    ui->pushButtonUAdd->setIconInfo(stAxisIconUAdd);
    ui->pushButtonUSub->setIconInfo(stAxisIconUSub);
    ui->pushButtonVAdd->setIconInfo(stAxisIconVAdd);
    ui->pushButtonVSub->setIconInfo(stAxisIconVSub);
    ui->pushButtonWAdd->setIconInfo(stAxisIconWAdd);
    ui->pushButtonWSub->setIconInfo(stAxisIconWSub);

    QStringList headerList;
    headerList<<QString::fromLocal8Bit("编号")<<QString::fromLocal8Bit("名称")<<QString::fromLocal8Bit("描述") \
        <<iconTipX<<iconTipY<<iconTipZ<<iconTipU<<iconTipV<<iconTipW;
    ui->tableWidgetpoint->setHorizontalHeaderLabels(headerList);
}

void MotionCtlDlg::initAxisMap()
{
	m_mapAxisMove[ui->pushButtonXAdd] = StAxisMove(ui->doubleSpinBox_x, 0, m_axisDir[0]);
	m_mapAxisMove[ui->pushButtonXSub] = StAxisMove(ui->doubleSpinBox_x, 0, !m_axisDir[0]);
	m_mapAxisMove[ui->pushButtonYAdd] = StAxisMove(ui->doubleSpinBox_y, 1, m_axisDir[1]);
	m_mapAxisMove[ui->pushButtonYSub] = StAxisMove(ui->doubleSpinBox_y, 1, !m_axisDir[1]);
	m_mapAxisMove[ui->pushButtonZAdd] = StAxisMove(ui->doubleSpinBox_z, 2, m_axisDir[2]);
	m_mapAxisMove[ui->pushButtonZSub] = StAxisMove(ui->doubleSpinBox_z, 2, !m_axisDir[2]);
	m_mapAxisMove[ui->pushButtonUAdd] = StAxisMove(ui->doubleSpinBox_u, 3, m_axisDir[3]);
	m_mapAxisMove[ui->pushButtonUSub] = StAxisMove(ui->doubleSpinBox_u, 3, !m_axisDir[3]);
	m_mapAxisMove[ui->pushButtonVAdd] = StAxisMove(ui->doubleSpinBox_v, 4, m_axisDir[4]);
	m_mapAxisMove[ui->pushButtonVSub] = StAxisMove(ui->doubleSpinBox_v, 4, !m_axisDir[4]);
	m_mapAxisMove[ui->pushButtonWAdd] = StAxisMove(ui->doubleSpinBox_w, 5, m_axisDir[5]);
	m_mapAxisMove[ui->pushButtonWSub] = StAxisMove(ui->doubleSpinBox_w, 5, !m_axisDir[5]);
}

void MotionCtlDlg::slot_resize( void )
{
    resize(this->minimumSizeHint());
}

void MotionCtlDlg::slot_timeout()
{
    if (this->isVisible())
    {
        if (NULL != m_pCustomWidget)
        {
            m_pCustomWidget->updateAllWidget();
        }

        refreshAxisStatu();
    }
    else
    {
        m_timer.stop();
    }

	refreshCurrentPos();
}

void MotionCtlDlg::on_pushButton_station_stop_clicked()
{
    if (NULL == m_pStation)
    {
        return;
    }

    int ires = -1;
    ires = m_pStation->Stop();

    if (SYS_ERR_OK != ires)
    {
        QMessageBox::warning(this,"warn",QString("Go home failed,error:%1").arg(ires));
    }
}

void MotionCtlDlg::showEvent( QShowEvent * )
{
    QTimer::singleShot(1,this, SLOT(slot_resize()));
	m_timer.start(500);
}

void MotionCtlDlg::hideEvent( QHideEvent * )
{
	m_timer.stop();
}

void MotionCtlDlg::refreshAxisStatu()
{
    if (NULL == m_pStation)
    {
        return;
    }
    if (STATION_UNINIT == m_pStation->GetStatus())
    {
        return;
    }

    EnStationStatus status = m_pStation->GetStatus();
    switch (status) {
    case STATION_UNINIT:
        ui->runStatus->setText(QString::fromLocal8Bit("未初始化"));
        break;
    case STATION_IDLE:
        ui->runStatus->setText(QString::fromLocal8Bit("就绪"));
        break;
    case STATION_MOVING:
        ui->runStatus->setText(QString::fromLocal8Bit("运动中"));
        break;
    case STATION_HOMING:
        ui->runStatus->setText(QString::fromLocal8Bit("回原中"));
        break;
    case STATION_MULTIMOVE:
        ui->runStatus->setText(QString::fromLocal8Bit("多点运动中"));
        break;
    case STATION_CONTIINTERP:
        ui->runStatus->setText(QString::fromLocal8Bit("连续插补中"));
        break;
    default:
        break;
    }

    int axisCount = m_stationInfo.axiscnt;

    iMotion *pMotion = (iMotion *)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);;
    if (pMotion)
    {
        for (int i=1;i<=6;i++)
        {
            IAxis* pAxis = pMotion->GetAxisInstance(m_stationInfo.axisname[i-1]);
            if (pAxis)
            {
                QString label_p = QString("label_status_p_%1").arg(i);
                IOLabel *pO = this->findChild<IOLabel*>(label_p);
                pO->setState(pAxis->IsLimitP());

                QString label_n = QString("label_status_n_%1").arg(i);
                pO = this->findChild<IOLabel*>(label_n);
                pO->setState(pAxis->IsLimitN());

                QString label_home = QString("label_status_home_%1").arg(i);
                pO = this->findChild<IOLabel*>(label_home);
                pO->setState(pAxis->IsHomeSig());

                QString label_on = QString("label_status_on_%1").arg(i);
                pO = this->findChild<IOLabel*>(label_on);
                bool bOn = pAxis->IsOn();
                pO->setState(bOn);

                QString label_alarm = QString("label_status_alarm_%1").arg(i);
                pO = this->findChild<IOLabel*>(label_alarm);
                pO->setState(pAxis->IsDrvAlarm());

                QString label_run = QString("label_status_run_%1").arg(i);
                pO = this->findChild<IOLabel*>(label_run);
                pO->setState(pAxis->IsMoving());

                QString button_on = QString("pushButton_axis_on_%1").arg(i);
                QPushButton *pbtn = this->findChild<QPushButton*>(button_on);
                if (pbtn)
                {
                    if (bOn)
                    {
                        //pbtn->setText("关使能");
                    }
                    else
                    {
                        //pbtn->setText("开使能");
                    }
                }
            }
        }
    }
}

void MotionCtlDlg::updatePointUI()
{
    //get lastest point information
    if(NULL == m_pStation)
    {
        return;
    }

    m_pointInfoList.clear();

    const map<int, StPoint> *allPointInfo = m_pStation->GetAllPoint();

    m_pointNameList.clear();
    QStringList pointDefineName;


    for(int i = 0; i < 1000; i++)
    {
        if(NULL != allPointInfo)
        {
            map<int, StPoint>::const_iterator iter = allPointInfo->find(i);
            if(iter!=allPointInfo->end())
            {
                m_pointNameList.append("P" + QString::number(i) + " - " + allPointInfo->at(i).name);
                m_pointInfoList.append(allPointInfo->at(i));
            }
            else
            {
                //m_pointNameList.append("P" + QString::number(i) + " - (未定义)");
            }
        }
        else
        {
            //m_pointNameList.append("P" + QString::number(i) + " - (未定义)");
        }

    }
    ui->comboBox_point->clear();
    ui->comboBox_point->addItems(m_pointNameList);
    //清内存
    ui->tableWidgetpoint->clearContents();
    m_isReadFile = true;
    ui->tableWidgetpoint->setRowCount(m_pointInfoList.count());
    //添加点信息
    for(int i = 0; i < m_pointInfoList.count();i++)
    {
        StPoint *tmp = &m_pointInfoList[i];

        QTableWidgetItem *item = new QTableWidgetItem(QString::number(tmp->index));
        //设置此项为不可编辑的
        if(item)
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));

        ui->tableWidgetpoint->setItem(i, 0, item);
        ui->tableWidgetpoint->setItem(i, 1, new QTableWidgetItem(tmp->name));
        ui->tableWidgetpoint->setItem(i, 2, new QTableWidgetItem(QString::fromUtf8(tmp->desc)));
        double pt[6] = {tmp->pos.x,tmp->pos.y,tmp->pos.z,tmp->pos.u,tmp->pos.v,tmp->pos.w};
        for (int j=0; j<6; ++j)
        {
            if ( j<m_stationInfo.axiscnt )
            {
                ui->tableWidgetpoint->setItem(i, 3+j, new QTableWidgetItem(QString::number(pt[j], 'f', 3)));
            }
            else
            {
                QTableWidgetItem *subItem = new QTableWidgetItem();
                subItem->setFlags(subItem->flags() & (~Qt::ItemIsEditable));
                ui->tableWidgetpoint->setItem(i, 3+j, subItem);
            }
        }
    }
    m_isReadFile = false;
}

void MotionCtlDlg::on_pushButton_movePoint_clicked()
{
    if(QMessageBox::Ok != QMessageBox::information(this, "warn", QString::fromLocal8Bit("请确认是否跑点？"), QMessageBox::Ok | QMessageBox::Cancel))
    {
        return;
    }

    bool bRes = false;
    int pointIndex = ui->comboBox_point->currentIndex();
    if ( pointIndex >= 0 && pointIndex < m_pointInfoList.size() )
    {
        MovingDlg dlg(m_stationInfo.name);
        dlg.MovePoint(pointIndex);
        dlg.exec();
    }
}

void MotionCtlDlg::on_pushButton_AddPoint_clicked()
{
    if (NULL == m_pStation)
    {
        return;
    }

    AddPointDialog addPointDlg(m_pStation);
    if (QDialog::Accepted == addPointDlg.exec())
    {
        int pointIndex = addPointDlg.getPointIndex();
        QString point      = addPointDlg.getPointName();
        StPoint stpoint;
        stpoint.index = pointIndex;
        sprintf(stpoint.name,point.toStdString().c_str());
        ERROR_CODE eCode = m_pStation->TeachPoint(stpoint);
        if (SYS_ERR_OK != eCode )
        {
            QMessageBox::warning(this,"warn",QString::fromLocal8Bit("TeachPoint failed,error:%1").arg(eCode));
            return;
        }

        updatePointUI();
        QString name = "P" + QString::number(pointIndex) + " - " + point;
        ui->comboBox_point->setCurrentIndex(ui->comboBox_point->findText(name));
    }
}

void MotionCtlDlg::on_pushbutton_deletepoint_clicked()
{
    int index= -1;//当前点的索引
    int iRes = 0;
    int ist = -1;
    QString strMsg;
    char szBuf[128] = {0};
    if (ui->comboBox_point->currentText().isEmpty())
    {
        QMessageBox::warning(this,"warn",QString::fromLocal8Bit("Please select a point!"));
        return ;
    }

    sprintf(szBuf, "确定删除 %s 点位信息？",m_pointNameList[ui->comboBox_point->currentIndex()].toStdString().c_str());
    strMsg = szBuf;
    if (QMessageBox::Yes == QMessageBox::information(this, QString::fromLocal8Bit("提示"),strMsg, QMessageBox::Yes | QMessageBox::No ))
    {
        QString point = ui->comboBox_point->currentText();
        int indexTmp = point.indexOf("-");
        index = point.mid(1,indexTmp-1).toInt();
        iRes = m_pStation->DeletePoint(index);
        if (iRes != SYS_ERR_OK)
        {
            QMessageBox::warning(this,"warn",QString("Delete point failed,error:%1").arg(iRes));
            return;
        }

        //更新显示
        updatePointUI();
    }
}

void MotionCtlDlg::on_pushButton_teachPoint_clicked()
{
    if(NULL == m_pStation)
    {
        return;
    }

    if(QMessageBox::Ok != QMessageBox::information(this, "warn", QString::fromLocal8Bit("请确认是否示教该点?"), QMessageBox::Ok | QMessageBox::Cancel))
    {
        return;
    }

    StPos pos;
    if( SYS_ERR_OK != m_pStation->GetCurrentPos(pos))
    {
        return;
    }

    QString currentPoint = ui->comboBox_point->currentText();
    //判断当前点是否已经定义
    if (currentPoint.isEmpty()) return;

    int index = ui->comboBox_point->currentIndex();
    m_pointInfoList[index].pos = pos;
    bool bRes = (SYS_ERR_OK == m_pStation->TeachPoint( m_pointInfoList[index] ));

    updatePointUI();
    ui->comboBox_point->setCurrentIndex(index);

    if ( bRes )
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("\n点位保存成功    "), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("\n点位保存失败    "), QMessageBox::Ok);
    }
}

void MotionCtlDlg::on_pushButton_station_home_clicked()
{
    if(QMessageBox::Ok != QMessageBox::information(this, "warn", QString::fromLocal8Bit("请确认是否进行工站回原?"), QMessageBox::Ok | QMessageBox::Cancel))
    {
        return;
    }

    if ( m_pStation )
    {
        MovingDlg movingDlg(m_stationInfo.name);
        movingDlg.MoveHome();
        movingDlg.exec();
    }
}

void MotionCtlDlg::on_pushButton_axis_on_1_clicked()
{
    axisEnable(STATION_X);
}

void MotionCtlDlg::on_pushButton_axis_on_2_clicked()
{
    axisEnable(STATION_Y);
}

void MotionCtlDlg::on_pushButton_axis_on_3_clicked()
{
    axisEnable(STATION_Z);
}

void MotionCtlDlg::on_pushButton_axis_on_4_clicked()
{
    axisEnable(STATION_U);
}

void MotionCtlDlg::on_pushButton_axis_on_5_clicked()
{
    axisEnable(STATION_V);
}

void MotionCtlDlg::on_pushButton_axis_on_6_clicked()
{
    axisEnable(STATION_W);
}

void MotionCtlDlg::on_pushButton_axis_clear_alarm_1_clicked()
{
    clearAlarm(STATION_X);
}
void MotionCtlDlg::on_pushButton_axis_clear_alarm_2_clicked()
{
    clearAlarm(STATION_Y);
}
void MotionCtlDlg::on_pushButton_axis_clear_alarm_3_clicked()
{
    clearAlarm(STATION_Z);
}
void MotionCtlDlg::on_pushButton_axis_clear_alarm_4_clicked()
{
    clearAlarm(STATION_U);
}
void MotionCtlDlg::on_pushButton_axis_clear_alarm_5_clicked()
{
    clearAlarm(STATION_V);
}
void MotionCtlDlg::on_pushButton_axis_clear_alarm_6_clicked()
{
    clearAlarm(STATION_W);
}

void MotionCtlDlg::on_pushButton_home_1_clicked()
{
    axisHome(STATION_X);
}
void MotionCtlDlg::on_pushButton_home_2_clicked()
{
    axisHome(STATION_Y);
}
void MotionCtlDlg::on_pushButton_home_3_clicked()
{
    axisHome(STATION_Z);
}
void MotionCtlDlg::on_pushButton_home_4_clicked()
{
    axisHome(STATION_U);
}
void MotionCtlDlg::on_pushButton_home_5_clicked()
{
    axisHome(STATION_V);
}
void MotionCtlDlg::on_pushButton_home_6_clicked()
{
    axisHome(STATION_W);
}

void MotionCtlDlg::axisEnable( int axis )
{
    iMotion *pMotion = (iMotion *)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);;
    if (pMotion)
    {
        IAxis* pAxis = pMotion->GetAxisInstance(m_stationInfo.axisname[axis]);
        if (pAxis)
        {
            if (!pAxis->IsOn())
            {
                if (pAxis->On())
                {
                    //pushButton_axis_on_1->setText("关使能");
                }
                else
                {
                    QMessageBox::warning(this,"warn", QString::fromLocal8Bit("轴使能失败,轴名字：%1").arg(m_stationInfo.axisname[axis]));
                }
            }
            else
            {
                if (pAxis->Off())
                {
                    //pushButton_axis_on_1->setText("开使能");
                }
                else
                {
                    QMessageBox::warning(this,"warn", QString::fromLocal8Bit("轴关闭使能失败,轴名字：%1").arg(m_stationInfo.axisname[axis]));
                }
            }
        }
        else
        {
            QMessageBox::warning(this,"warn",QString("GetAxisInstance return null pointer,%1").arg(m_stationInfo.axisname[axis]));
        }
    }
}

void MotionCtlDlg::clearAlarm( int axis )
{
    iMotion *pMotion = (iMotion *)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);;
    if (pMotion)
    {
        IAxis* pAxis = pMotion->GetAxisInstance(m_stationInfo.axisname[axis]);
        if (pAxis)
        {
            if (!pAxis->ClearStatus())
            {
                QMessageBox::warning(this,"warn", QString::fromLocal8Bit("清除报警失败,轴名字：%1").arg(m_stationInfo.axisname[axis]));
            }
        }
        else
        {
            QMessageBox::warning(this,"warn", QString::fromLocal8Bit("GetAxisInstance return null pointer,%1").arg(m_stationInfo.axisname[axis]));
        }
    }
}

void MotionCtlDlg::axisHome( int axis )
{
    if (QMessageBox::Yes != QMessageBox::warning(this,"warn",QString("确认回原吗?"),QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }

    MovingDlg dlg(m_stationInfo.name);
    E_AXISMASK mask;
    switch(axis)
    {
    case STATION_X:
        mask = Mask_X;
        break;
    case STATION_Y:
        mask = Mask_Y;
        break;
    case STATION_Z:
        mask = Mask_Z;
        break;
    case STATION_U:
        mask = Mask_U;
        break;
    case STATION_V:
        mask = Mask_V;
        break;
    case STATION_W:
        mask = Mask_W;
        break;
    }

    dlg.MoveHome(mask);
    dlg.exec();
}

void MotionCtlDlg::updateStationSpeed()
{
    RETURN_IF(NULL == m_pStation);

    int speedRatio = 0;
    int accRatio = 0;
    int decRatio = 0;
    if (SYS_ERR_OK == m_pStation->GetRatioSpeed(speedRatio, accRatio, decRatio))
    {
        if (speedRatio == 20)  // low speed
        {
            ui->lowSpeed->setChecked(true);
            int index = ui->defineSpeedVaule->findText(QString::number(speedRatio)+"%");
            RETURN_IF(index < 0);
            ui->defineSpeedVaule->setCurrentIndex(index);
        }
        else if(speedRatio == 60) // high speed
        {
            ui->highSpeed->setChecked(true);
            int index = ui->defineSpeedVaule->findText(QString::number(speedRatio)+"%");
            RETURN_IF(index < 0);
            ui->defineSpeedVaule->setCurrentIndex(index);
        }
        else
        {
            ui->defineSpeed->setChecked(true);
            int index = ui->defineSpeedVaule->findText(QString::number(speedRatio)+"%");
            RETURN_IF(index < 0);
            ui->defineSpeedVaule->setCurrentIndex(index);
        }
    }
}

void MotionCtlDlg::on_lowSpeed_clicked( bool bChecked)
{
    if(bChecked)
    {
        ui->defineSpeedVaule->setCurrentIndex(9);
    }
}

void MotionCtlDlg::on_highSpeed_clicked( bool bChecked)
{
    if(bChecked)
    {
        ui->defineSpeedVaule->setCurrentIndex(29);
    }
}

void MotionCtlDlg::on_defineSpeed_toggled( bool bChecked)
{
    ui->defineSpeedVaule->setEnabled(bChecked);
}

void MotionCtlDlg::keyPressEvent( QKeyEvent *e )
{
    switch(e->key())
    {
        //屏闭Esc键 
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(e);
    }
}

void MotionCtlDlg::on_tableWidgetpoint_currentCellChanged( int currentRow, int currentColumn, int previousRow, int previousColumn )
{
    if (-1 == currentRow)
    {
        return;
    }
    QString index = ui->tableWidgetpoint->item(currentRow,0)->text();
    QString pointName = ui->tableWidgetpoint->item(currentRow,1)->text();
    QString name = "P" + index + " - " + pointName;
    ui->comboBox_point->setCurrentIndex(ui->comboBox_point->findText(name));
}

void MotionCtlDlg::on_pushButton_setspeed_clicked()
{
    int ires = -1;
    RETURN_IF(NULL == m_pStation);

    if (ui->lowSpeed->isChecked())
    {
        ires = m_pStation->SetRatioSpeed(20);
    }
    else if(ui->highSpeed->isChecked())
    {
        ires = m_pStation->SetRatioSpeed(60);
    }
    else if(ui->defineSpeed->isChecked())
    {
        int index  = ui->defineSpeedVaule->currentIndex();
        int speed  = (index + 1)*2;
        ires = m_pStation->SetRatioSpeed(speed);
    }

    if (SYS_ERR_OK != ires)
    {
        QMessageBox::warning(this,"warn",QString("Set speed failed,error:%1").arg(ires));
    }

    return;
}

void MotionCtlDlg::slotAlwaysMove()
{
    RETURN_IF(NULL == m_pStation);
    if(STATION_IDLE != m_pStation->GetStatus())
    {
        //QMessageBox::warning(this, "warn", "工站正在运动中,请先停止.");
        return;
    }

    // 如果是连续运动,调用连续运动接口
    if ( ui->radioButtonConti->isChecked() )
    {
        on_pushButton_setspeed_clicked();//每次移动前设置下速度
        QPushButton* pbtn =  qobject_cast<QPushButton*>(sender());

        

        int index = -1;
        bool bDir = false;
        if(pbtn != NULL)
        {
            if(pbtn == ui->pushButtonXAdd)
            {
                index = 0; 
                bDir = true;
            }
            else if(pbtn == ui->pushButtonXSub)
            {
                index = 0; 
                bDir = false;
            }
            else if(pbtn == ui->pushButtonYAdd)
            {
                index = 1; 
                bDir = true;
            }
            else if(pbtn == ui->pushButtonYSub)
            {
                index = 1; 
                bDir = false;
            }
            else if(pbtn == ui->pushButtonZAdd)
            {
                index = 2; 
                bDir = true;
            }
            else if(pbtn == ui->pushButtonZSub)
            {
                index = 2;
                bDir = false;
            }
            else if(pbtn == ui->pushButtonUAdd)
            {
                index = 3;
                bDir = true;
            }
            else if(pbtn == ui->pushButtonUSub)
            {
                index = 3;
                bDir = false;
            }
            else if(pbtn == ui->pushButtonVAdd)
            {
                index = 4;
                bDir = true;
            }
            else if(pbtn == ui->pushButtonVSub)
            {
                index = 4;
                bDir = false;
            }
            else if(pbtn == ui->pushButtonWAdd)
            {
                index = 5;
                bDir = true;
            }
            else if(pbtn == ui->pushButtonWSub)
            {
                index = 5;
                bDir = false;
            }

            bool bReverse = m_axisDir[index];  //取反
            bDir = bReverse ? !bDir : bDir;

            int error =  m_pStation->AxisMoveJog(index, bDir);
            if (SYS_ERR_OK != error)
            {
                QMessageBox::warning(this,"warn",QString("Move failed,error:%1").arg(error));
            }
        }
    }
}

void MotionCtlDlg::slotAlwaysMoveFinish()
{
    // 如果是连续运动,调用连续运动接口
    if ( ui->radioButtonConti->isChecked() )
    {
        RETURN_IF(NULL == m_pStation);

        // 没有停止单个轴的接口，使用工站停止，效果一样
        bool bret = m_pStation->Stop();
    }
}

void MotionCtlDlg::initMoveJogBtn()
{
    connect(ui->pushButtonXAdd,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));
    connect(ui->pushButtonXSub,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));

    connect(ui->pushButtonYAdd,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));
    connect(ui->pushButtonYSub,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));

    connect(ui->pushButtonZAdd,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));
    connect(ui->pushButtonZSub,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));

    connect(ui->pushButtonUAdd,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));
    connect(ui->pushButtonUSub,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));

    connect(ui->pushButtonVAdd,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));
    connect(ui->pushButtonVSub,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));

    connect(ui->pushButtonWAdd,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));
    connect(ui->pushButtonWSub,SIGNAL(pressed()),this,SLOT(slotAlwaysMove()));


    connect(ui->pushButtonXAdd,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));
    connect(ui->pushButtonXSub,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));

    connect(ui->pushButtonYAdd,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));
    connect(ui->pushButtonYSub,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));

    connect(ui->pushButtonZAdd,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));
    connect(ui->pushButtonZSub,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));

    connect(ui->pushButtonUAdd,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));
    connect(ui->pushButtonUSub,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));

    connect(ui->pushButtonVAdd,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));
    connect(ui->pushButtonVSub,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));

    connect(ui->pushButtonWAdd,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));
    connect(ui->pushButtonWSub,SIGNAL(released()),this,SLOT(slotAlwaysMoveFinish()));

    connect(ui->radioButtoncustom,SIGNAL(clicked()),this,SLOT(slotChangedistance()));
    connect(ui->radioButtonlong,SIGNAL(clicked()),this,SLOT(slotChangedistance()));
    connect(ui->radioButtonmid,SIGNAL(clicked()),this,SLOT(slotChangedistance()));
    connect(ui->radioButtonsmall,SIGNAL(clicked()),this,SLOT(slotChangedistance()));
    connect(ui->radioButtonConti,SIGNAL(clicked()),this,SLOT(slotChangedistance()));
}

void MotionCtlDlg::slotChangedistance()
{
    double ddistance = 0.0;
    QRadioButton* pbtn =  qobject_cast<QRadioButton*>(sender());
    if(pbtn == NULL)
    {
        return;
    }

    ui->doubleSpinBox_x->setReadOnly(true);
    ui->doubleSpinBox_y->setReadOnly(true);
    ui->doubleSpinBox_z->setReadOnly(true);
    ui->doubleSpinBox_u->setReadOnly(true);
    ui->doubleSpinBox_v->setReadOnly(true);
    ui->doubleSpinBox_w->setReadOnly(true);

    if (pbtn == ui->radioButtonsmall || ui->radioButtonsmall->isChecked())
    {
        ddistance = 0.1;
    }
    else if(pbtn == ui->radioButtonmid || ui->radioButtonmid->isChecked())
    {
        ddistance = 1.0;
    }
    else if(pbtn == ui->radioButtonlong || ui->radioButtonlong->isChecked())
    {
        ddistance = 10;
    }
    else if(pbtn == ui->radioButtoncustom || ui->radioButtoncustom->isChecked())
    {
        ui->doubleSpinBox_x->setReadOnly(false);
        ui->doubleSpinBox_y->setReadOnly(false);
        ui->doubleSpinBox_z->setReadOnly(false);
        ui->doubleSpinBox_u->setReadOnly(false);
        ui->doubleSpinBox_v->setReadOnly(false);
        ui->doubleSpinBox_w->setReadOnly(false);
        ddistance = 0.0;
    }
    else if(pbtn == ui->radioButtonConti || ui->radioButtonConti->isChecked())
    {
        ddistance = 0.0;
    }


    ui->doubleSpinBox_x->setValue(ddistance);
    ui->doubleSpinBox_y->setValue(ddistance);
    ui->doubleSpinBox_z->setValue(ddistance);
    ui->doubleSpinBox_u->setValue(ddistance);
    ui->doubleSpinBox_v->setValue(ddistance);
    ui->doubleSpinBox_w->setValue(ddistance);


    return;
}

void MotionCtlDlg::slotModPoint( int row,int cloun )
{
    RETURN_IF(NULL == m_pStation);

    //如果是第0列不执行
    if(cloun == 0)
        return;

    if(m_isReadFile)
        return;

    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QString vaule = "";

    StPoint *oldPoint =  &m_pointInfoList[row];

    if(ui->tableWidgetpoint->item(row,cloun) != NULL)
        vaule = ui->tableWidgetpoint->item(row,cloun)->text();


    //判断用户输入是否合法
    if(cloun > 2 && !vaule.isEmpty())
    {
        QRegExp RegExp("^(-?\\d+)(\\.\\d+)?$");
        if(!RegExp.exactMatch(vaule))
        {
            QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请输入数字！"));

            m_isReadFile = true;

            ui->tableWidgetpoint->item(row,1)->setText(QString(oldPoint->name));
            ui->tableWidgetpoint->item(row,2)->setText(QString(oldPoint->desc));
            ui->tableWidgetpoint->item(row,3)->setText(QString::number(oldPoint->pos.x, 'f', 3));
            ui->tableWidgetpoint->item(row,4)->setText(QString::number(oldPoint->pos.y, 'f', 3));
            ui->tableWidgetpoint->item(row,5)->setText(QString::number(oldPoint->pos.z, 'f', 3));
            ui->tableWidgetpoint->item(row,6)->setText(QString::number(oldPoint->pos.u, 'f', 3));
            ui->tableWidgetpoint->item(row,7)->setText(QString::number(oldPoint->pos.v, 'f', 3));
            ui->tableWidgetpoint->item(row,8)->setText(QString::number(oldPoint->pos.w, 'f', 3));
            m_isReadFile = false;
            //更新显示
            updatePointUI();
            return;
        }
    }
    if(1 == cloun)
    {
        if(!vaule.isEmpty())
        {
            vaule = vaule.trimmed();
            //写文件之前判断输入的名称是否有同名，否则返回，提示

            const map<int,StPoint>* allPoint = m_pStation->GetAllPoint();
            if (NULL == allPoint) return;

            map<int,StPoint>::const_iterator iter = allPoint->begin();
            while(iter!=allPoint->end())
            {
                StPoint stPoint = iter->second;
                if (vaule == stPoint.name)
                {
                    QMessageBox::warning(this,"warn", QString::fromLocal8Bit("点名称重复!"));
                    //更新显示
                    updatePointUI();
                    return;
                }
                ++iter;
            }

            QStringList allPointName;
            for(int i = 0; i < m_pointInfoList.count(); i++)
            {
                if( i != row)
                    allPointName.append(m_pointInfoList[i].name);
            }
        }
        else
        {
            QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("输入的名称不能为空，请重新输入！"));
            m_isReadFile = true;

            ui->tableWidgetpoint->item(row,1)->setText(QString(oldPoint->name));
            ui->tableWidgetpoint->item(row,2)->setText(QString(oldPoint->desc));
            ui->tableWidgetpoint->item(row,3)->setText(QString::number(oldPoint->pos.x, 'f', 3));
            ui->tableWidgetpoint->item(row,4)->setText(QString::number(oldPoint->pos.y, 'f', 3));
            ui->tableWidgetpoint->item(row,5)->setText(QString::number(oldPoint->pos.z, 'f', 3));
            ui->tableWidgetpoint->item(row,6)->setText(QString::number(oldPoint->pos.u, 'f', 3));
            ui->tableWidgetpoint->item(row,7)->setText(QString::number(oldPoint->pos.v, 'f', 3));
            ui->tableWidgetpoint->item(row,8)->setText(QString::number(oldPoint->pos.w, 'f', 3));
            m_isReadFile = false;

            //更新显示
            updatePointUI();
            return;
        }
        strncpy(m_pointInfoList[row].name, vaule.toStdString().c_str(), NAME_LEN-1);
    }
    else if(2 == cloun)
    {
        strncpy(m_pointInfoList[row].desc, vaule.toUtf8(), NAME_LEN-1);
    }
    else if(3 == cloun)
    {
        m_pointInfoList[row].pos.x= vaule.toDouble();
    }
    else if(4 == cloun)
    {
        m_pointInfoList[row].pos.y= vaule.toDouble();
    }
    else if(5 == cloun)
    {
        m_pointInfoList[row].pos.z= vaule.toDouble();
    }
    else if(6 == cloun)
    {
        m_pointInfoList[row].pos.u= vaule.toDouble();
    }
    else if(7 == cloun)
    {
        m_pointInfoList[row].pos.v= vaule.toDouble();
    }
    else if(8 == cloun)
    {
        m_pointInfoList[row].pos.w= vaule.toDouble();
    }
    int error = m_pStation->TeachPoint(m_pointInfoList[row]);
    if(SYS_ERR_OK != error){
        m_isReadFile = true;
        ui->tableWidgetpoint->item(row,1)->setText(QString(oldPoint->name));
        ui->tableWidgetpoint->item(row,2)->setText(QString(oldPoint->desc));
        ui->tableWidgetpoint->item(row,3)->setText(QString::number(oldPoint->pos.x, 'f', 3));
        ui->tableWidgetpoint->item(row,4)->setText(QString::number(oldPoint->pos.y, 'f', 3));
        ui->tableWidgetpoint->item(row,5)->setText(QString::number(oldPoint->pos.z, 'f', 3));
        ui->tableWidgetpoint->item(row,6)->setText(QString::number(oldPoint->pos.u, 'f', 3));
        ui->tableWidgetpoint->item(row,7)->setText(QString::number(oldPoint->pos.v, 'f', 3));
        ui->tableWidgetpoint->item(row,8)->setText(QString::number(oldPoint->pos.w, 'f', 3));
        m_isReadFile = false;
        ui->tableWidgetpoint->item(row,cloun)->setText(vaule);
        QMessageBox::warning(this,"warn",QString("Save point failed,error:%1").arg(error));
    }


    //如果修改名称，单独处理
    if(1 == cloun)
    {
        m_pointNameList[row] = "P" + QString::number(m_pointInfoList[row].index) + "-" + m_pointInfoList[row].name;
    }

    //更新显示
    ui->comboBox_point->clear();
    ui->comboBox_point->addItems(m_pointNameList);
    updatePointUI();
    return;
}

void MotionCtlDlg::on_pushButton_expand_clicked()
{
    bool bShowPoint = ui->groupBox_point->isVisible();
    ui->groupBox_point->setVisible(!bShowPoint);

    if (bShowPoint)
    {
        QTimer::singleShot(1,this, SLOT(slot_resize()));
    }
}
