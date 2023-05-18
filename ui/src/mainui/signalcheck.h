#ifndef SIGNALCHECK_H
#define SIGNALCHECK_H

#include "maindefine.h"

class iMotion;
class IIOCtrlInterface;
class SignalCheck
{
public:
	SignalCheck(void);
	~SignalCheck(void);

	/***********************  输入状态监测    *****************/
	static bool IsStopSig();
	static bool IsEstopSig();
	static bool IsResetSig();
	static bool IsPauseSig();
	static bool IsStartSig();
	static bool IsPowerOffSig();	// 设备是否断电
	static bool IsSafeDoorSig();	// 安全门开启
	static bool IsCardOnline();		// 正运动是否掉线
	static bool OpenCard();
	static bool SrvOnCard();         // 卡所有轴使能
	static bool SrvOffCard();        // 卡急停断使能


	static void SetSignal(E_SYS_STATE signal);

	static bool readIo(const char* ioName);    // 读输入IO
	static bool writeIo(const char* ioName, bool bOpen);   // 写输出IO
	static bool OpenLight(const char* ioName);
	static bool readIoEx(const char* ioName,bool bFlag,int iDurTime = 50);    // 读输入IO
    static bool readIoEx(const char* ioName,bool bFlag,int iDurTime = 50,int tryTime = 0);    // 读输入IO


    static bool isCleanRunning();
    static bool isShangRunning();
    static bool isXiaRunning();
    static bool isClearMode();
	static bool isXiaRunning1();

    static void setCleanOn(bool bON);
    static void setCleanPause(bool bPause);
    static void setShangOn();
	static void setShangStop(bool bstop=true);//改动停止true 一直输出停止信号/ false 关闭输出
    static void setXiaStop();

	static bool CrlLightToRobot(const char* ioName);
	static bool CloseLightToRobot();
private:
	
	static bool IsUIClick(E_SYS_STATE signal); // UI点击触发

private:
	static E_SYS_STATE m_state;
    static iMotion* m_pImotion;
    static IIOCtrlInterface *m_pIo;
};

#endif
