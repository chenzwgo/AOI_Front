#include "illuminationcontroldlg_new.h".h"
#include "uicommon/extendstackwidget.h"
#include <QSettings>
#include "uicommon/commonutils.h"
#include "iniFile.h"
#include "sdkcommon/com/icommanager.h"
#include "sdkcommon/com/icom.h"
#include "callsdk.h"
#include "rtdefine.h"
#include "modbuscom.h"
//#include "configui/configoper.h"
#include "motion/imotion.h"
#include "card/icardcontinterp.h"
#include "g12common.h"
#include "g12globalenv.h"
#include "cimageprocess.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "QString"
#include "modbuscom.h"
#include "maindefine.h"
#include "stringcommon.h"
#include <QFileDialog>
#include <QInputDialog>


IllumChannelWdgt_new::IllumChannelWdgt_new(QString strSence,QString Channel, QWidget *parent):
	AbstractPageWidget(parent),
    m_strChannel(Channel),
    ui(new Ui::illForm_new),
    m_currentSence(strSence.toStdString())
{
    ui->setupUi(this);
    m_plightcontrol_new =  lightcontrol_new_manager::GetInstance()->getLightcontrol(strSence.toInt());
	m_iSection = Scan_line_1;


	IFileVariable* pFileVariable = getAcquisitionFileVariable(m_strProductName.toStdString(), strSence.toStdString());

	m_Channel_Index = ChannelNameToIndex(m_strChannel);
	readAllCfg();
	updateLightCfgUi();
	on_radioButton_section1_clicked();
}

IllumChannelWdgt_new::~IllumChannelWdgt_new()
{
    delete ui;
}

bool IllumChannelWdgt_new::ChangeProdutUpdate()
{
	ReadCfgToUi();
	return true;
}

void IllumChannelWdgt_new::updateLightCfgUi()
{

	IFileVariable *pFile = getLightCfgVariable(m_currentSence);
	for (int i = 0; i < LIGHT_SIZE; i++)	//4个光源
	{
		char lightName[128] = { 0 };
        sprintf(lightName, "light_%d", i + 1);
		switch (i)
		{
		case 0:
			ui->groupBox_0->setTitle(QString::fromLocal8Bit(pFile->getValue(lightName, "name").toCString().c_str()));
			break;
		case 1:
			ui->groupBox_1->setTitle(QString::fromLocal8Bit(pFile->getValue(lightName, "name").toCString().c_str()));
			break;
		case 2:
			ui->groupBox_2->setTitle(QString::fromLocal8Bit(pFile->getValue(lightName, "name").toCString().c_str()));
			break;
		case 3:
			ui->groupBox_3->setTitle(QString::fromLocal8Bit(pFile->getValue(lightName, "name").toCString().c_str()));
			break;
		default:
			break;
		}
		for (int j = 0; j < CD_Size; ++j)
		{
			char typeName[128] = { 0 };
			sprintf(typeName, "type_%d", j + 1);
			QString  uiTypeName = QString("label_%1_%2").arg(i).arg(j + 1);
			setUIItemValue(uiTypeName, QString::fromLocal8Bit(pFile->getValue(lightName, typeName).toCString().c_str()));
		}
	}
}

void IllumChannelWdgt_new::onEnable(bool enable)
{
	setEnabled(enable);
}

void IllumChannelWdgt_new::onSave()
{
    IFileVariable *pFile = getIniLightEleFileVariable_new();
    if (NULL != pFile)
    {
        pFile->setValue(LIGHT_SCENE_NAME, m_currentSence, true);
    }
	on_pushButton_saveFile_clicked();
}

void IllumChannelWdgt_new::onCancel()
{
    onUpdate();
}

void IllumChannelWdgt_new::onUpdate()
{
    ReadCfgToUi();
}

bool IllumChannelWdgt_new::ReadCfgToUi()
{
    bool bRet = false;

	int iEnablecnt = 0;
    IFileVariable *pFile = getIniLightEleFileVariable_new(m_strProductName.toStdString(), m_currentSence);
	if (NULL != pFile)
	{
		iEnablecnt = pFile->getValue(LIGHT_ENABLE_CNT).toInt();
		enableUIItem("groupBox_0", iEnablecnt > 0);
		enableUIItem("groupBox_1", iEnablecnt > 1);
		enableUIItem("groupBox_2", iEnablecnt > 2);
		enableUIItem("groupBox_3", iEnablecnt > 3);
	}

    bRet = m_plightcontrol_new->getllumChannelCfg( m_Channel_Index, m_iSection, &m_IllumChannelValue[m_iSection]);
	if (bRet)
	{
		updateUI(&m_IllumChannelValue[m_iSection]);
	}
    else
    {
        ui->listWidget_log->addItem(QString::fromLocal8Bit("获取配置错误"));
    }
	return false;
}

bool IllumChannelWdgt_new::UiToCfg()
{
	m_IllumChannelValue[m_iSection].iscan_line_count = getUItemValue("spinBox_scan_line", 0).toInt();
    m_IllumChannelValue[m_iSection].CamEnable[0] = getUItemValue("checkBox_CAMERA_0", 0).toBool();
    m_IllumChannelValue[m_iSection].CamEnable[1] = getUItemValue("checkBox_CAMERA_1", 0).toBool();
    m_IllumChannelValue[m_iSection].CamEnable[2] = getUItemValue("checkBox_CAMERA_2", 0).toBool();
    m_IllumChannelValue[m_iSection].CamEnable[3] = getUItemValue("checkBox_CAMERA_3", 0).toBool();

    IFileVariable *pFile = getIniLightEleFileVariable_new(m_strProductName.toStdString(), m_currentSence);
    int iLightEnableCnt  = LIGHT_SIZE;
    if (NULL != pFile)
    {
         //iLightEnableCnt = pFile->getValue(LIGHT_ENABLE_CNT).toInt();
    }

	for (int i = 0; i < LIGHT_SIZE;i ++)
	{
		for (int cd = 0; cd < CD_Size; cd++)
		{
			for (int section = 0; section < Section_Size; section++)
			{
				m_IllumChannelValue[m_iSection].LightEnale[i].type = (Light_Type) i;

				char keyName[128] = { 0 };
				int icdIndex = cd + 1;
				int iSectionIndex = section + 1;
                sprintf(keyName, "checkBox_%d_cd%d_%d", i, icdIndex, iSectionIndex);
                //if(i >= iLightEnableCnt)
                //{
                //    setUIItemValue(keyName,0);
                //}

				m_IllumChannelValue[m_iSection].LightEnale[i].enable[cd][section] = getUItemValue(keyName,0).toBool();
			}
			char keyName[128] = { 0 };
            sprintf(keyName, "doubleSpinBox_%d_pluse%d", i, cd + 1);
            //if(i >= iLightEnableCnt)
            //{
            //    setUIItemValue(keyName,0);
            //}
			m_IllumChannelValue[m_iSection].LightEnale[i].ipulse[cd] = getUItemValue(keyName, 0).toInt();
		}
	}
	return true;
}

void IllumChannelWdgt_new::updateUI(stIllumChannelCfg *pValue)
{;
	setUIItemValue("spinBox_scan_line", m_IllumChannelValue[m_iSection].iscan_line_count);
    setUIItemValue("checkBox_CAMERA_0", m_IllumChannelValue[m_iSection].CamEnable[0]);
    setUIItemValue("checkBox_CAMERA_1", m_IllumChannelValue[m_iSection].CamEnable[1]);
    setUIItemValue("checkBox_CAMERA_2", m_IllumChannelValue[m_iSection].CamEnable[2]);
    setUIItemValue("checkBox_CAMERA_3", m_IllumChannelValue[m_iSection].CamEnable[3]);

	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		for (int cd = 0; cd < CD_Size; cd++)
		{
			for (int section = 0; section < Section_Size; section++)
			{
				m_IllumChannelValue[m_iSection].LightEnale[i].type = (Light_Type)i;

				char keyName[128] = { 0 };
				int icdIndex = cd + 1;
				int iSectionIndex = section + 1;
                sprintf(keyName, "checkBox_%d_cd%d_%d", i, icdIndex, iSectionIndex);
				setUIItemValue(keyName, m_IllumChannelValue[m_iSection].LightEnale[i].enable[cd][section]);
			}

			char keyName[128] = { 0 };
            sprintf(keyName, "doubleSpinBox_%d_pluse%d", i, cd+1);
			setUIItemValue(keyName, m_IllumChannelValue[m_iSection].LightEnale[i].ipulse[cd]);
		}
	}
}

void IllumChannelWdgt_new::on_pushButton_send_clicked()
{
	UiToCfg();

	// 写本地文件
	bool bRet = m_plightcontrol_new->setllumChannelCfg( m_Channel_Index, m_iSection, &m_IllumChannelValue[m_iSection], m_currentSence);
	if (!bRet)
	{
		ui->listWidget_log->addItem(QString::fromLocal8Bit("保存配置文件失败"));
		return;
	}
	ui->listWidget_log->addItem(QString::fromLocal8Bit("保存配置文件成功"));

	bRet = m_plightcontrol_new->writellumChannelCfgToFlash( m_Channel_Index, m_iSection, &m_IllumChannelValue[m_iSection]);

    ui->listWidget_log->addItem(QString::fromLocal8Bit("写光源控制器成功"));
}

void IllumChannelWdgt_new::on_pushButton_SendMutFlash_clicked()
{
	UiToCfg();
	int iRet = m_plightcontrol_new->setMutFlashParams();
	if (iRet != 0)
	{
		ui->listWidget_log->addItem(QString::fromLocal8Bit("写光源控制器失败(setMutFlashParams)"));
		return;
	}
	QMessageBox::information(this, "Info", QString::fromLocal8Bit("写光源控制器成功(setMutFlashParams)"));
}

void IllumChannelWdgt_new::on_pushButton_SendAllMutFlash_clicked()
{
	UiToCfg();

	// 写控制器
	int iRet = m_plightcontrol_new->setAllMutFlashParams();
	if (iRet != 0)
	{
		ui->listWidget_log->addItem(QString::fromLocal8Bit("写光源控制器失败(setAllMutFlash)"));
		return;
	}
}

void IllumChannelWdgt_new::on_pushButton_saveFile_clicked()
{
	UiToCfg();
	bool bRet = m_plightcontrol_new->setllumChannelCfg( m_Channel_Index, m_iSection, &m_IllumChannelValue[m_iSection], m_currentSence);
	if (!bRet)
	{
		ui->listWidget_log->addItem(QString::fromLocal8Bit("获取配置错误"));
		return;
	}
	QMessageBox::information(this, "Info", QString::fromLocal8Bit("保存配置文件成功"));
}
void IllumChannelWdgt_new::on_pushButton_clear_clicked()
{
	ui->listWidget_log->clear();
}

void IllumChannelWdgt_new::on_pushButton_model_set_clicked(bool checked)
{

}

void IllumChannelWdgt_new::slotUpdateSenceName(string strSenceName)
{
    m_currentSence = strSenceName;
	ReadCfgToUi();
}

void IllumChannelWdgt_new::on_radioButton_section1_clicked()
{
	m_iSection = Scan_line_1;
	ReadCfgToUi();
}
void IllumChannelWdgt_new::on_radioButton_section2_clicked()
{
	m_iSection = Scan_line_2;
	ReadCfgToUi();
}

void IllumChannelWdgt_new::on_radioButton_section3_clicked()
{
	m_iSection = Scan_line_3;
	ReadCfgToUi();
}

void IllumChannelWdgt_new::on_radioButton_section4_clicked()
{
	m_iSection = Scan_line_4;
	ReadCfgToUi();
}

void IllumChannelWdgt_new::on_radioButton_section5_clicked()
{
	m_iSection = Scan_line_5;
	ReadCfgToUi();
}

void IllumChannelWdgt_new::on_save_config(int cameraIndex)
{
	QList<QCheckBox*> cameraList;
    cameraList << ui->checkBox_CAMERA_0 << ui->checkBox_CAMERA_1 << ui->checkBox_CAMERA_2 << ui->checkBox_CAMERA_3;
	for (int i = 0; i < cameraList.size();i++)
	{
		if (i==cameraIndex-1)
		{
			cameraList.at(i)->setChecked(true);
		}
		else
		{
			cameraList.at(i)->setChecked(false);
		}		
	}
	on_pushButton_send_clicked();
}

bool IllumChannelWdgt_new::checkIsError()
{
    return false;
}

Channel_Index IllumChannelWdgt_new::ChannelNameToIndex(QString strChannelName)
{
	QString strChannel[Channel_Index_Size] = { "SEQUENCE_A","SEQUENCE_B","SEQUENCE_C","SEQUENCE_D","SEQUENCE_E","SEQUENCE_F","SEQUENCE_G","SEQUENCE_H" };
	for (int i = 0; i < 8;i++)
	{
		if (strChannelName == strChannel[i])
		{
			return (Channel_Index)i;
		}
	}
	return Channel_Index_A;
}

void IllumChannelWdgt_new::readAllCfg()
{
	for (int i = 0; i < Scan_line_Size;i++)
	{
		m_plightcontrol_new->getllumChannelCfg(m_Channel_Index, (Scan_line_num)i, &m_IllumChannelValue[i]);
	}
}

void IllumChannelWdgt_new::showEvent(QShowEvent *event)
{
	onUpdate();
    update();
}
#define POWER_VAL     0.01
InitLightWdgt_new::InitLightWdgt_new(int index , QWidget *parent /*= 0*/ ):
	AbstractPageWidget(parent),
    ui(new Ui::ilightForm_new),
	m_index(index)
{
    ui->setupUi(this);
	m_plightcontrol_new = lightcontrol_new_manager::GetInstance()->getLightcontrol(index);
	m_bstroboflash = false;
	setUIItemValue("checkBox_stroboflash_on", 1);
	initUi();
}

InitLightWdgt_new::~InitLightWdgt_new()
{
	delete ui;
}
void InitLightWdgt_new::initUi()
{
	//ui->groupBox_front->hide();
	//ui->groupBox_rear->hide();

	m_plightcontrol_new->getInitEleCfg(&m_stEleCfg);
	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		for (int cd = 0; cd < 6; cd++)
		{
			for (int section = 0; section < 5; section++)
			{
				//int icdIndex = cd + 1;
				//int iSectionIndex = section + 1;
				//QString strDoubleSpinBox = QString("doubleSpinBox_%1_cd%2_%3").arg(lightName[i].arg(icdIndex).arg(iSectionIndex));

				char keyName[128] = { 0 };
				int icdIndex = cd + 1;
				int iSectionIndex = section + 1;
				sprintf(keyName, "doubleSpinBox_%d_cd%d_%d", i, icdIndex, iSectionIndex);

				setUIItemValue(keyName, m_stEleCfg.EleValue[i][cd][section]);
			}
		}
	}

    return ;
}

void InitLightWdgt_new::updateUI()
{

}

void InitLightWdgt_new::getUiData()
{
	// 频闪,常量
    int iLightCnt = getUItemValue("spinBox_light_cnt", 0).toInt();
	m_stEleCfg.model = getUItemValue("checkBox_stroboflash_on", 0).toBool() ? LS_MODEL_FLASH: LS_MODEL_NORMAL; 
	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		for (int cd = 0; cd < 6; cd++)
		{
			for (int section = 0; section < 5; section++)
			{
				char keyName[128] = { 0 };
				int icdIndex = cd + 1;
				int iSectionIndex = section + 1;
				sprintf(keyName, "doubleSpinBox_%d_cd%d_%d", i,icdIndex, iSectionIndex);
				m_stEleCfg.EleValue[i][cd][section] = getUItemValue(keyName,0).toInt();
                if(i >= iLightCnt)
                {
                    m_stEleCfg.EleValue[i][cd][section] = 0;
                    setUIItemValue(keyName,0);
                }
			}

			char EnableName[128] = { 0 };
			int icdtmp= cd + 1;
			sprintf(EnableName, "checkBox_%d_cd%d", i, icdtmp);
			m_stEleCfg.alwaysOn[i][cd] = getUItemValue(EnableName, 0).toBool();
            if(i >= iLightCnt)
            {
                m_stEleCfg.alwaysOn[i][cd] = 0;
                setUIItemValue(EnableName,0);
            }
		}
	}
}

void InitLightWdgt_new::saveIpAdd()
{
	IFileVariable *pFile = getLightComFile();
	if (pFile == NULL)
	{
		return;
	}
    int iLightCnt = getUItemValue("spinBox_light_cnt", 0).toInt();
	char lightStation[128] = { 0 };
	sprintf(lightStation, "LightIp_%d", m_index);
	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		char lightName[128] = { 0 };
		char lightPort[128] = { 0 };
		char lightEnable[128] = { 0 };
		

		QString  lineEditIP = QString("lineEdit_%1_ip").arg(i);
		QString  port = QString("lineEdit_%1_port").arg(i);
		QString  enable = QString("checkBox_%1_enable_light").arg(i);
		

		sprintf(lightName, "light_%d", i);
		sprintf(lightPort, "port_%d", i);
		sprintf(lightEnable, "enable_%d", i);

        if(i >= iLightCnt)
        {
            setUIItemValue(lineEditIP,"0");
            setUIItemValue(port,0);
            setUIItemValue(enable,0);
        }
		pFile->setValue(lightStation, lightName, getUItemValue(lineEditIP, 0).toString().toStdString() , true);

		pFile->setValue(lightStation, lightPort, getUItemValue(port, 0).toInt() , true);

		pFile->setValue(lightStation, lightEnable, getUItemValue(enable, 0).toBool(), true);
	}

	pFile->setValue(lightStation, "Light_cnt", getUItemValue("spinBox_light_cnt", 0).toInt(),true);
	
}

void InitLightWdgt_new::updateIPAddUI()
{
	bool bEn = false;

	IFileVariable *pFile = getLightComFile();
	if (pFile)
	{
		char lightStation[128] = { 0 };
		sprintf(lightStation, "LightIp_%d", m_index);

		for (int i = 0; i < LIGHT_SIZE; i++)
		{
			char lightName[128] = { 0 };
			char lightPort[128] = { 0 };
			char lightEnable[128] = { 0 };

			QString  lineEditIP = QString("lineEdit_%1_ip").arg(i);
			QString  port = QString("lineEdit_%1_port").arg(i);
			QString  enable = QString("checkBox_%1_enable_light").arg(i);
			sprintf(lightName, "light_%d", i);
			sprintf(lightPort, "port_%d", i);
			sprintf(lightEnable, "enable_%d", i);

			setUIItemValue(lineEditIP, QString::fromStdString(pFile->getValue(lightStation, lightName).toCString()));
			setUIItemValue(port, QString::fromStdString(pFile->getValue(lightStation, lightPort).toCString()));
			setUIItemValue(enable, QString::fromStdString(pFile->getValue(lightStation, lightEnable).toCString()));
		}
		int light_cnt = pFile->getValue(lightStation, "Light_cnt", 1).toInt();
		setUIItemValue("spinBox_light_cnt", light_cnt);
	}
}

void InitLightWdgt_new::updateLightInfoAddUI()
{
		QString index = QString::number(m_index);
		string lightIndex = index.toStdString();
		IFileVariable *pFile = getLightCfgVariable(lightIndex);
		for (int i = 0; i < LIGHT_SIZE; i++)	//4个光源
		{
			char lightName[128] = { 0 };
			sprintf(lightName, "light_%d", i + 1);
			//QString  uiLightName = QString("groupBox_%1").arg(i);
			switch (i)
			{
			case 0:
				ui->groupBox_0->setTitle(QString::fromLocal8Bit(pFile->getValue(lightName, "name").toCString().c_str()));
				break;
			case 1:
				ui->groupBox_1->setTitle(QString::fromLocal8Bit(pFile->getValue(lightName, "name").toCString().c_str()));
				break;
			case 2:
				ui->groupBox_2->setTitle(QString::fromLocal8Bit(pFile->getValue(lightName, "name").toCString().c_str()));
				break;
			case 3:
				ui->groupBox_3->setTitle(QString::fromLocal8Bit(pFile->getValue(lightName, "name").toCString().c_str()));
				break;
			default:
				break;
			}

			//setUIItemValue(uiLightName, QString::fromStdString(pFile->getValue(lightName, "name").toCString()));
			//groupBox_0
			for (int j = 0; j < CD_Size; ++j)
			{
				char typeName[128] = { 0 };
				sprintf(typeName, "type_%d", j + 1);
				QString  uiTypeName = QString("label_%1_%2").arg(i).arg(j + 1);
				setUIItemValue(uiTypeName, QString::fromLocal8Bit(pFile->getValue(lightName, typeName).toCString().c_str()));
			}
		}
}



void InitLightWdgt_new::onEnable(bool enable)
{
	setEnabled(enable);
}

void InitLightWdgt_new::onSave()
{
	getUiData();
	saveIpAdd();
	m_plightcontrol_new->setInitEleCfg(&m_stEleCfg);
}

void InitLightWdgt_new::onCancel()
{
    onUpdate();
}

void InitLightWdgt_new::onUpdate()
{
	int iEnablecnt = 0;
    IFileVariable *pFile = getIniLightEleFileVariable_new(m_strProductName.toStdString(),SString::IntToStr(m_index));
    if (NULL != pFile)
    {
        m_currentSence = pFile->getValue(LIGHT_SCENE_NAME).toCString();
		iEnablecnt = pFile->getValue(LIGHT_ENABLE_CNT).toInt();
    }
	
    initUi();
	updateIPAddUI();
	updateLightInfoAddUI();
	//setUIItemValue("spinBox_light_cnt", iEnablecnt);
	on_spinBox_light_cnt_valueChanged(iEnablecnt);
}



//   写入配置到控制器
void InitLightWdgt_new::on_pushButton_writedata_clicked()
{
	// 发送配置先保存
	on_pushButton_svae_set_clicked();
	bool bFlag = true;
	bool checked = getUItemValue("checkBox_stroboflash_on", 0).toBool();	
	if (checked != m_bstroboflash)
	{
		QString strMode = checked ? QString::fromLocal8Bit("设置频闪模式") : QString::fromLocal8Bit("设置常亮模式");
		uint8_t ls_model = checked?0:1;
		int iRet = m_plightcontrol_new->setFlashMode(ls_model);
		QString strMsg = iRet==0 ? QString::fromLocal8Bit("成功") : QString::fromLocal8Bit("失败");
		ui->listWidget_log->addItem(strMode + strMsg);
		if (bFlag)
		{
			m_bstroboflash = checked;
			setUIItemValue("checkBox_stroboflash_on", m_bstroboflash);
		}			
	}
	if (!bFlag)
	{
		return;
	}
	if (m_bstroboflash)
	{
		//频闪 设置频闪模式并设置电流值		
		bFlag = m_plightcontrol_new->writeEleCfgToFlash(&m_stEleCfg);
		QString strMsg = bFlag ? QString::fromLocal8Bit("成功") : QString::fromLocal8Bit("失败");
		ui->listWidget_log->addItem(QString::fromLocal8Bit("频闪模式,下发电流指令") + strMsg);
	}
	else
	{
		//常亮 设置常亮模式并设置灯管使能
		int iRet = m_plightcontrol_new->setLsSwitch();
		QString strMsg = iRet == 0 ? QString::fromLocal8Bit("成功") : QString::fromLocal8Bit("失败");
		ui->listWidget_log->addItem(QString::fromLocal8Bit("常亮模式,下发常亮使能指令") + strMsg);
	}
	return;
}

//   从控制器读取配置
void InitLightWdgt_new::on_pushButton_read_clicked()
{
	bool bFlag = m_plightcontrol_new->readEleCfgFromFlash(&m_stEleCfg);
	QString strMsg = bFlag ? QString::fromLocal8Bit("成功") : QString::fromLocal8Bit("失败");
	ui->listWidget_log->addItem(QString::fromLocal8Bit("保存电流参数") + strMsg);
	updateUI();
	return;
}

//   清除控制器计数
void InitLightWdgt_new::on_pushButton_clear_clicked()
{
	int iRet = m_plightcontrol_new->clearCounter();
	QString strMsg = iRet==0 ? QString::fromLocal8Bit("成功") : QString::fromLocal8Bit("失败");
	ui->listWidget_log->addItem(QString::fromLocal8Bit("清除计数") + strMsg);
	return;
}

void InitLightWdgt_new::on_pushButton_clear2_clicked()
{
	int iRet = m_plightcontrol_new->clearCounter();
	QString strMsg = iRet == 0 ? QString::fromLocal8Bit("成功") : QString::fromLocal8Bit("失败");
	ui->listWidget_log->addItem(QString::fromLocal8Bit("清除计数") + strMsg);
	return;
}

//   保存设置到文件
void InitLightWdgt_new::on_pushButton_svae_set_clicked()
{
	getUiData();
	bool bFlag = m_plightcontrol_new->setInitEleCfg(&m_stEleCfg);
	QString strMsg = bFlag ? QString::fromLocal8Bit("成功") : QString::fromLocal8Bit("失败");
	ui->listWidget_log->addItem(QString::fromLocal8Bit("保存电流参数") + strMsg);
	return;
}


//   设置另存为
void InitLightWdgt_new::on_pushButton_save_as_clicked()
{
	return;
}

//   加载配置文件
void InitLightWdgt_new::on_pushButton_loadcfg_clicked()
{
	return;
}

//   清空以下数据
void InitLightWdgt_new::on_pushButton_clear_print_clicked()
{
	ui->listWidget_log->clear();
	return;
}

void InitLightWdgt_new::on_pushButton_0_cd1_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_0_cd2_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_0_cd3_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_0_cd4_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_0_cd5_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_0_cd6_clicked()
{
    on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_1_cd1_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_1_cd2_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_1_cd3_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_1_cd4_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_1_cd5_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_1_cd6_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_2_cd1_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_2_cd2_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_2_cd3_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_2_cd4_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_2_cd5_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_2_cd6_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_3_cd1_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_pushButton_3_cd2_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_3_cd3_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_3_cd4_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_3_cd5_clicked()
{
	on_pushButton_writedata_clicked();
}
void InitLightWdgt_new::on_pushButton_3_cd6_clicked()
{
	on_pushButton_writedata_clicked();
}

void InitLightWdgt_new::on_spinBox_light_cnt_valueChanged(int i)
{
	int iCnt = getUItemValue("spinBox_light_cnt", 0).toInt();
	ui->groupBox_0->setEnabled(iCnt >0);
	ui->groupBox_1->setEnabled(iCnt >1);
	ui->groupBox_2->setEnabled(iCnt >2);
	ui->groupBox_3->setEnabled(iCnt >3);

    IFileVariable *pFile = getIniLightEleFileVariable_new(m_strProductName.toStdString(), SString::IntToStr(m_index));
	if (NULL != pFile)
	{
		 pFile->setValue(LIGHT_ENABLE_CNT, iCnt);
	}
}

void InitLightWdgt_new::on_pushButton_0_getlight_clicked()
{
	uint8_t ls_type;
	Light_Type type = LIGHT_0;
	int iRet = m_plightcontrol_new->getCtrType(ls_type, type);
	QString msg = (0 == iRet) ? QString::fromLocal8Bit("获取光源类型成功 ls_type = %1").arg(ls_type) : QString::fromLocal8Bit("获取光源类型失败");
	QMessageBox::information(this, "Info", msg);;

}

void InitLightWdgt_new::on_pushButton_1_getlight_clicked()
{
	uint8_t ls_type;
	Light_Type type = LIGHT_1;
	int iRet = m_plightcontrol_new->getCtrType(ls_type, type);
	QString msg = (0 == iRet) ? QString::fromLocal8Bit("获取光源类型成功 ls_type = %1").arg(ls_type) : QString::fromLocal8Bit("获取光源类型失败");
	QMessageBox::information(this, "Info", msg);;
}
void InitLightWdgt_new::on_pushButton_2_getlight_clicked()
{
	uint8_t ls_type;
	Light_Type type = LIGHT_2;
	int iRet = m_plightcontrol_new->getCtrType(ls_type, type);
	QString msg = (0 == iRet) ? QString::fromLocal8Bit("获取光源类型成功 ls_type = %1").arg(ls_type) : QString::fromLocal8Bit("获取光源类型失败");
	QMessageBox::information(this, "Info", msg);;
}
void InitLightWdgt_new::on_pushButton_3_getlight_clicked()
{
	uint8_t ls_type;
	Light_Type type = LIGHT_3;
	int iRet = m_plightcontrol_new->getCtrType(ls_type, type);
	QString msg = (0 == iRet) ? QString::fromLocal8Bit("获取光源类型成功 ls_type = %1").arg(ls_type) : QString::fromLocal8Bit("获取光源类型失败");
	QMessageBox::information(this, "Info", msg);;
}
