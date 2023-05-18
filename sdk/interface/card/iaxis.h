#ifndef _AXIS_INTERFACE_H_
#define _AXIS_INTERFACE_H_

#include "motor_common.h"

class IAxis
{
public:
	//���ڹ��캯�����������ĳ�ʼ������Ҫ����init()
	enum
	{
		IID_AXIS = 0x302
	};
	virtual ~IAxis(){}

	virtual bool Init() = 0;	// ���ʼ��
	virtual bool On() = 0;		// ��ʹ��
	virtual bool Off() = 0;		// ��ر�

	virtual bool SetAxisParam(StAxisParam* pAxisParam) = 0;		//���������
	virtual bool GetAxisParam(StAxisParam* pAxisParam) = 0;		//��ȡ�����
	virtual bool SetLmtValue(double limitP, double limitN) = 0;		//��������������λ�͸�������λ
	virtual bool GetLmtValue(double &limitP, double &limitN) = 0;	//��ȡ����������λ�͸�������λ
	
	virtual bool SetSpeed(const StSpeed &speed) = 0;    // �����ٶ�
	virtual bool GetSpeed(StSpeed &speed) = 0;           // ��ȡ�ٶ�
	virtual bool MovePosRel(double pos) = 0;            // �ƶ���ָ��λ��,���λ��
	virtual bool MovePosAbs(double pos) = 0;            // �ƶ���ָ��λ��,����λ��
	virtual bool MoveSucceed(double pos) = 0;		    // �Ƿ��Ѿ��ƶ���ָ��λ��
	virtual bool GetCurPos(double &pos) = 0;		    // ��ȡ��ǰλ��
	virtual bool GetActualPos(double &pos) = 0;         // ��ȡʵ��λ��,ר���ڿ�IO��������
	virtual bool GetCommandPos(double &pos) = 0;        // ��ȡ�滮λ��,ר���ڿ�IO��������

	virtual bool MoveJog(bool bDirection = true) = 0;   // JOG�˶���Ĭ��������
	
	virtual bool EStop() = 0;							// ��ͣ
	virtual bool SmoothStop() = 0;						// ƽ��ֹͣ
	virtual bool SetPosZero() = 0;						// λ������
	virtual bool ClearStatus() = 0;                     // ���״̬
	virtual bool ClearAlarm() = 0;                     // �������
	virtual bool IsMoving() = 0;						// ���Ƿ����ƶ���
	virtual bool IsOn() = 0;							// �ж����Ѿ���
	virtual bool IsLimitP() = 0;						// �ж��Ƿ񵽴�����λ
	virtual bool IsLimitN() = 0;                        // �ж��Ƿ񵽴︺��λ
	virtual bool IsDrvAlarm() = 0;                      // �ж�����������
	virtual bool IsHomeSig() = 0;						// �ж�ԭ���ź�

	//��ԭ
	virtual bool MoveToLimitN(bool bFindNLimit = true) = 0;	// �ƶ�������λ,true-�����ҵ���λ��false-�ҵ�ԭ��ͣ��
	virtual bool MoveToLimitP(bool bFindNLimit = true) = 0;	// �ƶ�������λ,true-�����ҵ���λ��false-�ҵ�ԭ��ͣ��
	virtual bool Home() = 0;							// ��ԭ��
	virtual EnMoveBool HomeStatus() = 0;					// �Ƿ��ԭ���
	virtual bool SetPos(EnPosMode model, double dValue) = 0;	// ����λ��
};

#endif
