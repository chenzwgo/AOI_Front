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

	/***********************  ����״̬���    *****************/
	static bool IsStopSig();
	static bool IsEstopSig();
	static bool IsResetSig();
	static bool IsPauseSig();
	static bool IsStartSig();
	static bool IsPowerOffSig();	// �豸�Ƿ�ϵ�
	static bool IsSafeDoorSig();	// ��ȫ�ſ���
	static bool IsCardOnline();		// ���˶��Ƿ����
	static bool OpenCard();
	static bool SrvOnCard();         // ��������ʹ��
	static bool SrvOffCard();        // ����ͣ��ʹ��


	static void SetSignal(E_SYS_STATE signal);

	static bool readIo(const char* ioName);    // ������IO
	static bool writeIo(const char* ioName, bool bOpen);   // д���IO
	static bool OpenLight(const char* ioName);
	static bool readIoEx(const char* ioName,bool bFlag,int iDurTime = 50);    // ������IO
    static bool readIoEx(const char* ioName,bool bFlag,int iDurTime = 50,int tryTime = 0);    // ������IO


    static bool isCleanRunning();
    static bool isShangRunning();
    static bool isXiaRunning();
    static bool isClearMode();
	static bool isXiaRunning1();

    static void setCleanOn(bool bON);
    static void setCleanPause(bool bPause);
    static void setShangOn();
	static void setShangStop(bool bstop=true);//�Ķ�ֹͣtrue һֱ���ֹͣ�ź�/ false �ر����
    static void setXiaStop();

	static bool CrlLightToRobot(const char* ioName);
	static bool CloseLightToRobot();
private:
	
	static bool IsUIClick(E_SYS_STATE signal); // UI�������

private:
	static E_SYS_STATE m_state;
    static iMotion* m_pImotion;
    static IIOCtrlInterface *m_pIo;
};

#endif
