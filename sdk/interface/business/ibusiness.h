#ifndef IBUSINESS_H
#define IBUSINESS_H

#include "StatusEnumInfo.h"

class IBusiness
{
public:
	enum
	{
        IDD_BUSINESS = 0x001
	};

    virtual ~IBusiness(){}
	// ��ʼ��
	virtual bool init() = 0;
	// ȥ��ʼ��
	virtual bool deinit() = 0;
	// ��ʼ
	virtual bool StartWork() = 0;
	// ��ͣ
	virtual bool PauseWork() = 0;
	// ����
	virtual bool ContinueWork() = 0;
	// ֹͣ
	virtual bool StopWork() = 0;
	// ��ͣ
	virtual bool EstopWork() = 0;
	// ��λ
	virtual bool ResetWork() = 0;
	// ����״̬
    virtual E_MAIN_WORK_STATE GetWorkState() = 0;
};

#endif // IWORKFLOW

