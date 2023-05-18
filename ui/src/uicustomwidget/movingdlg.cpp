#include "customui/movingdlg.h"
#include "motion/imotion.h"
#include "station/istation.h"
#include <QMessageBox>
#include "proxymanager.h"
#include "ui_movingdlg.h"
#include "rtdefine.h"

MovingDlg::MovingDlg( const QString &stationName, QWidget *parent /*= 0*/ )
    : QDialog(parent)
    , ui(new Ui::Moving)
    , m_pStation(NULL)
    , m_type(E_TYPE_OFFSET)
    , m_axisMask(Mask_ALL)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    m_pTimer = new QTimer(this);
    ui->toolButton_continue->hide();
    ui->label_home->hide();

	iMotion *pMotion = (iMotion *)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
	if(NULL != pMotion)
	{
		IStation *pStation = pMotion->GetStationInstance(stationName.toLatin1().data());
		if(NULL != pStation)
		{
			m_pStation = pStation;
		}
	}

    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(on_timeout()));
}

MovingDlg::~MovingDlg()
{
}

void MovingDlg::on_toolButton_pause_clicked()
{
	if (NULL != m_pStation)
	{
		m_pStation->Pause();
	}

	ui->label_move->setText(QString::fromLocal8Bit("暂停."));
    ui->label_home->setText(QString::fromLocal8Bit("暂停."));
	ui->toolButton_pause->hide();
	ui->toolButton_continue->show();
}

void MovingDlg::on_toolButton_continue_clicked()
{
	if (NULL != m_pStation)
	{
		m_pStation->Continue();
	}

	ui->label_move->setText(QString::fromLocal8Bit("运动中..."));
    ui->label_home->setText(QString::fromLocal8Bit("回原中..."));
	ui->toolButton_pause->show();
	ui->toolButton_continue->hide();
}

void MovingDlg::on_toolButton_stop_clicked()
{
	if (NULL != m_pStation)
	{
		m_pStation->Stop();
	}

	close();
}

ERROR_CODE MovingDlg::MovePoint( const QString &pointName, short mask, bool bAbs )
{
	RETURN_V_IF(NULL == m_pStation, SYS_ERR_STATION_GET_INSTANCE);
	int pointIndex = m_pStation->GetPointIndex(pointName.toLatin1().data());
    return MovePoint(pointIndex, mask, bAbs);	
}

ERROR_CODE MovingDlg::MovePoint( int pointIndex, short mask, bool bAbs )
{
    m_axisMask = mask;
	RETURN_V_IF(NULL == m_pStation, SYS_ERR_STATION_GET_INSTANCE);

    ui->label_move->show();
    ui->label_home->hide();
    ui->toolButton_stop->setEnabled(true);
    ui->toolButton_pause->setEnabled(true);

    StPoint point;
    RETURN_V_IF(SYS_ERR_OK != m_pStation->GetPoint(pointIndex, point), SYS_ERR_STATION_GET_POS);
    StPos curPos;
    RETURN_V_IF(SYS_ERR_OK != m_pStation->GetCurrentPos(curPos), SYS_ERR_STATION_GET_POS);

    if (bAbs)
    {
        curPos.x = (mask & Mask_X) ? point.pos.x : curPos.x;
        curPos.y = (mask & Mask_Y) ? point.pos.y : curPos.y;
        curPos.z = (mask & Mask_Z) ? point.pos.z : curPos.z;
        curPos.u = (mask & Mask_U) ? point.pos.u : curPos.u;
        curPos.v = (mask & Mask_V) ? point.pos.v : curPos.v;
        curPos.w = (mask & Mask_W) ? point.pos.w : curPos.w;
    }
    else
    {
        curPos.x += (mask & Mask_X) ? point.pos.x : 0;
        curPos.y += (mask & Mask_Y) ? point.pos.y : 0;
        curPos.z += (mask & Mask_Z) ? point.pos.z : 0;
        curPos.u += (mask & Mask_U) ? point.pos.u : 0;
        curPos.v += (mask & Mask_V) ? point.pos.v : 0;
        curPos.w += (mask & Mask_W) ? point.pos.w : 0;
    }


    m_pointPos = curPos;
	ERROR_CODE nRet = m_pStation->MovePos(m_pointPos);
	if(SYS_ERR_OK == nRet)
	{
		m_type = E_Type_POINT;
		m_pTimer->start(500);
	}
    else
    {
        ui->label_move->setText(QString::fromLocal8Bit("启动跑点失败!")+" Err:"+QString::number(nRet));
        ui->toolButton_stop->setEnabled(false);
        ui->toolButton_pause->setEnabled(false);
    }

	return nRet;
}

void MovingDlg::on_timeout()
{
	if (NULL == m_pStation)
	{
        return;
    }

    switch (m_type)
    {
    case E_Type_POINT:
        {
            if(STATION_IDLE == m_pStation->GetStatus())
            {
                if (MOVE_TRUE == m_pStation->MoveSucceed(m_pointPos))
                {
                    ui->label_move->setText(QString::fromLocal8Bit("运动完成。"));
                    close();
                }
                else if(MOVE_FALSE == m_pStation->MoveSucceed(m_pointPos))
                {
                    ui->label_move->setText(QString::fromLocal8Bit("运动失败!"));
                }
                ui->toolButton_stop->setEnabled(false);
                ui->toolButton_pause->setEnabled(false);
                m_pTimer->stop();
            }
        }
        break;
    case E_TYPE_OFFSET:
        {
            if(STATION_ERROR == m_pStation->GetStatus())
            {
                ui->label_move->setText(QString::fromLocal8Bit("移动失败！"));
                m_pTimer->stop();
            }
            else if(STATION_IDLE == m_pStation->GetStatus())
            {
                close();
                m_pTimer->stop();
            }
        }
        break;
    case E_Type_HOME:
        {
            if(STATION_IDLE == m_pStation->GetStatus())
            {
                if (MOVE_TRUE == m_pStation->HomeStatus())
                {
                    ui->label_home->setText(QString::fromLocal8Bit("回原成功。"));
                }
                else if(MOVE_FALSE == m_pStation->HomeStatus())
                {
                    ui->label_home->setText(QString::fromLocal8Bit("回原失败!"));
                }
                ui->toolButton_stop->setEnabled(false);
                ui->toolButton_pause->setEnabled(false);
                m_pTimer->stop();
            }
        }
        break;
    default:
        break;
    }
}

void MovingDlg::showEvent( QShowEvent * )
{
    if (!m_pTimer->isActive())
    {
        m_pTimer->start(500);
    }
}

void MovingDlg::hideEvent( QHideEvent * )
{
    m_pTimer->stop();
	if (NULL != m_pStation)
	{
		m_pStation->Stop();
	}
}

ERROR_CODE MovingDlg::MoveHome( short mask)
{
	RETURN_V_IF(NULL == m_pStation, SYS_ERR_STATION_GET_INSTANCE);

    ui->label_move->hide();
    ui->label_home->show();
    ui->toolButton_stop->setEnabled(true);
    ui->toolButton_pause->setEnabled(true);

    int homePri[6] = {-1,-1,-1,-1,-1,-1};
    homePri[0] = (mask & Mask_X) ? 0 : -1;
    homePri[1] = (mask & Mask_Y) ? 0 : -1;
    homePri[2] = (mask & Mask_Z) ? 0 : -1;
    homePri[3] = (mask & Mask_U) ? 0 : -1;
    homePri[4] = (mask & Mask_V) ? 0 : -1;
    homePri[5] = (mask & Mask_W) ? 0 : -1;

    ERROR_CODE nRet = SYS_ERR_OK;
    if (mask == Mask_ALL)
    {
        nRet = m_pStation->Home();
    }
    else
    {
        nRet = m_pStation->Home(homePri[0], homePri[1], homePri[2], homePri[3], homePri[4], homePri[5]);
    }
    
    if(SYS_ERR_OK == nRet)
    {
		m_type = E_Type_HOME;
        m_pTimer->start(500);
    }
    else
    {
        ui->label_home->setText(QString::fromLocal8Bit("启动回原失败!")+" Err:"+QString::number(nRet));
        ui->toolButton_stop->setEnabled(false);
        ui->toolButton_pause->setEnabled(false);
    }

    return nRet;
}
