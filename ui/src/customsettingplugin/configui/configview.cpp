#include "configview.h"
#include "ui_configview.h"
#include "configoper.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "g12globalenv.h"
#include "uicommon/commonutils.h"
#include "inotifyservice.h"
#include <QFileDialog>
#include "mainflow/imainflowcfg.h"
#include "mainflow/imainflowmgr.h"
#include "mainflow/imainflow.h"
#include "proxymanager.h"
#include "rtdefine.h"
#include "inotifyservice.h"
#include "uicommon/commonutils.h"
#include "stringcommon.h"
#include "netdevui/isocketlistcfg.h"
#include "netdevui/isocketproxy.h"
#include "iuiplugin.h"
#include "maindefine.h"
#include "lightcontrol_new.h"

#include <iostream>
#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/Prettywriter.h"
#include "rapidjson/stringbuffer.h"



using namespace rapidjson;
ConfigView::ConfigView(QFrame *parent) :
    QFrame(parent),
    m_currentImageFileIndex(0),
    m_ImageFilesCount(0),
	m_icurrentSN(0),
    ui(new Ui::ConfigView)
{
    ui->setupUi(this);
    InitUI();
}

ConfigView::~ConfigView()
{
    delete ui;
}
void ConfigView::InitUI()
{
    //ui->LightCom->Init();
    //ui->CameraParam->Init();
	ui->checkBox_auto->hide();//20200224
    updateCameraSet();
    ui->widget_grab_set->setEnabled(false);
    ui->widget_enable_set->setEnabled(false);
    ui->widget_image_set->setEnabled(false);
    ui->pushButton_OK->hide();
    ui->pushButton_Enable_OK->hide();
    //on_pbtn_cut_cancel_clicked();
    ui->pushButton_Image_Cancel->setText(QString::fromLocal8Bit("取消"));
    //ui->pushButton_R_Cancel->setText(QString::fromLocal8Bit("取消"));
    //ui->pushButton_Overlap_Cancel->setText(QString::fromLocal8Bit("取消"));
    ui->pushButton_H_alg_Cancel->setText(QString::fromLocal8Bit("取消"));
	ui->pushButton_data_Cancel->setText(QString::fromLocal8Bit("取消"));
	ui->pushButton_warn_Cancel->setText(QString::fromLocal8Bit("取消"));

    on_pushButton_Image_Cancel_clicked();
    ui->label_product_name->setText(QString::fromStdString(getCurrentProgramName()));
	//ui->comboBox_station_index->addItem();
	int stationCnt = getImageStationCnt();
	for (int i = 0; i < stationCnt; ++i)
	{
		QString stationName = QString::fromLocal8Bit(getStationName(i).c_str());
		ui->comboBox_station_index->addItem(stationName);
	}
	//ui->comboBox_station_index->setCurrentIndex(0);
    //on_pushButton_R_Cancel_clicked();
    on_pushButton_H_alg_Cancel_clicked();
	on_pushButton_data_Cancel_clicked();
	on_pushButton_warn_Cancel_clicked();
	

    IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
    int iModel = 0;
    if (NULL != pFileVariable)
    {
        iModel = pFileVariable->getValue(PRODUCT_LINE_RUN_MODEL, 0).toInt();
    }
    if (iModel == 0)
    {
        ui->radioButton_pingpong->setChecked(true);
    }
    else if (iModel == 1)
    {
        ui->radioButton_piece_by_piece->setChecked(true);
    }
    else
    {
        ui->radioButton_debug->setChecked(true);
    }
	//
	ui->label_57->hide();
	ui->checkBox_Enbale_Debug->hide();
	ui->label_41->hide();
	ui->label_56->hide();
	ui->checkBox_enable_fast->hide();
	ui->checkBox_Bunite->hide();
	//ui->checkBox_defect_detect->hide();	
	ui->label_577->hide();
	ui->checkBox_debug_mode->hide();
	ui->checkBox_EnbaleWhole->hide();
	//
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile)
	{
		int iChooseTime = pFile->getValue(WORK_MODE_AM_STRAT_TIME).toInt(8);//
		ui->comboBox_work_time_start->setCurrentIndex(iChooseTime);

		int iChooseFront= pFile->getValue(WORK_MODE_FRONT_CHOOSE).toInt();//
		ui->comboBox_choose_front->setCurrentIndex(iChooseFront);

        int CgTimeout = pFile->getValue(IMAGE_PARAM_CGBLOCK_TIMEOUT).toInt();
        ui->spinBox_cgblock->setValue(CgTimeout);

        int iDelaytime = pFile->getValue(IMAGE_PARAM_H_TIMEOUT_ENABLE).toInt();
        ui->spinBox_DvOutTime->setValue(iDelaytime);
	}
	
    //int station = JSON_CONFIG_FOR_BACK_DOWN;	//上下工位算法超时开启一致
    //bool flag = ReadJsonFile(string(JSON_TIMEOUT_SWITCH_KEY),station) == 0 ? false : true;
    ui->checkBox_H_timeout->setChecked(pFile->getValue(IMAGE_PARAM_H_TIMEOUT_ENABLE).toBool());


}
void ConfigView::on_comboBox_station_index_currentIndexChanged(int index)
{
	updateCameraSet();
}
void ConfigView::updateCameraSet()
{
	int index = ui->comboBox_station_index->currentIndex();
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	string stationIndex = SString::IntToStr(index);
	string key = CAMERAPARAM;
	key = key + "_" + stationIndex;
	if (pFile)
	{
		ui->spinBox_clear_time->setValue(pFile->getValue(CAMERAPARAM_CLAR_TIME).toInt());
		ui->dSpeed->setValue(pFile->getValue(CAMERAPARAM_SPEED).toDouble());
		ui->moveunit->setValue(pFile->getValue(key, CAMERAPARAM_MOVEUNIT).toDouble());

		ui->plusewidgh->setValue(pFile->getValue(key, CAMERAPARAM_PLUSEWIDGH).toDouble());

        ui->Fivs->setValue(pFile->getValue(key, CAMERAPARAM_FIVS).toInt());

		ui->dGetImageLen->setValue(pFile->getValue(key, CAMERAPARAM_DGETIMAGELEN).toDouble());
        ui->Fstartpos->setValue(pFile->getValue(key, CAMERAPARAM_FSTARTPOS).toDouble());

       
		
        ui->checkBox_save_source_image->setChecked(pFile->getValue(IMAGE_PARAM_SAVE_SIMAGE_ENABLE).toBool());
		ui->checkBox_save_dv_error_source_image->setChecked(pFile->getValue(IMAGE_PARAM_SAVE_DV_ER_SIMAGE_ENABLE).toBool());
		ui->checkBox_save_static_image->setChecked(pFile->getValue(IMAGE_PARAM_STATIC_SAVE_ENABLE).toBool());
        ui->checkBox_enable_fast->setChecked(pFile->getValue(IMAGE_PARAM_FAST_GRAB_ENABLE).toBool());

        ui->checkBox_defect_detect->setChecked(pFile->getValue(IMAGE_PARAM_DEFECT_DETECT_ENABLE).toBool());
        ui->checkBox_h_sy_detect->setChecked(pFile->getValue(IMAGE_PARAM_H_SY_DETECT_ENABLE).toBool());
        ui->checkBox_Bunite->setChecked(pFile->getValue(IMAGE_PARAM_H_ENABLEBUNITE).toBool());
        
        ui->checkBox_EnbaleWhole->setChecked(pFile->getValue(IMAGE_PARAM_IS_WHOLE).toBool());
    
        
		ui->checkBox_save_defectfile->setChecked(pFile->getValue(IMAGE_PARAM_SAVE_FILE_ENABLE,true).toBool());
		ui->checkBox_enable_level->setChecked(pFile->getValue(IMAGE_PARAM_DEFECT_LEVEL_ENABLE,true).toBool());
		ui->checkBox_h_level->setChecked(pFile->getValue(IMAGE_PARAM_ENABLE_H_LEVEL, false).toBool());
        ui->checkBox_Enbale_Debug->setChecked(pFile->getValue(IMAGE_PARAM_DEBUG_MODEL_ENABLE, false).toBool());
        //ui->checkBox_measrue->setChecked(pFile->getValue(IMAGE_PARAM_MEASURE_ENABLE, false).toBool());
        ui->checkBox_autoDeleteImage->setChecked(pFile->getValue(AUTO_DELETE_IMAGE_ENABLE, false).toBool());
		ui->checkBox_autoDeleteData->setChecked(pFile->getValue(AUTO_DELETE_DATA_ENABLE, false).toBool());

		ui->spinBox_daysDeleteData->setValue(pFile->getValue(AUTO_DELETE_IMAGE_DAYS, 7).toInt());  //超过天数删除数据
		ui->spinBox_daysDeleteImage->setValue(pFile->getValue(AUTO_DELETE_DATA_DAYS, 7).toInt());	//超过天数删除图片

		ui->MoveLenth->setValue(pFile->getValue(CAMERAPARAM_MOVELENTH).toInt());
		ui->normal_time->setValue(pFile->getValue(CAMERAPARAM_NORMAL_TIME).toInt());
		ui->busy_time->setValue(pFile->getValue(CAMERAPARAM_BUSY_TIME).toInt());
		ui->busy_mark->setValue(pFile->getValue(CAMERAPARAM_BUSY_MARK).toInt());

		ui->checkBox_debug_crl_clear->setChecked(pFile->getValue(IMAGE_PARAM_DISABLE_CLEAN, false).toBool());
        ui->checkBox_debug_up_flow->setChecked(pFile->getValue(IMAGE_PARAM_DISABLE_SHANG, false).toBool());
        ui->checkBox_debug_down_flow->setChecked(pFile->getValue(IMAGE_PARAM_DISABLE_XIA,false).toBool());
		ui->EnableFront->setChecked(pFile->getValue(IMAGE_PARAM_ENABLE_FRONT, false).toBool());
        ui->checkBox_debug_clear_mode->setChecked(pFile->getValue(IMAGE_PARAM_DISABLE_CLEAR_MODE,false).toBool());
		ui->checkBox_debug_mode->setChecked(pFile->getValue(IMAGE_PARAM_DEBUG).toBool());
		ui->checkBox_EnbaleStatic->setChecked(pFile->getValue(IMAGE_PARAM_RIGHT_IMAGE_ENABLE).toBool());

        int sendResultMode = pFile->getValue(RANDOM_SEND_RESULT_MODE,0).toInt();
        int oneResultMode = pFile->getValue(RANDOM_ONE_RESULT_MODE, 0).toInt();
        int twoResultMode = pFile->getValue(RANDOM_TWO_RESULT_MODE, 0).toInt();
        ui->comboBox_oneResult->setCurrentIndex(oneResultMode);
        ui->comboBox_twoResult->setCurrentIndex(twoResultMode);
		if (sendResultMode == sendResultMode::randomClose)
		{
			ui->checkBox_randomClose->setChecked(true);
		}
		else if (sendResultMode == sendResultMode::randomTwoBin)
		{
			ui->checkBox_randomTwoBins->setChecked(true);
		}
		else if (sendResultMode == sendResultMode::randomThreeBin)
		{
			ui->checkBox_randomThreeBins->setChecked(true);
		}
		else if (sendResultMode == sendResultMode::randomOneBin)
		{
			ui->checkBox_randomOneBin->setChecked(true);
		}
		
		CalculateToUi();
        update();
    }
    IFileVariable *pFileImage = getCurrentProgramFileVariable(CAM_ATTRIBUTE_FILE_NAME);
    if (NULL != pFileImage)
    {
        ui->DulLines->setValue(pFileImage->getValue(IMAGE_PARAM_DUL_LINES).toInt());
    }
}

void ConfigView::on_pushButton_OK_clicked()
{
    int uintLines = 256;
    IFileVariable *pFileCam = getCurrentProgramFileVariable(CAM_ATTRIBUTE_FILE_NAME);
    if (NULL != pFileCam)
    {
        uintLines = pFileCam->getValue(CAM_ATTRIBUTE_BLOCKSIZE).toInt(256);

    }
	int index = ui->comboBox_station_index->currentIndex();
	//面阵相机
	if (1 == getStationType(index))
	{
		uintLines = 10;
	}
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	string stationIndex = SString::IntToStr(index);
	string key = CAMERAPARAM;
	key = key + "_" + stationIndex;
	if (pFile)
	{
        double rt = ui->rtate_show->value();
        double dy = ui->plusewidgh->value()-ui->dutycyle->value();
		if(70<=rt)
        {
            ui->label_log->setStyleSheet("color:red;"); 
            ui->label_log->setText(QString::fromLocal8Bit("频率过大，已超过70K！保失败！"));
            //return;   //待修改
        }
        if (0<= dy)
        {
            ui->label_log->setStyleSheet("color:red;"); 
            ui->label_log->setText(QString::fromLocal8Bit("触发脉宽大于触发时长！保失败！"));
            //return;   //待修改
        }
		//
		double dOldLeng = pFile->getValue(key,CAMERAPARAM_DGETIMAGELEN).toDouble();
		double dNowLeng = ui->dGetImageLen->value();

        //长度补齐
        double moveUnit = ui->moveunit->value();
        int iLines = dNowLeng / moveUnit;
        if (0 != (iLines % uintLines))
        {
            iLines = ((iLines / uintLines) + 1) * uintLines;
        }
        dNowLeng = iLines * moveUnit;
        ui->dGetImageLen->setValue(dNowLeng);

        bool bUpdate = (dOldLeng != dNowLeng) ? true : false;

		//
        pFile->setValue(key,CAMERAPARAM_FIVS,ui->Fivs->value(),true);//
		pFile->setValue(CAMERAPARAM_SPEED,ui->dSpeed->value(),true);//速度
		pFile->setValue(key,CAMERAPARAM_MOVEUNIT,ui->moveunit->value(),true);//精度		
		pFile->setValue(key,CAMERAPARAM_DGETIMAGELEN, dNowLeng,true);//视野长度 
		pFile->setValue(key,CAMERAPARAM_PLUSEWIDGH,ui->plusewidgh->value(),true);//脉宽

		pFile->setValue(key,CAMERAPARAM_FSTARTPOS,ui->Fstartpos->value(),true);//启动延时
		
		
		pFile->setValue(key,CAMERAPARAM_FIVSCOUNT, iLines/*ui->Fivscount->value()*/,true);//单张图像行数
        pFile->setValue(CAMERAPARAM_ISEQUENCE, ui->iSequence->value(), true);//时序

		// normal_time
		pFile->setValue(CAMERAPARAM_MOVELENTH, ui->MoveLenth->value(), true);//
		pFile->setValue(CAMERAPARAM_NORMAL_TIME, ui->normal_time->value(), true);//时序
		pFile->setValue(CAMERAPARAM_BUSY_TIME, ui->busy_time->value(), true);//时序
		pFile->setValue(CAMERAPARAM_BUSY_MARK, ui->busy_mark->value(), true);//时序
		//;
        CalculateToUi();

		ui->pushButton_Cancel->setText(QString::fromLocal8Bit("修改"));
        ui->pushButton_OK->hide();
        ui->widget_grab_set->setEnabled(false);
        ui->label_log->setStyleSheet("color:blue;"); 
        ui->label_log->setText(QString::fromLocal8Bit("保存成功！"));
		if (bUpdate)
        {
            //保存图像长度
            IFileVariable* pFileVariable = getAcquisitionFileVariable();
            if (NULL != pFileVariable)
            {
                pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONY, iLines, true);
                pFileVariable->setValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGEHEIGHT, iLines, true);
            }
			INotifyService *pINotifyService = CommonUtils::getNotifyService();
			if (NULL != pINotifyService)
			{
				pINotifyService->send("mainui", NOTIFY_RESET_UPDATE_DECT, 0, 0);
			}
		}
    }
    IFileVariable *pFileImage = getCurrentProgramFileVariable(CAM_ATTRIBUTE_FILE_NAME);
    if (NULL != pFileImage)
    {
        pFileImage->setValue(IMAGE_PARAM_DUL_LINES,ui->DulLines->value(),true);//分块时图像重叠参数设置
    }
}

void ConfigView::on_pushButton_Cancel_clicked()
{
	if (QString::fromLocal8Bit("修改") == ui->pushButton_Cancel->text())
	{
        ui->pushButton_OK->show();
        ui->widget_grab_set->setEnabled(true);
		ui->pushButton_Cancel->setText(QString::fromLocal8Bit("取消"));
	}
	else{
        updateCameraSet();
        ui->widget_grab_set->setEnabled(false);
		ui->pushButton_OK->hide();
		ui->pushButton_Cancel->setText(QString::fromLocal8Bit("修改"));
        ui->label_log->setText("");
	}
}

//void ConfigView::setEnable( bool bEn )
//{
//	ui->dSpeed->setEnabled(bEn);
//	ui->Fstartpos->setEnabled(bEn);
//	ui->moveunit->setEnabled(bEn);
//	ui->dGetImageLen->setEnabled(bEn);
//	ui->plusewidgh->setEnabled(bEn);
//    ui->MoveLenth->setEnabled(bEn);
//    ui->Fivs->setEnabled(bEn);
//    ui->comboBox_camera->setEnabled(bEn);
//    ui->DulLines->setEnabled(bEn);
//}

void ConfigView::CalculateToUi()
{
	//时序
	int index = ui->comboBox_station_index->currentIndex();
	//只有8k，没有2k
	int iTmpiSpe   = getSequenceNumber(SString::IntToStr(index));

	ui->iSequence->setValue(iTmpiSpe);
	//频率
	double irtate_show = ui->dSpeed->value()/ui->moveunit->value();
	ui->rtate_show->setValue(irtate_show*iTmpiSpe/1000);

	//单次触发时长(ms):
	double dDutycyle = 1000* ui->moveunit->value()/ui->dSpeed->value();
	ui->dutycyle->setValue(dDutycyle/iTmpiSpe);


	//单张图像采集行数：
	double Fivscount = ui->dGetImageLen->value()/ui->moveunit->value();
	ui->Fivscount->setValue(Fivscount+0.5);

	//面阵相机
	if (1 == getStationType(index))
	{
		iTmpiSpe = getSequenceNumber(SString::IntToStr(index));
		char key[100] = { 0 };
		sprintf(key, "ImageStation_%d", index);
		IFileVariable* file = getStationCfgVariable();
		if (file != NULL) //面阵能通道个数使用配置文件指定
		{
			iTmpiSpe = file->getValue(key, ACQUISITION_MAX_CHANNEL_CNT).toInt();
		}
		if (iTmpiSpe>1)
		{
		  iTmpiSpe++;
		}
		ui->iSequence->setValue(iTmpiSpe);
		//频率
		irtate_show = ui->dSpeed->value() / ui->moveunit->value();
		ui->rtate_show->setValue(irtate_show*iTmpiSpe / 1000);

		//单次触发时长(ms):
		dDutycyle = 1000 * ui->moveunit->value() / ui->dSpeed->value();
		ui->dutycyle->setValue(dDutycyle / iTmpiSpe);

		//单张图像采集行数：
		Fivscount = ui->dGetImageLen->value() / ui->moveunit->value();
		ui->Fivscount->setValue(Fivscount + 0.5);
	}


	//等间距总触发次数:
	ui->targetNumber->setValue(Fivscount*iTmpiSpe+0.5);
	//视野宽度(mm):
    
	double dGetImageWid = ui->moveunit->value()* 1024*8;
	ui->dGetImageWid->setValue(dGetImageWid);

}

void ConfigView::showEvent( QShowEvent *event )
{
	updateCameraSet();
}

void ConfigView::hideEvent( QHideEvent *event )
{

}

void ConfigView::on_pushButton_Enable_OK_clicked()
{
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if (pFile)
    {
        pFile->setValue(IMAGE_PARAM_SAVE_SIMAGE_ENABLE,ui->checkBox_save_source_image->isChecked(),true);//使能保存原始图像
		pFile->setValue(IMAGE_PARAM_SAVE_DV_ER_SIMAGE_ENABLE, ui->checkBox_save_dv_error_source_image->isChecked(), true);//使能保存原始图像
		pFile->setValue(IMAGE_PARAM_STATIC_SAVE_ENABLE, ui->checkBox_save_static_image->isChecked(), true);//使能保存图像
        pFile->setValue(IMAGE_PARAM_DEFECT_DETECT_ENABLE,ui->checkBox_defect_detect->isChecked(),true);//使能H算法
        pFile->setValue(IMAGE_PARAM_H_SY_DETECT_ENABLE,ui->checkBox_h_sy_detect->isChecked(),true);//使能H_SY算法
        pFile->setValue(IMAGE_PARAM_H_ENABLEBUNITE, ui->checkBox_Bunite->isChecked(), true);

		pFile->setValue(IMAGE_PARAM_H_TIMEOUT_ENABLE, ui->checkBox_H_timeout->isChecked(), true);//使能H_SY算法超时
        pFile->setValue(IMAGE_PARAM_FAST_GRAB_ENABLE, ui->checkBox_enable_fast->isChecked(), true);//使能H算法

        pFile->setValue(IMAGE_PARAM_IS_WHOLE,ui->checkBox_EnbaleWhole->isChecked(),true);//使能保存处理后图像

        pFile->setValue(IMAGE_PARAM_SAVE_FILE_ENABLE,ui->checkBox_save_defectfile->isChecked(),true);//使能保存处理后图像
        pFile->setValue(IMAGE_PARAM_DEFECT_LEVEL_ENABLE,ui->checkBox_enable_level->isChecked(),true);//使能保存处理后图像


        pFile->setValue(IMAGE_PARAM_DEBUG_MODEL_ENABLE,ui->checkBox_Enbale_Debug->isChecked(),true);//是否使能空跑模式
        pFile->setValue(IMAGE_PARAM_DISABLE_CLEAN, ui->checkBox_debug_crl_clear->isChecked(), true);//
        pFile->setValue(IMAGE_PARAM_DISABLE_SHANG, ui->checkBox_debug_up_flow->isChecked(), true);//
        pFile->setValue(IMAGE_PARAM_DISABLE_XIA, ui->checkBox_debug_down_flow->isChecked(), true);//
		pFile->setValue(IMAGE_PARAM_ENABLE_FRONT, ui->EnableFront->isChecked(), true);//
        pFile->setValue(IMAGE_PARAM_DISABLE_CLEAR_MODE, ui->checkBox_debug_clear_mode->isChecked(), true);//
		
		pFile->setValue(IMAGE_PARAM_DEBUG, ui->checkBox_debug_mode->isChecked(), true);//

		pFile->setValue(IMAGE_PARAM_ENABLE_H_LEVEL, ui->checkBox_h_level->isChecked(), true);//

       // pFile->setValue(IMAGE_PARAM_MEASURE_ENABLE, ui->checkBox_measrue->isChecked(), true);//
pFile->setValue(AUTO_DELETE_IMAGE_ENABLE, ui->checkBox_autoDeleteImage->isChecked(), true);//
		pFile->setValue(AUTO_DELETE_DATA_ENABLE, ui->checkBox_autoDeleteData->isChecked(), true);//

		pFile->setValue(AUTO_DELETE_IMAGE_DAYS, ui->spinBox_daysDeleteImage->value(), true);//
		pFile->setValue(AUTO_DELETE_DATA_DAYS, ui->spinBox_daysDeleteData->value(), true);//
		pFile->setValue(IMAGE_PARAM_RIGHT_IMAGE_ENABLE, ui->checkBox_EnbaleStatic->isChecked(), true);//
		int resultMode = 0;
		if (ui->checkBox_randomClose->isChecked())
		{
			resultMode = 0;
		}
		else if (ui->checkBox_randomTwoBins->isChecked())
		{
			resultMode = 1;
		}
		else if (ui->checkBox_randomThreeBins->isChecked())
		{
			resultMode = 2;
		}
		else if (ui->checkBox_randomOneBin->isChecked())
		{
			resultMode = 3;
		}
		pFile->setValue(RANDOM_SEND_RESULT_MODE, resultMode, true);
		//checkBox_h_level
		int oneResultMode = ui->comboBox_oneResult->currentIndex();
		int twoResultMode = ui->comboBox_twoResult->currentIndex();

		pFile->setValue(RANDOM_ONE_RESULT_MODE, oneResultMode, true);
		pFile->setValue(RANDOM_TWO_RESULT_MODE, twoResultMode, true);

		bool b_timeoutFlag = ui->checkBox_H_timeout->isChecked();
        ui->pushButton_Enable_Cancel->setText(QString::fromLocal8Bit("修改"));
        ui->pushButton_Enable_OK->hide();
        ui->widget_enable_set->setEnabled(false);

		//writeDvTimeOutFlag(ui->checkBox_H_timeout->isChecked());
    }
}

void ConfigView::on_pushButton_Enable_Cancel_clicked()
{
    if (QString::fromLocal8Bit("修改") == ui->pushButton_Enable_Cancel->text())
    {
        ui->pushButton_Enable_OK->show();
        ui->widget_enable_set->setEnabled(true);
        ui->pushButton_Enable_Cancel->setText(QString::fromLocal8Bit("取消"));
    }
    else{
        updateCameraSet();
        ui->widget_enable_set->setEnabled(false);
        ui->pushButton_Enable_OK->hide();
        ui->pushButton_Enable_Cancel->setText(QString::fromLocal8Bit("修改"));
        ui->label_log->setText("");
    }
}
void ConfigView::on_pushButton_warn_Ok_clicked()
{
	IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
	if (NULL != pFileVariable)
	{
		pFileVariable->setValue(PRODUCT_WORK_ALARM_RI_ENABLE,ui->checkBox_RiMax_alarm->isChecked(), true);
		pFileVariable->setValue(PRODUCT_WORK_ALARM_RI_MAX, ui->spinBox_Ri_Max_alarm->value(), true);
		pFileVariable->setValue(PRODUCT_WORK_ALARM_NG_ENABLE, ui->checkBox_NgMax_alarm->isChecked(), true);
		pFileVariable->setValue(PRODUCT_WORK_ALARM_NG_MAX, ui->spinBox_Ng_Max_alarm->value(), true);
	}

	//
	ui->pushButton_warn_Cancel->setText(QString::fromLocal8Bit("修改"));
	ui->pushButton_warn_Ok->hide();
	ui->widget_alarm->setEnabled(false);
}
void ConfigView::on_pushButton_warn_Cancel_clicked()
{
	IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
	if (NULL != pFileVariable)
	{
		ui->checkBox_RiMax_alarm->setChecked(pFileVariable->getValue(PRODUCT_WORK_ALARM_RI_ENABLE).toBool());
		ui->spinBox_Ri_Max_alarm->setValue(pFileVariable->getValue(PRODUCT_WORK_ALARM_RI_MAX).toInt());
		ui->checkBox_NgMax_alarm->setChecked(pFileVariable->getValue(PRODUCT_WORK_ALARM_NG_ENABLE).toBool());
		ui->spinBox_Ng_Max_alarm->setValue(pFileVariable->getValue(PRODUCT_WORK_ALARM_NG_MAX).toInt());
	}
	if (QString::fromLocal8Bit("修改") == ui->pushButton_warn_Cancel->text())
	{
		ui->pushButton_warn_Ok->show();
		ui->widget_alarm->setEnabled(true);
		ui->pushButton_warn_Cancel->setText(QString::fromLocal8Bit("取消"));
	}
	else {

		ui->widget_alarm->setEnabled(false);
		ui->pushButton_warn_Ok->hide();
		ui->pushButton_warn_Cancel->setText(QString::fromLocal8Bit("修改"));

	}

}

void ConfigView::on_pushButton_clear_time_clicked()
{
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile)
	{
		int clear = ui->spinBox_clear_time->text().toInt();
		pFile->setValue(CAMERAPARAM_CLAR_TIME, clear, true);
	}
}

void ConfigView::on_pushButton_save_browse_clicked()
{
    QString strDefault = "./";
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if (NULL != pFile)
    {
        strDefault = QString::fromLocal8Bit(pFile->getValue(IMAGE_SAVE_PATH).toCString().c_str());
    }
    QString fileName = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("请选择图片保存路径..."), strDefault);
    if (!fileName.isNull())
    {
        ui->lineEdit_save_path->setText(fileName);
    }

}

void ConfigView::on_pushButton_read_browse_clicked()
{
    QString strDefault = "./";
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if (NULL != pFile)
    {
        strDefault = QString::fromLocal8Bit(pFile->getValue(IMAGE_OFFLINE_PATH).toCString().c_str());
    }
    QString fileName = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("请选择离线图片路径..."), strDefault);
    if (!fileName.isNull())
    {
        ui->lineEdit_offline_path->setText(fileName);
		if (m_snList.size() != 0)
		{
			m_snList.clear();
		}
		if (m_snSet.size() != 0)
		{
			m_snSet.clear();
		}
		if (m_NameListMap.size() != 0)
		{
			m_NameListMap.clear();
		}
		parseFileInfoList(GetFileList(ui->lineEdit_offline_path->text()));
        //GetChannelFileList(fileName);
    }
}

void ConfigView::on_pushButton_Image_OK_clicked()
{
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if (pFile)
    {
        pFile->setValue(IMAGE_SAVE_PATH, ui->lineEdit_save_path->text().toLocal8Bit().data(), true);
        pFile->setValue(IMAGE_OFFLINE_PATH, ui->lineEdit_offline_path->text().toLocal8Bit().data(), true);
        pFile->setValue(IMAGE_GROUP_NAME, getSeqNameWithID(0), ui->lineEdit_channel1_name->text().toLocal8Bit().data(), true);
        pFile->setValue(IMAGE_GROUP_NAME, getSeqNameWithID(1), ui->lineEdit_channel2_name->text().toLocal8Bit().data(), true);
        pFile->setValue(IMAGE_GROUP_NAME, getSeqNameWithID(2), ui->lineEdit_channel3_name->text().toLocal8Bit().data(), true);
        pFile->setValue(IMAGE_GROUP_NAME, getSeqNameWithID(3), ui->lineEdit_channel4_name->text().toLocal8Bit().data(), true);
        pFile->setValue(IMAGE_GROUP_NAME, getSeqNameWithID(4), ui->lineEdit_channel5_name->text().toLocal8Bit().data(), true);
        pFile->setValue(IMAGE_GROUP_NAME, getSeqNameWithID(5), ui->lineEdit_channel6_name->text().toLocal8Bit().data(), true);
        pFile->setValue(IMAGE_GROUP_NAME, getSeqNameWithID(6), ui->lineEdit_channel7_name->text().toLocal8Bit().data(), true);
        pFile->setValue(IMAGE_GROUP_NAME, getSeqNameWithID(7), ui->lineEdit_channel8_name->text().toLocal8Bit().data(), true);
        pFile->setValue(IMAGE_SAVE_NAME_SUFFIX, ui->lineEdit_saveimage_suffix->text().toLocal8Bit().data(), true);
		pFile->setValue(IMAGE_HISTORY_DATA_PATH, ui->lineEdit_history_data_path->text().toLocal8Bit().data(), true);
		pFile->setValue(IMAGE_HISTORY_DATA_ENABLE, ui->checkBox_use_history_data->isChecked(), true);
		pFile->setValue(WORK_PRODUCT_SN, ui->lineEdit_image_sn->text().toLocal8Bit().data(), true);
    }
    ui->widget_image_set->setEnabled(false);
    ui->pushButton_Image_OK->hide();
    ui->pushButton_Image_Cancel->setText(QString::fromLocal8Bit("修改"));
}

void ConfigView::on_pushButton_Image_Cancel_clicked()
{
    if (QString::fromLocal8Bit("修改") == ui->pushButton_Image_Cancel->text())
    {
        ui->pushButton_Image_OK->show();
        ui->widget_image_set->setEnabled(true);
        ui->pushButton_Image_Cancel->setText(QString::fromLocal8Bit("取消"));
    }
    else {
        IFileVariable *pFile = getCameraSetCfgFileVariable();
        if (pFile)
        {
            ui->lineEdit_save_path->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_SAVE_PATH).toCString().c_str()));
            ui->lineEdit_offline_path->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_OFFLINE_PATH).toCString().c_str()));

            ui->lineEdit_channel1_name->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_GROUP_NAME, getSeqNameWithID(0), getSeqNameWithID(0)).toCString().c_str()));
            ui->lineEdit_channel2_name->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_GROUP_NAME, getSeqNameWithID(1), getSeqNameWithID(1)).toCString().c_str()));
            ui->lineEdit_channel3_name->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_GROUP_NAME, getSeqNameWithID(2), getSeqNameWithID(2)).toCString().c_str()));
            ui->lineEdit_channel4_name->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_GROUP_NAME, getSeqNameWithID(3), getSeqNameWithID(3)).toCString().c_str()));
            ui->lineEdit_channel5_name->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_GROUP_NAME, getSeqNameWithID(4), getSeqNameWithID(4)).toCString().c_str()));
            ui->lineEdit_channel6_name->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_GROUP_NAME, getSeqNameWithID(5), getSeqNameWithID(5)).toCString().c_str()));
            ui->lineEdit_channel7_name->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_GROUP_NAME, getSeqNameWithID(6), getSeqNameWithID(6)).toCString().c_str()));
            ui->lineEdit_channel8_name->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_GROUP_NAME, getSeqNameWithID(7), getSeqNameWithID(7)).toCString().c_str()));
            ui->lineEdit_saveimage_suffix->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_SAVE_NAME_SUFFIX).toCString().c_str()));
			ui->lineEdit_history_data_path->setText(QString::fromLocal8Bit(pFile->getValue(IMAGE_HISTORY_DATA_PATH).toCString().c_str()));
			ui->checkBox_use_history_data->setChecked(pFile->getValue(IMAGE_HISTORY_DATA_ENABLE).toBool());
			ui->lineEdit_image_sn->setText(QString::number(pFile->getValue(WORK_PRODUCT_SN).toULong()));


        }
        ui->widget_image_set->setEnabled(false);
        ui->pushButton_Image_OK->hide();
        ui->pushButton_Image_Cancel->setText(QString::fromLocal8Bit("修改"));
    }
}

void ConfigView::on_pushButton_product_new_clicked()
{
    INotifyService *pINotifyService = CommonUtils::getNotifyService();
    if (pINotifyService == NULL)
    {
        QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("切换产品插件加载失败!"));
        ui->label_product_name->setText("Error!");
        return;
    }
    CommonUtils::getIUIPlugin("fileuiplugin");
    pINotifyService->send("fileuiplugin", "file_new", 0, 0);
    ui->label_product_name->setText(QString::fromStdString(getCurrentProgramName()));
}

void ConfigView::on_pushButton_product_open_clicked()
{
    INotifyService *pINotifyService = CommonUtils::getNotifyService();
    if (pINotifyService == NULL)
    {
        QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("切换产品插件加载失败!"));
        ui->label_product_name->setText("Error!");
        return;
    }
    CommonUtils::getIUIPlugin("fileuiplugin");
    pINotifyService->send("fileuiplugin", "file_open", 0, 0);
    ui->label_product_name->setText(QString::fromStdString(getCurrentProgramName()));
}

void ConfigView::on_pushButton_product_save_clicked()
{
    INotifyService *pINotifyService = CommonUtils::getNotifyService();
    if (pINotifyService == NULL)
    {
        QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("切换产品插件加载失败!"));
        ui->label_product_name->setText("Error!");
        return;
    }
    CommonUtils::getIUIPlugin("fileuiplugin");
    pINotifyService->send("fileuiplugin", "file_save", 0, 0);
    ui->label_product_name->setText(QString::fromStdString(getCurrentProgramName()));
}

void ConfigView::on_pushButton_product_modify_clicked()
{
    INotifyService *pINotifyService = CommonUtils::getNotifyService();
    if (pINotifyService == NULL)
    {
        QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("切换产品插件加载失败!"));
        ui->label_product_name->setText("Error!");
        return;
    }
    CommonUtils::getIUIPlugin("fileuiplugin");
    pINotifyService->send("fileuiplugin", "file_modify", 0, 0);
    ui->label_product_name->setText(QString::fromStdString(getCurrentProgramName()));
}

QString ConfigView::GetFileName()
{
    QString fileName = "";
    QString strDefault = "./";
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if (NULL != pFile)
    {
        strDefault = QString::fromLocal8Bit(pFile->getValue(IMAGE_OFFLINE_PATH).toCString().c_str());
    }
    QString filePath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择图片..."), strDefault);
    if (!fileName.isNull())
    {
        QFileInfo fileinfo;
        fileinfo = QFileInfo(filePath);
        //文件名
        fileName = fileinfo.fileName();
    }
    return fileName;
}

string ConfigView::getSelectFlowName(int index)
{
    QDialog dlg; 
    QVBoxLayout *vBoxLayout = new QVBoxLayout(&dlg);
    QCheckBox *pCheckBox = NULL;
    vector<string> nameList;
    IMainflowMgr *pMainFlowMgr = (IMainflowMgr *)GetSDKInstance("mainflow", IMainflowMgr::IDD_MAIN_FLOW_MGR);
    IMainFlow *pMainFlow = pMainFlowMgr->findMainflow("work");
    RETURN_V_IF(NULL == pMainFlow, NULL);
    // 流程配置接口
    iMainFlowCfg *pMainFlowCfg = pMainFlow->GetMainFlowCfg();
    RETURN_V_IF(NULL == pMainFlowCfg, NULL);

    string names;
    IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
    int iModel = 0;
    if (NULL != pFileVariable)
    {
        if (0 == index)
        {
			names = pFileVariable->getValue(PRODUCT_FLOW_NAMES_1, "").toCString();
        }
        else if(1 == index)
        {
			names = pFileVariable->getValue(PRODUCT_FLOW_NAMES_2, "").toCString();
        }
        else
        {
            names = pFileVariable->getValue(PRODUCT_FLOW_NAMES_3, "").toCString();
        }
    }
    vector<string> vecNames = SString::split(names, ",");

    vector<QCheckBox *> vecItemList;
    int iCount = pMainFlowCfg->GetWorkCount();
    int i = 0;
    for (; i < iCount; i++)
    {
        WORK_FLOW *pFlow = pMainFlowCfg->GetWorkCfg(i);
        if (NULL != pFlow)
        {
            pCheckBox = new QCheckBox(QString::fromUtf8(pFlow->description));
            pCheckBox->setToolTip(pFlow->name);
            vBoxLayout->addWidget(pCheckBox, i);
            vecItemList.push_back(pCheckBox);
            for (int j = 0; j < vecNames.size(); j++)
            {
                if (vecNames.at(j) == pFlow->name)
                {
                    pCheckBox->setChecked(true);
                }
            }
        }
    }
    QLabel *pLabel = new QLabel(QString::fromLocal8Bit("选择完毕后点击关闭按钮保存设置"));
    vBoxLayout->addWidget(pLabel, i++);
    QPushButton *pBt = new QPushButton(QString::fromLocal8Bit("OK"));
    vBoxLayout->addWidget(pBt, i);
    connect(pBt, SIGNAL(pressed()), &dlg, SLOT(accept()));
    if (0 == dlg.exec())
    {
        return "";
    }
    QString strSelectNames = "";
    for (int j = 0; j < vecItemList.size(); j++)
    {
        if (vecItemList.at(j)->isChecked())
        {
            if (strSelectNames.isEmpty())
            {
                strSelectNames = vecItemList.at(j)->toolTip();
            }
            else
            {
                strSelectNames.append(",").append(vecItemList.at(j)->toolTip());
            }
        }
    }
    return strSelectNames.toStdString();
}

void ConfigView::on_pushButton_name1_browse_clicked()
{
    QString fileName = GetFileName();
    if (fileName.isNull())
    {
        ui->lineEdit_channel1_name->setText(fileName);
    }
}

void ConfigView::on_pushButton_name2_browse_clicked()
{
    QString fileName = GetFileName();
    if (fileName.isNull())
    {
        ui->lineEdit_channel2_name->setText(fileName);
    }
}

void ConfigView::on_pushButton_name3_browse_clicked()
{
    QString fileName = GetFileName();
    if (fileName.isNull())
    {
        ui->lineEdit_channel3_name->setText(fileName);
    }
}

void ConfigView::on_pushButton_name4_browse_clicked()
{
    QString fileName = GetFileName();
    if (fileName.length())
    {
        ui->lineEdit_channel4_name->setText(fileName);
    }
}

void ConfigView::on_pushButton_name5_browse_clicked()
{
    QString fileName = GetFileName();
    if (fileName.length())
    {
        ui->lineEdit_channel5_name->setText(fileName);
    }
}

void ConfigView::on_pushButton_name6_browse_clicked()
{
    QString fileName = GetFileName();
    if (fileName.length())
    {
        ui->lineEdit_channel6_name->setText(fileName);
    }
}

void ConfigView::on_pushButton_name7_browse_clicked()
{
    QString fileName = GetFileName();
    if (fileName.length())
    {
        ui->lineEdit_channel7_name->setText(fileName);
    }
}

void ConfigView::on_pushButton_name8_browse_clicked()
{
    QString fileName = GetFileName();
    if (fileName.length())
    {
        ui->lineEdit_channel8_name->setText(fileName);
    }
}

void ConfigView::on_pushButton_image_sn_browse_clicked()
{
	QString fileName = GetFileName();
	if (fileName.length() == 0)
	{
		ui->lineEdit_image_sn->setText("");
		return;
	}
	if (ui->lineEdit_offline_path->text().length() ==0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("离线文件"), QString::fromLocal8Bit("离线文件路径为空"));
		return;
	}

	GetChannelFileList(ui->lineEdit_offline_path->text());
	//procuct(105) - SEQUENCE_A - rawImge.bmp
	QString strSN;
	QString strChannel;
	int ileft = fileName.indexOf("(");
	int iright= fileName.indexOf(")");
	if (ileft >0 && iright >ileft)
	{
		strSN = fileName.mid(ileft+1, iright - ileft-1);
	}
	ui->lineEdit_image_sn->setText(strSN);
	m_icurrentSN = strSN.toInt();
	ileft = fileName.indexOf("-");
	iright = fileName.lastIndexOf("-");
	strChannel = fileName.mid(ileft + 1, iright - ileft-1);

	for (int i = 0; i < SEQUE_NUMBER; i++)
	{
		if (getSeqNameWithID(i) != strChannel.toStdString())
		{
			continue;
		}
		QFileInfoList file_list = m_fileList[i];
		for (int j = 0; j < file_list.size();j++)
		{
			if (fileName == file_list.at(j).fileName())
			{
				m_currentImageFileIndex = j;
				break;
			}
		}
	}
	UpdateOfflineFile();
	if (ui->lineEdit_channel1_name->text().length() >0)
	{
		QString fileName = ui->lineEdit_channel1_name->text();
		QString strSN;
		int ileft = fileName.indexOf("(");
		int iright = fileName.indexOf(")");
		if (ileft > 0 && iright > ileft)
		{
			strSN = fileName.mid(ileft + 1, iright - ileft - 1);
		}
		ui->lineEdit_image_sn->setText(strSN);
	}
}

void ConfigView::on_pushButton_history_data_browse_clicked()
{
	QString strDefault = "./";
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (NULL != pFile)
	{
		strDefault = QString::fromLocal8Bit(pFile->getValue(IMAGE_HISTORY_DATA_PATH).toCString().c_str());
	}
	QString fileName = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("请选择离线数据路径..."), strDefault);
	if (!fileName.isNull())
	{
		ui->lineEdit_history_data_path->setText(fileName);
	}
}

void ConfigView::on_pushButton_forward_clicked()
{
	offlineChangeProduct("forward");
}

void ConfigView::on_pushButton_next_clicked()
{
	offlineChangeProduct("next");
}

void ConfigView::on_pushButton_do_clicked()
{
    ui->pushButton_do->setEnabled(false);
    int index  = ui->comboBox_image_type->currentIndex();
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if(pFile != NULL)
    {
        pFile->setValue(IMAGE_OFFLINE_IMAGETYPE,index,true);
    }

    INotifyService* pNotifyService = CommonUtils::getNotifyService();
    if (NULL != pNotifyService)
    {
        pNotifyService->send("mainui", "offline_test", 0, 0);
    }
    //if (ui->checkBox_auto->isChecked())
    //{
    //    on_pushButton_next_clicked();
    //}
    ui->pushButton_do->setEnabled(true);
}
void ConfigView::on_pushButton_do_aoto_clicked()
{
	//获取当前图像数-确认有图像与为整数图像则开始
	if (m_ImageFilesCount <= 0)
	{
		GetChannelFileList(ui->lineEdit_offline_path->text());
	}
	//
	QFileInfoList file_list;
	file_list = GetFileList(ui->lineEdit_offline_path->text());
	int iImageSize = file_list.size();
	if (iImageSize == 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("离线文件"), QString::fromLocal8Bit("当前目录下无离线图像"), QMessageBox::Ok);
		return ;
	}



	//m_currentImageFileIndex++;
	if (m_currentImageFileIndex >= m_ImageFilesCount)
	{
		m_currentImageFileIndex = 0;
	}
	UpdateOfflineFile();
	//
	int iStationCout = getImageStationCnt();
	int iProducsImage = 0;
	int iProducsMax = 1;
	vector<string> vect;
	for (int i = 0; i < iStationCout;i++)
	{
		vect.clear();
		getEnabelChannle(vect, SString::IntToStr(i));
		iProducsImage += vect.size();
	}
	if (iProducsImage>0)
	{
		iProducsMax = iImageSize / iProducsImage;
	}
	//
	QString str = QString::fromLocal8Bit("开始自动连续测试产品 ") + QString::number(iProducsMax) + QString::fromLocal8Bit(" 个");

	QMessageBox::warning(this, QString::fromLocal8Bit("自动连续测试"), str, QMessageBox::Ok);
	INotifyService* pNotifyService = CommonUtils::getNotifyService();

	if (NULL != pNotifyService)
	{
		pNotifyService->send("mainui", "offline_aoto_test", (OS_LPARAM)&iProducsMax, 0);//自动离线连续测试
	}
}
void ConfigView::on_pushButton_do_aoto_2_clicked()
{
	//获取当前图像数-确认有图像与为整数图像则开始
	if (m_ImageFilesCount <= 0)
	{
		GetChannelFileList(ui->lineEdit_offline_path->text());
	}
	//
	QFileInfoList file_list;
	file_list = GetFileList(ui->lineEdit_offline_path->text());
	int iImageSize = file_list.size();
	if (iImageSize == 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("离线文件"), QString::fromLocal8Bit("当前目录下无离线图像"), QMessageBox::Ok);
		return;
	}



	//m_currentImageFileIndex++;
	if (m_currentImageFileIndex >= m_ImageFilesCount)
	{
		m_currentImageFileIndex = 0;
	}
	UpdateOfflineFile();
	//
	int iStationCout = getImageStationCnt();
	int iProducsImage = 0;
	int iProducsMax = 1;
	vector<string> vect;
	for (int i = 0; i < iStationCout; i++)
	{
		vect.clear();
		getEnabelChannle(vect, SString::IntToStr(i));
		iProducsImage += vect.size();
	}
	if (iProducsImage > 0)
	{
		iProducsMax = iImageSize / iProducsImage;
		iProducsMax = iProducsMax * 10000;
	}
	//
	QString str = QString::fromLocal8Bit("开始压力测试产品 ") + QString::number(iProducsMax) + QString::fromLocal8Bit(" 个");

	QMessageBox::warning(this, QString::fromLocal8Bit("压力测试"), str, QMessageBox::Ok);
	INotifyService* pNotifyService = CommonUtils::getNotifyService();

	if (NULL != pNotifyService)
	{
		pNotifyService->send("mainui", "offline_aoto_test", (OS_LPARAM)&iProducsMax, 0);//自动离线连续测试
	}
}
void ConfigView::on_pushButton_refresh_clicked()
{
	if (m_snList.size() <= 0)
	{
		parseFileInfoList(GetFileList(ui->lineEdit_offline_path->text()));
	}
	if (m_snList.size() <= 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("离线文件"), QString::fromLocal8Bit("当前目录下无离线文件"), QMessageBox::Ok);
		return;
	}
	if (ui->lineEdit_image_sn->text() == "")
	{
		return;
	}
	int sn = ui->lineEdit_image_sn->text().toInt();
	if (!m_snList.contains(sn))
	{
		return;
	}
	QList<int>::iterator it = m_snList.begin();
	int index = 0;
	while (it != m_snList.end())
	{
		if (sn == *it)
		{
			break;
		}
		index++;
		it++;
	}
	m_currentImageFileIndex = index;
	m_fileNameList.clear();
	for (int i = 0; i < m_NameListMap[m_snList.at(m_currentImageFileIndex)].size(); ++i)
	{
		m_fileNameList.push_back(m_NameListMap[m_snList.at(m_currentImageFileIndex)].at(i));
	}
	UpdateOfflineFile();

}
bool ConfigView::GetChannelFileList(QString path)
{
    if (path.isEmpty())
        return false;

    for (int i = 0; i < SEQUE_NUMBER; i++)
    {
        m_fileList[i].clear();
    }
    QFileInfoList file_list;
    file_list = GetFileList(path);
    QString strChannelNames = getCurrentProgramRecipesList().c_str();
    QStringList ChannelNameList = strChannelNames.split(",");
    m_ImageFilesCount = 0;
	/*
   for (int k = 0; k < SEQUE_NUMBER; k++)
   {
       for (int j = 0; j < file_list.size(); )
       {
           QFileInfo fileInfo = file_list[j];
	
           if (fileInfo.fileName().contains(ChannelNameList[k]))
           {
               m_fileList[k].push_back(fileInfo);
               file_list.removeAt(j);
               if (k == 0)
               {
                   ++m_ImageFilesCount;
               }
           }
           else
           {
               ++j;
           }
       }
   }*/
	//只识别 4 个位数的图片 比哪如。个位数与十位数与百位数与千位数万位数 中的任4个
	int iLengFirst  = 0;//
	int iLengSecond = 0;
	int iLengThree  = 0;
	int iLengFour   = 0;
	int iFiles = 0;
	QList<QFileInfo> tmpList1;//
	QList<QFileInfo> tmpList2;
	QList<QFileInfo> tmpList3;
	QList<QFileInfo> tmpList4;
	QList<QString>  strList_1;

	
	QList<QFileInfo> tmp_fileList;
	for (int j = 0; j < file_list.size(); j++)
	{
		QFileInfo fileInfo = file_list[j];
		QString str = fileInfo.fileName();
		if (!str.contains(".bmp"))
		{
			continue;
		}
		iFiles++;
		int ileng = str.length();
		if (j == 0)
		{
			iLengFirst = ileng;
		}

		if (iLengFirst == ileng)
		{
			tmpList1.push_back(fileInfo);
			//file_list.removeAt(j);
		
		}
		else {
			if (0 == iLengSecond)
			{
				iLengSecond = ileng;
			}
			if (iLengSecond == ileng)
			{
				tmpList2.push_back(fileInfo);
				//file_list.removeAt(j);
				
			}
			else {
				if (0== iLengThree)
				{
					iLengThree = ileng;
				}
				if (iLengThree == ileng)
				{
					tmpList3.push_back(fileInfo);
					
					
				}
				else {

					if (0 == iLengFour)
					{
						iLengFour = ileng;
					}
					if (iLengFour == ileng)
					{
						tmpList4.push_back(fileInfo);
					}
					else {
						//
						QMessageBox::warning(this, QString::fromLocal8Bit("离线文件"), QString::fromLocal8Bit("离线文件过多！"), QMessageBox::Ok);
						return false;
					}
				}
			}
		}
	}
	int iVSort[4] = { 0,0,0,0 };
	//排序到一个qlist
	// 1 
	if (iLengFirst > 0 && iLengSecond == 0 && iLengThree == 0 && iLengFour == 0)
	{
		iVSort[0] = 1;
		ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);

	}
	// 2个组合
	else if (iLengFirst > 0 && iLengSecond > 0 && iLengThree == 0 && iLengFour == 0)
	{
		if (iLengFirst < iLengSecond) //1在前面 1<2
		{
			iVSort[0] = 1;
			iVSort[1] = 2;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		else { //1 在后面   //2<1

			iVSort[0] = 2;
			iVSort[1] = 1;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
	}
	// 3个组合
	else if (iLengFirst > 0 && iLengSecond > 0 && iLengThree > 0 && iLengFour == 0)
	{
		if (iLengFirst < iLengSecond && iLengSecond  < iLengThree)// 1 < 2  < 3 
		{
			iVSort[0] = 1;
			iVSort[1] = 2;
			iVSort[2] = 3;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		else if(iLengFirst < iLengThree && iLengThree < iLengSecond)//1 < 3 < 2
		{
			iVSort[0] = 1;
			iVSort[1] = 3;
			iVSort[2] = 2;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		else if (iLengSecond < iLengFirst  && iLengFirst  < iLengThree )//2 < 1 < 3
		{
			iVSort[0] = 2;
			iVSort[1] = 1;
			iVSort[2] = 3;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		else if (iLengSecond  < iLengThree && iLengThree < iLengFirst)//2 < 3 < 1
		{
			iVSort[0] = 2;
			iVSort[1] = 3;
			iVSort[2] = 1;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);

		}
		else if (iLengThree  < iLengSecond && iLengSecond < iLengFirst)//3 < 2 < 1
		{

			iVSort[0] = 3;
			iVSort[1] = 2;
			iVSort[2] = 1;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);

		}
		else if (iLengThree  < iLengFirst && iLengFirst < iLengSecond)//3 < 1< 2
		{

			iVSort[0] = 3;
			iVSort[1] = 1;
			iVSort[2] = 2;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);

		}

	}
	// 4个组合
	else {

		//1<2<3<4
		if (iLengFirst < iLengSecond && iLengSecond < iLengThree && iLengThree < iLengFour )
		{
			iVSort[0] = 1;
			iVSort[1] = 2;
			iVSort[2] = 3;
			iVSort[3] = 4;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//1<2<4<3
		else if (iLengFirst < iLengSecond && iLengSecond < iLengFour && iLengFour < iLengThree)
		{
			iVSort[0] = 1;
			iVSort[1] = 2;
			iVSort[2] = 4;
			iVSort[3] = 3;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//1<3<2<4
		else if (iLengFirst < iLengThree  && iLengThree < iLengSecond && iLengSecond < iLengFour)
		{
			iVSort[0] = 1;
			iVSort[1] = 3;
			iVSort[2] = 2;
			iVSort[3] = 4;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//1<3<4<3
		else if (iLengFirst < iLengThree  && iLengThree < iLengFour &&  iLengFour< iLengSecond)
		{
			iVSort[0] = 1;
			iVSort[1] = 3;
			iVSort[2] = 4;
			iVSort[3] = 2;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//1<4<3<2
		else if (iLengFirst < iLengFour && iLengFour < iLengThree && iLengThree< iLengSecond)
		{
			iVSort[0] = 1;
			iVSort[1] = 4;
			iVSort[2] = 3;
			iVSort[3] = 2;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//1<4<2<3
		else if (iLengFirst < iLengFour && iLengFour < iLengSecond && iLengSecond< iLengThree)
		{
			iVSort[0] = 1;
			iVSort[1] = 4;
			iVSort[2] = 2;
			iVSort[3] = 3;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//2<1<3<4
		else if (iLengSecond  <iLengFirst && iLengFirst < iLengThree && iLengThree < iLengFour)
		{
			iVSort[0] = 2;
			iVSort[1] = 1;
			iVSort[2] = 3;
			iVSort[3] = 4;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//2<1<4<3
		else if (iLengSecond  <iLengFirst && iLengFirst < iLengFour && iLengFour  < iLengThree)
		{
			iVSort[0] = 2;
			iVSort[1] = 1;
			iVSort[2] = 4;
			iVSort[3] = 3;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//2<3<1<4
		else if (iLengSecond  <iLengThree && iLengThree < iLengFirst && iLengFirst  < iLengFour)
		{
			iVSort[0] = 2;
			iVSort[1] = 3;
			iVSort[2] = 1;
			iVSort[3] = 4;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//2<3<4<1
		else if (iLengSecond  <iLengThree && iLengThree < iLengFour &&   iLengFour< iLengFirst)
		{
			iVSort[0] = 2;
			iVSort[1] = 3;
			iVSort[2] = 4;
			iVSort[3] = 1;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//2<4<1<3
		else if (iLengSecond  <iLengFour && iLengFour < iLengFirst &&   iLengFirst< iLengThree)
		{
			iVSort[0] = 2;
			iVSort[1] = 4;
			iVSort[2] = 1;
			iVSort[3] = 3;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//2<4<3<1
		else if (iLengSecond  <iLengFour && iLengFour < iLengThree &&   iLengThree< iLengFirst)
		{
			iVSort[0] = 2;
			iVSort[1] = 4;
			iVSort[2] = 3;
			iVSort[3] = 1;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//3<1<2<4
		else if (iLengThree  <iLengFirst && iLengFirst < iLengSecond &&   iLengSecond< iLengFour)
		{
			iVSort[0] = 3;
			iVSort[1] = 1;
			iVSort[2] = 2;
			iVSort[3] = 4;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//3<1<4<2
		else if (iLengThree  <iLengFirst && iLengFirst < iLengFour &&   iLengFour< iLengSecond)
		{
			iVSort[0] = 3;
			iVSort[1] = 1;
			iVSort[2] = 4;
			iVSort[3] = 2;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//3<2<1<4
		else if (iLengThree  <iLengSecond && iLengSecond < iLengFirst &&   iLengFirst< iLengFour)
		{
			iVSort[0] = 3;
			iVSort[1] = 2;
			iVSort[2] = 1;
			iVSort[3] = 4;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//3<2<4<1
		else if (iLengThree  <iLengSecond && iLengSecond < iLengFour &&   iLengFour< iLengFirst)
		{
			iVSort[0] = 3;
			iVSort[1] = 2;
			iVSort[2] = 4;
			iVSort[3] = 1;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//3<4<1<2
		else if (iLengThree  <iLengFour && iLengFour < iLengFirst &&   iLengFirst< iLengSecond)
		{
			iVSort[0] = 3;
			iVSort[1] = 4;
			iVSort[2] = 1;
			iVSort[3] = 2;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//3<4<2<1
		else if (iLengThree  <iLengFour && iLengFour < iLengSecond &&   iLengSecond< iLengFirst)
		{
			iVSort[0] = 3;
			iVSort[1] = 4;
			iVSort[2] = 2;
			iVSort[3] = 1;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//4<1<2<3
		else if (iLengFour  <iLengFirst && iLengFirst < iLengSecond &&   iLengSecond< iLengThree)
		{
			iVSort[0] = 4;
			iVSort[1] = 1;
			iVSort[2] = 2;
			iVSort[3] = 3;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//4<1<3<2
		else if (iLengFour  <iLengFirst && iLengFirst < iLengThree &&   iLengThree< iLengSecond)
		{
			iVSort[0] = 4;
			iVSort[1] = 1;
			iVSort[2] = 3;
			iVSort[3] = 2;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//4<2<1<3
		else if (iLengFour  <iLengSecond && iLengSecond < iLengFirst &&   iLengFirst< iLengThree)
		{
			iVSort[0] = 4;
			iVSort[1] = 2;
			iVSort[2] = 1;
			iVSort[3] = 3;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//4<2<3<1
		else if (iLengFour  <iLengSecond && iLengSecond < iLengThree &&   iLengThree< iLengFirst)
		{
			iVSort[0] = 4;
			iVSort[1] = 2;
			iVSort[2] = 3;
			iVSort[3] = 1;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//4<3<1<2
		else if (iLengFour  <iLengThree && iLengThree < iLengFirst &&   iLengFirst< iLengSecond)
		{
			iVSort[0] = 4;
			iVSort[1] = 3;
			iVSort[2] = 1;
			iVSort[3] = 2;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}
		//4<3<2<1
		else if (iLengFour  <iLengThree && iLengThree < iLengSecond &&  iLengSecond < iLengFirst)
		{
			iVSort[0] = 4;
			iVSort[1] = 3;
			iVSort[2] = 2;
			iVSort[3] = 1;
			ChannelImageSort(iVSort, 4, tmpList1, tmpList2, tmpList3, tmpList4, tmp_fileList);
		}

	}
	// 排序好赋值
	
	for (int k = 0; k < SEQUE_NUMBER; k++)
	{
		strList_1.clear();
	    for (int j = 0; j < iFiles;j++)
	    {
	       QFileInfo fileInfo = tmp_fileList[j];
		   QString strName = fileInfo.fileName();
		   strList_1.push_back(strName);
	       if (strName.contains(ChannelNameList[k]))
	       {
	           m_fileList[k].push_back(fileInfo);
			   if (k == 0)
			   {
				   m_ImageFilesCount++;
			   }
			  
	       }
	      
	    }
	}
	tmpList1.clear();//
	tmpList2.clear();
	tmpList3.clear();
	tmpList4.clear();
	tmp_fileList.clear();
    return true;
}

bool ConfigView::ChannelImageSort(int iVSort[4],int iVsize, QList<QFileInfo> tmpList1, QList<QFileInfo> tmpList2, QList<QFileInfo> tmpList3, QList<QFileInfo> tmpList4, QList<QFileInfo> &tmpList )
{
	for (int i = 0; i < iVsize; i++)
	{
		int isw = iVSort[i];
		switch (isw)
		{
		case 1:
			for (int k = 0; k < tmpList1.size(); k++)
			{
				tmpList.push_back(tmpList1.at(k));
			}
			break;
		case 2:
			for (int k = 0; k < tmpList2.size(); k++)
			{
				tmpList.push_back(tmpList2.at(k));
			}
			break;
		case 3:
			for (int k = 0; k < tmpList3.size(); k++)
			{
				tmpList.push_back(tmpList3.at(k));
			}
			break;
		case 4:
			for (int k = 0; k < tmpList4.size(); k++)
			{
				tmpList.push_back(tmpList4.at(k));
			}
			break;
		default:
			break;
		}
	}
	
	return true;;

}


void ConfigView::UpdateOfflineFile()
{
    QLineEdit *editList[SEQUE_NUMBER] = { ui->lineEdit_channel1_name,ui->lineEdit_channel2_name,ui->lineEdit_channel3_name,ui->lineEdit_channel4_name,
        ui->lineEdit_channel5_name,ui->lineEdit_channel6_name,ui->lineEdit_channel7_name,ui->lineEdit_channel8_name };
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    for (int i = 0; i < m_fileNameList.size(); i++)
    {
        QString fileName = m_fileNameList[i];
		editList[i]->setText(fileName);
		if (pFile)
		{
			pFile->setValue(IMAGE_GROUP_NAME, getSeqNameWithID(i), editList[i]->text().toLocal8Bit().data());
		}
    }
}


void ConfigView::on_pushButton_browse_h_config_file_clicked()
{
    
	QString strDefault = ui->lineEdit_hconfig_path->text();
	QString fileName = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("请选择算法产品路径..."), strDefault);
	if (!fileName.isNull())
	{
		ui->lineEdit_hconfig_path->setText(fileName);
	}
}

void ConfigView::on_pushButton_browse_h_log_path_clicked()
{
    QString fileName = "";
    QString strDefault = ui->lineEdit_h_log_path->text();
    QString filePath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择H算法日志保存路径..."), strDefault);
    if (!fileName.isNull())
    {
        QFileInfo fileinfo;
        fileinfo = QFileInfo(filePath);
        //文件名
        fileName = fileinfo.fileName();
        ui->lineEdit_h_log_path->setText(fileName);
    }
}

void ConfigView::on_pushButton_H_alg_OK_clicked()
{
    IFileVariable *pFile = getDVCfgFileVariable();
    if (NULL != pFile)
    {
        int iFlags = 0;
        if (ui->checkBox_debug->isChecked())
        {
            iFlags += 0x1;
        }
        if (ui->checkBox_info->isChecked())
        {
            iFlags += 0x2;
        }
        if (ui->checkBox_warning->isChecked())
        {
            iFlags += 0x4;
        }
        if (ui->checkBox_error->isChecked())
        {
            iFlags += 0x8;
        }

       pFile->setValue(DV_CONFIG_FILE, ui->lineEdit_hconfig_path->text().toLocal8Bit().data(),true);
	   pFile->setValue(DV_H_LOG_PATH, ui->lineEdit_h_log_path->text().toLocal8Bit().data(),true);
	   pFile->setValue(DV_H_LOG_FLAGS, iFlags,true);
	   ui->widget_H_alg->setEnabled(false);
	   ui->pushButton_H_alg_OK->hide();
	   ui->pushButton_H_alg_Cancel->setText(QString::fromLocal8Bit("修改"));
    }
}

void ConfigView::on_pushButton_H_alg_Cancel_clicked()
{
    if (QString::fromLocal8Bit("修改") == ui->pushButton_H_alg_Cancel->text())
    {
        ui->pushButton_H_alg_OK->show();
        ui->widget_H_alg->setEnabled(true);
        ui->pushButton_H_alg_Cancel->setText(QString::fromLocal8Bit("取消"));
    }
    else {
        IFileVariable *pFile = getDVCfgFileVariable();
        if (NULL != pFile)
        {
            ui->lineEdit_hconfig_path->setText(QString::fromLocal8Bit(pFile->getValue(DV_CONFIG_FILE).toCString().c_str()));
            ui->lineEdit_h_log_path->setText(QString::fromLocal8Bit(pFile->getValue(DV_H_LOG_PATH).toCString().c_str()));
            int iFlags = pFile->getValue(DV_H_LOG_FLAGS).toInt();

            ui->checkBox_debug->setChecked(iFlags & 0x1);

            ui->checkBox_info->setChecked(iFlags & 0x2);

            ui->checkBox_warning->setChecked(iFlags & 0x4);

            ui->checkBox_error->setChecked(iFlags & 0x8);
            
           
        }
        ui->widget_H_alg->setEnabled(false);
        ui->pushButton_H_alg_OK->hide();
        ui->pushButton_H_alg_Cancel->setText(QString::fromLocal8Bit("修改"));
    }
}

void ConfigView::on_radioButton_pingpong_clicked()
{
    IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
    int iModel = 0;
    const char *szKey = NULL;
    if (NULL != pFileVariable)
    {
        pFileVariable->setValue(PRODUCT_LINE_RUN_MODEL, 0, true);
    }
}

void ConfigView::on_radioButton_piece_by_piece_clicked()
{
    IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
    int iModel = 0;
    const char *szKey = NULL;
    if (NULL != pFileVariable)
    {
        pFileVariable->setValue(PRODUCT_LINE_RUN_MODEL, 1, true);
    }
}

void ConfigView::on_radioButton_debug_clicked()
{
    IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
    int iModel = 0;
    const char *szKey = NULL;
    if (NULL != pFileVariable)
    {
        pFileVariable->setValue(PRODUCT_LINE_RUN_MODEL, 2, true);
    }
}

void ConfigView::on_pushButton_pp_select_clicked()
{
    string strNameList = getSelectFlowName(0);
    if (!strNameList.empty())
    {
        IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
        int iModel = 0;
        const char *szKey = NULL;
        if (NULL != pFileVariable)
        {
            pFileVariable->setValue(PRODUCT_FLOW_NAMES_1, strNameList, true);
        }
    }
}

void ConfigView::on_pushButton_pbp_select_clicked()
{
    string strNameList = getSelectFlowName(1);
    if (!strNameList.empty())
    {
        IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
        int iModel = 0;
        const char *szKey = NULL;
        if (NULL != pFileVariable)
        {
            pFileVariable->setValue(PRODUCT_FLOW_NAMES_2, strNameList, true);
        }
    }
}

void ConfigView::on_pushButton_debug_select_clicked()
{
    string strNameList = getSelectFlowName(2);
    if (!strNameList.empty())
    {
        IFileVariable* pFileVariable = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
        int iModel = 0;
        const char *szKey = NULL;
        if (NULL != pFileVariable)
        {
            pFileVariable->setValue(PRODUCT_FLOW_NAMES_3, strNameList, true);
        }
    }
}
void  ConfigView::on_pushButton_browse_ClassifyResult_file_clicked()
{
	
	QString strDefault = ui->lineEdit_ClassifyResult_path->text();
	QString fileName = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("请选择等级分类信息保存路径..."), strDefault);
	if (!fileName.isNull())
	{
		ui->lineEdit_ClassifyResult_path->setText(fileName);
	}
}
void ConfigView::on_pushButton_ProductResult_path_clicked()
{
	
	QString strDefault = ui->lineEdit_ProductResult_path->text();
	QString fileName = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("请选择缺陷详细信息保存路径..."), strDefault);
	if (!fileName.isNull())
	{
		ui->lineEdit_ProductResult_path->setText(fileName);
	}
}
void  ConfigView::on_pushButton_ui_log_path_clicked()
{
	
	QString strDefault = ui->lineEdit_ui_log_path->text();
	QString fileName = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("请选择界面日记保存路径..."), strDefault);
	if (!fileName.isNull())
	{
		ui->lineEdit_ui_log_path->setText(fileName);
	}
}
void  ConfigView::on_pushButton_data_OK_clicked()
{
	IFileVariable *pFile = getDVCfgFileVariable();
	if (NULL != pFile)
	{
		int iFlags = 0;
		if (ui->checkBox_ClassifyResult->isChecked())
		{
			iFlags += 0x1;
		}
		if (ui->checkBox_ProductResult->isChecked())
		{
			iFlags += 0x2;
		}
		if (ui->checkBox_uilog->isChecked())
		{
			iFlags += 0x4;
		}
		pFile->setValue(G12_DATA_CLASSIFYRESULT, ui->lineEdit_ClassifyResult_path->text().toLocal8Bit().data(),true);
		pFile->setValue(G12_DATA_PRODUCTRESULT, ui->lineEdit_ProductResult_path->text().toLocal8Bit().data(),true);
		pFile->setValue(G12_DATA_UI_LOG_PATH, ui->lineEdit_ui_log_path->text().toLocal8Bit().data(),true);
		pFile->setValue(G12_DATA_UI_LOG_FLAGS, iFlags,true);
	}
	ui->widget_data_path->setEnabled(false);
	ui->pushButton_data_OK->hide();
	ui->pushButton_data_Cancel->setText(QString::fromLocal8Bit("修改"));
}
void  ConfigView::on_pushButton_data_Cancel_clicked()
{
	if (QString::fromLocal8Bit("修改") == ui->pushButton_data_Cancel->text())
	{
		ui->pushButton_data_OK->show();
		ui->widget_data_path->setEnabled(true);
		ui->pushButton_data_Cancel->setText(QString::fromLocal8Bit("取消"));
	}
	else {
		IFileVariable *pFile = getDVCfgFileVariable();
		if (NULL != pFile)
		{
			ui->lineEdit_ClassifyResult_path->setText(pFile->getValue(G12_DATA_CLASSIFYRESULT).toCString().c_str());
			ui->lineEdit_ProductResult_path->setText(pFile->getValue(G12_DATA_PRODUCTRESULT).toCString().c_str());
			ui->lineEdit_ui_log_path->setText(pFile->getValue(G12_DATA_UI_LOG_PATH).toCString().c_str());
			int iFlags = pFile->getValue(G12_DATA_UI_LOG_FLAGS).toInt();

			ui->checkBox_ClassifyResult->setChecked(iFlags & 0x1);

			ui->checkBox_ProductResult->setChecked(iFlags & 0x2);

			ui->checkBox_uilog->setChecked(iFlags & 0x4);
			
		}
		ui->widget_data_path->setEnabled(false);
		ui->pushButton_data_OK->hide();
		ui->pushButton_data_Cancel->setText(QString::fromLocal8Bit("修改"));
	}
}

void ConfigView::on_pushButton_move_right_clicked()
{
	INotifyService* pNotifyService = CommonUtils::getNotifyService();
	if (NULL == pNotifyService)
	{
		QMessageBox::information(this, "Info", QString::fromLocal8Bit("消息服务插件加载失败"));
		return ;
	}
	QString wParam = NOTIFY_MOVE_RIGHT;
	pNotifyService->send("mainui", "ctr_move", (OS_LPARAM)&wParam, NULL);
}
void ConfigView::on_pushButton_move_stop_clicked()
{
	INotifyService* pNotifyService = CommonUtils::getNotifyService();
	if (NULL == pNotifyService)
	{
		QMessageBox::information(this, "Info", QString::fromLocal8Bit("消息服务插件加载失败"));
		return;
	}
	QString wParam = NOTIFY_MOVE_STOP;
	pNotifyService->send("mainui", "ctr_move", (OS_LPARAM)&wParam, NULL);

}
void ConfigView::on_pushButton_mov_left_clicked()
{
	INotifyService* pNotifyService = CommonUtils::getNotifyService();
	if (NULL == pNotifyService)
	{
		QMessageBox::information(this, "Info", QString::fromLocal8Bit("消息服务插件加载失败"));
		return;
	}
	QString wParam = NOTIFY_MOVE_LEFT;
	pNotifyService->send("mainui", "ctr_move", (OS_LPARAM)&wParam, NULL);
}







void ConfigView::on_pushButton_pro_sn_clicked()
{
    unsigned int iProSn = ui->spinBox_pro_sn->value();
    if (QMessageBox::No == QMessageBox::warning(NULL, "warn", QString::fromLocal8Bit("请确认是否设置产品SN - %1 ？").arg(iProSn), QMessageBox::Yes | QMessageBox::No))
        return;

    INotifyService* pNotifyService = CommonUtils::getNotifyService();
    if (NULL == pNotifyService)
    {
        QMessageBox::information(this, "Info", QString::fromLocal8Bit("消息服务插件加载失败"));
        return;
    }

    pNotifyService->send("mainui", "set_product_sn", (OS_LPARAM)iProSn, NULL);
}
void ConfigView::on_pushButton_save_choose_front_clicked()
{
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile)
	{

		int iChoose = ui->comboBox_choose_front->currentIndex();
		pFile->setValue(WORK_MODE_FRONT_CHOOSE, iChoose, true);//

	}
}
void ConfigView::on_pushButton_work_time_start_clicked()
{
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile)
	{
		int iChoose = ui->comboBox_work_time_start->currentIndex();
		pFile->setValue(WORK_MODE_AM_STRAT_TIME, iChoose, true);//
	}
}

void ConfigView::on_pushButton_cgblockOutTime_clicked()
{
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile)
	{
        int ivalue = ui->spinBox_cgblock->value();
        pFile->setValue(IMAGE_PARAM_CGBLOCK_TIMEOUT, ivalue, true);//
	}
}


//不屏蔽其它信号，不保存图片，算法超时开启，显示为流水线模式
void ConfigView::on_pushButton_online_clicked()
{
    ui->checkBox_debug_crl_clear->setChecked(false);
	ui->checkBox_debug_up_flow->setChecked(false);
	//ui->checkBox_debug_mode->setChecked(false);
	ui->checkBox_save_source_image->setChecked(false);
	ui->checkBox_debug_down_flow->setChecked(false);
	ui->checkBox_debug_clear_mode->setChecked(false);
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile != NULL)
	{
        pFile->setValue(IMAGE_PARAM_DISABLE_CLEAN, false, true);
        pFile->setValue(IMAGE_PARAM_DISABLE_SHANG, false, true);
        pFile->setValue(IMAGE_PARAM_DISABLE_XIA, false, true);
		pFile->setValue(IMAGE_PARAM_SAVE_SIMAGE_ENABLE, false, true);
		pFile->setValue(IMAGE_PARAM_RUN_MODE, 0,true);
	}
	//writeDvTimeOutFlag(true);
}
//屏蔽其它信号，保存图片，算法超时关闭，显示为测试模式
void ConfigView::on_pushButton_offline_clicked()
{
	ui->checkBox_debug_crl_clear->setChecked(true);
	ui->checkBox_debug_up_flow->setChecked(true);
	//ui->checkBox_debug_mode->setChecked(true);
	ui->checkBox_save_source_image->setChecked(true);
	ui->checkBox_debug_down_flow->setChecked(true);
	ui->checkBox_debug_clear_mode->setChecked(true);
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile != NULL)
	{
        pFile->setValue(IMAGE_PARAM_DISABLE_CLEAN, true, true);
        pFile->setValue(IMAGE_PARAM_DISABLE_SHANG, true, true);
        pFile->setValue(IMAGE_PARAM_DISABLE_XIA, true, true);
		pFile->setValue(IMAGE_PARAM_SAVE_SIMAGE_ENABLE, true, true);
		pFile->setValue(IMAGE_PARAM_RUN_MODE, 1, true);
	}
	//writeDvTimeOutFlag(false);
}

//同时写json文件和ini文件
void ConfigView::writeDvTimeOutFlag(bool bOpen)
{
	//int station = ui->comboBox_timeoutStation->currentIndex();
	int flag = bOpen ? 1 : 0;
	//两个工位算法超时同时开启关闭
	ModifyJsonFile(string("bStart_timer"),flag, JSON_CONFIG_FOR_BACK_DOWN);
	ModifyJsonFile(string("bStart_timer"), flag, JSON_CONFIG_FOR_BACK_UP);
	ui->checkBox_H_timeout->setChecked(flag);
}

void ConfigView::on_pushButton_DvOutTime_clicked()
{
	//两个工位算法超时时间可单独设置
	//int station = ui->comboBox_timeoutStation->currentIndex();
	int station;
    QString text;// = ui->comboBox_timeoutStation->currentText();
	if (text == QString::fromLocal8Bit("后端下相机工位"))
	{
		station = JSON_CONFIG_FOR_BACK_DOWN;
	}
	else if (text == QString::fromLocal8Bit("后端上相机工位"))
	{
		station = JSON_CONFIG_FOR_BACK_UP;
	}
	else
	{
		return;
	}
	int iDelaytime = ui->spinBox_DvOutTime->value();
	ModifyJsonFile(string("iDelaytime"), iDelaytime,station);

    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if (pFile)
    {
        pFile->setValue(IMAGE_PARAM_H_TIMEOUT_ENABLE,iDelaytime,true);
    }
}

void ConfigView::ModifyJsonFile(string key,int value,int station)
{
	IFileVariable *pFile = getDVCfgFileVariable();
	string fileName = "";
	if (pFile != NULL)
	{
		fileName = pFile->getValue(DV_CONFIG_FILE).toCString() + "/hconfig/config.json";
	}
	int iRet = 0;
	char *json = NULL;
	QFile fileQ;
	do
	{
		fileQ.setFileName(QString::fromLocal8Bit(fileName.c_str()));
		if (!fileQ.open(QIODevice::ReadOnly))
		{
			//QMessageBox::information(this, QString::fromLocal8Bit("配置更新"), QString::fromLocal8Bit("配置文件读取失败(%1)").arg(fileName.c_str()));
			iRet = -1;
			break;
		}
		//读取原有配置文件
		int iSize = fileQ.size();
		json = new char[iSize + 1];
		memset(json, 0, iSize + 1);
		if (iSize != fileQ.read(json, iSize))
		{
			//QMessageBox::information(this, QString::fromLocal8Bit("配置更新"), QString::fromLocal8Bit("配置文件读取失败(%1)").arg(fileName.c_str()));
			break;
		}
		fileQ.close();
		//解析Json文件
		Document hConfigDoc;
		string strJson = SString::UTF8ToGBK(json);
		hConfigDoc.Parse(strJson.c_str());
		if (hConfigDoc.HasParseError())
		{
			//QMessageBox::information(this, QString::fromLocal8Bit("配置更新"), QString::fromLocal8Bit("配置文件解析失败,错误码%1, 位置：%2").arg(hConfigDoc.GetParseError()).arg(hConfigDoc.GetErrorOffset()));
			iRet = -1;
			break;
		}
		Value::MemberIterator itPruductItem;
		Value::MemberIterator itParamItem;
		itPruductItem = hConfigDoc.FindMember(JSON_FIRST_KEY);
		if (itPruductItem != hConfigDoc.MemberEnd() && itPruductItem->value.IsArray())
		{
			itParamItem = itPruductItem->value[station].FindMember(key.c_str());
			if (itParamItem->value.IsInt())
			{
				int currentValue = itParamItem->value.GetInt();
				if (currentValue != value)
				{
					itParamItem->value.SetInt(value);
					//更新Json Buffer内存
					StringBuffer buffer;
					PrettyWriter<StringBuffer> writer(buffer);
					hConfigDoc.Accept(writer);
					strJson = SString::GBKToUTF8(buffer.GetString());

					if (!fileQ.open(QIODevice::WriteOnly))
					{
						//QMessageBox::information(this, QString::fromLocal8Bit("配置更新"), QString::fromLocal8Bit("配置文件更新失败"));
						iRet = -1;
						break;
					}
					iSize = strJson.size();
					if (iSize != fileQ.write(strJson.c_str(), iSize))
					{
						//QMessageBox::information(this, QString::fromLocal8Bit("配置更新"), QString::fromLocal8Bit("配置文件更新失败"));
						iRet = -1;
						break;
					}
					fileQ.close();
				}
			}
		};
		//QMessageBox::information(this, QString::fromLocal8Bit("配置更新"), QString::fromLocal8Bit("配置文件更新成功"));
	} while (0);
	if (fileQ.isOpen())
	{
		fileQ.close();
	}
	if (NULL != json)
	{
		delete[]json;
		json = NULL;
	}
}


int ConfigView::ReadJsonFile(string key,int station)
{
	int value = 0;
	IFileVariable *pFile = getDVCfgFileVariable();
	string fileName = "";
	if (pFile != NULL)
	{
		fileName = pFile->getValue(DV_CONFIG_FILE).toCString() + "/hconfig/config.json";
	}
	int iRet = 0;
	char *json = NULL;
	QFile fileQ;
	do
	{
		fileQ.setFileName(QString::fromLocal8Bit(fileName.c_str()));
		if (!fileQ.open(QIODevice::ReadOnly))
		{
			//QMessageBox::information(this, QString::fromLocal8Bit("配置更新"), QString::fromLocal8Bit("配置文件读取失败(%1)").arg(fileName.c_str()));
			iRet = -1;
			break;
		}
		//读取原有配置文件
		int iSize = fileQ.size();
		json = new char[iSize + 1];
		memset(json, 0, iSize + 1);
		if (iSize != fileQ.read(json, iSize))
		{
			//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("配置文件读取失败(%1)").arg(fileName.c_str()), UILOG_ERR);
			break;
		}
		fileQ.close();
		//解析Json文件
		Document hConfigDoc;
		string strJson = SString::UTF8ToGBK(json);
		hConfigDoc.Parse(strJson.c_str());
		if (hConfigDoc.HasParseError())
		{
			//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("配置文件解析失败,错误码%1, 位置：%2").arg(hConfigDoc.GetParseError()).arg(hConfigDoc.GetErrorOffset()), UILOG_ERR);
			iRet = -1;
			break;
		}
		Value::MemberIterator itPruductItem;
		Value::MemberIterator itParamItem;
		itPruductItem = hConfigDoc.FindMember(JSON_FIRST_KEY);
		if (itPruductItem != hConfigDoc.MemberEnd() && itPruductItem->value.IsArray())
		{
			itParamItem = itPruductItem->value[station].FindMember(key.c_str());
			if (itParamItem->value.IsInt())
			{
				value = itParamItem->value.GetInt();
				fileQ.close();
			}
		};
	} while (0);
	if (fileQ.isOpen())
	{
		fileQ.close();
	}
	if (NULL != json)
	{
		delete[]json;
		json = NULL;
	}
	return value;
}

QList<QFileInfo> ConfigView::GetFileList(QString path)
{
	QFileInfoList file_list;
	file_list.clear();

	if (path.isEmpty())
		return file_list;

	QDir dir(path);
	//获取文件夹信息
	file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	if (ui->checkBox_recursion->isChecked())
	{
		QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

		for (int i = 0; i != folder_list.size(); i++)
		{
			QString name = folder_list.at(i).absoluteFilePath();
			QFileInfoList child_file_list = GetFileList(name);
			file_list.append(child_file_list);
		}
	}
	return file_list;
}



void ConfigView::parseFileInfoList(QList<QFileInfo> fileInfoList)
{
	for (int i = 0; i < fileInfoList.size(); ++i)
	{
		QString fileName = fileInfoList.at(i).fileName();
		if (fileName.contains(".bmp"))
		{
		   //QRegularExpression reg("(?<=\()\\d+(?=\))");
		   //QStringList strList = fileName.split(reg);
		   int index1 = fileName.indexOf("(");
		   int index2 = fileName.indexOf(")");
		   QString strSn = fileName.mid(index1 + 1, index2 - index1-1);
		   int sn = strSn.toInt();
		   if (m_snSet.find(sn) != m_snSet.end())
		   {
		   	  if (m_NameListMap.contains(sn))
		   	  {
		   	  	m_NameListMap[sn].push_back(fileName);
		   	  }
		   }
		   else
		   {
		   	  m_snSet.insert(sn);
		   	  QList<QString> nameList;
		   	  nameList.push_back(fileName);
		   	  m_NameListMap.insert(sn, nameList);
		   }
		//m_ImageFilesCount++;
		}
	}
	set<int>::iterator it = m_snSet.begin();
	while (it != m_snSet.end())
	{
		m_snList.push_back(*it);
		it++;
	}
}

void ConfigView::offlineChangeProduct(string type)
{
	if (m_snList.size() <= 0)
	{
		//GetChannelFileList(ui->lineEdit_offline_path->text());
		parseFileInfoList(GetFileList(ui->lineEdit_offline_path->text()));
	}
	if (m_snList.size() <= 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("离线文件"), QString::fromLocal8Bit("当前目录下无离线文件"), QMessageBox::Ok);
		return;
	}
	if (m_snList.size() == 0 || m_NameListMap.size() == 0)
	{
		return;
	}
	m_fileNameList.clear();
	int size = m_snList.size();
	int index = -1;
	for (int i = 0; i < m_snList.size(); ++i)
	{
		if (m_snList.at(i) == ui->lineEdit_image_sn->text().toInt())
		{
			index = i;
		}
	}
	if (index < 0)	//snList不包含edit上的sn,取第0個
	{
		index = 0;
	}
	if (type == "next")
	{
		if (++index >= size)
		{
			index = 0;
		}
	}
	else if(type == "forward")
	{
		if (--index <= -1)
		{
			index = size - 1;
		}
	}
	m_currentImageFileIndex = m_snList.at(index);
	for (int i = 0; i < m_NameListMap[m_currentImageFileIndex].size(); ++i)
	{
		m_fileNameList.push_back(m_NameListMap[m_currentImageFileIndex].at(i));
	}
	UpdateOfflineFile();
	ui->lineEdit_image_sn->setText(QString::number(m_currentImageFileIndex));
	IFileVariable* pFile = getCameraSetCfgFileVariable();
	if (pFile != NULL)
	{
		pFile->setValue(WORK_PRODUCT_SN, ui->lineEdit_image_sn->text().toLocal8Bit().data(), true);
	}
}
