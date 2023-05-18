#include "card.h"
#include "axis.h"
#include "io.h"
#include "card/iio.h"
#include "ioext.h"
#include "rtdefine.h"
#include "gts_common.h"
#include "card/iaxis.h"
#include "config.h"

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
	RETURN_V_IF(m_bOpened ,true);
	int error_num = -1;
	error_num = GT_Open(m_cardIndex,0,1);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	m_bOpened  = true;
	return true;
}

bool CCard::homeinit()
{
	int error_num = -1;
	error_num = GT_HomeInit(m_cardIndex);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	return true;
}

bool CCard::Close()
{
	GT_Close(m_cardIndex);
	m_bOpened = false;
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
	RETURN_V_IF(m_bConfigLoaded, true);
	int error_num = GT_LoadConfig(m_cardIndex,path);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	m_bConfigLoaded = true;
	return true;
}

bool CCard::saveConfig(char* path)
{
	RETURN_V_IF(!path, false);
	return true;
}

bool CCard::IsConnected()
{
	unsigned long clk;
	unsigned long loop;
	int error_num = GT_GetClock(m_cardIndex,&clk,&loop);						//判断新卡的通讯状态
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	RETURN_V_IF(0 <= clk, true);
	return false;
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
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0,false);
	stCoordInfo* pCoorPrm = GetCoordSysParam(iCoorSysIndex);
	RETURN_V_IF(pCoorPrm == NULL,false);
    RETURN_V_IF(pCoorPrm->listAxis.size() == 0,false);

	double dSpeed = speed.vel;
	double dAcc = speed.acc;
	double dDec = speed.dec;
	double dStart = speed.velStart;
	double dStop = speed.velStop;
	if (dSpeed > pCoorPrm->tcrprm.synVelMax)
	{
		dSpeed = pCoorPrm->tcrprm.synVelMax;
	}
	if (dAcc > pCoorPrm->tcrprm.synAccMax)
	{
		dAcc = pCoorPrm->tcrprm.synAccMax;
	}
	double dPermm = getPlusePermm(pCoorPrm->listAxis.at(0));//--1  d move 
    m_interpSpeed = StSpeed(dSpeed*dPermm/1000, dAcc*dPermm/1000/1000,
                            dDec*dPermm/1000/1000,dStart*dPermm/1000,
                            dStop*dPermm/1000, speed.smoothTime);

    return true;
}

bool CCard::LineXY( int xAxis, int yAxis, double xPos,double yPos )
{
	// 脉冲当量
	long xUnit = getPlusePermm(xAxis);
	long yUnit = getPlusePermm(yAxis);

	int iAxis[2] = {xAxis, yAxis};
	int iCoorSysIndex = FindCoordSys(iAxis, 2);
	RETURN_V_IF(iCoorSysIndex == 0,false);
	RETURN_V_IF(!SetCoorSysParam(iAxis, 2), false);

    int error_num = GT_CrdClear(m_cardIndex, iCoorSysIndex, 0);
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
    error_num = GT_LnXY(
		m_cardIndex,
        iCoorSysIndex,		//  该插补段的坐标系是坐标系 
        xPos*xUnit, 
		yPos*yUnit,			//  该插补段的终点坐标(200000, 0) 
        m_interpSpeed.vel,	//  该插补段的目标速度：100pulse/ms 
        m_interpSpeed.acc,	//  插补段的加速度：0.1pulse/ms^2 
        0.0,					//  终点速度为0 
        0);					//  向坐标系1的FIFO0缓存区传递该直线插补数据 
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);

    error_num = GT_CrdStart(m_cardIndex, iCoorSysIndex, 0);
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);

    return true;
}

bool CCard::LineXYZ( int xAxis, int yAxis, int zAxis, double xPos,double yPos,double zPos )
{
    // 脉冲当量
    long xUnit = getPlusePermm(xAxis);
    long yUnit = getPlusePermm(yAxis);
    long zUnit = getPlusePermm(zAxis);

	int iAxis[3] = {xAxis, yAxis, zAxis};
	int iCoorSysIndex = FindCoordSys(iAxis, 3);
	RETURN_V_IF(iCoorSysIndex == 0,false);
	RETURN_V_IF(!SetCoorSysParam(iAxis, 3), false);

    int error_num = GT_CrdClear(m_cardIndex, iCoorSysIndex, 0);
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
    error_num = GT_LnXYZ(
		m_cardIndex,
        iCoorSysIndex,			//  该插补段的坐标系是坐标系 
        xPos*xUnit,
		yPos*yUnit,
		zPos*zUnit,				//  该插补段的终点坐标(200000, 0) 
		m_interpSpeed.vel,		//  该插补段的目标速度：100pulse/ms 
		m_interpSpeed.acc,		//  插补段的加速度：0.1pulse/ms^2 
		0.0,						//  终点速度为0
        0);						//  向坐标系1的FIFO0缓存区传递该直线插补数据 
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);

    error_num = GT_CrdStart(m_cardIndex, iCoorSysIndex, 0);
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);

    return true;
}

bool CCard::LineXYZU( int xAxis, int yAxis, int zAxis, int uAxis, double xPos,double yPos,double zPos,double uPos )
{
    // 脉冲当量
    long xUnit = getPlusePermm(xAxis);
    long yUnit = getPlusePermm(yAxis);
    long zUnit = getPlusePermm(zAxis);
    long uUnit = getPlusePermm(uAxis);

	int iAxis[4] = {xAxis, yAxis, zAxis, uAxis};
	int iCoorSysIndex = FindCoordSys(iAxis, 4);
	RETURN_V_IF(iCoorSysIndex == 0,false);
	RETURN_V_IF(!SetCoorSysParam(iAxis, 4), false);

    int error_num = GT_CrdClear(m_cardIndex, iCoorSysIndex, 0);
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
    error_num = GT_LnXYZA(
		m_cardIndex,
        iCoorSysIndex,			//  该插补段的坐标系是坐标系 
        xPos*xUnit,
		yPos*yUnit,
		zPos*zUnit,
		uPos*uUnit,				//  该插补段的终点坐标(200000, 0) 
		m_interpSpeed.vel,		//  该插补段的目标速度：100pulse/ms 
		m_interpSpeed.acc,		//  插补段的加速度：0.1pulse/ms^2 
		0.0,						//  终点速度为0
        0);						//  向坐标系1的FIFO0缓存区传递该直线插补数据 
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);

    error_num = GT_CrdStart(m_cardIndex, iCoorSysIndex, 0);
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);

    return true;
}

bool CCard::ArcXY( int xAxis, int yAxis, double xPos1,double yPos1,double xPos2,double yPos2,double xPos3,double yPos3 )
{
    double a1,b1,a2,b2,c1,c2,x,y,x1_tmp,x2_tmp,x3_tmp,y1_tmp,y2_tmp,y3_tmp;
    short circleDir=0;
    long result=0;
	double x1 = xPos1;
	double y1 = yPos1;
	double x2 = xPos2;
	double y2 = yPos2;
	double x3 = xPos3;
	double y3 = yPos3;

    result=(x2-x1)*(y3-y2)-(x3-x2)*(y2-y1);

    if(result==0)
    {
        return false ;
    }
    else if (result>0)
    {
        circleDir=1;
    }
    else
    {
        circleDir=0;
    }

    x1_tmp=(double)x1;
    x2_tmp=(double)x2;
    x3_tmp=(double)x3;
    y1_tmp=(double)y1;
    y2_tmp=(double)y2;
    y3_tmp=(double)y3;

    a1=2*(x2_tmp-x1_tmp);
    a2=2*(x3_tmp-x1_tmp);
    b1=2*(y2_tmp-y1_tmp);
    b2=2*(y3_tmp-y1_tmp);
    c1=x2_tmp*x2_tmp-x1_tmp*x1_tmp+y2_tmp*y2_tmp-y1_tmp*y1_tmp;
    c2=x3_tmp*x3_tmp-x1_tmp*x1_tmp+y3_tmp*y3_tmp-y1_tmp*y1_tmp;

    x=(c1*b2-c2*b1)/(a1*b2-a2*b1);
    y=(c1*a2-c2*a1)/(b1*a2-b2*a1);
    x-=x1_tmp;
    y-=y1_tmp;

    // 脉冲当量
    long xUnit = getPlusePermm(xAxis);
    long yUnit = getPlusePermm(yAxis);

	int iAxis[2] = {xAxis, yAxis};
	int iCoorSysIndex = FindCoordSys(iAxis, 2);
	RETURN_V_IF(iCoorSysIndex == 0,false);
	RETURN_V_IF(!SetCoorSysParam(iAxis, 2), false);

    int error_num = GT_CrdClear(m_cardIndex, iCoorSysIndex, 0);
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);

	error_num = GT_ArcXYC(
		m_cardIndex,
		iCoorSysIndex,
		(long)x3*xUnit,
		(long)y3*yUnit,
		x*xUnit,y*yUnit,
		circleDir,
		m_interpSpeed.vel,	//  该插补段的目标速度：100pulse/ms 
		m_interpSpeed.acc,	//  插补段的加速度：0.1pulse/ms^2 
		0.0,					//  终点速度为0
		0);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}

    error_num = GT_CrdStart(m_cardIndex, iCoorSysIndex, 0);
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);

    return  true;
}

bool CCard::InitContinueInter(int axis[], int count ,int &hContinueInterHandle)
{
	RETURN_V_IF(count < 2 || count > 4, false);

	// 一张卡最多两个坐标系，坐标系1，坐标系2
	int iCoorSysIndex = FindCoordSys(axis, count);
    hContinueInterHandle = iCoorSysIndex;
	if (iCoorSysIndex != 0 )
	{
		return SetCoorSysParam(axis, count);
	} 
	else
	{
		RETURN_V_IF(2 <= m_iCoorSysCnt, 0);
		m_iCoorSysCnt++;
		iCoorSysIndex = m_iCoorSysCnt;
        hContinueInterHandle = iCoorSysIndex;
		TCrdPrm crdPrm;
		memset(&crdPrm, 0, sizeof(crdPrm)); 
		crdPrm.dimension = count;				//  坐标系坐标系 (最小2维，最大4维)
		crdPrm.setOriginFlag = 1;				//  表示需要指定坐标系的原点坐标的规划位置 
		crdPrm.synVelMax = getCoorSysMaxVec(axis, count);		//  默认最大合成速度：pulse/ms 
		crdPrm.synAccMax = getCoorSysMaxAcc(axis, count);		//  默认最大加速度：pulse/ms^2 
		crdPrm.evenTime = 10;					//  默认最小匀速时间：10ms 

		// 默认速度
		m_interpSpeed.vel = 100;
		m_interpSpeed.velStop = 100;
		m_interpSpeed.acc = 2;
		m_interpSpeed.dec = 2;
		m_interpSpeed.smoothTime = 10;

		// 关联规划器和坐标系，坐标系从X轴开始，坐标系X轴对应1，Y轴对应2，规划器为0表示不关联 
		for (int iCoorAxis=0; iCoorAxis<count; iCoorAxis++)
		{
			crdPrm.profile[axis[iCoorAxis]] = iCoorAxis+1;
			crdPrm.originPos[axis[iCoorAxis]] = 0; 
		}

		vector<int> vecAxis;
		for (int icnt=0; icnt<count; icnt++)
		{
			vecAxis.push_back(axis[icnt]);
		}
		stCoordInfo stInfo;
		stInfo.iCoorSysIndex = iCoorSysIndex;
		stInfo.tcrprm = crdPrm;
		stInfo.listAxis = vecAxis;
		m_mapCoorSysInfo.insert(make_pair(iCoorSysIndex, stInfo));

		//  建立坐标系，设置坐标系参数 
		short error_num = GT_SetCrdPrm(m_cardIndex,iCoorSysIndex,&crdPrm); 
		if (GTS_Common::GTS_SUCCESS != error_num)
		{
			string err = GTS_Common::GetGtsErrStr(error_num);
			return false;
		}
		
		error_num = GT_CrdClear(m_cardIndex, iCoorSysIndex, 0);
		if (GTS_Common::GTS_SUCCESS != error_num)
		{
			GTS_Common::GetGtsErrStr(error_num);
			return false;
		}

		long lSpace;	//插补缓冲区剩余空间
		error_num = GT_CrdSpace(m_cardIndex, iCoorSysIndex, &lSpace, 0);
		if (GTS_Common::GTS_SUCCESS != error_num)
		{
			GTS_Common::GetGtsErrStr(error_num);
			return false;
		}

		return true;
	}
}

bool CCard::SetCoordinateOffset(int hContinueInterHandle, double dOffset[4])
{
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0, false);
	for (int icnt=0; icnt < 4; icnt++)
	{
		m_szCoordOffset[iCoorSysIndex-1][icnt] = dOffset[icnt];
	}

	return true;
}

bool CCard::SetCoorSysParam(int axis[], int count )
{
	int iCoorSysIndex = FindCoordSys(axis, count);
	RETURN_V_IF(iCoorSysIndex == 0, false);

	stCoordInfo* pCoordInfo = GetCoordSysParam(iCoorSysIndex);
	RETURN_V_IF(pCoordInfo == NULL, false);

	for (int i=0; i<count; i++)
	{
		int index = (axis[i]>=m_axisCnt)?axis[i-1]:axis[i];
		index = (index<0)?0:index;//add 20180125
		pCoordInfo->tcrprm.originPos[axis[i]] = m_szCoordOffset[iCoorSysIndex-1][0]*getPlusePermm(index);
	}

	//  建立坐标系，设置坐标系参数 
	short error_num = GT_SetCrdPrm(m_cardIndex,iCoorSysIndex,&pCoordInfo->tcrprm); 
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		string err = GTS_Common::GetGtsErrStr(error_num);
		return false;
	}

	if (pCoordInfo->bEnableLookAhead)
	{
		RETURN_V_IF(InitLookAhead(iCoorSysIndex), false);
	}

	GT_CrdClear(m_cardIndex, iCoorSysIndex, 0);
	long lSpace;	//插补缓冲区剩余空间
	error_num = GT_CrdSpace(m_cardIndex, iCoorSysIndex, &lSpace, 0);
	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}
	return true;
}

bool CCard::BufLineXY( int xAxis, int yAxis, double xPos,double yPos )
{
    // 脉冲当量
    long xUnit = getPlusePermm(xAxis>=m_axisCnt?0:xAxis);// start ini 0 
    long yUnit = getPlusePermm(yAxis>=m_axisCnt?0:yAxis);

	int iAxis[2] = {xAxis, yAxis};
	int iCoorSysIndex = FindCoordSys(iAxis, 2);
	RETURN_V_IF(iCoorSysIndex == 0,false);

    int error_num = GT_LnXY(
		m_cardIndex,
        iCoorSysIndex,			//  该插补段的坐标系是坐标系
        xPos*xUnit,
		yPos*yUnit,				//  该插补段的终点坐标(200000, 0) 
		m_interpSpeed.vel,		//  该插补段的目标速度：100pulse/ms 
		m_interpSpeed.acc,		//  插补段的加速度：0.1pulse/ms^2 
		m_interpSpeed.velStop,	//  终点速度为0
        0);						//  向坐标系1的FIFO0缓存区传递该直线插补数据 
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
    return true;
}

bool CCard::BufLineXYZ( int xAxis, int yAxis, int zAxis, double xPos,double yPos,double zPos )
{
    // 脉冲当量
    long xUnit = getPlusePermm(xAxis);
    long yUnit = getPlusePermm(yAxis);
    long zUnit = getPlusePermm(zAxis);

	int iAxis[3] = {xAxis, yAxis, zAxis};
	int iCoorSysIndex = FindCoordSys(iAxis, 3);
	RETURN_V_IF(iCoorSysIndex == 0,false);

    int error_num = GT_LnXYZ(
		m_cardIndex,
        iCoorSysIndex,			//  该插补段的坐标系是坐标系 
        xPos*xUnit, 
		yPos*yUnit, 
		zPos*zUnit,				//  该插补段的终点坐标(200000, 0) 
		m_interpSpeed.vel,		//  该插补段的目标速度：100pulse/ms 
		m_interpSpeed.acc,		//  插补段的加速度：0.1pulse/ms^2 
		m_interpSpeed.velStop,	//  终点速度为0
        0);						//  向坐标系1的FIFO0缓存区传递该直线插补数据 
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
    return true;
}

bool CCard::BufLineXYZU( int xAxis, int yAxis, int zAxis, int uAxis, double xPos,double yPos,double zPos,double uPos )
{
    // 脉冲当量
    long xUnit = getPlusePermm(xAxis);
    long yUnit = getPlusePermm(yAxis);
    long zUnit = getPlusePermm(zAxis);
    long uUnit = getPlusePermm(uAxis);

	int iAxis[4] = {xAxis, yAxis, zAxis, uAxis};
	int iCoorSysIndex = FindCoordSys(iAxis, 4);
	RETURN_V_IF(iCoorSysIndex == 0,false);

    int error_num = GT_LnXYZA(
		m_cardIndex,
        iCoorSysIndex,			//  该插补段的坐标系是坐标系
        xPos*xUnit, 
		yPos*yUnit, 
		zPos*zUnit, 
		uPos*uUnit,				//  该插补段的终点坐标(200000, 0) 
		m_interpSpeed.vel,		//  该插补段的目标速度：100pulse/ms 
		m_interpSpeed.acc,		//  插补段的加速度：0.1pulse/ms^2 
		m_interpSpeed.velStop,	//  终点速度为0
        0);						//  向坐标系1的FIFO0缓存区传递该直线插补数据 
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
    return true;
}

bool CCard::BufArcXY( int xAxis, int yAxis, double xPos1,double yPos1,double xPos2,double yPos2,double xPos3,double yPos3 )
{
    double a1,b1,a2,b2,c1,c2,x,y,x1_tmp,x2_tmp,x3_tmp,y1_tmp,y2_tmp,y3_tmp;
    short circleDir=0;
    long result=0;
    double x1 = xPos1;
    double y1 = yPos1;
    double x2 = xPos2;
    double y2 = yPos2;
    double x3 = xPos3;
    double y3 = yPos3;

    result=(x2-x1)*(y3-y2)-(x3-x2)*(y2-y1);

    if(result==0)
    {
        return false ;
    }
    else if (result>0)
    {
        circleDir=1;
    }
    else
    {
        circleDir=0;
    }

    x1_tmp=(double)x1;
    x2_tmp=(double)x2;
    x3_tmp=(double)x3;
    y1_tmp=(double)y1;
    y2_tmp=(double)y2;
    y3_tmp=(double)y3;

    a1=2*(x2_tmp-x1_tmp);
    a2=2*(x3_tmp-x1_tmp);
    b1=2*(y2_tmp-y1_tmp);
    b2=2*(y3_tmp-y1_tmp);
    c1=x2_tmp*x2_tmp-x1_tmp*x1_tmp+y2_tmp*y2_tmp-y1_tmp*y1_tmp;
    c2=x3_tmp*x3_tmp-x1_tmp*x1_tmp+y3_tmp*y3_tmp-y1_tmp*y1_tmp;

    x=(c1*b2-c2*b1)/(a1*b2-a2*b1);
    y=(c1*a2-c2*a1)/(b1*a2-b2*a1);
    x-=x1_tmp;
    y-=y1_tmp;

    // 脉冲当量
    long xUnit = getPlusePermm(xAxis);
    long yUnit = getPlusePermm(yAxis);

	int iAxis[2] = {xAxis, yAxis};
	int iCoorSysIndex = FindCoordSys(iAxis, 2);
	RETURN_V_IF(iCoorSysIndex == 0,false);

    int error_num =  GT_ArcXYC(
		m_cardIndex, 
		iCoorSysIndex,
		(long)x3*xUnit,
		(long)y3*yUnit,
		x*xUnit,
		y*yUnit,
		circleDir,
		m_interpSpeed.vel,	//  该插补段的目标速度：100pulse/ms 
		m_interpSpeed.acc,	//  插补段的加速度：0.1pulse/ms^2 
		m_interpSpeed.velStop,//  终点速度为0
		0);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }

    return  true;
}

bool CCard::BufArcXYR( int xAxis, int yAxis, double xPos1,double yPos1, double xPos2,double yPos2,double Radius, int idir )
{
	// 脉冲当量
	long xUnit = getPlusePermm(xAxis);
	long yUnit = getPlusePermm(yAxis);
	int axis[2] = {xAxis, yAxis};
	int iCoorSysIndex = FindCoordSys(axis, 2);
	RETURN_V_IF(iCoorSysIndex == 0,false);

	int error_num =  GT_ArcXYR(
		m_cardIndex, 
		iCoorSysIndex,
		(long)xPos2*xUnit,
		(long)yPos2*yUnit,
		Radius*xUnit,
		idir,
		m_interpSpeed.vel,
		m_interpSpeed.acc,
		m_interpSpeed.velStop,
		0);

	if (GTS_Common::GTS_SUCCESS != error_num)
	{
		GTS_Common::GetGtsErrStr(error_num);
		return false;
	}

	return  true;
}

bool CCard::BufDelay(int hContinueInterHandle, int time )
{
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0,false);

    int error_num = GT_BufDelay(m_cardIndex, iCoorSysIndex, time, 0);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return  true;
}

bool CCard::BufIO(int hContinueInterHandle, int index, bool bOpen, int delay/*=0 */ )
{
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0,false);

	unsigned short domask = (0x0001 << index);
	unsigned short dovalue = (unsigned short)!bOpen;
	dovalue = (dovalue << index);

    int error_num = GT_BufIO(m_cardIndex, iCoorSysIndex, MC_GPO, domask, dovalue, 0);
    if (GTS_Common::GTS_SUCCESS != error_num)
    {
        GTS_Common::GetGtsErrStr(error_num);
        return false;
    }
    return  true;
}

bool CCard::ClearBuf(int hContinueInterHandle)
{
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0,false);

    int error_num = GT_CrdClear(m_cardIndex, iCoorSysIndex, 0);
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
    return true;
}

bool CCard::StartContInterp(int hContinueInterHandle)
{
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0,false);
	stCoordInfo* pCoordInfo = GetCoordSysParam(iCoorSysIndex);
	RETURN_V_IF(pCoordInfo == NULL,false);

	int error_num = 0;
	if (pCoordInfo->bEnableLookAhead)
	{
		error_num = GT_CrdData(m_cardIndex, iCoorSysIndex, NULL, 0);
		RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
	}

    error_num = GT_CrdStart(m_cardIndex, iCoorSysIndex, 0);
    RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
    return true;
}

bool CCard::PauseContInterp(int hContinueInterHandle)
{
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0,false);
    stCoordInfo* pCoordInfo = GetCoordSysParam(iCoorSysIndex);
    RETURN_V_IF(pCoordInfo == NULL,false);
    for (int i=0; i<pCoordInfo->listAxis.size(); i++)
	{
        int error_num = GT_Stop(m_cardIndex, 0x01<<(pCoordInfo->listAxis.at(i) - 1), 0);
		RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
	}
    return true;
}

bool CCard::StopContInterp(int hContinueInterHandle)
{
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0,false);
    stCoordInfo* pCoordInfo = GetCoordSysParam(iCoorSysIndex);
    RETURN_V_IF(pCoordInfo == NULL,false);
    for (int i=0; i< pCoordInfo->listAxis.size(); i++)
	{
        int error_num = GT_Stop(m_cardIndex, 0x01<<(pCoordInfo->listAxis.at(i) - 1), 0);
		RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
	}
    return true;
}

bool CCard::IsContInterpFinished(int hContinueInterHandle )
{
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0,true);

    short run; //  坐标系运动状态查询变量 
    long segment;
    //  查询坐标系1的FIFO的插补运动状态 
    short sRtn = GT_CrdStatus( m_cardIndex,
        iCoorSysIndex,		//  坐标系是坐标系1 
        &run,				//  读取插补运动状态 
        &segment,			//  读取当前已经完成的插补段数 
        0);					//  查询坐标系1的FIFO0缓存区 
    //  坐标系在运动,  查询到的run的值为1 
	return (1 == run)?false:true;
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
	int error_num = GT_CompareData(m_cardIndex,encoder,source, pulseType, startLevel,time,pBuf1,count1,pBuf2,count2);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,SYS_ERR_CARD_COMPARE_POS);
	return  SYS_ERR_OK;
}
short CCard::StopCompare()
{
	int error_num = GT_CompareStop(m_cardIndex);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,SYS_ERR_CARD_COMPARE_POS);
	return  SYS_ERR_OK; 
}

bool CCard::setComparePos( int axis, short startLevel, long pluseWidth, double pos1[], int count1, double pos2[], int count2 )
{
    if ((NULL == pos1 && NULL == pos2) || (0 == count1 && 0 == count2))
    {
        return false;
    }

    long unit = getPlusePermm(axis);
    long *lPos1 = NULL;
    long *lPos2 = NULL;
    if (0 != count1)
    {
        lPos1 = new long[count1];
        for (int i=0; i<count1; i++)
        {
            lPos1[i] = pos1[i]*unit;
        }
    }

    if (0 != count2)
    {
        lPos2 = new long[count2];
        for (int i=0; i<count2; i++)
        {
            lPos2[i] = pos2[i]*unit;
        }
    }


    //  初始电平为低电平，1轴编码器位置为1000，2000，3500位置反转HSIO1的电平。 
    short err = GT_CompareData(m_cardIndex,
        axis,          //  对axis轴进行进行位置比较输出 
        1,          //  需要进行比较的数据源为外部编码器 
        0,          //  到达比较位置后输出反转电平 
        startLevel, //  初始状态为低电平 
        pluseWidth, //  脉冲宽度us 
        lPos1,      // HSIO1的比较位置缓冲区数据起始地址 
        count1,     // HSIO1的比较位置缓冲区数据长度 
        lPos2,      // HSIO2的比较位置缓冲区数据起始地址
        count2);    // HSIO2的比较位置缓冲区数据长度 

    return err == GTS_Common::GTS_SUCCESS;
}

bool CCard::stopComparePos( int axis )
{
    short err = GT_CompareStop(m_cardIndex);
    return err == GTS_Common::GTS_SUCCESS;
}

bool CCard::setCompareOutput( short level, short type, short time )
{
    //  在HSIO通道1和通道2输出高电平。 
    short err = GT_ComparePulse(m_cardIndex,
                                level,          // HSIO通道1和通道2输出高电平 
                                type,          //  输出电平 
                                time);          //  无效参数
    return err == GTS_Common::GTS_SUCCESS;

}

bool CCard::GetCompareStatus(short &pStatus, long pCount[])
{
     short err = GT_CompareStatus(m_cardIndex,&pStatus,pCount);
     return err == GTS_Common::GTS_SUCCESS;
}

int CCard::FindCoordSys(int hContinueInterHandle)
{
    map<int ,stCoordInfo>::iterator mapCoorSys;
    for (mapCoorSys = m_mapCoorSysInfo.begin(); mapCoorSys != m_mapCoorSysInfo.end(); mapCoorSys++)
    {
        if (mapCoorSys->first == hContinueInterHandle)
        {
            return hContinueInterHandle;
        }
    }
    return 0;
}

int CCard::FindCoordSys( int AxisIndex[], int iAxisCnt )
{
	RETURN_V_IF(AxisIndex == NULL, 0);

	map<int ,stCoordInfo>::iterator mapCoorSys;
	for (mapCoorSys = m_mapCoorSysInfo.begin(); mapCoorSys != m_mapCoorSysInfo.end(); mapCoorSys++)
	{
		if (mapCoorSys->second.listAxis.size() == iAxisCnt)
		{
			bool bSame = true;
			vector<int>::iterator vecAxis;
			for (int icnt=0; icnt<mapCoorSys->second.listAxis.size(); icnt++)
			{
				if (mapCoorSys->second.listAxis.at(icnt) != AxisIndex[icnt])
				{
					bSame = false;
				}
			}

			if (bSame)
			{
				return mapCoorSys->first;
			}
		}
	}
	return 0;
}

stCoordInfo* CCard::GetCoordSysParam( int iCoorSysIndex )
{
	map<int ,stCoordInfo>::iterator mapCoorSys;
	mapCoorSys = m_mapCoorSysInfo.find(iCoorSysIndex);
	if (mapCoorSys != m_mapCoorSysInfo.end())
	{
		int iaxis[6] = {0};
		vector<int> vecAxis = mapCoorSys->second.listAxis;
		int axiscnt = mapCoorSys->second.listAxis.size();
		for (int i=0; i<axiscnt; i++)
		{
			iaxis[i] = vecAxis.at(i);
		}

		mapCoorSys->second.tcrprm.synVelMax = getCoorSysMaxVec(iaxis, axiscnt);
		mapCoorSys->second.tcrprm.synAccMax = getCoorSysMaxAcc(iaxis, axiscnt);
		return &mapCoorSys->second;
	}

	return NULL;
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
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0,false);
	stCoordInfo* pCoordInfo = GetCoordSysParam(iCoorSysIndex);
	RETURN_V_IF(pCoordInfo == NULL,false);

	pCoordInfo->bEnableLookAhead = bEnable;
	return true;
}

bool CCard::InitLookAhead( int iCoorSysIndex )
{
	RETURN_V_IF((1 > iCoorSysIndex || iCoorSysIndex > 2), false);
	memset(m_szCoordData[iCoorSysIndex-1], 0, sizeof(m_szCoordData[iCoorSysIndex-1]));
	stCoordInfo* pCoordInfo = GetCoordSysParam(iCoorSysIndex);
	RETURN_V_IF(pCoordInfo == NULL,false);

	int error_num = GT_InitLookAhead(m_cardIndex, iCoorSysIndex, 0, 5, pCoordInfo->tcrprm.synAccMax, 512, m_szCoordData[iCoorSysIndex-1]);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,SYS_ERR_CARD_COMPARE_POS);
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
    int iCoorSysIndex = FindCoordSys(hContinueInterHandle);
	RETURN_V_IF(iCoorSysIndex == 0,false);
	stCoordInfo* pCoordInfo = GetCoordSysParam(iCoorSysIndex);
	RETURN_V_IF(pCoordInfo == NULL,false);

	int error_num = GT_BufGear(m_cardIndex, iCoorSysIndex, iprofile, lpulse, icrd);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,SYS_ERR_CARD_COMPARE_POS);
	return true;
}

bool CCard::SetCompareLiner( short encoder, short channel,double startPos,long repeatTimes,double interval,short time,short source )
{
	long sPos = startPos * getPlusePermm(encoder-1);
	long unit = interval * getPlusePermm(encoder-1);
	int error_num = GT_CompareLinear(m_cardIndex, encoder, channel, sPos, repeatTimes, unit, time, source);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
	return true;
}

bool CCard::SetComparePluse( short level,short outputType, unsigned int time )
{
	int error_num = GT_ComparePulse(m_cardIndex, level, outputType, time);

	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);
	return true;
}

bool CCard::SetPrfMapAxis( short encoder, short prfMapAxis )
{
	TAxisConfig config;

	int error_num = GT_GetAxisConfig(m_cardIndex, encoder, &config);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);


	config.prfMap = (0x01 << (prfMapAxis-1));
	error_num = GT_SetAxisConfig(m_cardIndex, encoder, &config);
	RETURN_V_IF(GTS_Common::GTS_SUCCESS != error_num,false);

	return true;
}

int CCard::GetCompareCount()
{
	GT_CompareStatus(m_cardIndex, m_status, m_count);
	return m_count[0];
}
bool CCard::CompareMode2D( short chn,short mode )
{
	short err = GT_2DCompareMode(m_cardIndex,chn,mode);
	return err == GTS_Common::GTS_SUCCESS;
}

bool CCard::ComparePulse2D( short chn,short level,short outputType,short time )
{
	short err =GT_2DComparePulse(m_cardIndex,chn,level, outputType, time );
	return err == GTS_Common::GTS_SUCCESS;
}

bool CCard::CompareStop2D( short chn )
{
	short err =GT_2DCompareStop( m_cardIndex, chn);
	return err == GTS_Common::GTS_SUCCESS;
}

bool CCard::CompareClear2D( short chn )
{
	short err =GT_2DCompareClear( m_cardIndex, chn);
	return err == GTS_Common::GTS_SUCCESS;
}

bool CCard::CompareStatus2D( short chn,short *pStatus,long *pCount,short *pFifo,short *pFifoCount,short *pBufCount /*=NULL*/ )
{
	short err =GT_2DCompareStatus( m_cardIndex, chn,pStatus, pCount, pFifo, pFifoCount, pBufCount);
	return err == GTS_Common::GTS_SUCCESS;
}

bool CCard::CompareSetPrm2D( short chn,CardGGT2DComparePrm *pPrm )
{
	short err =GT_2DCompareSetPrm( m_cardIndex, chn,(T2DComparePrm*)pPrm);
	return err == GTS_Common::GTS_SUCCESS;
}

bool CCard::CompareData2D( short chn,short count,CardGGT2DCompareData *pBuf,short fifo )
{

	short err =GT_2DCompareData( m_cardIndex, chn,count, (T2DCompareData*)pBuf, fifo);
	return err == GTS_Common::GTS_SUCCESS;
}

bool CCard::CompareStart2D( short chn )
{
	short err =GT_2DCompareStart( m_cardIndex, chn);
	return err == GTS_Common::GTS_SUCCESS;
}

bool CCard::SetComparePort2D( short channel,short hsio0,short hsio1 )
{
	short err =GT_SetComparePort( m_cardIndex, channel, hsio0, hsio1);
	return err == GTS_Common::GTS_SUCCESS;
}
bool CCard::SetCompareContinuePulseMode(short mode, short count, short standTime)
{
	short err = GT_CompareContinuePulseMode(m_cardIndex,  mode,  count,  standTime);
	return err == GTS_Common::GTS_SUCCESS;
}

