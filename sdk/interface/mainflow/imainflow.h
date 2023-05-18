#ifndef IWORKFLOW_H
#define IWORKFLOW_H

#include <string>
#include "mainflow/workflowdefine.h"
#include "mainflowaction/workflowactiondefine.h"
#include "mainflowaction/actionparam.h"
using namespace std;
class CAction;
class IMainFlowEvent;
class IInOutVariable;
class IMainFlowFun;
class iMainFlowCfg;
class IMainFlow
{
public:
    enum
    {
        IDD_MAIN_FLOW = 0x001
    };

    virtual ~IMainFlow(){}


    /************************************************************************/
    // @brief ��������,��beginStepִ�е�endStep
    // @param flowName					-- [in ]��������
    // @param beginStep					-- [in ]��ʼ�������ƣ�Ϊ�գ���ʾ�ӵ�һ�����迪ʼִ��
    // @param endStep					-- [in ]�����������ƣ�Ϊ�գ���ʾִ�е����һ������
    // @param bRunLoop				    -- [in ]�Ƿ�ѭ��ִ��
    // @param bStepByStep				-- [in ]�Ƿ񵥲�ִ��
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
    virtual int runFlow(const string &flowName, const string &beginStep = "", const string &endStep = "", bool bRunLoop = false, bool bStepByStep = false) = 0;

    /************************************************************************/
    // @brief ��������,��beginStepִ�е�endStep
    // @param flowName					-- [in ]��������
    // @param beginStep					-- [in ]��ʼ����������Ϊ-1����ʾ�ӵ�һ�����迪ʼִ��
    // @param endStep					-- [in ]��������������Ϊ-1����ʾִ�е����һ������
    // @param bRunLoop				    -- [in ]�Ƿ�ѭ��ִ��
    // @param bStepByStep				-- [in ]�Ƿ񵥲�ִ��
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int runFlow(const string &flowName, int stepBeginIndex, int stepEndIndex, bool bRunLoop = false, bool bStepByStep = false) = 0;

    /************************************************************************/
    // @brief ��ͣ����
    // @param flowName					-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
    virtual int pauseFlow(const string &flowName) = 0;

    /************************************************************************/
    // @brief ��������
    // @param flowName					-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int continueFlow(const string &flowName, bool bStepByStep = false ) = 0;

    /************************************************************************/
    // @brief ֹͣ����
    // @param flowName					-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
    virtual int stopFlow(const string &flowName) = 0;

    /************************************************************************/
    // @brief ��λ����(Ԥ��,��ʱ����)
    // @param flowName					-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual void resetFlow(const string &flowName) = 0;

    /************************************************************************/
    // @brief ��ȡ��ǰ����(ĿǰUI��ʾ�õ�)
    // @param flowName					-- [in ]��������
    // @param stepname					-- [out]��������
    // @param actionName				-- [out]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int getCurrentAction(const string &flowName, string &stepname,string &actionName) = 0;

    /************************************************************************/
    // @brief ��ȡ��ǰ����(ĿǰUI��ʾ�õ�)
    // @param flowName					-- [in ]��������
    // @param stepname					-- [out]��������
    // @param actionName				-- [out]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int getCurrentAction(const string &flowName, int &stepIndex, int &actionIndex) = 0;

    /************************************************************************/
    // @brief ��ȡ���̵�ǰִ��״̬
    // @param flowName					-- [in ]��������
    // @return E_FLOW_STATUS                                     
    /************************************************************************/ 
    virtual E_FLOW_STATUS runStatus(const string &flowName) = 0;

    /************************************************************************/
    // @brief �������״̬(��������,������״̬)
    // @param flowName					-- [in ]��������
    // @param stepBeginIndex			-- [in ]��ʼ����
    // @param stepEndIndex				-- [in ]��������
    // @return void                                     
    /************************************************************************/ 
    virtual void clearStatus(const string &flowName, int stepBeginIndex = 0, int stepEndIndex = -1) = 0;

	/***** �ڲ� ����step ������ ���ڲ� ����action �Ĳ����������ϲ� �Զ������******/
    /************************************************************************/
    // @brief ִ�е�������
    // @param flowName					-- [in ]��������
    // @param stepName			        -- [in ]��������
    // @param actionName				-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int runAction(const string &flowName, const string &stepName ,const string &actionName)=0;

    /************************************************************************/
    // @brief ִ�е�������
    // @param flowName					-- [in ]��������
    // @param stepIndex			        -- [in ]��������
    // @param actionIndex				-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
	virtual int runAction(const string &flowName, int stepIndex ,int actionIndex) = 0;

    /************************************************************************/
    // @brief ֹͣ��������
    // @param flowName					-- [in ]��������
    // @param stepIndex			        -- [in ]��������
    // @param actionIndex				-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/ 
    virtual int stopAction(const string &flowName, int stepIndex ,int actionIndex) = 0;

    /************************************************************************/
    // @brief ��ͣ��������
    // @param flowName					-- [in ]��������
    // @param stepIndex			        -- [in ]��������
    // @param actionIndex				-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/
    virtual int pauseAction(const string &flowName, int stepIndex ,int actionIndex) = 0;

    /************************************************************************/
    // @brief ������������
    // @param flowName					-- [in ]��������
    // @param stepIndex			        -- [in ]��������
    // @param actionIndex				-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/
    virtual int continueAction(const string &flowName, int stepIndex ,int actionIndex) = 0;

    /************************************************************************/
    // @brief ��ȡ��������״̬
    // @param flowName					-- [in ]��������
    // @param stepName			        -- [in ]��������
    // @param actionName				-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/
	virtual E_ACTION_STATUS GetActionStatus(const string &flowName, const string &stepName ,const string &actionName) = 0; 

    /************************************************************************/
    // @brief ��ȡ��������״̬
    // @param flowName					-- [in ]��������
    // @param stepIndex			        -- [in ]��������
    // @param actionIndex				-- [in ]��������
    // @return EM_ERR_CONTROL                                     
    /************************************************************************/
	virtual E_ACTION_STATUS GetActionStatus(const string &flowName, int stepIndex ,int actionIndex) = 0; // ��ȡ����״̬

    /************************************************************************/
    // @brief ��ȡ��������ִ�к�ʱ
    // @param flowName					-- [in ]��������
    // @param stepIndex			        -- [in ]��������
    // @param actionIndex				-- [in ]��������
    // @return long                     -- ��ʱʱ��               
    /************************************************************************/
    virtual long getActionTimeSpent(const string &flowName, int stepIndex, int actionIndex) = 0;// ��ȡ����IAciton ʵ��

    /************************************************************************/
    // @brief ��ȡ����ִ�к�ʱ
    // @param flowName					-- [in ]��������
    // @return long                     -- ��ʱʱ��              
    /************************************************************************/
    virtual long getFlowTimeSpent(const string &flowName) = 0;

    /************************************************************************/
    // @brief ��ȡ�������ýӿ�
    // @return iMainFlowCfg
    /************************************************************************/
    virtual iMainFlowCfg* GetMainFlowCfg()= 0;
};

#endif // IWORKFLOW

