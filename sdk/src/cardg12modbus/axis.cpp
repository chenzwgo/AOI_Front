#include "axis.h"
#include "math.h"
#include "rtdefine.h"
#include "modbuscom.h"
#include "converymodbus.h"

CAxis::CAxis(unsigned short cardIndex, unsigned short axisIndex)
    : m_cardIndex(cardIndex)
    , m_axisIndex(axisIndex)
    , m_bOn(false)
    , m_bInit(false)
    , m_pCom(NULL)
{
    string comName = getComName();
}

CAxis::~CAxis()
{
	while(IsMoving())
	{
		SmoothStop();
		Sleep(10);
	}

	m_bInit = false;
	Off();
}

bool CAxis::Init()
{
    RETURN_V_IF(m_cardIndex < 0 || m_cardIndex >= 8,false);
    RETURN_V_IF(m_axisIndex < 0 || m_axisIndex >= 8,false);

    setAlarmOff();	// �رձ���
    setAlarmOn();		// �򿪱���
	if (m_pAxisParam.limitAlarmConnect)
	{
		setLmtsOn();	// ����λ����
	}
	else
	{
		setLmtsOff();	// �ر���λ����
	}
	ClearStatus();	// �����״̬
    On();			// ������
    return true;
}

bool CAxis::On()
{
    RETURN_V_IF(IsOn(),true);
    m_bOn = true;
    return true;
}

bool CAxis::Off()
{
    RETURN_V_IF(!IsOn(),true);
    return true;
}

bool CAxis::SetAxisParam( StAxisParam* pAxisParam )
{
    RETURN_V_IF(!pAxisParam, false);
    m_pAxisParam = *pAxisParam;
    m_bInit = true;
    return true;
}

bool CAxis::GetAxisParam( StAxisParam* pAxisParam )
{
    RETURN_V_IF(NULL==pAxisParam, false);
    *pAxisParam = m_pAxisParam;
    return true;
}

bool CAxis::SetLmtValue( double limitP, double limitN )
{
    return true;
}

bool CAxis::GetLmtValue( double &limitP, double &limitN )
{
    return true;
}

bool CAxis::SetSpeed( const StSpeed &speed )
{
    m_stSpeed = speed;
    return true;
}

bool CAxis::GetSpeed( StSpeed &speed )
{
    RETURN_V_IF(!m_bInit, false);

    return true;
}

bool CAxis::MovePosRel( double pos )
{
    int error_num = -1;
    RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(IsMoving(), false);

	int mode = 0x00;
	if (pos >= 0)
	{
		mode = 0x02;
	}
	else
	{
		pos = -pos;
		mode = 0x03;
	}

	uint pulse = pos*m_pAxisParam.pulsepermm;

	RETURN_V_IF(!setPtpSpeed(m_stSpeed), false);

	ConveryModbus::GetInstance()->setMoveMode(mode);
	bool bRet = ConveryModbus::GetInstance()->setMovePulse(pulse);
	bRet = ConveryModbus::GetInstance()->startMove();
    return bRet;
}

bool CAxis::MovePosAbs( double pos )
{
	// ͳһʹ������˶�
	return MovePosRel(pos);
}

bool CAxis::MoveSucceed( double pos )
{
    if (IsMoving())
    {
        return false;
    }
    else
    {
        // ĳ���Ѿ�ֹͣ�˶��ˣ���ȡ��ĵ�ǰλ�ã��͹滮��λ�ñȽϣ� Ȼ�� �ټ����˶���ֵλ�ã�fix
        double curPos = -1;
        if (GetCurPos(curPos))
        {
            if (abs(curPos - pos) > 0.01) // 100 ����ֵ�ɶ���
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}

bool CAxis::GetCurPos( double &pos )
{
    if (POSMODE_COMMAND == m_pAxisParam.posMode)
    {
        return GetCommandPos(pos);
    }
    else
    {
        return GetActualPos(pos);
    }
}

bool CAxis::GetActualPos( double &pos )
{    
	uint encPos = 0;
	ConveryModbus::GetInstance()->getActualPos(encPos);
    pos = encPos/m_pAxisParam.pulsepermm;
    return true;
}

bool CAxis::GetCommandPos( double &pos )
{
	uint encPos = 0;
	ConveryModbus::GetInstance()->getActualPos(encPos);
    pos = encPos /m_pAxisParam.pulsepermm;
    return true;
}

bool CAxis::MoveJog( bool bDirection /*= true */ )
{
    RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(IsMoving(), false);
	if (m_pAxisParam.limitAlarmConnect)
	{
		if (bDirection)
		{
			RETURN_V_IF(IsLimitP(), false);
		}
		else
		{
			RETURN_V_IF(IsLimitN(), false);
		}
	}
	
	int mode = 0x00;
	if (bDirection)
	{
		mode = 0x00;
	}
	else
	{
		mode = 0x01;
	}

    // jogģʽ
	RETURN_V_IF(!setJogSpeed(m_stSpeed, bDirection), false);
	ConveryModbus::GetInstance()->setMoveMode(mode);
	bool bRet = ConveryModbus::GetInstance()->startMove();
    return bRet;
}

bool CAxis::EStop()
{
	bool bRet = ConveryModbus::GetInstance()->stopMove();
	return bRet;
}

bool CAxis::SmoothStop()
{
	bool bRet = ConveryModbus::GetInstance()->stopMove();
	return bRet;
}

bool CAxis::SetPosZero()
{
    return true;
}

bool CAxis::ClearStatus()
{
    return true;
}

bool CAxis::ClearAlarm()
{
	return true;
}

bool CAxis::IsMoving()
{
	bool bRet = ConveryModbus::GetInstance()->isMoving();
    return bRet;
}

bool CAxis::IsOn()
{
	return true;
}

bool CAxis::IsLimitP()
{
	return false;
}

bool CAxis::IsLimitN()
{
	return false;
}

bool CAxis::IsDrvAlarm()
{
	return false;
}

bool CAxis::IsHomeSig()
{
	return false;
}

bool CAxis::MoveToLimitN(bool bFindNLimit/* = true*/)
{
	RETURN_V_IF(!ClearStatus(),false);
	RETURN_V_IF(IsLimitN(), true);
	double curPos = 0.0;
	bool bFindHomeSignal = false;
	double dDis = m_pAxisParam.homePos*2;//������

	if (!MovePosRel(-abs(dDis)))
	{
		return false;
	}

	//Ѱ�Ҹ������ҵ����޻�ԭ���źż���
	while (true)	
	{
		if (!IsMoving() && !IsLimitN())
		{//������λ��Ҳ�����˶�
			return false;
		}

		if (IsLimitN())  //����λ��, ֹͣ�˶�
		{
			break;
		}
		//else if (!bFindNLimit && IsHomeSig())
		//{
		//	bFindHomeSignal = true;
		//	break;
		//}
		else 
			Sleep(10);
	}

	while (true)	//�ȴ��뿪ԭ���ź�
	{
		if (bFindHomeSignal)
		{
			if (!IsMoving())
			{
				return false;
			}

			//if (!IsHomeSig()) // �뿪ԭ��
			//{
			//	bFindHomeSignal = false;
			//	Sleep(50);
			//}
			else
				continue;
		}

		if (IsMoving())
		{
			SmoothStop();
			Sleep(100);
			continue;
		}

		RETURN_V_IF(!GetCurPos(curPos), SYS_ERR_CARD_CTRL_FAIL);
		if (curPos != 0)
		{
			RETURN_V_IF(!SetPosZero(),SYS_ERR_CARD_SET_ZERO);
			Sleep(200);
			continue;
		}

		return true;
	}

	return false;
}

bool CAxis::MoveToLimitP(bool bFindNLimit/* = true*/)
{
	RETURN_V_IF(!ClearStatus(),false);
	RETURN_V_IF(IsLimitP(), true);
	double curPos = 0.0;
	bool bFindHomeSignal = false;
	double dDis = m_pAxisParam.homePos*2;//������

	if (!MovePosRel(abs(dDis)))
	{
		return false;
	}

	//Ѱ�Ҹ������ҵ����޻�ԭ���źż���
	while (true)	
	{
		if (!IsMoving() && !IsLimitP())
		{//������λ��Ҳ�����˶�
			return false;
		}

		if (IsLimitP())  //����λ��, ֹͣ�˶�
		{
			break;
		}
		//else if (!bFindNLimit && IsHomeSig())
		//{
		//	bFindHomeSignal = true;
		//	break;
		//}
		else 
			Sleep(10);
	}

	while (true)	//�ȴ��뿪ԭ���ź�
	{
		if (bFindHomeSignal)
		{
			if (!IsMoving())
			{
				return false;
			}

			//if (!IsHomeSig()) // �뿪ԭ��
			//{
			//	bFindHomeSignal = false;
			//	Sleep(50);
			//}
			else
				continue;
		}

		if (IsMoving())
		{
			SmoothStop();
			Sleep(100);
			continue;
		}

		RETURN_V_IF(!GetCurPos(curPos), SYS_ERR_CARD_CTRL_FAIL);
		if (curPos != 0)
		{
			RETURN_V_IF(!SetPosZero(),SYS_ERR_CARD_SET_ZERO);
			Sleep(200);
			continue;
		}

		return true;
	}

	return false;
}

bool CAxis::Home()
{
	m_bHomeResult = MOVE_TRUE;
	//m_bHomeResult = MOVE_DOING;
	//if (!m_hThreadHome->IsRunning())
	//{	
	//	m_hThreadHome->StartThread(this,&CAxis::homeThread,this);
	//}
 //   
    return true;
}

EnMoveBool CAxis::HomeStatus()
{
    return m_bHomeResult;
}


int CAxis::homeFun()
{
	return SYS_ERR_CARD_HOME_MODE;
}

void* CAxis::homeThread( void* param )
{
    int nRet = homeFun();
    if (SYS_ERR_OK == nRet)
    {
        m_bHomeResult = MOVE_TRUE;
    }
    else
    {
        m_bHomeResult = MOVE_FALSE;
    }
    return NULL;
}

bool CAxis::index()
{
    return true;
}

bool CAxis::homeSts( unsigned short *pStatus )
{
    return true;
}

bool CAxis::setVel( double vel )
{
     return true;
}

bool CAxis::getVel( double &vel )
{
    return true;
}

bool CAxis::setEncSns( int dir )
{
    return true;
}

bool CAxis::setCtrlMode( unsigned short mode )
{
    return true;
}

bool CAxis::setStepDir()
{
    return true;
}

bool CAxis::setEncOn()
{
    return true;
}

bool CAxis::setEncOff()
{
    return true;
}

bool CAxis::setAlarmOff()
{
    return true;
}

bool CAxis::setLmtsOff()
{
    return true;
}

bool CAxis::setAlarmOn()
{
    return true;
}

bool CAxis::setLmtsOn()
{
    return true;
}

bool CAxis::setPtpSpeed( StSpeed &speed )
{
    RETURN_V_IF(!m_bInit, false);
    RETURN_V_IF(speed.vel<0.000001, false);

	uint acc = speed.acc*m_pAxisParam.pulsepermm;
	uint dec = speed.dec*m_pAxisParam.pulsepermm;
	uint velStart = speed.velStart*m_pAxisParam.pulsepermm;
	uint vel = speed.vel*m_pAxisParam.pulsepermm;

	bool bRet = ConveryModbus::GetInstance()->setVel(vel);
	//bRet = ConveryModbus::GetInstance()->setAcc(acc);
	//bRet = ConveryModbus::GetInstance()->setDec(dec);
	//bRet = ConveryModbus::GetInstance()->setBeginVel(velStart);
    return bRet;
}

bool CAxis::setJogSpeed( StSpeed &speed, bool bDirect )
{
    RETURN_V_IF(!m_bInit, false);
    RETURN_V_IF(speed.vel<0.000001, false);

	uint acc = speed.acc*m_pAxisParam.pulsepermm;
	uint dec = speed.dec*m_pAxisParam.pulsepermm;
	uint velStart = speed.velStart*m_pAxisParam.pulsepermm;
	uint vel = speed.vel*m_pAxisParam.pulsepermm;

	bool bRet = ConveryModbus::GetInstance()->setVel(vel);
	//bRet = ConveryModbus::GetInstance()->setAcc(acc);
	//bRet = ConveryModbus::GetInstance()->setDec(dec);
	//bRet = ConveryModbus::GetInstance()->setBeginVel(velStart);
	return true;
}

int CAxis::ggHome()
{
	return SYS_ERR_OK;
}

bool CAxis::MoveToLimitNUserDefine()
{
	RETURN_V_IF(!ClearStatus(),false);
	RETURN_V_IF(IsLimitN() && !IsHomeSig(), true);//�ڸ���λ��ͬʱ����ԭ����, ֱ�Ӳ��Ҹ�λ�ź�

	long dDis = m_pAxisParam.homePos*2;    //������
	if (!MovePosRel(-abs(dDis)))
	{
		return false;
	}

	while (true)	//�ȴ��ƶ�������λ
	{
		if (!IsMoving() && !(IsLimitN() && !IsHomeSig()))
		{//������λ��Ҳ�����˶�
			return false;
		}

		if (IsLimitN() && !IsHomeSig())  //����λ��, ֹͣ�˶�
		{
			if (IsMoving())
			{
				SmoothStop();
				Sleep(10);
				continue;
			}

			
			break;
		}
		Sleep(10);
	}

	return SetPosZero();
}

bool CAxis::MoveToLimitPUserDefine()
{
	RETURN_V_IF(!ClearStatus(),false);
	RETURN_V_IF(IsLimitP() && !IsHomeSig(), true); //����λ��ͬʱ����ԭ��, Ҳֱ�Ӳ��Ҹ�λ�ź�

	double curPos = 0.0;
	long dDis = m_pAxisParam.homePos*2;    //������
	if (!MovePosRel(abs(dDis)))
	{
		return false;
	}

	while (true)    //�ȴ��ƶ�������λ
	{
		if (!IsMoving() && !(IsLimitP() && !IsHomeSig()))
		{//������λ��Ҳ�����˶�
			return false;
		}

		if (IsLimitP() && !IsHomeSig())  // ����λ��, ֹͣ�˶�
		{
			if (IsMoving())
			{
				SmoothStop();
				Sleep(10);
				continue;
			}

			RETURN_V_IF(!GetCurPos(curPos), SYS_ERR_CARD_CTRL_FAIL);
			if (curPos != 0)
			{
				RETURN_V_IF(!SetPosZero(),SYS_ERR_CARD_SET_ZERO);
				Sleep(200);
				continue;
			}

			Sleep(500);
			break;
		}
		Sleep(10);
	}
	return SetPosZero();
}

bool CAxis::MoveToLimitNProcess(bool bFindNLimit/* = true*/)
{
	if (m_pAxisParam.homeMode == HOMEMODE_USER_DEFINE)
	{
		return MoveToLimitNUserDefine();
	} 
	else
	{
		return MoveToLimitN(bFindNLimit);
	}
}

bool CAxis::MoveToLimitPProcess(bool bFindNLimit/* = true*/)
{
	if (m_pAxisParam.homeMode == HOMEMODE_USER_DEFINE)
	{
		return MoveToLimitPUserDefine();
	} 
	else
	{
		return MoveToLimitP(bFindNLimit);
	}
}

bool CAxis::SetPos(EnPosMode model, double dValue)
{
	return true;
}

bool CAxis::LeaveLimit( int iSignal )
{
	return false;
}

int CAxis::HomeLimitNProcess()
{
	return SYS_ERR_OK;
}

int CAxis::HomeLimitPProcess()
{
	return SYS_ERR_OK;
}

int CAxis::HomeOriginProcess(EnHomeMode homemode)
{
	return ggUserHome();
}

bool CAxis::LimitMoveOffset()
{
	double curPos = 0.0;
	double dOffset = 0.0;
	RETURN_V_IF(!(HOMEMODE_NLIMIT == m_pAxisParam.homeMode || HOMEMODE_PLIMIT == m_pAxisParam.homeMode), true);
	RETURN_V_IF(0 == m_pAxisParam.homeOffset, true);

	if (HOMEMODE_NLIMIT == m_pAxisParam.homeMode)
	{
		dOffset = abs(m_pAxisParam.homeOffset);
	} 
	else
	{
		dOffset = -abs(m_pAxisParam.homeOffset);
	}
	RETURN_V_IF(!MovePosRel(dOffset),SYS_ERR_CARD_CTRL_FAIL);
	while (true)
	{
		if (IsMoving())
		{
			Sleep(10);
			continue;
		}

		RETURN_V_IF(!GetCurPos(curPos), false);
		if (curPos != 0)
		{
			RETURN_V_IF(!SetPosZero(),false);
			Sleep(200);
			continue;
		}

		Sleep(500);
		break;
	}

	RETURN_V_IF(!ClearStatus(),false);
	RETURN_V_IF(!SetPosZero(),false);
	return true;
}

string CAxis::getComName()
{
    return string();
}

int CAxis::ggUserHome()
{
	return SYS_ERR_OK;
}

int CAxis::HomeIndexProcess()
{
	return SYS_ERR_OK;
}
