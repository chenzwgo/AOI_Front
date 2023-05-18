#ifndef IWORKFLOW_MGR_H
#define IWORKFLOW_MGR_H

#include <vector>
#include <string>
#include "mainflow/workflowdefine.h"
using namespace std;

// 流程状态上报函数
typedef void (*pFunReport)(string strFlowGroup,int iflowIndex, int istepIndex , 
	int iAcitonIndex ,int itype,int istatus,int iErrorCode);

// 流程log上报函数
typedef void (*pFunFlowLogReport)(string logMsg,int logLevel);

class IMainFlow;
class IMainflowMgr
{
public:
    enum
    {
        IDD_MAIN_FLOW_MGR = 0x066
    };
    virtual IMainFlow* createMainflow(const char* pName) = 0;

	virtual IMainFlow* findMainflow(const char* pName) = 0;

    virtual void releaseMainflow(const char* pName) = 0;

    // 获取流程列表
    virtual vector<string> getMainFlowList() = 0;

    // LOG上报
    virtual void addLogReproter(pFunFlowLogReport pfun) = 0;
    virtual void removeLogReproter(pFunFlowLogReport pfun ) = 0;

	//状态上报的函数
	virtual void addReproter(pFunReport pfun) = 0;

	virtual void removeReproter(pFunReport pfun ) = 0;


	virtual vector <StUIActionGroup> *getActionTypeCfg() = 0;

    virtual ~IMainflowMgr(){}
};

#endif // IWORKFLOW_MGR

