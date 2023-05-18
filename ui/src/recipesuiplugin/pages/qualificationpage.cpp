#include "qualificationpage.h"
#include "uicommon/extendstackwidget.h"
#include "uicommon/commonutils.h"
#include "defectcustomdelegate.h"
#include "callsdk.h"
#include "g12globalenv.h"
#include "expressionparser/Features.h"
#include "rtdefine.h"
#include "g12common.h"
#include "defect/idefectqualification.h"
#include <QPushButton>
#include "maindefine.h"
#include "stringcommon.h"
#include "pages/defectlevelpage/defectlevelpage.h"
#include "defect/idefectqualimanage.h"
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include "maindefine.h"


#define DEFECT_MAX_ROW_COUNT    50

#define DEFECT_KEY_ENABLE_INDEX    0
#define DEFECT_KEY_NAME_INDEX      1
#define DEFECT_KEY_LEVEL7_INDEX    2
#define DEFECT_KEY_LEVEL6_INDEX    3
#define DEFECT_KEY_LEVEL5_INDEX    4
#define DEFECT_KEY_LEVEL4_INDEX    5
#define DEFECT_KEY_LEVEL3_INDEX    6
#define DEFECT_KEY_LEVEL2_INDEX    7
#define DEFECT_KEY_LEVEL1_INDEX    8
#define DEFECT_KEY_COLUMN_COUNT    9

QualificationPage::QualificationPage(QWidget *parent):AbstractPageWidget(parent)
, m_pExtendQualification(NULL)
{
    QHBoxLayout *pLayout = new QHBoxLayout(this);
	m_pExtendQualification = new ExtendStackWidget(this);
	m_pExtendQualification->setTitleBtnWidth(60);
    pLayout->addWidget(m_pExtendQualification);
    setLayout(pLayout);

    m_pExtend1stCheck  = new ExtendStackWidget(this);
	m_pDefectLevel = new ExtendStackWidget(this);
    //m_pExtend2ndCheck  = new ExtendStackWidget(this);
	m_pExtend2ndCheck = NULL;
	
	string channels = getCurrentProgramRecipesList();
	vector<string> channellist = SString::split(channels,",");

	int iDlArithmetic = 1;// 0 为单线程等级， 1为多线程等级深视算法 //默认多线程版本
	IFileVariable *pFile = getDVCfgFileVariable();
	if (pFile != NULL)
	{
		iDlArithmetic = pFile->getValue(DV_ARITHMETIC, 1).toInt();
	}
	
	if (0 == iDlArithmetic) //多合一使用一个等级公式计算
	{
		
		QString key = "";
		QWidget* pIllumChannelWdgt = new defectlevelpage(0,key);
		m_pDefectLevel->addWidget(QString::fromLocal8Bit("公式"), pIllumChannelWdgt);
	}
	else {
		//
		int maxCnt = 0;
		IFileVariable* pFile = getStationCfgVariable();
		if (pFile != NULL)
		{
			maxCnt = pFile->getValue("ImageStation", "cnt").toInt();
		}
		vector<string> ChannelVec;
		for (size_t j = 0; j < maxCnt; j++)
		{
			
			string strSttion = std::to_string(j);

			getEnabelChannle(ChannelVec, strSttion);
			for (int index = 0; index < ChannelVec.size(); index++)
			{
				QString key = QString::fromStdString(channellist.at(index));
				int ilen = 1;
				if (j>0)
				{
					key += QString::number(j);
					ilen = 2;
				}
				QWidget* pIllumChannelWdgt = new defectlevelpage(j,key);
				m_pDefectLevel->addWidget(key.right(ilen), pIllumChannelWdgt);
			}

		}
	}
	

    QWidget* pOKgPage = new DefectQualificationPage("OK");
    QWidget* pRechekPage = new DefectQualificationPage("Rechek");
    QWidget* pReCleanPage = new DefectQualificationPage("ReClean");
    QWidget* pNG = new DefectQualificationPage("NG");
    //QWidget* pTest = new DefectQualificationPage(strTest);

    m_pExtend1stCheck->addWidget("A", pOKgPage);
    m_pExtend1stCheck->addWidget("B", pRechekPage);
    m_pExtend1stCheck->addWidget("C", pReCleanPage);
    m_pExtend1stCheck->addWidget("D", pNG);

    QWidget* pOKgPage2 = new DefectQualificationPage("OK2");
    QWidget* pRechekPage2 = new DefectQualificationPage("Rechek2");
    QWidget* pReCleanPage2 = new DefectQualificationPage("ReClean2");
    QWidget* pNG2 = new DefectQualificationPage("NG2");

	QWidget* pGeneralPage = new GeneralPage();
	m_pExtendQualification->addWidget("General", pGeneralPage);
    m_pExtendQualification->addWidget("1stCheck", m_pExtend1stCheck);
	m_pExtendQualification->addWidget(QString::fromLocal8Bit("等级设定"), m_pDefectLevel);
   // m_pExtendQualification->addWidget("2ndCheck", m_pExtend2ndCheck);

	m_pExtendQualification->setCurrentWidget(0);
    m_pExtend1stCheck->setCurrentWidget(0);
    //m_pExtend2ndCheck->setCurrentWidget(0);

	QObject::connect(m_pExtendQualification, SIGNAL(currentChanged(int)), this, SLOT(on_currentChanged(int)));
    QObject::connect(m_pExtend1stCheck, SIGNAL(currentChanged(int)), this, SLOT(on_currentChanged(int)));

	QObject::connect(pGeneralPage, SIGNAL(sig_enableChanged()), pOKgPage, SLOT(slot_enableChanged()));
	QObject::connect(pGeneralPage, SIGNAL(sig_enableChanged()), m_pDefectLevel, SLOT(slot_enableChanged()));
}

void QualificationPage::onEnable(bool enable)
{
    int index = m_pExtendQualification->currentIndex();
    AbstractPageWidget* pWidget  = NULL;
    if(index == 0 && m_pExtendQualification != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtendQualification->currentWidget());
    }
    else if(index == 1 && m_pExtend1stCheck != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtend1stCheck->currentWidget());
    }
    else if(index == 2 && m_pExtend2ndCheck != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtend2ndCheck->currentWidget());
    }
	else if (index == 2 && m_pDefectLevel != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pDefectLevel->currentWidget());
	}
    if (pWidget == NULL)
	{
		return;
	}
	pWidget->onEnable(enable);
	pWidget->setEnabled(enable);
}

void QualificationPage::onSave()
{
    int index = m_pExtendQualification->currentIndex();
    AbstractPageWidget* pWidget  = NULL;
    if(index == 0 && m_pExtendQualification != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtendQualification->currentWidget());
    }
    else if(index == 1 && m_pExtend1stCheck != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtend1stCheck->currentWidget());
    }
    else if(index == 2 && m_pExtend2ndCheck != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtend2ndCheck->currentWidget());
    }
	else if (index == 2 && m_pDefectLevel != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pDefectLevel->currentWidget());
	}
	if (pWidget == NULL)
	{
		return;
	}

	pWidget->updateProductName(m_strProductName);
	pWidget->onSave();
}

void QualificationPage::onCancel()
{
    int index = m_pExtendQualification->currentIndex();
    AbstractPageWidget* pWidget  = NULL;
    if(index == 0 && m_pExtendQualification != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtendQualification->currentWidget());
    }
    else if(index == 1 && m_pExtend1stCheck != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtend1stCheck->currentWidget());
    }
    else if(index == 2 && m_pExtend2ndCheck != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtend2ndCheck->currentWidget());
    }
	else if (index == 2 && m_pDefectLevel != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pDefectLevel->currentWidget());
	}
	if (pWidget == NULL)
	{
		return;
	}
	pWidget->onCancel();
}

void QualificationPage::onUpdate()
{
    int index = m_pExtendQualification->currentIndex();
    AbstractPageWidget* pWidget  = NULL;
    if(index == 0 && m_pExtendQualification != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtendQualification->currentWidget());
    }
    else if(index == 1 && m_pExtend1stCheck != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtend1stCheck->currentWidget());
    }
    else if(index == 2 && m_pExtend2ndCheck != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtend2ndCheck->currentWidget());
    }
	else if (index == 2 && m_pDefectLevel != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pDefectLevel->currentWidget());
	}
	pWidget->updateProductName(m_strProductName);
	pWidget->onUpdate();
}

void QualificationPage::on_currentChanged(int index)
{
    index = m_pExtendQualification->currentIndex();
    AbstractPageWidget* pWidget  = NULL;
    if(index == 0 && m_pExtendQualification != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtendQualification->currentWidget());
    }
    else if(index == 1 && m_pExtend1stCheck!= NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtend1stCheck->currentWidget());
    }
    else if(index == 2 && m_pExtend2ndCheck != NULL)
    {
        pWidget = qobject_cast<AbstractPageWidget*>(m_pExtend2ndCheck->currentWidget());
    }
	else if (index == 2 && m_pDefectLevel != NULL)
	{
		pWidget = qobject_cast<AbstractPageWidget*>(m_pDefectLevel->currentWidget());
	}
	if (pWidget != NULL)
	{
		pWidget->updateProductName(m_strProductName);
		pWidget->onUpdate();
	}

}

DefectQualificationPage::DefectQualificationPage(QString strKey, QWidget *parent) :
    m_strTypeName(strKey), 
    m_tableView(NULL),
    m_defect_model(NULL)
{
    setMinimumSize(800, 600);
    init();
}

DefectQualificationPage::~DefectQualificationPage()
{

}

void DefectQualificationPage::onEnable(bool enable)
{

}

void DefectQualificationPage::onSave()
{
    saveUIValue();
}

void DefectQualificationPage::onCancel()
{
    updateUIValue();
}

void DefectQualificationPage::onUpdate()
{
    updateUIValue();
}

void DefectQualificationPage::init()
{
    QGridLayout *gridLayout;
	QPushButton * pBtn = new QPushButton("Test");
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    m_tableView = new QTableView;
    m_defect_model = new QStandardItemModel();
    gridLayout->addWidget(m_tableView, 0, 0, 1, 1);
	gridLayout->addWidget(pBtn);
	pBtn->setHidden(m_strTypeName.compare("test", Qt::CaseInsensitive) != 0);
    setLayout(gridLayout);
    m_defect_model->setHorizontalHeaderItem(DEFECT_KEY_ENABLE_INDEX, new QStandardItem(QString::fromLocal8Bit("使能")));
    m_defect_model->setHorizontalHeaderItem(DEFECT_KEY_NAME_INDEX, new QStandardItem(QString::fromLocal8Bit("缺陷类别")));
	m_defect_model->setHorizontalHeaderItem(DEFECT_KEY_LEVEL7_INDEX, new QStandardItem(QString::fromLocal8Bit("7")));
	m_defect_model->setHorizontalHeaderItem(DEFECT_KEY_LEVEL6_INDEX, new QStandardItem(QString::fromLocal8Bit("6")));
    m_defect_model->setHorizontalHeaderItem(DEFECT_KEY_LEVEL5_INDEX, new QStandardItem(QString::fromLocal8Bit("5")));
    m_defect_model->setHorizontalHeaderItem(DEFECT_KEY_LEVEL4_INDEX, new QStandardItem(QString::fromLocal8Bit("4")));
    m_defect_model->setHorizontalHeaderItem(DEFECT_KEY_LEVEL3_INDEX, new QStandardItem(QString::fromLocal8Bit("3")));
    m_defect_model->setHorizontalHeaderItem(DEFECT_KEY_LEVEL2_INDEX, new QStandardItem(QString::fromLocal8Bit("2")));
    m_defect_model->setHorizontalHeaderItem(DEFECT_KEY_LEVEL1_INDEX, new QStandardItem(QString::fromLocal8Bit("1")));

    //设置表头大小
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //利用setModel()方法将数据模型与QTableView绑定
    m_tableView->setModel(m_defect_model);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    DefectCustomDelegate *pDelegateS = new DefectCustomDelegate;
    pDelegateS->setCurrentWidget(DefectCustomDelegate::TYPE_SPIN);
	m_tableView->setItemDelegateForColumn(DEFECT_KEY_LEVEL7_INDEX, pDelegateS);
	m_tableView->setItemDelegateForColumn(DEFECT_KEY_LEVEL6_INDEX, pDelegateS);
    m_tableView->setItemDelegateForColumn(DEFECT_KEY_LEVEL5_INDEX, pDelegateS);
    m_tableView->setItemDelegateForColumn(DEFECT_KEY_LEVEL4_INDEX, pDelegateS);
    m_tableView->setItemDelegateForColumn(DEFECT_KEY_LEVEL3_INDEX, pDelegateS);
    m_tableView->setItemDelegateForColumn(DEFECT_KEY_LEVEL2_INDEX, pDelegateS);
    m_tableView->setItemDelegateForColumn(DEFECT_KEY_LEVEL1_INDEX, pDelegateS);

    m_tableView->setColumnWidth(DEFECT_KEY_ENABLE_INDEX, 50);
    m_tableView->setColumnWidth(DEFECT_KEY_NAME_INDEX, 150);
	m_tableView->setColumnWidth(DEFECT_KEY_LEVEL7_INDEX, 150);
	m_tableView->setColumnWidth(DEFECT_KEY_LEVEL6_INDEX, 150);
	m_tableView->setColumnWidth(DEFECT_KEY_LEVEL5_INDEX, 150);
	m_tableView->setColumnWidth(DEFECT_KEY_LEVEL4_INDEX, 150);
	m_tableView->setColumnWidth(DEFECT_KEY_LEVEL3_INDEX, 150);
	m_tableView->setColumnWidth(DEFECT_KEY_LEVEL2_INDEX, 150);
	m_tableView->setColumnWidth(DEFECT_KEY_LEVEL1_INDEX, 150);

    int iCount = DEFECT_KEY_COLUMN_COUNT;

	IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
	if (NULL != p)
	{
		
		IDefectQualification* pIDefectQualification = p->getDefectQualificationInstance(0);
		if (NULL != pIDefectQualification)
		{
			const map<string, Defect_Qualification_Param> *pVecParam = pIDefectQualification->getQualificationParam(m_strProductName.toStdString());
			if (NULL != pIDefectQualification)
			{
				iCount = pVecParam->size();
			}
		}
		
	}
	IDefectListManage * p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p2)
	{
		m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
		m_defectEnNameVec = p2->getDefectListInstance(0)->getDefectEnNameList();
		m_defectEnableVec = p2->getDefectListInstance(0)->getDefectEnableList();
		m_indexVec = p2->getDefectListInstance(0)->getDefectIndexList();
		m_defectInfoList = p2->getDefectListInstance(0)->getDefectInfo();
	}
	connect(pBtn, SIGNAL(clicked()), this, SLOT(slot_test()));
    updateUIValue();
}

void DefectQualificationPage::updateUIValue()
{
	IDefectQualification* pQualification;
	IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
	if (NULL != p)
	{
		pQualification = p->getDefectQualificationInstance(0);
	}
    RETURN_IF(NULL == pQualification);

    const map<string, Defect_Qualification_Param> *pVecParam = pQualification->getQualificationParam(m_strProductName.toStdString());
    RETURN_IF(NULL == pQualification);

    map<string, Defect_Qualification_Param>::const_iterator itParam = pVecParam->begin();
    
    QStandardItem *pItem = NULL;

    CFeatures* pCFeatures = CFeaturesMgr::getCFeaturesMgr()->getCFeatures(m_strProductName.toStdString());
    RETURN_IF(NULL == pCFeatures);
    //string CategorysString = pCFeatures->getCategorysString();
    //QStringList DefectList = QString::fromStdString(CategorysString).split(",");

	//  更新使能的才显示等级勾选
	IFileVariable *pFileVariable = getQualificationCfgFileVariable(m_strProductName.toStdString());
	if (pFileVariable == NULL)
	{
		return;
	}
	vector<int> indexVec;
	int j = 0;
    for (int i = 0; i < m_defectEnNameVec.size(); i++)
    {
        string strName = m_defectEnNameVec[i];
		if (!m_defectEnableVec[i].second)//  使能的才显示等级
		{
			continue;
		}
		indexVec.push_back(i);
		///////////////////////////////////////
        itParam != pVecParam->begin();
        bool bEnable = false;
        int iValue1 = 99999;
        int iValue2 = 99999;
        int iValue3 = 99999;
        int iValue4 = 99999;
        int iValue5 = 99999;
		int iValue6 = 99999;
		int iValue7 = 99999;

        itParam = pVecParam->find(strName);
        if (itParam != pVecParam->end())
        {
            map<string, Defect_Result_Param>::const_iterator it = itParam->second.mapResultParam.find(m_strTypeName.toStdString());
            QString strName = itParam->second.strName.c_str();
            if (it != itParam->second.mapResultParam.end())
            {
                bEnable = it->second.bEnableQualification;

				iValue1 = it->second.iLevelCountArr[1];
                iValue2 = it->second.iLevelCountArr[2];
                iValue3 = it->second.iLevelCountArr[3];
                iValue4 = it->second.iLevelCountArr[4];
                iValue5 = it->second.iLevelCountArr[5];
				iValue6 = it->second.iLevelCountArr[6];
				iValue7 = it->second.iLevelCountArr[7];
            }
        }
        pItem = new QStandardItem();		
        if (NULL != pItem)
        {
            pItem->setCheckable(true);
            //pItem->setCheckState(bEnable ? Qt::Checked : Qt::Unchecked);
			pItem->setCheckState(m_defectEnableVec[i].second ? Qt::Checked : Qt::Unchecked);
            m_defect_model->setItem(j, DEFECT_KEY_ENABLE_INDEX, pItem);
        }
		QString des = QString::fromStdString(m_defectNameVec[i]);
		for (int def = 0; def < m_defectNameVec.size(); def++)
		{
			if (strName == m_defectNameVec[def])
			{
				des = QString::fromStdString(m_defectNameVec[def]);
			}
		}
        QStandardItem *pItem = new QStandardItem(des);
        pItem->setData(strName.c_str());
        m_defect_model->setItem(j, DEFECT_KEY_NAME_INDEX, pItem);
		m_defect_model->setItem(j, DEFECT_KEY_LEVEL7_INDEX, new QStandardItem(QString::number(iValue7)));
		m_defect_model->setItem(j, DEFECT_KEY_LEVEL6_INDEX, new QStandardItem(QString::number(iValue6)));
        m_defect_model->setItem(j, DEFECT_KEY_LEVEL5_INDEX, new QStandardItem(QString::number(iValue5)));
        m_defect_model->setItem(j, DEFECT_KEY_LEVEL4_INDEX, new QStandardItem(QString::number(iValue4)));
        m_defect_model->setItem(j, DEFECT_KEY_LEVEL3_INDEX, new QStandardItem(QString::number(iValue3)));
        m_defect_model->setItem(j, DEFECT_KEY_LEVEL2_INDEX, new QStandardItem(QString::number(iValue2)));
        m_defect_model->setItem(j, DEFECT_KEY_LEVEL1_INDEX, new QStandardItem(QString::number(iValue1)));
		j++;
    }
}

void DefectQualificationPage::saveUIValue()
{
	
    //IDefectQualification *pQualification = (IDefectQualification*)GetSDKInstance("defectqualification", IDefectQualification::IDD_DEFECT_QULIFICATION);
	IDefectQualification* pQualification;
	IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
	if (NULL != p)
	{

		pQualification = p->getDefectQualificationInstance(0);
		
	}
    RETURN_IF(NULL == pQualification);

    const map<string, Defect_Qualification_Param> *pMapParam = pQualification->getQualificationParam(m_strProductName.toStdString());
    RETURN_IF(NULL == pQualification);

    map<string, Defect_Qualification_Param> mapUIParam;
    map<string, Defect_Qualification_Param>::iterator itUIParam;
    map<string, Defect_Qualification_Param>::const_iterator itParam;
    map<string, Defect_Result_Param>::iterator itUIResultParam;

    mapUIParam = *pMapParam;
    QStandardItem *pItem = NULL;
    int iCount = m_defect_model->rowCount();
    for (int i = 0; i < iCount; i++)
    {
        pItem = m_defect_model->item(i, DEFECT_KEY_NAME_INDEX);
        CONTINUE_IF(NULL == pItem);
        string strDefectName = pItem->data().toString().toStdString();

        //itParam = pMapParam->find(strDefectName);
        //CONTINUE_IF(itParam == pMapParam->end());

        itUIParam = mapUIParam.find(strDefectName);
        if (itUIParam == mapUIParam.end())
        {
            Defect_Qualification_Param param;
            param.strName = strDefectName;
            mapUIParam.insert(make_pair(param.strName, param));
            itUIParam = mapUIParam.find(strDefectName);
        }

        string strResultType = m_strTypeName.toStdString();
        itUIResultParam = itUIParam->second.mapResultParam.find(strResultType);
        if (itUIResultParam == itUIParam->second.mapResultParam.end())
        {
            itUIParam->second.mapResultParam.insert(make_pair(strResultType, Defect_Result_Param()));
            itUIResultParam = itUIParam->second.mapResultParam.find(strResultType);
        }

        pItem = m_defect_model->item(i, DEFECT_KEY_ENABLE_INDEX);
        if (NULL != pItem)
        {
            itUIResultParam->second.bEnableQualification = (pItem->checkState() == Qt::Checked);
        }
        pItem = m_defect_model->item(i, DEFECT_KEY_LEVEL1_INDEX);
        if (NULL != pItem)
        {
            itUIResultParam->second.iLevelCountArr[1] = pItem->text().toInt();
        }
        pItem = m_defect_model->item(i, DEFECT_KEY_LEVEL2_INDEX);
        if (NULL != pItem)
        {
            itUIResultParam->second.iLevelCountArr[2] = pItem->text().toInt();
        }
        pItem = m_defect_model->item(i, DEFECT_KEY_LEVEL3_INDEX);
        if (NULL != pItem)
        {
            itUIResultParam->second.iLevelCountArr[3] = pItem->text().toInt();
        }
        pItem = m_defect_model->item(i, DEFECT_KEY_LEVEL4_INDEX);
        if (NULL != pItem)
        {
            itUIResultParam->second.iLevelCountArr[4] = pItem->text().toInt();
        }
        pItem = m_defect_model->item(i, DEFECT_KEY_LEVEL5_INDEX);
        if (NULL != pItem)
        {
            itUIResultParam->second.iLevelCountArr[5] = pItem->text().toInt();
        }
		pItem = m_defect_model->item(i, DEFECT_KEY_LEVEL6_INDEX);
		if (NULL != pItem)
		{
			itUIResultParam->second.iLevelCountArr[6] = pItem->text().toInt();
		}
		pItem = m_defect_model->item(i, DEFECT_KEY_LEVEL7_INDEX);
		if (NULL != pItem)
		{
			itUIResultParam->second.iLevelCountArr[7] = pItem->text().toInt();
		}
    }
    pQualification->setQualificationParam(&mapUIParam, m_strProductName.toStdString());
}

void DefectQualificationPage::slot_enableChanged()
{
	//删除
	//新增
}

GeneralPage::GeneralPage( QWidget *parent ):
    AbstractPageWidget(parent),
    ui(new Ui::GeneralForm)
{
    ui->setupUi(this);
	IDefectListManage * p = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p)
	{
		m_defectNameVec = p->getDefectListInstance(0)->getDefectCnNameList();
		m_defectEnNameVec = p->getDefectListInstance(0)->getDefectEnNameList();
		m_defectEnableVec = p->getDefectListInstance(0)->getDefectEnableList();
	}
	updateDefectEnablePage();
}

GeneralPage::~GeneralPage()
{
	delete ui;
}

void GeneralPage::onEnable(bool enable)
{

}

void GeneralPage::onSave()
{
	IFileVariable* pFileVariable = getQualificationCfgFileVariable(m_strProductName.toStdString());
	if (pFileVariable == NULL)
	{
		return;
	}

	bool iOK = getUItemValue("checkBox_ok", 0).toBool();
	bool iOK2 = getUItemValue("checkBox_ok2", 0).toBool();
	bool iOK3 = getUItemValue("checkBox_ok3", 0).toBool();
	bool iNG = getUItemValue("checkBox_ng", 0).toBool();

	bool ilevel1 = getUItemValue("checkBox_level1", 0).toBool();
	bool ilevel2 = getUItemValue("checkBox_level2", 0).toBool();
	bool ilevel3 = getUItemValue("checkBox_level3", 0).toBool();
	bool ilevel4 = getUItemValue("checkBox_level4", 0).toBool();
	bool ilevel5 = getUItemValue("checkBox_level5", 0).toBool();
	bool ilevel6 = getUItemValue("checkBox_level6", 0).toBool();
	bool ilevel7 = getUItemValue("checkBox_level7", 0).toBool();

	bool bEnableNgAbort = getUItemValue("checkBox_ng_abort", 0).toBool();
	bool bEnableCntAbort = getUItemValue("checkBox_enable_cnt_abort", 0).toBool();
	bool bEnableTimeout = getUItemValue("checkBox_enable_timeout_abort", 0).toBool();
	int iCnt =  getUItemValue("spinBox_cnt_abort", 0).toInt();
	int iTimeout =  getUItemValue("spinBox_timeout_abort", 0).toInt();


	for (int i = 0; i < DVTOPLIMITCFG_SIZE; i++)
	{
		QString strI = QString::number(i);
		m_stDvToplimitCfg[i].idvSmirchCnt = getUItemValue("spinBox_SmirchCnt"+ strI, 0).toInt();
		m_stDvToplimitCfg[i].idvHairCnt = getUItemValue("spinBox_HairCnt" + strI, 0).toInt();
		m_stDvToplimitCfg[i].idvDirtpitsCnt = getUItemValue("spinBox_DirtpitsCnt" + strI, 0).toInt();
		m_stDvToplimitCfg[i].ddvSmirchArea = getUItemValue("doubleSpinBox_SmirchArea" + strI, 0).toDouble();
		m_stDvToplimitCfg[i].ddvHairArea = getUItemValue("doubleSpinBox_HairArea" + strI, 0).toDouble();
		m_stDvToplimitCfg[i].ddvDirtArea = getUItemValue("doubleSpinBox_DirtArea" + strI, 0).toDouble();
	}

	pFileVariable->setValue( QUALIFICATION_ENABLE_OK, iOK, true);
	pFileVariable->setValue( QUALIFICATION_ENABLE_OK2, iOK2, true);
	pFileVariable->setValue( QUALIFICATION_ENABLE_OK3, iOK3, true);
	pFileVariable->setValue( QUALIFICATION_ENABLE_NG, iNG, true);

	pFileVariable->setValue( QUALIFICATION_ENABLE_LEVEL1, ilevel1, true);
	pFileVariable->setValue( QUALIFICATION_ENABLE_LEVEL2, ilevel2, true);
	pFileVariable->setValue( QUALIFICATION_ENABLE_LEVEL3, ilevel3, true);
	pFileVariable->setValue( QUALIFICATION_ENABLE_LEVEL4, ilevel4, true);
	pFileVariable->setValue( QUALIFICATION_ENABLE_LEVEL5, ilevel5, true);
	pFileVariable->setValue( QUALIFICATION_ENABLE_LEVEL6, ilevel6, true);
	pFileVariable->setValue( QUALIFICATION_ENABLE_LEVEL7, ilevel7, true);

	pFileVariable->setValue(QUALIFICATION_ENABLE_NG_ABORT, bEnableNgAbort, true);
	pFileVariable->setValue(QUALIFICATION_ENABLE_CNT_ABORT, bEnableCntAbort, true);
	pFileVariable->setValue(QUALIFICATION_ENABLE_TIMEOUT_ABORT, bEnableTimeout, true);
	pFileVariable->setValue(QUALIFICATION_CNT_ABORT, iCnt, true);
	pFileVariable->setValue(QUALIFICATION_TIMEOUT_ABORT, iTimeout, true);



	for (int i = 0; i < DVTOPLIMITCFG_SIZE; i++)
	{
		QString strI = QString::number(i);
		m_stDvToplimitCfg[i].idvSmirchCnt = getUItemValue("spinBox_SmirchCnt" + strI, 0).toInt();
		m_stDvToplimitCfg[i].idvHairCnt = getUItemValue("spinBox_HairCnt" + strI, 0).toInt();
		m_stDvToplimitCfg[i].idvDirtpitsCnt = getUItemValue("spinBox_DirtpitsCnt" + strI, 0).toInt();
		m_stDvToplimitCfg[i].ddvSmirchArea = getUItemValue("doubleSpinBox_SmirchArea" + strI, 0).toDouble();
		m_stDvToplimitCfg[i].ddvHairArea = getUItemValue("doubleSpinBox_HairArea" + strI, 0).toDouble();
		m_stDvToplimitCfg[i].ddvDirtArea = getUItemValue("doubleSpinBox_DirtArea" + strI, 0).toDouble();

		pFileVariable->setValue(QUALIFICATION_TOPLIMITCFG_SMIRCHCNT+ strI.toStdString(), m_stDvToplimitCfg[i].idvSmirchCnt, 999);
		pFileVariable->setValue(QUALIFICATION_TOPLIMITCFG_HAIRCNT + strI.toStdString(), m_stDvToplimitCfg[i].idvHairCnt, 999);
		pFileVariable->setValue(QUALIFICATION_TOPLIMITCFG_DIRTPITSCNT + strI.toStdString(), m_stDvToplimitCfg[i].idvDirtpitsCnt, 999);
		pFileVariable->setValue(QUALIFICATION_TOPLIMITCFG_SMIRCHAREA + strI.toStdString(), m_stDvToplimitCfg[i].ddvSmirchArea, 999.0);
		pFileVariable->setValue(QUALIFICATION_TOPLIMITCFG_HAIRAREA + strI.toStdString(), m_stDvToplimitCfg[i].ddvHairArea, 999.0);
		pFileVariable->setValue(QUALIFICATION_TOPLIMITCFG_DIRTAREA + strI.toStdString(), m_stDvToplimitCfg[i].ddvDirtArea, 999.0);
	}
	for (int i = 0; i < m_defectEnableCheckBoxVec.size(); ++i)
	{
		//*m_defectEnableVecPoint.push_back(make_pair(m_defectEnableCheckBoxVec[i].first,m_defectEnableCheckBoxVec[i].second->isChecked()));
		for (int j = 0; j < m_defectEnableVec.size(); ++j)
		{
			if (m_defectEnableVec[j].first == m_defectEnableCheckBoxVec[i].first)
			{
				m_defectEnableVec[j].second = m_defectEnableCheckBoxVec[i].second->isChecked();
				break;
			}
		}
	}
	IDefectListManage * p = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p)
	{
		p->getDefectListInstance(0)->setDefectEnable(m_defectEnableVec);
	}

	//defectEnableVec.clear();
	onSave_AreaWarningUI();
	emit sig_enableChanged();
}

void GeneralPage::onCancel()
{
	onUpdate();
	onUpdate_AreaWarningUI();
}

void GeneralPage::onUpdate()
{

	IFileVariable* pFileVariable = getQualificationCfgFileVariable(m_strProductName.toStdString());
	if (pFileVariable == NULL)
	{
		return;
	}

	bool iOK = pFileVariable->getValue( QUALIFICATION_ENABLE_OK,  true).toBool();;
	bool iOK2 = pFileVariable->getValue(QUALIFICATION_ENABLE_OK2,  true).toBool();;
	bool iOK3 = pFileVariable->getValue( QUALIFICATION_ENABLE_OK3,  true).toBool();;
	bool iNG = pFileVariable->getValue(QUALIFICATION_ENABLE_NG,  true).toBool();;
	

	bool ilevel1 = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL1, true).toBool();
	bool ilevel2 = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL2, true).toBool();
	bool ilevel3 = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL3, true).toBool();
	bool ilevel4 = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL4, true).toBool();
	bool ilevel5 = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL5, true).toBool();
	bool ilevel6 = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL6, true).toBool();
	bool ilevel7 = pFileVariable->getValue(QUALIFICATION_ENABLE_LEVEL7, true).toBool();


	bool bEnableNgAbort = pFileVariable->getValue(QUALIFICATION_ENABLE_NG_ABORT, true).toBool();
	bool bEnableCntAbort = pFileVariable->getValue(QUALIFICATION_ENABLE_CNT_ABORT, true).toBool();
	bool bEnableTimeout = pFileVariable->getValue(QUALIFICATION_ENABLE_TIMEOUT_ABORT, true).toBool();
	int iCnt = pFileVariable->getValue(QUALIFICATION_CNT_ABORT, 9999).toInt();
	int iTimeout = pFileVariable->getValue(QUALIFICATION_TIMEOUT_ABORT, 9999).toInt();



	for (int i = 0; i < DVTOPLIMITCFG_SIZE; i++)
	{
		QString strI = QString::number(i);
		m_stDvToplimitCfg[i].idvSmirchCnt = pFileVariable->getValue(QUALIFICATION_TOPLIMITCFG_SMIRCHCNT + strI.toStdString(), 999).toInt();
		m_stDvToplimitCfg[i].idvHairCnt = pFileVariable->getValue(QUALIFICATION_TOPLIMITCFG_HAIRCNT + strI.toStdString(), 999).toInt();
		m_stDvToplimitCfg[i].idvDirtpitsCnt = pFileVariable->getValue(QUALIFICATION_TOPLIMITCFG_DIRTPITSCNT + strI.toStdString(), 999).toInt();
		m_stDvToplimitCfg[i].ddvSmirchArea = pFileVariable->getValue(QUALIFICATION_TOPLIMITCFG_SMIRCHAREA + strI.toStdString(), 999.0).toDouble();
		m_stDvToplimitCfg[i].ddvHairArea = pFileVariable->getValue(QUALIFICATION_TOPLIMITCFG_HAIRAREA + strI.toStdString(), 999.0).toDouble();
		m_stDvToplimitCfg[i].ddvDirtArea = pFileVariable->getValue(QUALIFICATION_TOPLIMITCFG_DIRTAREA + strI.toStdString(), 999.0).toDouble();
	}

	setUIItemValue("checkBox_ok", iOK);
	setUIItemValue("checkBox_ok2", iOK2);
	setUIItemValue("checkBox_ok3", iOK3);
	setUIItemValue("checkBox_ng", iNG);


	for (int i = 0; i < m_defectNameVec.size(); ++i)
	{
		QString toolTipStr = QString::fromLocal8Bit("checkBox_") + QString::fromStdString(m_defectEnNameVec[i]);
		setUIItemToolTip(toolTipStr, QString::fromStdString(m_defectNameVec[i]));
	}
	
	setUIItemValue("checkBox_level1", ilevel1);
	setUIItemValue("checkBox_level2", ilevel2);
	setUIItemValue("checkBox_level3", ilevel3);
	setUIItemValue("checkBox_level4", ilevel4);
	setUIItemValue("checkBox_level5", ilevel5);
	setUIItemValue("checkBox_level6", ilevel6);
	setUIItemValue("checkBox_level7", ilevel7);

	setUIItemValue("checkBox_ng_abort", bEnableNgAbort);
	setUIItemValue("checkBox_enable_cnt_abort", bEnableCntAbort);
	setUIItemValue("checkBox_enable_timeout_abort", bEnableTimeout);
	setUIItemValue("spinBox_cnt_abort", iCnt);
	setUIItemValue("spinBox_timeout_abort", iTimeout);


	for (int i = 0; i < DVTOPLIMITCFG_SIZE; i++)
	{
		QString strI = QString::number(i);
		setUIItemValue("spinBox_SmirchCnt"+strI, m_stDvToplimitCfg[i].idvSmirchCnt);
		setUIItemValue("spinBox_HairCnt" + strI, m_stDvToplimitCfg[i].idvHairCnt);
		setUIItemValue("spinBox_DirtpitsCnt" + strI, m_stDvToplimitCfg[i].idvDirtpitsCnt);
		setUIItemValue("doubleSpinBox_SmirchArea" + strI, m_stDvToplimitCfg[i].ddvSmirchArea);
		setUIItemValue("doubleSpinBox_HairArea" + strI, m_stDvToplimitCfg[i].ddvHairArea);
		setUIItemValue("doubleSpinBox_DirtArea"+strI, m_stDvToplimitCfg[i].ddvDirtArea);
	}
	onUpdate_AreaWarningUI();

}

void GeneralPage::onUpdate_AreaWarningUI()
{

	IFileVariable* pFileVariable = getQualificationCfgFileVariable(m_strProductName.toStdString());
	if (pFileVariable == NULL)
	{
		return;
	}

	bool bEnableAll = pFileVariable->getValue(AREA_WARNING_ENABLE_ALL, true).toBool();
	int iWidth = pFileVariable->getValue(AREA_WARNING_WIDTH, true).toInt();
	int iHeight = pFileVariable->getValue(AREA_WARNING_HEIGHT, true).toInt();
	int iCnt = pFileVariable->getValue(AREA_WARNING_CNT, true).toInt();

	setUIItemValue("checkBox_area_warning_enableall", bEnableAll);
	setUIItemValue("lineEdit_area_warning_width", iWidth);
	setUIItemValue("lineEdit_area_warning_height", iHeight);
	setUIItemValue("spinBox_area_warning_cnt", iCnt);

	for (int i = 0; i < AREA_WARNING_ARRAY_CNT; i++)
	{
		bool bdefectenable =  pFileVariable->getValue(AREA_WARNING,AREA_WARNING_DEFECTENABLE+SString::IntToStr(i), true).toBool();
		int  idefectype = pFileVariable->getValue(AREA_WARNING, AREA_WARNING_DEFECTYPE + SString::IntToStr(i),0).toInt();
		int  idefectcnt = pFileVariable->getValue(AREA_WARNING, AREA_WARNING_DEFECTCNT + SString::IntToStr(i),0).toInt();
		setUIItemValue(QString("checkBox_area_warning_enable%1").arg(i), bdefectenable);

		QComboBox* pCombox =qobject_cast<QComboBox*>( getUIItem(QString("comboBox_areawarning_defectype%1").arg(i)));
		if (pCombox != NULL)
		{
			pCombox->setCurrentIndex(idefectype);
		}
		
		//setUIItemValue(QString("comboBox_areawarning_defectype%1").arg(i), idefectype);
		setUIItemValue(QString("spinBox_areawarning_cnt%1").arg(i), idefectcnt);
	}

}

void GeneralPage::onSave_AreaWarningUI()
{
	IFileVariable* pFileVariable = getQualificationCfgFileVariable(m_strProductName.toStdString());
	if (pFileVariable == NULL)
	{
		return;
	}

	bool bEnableNgAbort = getUItemValue("checkBox_area_warning_enableall", 0).toBool();
	int iWidth = getUItemValue("lineEdit_area_warning_width", 0).toInt();
	int iHeight = getUItemValue("lineEdit_area_warning_height", 0).toInt();
	int iCnt = getUItemValue("spinBox_area_warning_cnt", 0).toInt();


	pFileVariable->setValue(AREA_WARNING_ENABLE_ALL, bEnableNgAbort, true);
	pFileVariable->setValue(AREA_WARNING_WIDTH, iWidth, true);
	pFileVariable->setValue(AREA_WARNING_HEIGHT, iHeight, true);
	pFileVariable->setValue(AREA_WARNING_CNT, iCnt, true);



	for (int i = 0; i < AREA_WARNING_ARRAY_CNT; i++)
	{
		bool bdefectenable = getUItemValue(QString("checkBox_area_warning_enable%1").arg(i),false).toBool();
		int  idefectype = 0;
		int  idefectcnt = getUItemValue(QString("spinBox_areawarning_cnt%1").arg(i), 0).toInt();


		QComboBox* pCombox = qobject_cast<QComboBox*>(getUIItem(QString("comboBox_areawarning_defectype%1").arg(i)));
		if (pCombox != NULL)
		{
			idefectype = pCombox->currentIndex();
		}

		pFileVariable->setValue(AREA_WARNING, AREA_WARNING_DEFECTENABLE + SString::IntToStr(i), bdefectenable,true);
		pFileVariable->setValue(AREA_WARNING, AREA_WARNING_DEFECTYPE + SString::IntToStr(i), idefectype, true);
		pFileVariable->setValue(AREA_WARNING, AREA_WARNING_DEFECTCNT + SString::IntToStr(i), idefectcnt, true);
	}
}

void GeneralPage::on_pushButton_area_warning_clearCnt_clicked()
{
	if (QMessageBox::No == QMessageBox::warning(NULL, "warn", QString::fromLocal8Bit("请确认是否重新开启计算？"), QMessageBox::Yes | QMessageBox::No))
		return;


	IFileVariable* pFileVariable = getQualificationCfgFileVariable(m_strProductName.toStdString());
	if (pFileVariable == NULL)
	{
		return;
	}
	pFileVariable->setValue(AREA_WARNING_RECOUNT, true, true);
}


void GeneralPage::updateDefectEnablePage()
{
	m_defectEnableCheckBoxVec.clear();
	for (int i = 0; i < m_defectEnNameVec.size(); ++i)
	{
		QCheckBox* checkBox = new QCheckBox();
		checkBox->setText(QString::fromStdString(m_defectNameVec[i]));
		checkBox->setChecked(m_defectEnableVec[i].second);
		m_defectEnableCheckBoxVec.push_back(make_pair(m_defectEnNameVec[i], checkBox));
		ui->groupBox_DefectEnable->layout()->addWidget(checkBox);
	}
}
