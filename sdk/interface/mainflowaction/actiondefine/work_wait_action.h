#ifndef WORK_WAIT_ACTION_FINISH_H
#define WORK_WAIT_ACTION_FINISH_H

#include "../actioncfgdefine.h"

#define ITEM_NAME_LEN  256             // 流程编辑各项名字长度


#define WAIT_ACTION_FINISH  0
#define WAIT_ACTION_SUCCEED 1
#define WAIT_ACTION_FAILED  2
#define WAIT_ACTION_TIMEOUT 3

struct WORK_WAIT_ACTION_FINISH : ACTION_PARAM
{
	string cFlowNameGroup;
    string cFlowName;
    string cStepName;
    string cActionName;
    int   actionResult;  // 成功/失败/超时/完成
    //int   timeout; // -1为无限等待


    WORK_WAIT_ACTION_FINISH()
    {
        cFlowNameGroup = "";
        cFlowName = "";
        cStepName = "";
        cActionName = "";
        actionResult = WAIT_ACTION_FINISH;
        //timeout = -1;
    }

    WORK_WAIT_ACTION_FINISH& operator=(const WORK_WAIT_ACTION_FINISH &other)
    {
        if (this == &other)
        {
            return *this;
        }

        cFlowNameGroup = other.cFlowNameGroup;
        cFlowName = other.cFlowName;
        cStepName = other.cStepName;
        cActionName = other.cActionName;

        actionResult = other.actionResult;
        //timeout = other.timeout;
        return *this;
    }

    bool operator==(const WORK_WAIT_ACTION_FINISH &other)
    {
        return (cActionName == other.cActionName
			&& cFlowNameGroup == other.cFlowNameGroup
            && cFlowName == other.cFlowName
            && cStepName == other.cStepName
            /*&& timeout == other.timeout*/
            && actionResult == other.actionResult);
    }

    void setString(const string &data){}
    string toString(){return "";}
};


#endif // WORKFLOWDEFINE
