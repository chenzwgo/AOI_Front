#ifndef IMAINFLOWCFG_H
#define IMAINFLOWCFG_H

#include "workflowdefine.h"

class iMainFlowCfg
{
public:
	enum
	{
		IID_MAINFLOW_CFG = 0x002
	};

	virtual ~iMainFlowCfg(){}

    /************************************************************************/
    // @brief �ж������ļ��Ƿ����޸�
    // @return bool
    /************************************************************************/
	virtual bool IsModify( void ) = 0;

    /************************************************************************/
    // @brief ����һ����������
    // @param wIndex					-- [in ]��������
    // @param pstWork					-- [in ]��������
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int InsertWork(int wIndex, const WORK_FLOW* pstWork) = 0;

    /************************************************************************/
    // @brief ����һ������
    // @param wIndex					-- [in ]��������
    // @param sIndex					-- [in ]��������
    // @param pstStep					-- [in ]��������
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int InsertStep(int wIndex, int sIndex, const WORK_STEP* pstStep) = 0;

    /************************************************************************/
    // @brief ����һ������
    // @param wIndex					-- [in ]��������
    // @param sIndex					-- [in ]��������
    // @param pIndex					-- [in ]��������
    // @param pstItem					-- [in ]��������
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int InsertAction(int wIndex, int sIndex, int pIndex, const WORK_ACTION* pstItem) = 0;

    /************************************************************************/
    // @brief �������Ƿ����
    // @param wIndex					-- [in ]��������
    // @param name					    -- [in ]��������
    // @return bool
    /************************************************************************/
	virtual bool IsActionNameExist(int wIndex, const char* name) = 0 ;

    /************************************************************************/
    // @brief �����Ƿ�����
    // @param wIndex					-- [in ]��������
    // @param name					    -- [in ]��������
    // @return bool
    /************************************************************************/
    virtual bool IsActionNameRepeat(int wIndex, const char* name) = 0 ;

    /************************************************************************/
    // @brief �ƶ�һ������
    // @param oldIndex					-- [in ]���̵�ǰ����
    // @param newIndex					-- [in ]�����µ�����
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual int MoveWork( int oldIndex, int newIndex ) = 0;

    /************************************************************************/
    // @brief �ƶ�һ������
    // @param wIndex					-- [in ]��������
    // @param oldIndex					-- [in ]���赱ǰ����
    // @param newIndex					-- [in ]�����µ�����
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual int MoveStep( int wIndex, int oldIndex, int newIndex ) = 0;

    /************************************************************************/
    // @brief �ƶ�һ������
    // @param wIndex					-- [in ]��������
    // @param sIndex					-- [in ]��������
    // @param oldIndex					-- [in ]������ǰ����
    // @param newIndex					-- [in ]�����µ�����
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual int MoveAction(int wIndex, int sIndex, int oldIndex, int newIndex) = 0;

    /************************************************************************/
    // @brief ��ȡ���̸���
    // @return int           -- ���̸���
    /************************************************************************/
	virtual int GetWorkCount() = 0;

    /************************************************************************/
    // @brief ��ȡ����ĸ���
    // @param wIndex					-- [in ]��������
    // @return int                      -- �������
    /************************************************************************/
	virtual int GetStepCount(int wIndex) =0 ;

    /************************************************************************/
    // @brief ��ȡ�����ĸ���
    // @param wIndex					-- [in ]��������
    // @param sIndex					-- [in ]��������
    // @return int                      -- ��������
    /************************************************************************/
	virtual int GetActionCount(int wIndex, int sIndex) =0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param wIndex					-- [in ]��������
    // @return WORK_FLOW*               -- ��������
    /************************************************************************/
	virtual WORK_FLOW* GetWorkCfg(int wIndex) = 0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param WorkName					-- [in ]��������
    // @return WORK_FLOW*               -- ��������
    /************************************************************************/
    virtual WORK_FLOW* GetWorkCfg(const char* WorkName) = 0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param wIndex					-- [in ]��������
    // @param sIndex					-- [in ]��������
    // @return WORK_STEP*               -- ��������
    /************************************************************************/
	virtual WORK_STEP* GetStepCfg(int wIndex, int sIndex) = 0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param WorkName					-- [in ]��������
    // @param sIndex					-- [in ]��������
    // @return WORK_STEP*               -- ��������
    /************************************************************************/
	virtual WORK_STEP* GetStepCfg(const char* WorkName, int sIndex) = 0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param WorkName					-- [in ]��������
    // @param StepName					-- [in ]��������
    // @return WORK_STEP*               -- ��������
    /************************************************************************/
	virtual WORK_STEP* GetStepCfg(const char* WorkName, const char* StepName) = 0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param WorkName					-- [in ]��������
    // @param sIndex					-- [in ]��������
    // @param pIndex				    -- [in ]��������
    // @return WORK_ACTION*               -- ��������
    /************************************************************************/
	virtual WORK_ACTION* GetActionCfg(int wIndex, int sIndex, int pIndex) = 0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param WorkName					-- [in ]��������
    // @param StepName					-- [in ]��������
    // @param ActionName				-- [in ]��������
    // @return WORK_ACTION*               -- ��������
    /************************************************************************/
	virtual WORK_ACTION* GetActionCfg(const char* WorkName, const char* StepName,const char* ActionName) = 0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param WorkName					-- [in ]��������
    // @param StepName					-- [in ]��������
    // @param ActionName				-- [in ]��������
    // @return WORK_ACTION*               -- ��������
    /************************************************************************/
	virtual WORK_ACTION* GetActionCfg(const char* WorkName, int sIndex, int pIndex) = 0;

    /************************************************************************/
    // @brief �ж϶��������Ƿ�OK
    // @param wIndex					-- [in ]��������
    // @param pActionParam				-- [in ]��������
    // @return bool
    /************************************************************************/
	virtual bool IsActionParamReady(int wIndex, const WORK_ACTION* pActionParam) = 0;

    /************************************************************************/
    // @brief �ж�step�еĶ��������Ƿ�OK
    // @param WorkName					-- [in ]��������
    // @param sIndex				    -- [in ]��������
    // @return bool
    /************************************************************************/
	virtual bool IsStepParamReady(const char* WorkName, int sIndex) = 0;

    /************************************************************************/
    // @brief ɾ��һ����������
    // @param wIndex				    -- [in ]��������
    // @return bool
    /************************************************************************/
	virtual int DeleteWork(int wIndex) = 0;

    /************************************************************************/
    // @brief ɾ��һ������
    // @param wIndex				    -- [in ]��������
    // @param sIndex				    -- [in ]��������
    // @return bool
    /************************************************************************/
	virtual int DeleteStep(int wIndex, int sIndex) = 0;

    /************************************************************************/
    // @brief ɾ��һ������
    // @param wIndex				    -- [in ]��������
    // @param sIndex				    -- [in ]��������
    // @param pIndex				    -- [in ]��������
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int DeleteAction(int wIndex, int sIndex, int pIndex) = 0;

    /************************************************************************/
    // @brief ���ļ���ȡ��������
    // @param fileName				    -- [in ]�����ļ���
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int ReadCfg(const char *fileName = NULL) = 0;

    /************************************************************************/
    // @brief ���������ñ��浽�ļ�
    // @param fileName				    -- [in ]�����ļ���
    // @return EM_ERR_CONTROL
    /************************************************************************/
	virtual int SaveCfg(const char *fileName = NULL) = 0;

    /************************************************************************/
    // @brief �½���������(��д�ļ�)
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual int NewCfg() = 0;

    /************************************************************************/
    // @brief ��ȡĬ������(��������ʹ�õ����������ļ�·��)
    // @return string             -- �����ļ�·��
    /************************************************************************/
    virtual string getCurrentCfgPath() = 0 ;

    /************************************************************************/
    // @brief д��Ĭ������(��������ʹ�õ����������ļ�·��)
    // @param path				       -- [in ]�����ļ�·��
    // @return bool
    /************************************************************************/
    virtual bool writeCurrentCfgPath(string &path) = 0;
};

#endif // IWORKFLOWCFG

