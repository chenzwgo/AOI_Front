#include "competencesetdlg.h"
#include "ui_competencesetdlg.h"
#include "usermanagercfg.h"
#include <QMessageBox>
#include <QKeyEvent>

CompetenceSetDlg* CompetenceSetDlg::m_pSelfInstance = NULL;
CompetenceSetDlg::CompetenceSetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompetenceSetDlg),
    m_pUserManagerCfg(NULL),
    m_bChang(false),
    m_pCurQAbstractButton(NULL)
{
    ui->setupUi(this);
    m_pUserManagerCfg = UserManagerCfg::GetInstance();
    SetTreeWidgetStyle();
    setAttribute(Qt::WA_DeleteOnClose, true);

    Init();
}

CompetenceSetDlg::~CompetenceSetDlg()
{
    m_pSelfInstance = NULL;
    delete ui;
}

CompetenceSetDlg* CompetenceSetDlg::GetInstance( QWidget* pParent /*= NULL*/ )
{
    if (NULL == m_pSelfInstance)
    {
        m_pSelfInstance = new CompetenceSetDlg(pParent);
    }
    return m_pSelfInstance;
}

void CompetenceSetDlg::ReleaseInstance()
{
    if (NULL != m_pSelfInstance)
    {
        delete m_pSelfInstance;
        m_pSelfInstance = NULL;
    }
}

void CompetenceSetDlg::SetTreeWidgetStyle()
{
    /*item样式*/
    ui->treeWidget->setStyleSheet(QString::fromUtf8(
        "QTreeWidget{\n"
        "      alternate-background-color: rgb(225, 225, 225);\n"
        "      background-color: rgb(255, 255, 248);\n"
        "      border-color: rgb(195, 195, 195);\n"
        "}\n"
        "::item{\n"
        "    border: 1px solid;\n"
        "    border-color: rgb(195, 195, 195);\n"
        "    border-top-color: rgba(195, 195, 195,0);\n"
        "    border-right-color: rgb(195, 195, 195);\n"
        "    border-bottom-color: rgb(195, 195, 195);\n"
        "    border-left-color: rgba(195, 195, 195, 0);\n"
        "    height:22px;\n"
        "}"
        "::item:hover {\n"
        "    background: rgba(69, 187, 217, 80);\n"  
        "}" 
        "::item:selected:active{\n"
        "    background: rgb(69, 187, 217); \n"
        "}"
        "::item:selected:!active { \n" 
        "     background: rgba(69, 187, 217, 180); \n"
        "}"
        "::branch{ " 
        "    border: 1px solid;\n"
        "    border-top-color: rgba(195, 195, 195,0);\n"
        "    border-right-color: rgba(195, 195, 195,0);"
        "    border-bottom-color: rgb(195, 195, 195);\n"
        "    border-left-color: rgba(195, 195, 195, 0);\n"
        "}"
        "::branch:closed:has-children:!has-siblings,\n"  
        "::branch:closed:has-children:has-siblings { \n" 
        "    border-image: none;\n"  
        "    image: url(:/image/plus.png);\n"
        "}" 
        "::branch:closed:has-children:has-children { \n" 
        "    border-image: none;\n"  
        "    image: url(:/image/plus.png);\n"
        "}"
        "::branch:closed:has-children:!has-children { \n" 
        "    border-image: none;\n"  
        "    image: none;\n"
        "}"
        "::branch:open:has-children:!has-siblings,\n"  
        "::branch:open:has-children:has-siblings { \n" 
        "    border-image: none;\n"  
        "    image: url(:/image/minus.png);\n"  
        "}"
        "::branch:open:has-children:!has-siblings:adjoins-item,\n"  
        "::branch:open:has-children:has-siblings:adjoins-item { \n" 
        "    border-image: none;\n"  
        "    image: url(:/image/minus.png);\n"  
        "}"
        ));
    ui->treeWidget->setColumnWidth(0,200);
}

bool CompetenceSetDlg::Init()
{
    InitTree(MODE_PRODUCTION);

    ui->toolBtnPro->setChecked(true);
    m_pCurQAbstractButton = ui->toolBtnPro;
    return true;
}

void CompetenceSetDlg::on_treeWidget_itemChanged( QTreeWidgetItem * item, int column )
{
    m_bChang = true;
    for (int i = 0; i < item->childCount(); ++i)
    {
        item->child(i)->setCheckState(column, item->checkState(column));
    }
}

bool CompetenceSetDlg::InitTree(int iModel)
{
    QMap<QString, STWidgetInfo>* pVecWidgetID = m_pUserManagerCfg->GetWidgetID();
    ui->treeWidget->blockSignals(true);
    ui->treeWidget->clear();

	QMap<QString, STWidgetInfo>::Iterator itWidgetInfo = pVecWidgetID->begin();
	for (; itWidgetInfo != pVecWidgetID->end(); itWidgetInfo++)
	{
		QTreeWidgetItem* pTreeWidget = new QTreeWidgetItem(ui->treeWidget);
		pTreeWidget->setText(0, itWidgetInfo->stWidgetItem.strDesc);
		pTreeWidget->setToolTip(0, itWidgetInfo->stWidgetItem.strObjectName);
		pTreeWidget->setCheckState(0, itWidgetInfo->stWidgetItem.bDisable[iModel]? Qt::Checked : Qt::Unchecked);
		pTreeWidget->setTextColor(0, QColor(0,0,255));
		QFont qFont = pTreeWidget->font(0);
		qFont.setPixelSize(15);
		qFont.setBold(true);
		pTreeWidget->setFont(0, qFont);
		pTreeWidget->setExpanded(true);
		ui->treeWidget->insertTopLevelItem(0, pTreeWidget);
		if (itWidgetInfo->mapItem.size() == 0)
		{
			continue;
		}
		QMap<QString, STWidgetInfo*>::Iterator tempIter = itWidgetInfo->mapItem.begin();
		for (; tempIter != itWidgetInfo->mapItem.end(); ++tempIter)
		{
			InitTreeItem(iModel, pTreeWidget, tempIter.value());
		}
	}

    ui->treeWidget->blockSignals(false);
    return true;
}

bool CompetenceSetDlg::InitTreeItem(int iModel, QTreeWidgetItem* pTreeItem, STWidgetInfo* pInfo, bool bTopItem/*=false*/ )
{
	QTreeWidgetItem* pTreeWidgetItem = NULL;
	if (!bTopItem)
	{
		pTreeWidgetItem = new QTreeWidgetItem(pTreeItem);
		pTreeWidgetItem->setText(0, pInfo->stWidgetItem.strDesc);
		pTreeWidgetItem->setToolTip(0, pInfo->stWidgetItem.strObjectName);
		pTreeWidgetItem->setCheckState(0, pInfo->stWidgetItem.bDisable[iModel]? Qt::Checked : Qt::Unchecked);
		pTreeItem->addChild(pTreeWidgetItem);
	}
	else
	{
		pTreeWidgetItem = pTreeItem;
	}

	if (pInfo->mapItem.size() == 0)
	{
		return true;
	}

	QMap<QString, STWidgetInfo*>::Iterator itInfo = pInfo->mapItem.begin();
	for (; itInfo != pInfo->mapItem.end(); itInfo++)
	{
		InitTreeItem(iModel, pTreeWidgetItem, itInfo.value());
	}
	return true;
}

int CompetenceSetDlg::GetCurModel(QAbstractButton* pQAbstractButton/* = NULL*/)
{
    int iModel;

    if (NULL != pQAbstractButton)
    {
        if (ui->toolBtnLogout == pQAbstractButton)
        {
            iModel = MODE_LOGOUT;
        }
        else if (ui->toolBtnPro == pQAbstractButton)
        {
            iModel = MODE_PRODUCTION;
        }
		else if (ui->toolBtnDebug == pQAbstractButton)
		{
			iModel = MODE_DEBUG;
		}
        else if (ui->toolBtnManager == pQAbstractButton)
        {
            iModel = MODE_ENGINEER;
        }
		else
		{
			iModel = MODE_ROOT;
		}
        return iModel;
    }

    if (ui->toolBtnLogout->isChecked())
    {
        iModel = MODE_LOGOUT;
    }
    else if (ui->toolBtnPro->isChecked())
    {
        iModel = MODE_PRODUCTION;
    }
    else if (ui->toolBtnDebug->isChecked())
    {
        iModel = MODE_DEBUG;
    }
    else if (ui->toolBtnManager->isChecked())
    {
        iModel = MODE_ENGINEER;
	}
	else
	{
		iModel = MODE_ROOT;
	}
    return iModel;
}

void CompetenceSetDlg::on_buttonGroup_buttonClicked( QAbstractButton * button )
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
            if (!SaveComp(GetCurModel(m_pCurQAbstractButton)))
            {
                m_pCurQAbstractButton->setChecked(true);
                return;
            }
        }
        else if (QMessageBox::Cancel == iRes)
        {
            m_pCurQAbstractButton->setChecked(true);
            return;
        }
    }
	m_bChang = false;
	m_pCurQAbstractButton = button;
	int iModel = GetCurModel(button);
	ui->treeWidget->clear();
	InitTree(iModel);

}

void CompetenceSetDlg::on_btnSet_clicked()
{
    if(SaveComp(GetCurModel()))
    {
        m_bChang = false;
    }
}

bool CompetenceSetDlg::SaveComp( int iModel )
{
	QMap<QString, STWidgetInfo>* pVecWidgetID = m_pUserManagerCfg->GetWidgetID();
	if (pVecWidgetID == NULL)
	{
		return false;
	}

    QMap<QString, QStringList> mapComp;
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
		QTreeWidgetItem* pTopItem = ui->treeWidget->topLevelItem(i);
		QString strObjectName = pTopItem->toolTip(0);
		QMap<QString, STWidgetInfo>::Iterator itWidgetID = pVecWidgetID->find(strObjectName);
		if (itWidgetID != pVecWidgetID->end())
		{
			SaveWidgetInfo(iModel, pTopItem, &(*itWidgetID));
		}

    }

    if (m_pUserManagerCfg->SaveWidgetInfo())
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

bool CompetenceSetDlg::SaveWidgetInfo( int iModel, QTreeWidgetItem* pTreeWidget, STWidgetInfo* pInfo )
{
	QString strObjectName = pTreeWidget->toolTip(0);
	if (strObjectName == pInfo->stWidgetItem.strObjectName)
	{
		bool bDisable = Qt::Checked == pTreeWidget->checkState(0) ? true : false;
		pInfo->stWidgetItem.bDisable[iModel] = bDisable;

		for (int j = 0; j < pTreeWidget->childCount(); ++j)
		{
			QTreeWidgetItem* pChildItem = pTreeWidget->child(j);
			QString strItemName = pChildItem->toolTip(0);
			QMap<QString, STWidgetInfo*>::Iterator itWidgetInfo = pInfo->mapItem.find(strItemName);
			if (itWidgetInfo != pInfo->mapItem.end())
			{
				SaveWidgetInfo(iModel, pChildItem, *itWidgetInfo);
			}
		}
	}
	return true;
}

void CompetenceSetDlg::on_btnBack_clicked()
{
	int iModel = GetCurModel(m_pCurQAbstractButton);
	ui->treeWidget->clear();
	InitTree(iModel);
    m_bChang = false;
}

void CompetenceSetDlg::keyPressEvent( QKeyEvent *e )
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

void CompetenceSetDlg::closeEvent( QCloseEvent *event )
{
    if (m_bChang)
    {
        int iModel = GetCurModel();
        int ret = QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("是否保存当前设置？"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (QMessageBox::Yes == ret)
        {
            if(!SaveComp(iModel))
            {
                QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("参数保存失败!"));
            }
            m_bChang = false;
        }
        else if (QMessageBox::No == ret)
        {
            m_bChang = false;
        }
        else if (QMessageBox::Cancel == ret)
        {
            event->ignore();
        }
    }
}


