#ifndef _G12_AXIS_H_
#define _G12_AXIS_H_

#include "card/iaxis.h"
#include "jthreadt.h"
#include "motor_common.h"
#include "modbuscom.h"

class CAxis:public IAxis
{
public:
	//不在构造函数里面进行轴的初始化，需要调用init()
	CAxis(unsigned short cardIndex, unsigned short axisIndex);
	virtual ~CAxis();

    virtual bool Init();

    virtual bool On();

    virtual bool Off();

    virtual bool SetAxisParam( StAxisParam* pAxisParam );

    virtual bool GetAxisParam( StAxisParam* pAxisParam );

    virtual bool SetLmtValue( double limitP, double limitN );

    virtual bool GetLmtValue( double &limitP, double &limitN );

    virtual bool SetSpeed( const StSpeed &speed );

    virtual bool GetSpeed( StSpeed &speed );

    virtual bool MovePosRel( double pos );

    virtual bool MovePosAbs( double pos );

    virtual bool MoveSucceed( double pos );

    virtual bool GetCurPos( double &pos );

    virtual bool GetActualPos( double &pos );

    virtual bool GetCommandPos( double &pos );

    virtual bool MoveJog( bool bDirection = true );

    virtual bool EStop();

    virtual bool SmoothStop();

    virtual bool SetPosZero();

    virtual bool ClearStatus();

	virtual bool ClearAlarm();

    virtual bool IsMoving();

    virtual bool IsOn();

    virtual bool IsLimitP();

    virtual bool IsLimitN();

    virtual bool IsDrvAlarm();

    virtual bool IsHomeSig();

	virtual bool MoveToLimitN(bool bFindNLimit = true);

	virtual bool MoveToLimitP(bool bFindNLimit = true);

    virtual bool Home();

    virtual EnMoveBool HomeStatus();

	virtual bool setAlarmOff();						// 控制轴驱动报警信号无效
	virtual bool setAlarmOn();						// 控制轴驱动报警信号有效
	virtual bool setLmtsOff();						// 控制轴限位信号无效
	virtual bool setLmtsOn();							// 控制轴限位信号有效
	virtual bool SetPos(EnPosMode model, double dValue);
private:
    virtual bool setVel(double vel);				// 设置编码器速度
    virtual bool getVel(double &vel);				// 获取编码器速度
    bool homeSts(unsigned short *pStatus);  // 获取回原点状态 
    int getPrfMode();                       // 获取运动模式，1：Jog模式 2：PT模式 

    bool setEncOn();						// 设置为“外部编码器”的计数方式 
    bool setEncOff();						// 设置为 “脉冲计数器” 的计数方式
    bool setEncSns(int dir);				// 设置编码器的计数方向,0：该编码器计数方向不取反,1：该编码器计数方向取反
    bool setCtrlMode( unsigned short mode);	// 设置控制轴为模拟量输出或脉冲输出0：将指定轴切换为闭环控制模式(电压控制方式),1：将指定轴切换为开环控制模式(脉冲控制方式)
    bool setStepDir();						// 将脉冲输出通道的脉冲输出模式设置为“脉冲+方向”

    int homeFun();							// 回原点
    void* homeThread(void* param);			// 回原线程，param为 this

    virtual bool index();							// 设置自动回原点功能为home+index模式

    bool setPtpSpeed(StSpeed &speed);
    bool setJogSpeed(StSpeed &speed, bool bDirect = true);
    
	int ggHome();									// 固高卡自动回原 
	int ggUserHome();									// 固高卡手动回原 

	bool MoveToLimitNUserDefine();						// 自定义回负限
	bool MoveToLimitPUserDefine();						// 自定义回正限

	bool MoveToLimitNProcess(bool bFindNLimit = true);	// 移动至负限
	bool MoveToLimitPProcess(bool bFindNLimit = true);	// 移动至正限

	bool LeaveLimit(int iSignal);
	int HomeLimitNProcess();							// 负限+原点
	int HomeLimitPProcess();							// 正限+原点
	int HomeOriginProcess(EnHomeMode homemode);			// 原点
	int HomeIndexProcess();							// 寻找Index
	bool LimitMoveOffset();							// 限位偏移

    string getComName();

private:
	unsigned short m_cardIndex;				// 卡号,取值范围：[0,7]
	unsigned short m_axisIndex;				// 轴号,取值范围：[0,7]

	bool m_bOn;								// 轴使能标识
	bool m_bInit;							// 轴初始化标识

    JThreadTP<CAxis, void *, void *>*m_hThreadHome;       //回原线程句柄
    EnMoveBool    m_bHomeResult;					// 回原结果
    StAxisParam m_pAxisParam;				// 轴参数信息
    StSpeed m_stSpeed;                    // 速度信息
    CModbusCom * m_pCom;                  // modbus句柄
};
#endif
