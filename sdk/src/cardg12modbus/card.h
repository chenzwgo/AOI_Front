#ifndef _G12_CARD_H_
#define _G12_CARD_H_

#include "motor_common.h"
#include "card/icard.h"
#include <vector>
#include <map>
#include "modbuscom.h"
#include "card/icardcontinterp.h"

using std::vector;


class IAxis;
class IIO;

class CCard:public ICard, public ICardContInterp
{
public:
	/**
	 * @brief 构造函数，不在构造函数中进行卡的初始化，需调用init()
	 * @param cardIndex				-- [in]卡号，取值范围：[0,7]
	 * @param axisCnt					-- [in]轴号，取值范围：[0,7]
	 * @param extIndex				-- [in]扩展卡数量,0为本卡
	 */
	 CCard();
	~CCard();

	bool Init(unsigned short cardIndex, unsigned short axisCnt, unsigned short extIndex);					// 初始化卡
	bool Deinit();					// 释放卡
public:
	
	virtual IAxis* GetAxis(unsigned short index);	// 获取轴
	virtual IIO* GetIO(unsigned short index);		// 获取IO

	// 运动控制卡的基本操作
	
	virtual bool Open();					// 打开运动卡
	virtual bool Close();					// 关闭运动卡
	virtual bool Reset();					// 复位
	virtual unsigned int GetCardIndex();	// 获取卡索引
	virtual unsigned int GetAxisCnt();		// 获取轴数量
	virtual bool IsConnected();				// 判断卡是否连接控制器
    virtual bool EmgStop();
    virtual bool CardHome( int homeAxis[], int axisCount ); //卡回原，传入的各轴同时回原
    virtual EnMoveBool CardHomeStatus(int homeAxis[], int axisCount);//卡回原状态，MOVE_DOING回原中/MOVE_TRUE回原完成/MOVE_FALSE回原失败

    // 插补
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
    // 位置比较输出电平
    // 设置比较位置 axis:轴号，startLevel：初始电平, pluseWidth:脉冲宽度,1脉冲1us 
    // pos1:触发HSIO通道1的位置列表，count1:位置个数，pos2:触发HSIO通道2的位置列表，count2:位置个数，
    virtual bool setComparePos(int axis, short startLevel, long pluseWidth, double pos1[], int count1, double pos2[], int count2);

    // 取消位置比较输出
    virtual bool stopComparePos(int axis);

    //设置自动触发输出(level无用，type为0表示连续触发,为1表示按行数触发,time表示行数
    virtual bool setCompareOutput( short level, short type, short time );

    virtual bool GetCompareStatus(short &pStatus,long pCount[2]) ;

	// 等间距触发(channel无效,time无效,source无效)
	virtual bool SetCompareLiner(short encoder, short channel,double startPos,long repeatTimes,double interval,short time,short source);

	virtual bool SetComparePluse(short level,short outputType, uint time);

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
    bool homeinit();										// 初始化自动回原点功能
	bool SetCoorSysParam( int axis[], int count );			// 初始化插补参数
    bool loadConfig(char* path);							// 加载卡配置
    bool saveConfig(char* path);							// 保存卡配置(不支持)
	long getPlusePermm(short axisIndex);					// 获取脉冲当量
	double getCoorSysMaxVec(int AxisIndex[], int iAxisCnt);	// 获取坐标系最大速度pulse/ms^2
	double getCoorSysMaxAcc(int AxisIndex[], int iAxisCnt);	// 获取坐标系最大加速度pulse/ms^2
    double getcurPos(short axisIndex);						// 获取轴当前位置
	int FindCoordSys(int AxisIndex[], int iAxisCnt);		// 查找坐标系
    int FindCoordSys(int hContinueInterHandle);
	bool InitLookAhead(int iCoorSysIndex);					// 初始化前瞻预处理功能
public:
	unsigned short m_cardIndex;				// 卡号
	unsigned short m_axisCnt;					// 轴数量
	unsigned short m_extIndex;				// 固高扩展模块地址码，0代表本卡，>1为相应的扩展卡号(1扩展卡，2扩展卡...)

	vector<IAxis *> m_vec_Axis;				// 轴实例句柄容器,本卡上所有的轴都存着里面
	vector<IIO *> m_vec_IO;					// IO实例句柄容器,本卡上所有的IO都存着里面
    vector<IAxis*> m_homeAxis;					// 回原的轴列表
private:
	bool m_bOpened;							// 打开卡标识
	bool m_bhoming;							// 在回原点中
	bool m_bConfigLoaded;						// 加载配置文件标识
	bool m_bInit;							// 初始化标识
	StSpeed m_interpSpeed;					// 插补速度
	int m_iCoorSysCnt;						// 坐标系个数
	double m_szCoordOffset[2][4];				// 坐标系偏移
	short m_status[2];
	long m_count[2];
};
#endif
