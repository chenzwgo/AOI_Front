#include "communicationtestform.h"
#include "ui_communicationtestform.h"
#include "proxymanager.h"
#include "sdkcommon/notificationcenter/inotificationcenter.h"
#include <QMessageBox>
#include "socketclient.h"
#include "socketlistcfg.h"
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#define  SOCKET_FUN_FILE_PATH "./plugins/mainui/FunConfig.xml"
CommunicationTestForm* CommunicationTestForm::m_pInstance = NULL;

CommunicationTestForm::CommunicationTestForm(QString strCameraName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicationTestForm),
    m_strCameraName(strCameraName),
    m_bLoopSend(false),
    m_iLoopTime(100)
{
    ui->setupUi(this);
    ui->pushButton_cameraName->setText(m_strCameraName);
    setWindowTitle(QString::fromLocal8Bit("发送数据测试--")+m_strCameraName);
    m_pThreadLoopSend   = new JThreadT<CommunicationTestForm>;
    m_pClientListCfg =  CSocketClientListCfg::getInstance();
    m_pQtimer = new QTimer(this);
    m_pInstance = this;
    m_pQtimer->start(100);
    readFunCfg();
    ui->comboBox_funcCode->clear();

    ui->comboBox_funcCode->addItems(m_SendContextMap.keys());
    connect(m_pQtimer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
    connect(this, SIGNAL(sigMyLog(QString, int)), this, SLOT(slotMyLog(QString, int)));
    }

CommunicationTestForm::~CommunicationTestForm()
{
    m_pInstance = NULL;
    QMap<QString,STSendContext*>::const_iterator i = m_SendContextMap.constBegin();
    while (i != m_SendContextMap.constEnd())
    {
        STSendContext* ptmp = i.value();
        if (ptmp != NULL)
        {
            delete ptmp;
        }  
        ++i;
    }
    m_SendContextMap.clear();
    m_pQtimer->stop();
    delete m_pQtimer;
    m_pQtimer = NULL;

    m_bLoopSend = false;
    while(m_pThreadLoopSend->IsRunning())
    {
        Sleep(10);
    }
    delete m_pThreadLoopSend;
    m_pThreadLoopSend = NULL;
    delete ui;
}

void CommunicationTestForm::slotTimeOut()
{
    CSocketClient* pISMCameraClient = m_pClientListCfg->getSocketClientProxy(m_strCameraName.toStdString())->getSocketClient();
	if (NULL != pISMCameraClient)
	{
        bool bOnLine = pISMCameraClient->isConnected();
		if (!bOnLine)
		{
			if (ui->checkBox_loop->isChecked())
			{
				on_checkBox_loop_clicked(false);
				ui->checkBox_loop->setChecked(false);
			}
		}
		ui->pushButton_cameraName->setIcon(QIcon(bOnLine ? ":/images/IoOn.png" : ":/images/IoOff.png"));
	}
	else
	{
		close();
	}
}

void CommunicationTestForm::on_pushButton_send_clicked()
{
    GetUiSendContext(m_stSendContext);
    SendContext(m_stSendContext);
}

void CommunicationTestForm::on_checkBox_loop_clicked( bool bCkecked )
{
    if (bCkecked)
    {
        if (m_pThreadLoopSend->IsRunning())
        {
            ui->checkBox_loop->setChecked(false);
            QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("循环正在执行中！"));
            return;
        }
        GetUiSendContext(m_stSendContext);
        m_iLoopTime = ui->spinBox_loopTime->value();
        m_bLoopSend = bCkecked;
        m_pThreadLoopSend->StartThread(this,&CommunicationTestForm::ThreadLoopSend);
    }
    m_bLoopSend = bCkecked;
}

void CommunicationTestForm::on_comboBox_funcCode_currentIndexChanged( int index )
{
    QString strFunCode = ui->comboBox_funcCode->currentText();
    if (m_SendContextMap.contains(strFunCode))
    {
        ui->textEdit_context->setText(m_SendContextMap[strFunCode]->strBody);
        ui->comboBox_funcCode->setToolTip(m_SendContextMap[strFunCode]->strDes);
    }
}
void CommunicationTestForm::ThreadLoopSend()
{
    while(m_bLoopSend)
    {
        SendContext(m_stSendContext);
        Sleep(m_iLoopTime);
    }
}

void CommunicationTestForm::on_pushButton_clearLog_clicked()
{
    ui->textEdit_log->clear();
}

void CommunicationTestForm::hideEvent( QHideEvent *e )
{
    //m_bLoopSend = false;
    //ui->checkBox_loop->setChecked(false);
    QWidget::hideEvent(e);
}

void CommunicationTestForm::GetUiSendContext( STSendContext& stContext )
{
    stContext.strFunCode = ui->comboBox_funcCode->currentText();
    stContext.strBody = ui->textEdit_context->toPlainText();
}

void CommunicationTestForm::SendContext( const STSendContext& stContext )
{
    CSocketClient* pSocketClient = m_pClientListCfg->getSocketClientProxy(m_strCameraName.toStdString())->getSocketClient();
    CSocketproxy* pSocketProxy = m_pClientListCfg->getSocketClientProxy(m_strCameraName.toStdString());

    if (NULL == pSocketClient)
	{
		PrintLog(QString::fromLocal8Bit("获取pSocketClient实例失败！"), UILOG_ERR);
		return;
	}
    int iErr = ERR_OK;
    iErr = pSocketProxy->sendRequest(stContext.strFunCode.toStdString(),stContext.strBody.toStdString());

    if (ERR_OK != iErr)
    {
        PrintLog(QString::fromLocal8Bit("ui发送数据失败，错误码：") + errorCodeToStr(iErr), UILOG_ERR);
    }
}

void CommunicationTestForm::slotMyLog( QString strLog, int iLevel )
{
    ui->textEdit_log->Addlogmsg(strLog, (UILogLevel)iLevel);
}

int CommunicationTestForm::iCallBackPrintLog(  unsigned int enNotifyType, QString strMsg )
{
    if (m_pInstance != NULL)
    {
        m_pInstance->PrintLog(strMsg, (UILogLevel)enNotifyType);
    }  
    return 0;
}

void CommunicationTestForm::PrintLog( QString strLog, UILogLevel iLevel )
{
    if (this->isVisible())
    {
        emit sigMyLog(strLog, iLevel);
    }  
}


void CommunicationTestForm::readFunCfg()
{
    QString strPath = SOCKET_FUN_FILE_PATH;

    QFile localfile(strPath);
    if(!localfile.open(QFile::ReadOnly))
    {
        return  ;
    }

    QDomDocument dom;
    if(!dom.setContent(&localfile))
    {
        localfile.close();
        return ;
    }
    QDomElement rootNode = dom.documentElement();
    QDomNodeList NetTypeList = rootNode.childNodes();
    for (int i = 0; i < NetTypeList.size(); ++i)
    {
        QDomElement NetType_elem = NetTypeList.at(i).toElement();
        STSendContext* pContext = new STSendContext;
        pContext->strFunCode = NetType_elem.attribute("Fun");
        pContext->strBody = NetType_elem.attribute("Body");
        pContext->strDes =NetType_elem.attribute("des");
        m_SendContextMap.insert(pContext->strFunCode,pContext);
    }
    return ;
}
