#include "usersetdlg.h"
#include "ui_usersetdlg.h"
#include <QMessageBox>
#include <QKeyEvent>
#include "usermanagercfg.h"

UserSetDlg* UserSetDlg::m_pSelfInstance = NULL;
UserSetDlg::UserSetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSetDlg),
    m_bChang(false),
    m_pCurQAbstractButton(NULL),
    m_pMenu(NULL),
    m_pAddAction(NULL),
    m_pDelAction(NULL)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pUserManagerCfg = UserManagerCfg::GetInstance();
    m_pMenu = new QMenu(this);
    m_pAddAction = new QAction(QString::fromLocal8Bit("增加用户"), this);
    m_pDelAction = new QAction(QString::fromLocal8Bit("删除用户"), this);
    m_pMenu->addAction(m_pAddAction);
    m_pMenu->addAction(m_pDelAction);

    connect(m_pAddAction, SIGNAL(triggered()), this, SLOT(SlotAddUser()));
    connect(m_pDelAction, SIGNAL(triggered()), this, SLOT(SlotDelUser()));
    Init();
}

UserSetDlg::~UserSetDlg()
{
    m_pSelfInstance = NULL;
    delete ui;
}

bool UserSetDlg::Init()
{
    ui->toolBtnPro->setChecked(true);
    m_pCurQAbstractButton = ui->toolBtnPro;
    InitTableValue(PRODUCTION_MODELS);
    return true;
}

UserSetDlg* UserSetDlg::GetInstance( QWidget* pParent /*= NULL*/ )
{
    if (NULL == m_pSelfInstance)
    {
        m_pSelfInstance = new UserSetDlg(pParent);
    }
    return m_pSelfInstance;
}

void UserSetDlg::ReleaseInstance()
{
    if (NULL != m_pSelfInstance)
    {
        delete m_pSelfInstance;
        m_pSelfInstance = NULL;
    }
}

void UserSetDlg::on_tableWidget_itemChanged( QTableWidgetItem * item )
{
    m_bChang = true;
}

void UserSetDlg::keyPressEvent( QKeyEvent *e )
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

void UserSetDlg::closeEvent( QCloseEvent *event )
{
    if (m_bChang)
    {
        QString strTempModel = GetCurModel();
        int ret = QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否保存当前设置？"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (QMessageBox::Yes == ret)
        {
            /*if(!SaveComp(strTempModel))
            {
                QMessageBox::warning(this, "错误", "参数保存失败!");
            }*/
            m_bChang = false;
        }
        else if (QMessageBox::No == ret)
        {
            //InitTreeAtribute(strTempModel);
            m_bChang = false;
        }
        else if (QMessageBox::Cancel == ret)
        {
            event->ignore();
        }
    }
}

QString UserSetDlg::GetCurModel(QAbstractButton* pQAbstractButton/* = NULL*/)
{
    QString strModel;

    if (NULL != pQAbstractButton)
    {
        if (ui->toolBtnPro == pQAbstractButton)
        {
            strModel = PRODUCTION_MODELS;
        }
		else if (ui->toolBtnDebug == pQAbstractButton)
		{
			strModel = DEBUG_MODELS;
		}
        else if (ui->toolBtnEngineer == pQAbstractButton)
        {
            strModel = ENGINEER_MODELS;
        }

        return strModel;
    }

    if (ui->toolBtnPro->isChecked())
    {
        strModel = PRODUCTION_MODELS;
    }
	else if (ui->toolBtnDebug->isChecked())
	{
		strModel = DEBUG_MODELS;
	}
    else if (ui->toolBtnEngineer->isChecked())
    {
        strModel = ENGINEER_MODELS;
    }
    return strModel;
}

void UserSetDlg::on_btnSet_clicked()
{
    QString strCurModel = GetCurModel();
    if (strCurModel.isEmpty())
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("当前模式为空！"));
        return;
    }

    if(SaveUserInfo(strCurModel))
    {
        m_bChang = false;
    }
}

void UserSetDlg::on_btnBack_clicked()
{
    QString strModel = GetCurModel();
    InitTableValue(strModel);
    m_bChang = false;
}

void UserSetDlg::on_buttonGroup_buttonClicked( QAbstractButton * button )
{
    if (m_pCurQAbstractButton == button)
    {
        return;
    }
    if (m_bChang)
    {
        QMessageBox::StandardButton iRes = QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否保存当前设置？"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (QMessageBox::Yes == iRes)
        {
            QString strTempModel = GetCurModel(m_pCurQAbstractButton);
            if (!SaveUserInfo(strTempModel))
            {
                m_pCurQAbstractButton->setChecked(true);
                return;
            }
        }
        else if (QMessageBox::No == iRes)
        {

        }
        else if (QMessageBox::Cancel == iRes)
        {
            m_pCurQAbstractButton->setChecked(true);
            return;
        }
    }

    QString strModel = GetCurModel();

    m_pCurQAbstractButton = button;
    m_bChang = false;

    InitTableValue(strModel);
}

bool UserSetDlg::InitTableValue( QString strModels )
{
    QMap<QString, QMap<QString, QString> >* pMapUserInfo = m_pUserManagerCfg->GetUserInfo();
    if (pMapUserInfo == NULL)
    {
        return false;
    }

    QMap<QString, QMap<QString, QString> >::Iterator tempIter = pMapUserInfo->find(strModels);
    if (tempIter == pMapUserInfo->end())
    {
        return false;
    }

    ui->tableWidget->blockSignals(true);
    ui->tableWidget->setRowCount(tempIter.value().count());
    QMap<QString, QString>::Iterator tempUserIter = tempIter.value().begin();
    for (int iRow = 0; tempUserIter != tempIter.value().end(); ++tempUserIter, ++iRow)
    {
        QTableWidgetItem* pNameItem = new QTableWidgetItem(tempUserIter.key());
        QTableWidgetItem* pPwdItem  = new QTableWidgetItem(tempUserIter.value());
        ui->tableWidget->setItem(iRow, 0, pNameItem);
        ui->tableWidget->setItem(iRow, 1, pPwdItem);
    }
    ui->tableWidget->blockSignals(false);
    return true;
}

void UserSetDlg::on_tableWidget_customContextMenuRequested( const QPoint & pos )
{
    
    QTableWidgetItem* pItem = ui->tableWidget->itemAt(pos);
    if (NULL == pItem)
    {
        m_pAddAction->setEnabled(true);
        m_pDelAction->setEnabled(false);
    }
    else
    {
        m_pAddAction->setEnabled(true);
        m_pDelAction->setEnabled(true);
    }

    m_pMenu->exec(mapToGlobal(ui->tableWidget->mapToParent(pos)));
}

bool UserSetDlg::SaveUserInfo( QString strModel )
{
    QMap<QString, QString> mapUser;
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        QString strName = ui->tableWidget->item(i, 0)->text();
        if (strName.isEmpty() || mapUser.find(strName) != mapUser.end())
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("用户名为空，或用户名已存在！"));
            ui->tableWidget->item(i, 0)->setBackgroundColor(QColor(255,0,0));
            ui->tableWidget->setCurrentItem(NULL);
            return false;
        }

        QString strPwd  = ui->tableWidget->item(i, 1)->text();
        if (strPwd.isEmpty())
        {
            QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("密码不能为空！"));
            ui->tableWidget->item(i, 1)->setBackgroundColor(QColor(255,0,0));
            ui->tableWidget->setCurrentItem(NULL);
            return false;
        }
        ui->tableWidget->item(i, 0)->setBackgroundColor(QColor(255,255,255));
        ui->tableWidget->item(i, 1)->setBackgroundColor(QColor(255,255,255));

        mapUser.insert(strName, strPwd);
    }

    if (m_pUserManagerCfg->SetUserInfo(strModel, mapUser))
    {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存成功！"));
        return true;
    }
    else
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("保存失败！"));
        return false;
    }
    return true;
}

void UserSetDlg::SlotAddUser()
{
    int iLastRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(iLastRow);
    QTableWidgetItem* pNameItem = new QTableWidgetItem();
    QTableWidgetItem* pPwdItem  = new QTableWidgetItem();
    ui->tableWidget->setItem(iLastRow, 0, pNameItem);
    ui->tableWidget->setItem(iLastRow, 1, pPwdItem);
    ui->tableWidget->setCurrentItem(pNameItem);
}

void UserSetDlg::SlotDelUser()
{
    QString strModel = GetCurModel();
    if (ENGINEER_MODELS == strModel)
    {
        if (ui->tableWidget->rowCount() == 1)
        {
            QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("不能删除唯一的管理员用户！"));
            return;
        }
    }

    int iCurRow = ui->tableWidget->currentRow();
    if (iCurRow < 0)
    {
        return;
    }
    ui->tableWidget->removeRow(iCurRow);
    m_bChang = true;
}
