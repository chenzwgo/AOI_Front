#include "filter.h"
#include "ui_filter.h"
#include "uicommon/extendstackwidget.h"
#include "filevariable/ifilevariable.h"
#include "defect/idefectqualimanage.h"
#include "callsdk.h"
#include "g12common.h"
#include <QTableWidgetItem>
#include <QMessageBox>
#include "expressionparser/scriptLexer.h"
#include "maindefine.h"
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"
#include "callsdk.h"
filter::filter(QDialog *parent) :
	QDialog(parent),
	ui(new Ui::filter),
	m_pExtendStackWidget(NULL),
	m_pVarExpressionWidget(NULL)
{
    ui->setupUi(this);
	m_pExtendStackWidget = new ExtendStackWidget(this);
	m_pVarExpressionWidget = new VarExpressionWidget();
	ui->horizontalLayout_2->addWidget(m_pExtendStackWidget);
	setWindowFlags(Qt::Dialog);// set move and close crl;
	ui->pushButton_modify->setText(QString::fromLocal8Bit("取消"));
    //memset(defectTypeEnableArr, true, sizeof(defectTypeEnableArr));
	
	InitUi();
	m_pExtendStackWidget->setCurrentWidget(0);
	on_pushButton_modify_clicked();
	
	
}

filter::~filter()
{
    delete ui;
}

void filter::InitUi()
{
	
	//IDefectQualification* pIDefectQualification = (IDefectQualification*)GetSDKInstance("defectqualification", IDefectQualification::IDD_DEFECT_QULIFICATION);

	IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p2)
	{
		m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
        m_defectTypeEnableVec = p2->getDefectListInstance(0)->getDefectEnableList();
	}
	//
	IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
	if (NULL != p)
	{
		int maxCnt = getImageStationCnt();

		for (size_t i = 0; i < maxCnt; i++)
		{
			IDefectQualification* pIDefectQualification = p->getDefectQualificationInstance(i);

			if (NULL != pIDefectQualification)
			{
				getenableCfg();
				map<string, vector<stdefectlevelRegion> *>* mapDefect = pIDefectQualification->getDefectLevelParamMap();
				if (NULL != mapDefect)
				{
					map<string, vector<stdefectlevelRegion> * >::iterator it = mapDefect->begin();
					while (it != mapDefect->end())
					{
						if (m_mapDefectEnable[it->first])
						{
							FilterSet* pDefectItem = NULL;
							QString strTabText = (QString::fromStdString(pIDefectQualification->getDefectDesByName(it->first)));
							ExtendStackWidget* pExtendStackWidget = new ExtendStackWidget(this);
							for (int i = 0; i < it->second->size() && i <DEFECT_FLAG_SIZE; i++)
							{
								pDefectItem = new FilterSet(it->second->at(i).pDefectlevelVec, it->second->at(i).iRegion, this);
								pExtendStackWidget->addWidget(QString::fromLocal8Bit(g_strFlag[i]), pDefectItem);
								m_vecFilterSet.push_back(pDefectItem);
							}
							pExtendStackWidget->setCurrentWidget(0);
							m_pExtendStackWidget->addWidget(strTabText, pExtendStackWidget);
						}
						it++;
					}
				}
				m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("变量组合"), m_pVarExpressionWidget);
				m_vecFilterSet.push_back(m_pVarExpressionWidget);
			}
		}
	}
	//


}
void filter::on_pushButton_save_clicked()
{
	//IDefectQualification* pIDefectQualification = (IDefectQualification*)GetSDKInstance("defectqualification", IDefectQualification::IDD_DEFECT_QULIFICATION);

	//

	IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p2)
	{
		m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
        m_defectEnNameVec = p2->getDefectListInstance(0)->getDefectEnNameList();
        m_defectTypeEnableVec = p2->getDefectListInstance(0)->getDefectEnableList();
	}
	IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
	if (NULL != p)
	{
		int maxCnt = getImageStationCnt();

		for (size_t j = 0; j < maxCnt; j++)
		{
			IDefectQualification* pIDefectQualification = p->getDefectQualificationInstance(j);

			if (NULL != pIDefectQualification)
			{
				for (int i = 0; i < m_vecFilterSet.size(); i++)
				{
					FilterSet* pFilterSet = qobject_cast<FilterSet*>(m_vecFilterSet.at(i));
					if (pFilterSet != NULL)
					{
						pFilterSet->getUiVale();
					}

					VarExpressionWidget* pExpressionWidget = qobject_cast<VarExpressionWidget*>(m_vecFilterSet.at(i));
					if (pExpressionWidget != NULL)
					{
						pExpressionWidget->getUiVale();
					}
				}
				pIDefectQualification->saveDefectLevelParam();
			}
		}
	}
	setUisetEnabled(false);
	ui->pushButton_save->hide();
	ui->pushButton_modify->setText(QString::fromLocal8Bit("修改"));
}

void filter::on_pushButton_modify_clicked()
{
	bool bEn = false;
	if (QString::fromLocal8Bit("修改") == ui->pushButton_modify->text())
	{
		ui->pushButton_modify->setText(QString::fromLocal8Bit("取消"));
		bEn = true;
		ui->pushButton_save->show();
	}
	else
	{
		for (int i = 0; i < m_vecFilterSet.size(); i++)
		{
			FilterSet* pFilterSet = qobject_cast<FilterSet*>(m_vecFilterSet.at(i));
			if (pFilterSet != NULL)
			{
				pFilterSet->UiCancel();
			}
			VarExpressionWidget* pExpressionWidget = qobject_cast<VarExpressionWidget*>(m_vecFilterSet.at(i));
			if (pExpressionWidget != NULL)
			{
				pExpressionWidget->UiCancel();
			}
		}
		ui->pushButton_modify->setText(QString::fromLocal8Bit("修改"));
		ui->pushButton_save->hide();
	}
	setUisetEnabled(bEn);
}

void filter::setUisetEnabled(bool bEnabled)
{
	for (int i = 0; i < m_vecFilterSet.size(); i++)
	{
		m_vecFilterSet.at(i)->setEnabled(bEnabled);
	}
}

void filter::getenableCfg()
{
	
	IFileVariable *pFileVariable = getQualificationCfgFileVariable();
	if (pFileVariable == NULL)
	{
		return;
    }
	for (int i = 0; i < m_defectNameVec.size(); ++i)
	{
        m_mapDefectEnable.insert(pair<string, bool>(m_defectNameVec[i], m_defectTypeEnableVec[i].second));
	}
}


VarExpressionWidget::VarExpressionWidget(QWidget *parent /*= 0*/):m_pExpressionTableWidget(NULL),
m_pitem(NULL)
{
	QVBoxLayout *pQVBoxLayout = new QVBoxLayout();
	setLayout(pQVBoxLayout);
	m_pExpressionTableWidget = new QTableWidget();
	pQVBoxLayout->addWidget(m_pExpressionTableWidget);
	m_pExpressionTableWidget->setColumnCount(3); //设置列数
	m_pExpressionTableWidget->setRowCount(50);
	m_pExpressionTableWidget->horizontalHeader()->setDefaultSectionSize(150);

	//设置表头内容
	QStringList header;
	header << QString::fromLocal8Bit("变量名") << QString::fromLocal8Bit("描述") << QString::fromLocal8Bit("表达式(Area,Score,Contrast,Width,Height,SkeletonLength,PosX,PoxY)");
	m_pExpressionTableWidget->setHorizontalHeaderLabels(header);

	//设置表头字体加粗
	QFont font = m_pExpressionTableWidget->horizontalHeader()->font();
	font.setBold(true);
	m_pExpressionTableWidget->horizontalHeader()->setFont(font);

	m_pExpressionTableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
	m_pExpressionTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_pExpressionTableWidget->verticalHeader()->setDefaultSectionSize(10); //设置行高
	m_pExpressionTableWidget->setEditTriggers(QAbstractItemView::DoubleClicked); //设置不可编辑
	m_pExpressionTableWidget->horizontalHeader()->resizeSection(0, 150); //设置表头第一列的宽度为150
	m_pExpressionTableWidget->horizontalHeader()->setFixedHeight(25); //设置表头的高度


	connect(m_pExpressionTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(openEditor(QTableWidgetItem*)));
	connect(m_pExpressionTableWidget, SIGNAL(itemSelectionChanged()),this, SLOT(closeEditor()));
	initTableWidget();
}

VarExpressionWidget::~VarExpressionWidget()
{

}

void VarExpressionWidget::initTableWidget()
{
	//IDefectQualification* pIDefectQualification = (IDefectQualification*)GetSDKInstance("defectqualification", IDefectQualification::IDD_DEFECT_QULIFICATION);

	IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
	if (NULL != p)
	{
		int maxCnt = getImageStationCnt();

		for (size_t i = 0; i < maxCnt; i++)
		{
			IDefectQualification* pIDefectQualification = p->getDefectQualificationInstance(i);

			if (NULL != pIDefectQualification)
			{
				m_pExpressionTableWidget->clearContents();
				const map<string, stVarExpression> *pVarExpressionMap = pIDefectQualification->getVarExpressionParam();
				map<string, stVarExpression>::const_iterator it = pVarExpressionMap->begin();
				int  iRow = 0;
				while (it != pVarExpressionMap->end())
				{
					m_pExpressionTableWidget->setItem(iRow, 0, new QTableWidgetItem(QString::fromStdString(it->second.varName)));
					m_pExpressionTableWidget->setItem(iRow, 1, new QTableWidgetItem(QString::fromStdString(it->second.varDes)));
					m_pExpressionTableWidget->setItem(iRow, 2, new QTableWidgetItem(QString::fromStdString(it->second.varExp)));
					it++;
					iRow++;
				}
			}
		}
	}
}

void VarExpressionWidget::UiCancel()
{
	initTableWidget();
}

void VarExpressionWidget::getUiVale()
{
	IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
	if (NULL != p)
	{
		int maxCnt = getImageStationCnt();;
		for (size_t i = 0; i < maxCnt; i++)
		{
			IDefectQualification* pIDefectQualification = p->getDefectQualificationInstance(i);
			if (NULL != pIDefectQualification)
			{
				map<string, stVarExpression> mapVarExpression;
				int iRow = m_pExpressionTableWidget->rowCount();
				for (int i = 0; i < iRow; i++)
				{
					QTableWidgetItem *itemVarName = m_pExpressionTableWidget->item(i, 0);
					QTableWidgetItem *itemVarDes = m_pExpressionTableWidget->item(i, 1);
					QTableWidgetItem *itemVarExp = m_pExpressionTableWidget->item(i, 2);
					if (itemVarName == NULL || itemVarName->text().length() == 0)
					{
						continue;
					}
					if (itemVarExp == NULL || itemVarExp->text().length() == 0)
					{
						continue;
					}
					stVarExpression tmp;
					if (itemVarName->text().length() >0)
					{
						tmp.varName = itemVarName->text().toStdString();
					}
					if (itemVarDes != NULL && itemVarDes->text().length() >0)
					{
						tmp.varDes = itemVarDes->text().toStdString();
					}
					if (itemVarExp->text().length() >0)
					{
						tmp.varExp = itemVarExp->text().toStdString();
					}
					if (mapVarExpression.find(tmp.varName) != mapVarExpression.end())
					{
						QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("已经存在 %1").arg(QString::fromStdString(tmp.varName)));
						return;
					}
					mapVarExpression.insert(pair<string, stVarExpression>(tmp.varName, tmp));
				}
				pIDefectQualification->setVarExpressionParam(&mapVarExpression);
			}
		}
	}
}

void VarExpressionWidget::openEditor(QTableWidgetItem* item)
{
	m_pitem = NULL;
	m_pExpressionTableWidget->openPersistentEditor(item);// 设置某一item可以编辑
	m_pitem = item;
	m_itemName = item->text();
}

void VarExpressionWidget::closeEditor()
{
	if (m_pitem != NULL )
	{
		m_pExpressionTableWidget->closePersistentEditor(m_pitem);// 设置某一item不可编辑
		QString strName = m_pitem->text();
		if (strName.length() == 0)
		{
			return;
		}

		int iCol = m_pitem->column();
		if (iCol == 0)
		{
			int iRow = m_pExpressionTableWidget->rowCount();
			for (int i = 0; i < iRow; i++)
			{
				QTableWidgetItem *itemVarName = m_pExpressionTableWidget->item(i, 0);
				if (itemVarName != NULL && m_pitem != itemVarName && m_pitem->text() == itemVarName->text() && !strName.isEmpty())
				{
					QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("已经存在 %1").arg(strName));
					m_pitem->setText(m_itemName);
					return;
				}
			}
		}
		else if (iCol == 2)
		{
			ScriptLexer	ScriptLevel ;
			string strExpression = strName.toStdString();
			strExpression.append("\n");
			ScriptLevel.setParserString(strExpression);
			if (0 != ScriptLevel.parser())
			{
				QString strError = QString::fromLocal8Bit("表达式格式错误 %1 ").arg(QString::fromStdString(ScriptLevel.getErrString()));
				QMessageBox::warning(this, "Warning", strError);
				m_pitem->setText(m_itemName);
				return ;
			}
		}
	}
}

