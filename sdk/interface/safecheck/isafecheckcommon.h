#ifndef I_SAFECHECKCOMMON_H
#define I_SAFECHECKCOMMON_H

using namespace std;
struct STSCFlowData;
struct STSCActData;
class ISafeCheckCommon
{
public:
    enum
    {
        I_SAFECHECKCOMMON_IID = 0x12
    };
    //检查数据是否合法
    virtual bool CheckSafeFlowLegel(STSCFlowData* FlowData) = 0;
    virtual bool CheckSafeActionLegel(STSCActData* ActionData) = 0;
    //复制、删除
    virtual STSCFlowData* CopySafeCheckFlowInfo(const STSCFlowData* pSTSCFlowData) = 0;
    virtual void          DeleteSafeCheckFlowInfo(STSCFlowData *&FlowData) = 0;
    virtual STSCActData*  CopySafeCheckActionInfo(const STSCActData* pSrcAction ) = 0;
    virtual void          DeleteSafeCheckActionInfo(STSCActData *&pActionData) = 0;
    //获取类型索引
    virtual int GetFlowTypeIndexByName(const string& strFlowType) = 0;
    virtual int GetActionTypeIndexByName(const string& strActionType) = 0;    
    //拼流程ID
    virtual string getIOCheckID(const char* pIOName,bool On) = 0;
    virtual string getAxisMoveCheckID(const char* pAxisName) = 0;
    virtual string getAxisHomeCheckID(const char* pAxisName) = 0;
    virtual string getStationMoveCheckID(const char* pStationName,const char* pointName) = 0;
    virtual string getStationHomeCheckID(const char* pStationName) = 0;
    virtual string getFlowAcitonCheckID(const char* pFlowType, const char* pWorkName,const char* pStepName,const char* pActionName) = 0;
public:
    virtual bool ExcuteVecCheck(vector<STSCActData*>& vecActions,bool& bRes) = 0;
    virtual bool ExcuteItmeCheck(STSCActData* pSTSCActData ,bool& bRes) = 0;
};

#endif
