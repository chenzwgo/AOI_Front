#include "defectitemset.h"
#include "expressionparser/scriptLexer.h"
#include "defect/idefectqualification.h"
#include "defect/idefectqualimanage.h"
#include "proxymanager.h"
#include "callsdk.h"
#include <string> 
#include <QMessageBox>
#include <QTime>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"
#include "callsdk.h"

#define  TREE_COL_INDEX_DES			  0
#define  TREE_COL_INDEX_EXPRESSION    1

DefectItemSet::DefectItemSet(int iStation, string strChannel ,vector<defectlevel>* VecDefect, int iRegion,QWidget *parent) :
    QWidget(parent),
	m_iRegion(iRegion),
	m_VecCurrData(NULL),
	m_pScriptLevel(NULL),
	m_ptypeitem(NULL),
	m_pIDefectQualification(NULL),
	m_pCurrItem(NULL),
	m_strChannel(strChannel)
{
	m_iStation = iStation;
	m_pverticalLayout = new QVBoxLayout(this);
	m_pverticalLayout->setSpacing(0);
	m_pverticalLayout->setObjectName(QStringLiteral("verticalLayout"));
	m_pverticalLayout->setContentsMargins(0, 0, 0, 0);
	m_ptreeWidget = new QTreeWidget(this);
	m_pverticalLayout->addWidget(m_ptreeWidget);
	QTreeWidgetItem *pheaderItem = m_ptreeWidget->headerItem();
	pheaderItem->setText(0, QString::fromLocal8Bit("等级"));
	pheaderItem->setText(1, QString::fromLocal8Bit("条件  (面积(Area), 得分(Score), 对比度(Contrast), 宽度(Width),  高度(Height), 长边(SkeletonLength), 位置X(PosX), 位置Y(PoxY)"));
	pheaderItem->setToolTip(1, QString::fromLocal8Bit("条件  (面积(Area), 得分(Score), 对比度(Contrast), 宽度(Width),  高度(Height), 长边(SkeletonLength), 位置X(PosX), 位置Y(PoxY)"));

	setLayout(m_pverticalLayout);
	
	m_VecCurrData = VecDefect;
	m_pContextMenu = new QMenu(this);
	m_pActAdd = new QAction(QString::fromLocal8Bit("添加"), this);
	m_pActDelete = new QAction(QString::fromLocal8Bit("删除"), this);
	m_pActCopy = new QAction(QString::fromLocal8Bit("复制"), this);
	m_pActPaste = new QAction(QString::fromLocal8Bit("粘贴"), this);
	m_pActFullExp = new QAction(QString::fromLocal8Bit("获取完整语句"), this);
	m_pActExec = new QAction(QString::fromLocal8Bit("此项测试"), this);
	m_pActLevelExec = new QAction(QString::fromLocal8Bit("整个等级判定测试"), this);

	connect(m_pActAdd, SIGNAL(triggered()), this, SLOT(slotActAdd()));
	connect(m_pActDelete, SIGNAL(triggered()), this, SLOT(slotActDelete()));
	connect(m_pActCopy, SIGNAL(triggered()), this, SLOT(slotActCopy()));
	connect(m_pActPaste, SIGNAL(triggered()), this, SLOT(slotActPaste()));
	connect(m_pActFullExp, SIGNAL(triggered()), this, SLOT(slotActFullExp()));
	connect(m_pActExec, SIGNAL(triggered()), this, SLOT(slotActExec()));
	connect(m_pActLevelExec, SIGNAL(triggered()), this, SLOT(slotActLevelExec()));
	m_ptreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_ptreeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(popMenu(const QPoint&)));

	m_pContextMenu->addAction(m_pActAdd);
	m_pContextMenu->addAction(m_pActDelete);
	m_pContextMenu->addAction(m_pActFullExp);
	m_pContextMenu->addAction(m_pActExec);
	m_pContextMenu->addAction(m_pActLevelExec);
	connect(m_ptreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),this, SLOT(slotitemDoubleClicked(QTreeWidgetItem*, int)));
	connect(m_ptreeWidget, SIGNAL(itemSelectionChanged()),this, SLOT(slotitemSelectionChanged()));

	if (NULL == m_pIDefectQualification)
	{
		IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
		if (NULL != p)
		{
			m_pIDefectQualification = p->getDefectQualificationInstance(m_iStation);
		}
	}
	IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (p2 != NULL)
	{
		m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
	}
	InitUi();
}

DefectItemSet::~DefectItemSet()
{
	m_VecCurrData = NULL;
}

void DefectItemSet::InitUi()
{ 
	if (NULL == m_VecCurrData)
	{
		return;
	}
	
	// clear
	m_ptreeWidget->clear();

	for (int i = 0; i < m_VecCurrData->size();i++)
	{
		defectlevel* pDefect = &m_VecCurrData->at(i);
		m_strDefectName = pDefect->strDefectName;
		if (NULL != pDefect)
		{
			QTreeWidgetItem* pItemLevel = new QTreeWidgetItem;
			pItemLevel->setText(TREE_COL_INDEX_DES, QString::fromLocal8Bit("等级%1").arg(i+1));
			QFont tmpFont = pItemLevel->font(TREE_COL_INDEX_DES);
			pItemLevel->setTextColor(TREE_COL_INDEX_DES, QColor(0, 48, 70));
			tmpFont.setPointSize(12);
			pItemLevel->setFont(TREE_COL_INDEX_DES, tmpFont);
			Qt::CheckState state = pDefect->bEnable ? Qt::Checked : Qt::Unchecked;
			pItemLevel->setCheckState(0, state);
			m_ptreeWidget->addTopLevelItem(pItemLevel);
			for (int j = 0; j < pDefect->defectlevelItemVec.size();j++)
			{
				QTreeWidgetItem* pItem = new QTreeWidgetItem;
				pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable| Qt::ItemIsSelectable | Qt::ItemIsEditable);

				Qt::CheckState state = pDefect->defectlevelItemVec.at(j).bEnable ? Qt::Checked : Qt::Unchecked;
				pItem->setCheckState(TREE_COL_INDEX_DES, state);
				QString strItemDes = QString::fromStdString(pDefect->defectlevelItemVec.at(j).strItemDes);
				if (strItemDes.isEmpty())
				{
					strItemDes = QString::number(j);
				}
				
				pItem->setText(TREE_COL_INDEX_DES, QString::fromStdString(pDefect->defectlevelItemVec.at(j).strItemDes));
				pItem->setText(TREE_COL_INDEX_EXPRESSION, QString::fromStdString(pDefect->defectlevelItemVec.at(j).strItemExpression));
				pItem->setData(TREE_COL_INDEX_DES, Qt::UserRole, QString::fromStdString(pDefect->defectlevelItemVec.at(j).strItemDes));
				pItem->setData(TREE_COL_INDEX_EXPRESSION, Qt::UserRole, QString::fromStdString(pDefect->defectlevelItemVec.at(j).strItemExpression));

				pItemLevel->addChild(pItem);
			}
			pItemLevel->setExpanded(true);
		}
	}

	
	//自适应长宽
	//ui->tableWidget->resizeColumnsToContents();
	//ui->tableWidget->resizeRowsToContents();
}

bool DefectItemSet::execSingle(QString strExpr, itemValue& value)
{
	if (strExpr.length() == 0)
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("表达式为空"));
		return false;
	}
	bool bRes = false;
	//  根据表达式执行获取结果
	do
	{
		if (m_pScriptLevel == NULL)
		{
			m_pScriptLevel = new ScriptLexer(); 
			m_pScriptLevel->setChannel(m_strChannel);

		}

		string strExpression = strExpr.toStdString();
		strExpression.append("\n");
		if (strExpression != m_pScriptLevel->getParserString())
		{
			m_pScriptLevel->setParserString(strExpression);
		}

		if (!m_pScriptLevel->isParser() && 0 != m_pScriptLevel->parser())
		{
			QString strError = QString::fromLocal8Bit("表达式格式错误 %1 ").arg(QString::fromStdString(m_pScriptLevel->getErrString()));
			QMessageBox::warning(this, "Warning", strError);
			return false;
		}

		m_pScriptLevel->addVarValue("Area", value.dArea);
		m_pScriptLevel->addVarValue("Score", value.dScore);
		m_pScriptLevel->addVarValue("Contrast", value.dContrast);
		m_pScriptLevel->addVarValue("Width", value.dWidth);
		m_pScriptLevel->addVarValue("Height", value.dHeight);
		m_pScriptLevel->addVarValue("SkeletonLength", value.dSkeletonLength);
		m_pScriptLevel->addVarValue("PosX", value.dPosX);
		m_pScriptLevel->addVarValue("PosY", value.dPosX);
		m_pScriptLevel->addVarValue("Svm", value.iSvm);

		double dResult = 0.0;
		if (0 != m_pScriptLevel->evaluation(dResult))
		{
			QString strError = QString::fromLocal8Bit("执行错误 %1 ").arg(QString::fromStdString(m_pScriptLevel->getErrString()));
			QMessageBox::warning(this, "Warning", strError);
			return false;
		}
		if (dResult > 0.0000001 || dResult < -0.0000001)
		{
			bRes = true;
		}

	} while (0);

	QString stMsg = bRes ? QString::fromLocal8Bit("执行成功 执行结果为1") : QString::fromLocal8Bit("执行成功 执行结果为0");
	QMessageBox::warning(this, "Warning", stMsg);
	return bRes;
}

void DefectItemSet::UiCancel()
{
	InitUi();
}

vector<defectlevel>* DefectItemSet::getUiVale()
{
	if (m_VecCurrData == NULL)
	{
		return NULL;
	}


	bool bError = false;
	int iTopCnt = m_ptreeWidget->topLevelItemCount();
	for (int i = 0; i < iTopCnt; i++)
	{
		QTreeWidgetItem *pTopItem = m_ptreeWidget->topLevelItem(i);
		bool bEnable = pTopItem->checkState(0) == Qt::Checked ? true : false;
		if (!bEnable)
		{
			continue;
		}

		int iChildCnt = pTopItem->childCount();
		for (int j = 0; j < iChildCnt; j++)
		{
			QTreeWidgetItem *pChildItem = pTopItem->child(j);
			bool bEnable = pChildItem->checkState(0) == Qt::Checked ? true : false;
			if (!bEnable)
			{
				continue;
			}

			string strExpression = pChildItem->text(1).toStdString();
			pChildItem->setTextColor(1, QColor(0, 0, 0, 255));

			ScriptLexer ScriptLexertmp;
			ScriptLexertmp.setChannel(m_strChannel);
			strExpression.append("\n");
			ScriptLexertmp.setParserString(strExpression);

			if (0 != ScriptLexertmp.parser())
			{
				pChildItem->setTextColor(1, QColor(255, 0, 0, 200));
				bError = true;
			}
		}
	}

	if (bError)
	{
		QMessageBox::StandardButton iRes = QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("表达式格式错误,是否继续保存"), QMessageBox::Yes | QMessageBox::No);
		if (QMessageBox::No == iRes)
		{
			return NULL;
		}	
	}


	iTopCnt = m_ptreeWidget->topLevelItemCount();
	for (int i = 0; i < iTopCnt && m_VecCurrData->size();i++)
	{
		QTreeWidgetItem *pTopItem = m_ptreeWidget->topLevelItem(i);
		m_VecCurrData->at(i).bEnable = pTopItem->checkState(0) == Qt::Checked ? true : false;

		int iChildCnt = pTopItem->childCount();
		if (iChildCnt != m_VecCurrData->at(i).defectlevelItemVec.size())
		{
			m_VecCurrData->at(i).defectlevelItemVec.resize(iChildCnt);
		}
		for (int j = 0; j < iChildCnt;j++)
		{
			QTreeWidgetItem *pChileItem = pTopItem->child(j);
			m_VecCurrData->at(i).defectlevelItemVec.at(j).bEnable = pChileItem->checkState(0)== Qt::Checked ?true :false;
			m_VecCurrData->at(i).defectlevelItemVec.at(j).strItemDes = pChileItem->text(0).toStdString();;
			m_VecCurrData->at(i).defectlevelItemVec.at(j).strItemExpression = pChileItem->text(1).toStdString();
		}
	}

	return m_VecCurrData;
}

void DefectItemSet::slotActAdd()
{
	QTreeWidgetItem* pCurItem = m_ptreeWidget->currentItem();
	if (pCurItem == NULL)
	{
		return;
	}
	QTreeWidgetItem *pParentItem = pCurItem->parent();
	if (NULL == pParentItem)
	{
		
	}
	else
	{
		int iIndex = pParentItem->indexOfChild(pCurItem);

		QTreeWidgetItem * pItem = new QTreeWidgetItem();
		pItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		pItem->setCheckState(0, Qt::Checked);
		QString strItemDes =  QString::number(iIndex);
		pItem->setText(TREE_COL_INDEX_DES, strItemDes);
		pItem->setText(TREE_COL_INDEX_EXPRESSION, "");

		pParentItem->insertChild(iIndex, pItem);
	}
}

void DefectItemSet::slotActDelete()
{
	QTreeWidgetItem* pCurItem = m_ptreeWidget->currentItem();
	if (pCurItem == NULL)
	{
		return;
	}
	QTreeWidgetItem *pParentItem = pCurItem->parent();
	if (pParentItem == NULL)
	{
		return;
	}
	else
	{
		int iIndex = pParentItem->indexOfChild(pCurItem);
		pParentItem->removeChild(pCurItem);
	}
}

void DefectItemSet::slotActCopy()
{
	QTreeWidgetItem* pCurItem = m_ptreeWidget->currentItem();
	if (pCurItem == NULL)
	{
		return;
	}
	QTreeWidgetItem *pParentItem = pCurItem->parent();
	if (pParentItem == NULL)
	{
		return;
	}
}

void DefectItemSet::slotActPaste()
{
	QTreeWidgetItem* pCurItem = m_ptreeWidget->currentItem();
	if (pCurItem == NULL)
	{
		return;
	}
	QTreeWidgetItem *pParentItem = pCurItem->parent();
	if (pParentItem == NULL)
	{
		return;
	}

}

void DefectItemSet::slotActExec()
{
	QTreeWidgetItem* pCurItem = m_ptreeWidget->currentItem();
	if (pCurItem == NULL)
	{
		return;
	}
	itemValue itemValuetmp;
    defectvalue m_ptypeitem(&itemValuetmp);
	int iRet = m_ptypeitem.exec();
	if (iRet != QDialog::Accepted)
	{
		return;
	}

	QTreeWidgetItem *pParentItem = pCurItem->parent();
	if (pParentItem == NULL)
	{
		QString strExp;
		int iChildCnt = pCurItem->childCount();
		bool bError = false;
		for (int i = 0; i < iChildCnt;i++)
		{
			QTreeWidgetItem *pChildItem =pCurItem->child(i);
			QString strText;
			if (pChildItem == NULL || pChildItem->checkState(0) == Qt::Unchecked || pChildItem->text(1).isEmpty())
			{
				continue;
			}
			strText = pChildItem->text(1);
			pChildItem->setTextColor(1, QColor(0, 0, 0, 255));

			ScriptLexer ScriptLexertmp;
			ScriptLexertmp.setChannel(m_strChannel);
			string strExpression = strText.toStdString();
			strExpression.append("\n");
			ScriptLexertmp.setParserString(strExpression);

			if ( 0 != ScriptLexertmp.parser())
			{
				pChildItem->setTextColor(1, QColor(255, 0, 0, 200));
				bError = true;
			}

			if (strExp.isEmpty())
			{   
				QString Tmp = "(" + strText + ")";
				strExp = Tmp;
			}
			else
			{
				QString Tmp =  "| (" + strText + ")";
				strExp += Tmp;
			}
		}
		if (bError)
		{
			QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("表达式格式错误"));
			return;
		}
		bool bRes = execSingle(strExp, itemValuetmp);
		return;
	}
	else
	{
		QString strText = pCurItem->text(1);
		bool bRes = execSingle(strText, itemValuetmp);
	}
}

void DefectItemSet::slotActFullExp()
{
	QTreeWidgetItem* pCurItem = m_ptreeWidget->currentItem();
	if (pCurItem == NULL)
	{
		return;
	}

	QTreeWidgetItem *pParentItem = pCurItem->parent();
	if (pParentItem == NULL)
	{

		QString strExp;
		int iChildCnt = pCurItem->childCount();
		bool bError = false;
		for (int i = 0; i < iChildCnt; i++)
		{
			QTreeWidgetItem *pChildItem = pCurItem->child(i);
			QString strText;
			if (pChildItem == NULL || pChildItem->checkState(0) == Qt::Unchecked || pChildItem->text(1).isEmpty())
			{
				continue;
			}
			strText = pChildItem->text(1);
			pChildItem->setTextColor(1, QColor(0, 0, 0, 255));

			ScriptLexer ScriptLexertmp;
			ScriptLexertmp.setChannel(m_strChannel);
			string fullScript;
			string strExpression = strText.toStdString();
			strExpression.append("\n");
			bool bRet = ScriptLexertmp.getFullParserString(strExpression, fullScript);
			if (!bRet)
			{
				QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("解析失败"));
				return;
			}

			if (strExp.isEmpty())
			{
				QString Tmp = "(" + QString::fromStdString(fullScript) + ")";
				strExp = Tmp;
			}
			else
			{
				QString Tmp = "| (" + QString::fromStdString(fullScript) + ")";
				strExp += Tmp;
			}
		}
		QMessageBox::information(this, "info", QString::fromLocal8Bit("完整语句如下 : %1").arg(strExp));
		return;
	}
	else
	{
		QString strText = pCurItem->text(1);

		ScriptLexer ScriptLexertmp;
		ScriptLexertmp.setChannel(m_strChannel);
		string fullScript;
		string strExpression = strText.toStdString();
		strExpression.append("\n");
		bool bRet = ScriptLexertmp.getFullParserString(strExpression, fullScript);
		if (!bRet)
		{
			QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("解析失败"));
			return;
		}
		else
		{
			QMessageBox::information(this, "info", QString::fromLocal8Bit("完整语句如下 : %1").arg(QString::fromStdString(fullScript)));
			return;
		}
	}
}

void DefectItemSet::slotActLevelExec()
{

	bool bError = false;
	int iTopCnt = m_ptreeWidget->topLevelItemCount();
	for (int i = 0; i < iTopCnt ; i++)
	{
		QTreeWidgetItem *pTopItem = m_ptreeWidget->topLevelItem(i);
		bool bEnable = pTopItem->checkState(0) == Qt::Checked ? true : false;
		if (!bEnable)
		{
			continue;
		}

		int iChildCnt = pTopItem->childCount();
		for (int j = 0; j < iChildCnt; j++)
		{
			QTreeWidgetItem *pChildItem = pTopItem->child(j);
			bool bEnable = pChildItem->checkState(0) == Qt::Checked ? true : false;
			if (!bEnable)
			{
				continue;
			}
	
			string strExpression = pChildItem->text(1).toStdString();
			pChildItem->setTextColor(1, QColor(0, 0, 0, 255));

			ScriptLexer ScriptLexertmp;
			ScriptLexertmp.setChannel(m_strChannel);
			strExpression.append("\n");
			ScriptLexertmp.setParserString(strExpression);

			if (0 != ScriptLexertmp.parser())
			{
				pChildItem->setTextColor(1, QColor(255, 0, 0, 200));
				bError = true;
			}
		}
	}

	if (bError)
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("表达式格式错误，已经标注为红色"));
		return;
	}

	itemValue itemValuetmp;
    defectvalue m_ptypeitem(&itemValuetmp);
	int iRet = m_ptypeitem.exec();
	if (iRet != QDialog::Accepted)
	{
		return;
	}


	if (m_pIDefectQualification != NULL)
	{
		endefectType defectType;
        for (int i = 0; i < m_defectNameVec.size(); i++)
		{
			if (m_strDefectName.compare(m_defectNameVec[i]) == 0)
			{
				defectType = (endefectType)i;
				break;
			}
		}

		stDefectFeature DefectFeaturetmp;

		DefectFeaturetmp.iposition = m_iRegion;
		DefectFeaturetmp.dSizeArea = itemValuetmp.dArea;
		DefectFeaturetmp.fdetect_score = itemValuetmp.dScore;
		DefectFeaturetmp.contrast = itemValuetmp.dContrast;
		DefectFeaturetmp.dWide = itemValuetmp.dWidth;
		DefectFeaturetmp.dHigh = itemValuetmp.dHeight;
		DefectFeaturetmp.SkeletonLength = itemValuetmp.dSkeletonLength;
		DefectFeaturetmp.posx = itemValuetmp.dPosX;
		DefectFeaturetmp.posy = itemValuetmp.dPosY;
		DefectFeaturetmp.idetect_flag = itemValuetmp.iSvm;

		QTime startTimeProcess = QTime::currentTime();
		bool bRes =true;
		bRes = m_pIDefectQualification->getDefectSeverityLevel(defectType, &DefectFeaturetmp);
		QTime endTimeProcess = QTime::currentTime();
		int iTime = startTimeProcess.msecsTo(endTimeProcess);
		QString stMsg = "";
		if (bRes)
		{
			stMsg = QString::fromLocal8Bit("执行成功 ,判断等级为%1,耗时%2 ms").arg(QString::number(DefectFeaturetmp.iDefectLevel)).arg(QString::number(iTime));
		}
		else
		{
			stMsg = QString::fromLocal8Bit("执行失败");
		}

		QMessageBox::warning(this, "Warning", stMsg);
	}
}

void DefectItemSet::popMenu(const QPoint& pot)
{
	QTreeWidgetItem *pItem = m_ptreeWidget->currentItem();
	if (NULL == pItem)
	{
		m_pActAdd->setEnabled(false);
		m_pActDelete->setEnabled(false);
		m_pActCopy->setEnabled(false);
		m_pActPaste->setEnabled(false);
		m_pActExec->setEnabled(false);
		m_pActFullExp->setEnabled(false);
	}
	else
	{
		QTreeWidgetItem *pParentItem = pItem->parent();
		m_pActAdd->setEnabled(NULL != pParentItem);
		m_pActDelete->setEnabled(NULL != pParentItem);
		m_pActCopy->setEnabled(NULL != pParentItem);
		m_pActLevelExec->setEnabled(NULL == pParentItem);
		m_pActFullExp->setEnabled(true);
		m_pActExec->setEnabled(true);
		m_pActPaste->setEnabled(false);
	}

	QPoint globalPos = m_ptreeWidget->viewport()->mapToGlobal(pot);
	m_pContextMenu->exec(globalPos);
}



void DefectItemSet::slotitemDoubleClicked(QTreeWidgetItem* item, int column)
{
	m_pCurrItem = item;
}

void DefectItemSet::slotitemSelectionChanged()
{
	if (m_pCurrItem == NULL)
	{
		return;
	}
	QTreeWidgetItem *pParentItem = m_pCurrItem->parent();
	if (pParentItem == NULL)
	{
		return;
	}

	bool bError = false;
	QString strText = m_pCurrItem->text(1);

	ScriptLexer ScriptLexertmp;
	ScriptLexertmp.setChannel(m_strChannel);
	string strExpression = strText.toStdString();
	strExpression.append("\n");
	ScriptLexertmp.setParserString(strExpression);
	m_pCurrItem->setTextColor(1, QColor(0, 0, 0, 255));

	if (0 != ScriptLexertmp.parser())
	{
		m_pCurrItem->setTextColor(1, QColor(255, 0, 0, 200));
		bError = true;
	}

	if (bError)
	{
		QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("表达式格式错误"));
	}
	m_pCurrItem = NULL;
}

