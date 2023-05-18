#include "clientinfodlg.h"
#include "ui_ClientInfoDlg.h"
#include "proxymanager.h"
#include "socketlistcfg.h"
#include <QMessageBox>

ClientInfoDlg::ClientInfoDlg(OperatorModel enModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientInfoDlg),
    m_enModel(enModel)
{
    ui->setupUi(this);
    switch(m_enModel)
    {
    case Create:
        setWindowTitle(QString::fromLocal8Bit("新增"));
        break;
    case Alter:
        setWindowTitle(QString::fromLocal8Bit("修改"));
        break;
    }
    //不输入中文
    ui->lineEdit_ClientName->setAttribute(Qt::WA_InputMethodEnabled, false);
    
    //ip
    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    QRegExpValidator* IPValidator = new QRegExpValidator(rx, this);
    ui->lineEdit_ClientIP->setValidator(IPValidator);
    ui->lineEdit_ClientIP_des->setValidator(IPValidator);

    //光标
    ui->lineEdit_ClientName->setFocus();
}

ClientInfoDlg::~ClientInfoDlg()
{
    delete ui;
}

clientCfg ClientInfoDlg::GetUiData()
{
    clientCfg stCfg;
    stCfg.ClientName = ui->lineEdit_ClientName->text().toStdString();
    stCfg.ClientIP = ui->lineEdit_ClientIP->text().toStdString();
    stCfg.ClientPort = ui->spinBox_ClientPort->value();
    stCfg.DesIP = ui->lineEdit_ClientIP_des->text().toStdString();
    stCfg.DesPort = ui->spinBox_ClientPort_des->value();
    return stCfg;
}

void ClientInfoDlg::on_pushButton_ok_clicked()
{
    clientCfg stCfg = GetUiData();
    CSocketClientListCfg* pServerCfg = CSocketClientListCfg::getInstance();
    if (NULL == pServerCfg)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("获取 CSocketClientListCfg 实例失败！"), QMessageBox::Ok);
        return;
    }
    if ("" == stCfg.ClientName
        || "" == stCfg.ClientIP)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("内容不允许为空！"), QMessageBox::Ok);
        return;
    }
    vector<string> vecClientName;
    pServerCfg->getClientList(vecClientName);
    for (int i = 0; i < vecClientName.size(); ++i)
    {
        if ((Create == m_enModel && vecClientName[i] == stCfg.ClientName)
            || (Alter == m_enModel && vecClientName[i] != m_stSrcCfg.ClientName && vecClientName[i] == stCfg.ClientName))
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("ClientName已存在！"), QMessageBox::Ok);
            return;
        }
    }

    for (int i = 0; i < vecClientName.size(); ++i)
    {
        clientCfg* pStCfg = pServerCfg->getClientCfg(vecClientName[i]);
        if (NULL == pStCfg)
        {
            continue;
        }
        if ((Create == m_enModel && pStCfg->ClientName == stCfg.ClientName)
            || (Alter == m_enModel && vecClientName[i] != m_stSrcCfg.ClientName && pStCfg->ClientName == stCfg.ClientName))
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("ClientSN已存在！"), QMessageBox::Ok);
            return;
        }
    }

    for (int i = 0; i < vecClientName.size(); ++i)
    {
        clientCfg* pStCfg = pServerCfg->getClientCfg(vecClientName[i]);
        if (NULL == pStCfg)
        {
            continue;
        }
        if ((Create == m_enModel && (pStCfg->ClientIP == stCfg.ClientIP && pStCfg->ClientPort == stCfg.ClientPort))
            || (Alter == m_enModel && vecClientName[i] != m_stSrcCfg.ClientName && (pStCfg->ClientIP == stCfg.ClientIP && pStCfg->ClientPort == stCfg.ClientPort)))
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("ClientIP、ClientPort已存在！"), QMessageBox::Ok);
            return;
        }
    }

    accept();
}

void ClientInfoDlg::SetUiData( const clientCfg& stCfg )
{
     ui->lineEdit_ClientName->setText(stCfg.ClientName.c_str());
     ui->lineEdit_ClientIP->setText(stCfg.ClientIP.c_str());
     ui->spinBox_ClientPort->setValue(stCfg.ClientPort);
     ui->lineEdit_ClientIP_des->setText(stCfg.DesIP.c_str());
     ui->spinBox_ClientPort_des->setValue(stCfg.DesPort);
     m_stSrcCfg = stCfg;
}
