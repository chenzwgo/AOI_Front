#include "gts.h"
#include "axis.h"
#include "math.h"
#include "rtdefine.h"

CAxis::CAxis(unsigned short cardIndex, unsigned short axisIndex)
	: m_bHomeResult(MOVE_FALSE)
{
    m_cardIndex = cardIndex;
    m_axisIndex = axisIndex;
    m_bOn = false;
    m_bInit = false;

    m_hThreadHome = new JThreadTP<CAxis, void *, void *>;
}

CAxis::~CAxis()
{
	while(IsMoving())
	{
		SmoothStop();
		Sleep(10);
	}

	m_bInit = false;
	while(m_hThreadHome && m_hThreadHome->IsRunning())
		Sleep(50);
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
	GT_AxisOn(m_cardIndex, m_axisIndex);;			// ������
    return true;
}

bool CAxis::On()
{
    RETURN_V_IF(IsOn(),true);
	RETURN_V_IF(!m_pAxisParam.servOnConnect,true);
    int error_num = -1;
    error_num = GT_AxisOn(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    m_bOn = true;
    return true;
}

bool CAxis::Off()
{
    RETURN_V_IF(!IsOn(),true);
    RETURN_V_IF(!EStop(),false);
	RETURN_V_IF(!m_pAxisParam.servOnConnect,true);
    int error_num = -1;
    error_num = GT_AxisOff(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    m_bOn = false;
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
    long pluseLimitP = limitP*m_pAxisParam.pulsepermm;
    long pluseLimitN = limitN*m_pAxisParam.pulsepermm;
    int error_num = GT_SetSoftLimit(m_cardIndex, m_axisIndex,pluseLimitP,pluseLimitN);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::GetLmtValue( double &limitP, double &limitN )
{
    long pluseLimitP = 0;
    long pluseLimitN = 0;
    int error_num = GT_GetSoftLimit(m_cardIndex, m_axisIndex,&pluseLimitP,&pluseLimitN);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    limitP = pluseLimitP/m_pAxisParam.pulsepermm;
    limitN = pluseLimitN/m_pAxisParam.pulsepermm;
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
    RETURN_V_IF(speed.vel<0.000001, false);

    int mode = getPrfMode();
    if (mode == 0) // ptp
    {
		TTrapPrm param;
		if(!getTrapPrm(param))
		{
			return false;
		}

		speed.acc = param.acc/m_pAxisParam.pulsepermm*1000*1000;
		speed.dec = param.dec/m_pAxisParam.pulsepermm*1000*1000;
		speed.smoothTime = param.smoothTime / m_pAxisParam.pulsepermm;
		speed.velStart = param.velStart/m_pAxisParam.pulsepermm*1000;
	}
    else if (mode == 1) //jog
    {
		TJogPrm param;
		if(!getJogprm(param))
		{
			return false;
		}

		speed.acc = param.acc/m_pAxisParam.pulsepermm*1000*1000;
		speed.dec = param.dec/m_pAxisParam.pulsepermm*1000*1000;
		speed.smoothTime = param.smooth / m_pAxisParam.pulsepermm;

		double dVel = 0.0;
		int error_num = GT_GetAxisPrfVel(m_cardIndex, m_axisIndex, &dVel);
		if (GTS_Common::GTS_SUCCESS != error_num)
		{
			GTS_Common::GetGtsErrStr(error_num);
			return false;
		}
		speed.vel = dVel/m_pAxisParam.pulsepermm*1000;
		return true;

    }
    else
    {
		double dVel = 0.0;
		double dAcc = 0.0;
		int error_num = GT_GetAxisPrfVel(m_cardIndex, m_axisIndex, &dVel);
		if (GTS_Common::GTS_SUCCESS != error_num)
		{
			GTS_Common::GetGtsErrStr(error_num);
			return false;
		}
		error_num = GT_GetAxisPrfAcc(m_cardIndex, m_axisIndex, &dAcc);
		if (GTS_Common::GTS_SUCCESS != error_num)
		{
			GTS_Common::GetGtsErrStr(error_num);
			return false;
		}
		speed.vel = dVel/m_pAxisParam.pulsepermm*1000;
		speed.acc = dAcc/m_pAxisParam.pulsepermm*1000*1000;
		return true;
    }

    double vel;
    if (!getVel(vel))
    {
        return false;
    }

    speed.vel = vel/m_pAxisParam.pulsepermm*1000;
    return true;
}

bool CAxis::MovePosRel( double pos )
{
    int error_num = -1;
    RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(IsMoving(), false);
	if (m_pAxisParam.limitAlarmConnect)
	{
		if (pos>0)
		{
			RETURN_V_IF(IsLimitP(), false);
		}
		else
		{
			RETURN_V_IF(IsLimitN(), false);
		}
	}

    // ��λģʽ
    RETURN_V_IF(!setPrfTrap(), false);
    RETURN_V_IF(!setPtpSpeed(m_stSpeed), false);

    double curpos = 0;
    bool bRet = GetCurPos(curpos); //ʹ�ñ�����λ�û�����ۻ����  GT_GetPrfPos
    RETURN_V_IF(!bRet, false);

    curpos += pos;

    long pluseCurpos = curpos*m_pAxisParam.pulsepermm;
    error_num = GT_SetPos(m_cardIndex, m_axisIndex, pluseCurpos);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    error_num = GT_Update(m_cardIndex,(0x01 << (m_axisIndex-1)));  
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    return true;
}

bool CAxis::MovePosAbs( double pos )
{
    RETURN_V_IF(!m_bInit, false);
	RETURN_V_IF(IsMoving(), false);
	double curPos = 0.0;

	RETURN_V_IF(!GetCurPos(curPos), false);
	if (m_pAxisParam.limitAlarmConnect)
	{
		if ((pos - curPos)>0)  //�����ƶ�
		{
			RETURN_V_IF(IsLimitP(), false);
		}
		else //�����ƶ�
		{
			RETURN_V_IF(IsLimitN(), false);
		}
	}

    // ��λģʽ
    RETURN_V_IF(!setPrfTrap(), false);
    RETURN_V_IF(!setPtpSpeed(m_stSpeed), false);

    long pluseCurpos = pos*m_pAxisParam.pulsepermm;
    int error_num = GT_SetPos(m_cardIndex, m_axisIndex, pluseCurpos);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    error_num = GT_Update(m_cardIndex,(0x01 << (m_axisIndex-1)));  
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
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
    double encPos = 0;
    int error_num = GT_GetEncPos(m_cardIndex,m_axisIndex,&encPos);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        encPos = 0;
        return false;
    }
    
    pos = encPos/m_pAxisParam.pulsepermm;
    return true;
}

bool CAxis::GetCommandPos( double &pos )
{
    double prfPos = 0;
    int error_num = GT_GetPrfPos(m_cardIndex, m_axisIndex, &prfPos);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    pos = prfPos/m_pAxisParam.pulsepermm;
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
	

    // jogģʽ
    RETURN_V_IF(!setPrfJog(), false);
    RETURN_V_IF(!setJogSpeed(m_stSpeed, bDirection), false);

    int error_num = GT_Update(m_cardIndex,(0x01 << (m_axisIndex-1)));
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::EStop()
{
    int error_num = GT_Stop(m_cardIndex,(0x01 << (m_axisIndex-1)),(0x01 << (m_axisIndex-1)));
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::SmoothStop()
{
    int error_num = GT_Stop(m_cardIndex,(0x01 << (m_axisIndex-1)),0);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::SetPosZero()
{
    int error_num = GT_ZeroPos(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::ClearStatus()
{
    int error_num = GT_ClrSts(m_cardIndex, m_axisIndex, 1);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::ClearAlarm()
{
	int error_num = GT_SetDoBit(m_cardIndex, MC_CLEAR, m_axisIndex, 1);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	return true;
}

bool CAxis::IsMoving()
{
    long status = 0;
    int error_num = GT_GetSts(m_cardIndex, m_axisIndex, &status);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    if (status & 0x400)
    {
        return true;
    }
    return false;
}

bool CAxis::IsOn()
{
	RETURN_V_IF(!m_pAxisParam.servOnConnect, true);
    bool bflag = false;
    int error_num = -1;
    long value = 0;

    error_num = GT_GetSts(m_cardIndex,m_axisIndex, &value);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    if(value & 0x200)
    {
        bflag = true;
    }
    return (m_pAxisParam.servOnLevel == bflag);
}

bool CAxis::IsLimitP()
{
	RETURN_V_IF(!m_pAxisParam.limitPConnect, false);
	bool bflag = false;
    int error_num = -1;
    long value = 0;

    error_num = GT_GetDi(m_cardIndex, MC_LIMIT_POSITIVE, &value);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    if (0 != (value & (0x01 << (m_axisIndex-1))))
    {
        bflag = true;
    }
	return (m_pAxisParam.limitPLevel == bflag);
}

bool CAxis::IsLimitN()
{
	RETURN_V_IF(!m_pAxisParam.limitNConnect, false);
	bool bflag = false;
    int error_num = -1;
    long value = 0;

    error_num = GT_GetDi(m_cardIndex, MC_LIMIT_NEGATIVE,&value);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}

	if (0 != (value & (0x01 << (m_axisIndex-1))))
	{
		bflag = true;
	}
    return (m_pAxisParam.limitNLevel == bflag);
}

bool CAxis::IsDrvAlarm()
{
	RETURN_V_IF(!m_pAxisParam.alarmConnect, false);
	bool bflag = false;
    int error_num = -1;
    long value = 0;

	error_num = GT_GetDi(m_cardIndex, MC_ALARM,&value);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}

	if (value & (0x01 << (m_axisIndex-1)))
	{
		bflag = true;
	}
    return (m_pAxisParam.alarmLevel == bflag);
}

bool CAxis::IsHomeSig()
{
	RETURN_V_IF(!m_pAxisParam.homeConnect, false);
	bool bflag = false;
    int error_num = -1;
    long value = 0;

    error_num = GT_GetDi(m_cardIndex, MC_HOME,&value);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    if (value & (0x01 << (m_axisIndex-1)))
    {
        bflag = true;
    }
    return (m_pAxisParam.homeLevel == bflag);
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
	m_bHomeResult = MOVE_DOING;
	if (!m_hThreadHome->IsRunning())
	{	
		m_hThreadHome->StartThread(this,&CAxis::homeThread,this);
	}
    
    return true;
}

EnMoveBool CAxis::HomeStatus()
{
    return m_bHomeResult;
}

bool CAxis::setJogPrm( TJogPrm &jogprm )
{
    int error_num = GT_SetJogPrm(m_cardIndex, m_axisIndex, &jogprm);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    return true;
}

bool CAxis::getJogprm( TJogPrm &jogprm )
{
    int error_num = GT_GetJogPrm(m_cardIndex, m_axisIndex, &jogprm);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    return true;
}

bool CAxis::setPrfJog()
{
    int error_num = GT_PrfJog(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::setPrfTrap()
{
    int error_num = GT_PrfTrap(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

int CAxis::homeFun()
{
	int iRes = 0;
    unsigned short state = 0;
	ClearStatus();
	RETURN_V_IF(!On(), SYS_ERR_CARD_AXIS_ON);
	
    setPrfTrap();					//���õ�λģʽ
    SetPosZero();					//���λ����Ϣ
    //���û�ԭ�ٶ�
    StSpeed speed(m_pAxisParam.homeVel, m_pAxisParam.homeAcc, m_pAxisParam.dec,m_pAxisParam.velStart,m_pAxisParam.velStop,m_pAxisParam.smoothtime);
    SetSpeed(speed);

    switch (m_pAxisParam.homeMode)
    {
    case HOMEMODE_NONE:			// ���û�ԭ
        {
            return SYS_ERR_OK;
        }        
        break;
    case HOMEMODE_NLIMIT:			// �ظ���λ
        {
			RETURN_V_IF(!m_pAxisParam.limitNConnect, SYS_ERR_CARD_HOME_MODE);
			return HomeLimitNProcess();
			
        }
        break;
    case HOMEMODE_PLIMIT:			// ������λ
        {
			RETURN_V_IF(!m_pAxisParam.limitPConnect, SYS_ERR_CARD_HOME_MODE);
			return HomeLimitPProcess();
        }
        break;
    case HOMEMODE_NLIMIT_HOME:		// �ظ���λ+ԭ��
        {
			RETURN_V_IF(!m_pAxisParam.limitNConnect || !m_pAxisParam.homeConnect, SYS_ERR_CARD_HOME_MODE);
			return HomeOriginProcess(m_pAxisParam.homeMode);
        }
        break;
    case HOMEMODE_PLIMIT_HOME:		// ������λ+ԭ��
        {
			RETURN_V_IF(!m_pAxisParam.limitPConnect || !m_pAxisParam.homeConnect, SYS_ERR_CARD_HOME_MODE);
			return HomeOriginProcess(m_pAxisParam.homeMode);
        }
        break;
    case HOMEMODE_HOME_N:			// �������ԭ��
    case HOMEMODE_HOME_P:			// �������ԭ��
        {
			RETURN_V_IF(!m_pAxisParam.homeConnect, SYS_ERR_CARD_HOME_MODE);
			return HomeOriginProcess(m_pAxisParam.homeMode);
        }
	case HOMEMODE_INDEX_P:		// ������Ѱ��Index
	case HOMEMODE_INDEX_N:		// ������Ѱ��Index
		{
			return HomeIndexProcess();
		}
        break;
    default:
        break;
    }

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

bool CAxis::setTrapPrm( TTrapPrm prm )
{
    int error_num = GT_SetTrapPrm(m_cardIndex, m_axisIndex, &prm);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    return true;
}

bool CAxis::getTrapPrm( TTrapPrm &prm )
{
    int error_num = GT_GetTrapPrm(m_cardIndex, m_axisIndex, &prm);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}


bool CAxis::index()
{
    int error_num = -1;
    error_num = GT_Index(m_cardIndex, m_axisIndex, m_pAxisParam.indexPos, m_pAxisParam.indexOffset);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    return true;
}

bool CAxis::homeSts( unsigned short *pStatus )
{
    int error_num = -1;
    error_num = GT_HomeSts(m_cardIndex, m_axisIndex, pStatus);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    return true;
}

bool CAxis::setVel( double vel )
{
    int error_num = GT_SetVel(m_cardIndex,m_axisIndex,vel);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    return true;
}

bool CAxis::getVel( double &vel )
{
    int error_num = GT_GetEncVel(m_cardIndex, m_axisIndex, &vel);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::setEncSns( int dir )
{
    unsigned short sence  = 0;  
    sence = dir << (m_axisIndex-1);
    int error_num = GT_EncSns(m_cardIndex,sence);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::setCtrlMode( unsigned short mode )
{
    //1�������,0��ѹ���
    int error_num = GT_CtrlMode(m_cardIndex, m_axisIndex, mode);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::setStepDir()
{
    //���������ͨ�����������ģʽ����Ϊ������+����
    int error_num = GT_StepDir(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::setEncOn()
{
    int error_num = GT_EncOn(m_cardIndex,m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::setEncOff()
{
    int error_num = GT_EncOff(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    return true;
}

bool CAxis::setAlarmOff()
{
    int error_num = GT_AlarmOff(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::setLmtsOff()
{
    int error_num = GT_LmtsOff(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::setAlarmOn()
{
    int error_num = GT_AlarmOn(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::setLmtsOn()
{
    int error_num = GT_LmtsOn(m_cardIndex, m_axisIndex);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return true;
}

bool CAxis::setPtpSpeed( StSpeed &speed )
{
    RETURN_V_IF(!m_bInit, false);
    RETURN_V_IF(speed.vel<0.000001, false);

    TTrapPrm param;
    param.acc = speed.acc*m_pAxisParam.pulsepermm/1000/1000;
    param.dec = speed.dec*m_pAxisParam.pulsepermm/1000/1000;
    param.smoothTime = speed.smoothTime;
    param.velStart = speed.velStart*m_pAxisParam.pulsepermm/1000;
    if(!setTrapPrm(param))
    {
        return false;
    }

    double vel = speed.vel*m_pAxisParam.pulsepermm/1000;
    return setVel(vel);
}

bool CAxis::setJogSpeed( StSpeed &speed, bool bDirect )
{
    RETURN_V_IF(!m_bInit, false);
    RETURN_V_IF(speed.vel<0.000001, false);

    TJogPrm jogParam;
    jogParam.acc = speed.acc*m_pAxisParam.pulsepermm/1000/1000;
    jogParam.dec = speed.dec*m_pAxisParam.pulsepermm/1000/1000;
    jogParam.smooth = speed.smoothTime;
    if(!setJogPrm(jogParam))
    {
        return false;
    }

    double vel = speed.vel*m_pAxisParam.pulsepermm/1000;
    vel = bDirect ? vel : -vel;
    return setVel(vel);
}

int CAxis::getPrfMode()
{
    long pValue;
    int error_num = GT_GetPrfMode(m_cardIndex, m_axisIndex, &pValue);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return -1;
    }
    return pValue;
}

int CAxis::ggHome()
{
    RETURN_V_IF(!ClearStatus(),SYS_ERR_CARD_CLEAR_STATUS);
	if (m_pAxisParam.indexPos != 0)
	{
		RETURN_V_IF(!index(),SYS_ERR_CARD_HOME_MODE);			//������ת��C���ȷ��λԭ��
	}

	long homepos = 0;
	if (m_pAxisParam.homeMode == HOMEMODE_PLIMIT_HOME
		|| m_pAxisParam.homeMode == HOMEMODE_HOME_N)
	{
		homepos = -abs(m_pAxisParam.homePos*m_pAxisParam.pulsepermm);
	} 
	else
	{
		homepos = abs(m_pAxisParam.homePos*m_pAxisParam.pulsepermm);
	}

	double curPos = 0.0;
    double homeVel = m_pAxisParam.homeVel*m_pAxisParam.pulsepermm/1000;
    double homeAcc = m_pAxisParam.homeAcc*m_pAxisParam.pulsepermm/1000/1000;
    double homeOffset = m_pAxisParam.homeOffset*m_pAxisParam.pulsepermm;
    int error_num = GT_Home(m_cardIndex, m_axisIndex, homepos,
        homeVel, homeAcc, homeOffset);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return SYS_ERR_CARD_HOME_MODE;
    }

    unsigned short state;
    do 
    {
        RETURN_V_IF(!homeSts(&state),SYS_ERR_CARD_HOME_MODE);
        if (0 == state)
        {//�Զ���ԭ����ִ����...
            Sleep(50);
        } 
        else //����ʱ homeSts ��ֹ 0�� 1
        {//�Զ���ԭִ�����.
            if (IsMoving())
            {
                Sleep(10);
				continue;
            }

			RETURN_V_IF(!GetCurPos(curPos), SYS_ERR_CARD_CTRL_FAIL);
			if (curPos != 0)
			{
				RETURN_V_IF(!SetPosZero(),SYS_ERR_CARD_SET_ZERO);
				Sleep(500);
				continue;
			}

			Sleep(500);		//�ȴ����ͣ��
			RETURN_V_IF(!SetPosZero(),SYS_ERR_CARD_SET_ZERO);
			if (!ClearStatus())
			{
				return SYS_ERR_CARD_CLEAR_STATUS;
			}
            return SYS_ERR_OK;
        }
    } while (1);
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
	long lCurrentPos = 0.0;
	int error_num = 0;
	RETURN_V_IF(IsMoving(), false);
	lCurrentPos = dValue *m_pAxisParam.pulsepermm;

	if (model == POSMODE_COMMAND)
	{
		error_num = GT_SetEncPos(m_cardIndex, m_axisIndex, lCurrentPos);
	} 
	else if (model == POSMODE_ACTUAL)
	{
		error_num = GT_SetPrfPos(m_cardIndex, m_axisIndex, lCurrentPos);
	}
	else
	{
		long mark = 0x01 << (m_axisIndex-1);
		error_num = GT_SetEncPos(m_cardIndex, m_axisIndex, lCurrentPos);
		if (GTS_Common::GTS_SUCCESS != error_num)
		{
			GTS_Common::GetGtsErrStr(error_num);
			return false;
		}
		error_num = GT_SetPrfPos(m_cardIndex, m_axisIndex, lCurrentPos);
		if (GTS_Common::GTS_SUCCESS != error_num)
		{
			GTS_Common::GetGtsErrStr(error_num);
			return false;
		}
		//ͬ���滮���ͱ�����λ��
		error_num = GT_SynchAxisPos(m_cardIndex, mark);
	}
	
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	return true;
}

bool CAxis::LeaveLimit( int iSignal )
{
	RETURN_V_IF(!ClearStatus(),false);

	double dDis = 0;//������
	bool bSignal = false;
	if (iSignal == MC_LIMIT_POSITIVE)
	{
		RETURN_V_IF(!IsLimitP(), true);
		dDis = -abs(m_pAxisParam.homePos)*2;
	} 
	else if (iSignal == MC_LIMIT_NEGATIVE)
	{
		RETURN_V_IF(!IsLimitN(), true);
		dDis = abs(m_pAxisParam.homePos)*2;
	}
	else
	{
		RETURN_V_IF(!IsHomeSig(), true);
		if (m_pAxisParam.homeMode == HOMEMODE_HOME_N)
			dDis = abs(m_pAxisParam.homePos)*2;
		else
			dDis = -abs(m_pAxisParam.homePos)*2;
	}

	RETURN_V_IF(!MovePosRel(dDis), false);
	while (true)
	{
		if (iSignal == MC_LIMIT_POSITIVE)
		{
			bSignal = IsLimitP();
		} 
		else if (iSignal == MC_LIMIT_NEGATIVE)
		{
			bSignal = IsLimitN();
		}
		else
		{
			bSignal = IsHomeSig();
		}

		if (!IsMoving() && bSignal)
		{
			return false;
		}
		
		if (!bSignal)
		{
			if (IsMoving())
			{
				SmoothStop();
				Sleep(100);
			}
			return true;
		}
		Sleep(10);
	}
	return false;
}

int CAxis::HomeLimitNProcess()
{
	RETURN_V_IF(!LeaveLimit(MC_LIMIT_NEGATIVE), SYS_ERR_CARD_CTRL_FAIL);
	RETURN_V_IF(!MoveToLimitNProcess(), SYS_ERR_CARD_AXIS_MOVE_N);

	RETURN_V_IF(!LeaveLimit(MC_LIMIT_NEGATIVE), SYS_ERR_CARD_CTRL_FAIL);
	RETURN_V_IF(!MoveToLimitNProcess(), SYS_ERR_CARD_AXIS_MOVE_N);
	RETURN_V_IF(!LimitMoveOffset(), SYS_ERR_CARD_CTRL_FAIL);
	return SYS_ERR_OK;
}

int CAxis::HomeLimitPProcess()
{
	RETURN_V_IF(!LeaveLimit(MC_LIMIT_POSITIVE), SYS_ERR_CARD_CTRL_FAIL);
	RETURN_V_IF(!MoveToLimitPProcess(), SYS_ERR_CARD_AXIS_MOVE_P);

	RETURN_V_IF(!LeaveLimit(MC_LIMIT_POSITIVE), SYS_ERR_CARD_CTRL_FAIL);
	RETURN_V_IF(!MoveToLimitPProcess(), SYS_ERR_CARD_AXIS_MOVE_P);
	RETURN_V_IF(!LimitMoveOffset(), SYS_ERR_CARD_CTRL_FAIL);
	return SYS_ERR_OK;
}

int CAxis::HomeOriginProcess(EnHomeMode homemode)
{
	RETURN_V_IF(homemode == HOMEMODE_NLIMIT
			|| homemode == HOMEMODE_PLIMIT
			|| homemode == HOMEMODE_USER_DEFINE, SYS_ERR_CARD_HOME_MODE);
	if (homemode == HOMEMODE_NLIMIT_HOME)
	{
		RETURN_V_IF(!MoveToLimitNProcess(false), SYS_ERR_CARD_AXIS_MOVE_N);
	} 
	else if (homemode == HOMEMODE_PLIMIT_HOME)
	{
		RETURN_V_IF(!MoveToLimitPProcess(false), SYS_ERR_CARD_AXIS_MOVE_P);
	}
	else
	{
		RETURN_V_IF(!LeaveLimit(MC_HOME), SYS_ERR_CARD_CTRL_FAIL);
	}
	
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

int CAxis::ggUserHome()
{
	RETURN_V_IF(!ClearStatus(),SYS_ERR_CARD_CLEAR_STATUS);
	long homepos = 0;
	int error_num = 0;
	long status;
	TTrapPrm trapPrm;
	short capSts;
	long capPos;
	double encPos;
	double CurPos = 0;

	double homeVel = m_pAxisParam.homeVel*m_pAxisParam.pulsepermm/1000;
	double homeAcc = m_pAxisParam.homeAcc*m_pAxisParam.pulsepermm/1000/1000;
	double homeDec = m_pAxisParam.homeDec*m_pAxisParam.pulsepermm/1000/1000;
	double homeOffset = m_pAxisParam.homeOffset*m_pAxisParam.pulsepermm;

	//��λ�˶�
	RETURN_V_IF(!ClearStatus(),SYS_ERR_CARD_CLEAR_STATUS);
	error_num = GT_PrfTrap(m_cardIndex, m_axisIndex);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	error_num = GT_GetTrapPrm(m_cardIndex, m_axisIndex,&trapPrm);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	trapPrm.acc = homeAcc;
	trapPrm.dec = homeDec;
	trapPrm.smoothTime = m_pAxisParam.homeSmoothtime;
	error_num = GT_SetTrapPrm(m_cardIndex, m_axisIndex,&trapPrm);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

	if (m_pAxisParam.homeMode == HOMEMODE_PLIMIT_HOME
		|| m_pAxisParam.homeMode == HOMEMODE_HOME_N)
	{
		homepos = -abs(m_pAxisParam.homePos*m_pAxisParam.pulsepermm);
	} 
	else
	{
		homepos = abs(m_pAxisParam.homePos*m_pAxisParam.pulsepermm);
	}

	//����Home����
	error_num = GT_ClearCaptureStatus(m_cardIndex, m_axisIndex);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	error_num = GT_SetCaptureMode(m_cardIndex, m_axisIndex,CAPTURE_HOME);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

	//Ѱ��Home
	error_num = GT_SetVel(m_cardIndex, m_axisIndex, homeVel);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	error_num = GT_SetPos(m_cardIndex, m_axisIndex, homepos);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	error_num = GT_Update(m_cardIndex, 1<<(m_axisIndex - 1));
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

	//�ж��Ƿ��ҵ�Home
	while(true)
	{
		error_num = GT_GetCaptureStatus(m_cardIndex, m_axisIndex,&capSts,&capPos);
		RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

		if (capSts != 0)
			break;

		RETURN_V_IF(!IsMoving(), SYS_ERR_HOME);
		Sleep(2);
	}

	EStop();
	error_num = GT_ClrSts(m_cardIndex, m_axisIndex);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	long pos1 = capPos + homeOffset;
	error_num = GT_SetPos(m_cardIndex, m_axisIndex, pos1);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	error_num = GT_Update(m_cardIndex, 1<<(m_cardIndex, m_axisIndex - 1));
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

	while(true)
	{
		if (IsMoving()){
			Sleep(2);
			continue;
		}

		if (m_pAxisParam.indexPos != 0)
		{

			Sleep(500);
			if (GetCurPos(CurPos)){
				if (abs(CurPos*m_pAxisParam.pulsepermm - pos1) > 5)
					return SYS_ERR_CARD_CTRL_FAIL;
			}
			else
				return SYS_ERR_HOME;
		}
		break;
	}

	if (m_pAxisParam.indexPos > 0)
	{
		long indexPos = m_pAxisParam.indexPos*m_pAxisParam.pulsepermm;
		long indexOffset = m_pAxisParam.indexOffset*m_pAxisParam.pulsepermm;
		// ����index����
		error_num = GT_SetCaptureMode(m_cardIndex, m_axisIndex, CAPTURE_INDEX);
		RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

		// ���õ�ǰλ��+index ��������ΪĿ��λ��
		error_num = GT_SetPos(m_cardIndex, m_axisIndex, (long)(pos1 + indexPos));
		RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
		error_num = GT_Update(m_cardIndex, 1<<(m_axisIndex-1));
		RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

		// �ȴ�index�����źŴ���
		while(true)
		{
			// ���index�ź��Ѿ����������˳�ѭ��������λ���Ѿ���capPos�����б���
			error_num = GT_GetCaptureStatus(m_cardIndex, m_axisIndex, &capSts, &capPos);
			RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
			if (capSts != 0)
				break;

			RETURN_V_IF(!IsMoving(), SYS_ERR_HOME);
			Sleep(2);
		}

		EStop();
		// ���ò���λ��+indexƫ����ΪĿ��λ��
		pos1 = capPos + indexOffset;
		error_num = GT_SetPos(m_cardIndex, m_axisIndex, capPos+ indexOffset);
		RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
		error_num = GT_Update(m_cardIndex, 1<<(m_axisIndex-1));
		RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

		while(true)
		{
			if (IsMoving()){
				Sleep(2);
				continue;
			}

			Sleep(500);
			if (GetCurPos(CurPos)){
				if (abs(CurPos*m_pAxisParam.pulsepermm - pos1) > 5)
					return SYS_ERR_CARD_CTRL_FAIL;
			}
			else
				return SYS_ERR_HOME;

			break;
		}

	}

	while(true)
	{
		RETURN_V_IF(!GetCurPos(CurPos), SYS_ERR_CARD_CTRL_FAIL);
		if (CurPos != 0)
		{
			RETURN_V_IF(!SetPosZero(),SYS_ERR_CARD_SET_ZERO);
			Sleep(200);
			continue;
		}
		break;
	}

	Sleep(500);
	error_num = GT_ZeroPos(m_cardIndex, m_axisIndex);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	return SYS_ERR_OK;
}

int CAxis::HomeIndexProcess()
{
	RETURN_V_IF(!ClearStatus(),SYS_ERR_CARD_CLEAR_STATUS);
	RETURN_V_IF(m_pAxisParam.indexPos <= 0,SYS_ERR_CARD_CLEAR_STATUS);

	int error_num = 0;
	TTrapPrm trapPrm;
	short capSts;
	long capPos;
	double CurPos = 0;
	double targetPos = 0;

	long indexPos;;
	double homeVel = m_pAxisParam.homeVel*m_pAxisParam.pulsepermm/1000;
	double homeAcc = m_pAxisParam.homeAcc*m_pAxisParam.pulsepermm/1000/1000;
	double homeDec = m_pAxisParam.homeDec*m_pAxisParam.pulsepermm/1000/1000;
	long indexOffset = m_pAxisParam.indexOffset*m_pAxisParam.pulsepermm;

	//��λ�˶�
	RETURN_V_IF(!ClearStatus(),SYS_ERR_CARD_CLEAR_STATUS);
	error_num = GT_PrfTrap(m_cardIndex, m_axisIndex);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	error_num = GT_GetTrapPrm(m_cardIndex, m_axisIndex,&trapPrm);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	trapPrm.acc = homeAcc;
	trapPrm.dec = homeDec;
	trapPrm.smoothTime = m_pAxisParam.homeSmoothtime;
	error_num = GT_SetTrapPrm(m_cardIndex, m_axisIndex,&trapPrm);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

	if (m_pAxisParam.homeMode == HOMEMODE_PLIMIT_HOME
		|| m_pAxisParam.homeMode == HOMEMODE_HOME_N)
	{
		indexPos = -abs(m_pAxisParam.indexPos*m_pAxisParam.pulsepermm);
	} 
	else
	{
		indexPos = abs(m_pAxisParam.indexPos*m_pAxisParam.pulsepermm);
	}

	// ����index����
	error_num = GT_ClearCaptureStatus(m_cardIndex, m_axisIndex);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	error_num = GT_SetCaptureMode(m_cardIndex, m_axisIndex, CAPTURE_INDEX);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

	//Ѱ��Index
	error_num = GT_SetVel(m_cardIndex, m_axisIndex, homeVel);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	error_num = GT_SetPos(m_cardIndex, m_axisIndex, indexPos);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	error_num = GT_Update(m_cardIndex, 1<<(m_axisIndex - 1));
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);

	// �ȴ�index�����źŴ���
	while(true)
	{
		// ���index�ź��Ѿ����������˳�ѭ��������λ���Ѿ���capPos�����б���
		error_num = GT_GetCaptureStatus(m_cardIndex, m_axisIndex, &capSts, &capPos);
		RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
		if (capSts != 0)
			break;

		RETURN_V_IF(!IsMoving(), SYS_ERR_HOME);
		Sleep(2);
	}

	EStop();
	// ���ò���λ��+indexƫ����ΪĿ��λ��
	targetPos = capPos+ indexOffset;
	error_num = GT_SetPos(m_cardIndex, m_axisIndex, targetPos);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	error_num = GT_Update(m_cardIndex, 1<<(m_axisIndex-1));
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	while(true)
	{
		if (IsMoving()){
			Sleep(10);
			continue;
		}

		Sleep(500);
		if (GetCurPos(CurPos)){
			if (abs(CurPos*m_pAxisParam.pulsepermm - targetPos) > 5)
				return SYS_ERR_CARD_CTRL_FAIL;
		}
		else
			return SYS_ERR_HOME;

		break;
	}

	Sleep(500);
	error_num = GT_ZeroPos(m_cardIndex, m_axisIndex, 1);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num, error_num);
	return SYS_ERR_OK;
}
