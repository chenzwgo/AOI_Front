#ifndef _GG_CARD_H_
#define _GG_CARD_H_

#include "motor_common.h"
#include "card/icard.h"
#include "card/icardcontinterp.h"
#include "gts.h"
#include <vector>
#include <map>
using std::vector;

//����ϵ���ԣ����������Ŀ��ź�����ϵ�ţ��岹�˶�����ٶȣ����ٶȣ�����ϵ��������
typedef struct _CoordInfo
{
	int iCoorSysIndex;		// ����ϵ��
	vector<int> listAxis;		// ���������
	TCrdPrm tcrprm;			// �岹����
	bool bEnableLookAhead;
	_CoordInfo()
	{
		iCoorSysIndex = 0;
		bEnableLookAhead = false;
		memset(&tcrprm,0,sizeof(tcrprm));
	}

	_CoordInfo &operator=(_CoordInfo copy) 
	{
		iCoorSysIndex = copy.iCoorSysIndex;
		listAxis = copy.listAxis;
		bEnableLookAhead = copy.bEnableLookAhead;
		tcrprm.dimension = copy.tcrprm.dimension;
		tcrprm.evenTime = copy.tcrprm.evenTime;
		tcrprm.synAccMax = copy.tcrprm.synAccMax;
		tcrprm.synVelMax = copy.tcrprm.synVelMax;
		tcrprm.profile[0] = copy.tcrprm.profile[0];
		tcrprm.profile[1] = copy.tcrprm.profile[1];
		tcrprm.profile[2] = copy.tcrprm.profile[2];
		tcrprm.profile[3] = copy.tcrprm.profile[3];
		tcrprm.profile[4] = copy.tcrprm.profile[4];
		tcrprm.profile[5] = copy.tcrprm.profile[5];
		tcrprm.profile[6] = copy.tcrprm.profile[6];
		tcrprm.profile[7] = copy.tcrprm.profile[7];
		tcrprm.setOriginFlag = copy.tcrprm.setOriginFlag;
		tcrprm.originPos[0] = copy.tcrprm.originPos[0];
		tcrprm.originPos[1] = copy.tcrprm.originPos[1];
		tcrprm.originPos[2] = copy.tcrprm.originPos[2];
		tcrprm.originPos[3] = copy.tcrprm.originPos[3];
		tcrprm.originPos[4] = copy.tcrprm.originPos[4];
		tcrprm.originPos[5] = copy.tcrprm.originPos[5];
		tcrprm.originPos[6] = copy.tcrprm.originPos[6];
		tcrprm.originPos[7] = copy.tcrprm.originPos[7];
		return *this;
	}

}stCoordInfo;

class IAxis;
class IIO;

class CCard:public ICard, public ICardContInterp
{
public:
	/**
	 * @brief ���캯�������ڹ��캯���н��п��ĳ�ʼ���������init()
	 * @param cardIndex				-- [in]���ţ�ȡֵ��Χ��[0,7]
	 * @param axisCnt					-- [in]��ţ�ȡֵ��Χ��[0,7]
	 * @param extIndex				-- [in]��չ������,0Ϊ����
	 */
	 CCard();
	~CCard();

	bool Init(unsigned short cardIndex, unsigned short axisCnt, unsigned short extIndex);					// ��ʼ����
	bool Deinit();					// �ͷſ�
public:
	
	virtual IAxis* GetAxis(unsigned short index);	// ��ȡ��
	virtual IIO* GetIO(unsigned short index);		// ��ȡIO

	// �˶����ƿ��Ļ�������
	
	virtual bool Open();					// ���˶���
	virtual bool Close();					// �ر��˶���
	virtual bool Reset();					// ��λ
	virtual unsigned int GetCardIndex();	// ��ȡ������
	virtual unsigned int GetAxisCnt();		// ��ȡ������
	virtual bool IsConnected();				// �жϿ��Ƿ����ӿ�����
    virtual bool EmgStop();
    virtual bool CardHome( int homeAxis[], int axisCount ); //����ԭ������ĸ���ͬʱ��ԭ
    virtual EnMoveBool CardHomeStatus(int homeAxis[], int axisCount);//����ԭ״̬��MOVE_DOING��ԭ��/MOVE_TRUE��ԭ���/MOVE_FALSE��ԭʧ��

    // �岹
    virtual bool InitContinueInter(int axis[], int count ,int &hContinueInterHandle);

    virtual bool SetCoordinateOffset(int hContinueInterHandle, double dOffset[4]);

    virtual bool LineXY( int xAxis, int yAxis, double xPos,double yPos );

    virtual bool LineXYZ( int xAxis, int yAxis, int zAxis, double xPos,double yPos,double zPos );

    virtual bool LineXYZU( int xAxis, int yAxis, int zAxis, int uAxis, double xPos,double yPos,double zPos,double uPos );

    virtual bool ArcXY( int xAxis, int yAxis, double xPos1,double yPos1,double xPos2,double yPos2,double xPos3,double yPos3 );

    virtual bool SetContinueInterSpeed(int hContinueInterHandle, const StSpeed &speed );

    virtual bool BufLineXY( int xAxis, int yAxis, double xPos,double yPos );

    virtual bool BufLineXYZ( int xAxis, int yAxis, int zAxis, double xPos,double yPos,double zPos );

    virtual bool BufLineXYZU( int xAxis, int yAxis, int zAxis, int uAxis, double xPos,double yPos,double zPos,double uPos );

    virtual bool BufArcXY( int xAxis, int yAxis, double xPos1,double yPos1,double xPos2,double yPos2,double xPos3,double yPos3 );
	
	virtual bool BufArcXYR(int xAxis, int yAxis, double xPos1,double yPos1, double xPos2,double yPos2,double Radius, int idir);
   
    virtual bool BufDelay( int hContinueInterHandle, int time );

    virtual bool BufDA(int hContinueInterHandle, int channel, short value);

    virtual bool BufIO(int hContinueInterHandle, int index, bool bOpen, int delay=0 );

    virtual bool ClearBuf(int hContinueInterHandle);

    virtual bool StartContInterp(int hContinueInterHandle);

    virtual bool PauseContInterp(int hContinueInterHandle);

    virtual bool StopContInterp(int hContinueInterHandle);

    virtual bool IsContInterpFinished(int hContinueInterHandle);

    virtual bool EnableLookAhead(int hContinueInterHandle, bool bEnable);

    virtual int  GetCurSegNum(int hContinueInterHandle);

    virtual bool IsBufFull(int hContinueInterHandle);

    virtual bool BufGear(int hContinueInterHandle, int iprofile, long lpulse, int icrd=0);

	virtual short SetCompareData(short encoder, short source, short pulseType, short startLevel, short time, long *pBuf1, short count1, long *pBuf2, short count2);
	virtual short StopCompare();
    // λ�ñȽ������ƽ
    // ���ñȽ�λ�� axis:��ţ�startLevel����ʼ��ƽ, pluseWidth:������,1����1us 
    // pos1:����HSIOͨ��1��λ���б�count1:λ�ø�����pos2:����HSIOͨ��2��λ���б�count2:λ�ø�����
    virtual bool setComparePos(int axis, short startLevel, long pluseWidth, double pos1[], int count1, double pos2[], int count2);

    // ȡ��λ�ñȽ����
    virtual bool stopComparePos(int axis);

    //ֱ�����
    //������ֵΪ1�� 
    //����Ƿ��Ѿ�����λ�ñȽ�����������ڽ���λ�ñȽ�������޷���HSIO�����
    //��ƽ�����塣 
    virtual bool setCompareOutput( short level, short type, short time );

    virtual bool GetCompareStatus(short &pStatus,long pCount[2]) ;

	virtual bool SetCompareLiner(short encoder, short channel,double startPos,long repeatTimes,double interval,short time,short source);

	virtual bool SetComparePluse(short level,short outputType, unsigned int time);

	virtual bool SetPrfMapAxis(short encoder, short prfMapAxis);

	virtual int GetCompareCount();
	//
	virtual bool  CompareMode2D(short chn,short mode) ;
	virtual bool  ComparePulse2D( short chn,short level,short outputType,short time) ;
	virtual bool  CompareStop2D(short chn) ;
	virtual bool  CompareClear2D( short chn) ;
	virtual bool  CompareStatus2D(short chn,short *pStatus,long *pCount,short *pFifo,short *pFifoCount,short *pBufCount =NULL) ;
	virtual bool  CompareSetPrm2D( short chn,CardGGT2DComparePrm *pPrm) ;
	virtual bool  CompareData2D( short chn,short count,CardGGT2DCompareData *pBuf,short fifo) ;
	virtual bool  CompareStart2D(short chn) ;
	virtual bool  SetComparePort2D(short channel,short hsio0,short hsio1) ;
	virtual bool SetCompareContinuePulseMode(short mode, short count, short standTime);
	//
private:
    bool homeinit();										// ��ʼ���Զ���ԭ�㹦��
	bool SetCoorSysParam( int axis[], int count );			// ��ʼ���岹����
    bool loadConfig(char* path);							// ���ؿ�����
    bool saveConfig(char* path);							// ���濨����(��֧��)
	long getPlusePermm(short axisIndex);					// ��ȡ���嵱��
	double getCoorSysMaxVec(int AxisIndex[], int iAxisCnt);	// ��ȡ����ϵ����ٶ�pulse/ms^2
	double getCoorSysMaxAcc(int AxisIndex[], int iAxisCnt);	// ��ȡ����ϵ�����ٶ�pulse/ms^2
    double getcurPos(short axisIndex);						// ��ȡ�ᵱǰλ��
	int FindCoordSys(int AxisIndex[], int iAxisCnt);		// ��������ϵ
    int FindCoordSys(int hContinueInterHandle);
	stCoordInfo* GetCoordSysParam(int iCoorSysIndex);		// ��ȡ����ϵ����
	bool InitLookAhead(int iCoorSysIndex);					// ��ʼ��ǰհԤ������
public:
	unsigned short m_cardIndex;				// ����
	unsigned short m_axisCnt;					// ������
	unsigned short m_extIndex;				// �̸���չģ���ַ�룬0��������>1Ϊ��Ӧ����չ����(1��չ����2��չ��...)

	vector<IAxis *> m_vec_Axis;				// ��ʵ���������,���������е��ᶼ��������
	vector<IIO *> m_vec_IO;					// IOʵ���������,���������е�IO����������
    vector<IAxis*> m_homeAxis;					// ��ԭ�����б�
	map<int ,stCoordInfo> m_mapCoorSysInfo;		// ����ϵ������ ����Ϣ
private:
	bool m_bOpened;							// �򿪿���ʶ
	bool m_bhoming;							// �ڻ�ԭ����
	bool m_bConfigLoaded;						// ���������ļ���ʶ
	bool m_bInit;							// ��ʼ����ʶ
	StSpeed m_interpSpeed;					// �岹�ٶ�
	int m_iCoorSysCnt;						// ����ϵ����
	TCrdData m_szCoordData[2][512];			// ǰհԤ��������
	double m_szCoordOffset[2][4];				// ����ϵƫ��
	short m_status[2];
	long m_count[2];
};
#endif
