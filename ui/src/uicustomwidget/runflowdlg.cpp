#include "runflowdlg.h"
#include "ui_runflowdlg.h"
#include "mainflow/imainflow.h"
#include "mainflow/imainflowmgr.h"
#include "callsdk.h"

RunFlowDlg::RunFlowDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::runFlowDlg)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
    m_pTimer = new QTimer(this);
    ui->toolButton_continue->hide();
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(on_timeout()));
}

RunFlowDlg::~RunFlowDlg()
{
    delete ui;
}


void RunFlowDlg::on_toolButton_pause_clicked()
{
    if (NULL != m_pMainFow && !m_flowname.isEmpty())
    {
        m_pMainFow->pauseFlow(m_flowname.toStdString());
    }

    ui->label_move->setText(QString::fromLocal8Bit("��ͣ."));
    ui->toolButton_pause->hide();
    ui->toolButton_continue->show();
}

void RunFlowDlg::on_toolButton_continue_clicked()
{
    if (NULL != m_pMainFow && !m_flowname.isEmpty())
    {
        m_pMainFow->continueFlow(m_flowname.toStdString());
    }

    ui->label_move->setText(QString::fromLocal8Bit("�˶���..."));
    ui->toolButton_pause->show();
    ui->toolButton_continue->hide();
}

void RunFlowDlg::on_toolButton_stop_clicked()
{
    if (NULL != m_pMainFow && !m_flowname.isEmpty())
    {
        m_pMainFow->stopFlow(m_flowname.toStdString());
    }

    close();
    m_pMainFow = NULL;
}

ERROR_CODE RunFlowDlg::runFlow( const QString &group, const QString &flowName, const QString &beginStep/*=""*/, const QString &endStep/*=""*/ )
{
    IMainflowMgr *pMainFlowMgr = (IMainflowMgr*)GetSDKInstance("mainflow", IMainflowMgr::IDD_MAIN_FLOW_MGR);
    RETURN_V_IF(NULL == pMainFlowMgr, SYS_ERR_MAINFLOW_NOINSTANCE);
    
    m_pMainFow = pMainFlowMgr->findMainflow(group.toLatin1().data());
    RETURN_V_IF(NULL == m_pMainFow, SYS_ERR_MAINFLOW_NOINSTANCE);
    m_flowname = flowName;
    
    int nRet = m_pMainFow->runFlow(flowName.toStdString(), beginStep.toStdString(), endStep.toStdString());
    if (SYS_ERR_OK != nRet)
    {
        ui->label_move->setText(QString::fromLocal8Bit("��������ʧ��! Err:") + QString::number(nRet));
        ui->toolButton_stop->setEnabled(false);
        ui->toolButton_pause->setEnabled(false);
        return nRet;
    }

    m_pTimer->start(500);
    return SYS_ERR_OK;
}


void RunFlowDlg::showEvent( QShowEvent * )
{
}

void RunFlowDlg::hideEvent( QHideEvent * )
{
    if (NULL != m_pMainFow && !m_flowname.isEmpty())
    {
        m_pMainFow->stopFlow(m_flowname.toStdString());
    }

    m_pTimer->stop();
}

void RunFlowDlg::on_timeout()
{
    if (m_pMainFow)
    {
        QString message;
        E_FLOW_STATUS status = (E_FLOW_STATUS)m_pMainFow->runStatus(m_flowname.toStdString());
        RETURN_IF(FLOW_STATUS_RUNING == status);

        switch(status)
        {
        case FLOW_STATUS_IDLE:   // δ����
            {
                message = "δ����!";
            }
            break;
        case FLOW_STATUS_OK:
            {
                message = "ִ�гɹ�.";
            }
            break;
        case FLOW_STATUS_FAILED:
            {
                message = "ִ��ʧ��.";
            }
            break;
        case FLOW_STATUS_PAUSE:
            {
                message = "��ͣ!";
            }
            break;
        case FLOW_STATUS_STOP:
            {
                message = "ֹͣ!";
            }
            break;
        case FLOW_STATUS_ERROR:
            {
                message = "ִ�г���!";
            }
            break;
        default:
            break;
        }

        string stepName;
        string actionName;
        if(SYS_ERR_OK == m_pMainFow->getCurrentAction(m_flowname.toStdString(), stepName, actionName))
        {
            message += "\n"+m_flowname+"->"+QString::fromStdString(stepName)+"->"+QString::fromStdString(actionName);
        }

        ui->label_move->setText(message);
        ui->toolButton_stop->setEnabled(false);
        ui->toolButton_pause->setEnabled(false);
        m_pTimer->stop();
    }
}
