#include "usersetting.h"
#include "ui_usersetting.h"
#include "QMessageBox"
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QStackedWidget>
#include "configoper.h"

UserSetting::UserSetting(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::UserSetting),
	m_bInit(false),
	m_pButtonOk(NULL),
	m_pButtonCancle(NULL),
	m_bModify(false),
	m_pStackedWidget(NULL),
	m_bConnetIndexChanged(false),
	m_bMulStackedWidget(false),
	m_pCombox(NULL)
{
    ui->setupUi(this);
}

UserSetting::~UserSetting()
{
    delete ui;
}

void UserSetting::on_pushBtnOk_clicked()
{
	SaveData();
	Modify(m_bModify);
}

void UserSetting::on_pushBtnCancel_clicked()
{
	Modify(m_bModify);
	ShowData();
}

void UserSetting::on_comboBox_currentIndexChanged(int index)
{
	if (m_pStackedWidget){
		if (index < m_pStackedWidget->count()){
			m_pStackedWidget->setCurrentIndex(index);
		}
	}
}

void UserSetting::Init()
{
	if (m_bInit)
		return;
	m_bInit = true;

	InitControl(this);
	QString strGroupBoxName = this->objectName();
	if (m_pButtonOk == NULL)
	{
		QString strMsg = QString::fromLocal8Bit("用户设置界面GroupBox %1 没有检测到\"确定\"按钮.").arg(strGroupBoxName);
		QMessageBox::warning(this, "Warning", strMsg);
	}
	if (m_pButtonCancle == NULL)
	{
		QString strMsg = QString::fromLocal8Bit("用户设置界面GroupBox %1 没有检测到\"取消\"按钮.").arg(strGroupBoxName);
		QMessageBox::warning(this, "Warning", strMsg);
	}
	Modify(true);
	ShowData();
}

void UserSetting::InitControl( QObject *obj )
{
	QObjectList list = obj->children();
	foreach( QObject *obj , list)
	{
		QPushButton* pButtonOk = qobject_cast<QPushButton*>(obj);
		if(pButtonOk && (0 == pButtonOk->text().compare(QString::fromLocal8Bit("确定")))){
			connect(pButtonOk, SIGNAL(clicked()), this, SLOT(on_pushBtnOk_clicked()));
			pButtonOk->hide();
			m_pButtonOk = pButtonOk;
		}

		QPushButton* pButtonCancle = qobject_cast<QPushButton*>(obj);
		if(pButtonCancle && (0 == pButtonCancle->text().compare(QString::fromLocal8Bit("取消")))){
			connect(pButtonCancle, SIGNAL(clicked()), this, SLOT(on_pushBtnCancel_clicked()));
			pButtonCancle->setText(QString::fromLocal8Bit("修改" ));
			pButtonCancle->setDisabled(false);
			m_pButtonCancle = pButtonCancle;
		}

		ControlSetting controlsetting;
		QString strClassName = obj->metaObject()->className();
		if (strClassName == "QWidget" ||
			strClassName == "QFrame" ||
			strClassName == "QGroupBox"){
			InitControl(obj);
			continue;
		}
		else if (strClassName == "QStackedWidget"){
			QStackedWidget *pStackedWidget = qobject_cast<QStackedWidget*>(obj);
			m_pStackedWidget = pStackedWidget;
			m_bMulStackedWidget = true;
			int iCnt = pStackedWidget->count();
			for (int i=0; i<iCnt; i++)
			{
				InitControl(pStackedWidget->widget(i));
			}
			continue;
		} 
		else if (strClassName == "QLineEdit"){
			controlsetting.eType = SETTING_LINE_EDIT;
		} 
		else if (strClassName == "QComboBox"){
			controlsetting.eType = SETTING_COMBO_BOX;
			if (!m_bConnetIndexChanged)
			{
				m_bConnetIndexChanged = true;
				QComboBox *pComboBox = qobject_cast<QComboBox*>(obj);
				m_pCombox = pComboBox;
				connect(pComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBox_currentIndexChanged(int)));
			}
		}
		else if (strClassName == "QCheckBox"){
			controlsetting.eType = SETTING_CHECK_BOX;
		}
		else if (strClassName == "QSpinBox"){
			controlsetting.eType = SETTING_SPIN_BOX;
		}
		else if (strClassName == "QDoubleSpinBox"){
			controlsetting.eType = SETTING_DSPIN_BOX;
		}
		else if (strClassName == "QRadioButton"){
			controlsetting.eType = SETTING_RADIO_BTN;
		}
		else{
			continue;
		}

		QWidget *pWidget = qobject_cast<QWidget*>(obj);
		pWidget->setDisabled(true);
		controlsetting.strOjectName = pWidget->objectName();
		controlsetting.strClassName = strClassName;
		controlsetting.pOject = obj;
		m_mapControl.insert(controlsetting.strOjectName, controlsetting);
	}

	if (m_bMulStackedWidget && m_pStackedWidget != NULL)
	{
		m_pStackedWidget->setCurrentIndex(0);
	}
}

void UserSetting::ShowData()
{
	SettingInfo* pSetting = CONFIG_INSTANCE->GetSetttingInfo(this->objectName());
	if (pSetting){
		for (int iRow=0; iRow<pSetting->listSetting.size(); iRow++)
		{
			QMap<QString, ControlSetting>::iterator mapControl;
			mapControl = m_mapControl.find(pSetting->listSetting[iRow].strKey);
			if (mapControl != m_mapControl.end()){
				if (mapControl->eType == SETTING_LINE_EDIT){
					QLineEdit *pLineEdit = qobject_cast<QLineEdit*>(mapControl->pOject);
					pLineEdit->setText(pSetting->listSetting[iRow].strValue);
				}
				else if (mapControl->eType == SETTING_COMBO_BOX){
					QComboBox *pComboBox = qobject_cast<QComboBox*>(mapControl->pOject);
					pComboBox->setCurrentIndex( pSetting->listSetting[iRow].strValue.toInt());
				}
				else if (mapControl->eType == SETTING_CHECK_BOX){
					QCheckBox *pCheckBox = qobject_cast<QCheckBox*>(mapControl->pOject);
					if (0 == pSetting->listSetting[iRow].strValue.compare("1")){
						pCheckBox->setCheckState(Qt::Checked);
					}
					else{
						pCheckBox->setCheckState(Qt::Unchecked);
					}
				}
				else if (mapControl->eType == SETTING_RADIO_BTN){
					QRadioButton *pRadio = qobject_cast<QRadioButton*>(mapControl->pOject);
					if (0 == pSetting->listSetting[iRow].strValue.compare("1")){
						pRadio->setChecked(true);
					}
					else{
						pRadio->setChecked(false);
					}
				}
				else if (mapControl->eType == SETTING_SPIN_BOX){
					QSpinBox *pSpinBox = qobject_cast<QSpinBox*>(mapControl->pOject);
					pSpinBox->setValue(pSetting->listSetting[iRow].strValue.toInt());
				}
				else{
					QDoubleSpinBox *pDSpinBox = qobject_cast<QDoubleSpinBox*>(mapControl->pOject);
					pDSpinBox->setValue(pSetting->listSetting[iRow].strValue.toDouble());
				}
			}
		}
		m_stSetting = *pSetting;
	}
}

void UserSetting::SaveData()
{
	m_stSetting.strBeginGroup = this->objectName();
	QMap<QString, ControlSetting>::iterator mapControl;
	for (mapControl = m_mapControl.begin(); mapControl != m_mapControl.end(); mapControl++)
	{
		bool bFind = false;
		QString strValue = "";
		if (mapControl->eType == SETTING_LINE_EDIT){
			QLineEdit *pLineEdit = qobject_cast<QLineEdit*>(mapControl->pOject);
			strValue = pLineEdit->text();
		}
		else if (mapControl->eType == SETTING_COMBO_BOX){
			QComboBox *pComboBox = qobject_cast<QComboBox*>(mapControl->pOject);
			strValue = QString::number(pComboBox->currentIndex());
		}
		else if (mapControl->eType == SETTING_CHECK_BOX){
			QCheckBox *pCheckBox = qobject_cast<QCheckBox*>(mapControl->pOject);
			if (pCheckBox->checkState() == Qt::Checked){
				strValue = "1";
			} 
			else{
				strValue = "0";
			}
		}
		else if (mapControl->eType == SETTING_RADIO_BTN){
			QRadioButton *pRadio = qobject_cast<QRadioButton*>(mapControl->pOject);
			if (pRadio->isChecked()){
				strValue = "1";
			} 
			else{
				strValue = "0";
			}
		}
		else if (mapControl->eType == SETTING_SPIN_BOX){
			QSpinBox *pSpinBox = qobject_cast<QSpinBox*>(mapControl->pOject);
			strValue = pSpinBox->cleanText();
		}
		else{
			QDoubleSpinBox *pDSpinBox = qobject_cast<QDoubleSpinBox*>(mapControl->pOject);
			strValue = pDSpinBox->cleanText();
		}

		for (int i=0; i<m_stSetting.listSetting.size(); i++)
		{
			if (0 == m_stSetting.listSetting[i].strKey.compare(mapControl->strOjectName)){
				bFind = true;
				m_stSetting.listSetting[i].strValue = strValue;
			}
		}

		if (!bFind){
			SettingValue setting;
			setting.strKey = mapControl->strOjectName;
			setting.strCtrlType = mapControl->strClassName;
			setting.strValue = strValue;
			m_stSetting.listSetting.append(setting);
		}
	}
	CONFIG_INSTANCE->SetSetttingInfo(&m_stSetting);
}

void UserSetting::Modify( bool bmodify )
{
    if (NULL == m_pButtonOk || NULL == m_pButtonCancle)
    {
        return;
    }

	if (!bmodify){
		m_bModify = true;
		m_pButtonOk->show();
		m_pButtonCancle->setText(QString::fromLocal8Bit("取消" ));
	}
	else{
		m_bModify = false;
		m_pButtonOk->hide();
		m_pButtonCancle->setText(QString::fromLocal8Bit("修改" ));
	}

	QMap<QString, ControlSetting>::iterator mapControl;
	for (mapControl = m_mapControl.begin(); mapControl != m_mapControl.end(); mapControl++)
	{
		SettingValue setting;
		setting.strKey = mapControl->strOjectName;
		QWidget* pWidget = qobject_cast<QWidget*>(mapControl->pOject);
		if (pWidget)
		{
			pWidget->setDisabled(bmodify);
		}
	}

	if (m_bMulStackedWidget && m_pCombox!= NULL)
	{
		m_pCombox->setDisabled(false);
	}
}

void UserSetting::showEvent( QShowEvent * )
{
    Init();
}

