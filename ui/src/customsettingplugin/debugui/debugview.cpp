#include "debugview.h"
#include "ui_debugview.h"
#include <QMessageBox>
#include <QTimer>
#include "customwidget.h"
#include "station/istation.h"
#include "motion/imotion.h"
#include "callsdk.h"
#include "rtdefine.h"
#include "movingdlg.h"
#include "modbuscom.h"
#include "configui/configoper.h"
#include "g12globalenv.h"
#include "card/icard.h"
#include "maindefine.h"
#include "g12utils.h"

#define ON_LIGHT  ":/mainui/image/greenCircleSmall.png"
#define OFF_LIGHT ":/mainui/image/redCircleSmall.png"

#define IO_ON_IMG  ":/mainui/image/IoOn.png"
#define IO_OFF_IMG ":/mainui/image/IoOff.png"

// #define LIGHT_STROBOFLASH_ON_ON_ADDR   2025   //使能频闪
//#define LIGHT_CLEAR_ADDR   2026               //清计数
//#define LIGHT_MODE_ADDR   2027                //模式
//#define LIGHT_MAIN_ADDR   2028                //上下光源
//#define LIGHT_SAVE_ADDR   2029                //保存设置
DebugView::DebugView(QFrame *parent) :
    QFrame(parent),
    ui(new Ui::DebugView),
	m_pCustomWidget(NULL),
    m_pLight(NULL)
{
	m_pMotion = NULL;
    
    m_pTarget = NULL;
    ui->setupUi(this);
	m_pTimer = new QTimer(this);
	m_pCustomWidget = new CustomWidget(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(slotTimerEvent()));
	m_pMotion =  (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);

    m_pTarget   =  m_pMotion->GetStationInstance("Target");

	initUi();
	//
    
}

DebugView::~DebugView()
{
	if (NULL != m_pCustomWidget)
	{
		delete m_pCustomWidget;
		m_pCustomWidget = NULL;
	}

    if (NULL != m_pTimer)
    {
        delete m_pTimer;
    }
    delete ui;
}

void DebugView::showEvent( QShowEvent *event )
{
	Q_UNUSED(event);
    if (NULL != m_pTimer)
    {
        m_pTimer->start();
    }
	initUi();//重新读一次光源设置//
}

void DebugView::hideEvent( QHideEvent *event )
{
	Q_UNUSED(event);
	if (NULL != m_pTimer)
	{
		m_pTimer->stop();
	}	
}

void DebugView::slotTimerEvent()
{
	if (NULL != m_pCustomWidget)
	{
		m_pCustomWidget->updateAllWidget();
	}	
}

void DebugView::slotTestCylinder(bool &bOk)
{
    bOk = (QMessageBox::Yes == QMessageBox::warning(this, "warn", QString::fromLocal8Bit("确定要打开气缸吗?"), QMessageBox::Yes | QMessageBox::No));
}

void DebugView::slotTestOutPut( bool &bOk )
{
    bOk = (QMessageBox::Yes == QMessageBox::warning(this, "warn", QString::fromLocal8Bit("确定要打开IO吗吗?"), QMessageBox::Yes | QMessageBox::No));}

void DebugView::slotTestVacuum( bool &bOk )
{
    bOk = (QMessageBox::Yes == QMessageBox::warning(this, "warn", QString::fromLocal8Bit("确定要打开真空吗?"), QMessageBox::Yes | QMessageBox::No));}

void DebugView::slotTestRunPoint( bool &bOk )
{
    bOk = (QMessageBox::Yes == QMessageBox::warning(this, "warn", QString::fromLocal8Bit("确定要跑点吗?"), QMessageBox::Yes | QMessageBox::No));
}

void DebugView::slotTestPreProcess( bool &bOk )
{
    bOk = (QMessageBox::Yes == QMessageBox::warning(this, "warn", QString::fromLocal8Bit("预处理?"), QMessageBox::Yes | QMessageBox::No));
}

void DebugView::LightTypeSet( QString strLightType )
{
	//if(NULL != m_pLight)
	//{
	//	IllumChannelWdgt* pCl = NULL;
	//	pCl = m_pLight->getLightClsssP(strLightType);
	//	if(NULL !=pCl)
	//	{
	//		pCl->on_pushButton_send_clicked();
	//	}
	//}
}

void DebugView::SetLightInstance( IlluminationControlDlg* pInt )
{
	m_pLight  = pInt;
}

void DebugView::on_pushButton_clear_clicked()
{

//	if (!lightcontrol::GetInstance()->ClearNumber())
//	{
//		QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("清计数失败失败！"));
//	}
	//

}

bool DebugView::intCom()
{
	//ComConfig	m_stLightCom;
	//m_stLightCom = *CONFIG_INSTANCE->GetLightCom();
	//QString strCom = QString::fromStdString(m_stLightCom.name);
	//RETURN_V_IF(!CModbusCom::GetInstance()->init(strCom,m_stLightCom.baudRate,m_stLightCom.parity,m_stLightCom.byteSize,m_stLightCom.stopBits),false);
	//RETURN_V_IF(!CModbusCom::GetInstance()->isOpen(),false);
	return  true;
}

//void DebugView::on_checkBox_A_clicked( bool checked )
//{
//	//int ival = checked ? 1:0;
//    //if (!WriteCheck(1,LIGHT_A_ON_ADDR,ival))
//    if (lightcontrol::GetInstance()->setChannelPowerOn(UP_LIGHT_COM, LIGHT_INDEX_A, checked))
//    {
//        ui->checkBox_A->setChecked(!checked);
//        QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("写入失败！"));
//    }
//}

//bool DebugView::WriteCheck( int iPro,int iAddr,int ival )
//{
//	//if (!intCom())
//	//{
//	//	QMessageBox::about(this, "错误", "串口打开失败！");
//	//	return false;
//	//}
//    QString strCom = "";
//    eModbusComMgr idd ;
//    idd = UP_LIGHT_COM;
//    if (2 == iPro)
//    {
//        idd = DOWN_LIGHT_COM;
//    }
//   // CModbusCom* pCModbusCom  = CModbusComMgr::GetInstance()->getCModbusCom(idd,strCom);
//   // if(NULL != pCModbusCom)
//    //{
//	//   return pCModbusCom->writeOneLight(1,iAddr,ival,500);
//   // }
//    //QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("串口打开失败！"));
//    return lightcontrol::GetInstance()->writeOneLight(idd, 1, iAddr, ival, 500);
//}
//
//void DebugView::on_checkBox_B_clicked( bool checked )
//{
//	int ival = checked ? 1:0;
//    //if (!WriteCheck(1,LIGHT_B_ON_ADDR,ival))
//    if (lightcontrol::GetInstance()->setChannelPowerOn(UP_LIGHT_COM, LIGHT_INDEX_B, checked))
//    {
//        ui->checkBox_B->setChecked(!checked);
//        QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("写入失败！"));
//    }
//}
//
//void DebugView::on_checkBox_C_clicked( bool checked )
//{
//	int ival = checked ? 1:0;
//    //if (!WriteCheck(1,LIGHT_C_ON_ADDR,ival)
//    if (lightcontrol::GetInstance()->setChannelPowerOn(UP_LIGHT_COM, LIGHT_INDEX_C, checked))
//    {
//        ui->checkBox_C->setChecked(!checked);
//        QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("写入失败！"));
//    }
//}
//
//void DebugView::on_checkBox_D_clicked( bool checked )
//{
//	int ival = checked ? 1:0;
//    //if (!WriteCheck(1,LIGHT_D_ON_ADDR,ival))
//    if (lightcontrol::GetInstance()->setChannelPowerOn(UP_LIGHT_COM, LIGHT_INDEX_D, checked))
//    {
//        ui->checkBox_D->setChecked(!checked);
//        QMessageBox::about(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("写入失败！"));
//    }
//}

//bool DebugView::ReadLightData(int iPro,int iAddr,unsigned short int RegCount,short int *list)
//{
//
//    QString strCom = "";
//    eModbusComMgr idd ;
//    idd = UP_LIGHT_COM;
//    if (2 == iPro)
//    {
//        idd = DOWN_LIGHT_COM;
//    }
//	//CModbusCom* pCModbusCom  = CModbusComMgr::GetInstance()->getCModbusCom(CModbusComMgr::UP_LIGHT_COM,strCom);
//	//if(NULL != pCModbusCom)
//	//{
//	//	return pCModbusCom->readMultiRegD( 1,iAddr,RegCount,list);
//	//}
//	return lightcontrol::GetInstance()->readMultiRegD(idd,1, iAddr, RegCount, list);
//}

void DebugView::initUi()
{ 
	//// 使用频闪  --  清除计数 -- 模式 
	////int iPro = 1;
	////int iAddr = LIGHT_STROBOFLASH_ON_ADDR;
	////int isize  = 3;
	////short sData[3] = {0,0,0};
 ////   short sDataD[3] = {0,0,0};
	////ReadLightData(1,iAddr,isize,sData);
	////ReadLightData(2,iAddr,isize,sDataD);
 //   bool bOn = false;// ((1 == sData[0]) && (1 == sDataD[0])) ? true : false;
 //   bOn = lightcontrol::GetInstance()->isLightOn();
 //   ui->checkBox_stroboflash_on->setChecked(bOn);
 //   //ui->checkBox_stroboflash_ban->setChecked(!bOn);
 //   //ui->checkBox_stroboflash->setChecked(1 == sData[2]?true:false);
	////ui->checkBox_allwayok->setChecked(0 == sData[02]?true:false);
 //   /*ui->label_mode->setText((1 == sData[2])?"频闪":"常亮");*/
 //   

	////
	//// 常亮模式 亮度- 4个 使用 4个
 //   bool bChecked = false;
 //   bChecked = lightcontrol::GetInstance()->isChannelPowerOn(UP_LIGHT_COM, 0);
 //   ui->checkBox_A->setChecked(bChecked);
 //   bChecked = lightcontrol::GetInstance()->isChannelPowerOn(UP_LIGHT_COM, 1);
 //   ui->checkBox_B->setChecked(bChecked);
 //   bChecked = lightcontrol::GetInstance()->isChannelPowerOn(UP_LIGHT_COM, 2);
 //   ui->checkBox_C->setChecked(bChecked);
 //   bChecked = lightcontrol::GetInstance()->isChannelPowerOn(UP_LIGHT_COM, 3);
	//ui->checkBox_D->setChecked(bChecked);

 //   bChecked = lightcontrol::GetInstance()->isChannelPowerOn(DOWN_LIGHT_COM, 0);
 //   ui->checkBox_B_R->setChecked(bChecked);
 //   bChecked = lightcontrol::GetInstance()->isChannelPowerOn(DOWN_LIGHT_COM, 1);
 //   ui->checkBox_C_R->setChecked(bChecked);
 //   bChecked = lightcontrol::GetInstance()->isChannelPowerOn(DOWN_LIGHT_COM, 2);
 //   ui->checkBox_D_R->setChecked(bChecked);

	//setEnale(bOn);
	update();
}

void DebugView::on_pushButton_test_clicked( bool checked )
{
    int i = ui->spinBox_times->value();
    int sleepTime = ui->spinBox_sleep->value();
   iMotion*          m_pMotion;
   ICard *           m_pCard;
   ICardContInterp*  m_pCardConInterp;
   m_pMotion = (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
   RETURN_IF(NULL == m_pMotion);
   m_pCard = (ICard *)m_pMotion->GetCardInstance(CONTROLCARD_NAME);
   RETURN_IF(NULL == m_pCard);
   m_pCardConInterp = dynamic_cast<ICardContInterp*>(m_pCard);
   RETURN_IF(NULL == m_pCardConInterp);

   iMotionCfg *pMotionCfg = (iMotionCfg*)GetSDKInstance("motion", iMotionCfg::IID_MOTION_CONFIG);
   RETURN_IF(NULL == pMotionCfg);
   StCardInfo cardInfo;
   pMotionCfg->getCardInfo("card", cardInfo);
   bool bIsGGCard = false;
   if (0 == strcmp(cardInfo.cardType, "cardgg"))
   {
       bIsGGCard = true;
   }
   short sss = 0;
   long test[2] = { 0 };
   bool res = false;
   if (bIsGGCard)
   {
       do
       {
           m_pCardConInterp->SetComparePluse(2, 0, 100);
           usSleep(sleepTime * 1000);
           i--;
       } while (i > 0);

       res = m_pCardConInterp->GetCompareStatus(sss, test);
   }
   else
   {
       res = m_pCardConInterp->SetComparePluse(0, 1, i);
       short istu = 0;//触发是否完成 值1为完成 值 0为正在进行
       long  ldata[2] = { 0 };
       Sleep(1500);
       do
       {
           res = m_pCardConInterp->GetCompareStatus(istu, test);
       } while (istu != 1);
       m_pCardConInterp->stopComparePos(0);
   }

   QString strTile = res? QString::fromLocal8Bit("成功"): QString::fromLocal8Bit("错误");
    QString strLog = res?(QString::number(test[0]) + "  "+ QString::number(test[1])):"error";
    QMessageBox::about(this,strTile , strLog);
}

void DebugView::on_pushButton_calib_clicked( bool checked )
{

}

void DebugView::on_pushButton_signal_clicked(bool checked)
{
	int i = 1000;
	iMotion*          m_pMotion;
	ICard *           m_pCard;
	ICardContInterp*  m_pCardConInterp;
	m_pMotion = (iMotion*)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);
	RETURN_IF(NULL == m_pMotion);
	m_pCard = (ICard *)m_pMotion->GetCardInstance(CONTROLCARD_NAME);
	RETURN_IF(NULL == m_pCard);
	m_pCardConInterp = dynamic_cast<ICardContInterp*>(m_pCard);
	RETURN_IF(NULL == m_pCardConInterp);
    m_pCardConInterp->SetComparePluse(2, 0, 100);
    m_pCardConInterp->stopComparePos(0);
}

void DebugView::on_pushButton_calib_pluse_ratio_clicked(bool checked)
{

}

void DebugView::on_pushButton_svae_set_clicked()
{
    //lightcontrol::GetInstance()->saveParamToStorage();
}

bool DebugView::ClearLightNumber()
{

    //return lightcontrol::GetInstance()->ClearNumber();
    return true;
}

bool DebugView::ResetStroboflash()
{
	//关 频闪
   // lightcontrol::GetInstance()->WriteOn(false);

	Sleep(50);
    //开 //频闪 --操作模式  首先切到频闪模式 再开频闪使能
   // lightcontrol::GetInstance()->WriteOn(true);
	return true;
}




