#include "defectcfg.h"
#include "ui_defectcfg.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "sendmessage.h"
defectcfg* defectcfg::m_pSelfInstance = NULL;

defectcfg::defectcfg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::defectcfg)
{
    ui->setupUi(this);
	setWindowFlags(Qt::Dialog);// set move and close crl;
	on_pushButton_canle_clicked();
	//ÇÐ²úÆ·¸üÐÂ¶ÔÓ¦µÄui
	connect(CSendMessage::GetInstance(), SIGNAL(signalChangeProduct()), this, SLOT(slotChangePruduct()));
}

defectcfg::~defectcfg()
{
    delete ui;
}

void defectcfg::setEnable(bool bEn)
{
	//1
	ui->LightLeak_enable->setEnabled(bEn);//
	ui->LightLeak_area_max->setEnabled(bEn);//
	ui->LightLeak_len_max->setEnabled(bEn);//
	ui->LightLeak_ch_area_min->setEnabled(bEn);//
	ui->LightLeak_ch_area_max->setEnabled(bEn);//
	ui->LightLeak_ch_area_number->setEnabled(bEn);//
	ui->LightLeak_ch_len_min->setEnabled(bEn);//
	ui->LightLeak_ch_len_max->setEnabled(bEn);//
	ui->LightLeak_ch_len_number->setEnabled(bEn);//
	ui->LightLeak_ch_sec_area_max->setEnabled(bEn);//
	ui->LightLeak_ch_sec_area_number->setEnabled(bEn);//
	ui->LightLeak_ch_sec_len_max->setEnabled(bEn);//
	ui->LightLeak_ch_sec_len_number->setEnabled(bEn);//
	//2	
	ui->ExcessLnk_enable->setEnabled(bEn);//
	ui->ExcessLnk_area_max->setEnabled(bEn);//
	ui->ExcessLnk_len_max->setEnabled(bEn);//
	ui->ExcessLnk_ch_area_min->setEnabled(bEn);//
	ui->ExcessLnk_ch_area_max->setEnabled(bEn);//
	ui->ExcessLnk_ch_area_number->setEnabled(bEn);//
	ui->ExcessLnk_ch_len_min->setEnabled(bEn);//
	ui->ExcessLnk_ch_len_max->setEnabled(bEn);//
	ui->ExcessLnk_ch_len_number->setEnabled(bEn);//
	ui->ExcessLnk_ch_sec_area_max->setEnabled(bEn);//
	ui->ExcessLnk_ch_sec_area_number->setEnabled(bEn);//
	ui->ExcessLnk_ch_sec_len_max->setEnabled(bEn);//
	ui->ExcessLnk_ch_sec_len_number->setEnabled(bEn);//

	 //3
	ui->Discolouration_enable->setEnabled(bEn);//
	ui->Discolouration_area_max->setEnabled(bEn);//
	ui->Discolouration_len_max->setEnabled(bEn);//
	ui->Discolouration_ch_area_min->setEnabled(bEn);//
	ui->Discolouration_ch_area_max->setEnabled(bEn);//
	ui->Discolouration_ch_area_number->setEnabled(bEn);//
	ui->Discolouration_ch_len_min->setEnabled(bEn);//
	ui->Discolouration_ch_len_max->setEnabled(bEn);//
	ui->Discolouration_ch_len_number->setEnabled(bEn);//
	ui->Discolouration_ch_sec_area_max->setEnabled(bEn);//
	ui->Discolouration_ch_sec_area_number->setEnabled(bEn);//
	ui->Discolouration_ch_sec_len_max->setEnabled(bEn);//
	ui->Discolouration_ch_sec_len_number->setEnabled(bEn);//

     //4
	ui->SawEgdge_enable->setEnabled(bEn);//
	ui->SawEgdge_area_max->setEnabled(bEn);//
	ui->SawEgdge_len_max->setEnabled(bEn);//
	ui->SawEgdge_ch_area_min->setEnabled(bEn);//
	ui->SawEgdge_ch_area_max->setEnabled(bEn);//
	ui->SawEgdge_ch_area_number->setEnabled(bEn);//
	ui->SawEgdge_ch_len_min->setEnabled(bEn);//
	ui->SawEgdge_ch_len_max->setEnabled(bEn);//
	ui->SawEgdge_ch_len_number->setEnabled(bEn);//
	ui->SawEgdge_ch_sec_area_max->setEnabled(bEn);//
	ui->SawEgdge_ch_sec_area_number->setEnabled(bEn);//
	ui->SawEgdge_ch_sec_len_max->setEnabled(bEn);//
	ui->SawEgdge_ch_sec_len_number->setEnabled(bEn);//

	//5 ÔàÎÛ
	ui->Smirch_enable->setEnabled(bEn);//
	ui->Smirch_area_max->setEnabled(bEn);//
	ui->Smirch_len_max->setEnabled(bEn);//
	ui->Smirch_ch_area_min->setEnabled(bEn);//
	ui->Smirch_ch_area_max->setEnabled(bEn);//
	ui->Smirch_ch_area_number->setEnabled(bEn);//
	ui->Smirch_ch_len_min->setEnabled(bEn);//
	ui->Smirch_ch_len_max->setEnabled(bEn);//
	ui->Smirch_ch_len_number->setEnabled(bEn);//
	ui->Smirch_ch_sec_area_max->setEnabled(bEn);//
	ui->Smirch_ch_sec_area_number->setEnabled(bEn);//
	ui->Smirch_ch_sec_len_max->setEnabled(bEn);//
	ui->Smirch_ch_sec_len_number->setEnabled(bEn);//

    //6
	ui->AccuLnk_enable->setEnabled(bEn);//
	ui->AccuLnk_area_max->setEnabled(bEn);//
	ui->AccuLnk_len_max->setEnabled(bEn);//
	ui->AccuLnk_ch_area_min->setEnabled(bEn);//
	ui->AccuLnk_ch_area_max->setEnabled(bEn);//
	ui->AccuLnk_ch_area_number->setEnabled(bEn);//
	ui->AccuLnk_ch_len_min->setEnabled(bEn);//
	ui->AccuLnk_ch_len_max->setEnabled(bEn);//
	ui->AccuLnk_ch_len_number->setEnabled(bEn);//
	ui->AccuLnk_ch_sec_area_max->setEnabled(bEn);//
	ui->AccuLnk_ch_sec_area_number->setEnabled(bEn);//
	ui->AccuLnk_ch_sec_len_max->setEnabled(bEn);//
	ui->AccuLnk_ch_sec_len_number->setEnabled(bEn);//


	//7
	ui->DistHole_enable->setEnabled(bEn);//
	ui->DistHole_area_max->setEnabled(bEn);//
	ui->DistHole_len_max->setEnabled(bEn);//
	ui->DistHole_ch_area_min->setEnabled(bEn);//
	ui->DistHole_ch_area_max->setEnabled(bEn);//
	ui->DistHole_ch_area_number->setEnabled(bEn);//
	ui->DistHole_ch_len_min->setEnabled(bEn);//
	ui->DistHole_ch_len_max->setEnabled(bEn);//
	ui->DistHole_ch_len_number->setEnabled(bEn);//
	ui->DistHole_ch_sec_area_max->setEnabled(bEn);//
	ui->DistHole_ch_sec_area_number->setEnabled(bEn);//
	ui->DistHole_ch_sec_len_max->setEnabled(bEn);//
	ui->DistHole_ch_sec_len_number->setEnabled(bEn);//


    //8
	ui->MisPrint_enable->setEnabled(bEn);//
	ui->MisPrint_area_max->setEnabled(bEn);//
	ui->MisPrint_len_max->setEnabled(bEn);//
	ui->MisPrint_ch_area_min->setEnabled(bEn);//
	ui->MisPrint_ch_area_max->setEnabled(bEn);//
	ui->MisPrint_ch_area_number->setEnabled(bEn);//
	ui->MisPrint_ch_len_min->setEnabled(bEn);//
	ui->MisPrint_ch_len_max->setEnabled(bEn);//
	ui->MisPrint_ch_len_number->setEnabled(bEn);//
	ui->MisPrint_ch_sec_area_max->setEnabled(bEn);//
	ui->MisPrint_ch_sec_area_number->setEnabled(bEn);//
	ui->MisPrint_ch_sec_len_max->setEnabled(bEn);//
	ui->MisPrint_ch_sec_len_number->setEnabled(bEn);//

	//9 ¹ÎÉË
	ui->Scratches_enable->setEnabled(bEn);//
	ui->Scratches_area_max->setEnabled(bEn);//
	ui->Scratches_len_max->setEnabled(bEn);//
	ui->Scratches_ch_area_min->setEnabled(bEn);//
	ui->Scratches_ch_area_max->setEnabled(bEn);//
	ui->Scratches_ch_area_number->setEnabled(bEn);//
	ui->Scratches_ch_len_min->setEnabled(bEn);//
	ui->Scratches_ch_len_max->setEnabled(bEn);//
	ui->Scratches_ch_len_number->setEnabled(bEn);//
	ui->Scratches_ch_sec_area_max->setEnabled(bEn);//
	ui->Scratches_ch_sec_area_number->setEnabled(bEn);//
	ui->Scratches_ch_sec_len_max->setEnabled(bEn);//
	ui->Scratches_ch_sec_len_number->setEnabled(bEn);//


	//10±À±ß
	ui->Chips_enable->setEnabled(bEn);//
	ui->Chips_area_max->setEnabled(bEn);//
	ui->Chips_len_max->setEnabled(bEn);//
	ui->Chips_ch_area_min->setEnabled(bEn);//
	ui->Chips_ch_area_max->setEnabled(bEn);//
	ui->Chips_ch_area_number->setEnabled(bEn);//
	ui->Chips_ch_len_min->setEnabled(bEn);//
	ui->Chips_ch_len_max->setEnabled(bEn);//
	ui->Chips_ch_len_number->setEnabled(bEn);//
	ui->Chips_ch_sec_area_max->setEnabled(bEn);//
	ui->Chips_ch_sec_area_number->setEnabled(bEn);//
	ui->Chips_ch_sec_len_max->setEnabled(bEn);//
	ui->Chips_ch_sec_len_number->setEnabled(bEn);//
	//11°¼Í¹µã
	ui->Dent_enable->setEnabled(bEn);//
	ui->Dent_area_max->setEnabled(bEn);//
	ui->Dent_ch_area_min->setEnabled(bEn);//
	ui->Dent_ch_area_max->setEnabled(bEn);//
	ui->Dent_ch_area_number->setEnabled(bEn);//
	ui->Dent_ch_sec_area_max->setEnabled(bEn);//
	ui->Dent_ch_sec_area_number->setEnabled(bEn);//

	ui->defect_max_number->setEnabled(bEn);
}

void defectcfg::updateUi()
{
	IFileVariable *pFile = getDefectFileVariable();
	if (pFile)
	{
		//1 Â©¹â
		ui->LightLeak_enable->setChecked(pFile->getValue(LIGHTLEAK_ENABLE).toBool());
		ui->LightLeak_area_max->setValue(pFile->getValue(LIGHTLEAK_AREA_MAX).toDouble());
		ui->LightLeak_len_max->setValue(pFile->getValue(LIGHTLEAK_LEN_MAX).toDouble());
		ui->LightLeak_ch_area_min->setValue(pFile->getValue(LIGHTLEAK_CH_AREA_MIN).toDouble());
		ui->LightLeak_ch_area_max->setValue(pFile->getValue(LIGHTLEAK_CH_AREA_MAX).toDouble());
		ui->LightLeak_ch_area_number->setValue(pFile->getValue(LIGHTLEAK_CH_AREA_NUMBER).toInt());
		ui->LightLeak_ch_len_min->setValue(pFile->getValue(LIGHTLEAK_CH_LEN_MIN).toDouble());
		ui->LightLeak_ch_len_max->setValue(pFile->getValue(LIGHTLEAK_CH_LEN_MAX).toDouble());
		ui->LightLeak_ch_len_number->setValue(pFile->getValue(LIGHTLEAK_CH_LEN_NUMBER).toInt());
		ui->LightLeak_ch_sec_area_max->setValue(pFile->getValue(LIGHTLEAK_CH_SEC_AREA_MAX).toDouble());
		ui->LightLeak_ch_sec_area_number->setValue(pFile->getValue(LIGHTLEAK_CH_SEC_AREA_NUMBER).toInt());
		ui->LightLeak_ch_sec_len_max->setValue(pFile->getValue(LIGHTLEAK_CH_SEC_LEN).toDouble());
		ui->LightLeak_ch_sec_len_number->setValue(pFile->getValue(LIGHTLEAK_CH_SEC_LEN_NUMBER).toInt());

		//2 ÒçÄ«
		ui->ExcessLnk_enable->setChecked(pFile->getValue(EXCESSLNK_ENABLE).toBool());
		ui->ExcessLnk_area_max->setValue(pFile->getValue(EXCESSLNK_AREA_MAX).toDouble());
		ui->ExcessLnk_len_max->setValue(pFile->getValue(EXCESSLNK_LEN_MAX).toDouble());
		ui->ExcessLnk_ch_area_min->setValue(pFile->getValue(EXCESSLNK_CH_AREA_MIN).toDouble());
		ui->ExcessLnk_ch_area_max->setValue(pFile->getValue(EXCESSLNK_CH_AREA_MAX).toDouble());
		ui->ExcessLnk_ch_area_number->setValue(pFile->getValue(EXCESSLNK_CH_AREA_NUMBER).toInt());
		ui->ExcessLnk_ch_len_min->setValue(pFile->getValue(EXCESSLNK_CH_LEN_MIN).toDouble());
		ui->ExcessLnk_ch_len_max->setValue(pFile->getValue(EXCESSLNK_CH_LEN_MAX).toDouble());
		ui->ExcessLnk_ch_len_number->setValue(pFile->getValue(EXCESSLNK_CH_LEN_NUMBER).toInt());
		ui->ExcessLnk_ch_sec_area_max->setValue(pFile->getValue(EXCESSLNK_CH_SEC_AREA_MAX).toDouble());
		ui->ExcessLnk_ch_sec_area_number->setValue(pFile->getValue(EXCESSLNK_CH_SEC_AREA_NUMBER).toInt());
		ui->ExcessLnk_ch_sec_len_max->setValue(pFile->getValue(EXCESSLNK_CH_SEC_LEN).toDouble());
		ui->ExcessLnk_ch_sec_len_number->setValue(pFile->getValue(EXCESSLNK_CH_SEC_LEN_NUMBER).toInt());

		//3 ÒìÉ«
		ui->Discolouration_enable->setChecked(pFile->getValue(DISCOLOURATION_ENABLE).toBool());
		ui->Discolouration_area_max->setValue(pFile->getValue(DISCOLOURATION_AREA_MAX).toDouble());
		ui->Discolouration_len_max->setValue(pFile->getValue(DISCOLOURATION_LEN_MAX).toDouble());
		ui->Discolouration_ch_area_min->setValue(pFile->getValue(DISCOLOURATION_CH_AREA_MIN).toDouble());
		ui->Discolouration_ch_area_max->setValue(pFile->getValue(DISCOLOURATION_CH_AREA_MAX).toDouble());
		ui->Discolouration_ch_area_number->setValue(pFile->getValue(DISCOLOURATION_CH_AREA_NUMBER).toInt());
		ui->Discolouration_ch_len_min->setValue(pFile->getValue(DISCOLOURATION_CH_LEN_MIN).toDouble());
		ui->Discolouration_ch_len_max->setValue(pFile->getValue(DISCOLOURATION_CH_LEN_MAX).toDouble());
		ui->Discolouration_ch_len_number->setValue(pFile->getValue(DISCOLOURATION_CH_LEN_NUMBER).toInt());
		ui->Discolouration_ch_sec_area_max->setValue(pFile->getValue(DISCOLOURATION_CH_SEC_AREA_MAX).toDouble());
		ui->Discolouration_ch_sec_area_number->setValue(pFile->getValue(DISCOLOURATION_CH_SEC_AREA_NUMBER).toInt());
		ui->Discolouration_ch_sec_len_max->setValue(pFile->getValue(DISCOLOURATION_CH_SEC_LEN).toDouble());
		ui->Discolouration_ch_sec_len_number->setValue(pFile->getValue(DISCOLOURATION_CH_SEC_LEN_NUMBER).toInt());

		//4 ¾â³Ý
		ui->SawEgdge_enable->setChecked(pFile->getValue(          SAWEGDGE_ENABLE).toBool());
		ui->SawEgdge_area_max->setValue(pFile->getValue(          SAWEGDGE_AREA_MAX).toDouble());
		ui->SawEgdge_len_max->setValue(pFile->getValue(           SAWEGDGE_LEN_MAX).toDouble());
		ui->SawEgdge_ch_area_min->setValue(pFile->getValue(       SAWEGDGE_CH_AREA_MIN).toDouble());
		ui->SawEgdge_ch_area_max->setValue(pFile->getValue(       SAWEGDGE_CH_AREA_MAX).toDouble());
		ui->SawEgdge_ch_area_number->setValue(pFile->getValue(    SAWEGDGE_CH_AREA_NUMBER).toInt());
		ui->SawEgdge_ch_len_min->setValue(pFile->getValue(        SAWEGDGE_CH_LEN_MIN).toDouble());
		ui->SawEgdge_ch_len_max->setValue(pFile->getValue(        SAWEGDGE_CH_LEN_MAX).toDouble());
		ui->SawEgdge_ch_len_number->setValue(pFile->getValue(     SAWEGDGE_CH_LEN_NUMBER).toInt());
		ui->SawEgdge_ch_sec_area_max->setValue(pFile->getValue(   SAWEGDGE_CH_SEC_AREA_MAX).toDouble());
		ui->SawEgdge_ch_sec_area_number->setValue(pFile->getValue(SAWEGDGE_CH_SEC_AREA_NUMBER).toInt());
		ui->SawEgdge_ch_sec_len_max->setValue(pFile->getValue(    SAWEGDGE_CH_SEC_LEN).toDouble());
		ui->SawEgdge_ch_sec_len_number->setValue(pFile->getValue( SAWEGDGE_CH_SEC_LEN_NUMBER).toInt());

		//5 ÔàÎÛ
		ui->Smirch_enable->setChecked(pFile->getValue(SMIRCH_ENABLE).toBool());
		ui->Smirch_area_max->setValue(pFile->getValue(SMIRCH_AREA_MAX).toDouble());
		ui->Smirch_len_max->setValue(pFile->getValue(SMIRCH_LEN_MAX).toDouble());
		ui->Smirch_ch_area_min->setValue(pFile->getValue(SMIRCH_CH_AREA_MIN).toDouble());
		ui->Smirch_ch_area_max->setValue(pFile->getValue(SMIRCH_CH_AREA_MAX).toDouble());
		ui->Smirch_ch_area_number->setValue(pFile->getValue(SMIRCH_CH_AREA_NUMBER).toInt());
		ui->Smirch_ch_len_min->setValue(pFile->getValue(SMIRCH_CH_LEN_MIN).toDouble());
		ui->Smirch_ch_len_max->setValue(pFile->getValue(SMIRCH_CH_LEN_MAX).toDouble());
		ui->Smirch_ch_len_number->setValue(pFile->getValue(SMIRCH_CH_LEN_NUMBER).toInt());
		ui->Smirch_ch_sec_area_max->setValue(pFile->getValue(SMIRCH_CH_SEC_AREA_MAX).toDouble());
		ui->Smirch_ch_sec_area_number->setValue(pFile->getValue(SMIRCH_CH_SEC_AREA_NUMBER).toInt());
		ui->Smirch_ch_sec_len_max->setValue(pFile->getValue(SMIRCH_CH_SEC_LEN).toDouble());
		ui->Smirch_ch_sec_len_number->setValue(pFile->getValue(SMIRCH_CH_SEC_LEN_NUMBER).toInt());

		//6 ÓÍÄ«¶Ñ»ý
		ui->AccuLnk_enable->setChecked(pFile->getValue(          ACCULNK_ENABLE).toBool());
		ui->AccuLnk_area_max->setValue(pFile->getValue(          ACCULNK_AREA_MAX).toDouble());
		ui->AccuLnk_len_max->setValue(pFile->getValue(           ACCULNK_LEN_MAX).toDouble());
		ui->AccuLnk_ch_area_min->setValue(pFile->getValue(       ACCULNK_CH_AREA_MIN).toDouble());
		ui->AccuLnk_ch_area_max->setValue(pFile->getValue(       ACCULNK_CH_AREA_MAX).toDouble());
		ui->AccuLnk_ch_area_number->setValue(pFile->getValue(    ACCULNK_CH_AREA_NUMBER).toInt());
		ui->AccuLnk_ch_len_min->setValue(pFile->getValue(        ACCULNK_CH_LEN_MIN).toDouble());
		ui->AccuLnk_ch_len_max->setValue(pFile->getValue(        ACCULNK_CH_LEN_MAX).toDouble());
		ui->AccuLnk_ch_len_number->setValue(pFile->getValue(     ACCULNK_CH_LEN_NUMBER).toInt());
		ui->AccuLnk_ch_sec_area_max->setValue(pFile->getValue(   ACCULNK_CH_SEC_AREA_MAX).toDouble());
		ui->AccuLnk_ch_sec_area_number->setValue(pFile->getValue(ACCULNK_CH_SEC_AREA_NUMBER).toInt());
		ui->AccuLnk_ch_sec_len_max->setValue(pFile->getValue(    ACCULNK_CH_SEC_LEN).toDouble());
		ui->AccuLnk_ch_sec_len_number->setValue(pFile->getValue( ACCULNK_CH_SEC_LEN_NUMBER).toInt());

		//7 ¿×±äÐÎ
		ui->DistHole_enable->setChecked(pFile->getValue(DISTHOLE_ENABLE).toBool());
		ui->DistHole_area_max->setValue(pFile->getValue(DISTHOLE_AREA_MAX).toDouble());
		ui->DistHole_len_max->setValue(pFile->getValue(DISTHOLE_LEN_MAX).toDouble());
		ui->DistHole_ch_area_min->setValue(pFile->getValue(DISTHOLE_CH_AREA_MIN).toDouble());
		ui->DistHole_ch_area_max->setValue(pFile->getValue(DISTHOLE_CH_AREA_MAX).toDouble());
		ui->DistHole_ch_area_number->setValue(pFile->getValue(DISTHOLE_CH_AREA_NUMBER).toInt());
		ui->DistHole_ch_len_min->setValue(pFile->getValue(DISTHOLE_CH_LEN_MIN).toDouble());
		ui->DistHole_ch_len_max->setValue(pFile->getValue(DISTHOLE_CH_LEN_MAX).toDouble());
		ui->DistHole_ch_len_number->setValue(pFile->getValue(DISTHOLE_CH_LEN_NUMBER).toInt());
		ui->DistHole_ch_sec_area_max->setValue(pFile->getValue(DISTHOLE_CH_SEC_AREA_MAX).toDouble());
		ui->DistHole_ch_sec_area_number->setValue(pFile->getValue(DISTHOLE_CH_SEC_AREA_NUMBER).toInt());
		ui->DistHole_ch_sec_len_max->setValue(pFile->getValue(DISTHOLE_CH_SEC_LEN).toDouble());
		ui->DistHole_ch_sec_len_number->setValue(pFile->getValue(DISTHOLE_CH_SEC_LEN_NUMBER).toInt());

		//8 Â©Ó¡
		ui->MisPrint_enable->setChecked(pFile->getValue(MISPRINT_ENABLE).toBool());
		ui->MisPrint_area_max->setValue(pFile->getValue(MISPRINT_AREA_MAX).toDouble());
		ui->MisPrint_len_max->setValue(pFile->getValue(MISPRINT_LEN_MAX).toDouble());
		ui->MisPrint_ch_area_min->setValue(pFile->getValue(MISPRINT_CH_AREA_MIN).toDouble());
		ui->MisPrint_ch_area_max->setValue(pFile->getValue(MISPRINT_CH_AREA_MAX).toDouble());
		ui->MisPrint_ch_area_number->setValue(pFile->getValue(MISPRINT_CH_AREA_NUMBER).toInt());
		ui->MisPrint_ch_len_min->setValue(pFile->getValue(MISPRINT_CH_LEN_MIN).toDouble());
		ui->MisPrint_ch_len_max->setValue(pFile->getValue(MISPRINT_CH_LEN_MAX).toDouble());
		ui->MisPrint_ch_len_number->setValue(pFile->getValue(MISPRINT_CH_LEN_NUMBER).toInt());
		ui->MisPrint_ch_sec_area_max->setValue(pFile->getValue(MISPRINT_CH_SEC_AREA_MAX).toDouble());
		ui->MisPrint_ch_sec_area_number->setValue(pFile->getValue(MISPRINT_CH_SEC_AREA_NUMBER).toInt());
		ui->MisPrint_ch_sec_len_max->setValue(pFile->getValue(MISPRINT_CH_SEC_LEN).toDouble());
		ui->MisPrint_ch_sec_len_number->setValue(pFile->getValue(MISPRINT_CH_SEC_LEN_NUMBER).toInt());

		//9 ¹ÎÉË
		ui->Scratches_enable->setChecked(pFile->getValue(          SCRATCHES_ENABLE).toBool());
		ui->Scratches_area_max->setValue(pFile->getValue(          SCRATCHES_AREA_MAX).toDouble());
		ui->Scratches_len_max->setValue(pFile->getValue(           SCRATCHES_LEN_MAX).toDouble());
		ui->Scratches_ch_area_min->setValue(pFile->getValue(       SCRATCHES_CH_AREA_MIN).toDouble());
		ui->Scratches_ch_area_max->setValue(pFile->getValue(       SCRATCHES_CH_AREA_MAX).toDouble());
		ui->Scratches_ch_area_number->setValue(pFile->getValue(    SCRATCHES_CH_AREA_NUMBER).toInt());
		ui->Scratches_ch_len_min->setValue(pFile->getValue(        SCRATCHES_CH_LEN_MIN).toDouble());
		ui->Scratches_ch_len_max->setValue(pFile->getValue(        SCRATCHES_CH_LEN_MAX).toDouble());
		ui->Scratches_ch_len_number->setValue(pFile->getValue(     SCRATCHES_CH_LEN_NUMBER).toInt());
		ui->Scratches_ch_sec_area_max->setValue(pFile->getValue(   SCRATCHES_CH_SEC_AREA_MAX).toDouble());
		ui->Scratches_ch_sec_area_number->setValue(pFile->getValue(SCRATCHES_CH_SEC_AREA_NUMBER).toInt());
		ui->Scratches_ch_sec_len_max->setValue(pFile->getValue(    SCRATCHES_CH_SEC_LEN).toDouble());
		ui->Scratches_ch_sec_len_number->setValue(pFile->getValue( SCRATCHES_CH_SEC_LEN_NUMBER).toInt());

		//10±À±ß
		ui->Chips_enable->setChecked(pFile->getValue(           CHIPS_ENABLE).toBool());
		ui->Chips_area_max->setValue(pFile->getValue(           CHIPS_AREA_MAX).toDouble());
		ui->Chips_len_max->setValue(pFile->getValue(            CHIPS_LEN_MAX).toDouble());
		ui->Chips_ch_area_min->setValue(pFile->getValue(        CHIPS_CH_AREA_MIN).toDouble());
		ui->Chips_ch_area_max->setValue(pFile->getValue(        CHIPS_CH_AREA_MAX).toDouble());
		ui->Chips_ch_area_number->setValue(pFile->getValue(     CHIPS_CH_AREA_NUMBER).toInt());
		ui->Chips_ch_len_min->setValue(pFile->getValue(         CHIPS_CH_LEN_MIN).toDouble());
		ui->Chips_ch_len_max->setValue(pFile->getValue(         CHIPS_CH_LEN_MAX).toDouble());
		ui->Chips_ch_len_number->setValue(pFile->getValue(      CHIPS_CH_LEN_NUMBER).toInt());
		ui->Chips_ch_sec_area_max->setValue(pFile->getValue(    CHIPS_CH_SEC_AREA_MAX).toDouble());
		ui->Chips_ch_sec_area_number->setValue(pFile->getValue( CHIPS_CH_SEC_AREA_NUMBER).toInt());
		ui->Chips_ch_sec_len_max->setValue(pFile->getValue(     CHIPS_CH_SEC_LEN).toDouble());
		ui->Chips_ch_sec_len_number->setValue(pFile->getValue(  CHIPS_CH_SEC_LEN_NUMBER).toInt());

		//11°¼Í¹
		ui->Dent_enable->setChecked(pFile->getValue(          DENT_ENABLE).toBool());
		ui->Dent_area_max->setValue(pFile->getValue(          DENT_AREA_MAX).toDouble());
		ui->Dent_ch_area_min->setValue(pFile->getValue(       DENT_CH_AREA_MIN).toDouble());
		ui->Dent_ch_area_max->setValue(pFile->getValue(       DENT_CH_AREA_MAX).toDouble());
		ui->Dent_ch_area_number->setValue(pFile->getValue(    DENT_CH_AREA_NUMBER).toInt());
		ui->Dent_ch_sec_area_max->setValue(pFile->getValue(   DENT_CH_SEC_AREA_MAX).toDouble());
		ui->Dent_ch_sec_area_number->setValue(pFile->getValue(DENT_CH_SEC_AREA_NUMBER).toInt());

		ui->defect_max_number->setValue(pFile->getValue(DEFECT_MAX_NUMBER).toInt());
	}
}

void defectcfg::on_pushButton_save_clicked()
{

	IFileVariable *pFile = getDefectFileVariable();
	if (pFile)
	{
		//	1
		pFile->setValue(LIGHTLEAK_ENABLE, ui->LightLeak_enable->isChecked(), true);//
		pFile->setValue(LIGHTLEAK_AREA_MAX,           ui->LightLeak_area_max->value(), true);//
		pFile->setValue(LIGHTLEAK_LEN_MAX,            ui->LightLeak_len_max->value(), true);//
		pFile->setValue(LIGHTLEAK_CH_AREA_MIN,        ui->LightLeak_ch_area_min->value(), true);//
		pFile->setValue(LIGHTLEAK_CH_AREA_MAX,        ui->LightLeak_ch_area_max->value(), true);//
		pFile->setValue(LIGHTLEAK_CH_AREA_NUMBER,     ui->LightLeak_ch_area_number->value(), true);//
		pFile->setValue(LIGHTLEAK_CH_LEN_MIN,         ui->LightLeak_ch_len_min->value(), true);//
		pFile->setValue(LIGHTLEAK_CH_LEN_MAX,         ui->LightLeak_ch_len_max->value(), true);//
		pFile->setValue(LIGHTLEAK_CH_LEN_NUMBER,      ui->LightLeak_ch_len_number->value(), true);//
		pFile->setValue(LIGHTLEAK_CH_SEC_AREA_MAX,    ui->LightLeak_ch_sec_area_max->value(), true);//
		pFile->setValue(LIGHTLEAK_CH_SEC_AREA_NUMBER, ui->LightLeak_ch_sec_area_number->value(), true);//
		pFile->setValue(LIGHTLEAK_CH_SEC_LEN,         ui->LightLeak_ch_sec_len_max->value(), true);//
		pFile->setValue(LIGHTLEAK_CH_SEC_LEN_NUMBER,  ui->LightLeak_ch_sec_len_number->value(), true);//

		//2	
		pFile->setValue(EXCESSLNK_ENABLE, ui->ExcessLnk_enable->isChecked(), true);//
		pFile->setValue(EXCESSLNK_AREA_MAX,           ui->ExcessLnk_area_max->value(), true);//
		pFile->setValue(EXCESSLNK_LEN_MAX,            ui->ExcessLnk_len_max->value(), true);//
		pFile->setValue(EXCESSLNK_CH_AREA_MIN,        ui->ExcessLnk_ch_area_min->value(), true);//
		pFile->setValue(EXCESSLNK_CH_AREA_MAX,        ui->ExcessLnk_ch_area_max->value(), true);//
		pFile->setValue(EXCESSLNK_CH_AREA_NUMBER,     ui->ExcessLnk_ch_area_number->value(), true);//
		pFile->setValue(EXCESSLNK_CH_LEN_MIN,         ui->ExcessLnk_ch_len_min->value(), true);//
		pFile->setValue(EXCESSLNK_CH_LEN_MAX,         ui->ExcessLnk_ch_len_max->value(), true);//
		pFile->setValue(EXCESSLNK_CH_LEN_NUMBER,      ui->ExcessLnk_ch_len_number->value(), true);//
		pFile->setValue(EXCESSLNK_CH_SEC_AREA_MAX,    ui->ExcessLnk_ch_sec_area_max->value(), true);//
		pFile->setValue(EXCESSLNK_CH_SEC_AREA_NUMBER, ui->ExcessLnk_ch_sec_area_number->value(), true);//
		pFile->setValue(EXCESSLNK_CH_SEC_LEN,         ui->ExcessLnk_ch_sec_len_max->value(), true);//
		pFile->setValue(EXCESSLNK_CH_SEC_LEN_NUMBER,  ui->ExcessLnk_ch_sec_len_number->value(), true);//

		//3
		pFile->setValue(DISCOLOURATION_ENABLE, ui->Discolouration_enable->isChecked(), true);//
		pFile->setValue(DISCOLOURATION_AREA_MAX,           ui->Discolouration_area_max->value(), true);//
		pFile->setValue(DISCOLOURATION_LEN_MAX,            ui->Discolouration_len_max->value(), true);//
		pFile->setValue(DISCOLOURATION_CH_AREA_MIN,        ui->Discolouration_ch_area_min->value(), true);//
		pFile->setValue(DISCOLOURATION_CH_AREA_MAX,        ui->Discolouration_ch_area_max->value(), true);//
		pFile->setValue(DISCOLOURATION_CH_AREA_NUMBER,     ui->Discolouration_ch_area_number->value(), true);//
		pFile->setValue(DISCOLOURATION_CH_LEN_MIN,         ui->Discolouration_ch_len_min->value(), true);//
		pFile->setValue(DISCOLOURATION_CH_LEN_MAX,         ui->Discolouration_ch_len_max->value(), true);//
		pFile->setValue(DISCOLOURATION_CH_LEN_NUMBER,      ui->Discolouration_ch_len_number->value(), true);//
		pFile->setValue(DISCOLOURATION_CH_SEC_AREA_MAX,    ui->Discolouration_ch_sec_area_max->value(), true);//
		pFile->setValue(DISCOLOURATION_CH_SEC_AREA_NUMBER, ui->Discolouration_ch_sec_area_number->value(), true);//
		pFile->setValue(DISCOLOURATION_CH_SEC_LEN,         ui->Discolouration_ch_sec_len_max->value(), true);//
		pFile->setValue(DISCOLOURATION_CH_SEC_LEN_NUMBER,  ui->Discolouration_ch_sec_len_number->value(), true);//

	    //4 
		pFile->setValue(SAWEGDGE_ENABLE, ui->SawEgdge_enable->isChecked(), true);//
		pFile->setValue(SAWEGDGE_AREA_MAX,                  ui->SawEgdge_area_max->value(), true);//
		pFile->setValue(SAWEGDGE_LEN_MAX,                   ui->SawEgdge_len_max->value(), true);//
		pFile->setValue(SAWEGDGE_CH_AREA_MIN,               ui->SawEgdge_ch_area_min->value(), true);//
		pFile->setValue(SAWEGDGE_CH_AREA_MAX,               ui->SawEgdge_ch_area_max->value(), true);//
		pFile->setValue(SAWEGDGE_CH_AREA_NUMBER,            ui->SawEgdge_ch_area_number->value(), true);//
		pFile->setValue(SAWEGDGE_CH_LEN_MIN,                ui->SawEgdge_ch_len_min->value(), true);//
		pFile->setValue(SAWEGDGE_CH_LEN_MAX,                ui->SawEgdge_ch_len_max->value(), true);//
		pFile->setValue(SAWEGDGE_CH_LEN_NUMBER,             ui->SawEgdge_ch_len_number->value(), true);//
		pFile->setValue(SAWEGDGE_CH_SEC_AREA_MAX,           ui->SawEgdge_ch_sec_area_max->value(), true);//
		pFile->setValue(SAWEGDGE_CH_SEC_AREA_NUMBER,        ui->SawEgdge_ch_sec_area_number->value(), true);//
		pFile->setValue(SAWEGDGE_CH_SEC_LEN,                ui->SawEgdge_ch_sec_len_max->value(), true);//
		pFile->setValue(SAWEGDGE_CH_SEC_LEN_NUMBER,         ui->SawEgdge_ch_sec_len_number->value(), true);//

		//	5 ÔàÎÛ
		pFile->setValue(SMIRCH_ENABLE, ui->Smirch_enable->isChecked(), true);//
		pFile->setValue(SMIRCH_AREA_MAX, ui->Smirch_area_max->value(), true);//
		pFile->setValue(SMIRCH_LEN_MAX, ui->Smirch_len_max->value(), true);//
		pFile->setValue(SMIRCH_CH_AREA_MIN, ui->Smirch_ch_area_min->value(), true);//
		pFile->setValue(SMIRCH_CH_AREA_MAX, ui->Smirch_ch_area_max->value(), true);//
		pFile->setValue(SMIRCH_CH_AREA_NUMBER, ui->Smirch_ch_area_number->value(), true);//
		pFile->setValue(SMIRCH_CH_LEN_MIN, ui->Smirch_ch_len_min->value(), true);//
		pFile->setValue(SMIRCH_CH_LEN_MAX, ui->Smirch_ch_len_max->value(), true);//
		pFile->setValue(SMIRCH_CH_LEN_NUMBER, ui->Smirch_ch_len_number->value(), true);//
		pFile->setValue(SMIRCH_CH_SEC_AREA_MAX, ui->Smirch_ch_sec_area_max->value(), true);//
		pFile->setValue(SMIRCH_CH_SEC_AREA_NUMBER, ui->Smirch_ch_sec_area_number->value(), true);//
		pFile->setValue(SMIRCH_CH_SEC_LEN, ui->Smirch_ch_sec_len_max->value(), true);//
		pFile->setValue(SMIRCH_CH_SEC_LEN_NUMBER, ui->Smirch_ch_sec_len_number->value(), true);//

		//6
		pFile->setValue(ACCULNK_ENABLE, ui->AccuLnk_enable->isChecked(), true);//
		pFile->setValue(ACCULNK_AREA_MAX,              ui->AccuLnk_area_max->value(), true);//
		pFile->setValue(ACCULNK_LEN_MAX,               ui->AccuLnk_len_max->value(), true);//
		pFile->setValue(ACCULNK_CH_AREA_MIN,           ui->AccuLnk_ch_area_min->value(), true);//
		pFile->setValue(ACCULNK_CH_AREA_MAX,           ui->AccuLnk_ch_area_max->value(), true);//
		pFile->setValue(ACCULNK_CH_AREA_NUMBER,        ui->AccuLnk_ch_area_number->value(), true);//
		pFile->setValue(ACCULNK_CH_LEN_MIN,            ui->AccuLnk_ch_len_min->value(), true);//
		pFile->setValue(ACCULNK_CH_LEN_MAX,            ui->AccuLnk_ch_len_max->value(), true);//
		pFile->setValue(ACCULNK_CH_LEN_NUMBER,         ui->AccuLnk_ch_len_number->value(), true);//
		pFile->setValue(ACCULNK_CH_SEC_AREA_MAX,       ui->AccuLnk_ch_sec_area_max->value(), true);//
		pFile->setValue(ACCULNK_CH_SEC_AREA_NUMBER,    ui->AccuLnk_ch_sec_area_number->value(), true);//
		pFile->setValue(ACCULNK_CH_SEC_LEN,            ui->AccuLnk_ch_sec_len_max->value(), true);//
		pFile->setValue(ACCULNK_CH_SEC_LEN_NUMBER,     ui->AccuLnk_ch_sec_len_number->value(), true);//

        //7
		pFile->setValue(DISTHOLE_ENABLE, ui->DistHole_enable->isChecked(), true);//
		pFile->setValue(DISTHOLE_AREA_MAX,           ui->DistHole_area_max->value(), true);//
		pFile->setValue(DISTHOLE_LEN_MAX,            ui->DistHole_len_max->value(), true);//
		pFile->setValue(DISTHOLE_CH_AREA_MIN,        ui->DistHole_ch_area_min->value(), true);//
		pFile->setValue(DISTHOLE_CH_AREA_MAX,        ui->DistHole_ch_area_max->value(), true);//
		pFile->setValue(DISTHOLE_CH_AREA_NUMBER,     ui->DistHole_ch_area_number->value(), true);//
		pFile->setValue(DISTHOLE_CH_LEN_MIN,         ui->DistHole_ch_len_min->value(), true);//
		pFile->setValue(DISTHOLE_CH_LEN_MAX,         ui->DistHole_ch_len_max->value(), true);//
		pFile->setValue(DISTHOLE_CH_LEN_NUMBER,      ui->DistHole_ch_len_number->value(), true);//
		pFile->setValue(DISTHOLE_CH_SEC_AREA_MAX,    ui->DistHole_ch_sec_area_max->value(), true);//
		pFile->setValue(DISTHOLE_CH_SEC_AREA_NUMBER, ui->DistHole_ch_sec_area_number->value(), true);//
		pFile->setValue(DISTHOLE_CH_SEC_LEN,         ui->DistHole_ch_sec_len_max->value(), true);//
		pFile->setValue(DISTHOLE_CH_SEC_LEN_NUMBER,  ui->DistHole_ch_sec_len_number->value(), true);//

		//8
		pFile->setValue(MISPRINT_ENABLE, ui->MisPrint_enable->isChecked(), true);//
		pFile->setValue(MISPRINT_AREA_MAX,                  ui->MisPrint_area_max->value(), true);//
		pFile->setValue(MISPRINT_LEN_MAX,                   ui->MisPrint_len_max->value(), true);//
		pFile->setValue(MISPRINT_CH_AREA_MIN,               ui->MisPrint_ch_area_min->value(), true);//
		pFile->setValue(MISPRINT_CH_AREA_MAX,               ui->MisPrint_ch_area_max->value(), true);//
		pFile->setValue(MISPRINT_CH_AREA_NUMBER,            ui->MisPrint_ch_area_number->value(), true);//
		pFile->setValue(MISPRINT_CH_LEN_MIN,                ui->MisPrint_ch_len_min->value(), true);//
		pFile->setValue(MISPRINT_CH_LEN_MAX,                ui->MisPrint_ch_len_max->value(), true);//
		pFile->setValue(MISPRINT_CH_LEN_NUMBER,             ui->MisPrint_ch_len_number->value(), true);//
		pFile->setValue(MISPRINT_CH_SEC_AREA_MAX,           ui->MisPrint_ch_sec_area_max->value(), true);//
		pFile->setValue(MISPRINT_CH_SEC_AREA_NUMBER,        ui->MisPrint_ch_sec_area_number->value(), true);//
		pFile->setValue(MISPRINT_CH_SEC_LEN,                ui->MisPrint_ch_sec_len_max->value(), true);//
		pFile->setValue(MISPRINT_CH_SEC_LEN_NUMBER,         ui->MisPrint_ch_sec_len_number->value(), true);//
		

		//9¹ÎÉË
		pFile->setValue(SCRATCHES_ENABLE, ui->Scratches_enable->isChecked(), true);//
		pFile->setValue(SCRATCHES_AREA_MAX,           ui->Scratches_area_max->value(), true);//
		pFile->setValue(SCRATCHES_LEN_MAX,            ui->Scratches_len_max->value(), true);//
		pFile->setValue(SCRATCHES_CH_AREA_MIN,        ui->Scratches_ch_area_min->value(), true);//
		pFile->setValue(SCRATCHES_CH_AREA_MAX,        ui->Scratches_ch_area_max->value(), true);//
		pFile->setValue(SCRATCHES_CH_AREA_NUMBER,     ui->Scratches_ch_area_number->value(), true);//
		pFile->setValue(SCRATCHES_CH_LEN_MIN,         ui->Scratches_ch_len_min->value(), true);//
		pFile->setValue(SCRATCHES_CH_LEN_MAX,         ui->Scratches_ch_len_max->value(), true);//
		pFile->setValue(SCRATCHES_CH_LEN_NUMBER,      ui->Scratches_ch_len_number->value(), true);//
		pFile->setValue(SCRATCHES_CH_SEC_AREA_MAX,    ui->Scratches_ch_sec_area_max->value(), true);//
		pFile->setValue(SCRATCHES_CH_SEC_AREA_NUMBER, ui->Scratches_ch_sec_area_number->value(), true);//
		pFile->setValue(SCRATCHES_CH_SEC_LEN,         ui->Scratches_ch_sec_len_max->value(), true);//
		pFile->setValue(SCRATCHES_CH_SEC_LEN_NUMBER,  ui->Scratches_ch_sec_len_number->value(), true);//

	   //	10ÔàÎÛ
		pFile->setValue(CHIPS_ENABLE, ui->Chips_enable->isChecked(), true);//
		pFile->setValue(CHIPS_AREA_MAX,             ui->Chips_area_max->value(), true);//
		pFile->setValue(CHIPS_LEN_MAX,              ui->Chips_len_max->value(), true);//
		pFile->setValue(CHIPS_CH_AREA_MIN,          ui->Chips_ch_area_min->value(), true);//
		pFile->setValue(CHIPS_CH_AREA_MAX,          ui->Chips_ch_area_max->value(), true);//
		pFile->setValue(CHIPS_CH_AREA_NUMBER,       ui->Chips_ch_area_number->value(), true);//
		pFile->setValue(CHIPS_CH_LEN_MIN,           ui->Chips_ch_len_min->value(), true);//
		pFile->setValue(CHIPS_CH_LEN_MAX,           ui->Chips_ch_len_max->value(), true);//
		pFile->setValue(CHIPS_CH_LEN_NUMBER,        ui->Chips_ch_len_number->value(), true);//
		pFile->setValue(CHIPS_CH_SEC_AREA_MAX,      ui->Chips_ch_sec_area_max->value(), true);//
		pFile->setValue(CHIPS_CH_SEC_AREA_NUMBER,   ui->Chips_ch_sec_area_number->value(), true);//
		pFile->setValue(CHIPS_CH_SEC_LEN,           ui->Chips_ch_sec_len_max->value(), true);//
		pFile->setValue(CHIPS_CH_SEC_LEN_NUMBER,    ui->Chips_ch_sec_len_number->value(), true);//

		//	11°¼Í¹
		pFile->setValue(DENT_ENABLE, ui->Dent_enable->isChecked(), true);//
		pFile->setValue(DENT_AREA_MAX,           ui->Dent_area_max->value(), true);//
		pFile->setValue(DENT_CH_AREA_MIN,        ui->Dent_ch_area_min->value(), true);//
		pFile->setValue(DENT_CH_AREA_MAX,        ui->Dent_ch_area_max->value(), true);//
		pFile->setValue(DENT_CH_AREA_NUMBER,     ui->Dent_ch_area_number->value(), true);//
		pFile->setValue(DENT_CH_SEC_AREA_MAX,    ui->Dent_ch_sec_area_max->value(), true);//
		pFile->setValue(DENT_CH_SEC_AREA_NUMBER, ui->Dent_ch_sec_area_number->value(), true);//

		pFile->setValue(DEFECT_MAX_NUMBER, ui->defect_max_number->value(), true);//

		ui->pushButton_canle->setText(QString::fromLocal8Bit("ÐÞ¸Ä"));
		ui->pushButton_save->hide();
		setEnable(false);
		CSendMessage::GetInstance()->sendUpdateDefectPara();
	}

}

void defectcfg::on_pushButton_canle_clicked()
{
	if (QString::fromLocal8Bit("ÐÞ¸Ä") == ui->pushButton_canle->text())
	{
		ui->pushButton_save->show();
		setEnable(true);
		ui->pushButton_canle->setText(QString::fromLocal8Bit("È¡Ïû"));
	}
	else {
		updateUi();
		setEnable(false);
		ui->pushButton_save->hide();
		ui->pushButton_canle->setText(QString::fromLocal8Bit("ÐÞ¸Ä"));
		
	}
}

void defectcfg::slotChangePruduct()
{
	updateUi();
}

defectcfg* defectcfg::GetInstance(QWidget* pParant /*= NULL*/)
{
	if (NULL == m_pSelfInstance)
	{
		m_pSelfInstance = new defectcfg(pParant);
	}
	return m_pSelfInstance;
}

void defectcfg::ReleaseInstance()
{
	if (NULL != m_pSelfInstance)
	{

		delete m_pSelfInstance;
		m_pSelfInstance = NULL;
	}
}