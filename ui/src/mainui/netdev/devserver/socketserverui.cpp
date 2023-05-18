﻿#include "socketserverui.h"
#include "uicommon/change.h"
#include "proxymanager.h"
#include "clientinfodlg.h"
#include "communicationtestform.h"
#include <QMessageBox>
#include <QMenu>
#include "socketPC1.h"

#define PLUGIN_PATH "plugins/mainui"
#define PLUGIN_NAME "smcameramanagerui"
#define YSE_CHINESE QString::fromLocal8Bit("是")
#define NO_CHINESE QString::fromLocal8Bit("否")

static const char *pluginText[] = {
    QT_TRANSLATE_NOOP( "Socketserverui", "PluginName" ),
};


Socketserverui::Socketserverui(QWidget *parent)
    : QWidget(parent)
{
    this->setupUi(this);
    m_pQTimer        = new QTimer;
    m_pActConnect    = new QAction(QString::fromLocal8Bit("连接"), this);
    m_pActDisconnect = new QAction(QString::fromLocal8Bit("断开"), this);
    m_pActDebug      = new QAction(QString::fromLocal8Bit("测试"), this);
    m_pActAdd        = new QAction(QString::fromLocal8Bit("新增"), this);
    m_pActDel        = new QAction(QString::fromLocal8Bit("删除"), this);
    m_pActAlter      = new QAction(QString::fromLocal8Bit("修改"), this);
    m_pMenu          = new QMenu(this);
    //m_pMenu->addAction(m_pActConnect   );
    m_pMenu->addAction(m_pActDisconnect);
    m_pMenu->addAction(m_pActDebug     );
    m_pMenu->addAction(m_pActAdd       );
    m_pMenu->addAction(m_pActAlter     );
    m_pMenu->addAction(m_pActDel       );
    connect(m_pQTimer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
    connect(m_pActConnect   , SIGNAL(triggered()), this,  SLOT(slotActConnect()   ));
    connect(m_pActDisconnect, SIGNAL(triggered()), this,  SLOT(slotActDisConnect()));
    connect(m_pActDebug, SIGNAL(triggered()), this,  SLOT(slotActDebug()   ));
    connect(m_pActAdd  , SIGNAL(triggered()), this,  SLOT(slotActAdd()));
    connect(m_pActDel  , SIGNAL(triggered()), this,  SLOT(slotActDel()   ));
    connect(m_pActAlter, SIGNAL(triggered()), this,  SLOT(slotActAlter()));
    init();
}

Socketserverui::~Socketserverui()
{
    m_pQTimer->stop();
    delete m_pQTimer;
    m_pQTimer =NULL;
}

bool Socketserverui::init()
{
    m_pSocketServer = CSocketServer::getInstance();
    m_pSocketServerCfg = CSocketServerCfg::getInstance();
    initCameralistTable();
    return true;
}

bool Socketserverui::deinit()
{
    m_pQTimer->stop();
    return true;
}

bool Socketserverui::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
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
    return true;
}

bool Socketserverui::hideWindow()
{
    hide();
    return true;
}


void Socketserverui::fitSize(const QRect& rect)
{
    resize(rect.width(), rect.height());
}

bool Socketserverui::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    qDebug("%s:%d(%s) event is %s, wparam is %d, lparam is %d",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(event), wparam, lparam);


    return true;
}

void Socketserverui::SetLanguage(QString strlanguage)
{
    QString strLangFilePath = Change::GetUITranslatorPath(PLUGIN_NAME);
    qDebug("%s:%d(%s) LangFile is \"%s\" ",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));


    if (m_translator.load(strLangFilePath))
    {
        qApp->installTranslator(&m_translator);

        retranslateUi(this); 
    }
    else
    {
        qWarning("%s:%d(%s) translator load LangFile\"%s\" failed, or is not exist",
                 __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));
    }
}

void Socketserverui::keyPressEvent( QKeyEvent *e )
{
    switch(e->key())
    {
        //屏闭Esc键 
    case Qt::Key_Escape:
        break;
    default:
        QWidget::keyPressEvent(e);
    }
}

bool Socketserverui::initCameralistTable()
{
    //表头
    if (CameraAtribute_End != tableWidget_clientList->columnCount())
    {
        tableWidget_clientList->setColumnCount(CameraAtribute_End);
        QStringList tempList;
        tempList << "ClientName" << "ClientIP" << "ClientPort" << "bOnLine";
        tableWidget_clientList->setHorizontalHeaderLabels(tempList);
    }
    //表内容
    tableWidget_clientList->clearContents();
    vector<string> vecCamraName;
    if (NULL == m_pSocketServerCfg || !m_pSocketServerCfg->getClientList(vecCamraName))
    {
        return false;
    }
    tableWidget_clientList->setRowCount(vecCamraName.size());
    for (int i = 0; i < vecCamraName.size(); ++i)
    {
        clientCfg* pSMCameraCfg = m_pSocketServerCfg->getClientCfg(vecCamraName[i]);
        if (NULL == pSMCameraCfg)
        {
            continue;
        }

        SetTableRow(i, *pSMCameraCfg);
    }
    serverCfg* pServerCfg = m_pSocketServerCfg->getServerCfg();
    if (pServerCfg != NULL)
    {
        lineEdit_server_name->setText(QString::fromStdString(pServerCfg->serverName));
        lineEdit_server_ip->setText(QString::fromStdString(pServerCfg->serverIP));
        lineEdit_server_port->setText(QString::number(pServerCfg->serverPort));
    }

    return true;
}

void Socketserverui::SetTableRow( int iRow, const clientCfg& clientCfg )
{
    QTableWidgetItem* pItemCamName = tableWidget_clientList->item(iRow, CameraAtribute_CamName);
    if (NULL == pItemCamName)
    {
        pItemCamName = new QTableWidgetItem;
        tableWidget_clientList->setItem(iRow, CameraAtribute_CamName, pItemCamName);
    }
    pItemCamName->setText(clientCfg.ClientName.c_str());

    QTableWidgetItem* pItemCamIP = tableWidget_clientList->item(iRow, CameraAtribute_CamIP);
    if (NULL == pItemCamIP)
    {
        pItemCamIP = new QTableWidgetItem;
        tableWidget_clientList->setItem(iRow, CameraAtribute_CamIP, pItemCamIP);
    }
    pItemCamIP->setText(clientCfg.ClientIP.c_str());

    QTableWidgetItem* pItemCamPort = tableWidget_clientList->item(iRow, CameraAtribute_CamPort);
    if (NULL == pItemCamPort)
    {
        pItemCamPort = new QTableWidgetItem;
        tableWidget_clientList->setItem(iRow, CameraAtribute_CamPort, pItemCamPort);
    }
    pItemCamPort->setText(QString::number(clientCfg.ClientPort));

    QTableWidgetItem* pItemOnLine = tableWidget_clientList->item(iRow, CameraAtribute_bOnline);
    if (NULL == pItemOnLine)
    {
        pItemOnLine = new QTableWidgetItem;
        tableWidget_clientList->setItem(iRow, CameraAtribute_bOnline, pItemOnLine);
    }
}


void Socketserverui::slotTimeout()
{
    for (int i = 0; i < tableWidget_clientList->rowCount(); ++i)
    {
        QString strCameraName = tableWidget_clientList->item(i, CameraAtribute_CamName)->text();
        bool bConnect = false;
        do 
        {
            if (NULL == m_pSocketServer)
            {
                break;
            }
            CSocketClientPC1* pISMCameraClient = m_pSocketServer->getSocketClient(strCameraName.toStdString());
            if (NULL == pISMCameraClient)
            {
                break;
            }
            bConnect = pISMCameraClient->isConnected();
        } while (0);

        //log更新
        if (m_mapCamStatus.end() == m_mapCamStatus.find(strCameraName))
        {
            m_mapCamStatus.insert(strCameraName, bConnect);
            bConnect ? textEdit_log->Addlogmsg(strCameraName + QString::fromLocal8Bit("已连接！"))
                     : textEdit_log->Addlogmsg(strCameraName + QString::fromLocal8Bit("未连接！"), UILOG_ERR);
            QTableWidgetItem* pItemOnLine = tableWidget_clientList->item(i, CameraAtribute_bOnline);
            if (NULL == pItemOnLine)
            {
                pItemOnLine = new QTableWidgetItem;
                tableWidget_clientList->setItem(i, CameraAtribute_bOnline, pItemOnLine);
            }
            pItemOnLine->setText(bConnect ? QString::fromLocal8Bit("已连接") : QString::fromLocal8Bit("已断开"));
            pItemOnLine->setIcon(bConnect ? QIcon(":/images/IoOn.png") : QIcon(":/images/IoOff.png"));
            pItemOnLine->setData(Qt::UserRole, bConnect);
        }
        else if (m_mapCamStatus[strCameraName] != bConnect)
        {
            m_mapCamStatus[strCameraName] = bConnect;
            bConnect ? textEdit_log->Addlogmsg(strCameraName + QString::fromLocal8Bit("已连接！"))
                : textEdit_log->Addlogmsg(strCameraName + QString::fromLocal8Bit("未连接！"), UILOG_ERR);
            QTableWidgetItem* pItemOnLine = tableWidget_clientList->item(i, CameraAtribute_bOnline);
            if (NULL == pItemOnLine)
            {
                pItemOnLine = new QTableWidgetItem;
                tableWidget_clientList->setItem(i, CameraAtribute_bOnline, pItemOnLine);
            }
            pItemOnLine->setText(bConnect ? QString::fromLocal8Bit("已连接") : QString::fromLocal8Bit("已断开"));
            pItemOnLine->setIcon(bConnect ? QIcon(":/images/IoOn.png") : QIcon(":/images/IoOff.png"));
            pItemOnLine->setData(Qt::UserRole, bConnect);
        }
    }
    if (m_pSocketServer != NULL)
    {
        enableServerUi(!m_pSocketServer->isServerRunning());
    }
}

void Socketserverui::showEvent( QShowEvent * e )
{
    m_pQTimer->start(50);
    QWidget::showEvent(e);
}

void Socketserverui::hideEvent( QHideEvent * e )
{
    m_pQTimer->stop();
    QWidget::hideEvent(e);
}

void Socketserverui::slotActDisConnect()
{
    int iRowIndex = tableWidget_clientList->currentRow();
    QString strCameraName = tableWidget_clientList->item(iRowIndex, CameraAtribute_CamName)->text();
    if (NULL == m_pSocketServer)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("获取ISMCameraManager实例失败！"), QMessageBox::Ok);
        return;
    }
    CSocketClientPC1* pISMCameraClient = m_pSocketServer->getSocketClient(strCameraName.toStdString());
    if (NULL == pISMCameraClient)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("获取ISMCameraClient实例失败！"), QMessageBox::Ok);
        return;
    }
    pISMCameraClient->disConneced();
}

void Socketserverui::slotActConnect()
{
    int iRowIndex = tableWidget_clientList->currentRow();
    QString strCameraName = tableWidget_clientList->item(iRowIndex, CameraAtribute_CamName)->text();
    if (NULL == m_pSocketServer)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("获取ISMCameraManager实例失败！"), QMessageBox::Ok);
        return;
    }
    CSocketClientPC1* pISMCameraClient = m_pSocketServer->getSocketClient(strCameraName.toStdString());
    if (NULL == pISMCameraClient)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("获取ISMCameraClient实例失败！"), QMessageBox::Ok);
        return;
    }
//    if (!pISMCameraClient->connect())
//    {
//        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), strCameraName + QString::fromLocal8Bit("连接失败！"), QMessageBox::Ok);
//        return;
//    }
}

void Socketserverui::slotActDebug()
{
    QList<QTableWidgetItem*> selectItemList = tableWidget_clientList->selectedItems();
    if (0 == selectItemList.size())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选中要调试的相机的相机！"), QMessageBox::Ok);
        return;
    }
    QString strCameraName = selectItemList[0]->text();

    CommunicationTestForm *pCommunicationTestForm = new CommunicationTestForm(strCameraName);
    pCommunicationTestForm->setParent(this);
    pCommunicationTestForm->setWindowFlags(Qt::Dialog);
    pCommunicationTestForm->setAttribute(Qt::WA_DeleteOnClose);
    pCommunicationTestForm->show();
}

void Socketserverui::slotActAdd()
{
    ClientInfoDlg camDlg(ClientInfoDlg::Create, this);
    if (QDialog::Accepted == camDlg.exec())
    {
        clientCfg stCfg = camDlg.GetUiData();
        if (!m_pSocketServerCfg->addClientCfg(stCfg))
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("增加相机失败！"), QMessageBox::Ok);
            return;
        }
        tableWidget_clientList->setRowCount(tableWidget_clientList->rowCount() + 1);
        SetTableRow(tableWidget_clientList->rowCount() - 1, stCfg);
        tableWidget_clientList->selectRow(tableWidget_clientList->rowCount() - 1);
    }
}

void Socketserverui::slotActDel()
{
    QList<QTableWidgetItem*> selectItemList = tableWidget_clientList->selectedItems();
    if (0 == selectItemList.size())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选中要删除的相机！"), QMessageBox::Ok);
        return;
    }
    int iSelectRow = tableWidget_clientList->row(selectItemList[0]);
    QString strCameraName = selectItemList[0]->text();
    if (QMessageBox::Yes != QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否删除：") + strCameraName, QMessageBox::Yes, QMessageBox::No))
    {
        return;
    }
    if (NULL == m_pSocketServer)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("获取ISMCameraManager实例失败！"), QMessageBox::Ok);
        return;
    }
    if (!m_pSocketServerCfg->delClientCfg(strCameraName.toStdString()))
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("删除相机失败！"), QMessageBox::Ok);
        return;
    }
    tableWidget_clientList->removeRow(iSelectRow);
}

void Socketserverui::slotActAlter()
{
    QList<QTableWidgetItem*> selectItemList = tableWidget_clientList->selectedItems();
    if (0 == selectItemList.size())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选中要删除的相机！"), QMessageBox::Ok);
        return;
    }
    int iSelectRow = tableWidget_clientList->row(selectItemList[0]);
    clientCfg stCfg;
    stCfg.ClientName = tableWidget_clientList->item(iSelectRow, CameraAtribute_CamName)->text().toStdString();
    stCfg.ClientIP    = tableWidget_clientList->item(iSelectRow, CameraAtribute_CamIP   )->text().toStdString();
    stCfg.ClientPort  = tableWidget_clientList->item(iSelectRow, CameraAtribute_CamPort )->text().toInt();
    ClientInfoDlg camDlg(ClientInfoDlg::Alter, this);
    camDlg.SetUiData(stCfg);
    if (QDialog::Accepted == camDlg.exec())
    {
        clientCfg stTagetCfg  = camDlg.GetUiData();;
        if (!m_pSocketServerCfg->ModifyClientCfg(stCfg.ClientName, stTagetCfg))
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("修改相机失败！"), QMessageBox::Ok);
            return;
        }
        SetTableRow(iSelectRow, stTagetCfg);
    }
}

void Socketserverui::on_tableWidget_clientList_customContextMenuRequested( const QPoint & pos )
{
    m_pActConnect->setEnabled(false);
    m_pActDisconnect->setEnabled(false);
    m_pActDebug->setEnabled(false);
    m_pActAdd->setEnabled(true);
    m_pActDel->setEnabled(false);
    m_pActAlter->setEnabled(false);

    do 
    {
        QTableWidgetItem* pItem = tableWidget_clientList->itemAt(pos);
        if (NULL == pItem)
        {
            break;
        }

        QTableWidgetItem* pStatusItem = tableWidget_clientList->item(tableWidget_clientList->row(pItem), CameraAtribute_bOnline);
        if (pStatusItem->data(Qt::UserRole).toBool())
        {
            m_pActDisconnect->setEnabled(true);
            m_pActDebug->setEnabled(true);
        }
        else
        {
            m_pActConnect->setEnabled(true);
            m_pActDel->setEnabled(true);
            m_pActAlter->setEnabled(true);
        }
    } while (0);
    m_pMenu->exec(mapToGlobal(tableWidget_clientList->mapToParent(pos)));
}

void Socketserverui::on_pushButton_clearLog_clicked()
{
    textEdit_log->clear();
}

void Socketserverui::on_pushButton_save_clicked()
{
    if (NULL == m_pSocketServerCfg)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("获取ISMCameraManager实例失败！"), QMessageBox::Ok);
        return;
    }
    serverCfg* pServerCfg = m_pSocketServerCfg->getServerCfg();
    if (pServerCfg != NULL)
    {
        pServerCfg->serverName = lineEdit_server_name->text().toStdString();
        pServerCfg->serverIP = lineEdit_server_ip->text().toStdString();
        pServerCfg->serverPort = lineEdit_server_port->text().toInt();
    }

    if (m_pSocketServerCfg->writeServerConfig())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存成功！"), QMessageBox::Ok);
        return;
    }
    else
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("保存失败！"), QMessageBox::Ok);
        return;
    }
}

void Socketserverui::on_pushButton_startServer_clicked()
{
    if (m_pSocketServer->isServerRunning())
    {
        return;
    }
    m_pSocketServer->initServer();
    if (ERR_OK != m_pSocketServer->startServer())
    {
        textEdit_log->Addlogmsg(QString::fromLocal8Bit("服务器启动失败！"));
    }
    else
    {
        textEdit_log->Addlogmsg(QString::fromLocal8Bit("服务器启动成功！"));
    } 
}

void Socketserverui::on_pushButton_stopServer_clicked()
{
    if (!m_pSocketServer->isServerRunning())
    {
        return;
    }
    if (ERR_OK != m_pSocketServer->stopServer())
    {
        textEdit_log->Addlogmsg(QString::fromLocal8Bit("服务器停止失败！"));
    }
    else
    {
        textEdit_log->Addlogmsg(QString::fromLocal8Bit("服务器停止成功！"));
    }

}

void Socketserverui::enableServerUi( bool bEnable )
{
    lineEdit_server_name->setEnabled(bEnable);
    lineEdit_server_ip->setEnabled(bEnable);
    lineEdit_server_port->setEnabled(bEnable);
}