#ifndef _G12_AXIS_H_
#define _G12_AXIS_H_

#include "card/iaxis.h"
#include "jthreadt.h"
#include "motor_common.h"
#include "modbuscom.h"

class CAxis:public IAxis
{
public:
	//���ڹ��캯�����������ĳ�ʼ������Ҫ����init()
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

	virtual bool setAlarmOff();						// ���������������ź���Ч
	virtual bool setAlarmOn();						// ���������������ź���Ч
	virtual bool setLmtsOff();						// ��������λ�ź���Ч
	virtual bool setLmtsOn();							// ��������λ�ź���Ч
	virtual bool SetPos(EnPosMode model, double dValue);
private:
    virtual bool setVel(double vel);				// ���ñ������ٶ�
    virtual bool getVel(double &vel);				// ��ȡ�������ٶ�
    bool homeSts(unsigned short *pStatus);  // ��ȡ��ԭ��״̬ 
    int getPrfMode();                       // ��ȡ�˶�ģʽ��1��Jogģʽ 2��PTģʽ 

    bool setEncOn();						// ����Ϊ���ⲿ���������ļ�����ʽ 
    bool setEncOff();						// ����Ϊ ������������� �ļ�����ʽ
    bool setEncSns(int dir);				// ���ñ������ļ�������,0���ñ�������������ȡ��,1���ñ�������������ȡ��
    bool setCtrlMode( unsigned short mode);	// ���ÿ�����Ϊģ����������������0����ָ�����л�Ϊ�ջ�����ģʽ(��ѹ���Ʒ�ʽ),1����ָ�����л�Ϊ��������ģʽ(������Ʒ�ʽ)
    bool setStepDir();						// ���������ͨ�����������ģʽ����Ϊ������+����

    int homeFun();							// ��ԭ��
    void* homeThread(void* param);			// ��ԭ�̣߳�paramΪ this

    virtual bool index();							// �����Զ���ԭ�㹦��Ϊhome+indexģʽ

    bool setPtpSpeed(StSpeed &speed);
    bool setJogSpeed(StSpeed &speed, bool bDirect = true);
    
	int ggHome();									// �̸߿��Զ���ԭ 
	int ggUserHome();									// �̸߿��ֶ���ԭ 

	bool MoveToLimitNUserDefine();						// �Զ���ظ���
	bool MoveToLimitPUserDefine();						// �Զ��������

	bool MoveToLimitNProcess(bool bFindNLimit = true);	// �ƶ�������
	bool MoveToLimitPProcess(bool bFindNLimit = true);	// �ƶ�������

	bool LeaveLimit(int iSignal);
	int HomeLimitNProcess();							// ����+ԭ��
	int HomeLimitPProcess();							// ����+ԭ��
	int HomeOriginProcess(EnHomeMode homemode);			// ԭ��
	int HomeIndexProcess();							// Ѱ��Index
	bool LimitMoveOffset();							// ��λƫ��

    string getComName();

private:
	unsigned short m_cardIndex;				// ����,ȡֵ��Χ��[0,7]
	unsigned short m_axisIndex;				// ���,ȡֵ��Χ��[0,7]

	bool m_bOn;								// ��ʹ�ܱ�ʶ
	bool m_bInit;							// ���ʼ����ʶ

    JThreadTP<CAxis, void *, void *>*m_hThreadHome;       //��ԭ�߳̾��
    EnMoveBool    m_bHomeResult;					// ��ԭ���
    StAxisParam m_pAxisParam;				// �������Ϣ
    StSpeed m_stSpeed;                    // �ٶ���Ϣ
    CModbusCom * m_pCom;                  // modbus���
};
#endif
