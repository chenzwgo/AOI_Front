#include "customui/customwidget.h"
#include "motion/iioctrlinterface.h"
#include "motion/imotion.h"
#include "card/iaxis.h"
#include "callsdk.h"
#include "customui/qcylinder.h"
#include "customui/axisstatus.h"
#include <QMessageBox>
#include "customui/motionctldlg.h"
#include "customui/vacuum.h"
#include "customui/ioinput.h"
#include "customui/iooutput.h"
#include "customui/stationmove.h"
#include "customui/motionctldlg.h"
#include "customui/axisjog.h"
#include "customui/runflowbtn.h"
#include "customui/runflowdlg.h"
#include "customui/runpointbtn.h"

#include "movingdlg.h"
#include <QMetaMethod>
#include <QDebug>


CustomWidget::CustomWidget(QObject *parent) : QObject(parent),
    m_pStationMoveDlg(NULL)
{
    // 罗列parent下所有控件，选出自定义控件进行槽函数连接
	addCustomWidget(parent);
}

CustomWidget::~CustomWidget()
{
}


void CustomWidget::addCustomWidget( QObject *parent )
{
	if (NULL == parent)
	{
		return;
	}

	initWidgetConnect(parent);
	QObjectList childs = parent->children();
	if (childs.isEmpty())
	{
		return;
	}

	for (int i=0; i<childs.size(); i++)
	{
		addCustomWidget(childs.at(i));
	}
}

void CustomWidget::initWidgetConnect( QObject *pWidget )
{
	if (NULL == pWidget)
	{
		return;
	}

	QString className = pWidget->metaObject()->className();
	if (0 == className.compare("QCylinder"))
	{
		connect(pWidget, SIGNAL(clicked()), this, SLOT(slot_Cylinder_Open()));
		connect(pWidget, SIGNAL(sigUpdate()), this, SLOT(slot_Cylinder_Update()),Qt::DirectConnection);
	}
	else if (0 == className.compare("QAxisStatus"))
	{
		connect(pWidget, SIGNAL(sigAxisOn()), this, SLOT(slot_Axis_Enable()));
		connect(pWidget, SIGNAL(sigClearAlarm()), this, SLOT(slot_Axis_ClearAlarm()));
		connect(pWidget, SIGNAL(sigStop()), this, SLOT(slot_Axis_Stop()));
		connect(pWidget, SIGNAL(sigUpdate()), this, SLOT(slot_Axis_Update()),Qt::DirectConnection);
	}
	else if (0 == className.compare("Vacuum"))
	{
		connect(pWidget, SIGNAL(sigAbsorbVacuum()), this, SLOT(slot_Vacuum_Open()));
		connect(pWidget, SIGNAL(sigUpdate()), this, SLOT(slot_Vacuum_Update()),Qt::DirectConnection);
	}
	else if (0 == className.compare("IoInput"))
	{
		connect(pWidget, SIGNAL(sigUpdate()), this, SLOT(slot_ioInput_Update()),Qt::DirectConnection);
	}
	else if (0 == className.compare("IoOutPut"))
	{
		connect(pWidget, SIGNAL(clicked()), this, SLOT(slot_ioOutPut_Open()));
		connect(pWidget, SIGNAL(sigUpdate()), this, SLOT(slot_ioOutPut_Update()),Qt::DirectConnection);
	}
    else if (0 == className.compare("StationMove"))
    {
        connect(pWidget, SIGNAL(clicked()), this, SLOT(slot_station_move()));
    }
    else if (0 == className.compare("RunFlowBtn"))
    {
        connect(pWidget, SIGNAL(clicked()), this, SLOT(slot_runFlow()));
    }
    else if (0 == className.compare("AxisJog"))
    {
        connect(pWidget, SIGNAL(sigUpdate()), this, SLOT(slot_updateMovStatus()));
        connect(pWidget, SIGNAL(sigStop()), this, SLOT(slot_jogStop()));
        connect(pWidget, SIGNAL(sigMoveP()), this, SLOT(slot_jogMoveP()));
        connect(pWidget, SIGNAL(sigMoveN()), this, SLOT(slot_jogMoveN()));
    }
    else if (0 == className.compare("RunPointBtn"))
    {
        connect(pWidget, SIGNAL(clicked()), this, SLOT(slot_run_point()));
    }
	else
	{
		return;
	}
	
	m_objList.append(pWidget);
}


void CustomWidget::slot_Cylinder_Open()
{
	/* 用qobject_cast<QCylinder*>的方式，在dll中对sender()类转化会失败，不清楚原因 */ 
	QCylinder* pQCylinder = (QCylinder*)(sender());
    if(pQCylinder == NULL)
    {
        return;
    }

    bool bopen  = pQCylinder->isChecked() ;

    // 预处理
    if (!pQCylinder->slotPreProcess().isEmpty())
    {
        RETURN_IF(!pQCylinder->preProcess());
    }

    iMotion* piMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
    if (piMotion == NULL)
    {
        return ;
    }
    QString strCylinderOpenName  = pQCylinder->ioNameOpen();
    QString strCylinderCloseName  = pQCylinder->ioNameClose();
    IIOCtrlInterface* pIIOCtrlInterface = piMotion->GetIOCtrlInterface();
    if(pIIOCtrlInterface == NULL)
    {
        return ;
    }

    // 双向电磁阀,必须先关再开（先开在关影响寿命）
    if (bopen)
    {
        bool bflag = false;
        if(pQCylinder->isTwoWay())
        {
            bflag = pIIOCtrlInterface->writeio(strCylinderCloseName.toStdString().c_str(), false);
            if(!bflag)
            {
                // io操作失败
                QMessageBox::warning(NULL,"Warning","Write IO false");
                return;
            }
        }

        bflag = pIIOCtrlInterface->writeio(strCylinderOpenName.toStdString().c_str(),true);
        if(!bflag)
        {
            // io操作失败
            QMessageBox::warning(NULL,"Warning","Write IO false");
            return;

        }
    }
    else
    {
        bool bflag = pIIOCtrlInterface->writeio(strCylinderOpenName.toStdString().c_str(),false);
        if(!bflag)
        {
            // io操作失败
            QMessageBox::warning(NULL,"Warning","Write IO false");
            return;

        }

        if(pQCylinder->isTwoWay())
        {
            bflag = pIIOCtrlInterface->writeio(strCylinderCloseName.toStdString().c_str(),true);
            if(!bflag)
            {
                // io操作失败
                QMessageBox::warning(NULL,"Warning","Write IO false");
                return;
            }
        }
    }
}

void CustomWidget::slot_Cylinder_Update()
{

    QCylinder* pQCylinder = /*qobject_cast<QCylinder*>*/ (QCylinder*)(sender());
    if(pQCylinder == NULL)
    {
        return;
    }

    QString strCylinderOpenName = pQCylinder->ioNameOpen();
    QString strCylinderCloseName = pQCylinder->ioNameClose();
    QString strCylinderUP = pQCylinder->ioNameLimitP();
    QString strCylinderDown = pQCylinder->ioNameLimitN();
    iMotion* piMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
    if (piMotion == NULL)
    {
        return ;
    }
    IIOCtrlInterface* pIIOCtrlInterface = piMotion->GetIOCtrlInterface();
    if(pIIOCtrlInterface == NULL)
    {
        return ;
    }
    bool bflag  = false ; // for fix
    bool bopen = false;
    bflag = pIIOCtrlInterface->readio(strCylinderOpenName.toStdString().c_str(),bopen);
    if(!bflag)
    {
        //QMessageBox::warning(NULL,"Warning","Read IO false");
		return;
    }

    if(pQCylinder->isTwoWay())
    {
        bool bclose = false;
        bflag = pIIOCtrlInterface->readio(strCylinderCloseName.toStdString().c_str(),bclose);
        if(!bflag)
        {
            //QMessageBox::warning(NULL,"Warning","Read IO false");
            return;
        }
    }

    // 设置将Button的状态
    pQCylinder->setChecked(bopen);
    // for fix setButton(bopen);
    bflag = pIIOCtrlInterface->readio(strCylinderUP.toStdString().c_str(),bopen);
    if(!bflag)
    {
        //QMessageBox::warning(NULL,"Warning","Read IO false");
    }
    pQCylinder->setLimitPOn(bopen);

    bflag = pIIOCtrlInterface->readio(strCylinderDown.toStdString().c_str(),bopen);
    if(!bflag)
    {
       //QMessageBox::warning(NULL,"Warning","Read IO false");
    }
    pQCylinder->setLimitNOn(bopen);
}


void CustomWidget::slot_Axis_Enable()
{
    QAxisStatus* pAxisStatus = /*qobject_cast<QAxisStatus*>*/ (QAxisStatus*)(sender());
    if(pAxisStatus == NULL)
    {
        return;
    }
    QString strAixsName = pAxisStatus->axisName();
    if(strAixsName.isEmpty())
    {
        return;
    }

    iMotion* piMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
    if (piMotion == NULL)
    {
        return ;
    }
    IAxis* pAxis = piMotion->GetAxisInstance(strAixsName.toStdString().c_str());
    if(pAxis == NULL)
    {
        return ;
    }

    bool bopen= pAxis->IsOn();
    if(bopen)
    {
        pAxis->Off();
    }
    else
    {
        pAxis->On();
    }

	//MotionCtlDlg dlg;
	//dlg.exec();
}

void CustomWidget::slot_Axis_Stop()
{
    QAxisStatus* pAxisStatus = (QAxisStatus*)(sender());
    if(pAxisStatus == NULL)
    {
        return;
    }
    QString strAixsName = pAxisStatus->axisName();
    if(strAixsName.isEmpty())
    {
        return;
    }

    iMotion* piMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
    if (piMotion == NULL)
    {
        return ;
    }
    IAxis* pAxis = piMotion->GetAxisInstance(strAixsName.toStdString().c_str());
    if(pAxis == NULL)
    {
        return ;
    }
    bool bopen= pAxis->IsMoving();
    if(bopen)
    {
        pAxis->SmoothStop();
    }
}

void CustomWidget::slot_Axis_ClearAlarm()
{
    QAxisStatus* pAxisStatus = (QAxisStatus*)(sender());
    if(pAxisStatus == NULL)
    {
        return;
    }
    QString strAixsName = pAxisStatus->axisName();
    if(strAixsName.isEmpty())
    {
        return;
    }

    iMotion* piMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
    if (piMotion == NULL)
    {
        return ;
    }
    IAxis* pAxis = piMotion->GetAxisInstance(strAixsName.toStdString().c_str());
    if(pAxis == NULL)
    {
        return ;
    }
    pAxis->ClearStatus();
}


void CustomWidget::slot_Axis_Update()
{
    QAxisStatus* pAxisStatus = (QAxisStatus*)(sender());
    if(pAxisStatus == NULL)
    {
        return;
    }
    QString strAixsName = pAxisStatus->axisName();
    if(strAixsName.isEmpty())
    {
        return;
    }

    iMotion* piMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
    if (piMotion == NULL)
    {
        return ;
    }
    IAxis* pAxis = piMotion->GetAxisInstance(strAixsName.toStdString().c_str());
    if(pAxis == NULL)
    {
        return ;
    }
    pAxisStatus->setLimitPOn(pAxis->IsLimitP());
    pAxisStatus->setLimitNOn(pAxis->IsLimitN());
    pAxisStatus->setHomeOnOn(pAxis->IsHomeSig());
    pAxisStatus->setAlarmOn(pAxis->IsDrvAlarm());
    pAxisStatus->setPowerOn(pAxis->IsOn());
    pAxisStatus->setRuning(pAxis->IsMoving());

}

void CustomWidget::slot_Vacuum_Open()
{
	Vacuum* pVacuum = (Vacuum*)(sender());
	RETURN_IF(pVacuum == NULL);

    bool bopen  = pVacuum->isAbsorbVacuumChecked() ;

    // 预处理
    if (!pVacuum->slotPreProcess().isEmpty())
    {
        RETURN_IF(!pVacuum->preProcess());
    }

	iMotion* piMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
	RETURN_IF (piMotion == NULL);

	QString strIoNameAbsorb  = pVacuum->ioNameAbsorbVacuum();
    QString strIoNameBlow  = pVacuum->ioNameBlowVacuum();
	IIOCtrlInterface* pIIOCtrlInterface = piMotion->GetIOCtrlInterface();
	if(pIIOCtrlInterface == NULL)
	{
		return ;
	}

	bool bflag = pIIOCtrlInterface->writeio(strIoNameAbsorb.toStdString().c_str(),bopen);
	if(!bflag)
	{
		// io操作失败
		QMessageBox::warning(NULL,"Warning","Write IO false");
		return;
	}

    if (!strIoNameBlow.isEmpty())
    {
        bool bflag = pIIOCtrlInterface->writeio(strIoNameBlow.toStdString().c_str(),!bopen);
        if(!bflag)
        {
            // io操作失败
            QMessageBox::warning(NULL,"Warning","Write IO false");
            return;
        }
    }
}

void CustomWidget::slot_Vacuum_Update()
{
	Vacuum* pVacuum = (Vacuum*)(sender());
	if(pVacuum == NULL)
	{
		return;
	}

	QString strIoNameAbsorb = pVacuum->ioNameAbsorbVacuum();
	QString strIoNameBlow = pVacuum->ioNameBlowVacuum();
	QString strIoNameArrive = pVacuum->ioNameVacuumArrive();
	iMotion* piMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
	if (piMotion == NULL)
	{
		return ;
	}
	IIOCtrlInterface* pIIOCtrlInterface = piMotion->GetIOCtrlInterface();
	if(pIIOCtrlInterface == NULL)
	{
		return ;
	}
	bool bflag  = false ; // for fix
	bool bopen = false;
	bflag = pIIOCtrlInterface->readio(strIoNameAbsorb.toStdString().c_str(),bopen);
	if(!bflag)
	{
		return;
	}

	// 设置将Button的状态
	pVacuum->setAbsorbVacuumChecked(bopen);
	
	bflag = pIIOCtrlInterface->readio(strIoNameArrive.toStdString().c_str(),bopen);
	if(!bflag)
	{
		return;
	}

	pVacuum->setVacuumArrived(bopen);
}

void CustomWidget::slot_ioInput_Update()
{
	IoInput* pIoIn = (IoInput*)(sender());
	if(pIoIn == NULL)
	{
		return;
	}

	QString ioName = pIoIn->ioName();
	iMotion* piMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
	if (piMotion == NULL)
	{
		return ;
	}
	IIOCtrlInterface* pIIOCtrlInterface = piMotion->GetIOCtrlInterface();
	if(pIIOCtrlInterface == NULL)
	{
		return ;
	}
	bool bflag  = false ; // for fix
	bool bopen = false;
	bflag = pIIOCtrlInterface->readio(ioName.toStdString().c_str(),bopen);
	if(!bflag)
	{
		return;
	}

	// 设置将Button的状态
	pIoIn->setState(bopen);
}

void CustomWidget::slot_ioOutPut_Open()
{
	IoOutPut* pIoOut = (IoOutPut*)(sender());
	RETURN_IF(pIoOut == NULL);

    bool bopen = pIoOut->state();

    // 预处理
    if (!pIoOut->slotPreProcess().isEmpty())
    {
        RETURN_IF(!pIoOut->preProcess());
    }

	QString ioName = pIoOut->ioName();
	iMotion* pMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
	RETURN_IF (pMotion == NULL);

	IIOCtrlInterface* pIIOCtrlInterface = pMotion->GetIOCtrlInterface();
	RETURN_IF (pIIOCtrlInterface == NULL)

	bool bflag  = false ; // for fix
	bflag = pIIOCtrlInterface->writeio(ioName.toStdString().c_str(),bopen);
	if(!bflag)
	{
		// io操作失败
		QMessageBox::warning(NULL,"Warning","Write IO false");
		return;
	}
}

void CustomWidget::slot_ioOutPut_Update()
{
	IoOutPut* pIoOut = (IoOutPut*)(sender());
	if(pIoOut == NULL)
	{
		return;
	}

	QString ioName = pIoOut->ioName();
	iMotion* piMotion= (iMotion*)GetSDKInstance("motion",iMotion::IDD_MOTION_MANAGE);
	if (piMotion == NULL)
	{
		return ;
	}
	IIOCtrlInterface* pIIOCtrlInterface = piMotion->GetIOCtrlInterface();
	if(pIIOCtrlInterface == NULL)
	{
		return ;
	}
	bool bflag  = false ; // for fix
	bool bopen = false;
	bflag = pIIOCtrlInterface->readio(ioName.toStdString().c_str(),bopen);
	if(!bflag)
	{
		return;
	}

	// 设置将Button的状态
	pIoOut->setState(bopen);
}


void CustomWidget::slot_station_move()
{
    StationMove* pMove = (StationMove*)(sender());
    if(pMove == NULL)
    {
        return;
    }

    QString stationName = pMove->staionName();
    QString mask = pMove->pointMask();

    if (NULL == m_pStationMoveDlg)
    {
        m_pStationMoveDlg = new MotionCtlDlg(qobject_cast<QWidget*>(this->parent()));
    }

    if (NULL != m_pStationMoveDlg)
    {
        m_pStationMoveDlg->setStation(stationName, mask);
        m_pStationMoveDlg->show();
    }
}


void CustomWidget::slot_run_point()
{
    RunPointBtn* pPointBtn = (RunPointBtn*)(sender());
    RETURN_IF(pPointBtn == NULL)

    // 预处理
    if (!pPointBtn->slotPreProcess().isEmpty())
    {
        RETURN_IF(!pPointBtn->preProcess());
    }

    if (pPointBtn->enablePrompt())
    {
        QString info = pPointBtn->prompt();
        int nRet = QMessageBox::warning((QWidget*)pPointBtn->parent(), "warn", info, QString::fromLocal8Bit("确定"), QString::fromLocal8Bit("取消"));
        if(0 != nRet)
        {
            return;
        }
    }

    QString stationName = pPointBtn->stationName();
    QString pointName = pPointBtn->pointName();
    if (stationName.isEmpty() || pointName.isEmpty())
    {
        QMessageBox::warning((QWidget*)pPointBtn->parent(), "warn", QString::fromLocal8Bit("工站点位名称未配置!"));
        return;
    }
    QString mask = pPointBtn->mask();
    bool bRelative = pPointBtn->relative();

    short nMask = 0;
    if(!mask.isEmpty())
    {
        QStringList axisPri = mask.split(',');
        if (axisPri.size() != 6)
        {
            QMessageBox::warning((QWidget*)pPointBtn->parent(), "warn", QString::fromLocal8Bit("工站的轴掩码设置错误!"));
        }

        if(1 == axisPri.at(0).toInt())
        {
            nMask |= Mask_X;
        }
        if(1 == axisPri.at(1).toInt())
        {
            nMask |= Mask_Y;
        }
        if(1 == axisPri.at(2).toInt())
        {
            nMask |= Mask_Z;
        }
        if(1 == axisPri.at(3).toInt())
        {
            nMask |= Mask_U;
        }
        if(1 == axisPri.at(4).toInt())
        {
            nMask |= Mask_V;
        }
        if(1 == axisPri.at(5).toInt())
        {
            nMask |= Mask_W;
        }
    }
    else
    {
        nMask = Mask_ALL;
    }

    MovingDlg dlg(stationName);
    dlg.MovePoint(pointName, nMask, !bRelative);
    dlg.exec();
}

void CustomWidget::updateWidget( QObject *pWidget )
{
	if (NULL == pWidget)
	{
		return;
	}

	int methodIndex = pWidget->metaObject()->indexOfSignal("sigUpdate()");
	if (-1 != methodIndex)
	{
		pWidget->metaObject()->method(methodIndex).invoke(pWidget,Qt::DirectConnection);
	}
}

void CustomWidget::updateAllWidget( )
{
	foreach(QObject* obj,m_objList)
	{
		updateWidget(obj);
	}
}

void CustomWidget::slot_jogMoveP()
{
    AxisJog* pMove = (AxisJog*)(sender());
    RETURN_IF(NULL == pMove);

    // 预处理
    if (!pMove->slotPreProcess().isEmpty())
    {
        RETURN_IF(!pMove->preProcess());
    }

    QString axisName = pMove->name();
    if (axisName.isEmpty())
    {
        QMessageBox::warning((QWidget*)pMove->parent(), "warn", QString::fromLocal8Bit("轴名称未配置!"));
        return;
    }

    iMotion* pMotion = (iMotion *)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);;
    RETURN_IF (NULL == pMotion)

    IAxis *pAxis = pMotion->GetAxisInstance(axisName.toStdString().c_str());
    RETURN_IF (NULL == pAxis)

    pAxis->MoveJog(true);
}

void CustomWidget::slot_jogMoveN()
{
    AxisJog* pMove = (AxisJog*)(sender());
    RETURN_IF(NULL == pMove);

    // 预处理
    if (!pMove->slotPreProcess().isEmpty())
    {
        RETURN_IF(!pMove->preProcess());
    }

    QString axisName = pMove->name();
    if (axisName.isEmpty())
    {
        QMessageBox::warning((QWidget*)pMove->parent(), "warn", QString::fromLocal8Bit("轴名称未配置!"));
        return;
    }

    IStation *pAxisStation = NULL;
    iMotion* pMotion = (iMotion *)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);;
    RETURN_IF (NULL == pMotion);

    IAxis *pAxis = pMotion->GetAxisInstance(axisName.toStdString().c_str());
    RETURN_IF (NULL == pAxis)

    pAxis->MoveJog(false);
}

void CustomWidget::slot_jogStop()
{
    AxisJog* pMove = (AxisJog*)(sender());
    QString axisName = pMove->name();
    if (axisName.isEmpty())
    {
        QMessageBox::warning((QWidget*)pMove->parent(), "warn", QString::fromLocal8Bit("轴名称未配置!"));
        return;
    }

    IStation *pAxisStation = NULL;
    iMotion* pMotion = (iMotion *)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);;
    if (NULL == pMotion)
    {
        return ;   
    }
    IAxis *pAxis = pMotion->GetAxisInstance(axisName.toStdString().c_str());
    if (NULL == pAxis)
    {
        return ;   
    }
    pAxis->EStop();
}

void CustomWidget::slot_updateMovStatus()
{
    AxisJog* pMove = (AxisJog*)(sender());
    QString axisName = pMove->name();

    IStation *pAxisStation = NULL;
    iMotion* pMotion = (iMotion *)GetSDKInstance("motion", iMotion::IDD_MOTION_MANAGE);;
    if (NULL == pMotion)
    {
        return ;   
    }
    IAxis *pAxis = pMotion->GetAxisInstance(axisName.toStdString().c_str());
    if (NULL == pAxis)
    {
        return ;   
    }
    bool bflag =  pAxis->IsMoving();
    pMove->setMoving(bflag);
}

void CustomWidget::slot_runFlow()
{
    RunFlowBtn* pFlowBtn = (RunFlowBtn*)(sender());
    RETURN_IF(pFlowBtn == NULL);

    // 预处理
    if (!pFlowBtn->slotPreProcess().isEmpty())
    {
        RETURN_IF(!pFlowBtn->preProcess());
    }

    if (pFlowBtn->enablePrompt())
    {
        QString info = pFlowBtn->prompt();
        int nRet = QMessageBox::warning((QWidget*)pFlowBtn->parent(), "warn", info, QString::fromLocal8Bit("确定"), QString::fromLocal8Bit("取消"));
        if(0 != nRet)
        {
            return;
        }
    }

    QString groupName = pFlowBtn->groupName();
    QString flowName = pFlowBtn->flowName();
    if (groupName.isEmpty() || flowName.isEmpty())
    {
        QMessageBox::warning((QWidget*)pFlowBtn->parent(), "warn", QString::fromLocal8Bit("组名或流程名称未配置!"));
        return;
    }

    QString beginStep = pFlowBtn->beginStep();
    QString endStep = pFlowBtn->endStep();

    RunFlowDlg runFlowDlg;
    runFlowDlg.runFlow(groupName, flowName, beginStep, endStep);
    runFlowDlg.exec();
}
