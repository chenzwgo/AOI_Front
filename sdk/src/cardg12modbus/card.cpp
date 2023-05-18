#include "card.h"
#include "axis.h"
#include "io.h"
#include "card/iio.h"
#include "ioext.h"
#include "rtdefine.h"
#include "card/iaxis.h"
#include "converymodbus.h"

#define MAX_AXIS_LEN 8
#define CRD_NUM 1       //坐标系号,取值范围：[1, 2]

CCard::CCard()
{
	m_cardIndex = -1;
	m_axisCnt = 0;
	m_extIndex = 0;
	m_bOpened = false;
	m_bhoming = false;
	m_bConfigLoaded = false;
	m_bInit = false;
	m_iCoorSysCnt = 0;
	memset(m_szCoordOffset, 0, sizeof(m_szCoordOffset));
	memset(m_status, 0, sizeof(m_status));
	memset(m_count, 0, sizeof(m_count));
	//init();	//不在构造函数中初始化
}

CCard::~CCard()
{
	Deinit();
}

bool CCard::Init(unsigned short cardIndex, unsigned short axisCnt, unsigned short extIndex)
{
	RETURN_V_IF(m_bInit, true);
	RETURN_V_IF(cardIndex < 0 && cardIndex >= 8,false);
	RETURN_V_IF(axisCnt < 0  && axisCnt > 8,false);

	m_cardIndex = cardIndex;
	m_axisCnt = axisCnt;
	m_extIndex = extIndex;

	m_vec_Axis.resize(MAX_AXIS_LEN);
	m_vec_IO.resize(m_extIndex+1, NULL);

	//1.开卡
	int i = 0;
	bool bCardInit = true;
	bool bAxisInit = true;
	if (!Open())
	{
		bCardInit = false;
	}

	//2.加载轴资源，IO资源
	for (i=0; i<m_axisCnt; i++)
	{
		m_vec_Axis[i] = (IAxis*)new CAxis(m_cardIndex, i + 1);//轴索引从1开始
		m_vec_Axis[i]->Init();
	}

	//卡自带的IO
	m_vec_IO[0] = new CIO(m_cardIndex);
	for (i = 1; i <= m_extIndex; i++)
	{//扩展卡IO
		m_vec_IO[i] = new CIOExt(m_cardIndex, i - 1);
	}
	
	//3.加载卡的配置文件信息
	char szBuf[64] = {0};
	_snprintf_s(szBuf, sizeof(szBuf) - 1, "./plugins/cardgg/%s%d.cfg","card_",m_cardIndex);
	RETURN_V_IF(!loadConfig(szBuf), false);

	for (int i=0; i<m_vec_Axis.size(); i++)
	{
		IAxis* paixs = m_vec_Axis[i];
		if (NULL != paixs)
		{
			if (!paixs->Init())
			{
				bAxisInit = false;
			}
		}
	}
	m_bInit = true;
	return bAxisInit && bCardInit;
}

bool CCard::Deinit()
{
	// 释放资源
	ConveryModbus::ReleaseInstance();


	int i = 0;
	int n = m_vec_Axis.size();
	for (i=0; i<n; i++)
	{
		if (NULL != m_vec_Axis[i])
		{
			delete m_vec_Axis[i];
			m_vec_Axis[i] = NULL;
		}
	}
	m_vec_Axis.swap(vector<IAxis *>());

	n = m_vec_IO.size();
	for (i = 0; i < n; i++)
	{
		if (NULL != m_vec_IO[i])
		{
			delete m_vec_IO[i];
			m_vec_IO[i] = NULL;
		}
	}
	m_vec_IO.swap(vector<IIO *>());
	Close();
	return true;
}

bool CCard::Reset()
{
	for (int i=0; i<m_vec_Axis.size(); i++)
	{
		IAxis* paixs = m_vec_Axis[i];
		if (NULL != paixs)
		{
			paixs->ClearStatus();
			paixs->On();
		}
	}
	return true;
}

bool CCard::Open()
{
	ConveryModbus::GetInstance();
	return true;
}

bool CCard::homeinit()
{
	return true;
}

bool CCard::Close()
{
	return true;
}

IAxis *CCard::GetAxis(unsigned short index)
{
	RETURN_V_IF(index >= m_vec_Axis.size(), NULL);
	return m_vec_Axis[index];
}

// 获取IO
IIO *CCard::GetIO(unsigned short index)
{
	RETURN_V_IF(index >= m_vec_IO.size(), NULL);
	return m_vec_IO[index];
}

unsigned int CCard::GetCardIndex()
{
	return m_cardIndex;
}

unsigned int CCard::GetAxisCnt()
{
	return m_axisCnt;
}

bool CCard::loadConfig(char* path)
{
	return true;
}

bool CCard::saveConfig(char* path)
{
	RETURN_V_IF(!path, false);
	return true;
}

bool CCard::IsConnected()
{
	return ConveryModbus::GetInstance()->isConnect();
}

bool CCard::CardHome( int homeAxis[], int axisCount )
{
    for (int i=0; i<axisCount; i++)
    {
        int axisIndex = homeAxis[i];
        if(NULL != m_vec_Axis[axisIndex])
        {
            if(!m_vec_Axis[axisIndex]->Home())
            {
                return false;
            }
        }
    }

    return true;
}

EnMoveBool CCard::CardHomeStatus(int homeAxis[], int axisCount)
{
    for (int i=0; i<axisCount && i < m_vec_Axis.size(); i++)
    {
        if(NULL != m_vec_Axis[homeAxis[i]])
        {
            if(MOVE_FALSE == m_vec_Axis[homeAxis[i]]->HomeStatus())
            {
                return MOVE_FALSE;
            }
            else if(MOVE_DOING == m_vec_Axis[homeAxis[i]]->HomeStatus())
            {
                return MOVE_DOING;
            }
            
            if (m_vec_Axis[homeAxis[i]]->IsDrvAlarm())
            {
                EmgStop();
                return MOVE_FALSE;
            }
        }
    }

    return MOVE_TRUE;
}

bool CCard::EmgStop()
{
    bool bEStopSucceed = true;
    for (int i=0; i<m_vec_Axis.size(); i++)
    {
        IAxis* paixs = m_vec_Axis[i];
        if(NULL != paixs && !paixs->EStop())	// 急停
        {
            bEStopSucceed = false;
        }
    }
    return bEStopSucceed;
}

bool CCard::SetContinueInterSpeed(int hContinueInterHandle, const StSpeed &speed )
{
    return true;
}

bool CCard::LineXY( int xAxis, int yAxis, double xPos,double yPos )
{
    return true;
}

bool CCard::LineXYZ( int xAxis, int yAxis, int zAxis, double xPos,double yPos,double zPos )
{
    return true;
}

bool CCard::LineXYZU( int xAxis, int yAxis, int zAxis, int uAxis, double xPos,double yPos,double zPos,double uPos )
{
    return true;
}

bool CCard::ArcXY( int xAxis, int yAxis, double xPos1,double yPos1,double xPos2,double yPos2,double xPos3,double yPos3 )
{
    return  true;
}

bool CCard::InitContinueInter(int axis[], int count ,int &hContinueInterHandle)
{
	return true;
}

bool CCard::SetCoordinateOffset(int hContinueInterHandle, double dOffset[4])
{
	return true;
}

bool CCard::SetCoorSysParam(int axis[], int count )
{
	return true;
}

bool CCard::BufLineXY( int xAxis, int yAxis, double xPos,double yPos )
{
     return true;
}

bool CCard::BufLineXYZ( int xAxis, int yAxis, int zAxis, double xPos,double yPos,double zPos )
{
    return true;
}

bool CCard::BufLineXYZU( int xAxis, int yAxis, int zAxis, int uAxis, double xPos,double yPos,double zPos,double uPos )
{
     return true;
}

bool CCard::BufArcXY( int xAxis, int yAxis, double xPos1,double yPos1,double xPos2,double yPos2,double xPos3,double yPos3 )
{
     return  true;
}

bool CCard::BufArcXYR( int xAxis, int yAxis, double xPos1,double yPos1, double xPos2,double yPos2,double Radius, int idir )
{
	return  true;
}

bool CCard::BufDelay(int hContinueInterHandle, int time )
{
    return  true;
}

bool CCard::BufIO(int hContinueInterHandle, int index, bool bOpen, int delay/*=0 */ )
{
    return  true;
}

bool CCard::ClearBuf(int hContinueInterHandle)
{
    return true;
}

bool CCard::StartContInterp(int hContinueInterHandle)
{
    return true;
}

bool CCard::PauseContInterp(int hContinueInterHandle)
{
    return true;
}

bool CCard::StopContInterp(int hContinueInterHandle)
{
    return true;
}

bool CCard::IsContInterpFinished(int hContinueInterHandle )
{
	return true;
}

long CCard::getPlusePermm( short axisIndex )
{
    RETURN_V_IF(axisIndex >= m_vec_Axis.size(), 1);

    StAxisParam pAxisParam;
    bool bRet = m_vec_Axis.at(axisIndex)->GetAxisParam(&pAxisParam);
    RETURN_V_IF(!bRet, 1);

    return pAxisParam.pulsepermm;
}

double CCard::getcurPos( short axisIndex )
{
    RETURN_V_IF(axisIndex >= m_vec_Axis.size(), 1);

    double pos = 0;
    bool bRet = m_vec_Axis.at(axisIndex)->GetCurPos(pos);
    RETURN_V_IF(!bRet, 1);

    return pos;
}

short CCard::SetCompareData(short encoder, short source, short pulseType, short startLevel, short time, long *pBuf1, short count1, long *pBuf2, short count2)
{
	return  SYS_ERR_OK;
}
short CCard::StopCompare()
{
	return  SYS_ERR_OK; 
}

bool CCard::setComparePos( int axis, short startLevel, long pluseWidth, double pos1[], int count1, double pos2[], int count2 )
{
	return true;
}

bool CCard::stopComparePos( int axis )
{
	ConveryModbus::GetInstance()->stopAutoTrigger();
	return true;
}

bool CCard::setCompareOutput( short level, short type, short time )
{
	return true;
}

bool CCard::GetCompareStatus(short &pStatus, long pCount[])
{
	bool bRet = ConveryModbus::GetInstance()->isTriggerFinished();
	if (bRet) pStatus = 1;
	return true;
}

int CCard::FindCoordSys(int hContinueInterHandle)
{
    return 0;
}

int CCard::FindCoordSys( int AxisIndex[], int iAxisCnt )
{
	RETURN_V_IF(AxisIndex == NULL, 0);

	return 0;
}

double CCard::getCoorSysMaxVec( int AxisIndex[], int iAxisCnt )
{
	bool bFirst = false;
	double dVecMax = 0.0;
	StAxisParam stAxisParamMax;

	for (int inum = 0; inum < iAxisCnt; inum++)
	{
		//查找轴配置信息
		StAxisParam stAxisParam;
		int i = inum-1;
		int iaxs = AxisIndex[inum]>=m_axisCnt?AxisIndex[ i>0?i:0]:AxisIndex[inum];//为了使用一个轴的二维插补//需要加一个不使用的轴
		bool bRet = m_vec_Axis.at(iaxs)->GetAxisParam(&stAxisParam);
		RETURN_V_IF(!bRet, 0);

		if (!bFirst)
		{
			stAxisParamMax = stAxisParam;
			bFirst = true;
		}

		if (dVecMax > stAxisParam.vel)
		{//以轴的最小速度参数作为坐标系的速度参数
			stAxisParamMax = stAxisParam;
		}
	}

	dVecMax = stAxisParamMax.vel;
	return dVecMax;
}


double CCard::getCoorSysMaxAcc( int AxisIndex[], int iAxisCnt )
{
	bool bFirst = false;
	double dMaxAcc = 0.0;
	StAxisParam stAxisParamMax;

	for (int inum = 0; inum < iAxisCnt; inum++)
	{
		//查找轴配置信息
		StAxisParam stAxisParam;
		int i = inum-1;
		int iaxs = AxisIndex[inum]>=m_axisCnt?AxisIndex[i>0?i:0]:AxisIndex[inum];//为了使用一个轴的二维插补//需要加一个不使用的轴
		bool bRet = m_vec_Axis.at(iaxs)->GetAxisParam(&stAxisParam);
		RETURN_V_IF(!bRet, 0);

		if (!bFirst)
		{
			stAxisParamMax = stAxisParam;
			bFirst = true;
		}

		if (dMaxAcc > stAxisParam.acc)
		{//以轴的最小加速度参数作为坐标系的速度参数
			stAxisParamMax = stAxisParam;
		}
	}

	dMaxAcc = stAxisParamMax.acc;
	return dMaxAcc;
}

bool CCard::EnableLookAhead(int hContinueInterHandle, bool bEnable)
{
	return true;
}

bool CCard::InitLookAhead( int iCoorSysIndex )
{	
	return  SYS_ERR_OK; 
}

int CCard::GetCurSegNum(int hContinueInterHandle)
{
	return  SYS_ERR_OK;
}

bool CCard::BufDA(int hContinueInterHandle, int channel, short value )
{
	return  true;
}

bool CCard::IsBufFull(int hContinueInterHandle)
{
	return  true;
}

bool CCard::BufGear(int hContinueInterHandle, int iprofile, long lpulse, int icrd/*=0*/ )
{
	return true;
}

bool CCard::SetCompareLiner( short encoder, short channel,double startPos,long repeatTimes,double interval,short time,short source )
{
	uint nPos = startPos * getPlusePermm(encoder - 1);
	uint nInterval = interval * getPlusePermm(encoder - 1);
	ConveryModbus::GetInstance()->setTriggerDelay(nPos);
	ConveryModbus::GetInstance()->setTriggerInterval(nInterval);
	ConveryModbus::GetInstance()->setTriggerCount(repeatTimes);
	return true;
}

bool CCard::SetComparePluse( short level,short outputType, uint time )
{
	if (outputType == 1)
	{
		ConveryModbus::GetInstance()->setTriggerCount(time);
		ConveryModbus::GetInstance()->startAutoTriggerByLines();
	}
	else
	{
		ConveryModbus::GetInstance()->startAutoTrigger();
	}
	
	return true;
}

bool CCard::SetPrfMapAxis( short encoder, short prfMapAxis )
{
	return true;
}

int CCard::GetCompareCount()
{
	return m_count[0];
}
bool CCard::CompareMode2D( short chn,short mode )
{
	return true;
}

bool CCard::ComparePulse2D( short chn,short level,short outputType,short time )
{
	return true;
}

bool CCard::CompareStop2D( short chn )
{
	return true;
}

bool CCard::CompareClear2D( short chn )
{
	return true;
}

bool CCard::CompareStatus2D( short chn,short *pStatus,long *pCount,short *pFifo,short *pFifoCount,short *pBufCount /*=NULL*/ )
{
	return true;
}

bool CCard::CompareSetPrm2D( short chn,CardGGT2DComparePrm *pPrm )
{
	return true;
}

bool CCard::CompareData2D( short chn,short count,CardGGT2DCompareData *pBuf,short fifo )
{
	return true;
}

bool CCard::CompareStart2D( short chn )
{
	return true;
}

bool CCard::SetComparePort2D( short channel,short hsio0,short hsio1 )
{
	return true;
}
bool CCard::SetCompareContinuePulseMode(short mode, short count, short standTime)
{
	return true;
}
