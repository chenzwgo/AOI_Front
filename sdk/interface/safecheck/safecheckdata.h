#ifndef SAFECHECKDATA_H
#define SAFECHECKDATA_H
#include <string>
#include <vector>
using namespace std;

#define SC_SEPARATOR_ID "_"
enum enSafeCheckActionType
{
    EM_SAFECHECKACTIONTYPE_GROUP = 0,
    EM_SAFECHECKACTIONTYPE_IO,
    EM_SAFECHECKACTIONTYPE_AXISSTATUS,
    EM_SAFECHECKACTIONTYPE_AXISPOS,
    EM_SAFECHECKACTIONTYPE_STATIONPOS,
    EM_SAFECHECKACTIONTYPE_END
};
static const char* g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_END] = {"group"   , "io", "axisStatus", "axisPos", "stationPos"};
static const char* g_safeCheckActionDesc[EM_SAFECHECKACTIONTYPE_END] = {"条件组合", "io", "轴状态"    , "轴位置" , "工站位置"};

enum SafeCheckLogic
{
    SAFECHECKLOGIC_AND = 0,	// 与关系
    SAFECHECKLOGIC_OR	    // 或关系
};

enum SafeCheckRunStatus
{
    SAFECHECKRUNSTATUS_INIT = 0, //未执行
    SAFECHECKRUNSTATUS_RUNNING,  //执行中
    SAFECHECKRUNSTATUS_RUN_TRUE, //执行判断成功
    SAFECHECKRUNSTATUS_RUN_FALSE,//执行判断失败
    SAFECHECKRUNSTATUS_RUN_FAIL, //执行失败
};

struct STSCActData
{
    bool               bEnabled   ; //是否是能
    SafeCheckLogic     emLogicType; //运算符号
    bool               bValue     ; //结果判断
    string             strID      ; //actionID  
    string             strDesc    ; //描述
    SafeCheckRunStatus emStatus   ; //运行状态
    string             strErrInfo; //语法错误信息
    //获取类型
    virtual const char* dataType() const {return "";}
    virtual const char* dataDesc() const {return "";}
    virtual ~STSCActData(){}

    STSCActData()
        :bEnabled(true)
        ,emLogicType(SAFECHECKLOGIC_AND)
        ,bValue(true)
        ,strDesc("") 
        ,strID("")
        ,emStatus(SAFECHECKRUNSTATUS_INIT)
        ,strErrInfo("")
    {
    }

    STSCActData(const STSCActData &other)
    { 
        bEnabled    = other.bEnabled   ;
        emLogicType = other.emLogicType;
        bValue      = other.bValue     ;
        strID       = other.strID      ;  
        strDesc     = other.strDesc    ;
        emStatus    = other.emStatus   ;
        strErrInfo  = other.strErrInfo ;
    }

    STSCActData &operator=(const STSCActData &other)
    {
        if (this != &other)
        {   
            bEnabled    = other.bEnabled   ;
            emLogicType = other.emLogicType;
            bValue      = other.bValue     ;
            strID       = other.strID      ;  
            strDesc     = other.strDesc    ;
            emStatus    = other.emStatus   ;
            strErrInfo  = other.strErrInfo ;
        }
        return *this;
    }

    bool operator!=(const STSCActData &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return (bEnabled    != other.bEnabled   
             || emLogicType != other.emLogicType
             || bValue      != other.bValue     
             || strID       != other.strID      
             || strDesc     != other.strDesc );
    }

    bool operator==(const STSCActData &other) const
    {
        return !(*this != other);
    }
};
//actionGroup条件组合
struct STSCActGroup : public STSCActData
{
    vector<STSCActData*> childrenItems;
    //获取类型
    virtual const char* dataType() const {return g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_GROUP];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckActionDesc[EM_SAFECHECKACTIONTYPE_GROUP];}

    STSCActGroup()
    {
        strDesc = g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_GROUP];
    }

    STSCActGroup(const STSCActGroup &other)
        :STSCActData(other)
    { 
        childrenItems  = other.childrenItems ;
    }

    STSCActGroup &operator=(const STSCActGroup &other)
    {
        if (this != &other)
        {   
            STSCActData::operator=(other);
            childrenItems  = other.childrenItems ;
        }
        return *this;
    }

    bool operator!=(const STSCActGroup &other) const 
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCActData::operator!=(other));
    }
    bool operator==(const STSCActGroup &other) const
    {
        return !(*this != other);
    }
};

//actionIO参数
struct STSCActIo : public STSCActData
{
    string         strIOname  ; //io名字
    //获取类型
    virtual const char* dataType() const {return g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_IO];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckActionDesc[EM_SAFECHECKACTIONTYPE_IO];};

    STSCActIo()
        :strIOname("") 
    {
        strDesc = g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_IO];
    }

    STSCActIo(const STSCActIo &other)
        :STSCActData(other)
    { 
        strIOname = other.strIOname;
    }

    STSCActIo &operator=(const STSCActIo &other)
    {
        if (this != &other)
        {   
            STSCActData::operator=(other);
            strIOname = other.strIOname;
        }
        return *this;
    }

    bool operator!=(const STSCActIo &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCActData::operator!=(other)
                 || strIOname != other.strIOname);
    }
    bool operator==(const STSCActIo &other) const
    {
        return !(*this != other);
    }
};

//action轴状态参数
struct STSCActAxisStatus : public STSCActData
{
    string         strAxisName   ; //名字
    enum AxisStatusType
    {
        AxisStatusType_alarm = 0, //报警
        AxisStatusType_origin,    //远点
        AxisStatusType_limitP,    //正限位
        AxisStatusType_limitN,    //负限位
        AxisStatusType_enabled    //使能
    } emAxisStatusType;
    //获取类型
    virtual const char* dataType() const {return g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_AXISSTATUS];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckActionDesc[EM_SAFECHECKACTIONTYPE_AXISSTATUS];};

    STSCActAxisStatus()
        :strAxisName("") 
        ,emAxisStatusType(AxisStatusType_alarm)
    {
        strDesc = g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_AXISSTATUS];
    }

    STSCActAxisStatus(const STSCActAxisStatus &other)
        :STSCActData(other)
    { 
        strAxisName      = other.strAxisName;
        emAxisStatusType = other.emAxisStatusType;
    }

    STSCActAxisStatus &operator=(const STSCActAxisStatus &other)
    {
        if (this != &other)
        {   
            STSCActData::operator=(other);
            strAxisName      = other.strAxisName;
            emAxisStatusType = other.emAxisStatusType;
        }
        return *this;
    }

    bool operator!=(const STSCActAxisStatus &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCActData::operator!=(other)
            || strAxisName != other.strAxisName
            || emAxisStatusType != other.emAxisStatusType);
    }
    bool operator==(const STSCActAxisStatus &other) const
    {
        return !(*this != other);
    }
};

//action轴位置参数
struct STSCActAxisPos : public STSCActData
{
    string         strAxisName; //名字
    double         dMinPos    ; //mm
    double         dMaxPos    ; //mm
    //获取类型
    virtual const char* dataType() const {return g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_AXISPOS];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckActionDesc[EM_SAFECHECKACTIONTYPE_AXISPOS];};

    STSCActAxisPos()
        :strAxisName("") 
        ,dMinPos(-1000)
        ,dMaxPos(1000)
    {
        strDesc = g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_AXISPOS];
    }

    STSCActAxisPos(const STSCActAxisPos &other)
        :STSCActData(other)
    { 
        strAxisName = other.strAxisName;
        dMinPos     = other.dMinPos;
        dMaxPos     = other.dMaxPos;
    }

    STSCActAxisPos &operator=(const STSCActAxisPos &other)
    {
        if (this != &other)
        {   
            STSCActData::operator=(other);
            strAxisName = other.strAxisName;
            dMinPos     = other.dMinPos;
            dMaxPos     = other.dMaxPos;
        }
        return *this;
    }

    bool operator!=(const STSCActAxisPos &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCActData::operator!=(other)
            || strAxisName != other.strAxisName
            || dMinPos     != other.dMinPos
            || dMaxPos     != other.dMaxPos);
    }
    bool operator==(const STSCActAxisPos &other) const
    {
        return !(*this != other);
    }
};

//action工站位置参数
struct STSCActStationPos : public STSCActData
{
    string         strStationName; //工站名字
    int            iAxisIndex    ; //工站索引
    string         strPosName    ; //点位名字
    double         dMinPos       ; //mm
    double         dMaxPos       ; //mm
    //获取类型
    virtual const char* dataType() const {return g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_STATIONPOS];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckActionDesc[EM_SAFECHECKACTIONTYPE_STATIONPOS];}

    STSCActStationPos()
        :strStationName("") 
        ,iAxisIndex(0)
        ,strPosName("")
        ,dMinPos(-1000)
        ,dMaxPos(1000)
    {
        strDesc = g_safeCheckActionType[EM_SAFECHECKACTIONTYPE_STATIONPOS];
    }

    STSCActStationPos(const STSCActStationPos &other)
        :STSCActData(other)
    { 
        strStationName = other.strStationName;
        iAxisIndex     = other.iAxisIndex    ;
        strPosName     = other.strPosName    ;
        dMinPos        = other.dMinPos       ;
        dMaxPos        = other.dMaxPos       ;
    }

    STSCActStationPos &operator=(const STSCActStationPos &other)
    {
        if (this != &other)
        {   
            STSCActData::operator=(other);
            strStationName = other.strStationName;
            iAxisIndex     = other.iAxisIndex    ;
            strPosName     = other.strPosName    ;
            dMinPos        = other.dMinPos       ;
            dMaxPos        = other.dMaxPos       ;
        }
        return *this;
    }

    bool operator!=(const STSCActStationPos &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCActData::operator!=(other)
           || strStationName != other.strStationName
           || iAxisIndex     != other.iAxisIndex    
           || strPosName     != other.strPosName    
           || dMinPos        != other.dMinPos       
           || dMaxPos        != other.dMaxPos       );
    }
    bool operator==(const STSCActStationPos &other) const
    {
        return !(*this != other);
    }
};

enum enSafeCheckFlowType
{
    EM_SAFECHECKFLOWTYPE_IO = 0,
    EM_SAFECHECKFLOWTYPE_AXISMOVE,
    EM_SAFECHECKFLOWTYPE_AXISHOME,
    EM_SAFECHECKFLOWTYPE_STATIONMOVEPOS,
    EM_SAFECHECKFLOWTYPE_STATIONHOME,
    EM_SAFECHECKFLOWTYPE_FLOWACTION,
    EM_SAFECHECKFLOWTYPE_CUSTOM,
    EM_SAFECHECKFLOWTYPE_END
};
static const char* g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_END] = {"io"    ,"axisMove","axisHome"    ,"stationMovePos" ,"stationHome"    ,"flowAction", "custom"};
static const char* g_safeCheckFlowDesc[EM_SAFECHECKFLOWTYPE_END] = {"io检测","单轴移动","单轴回原检测","station跑点检测","station回原检测","action"    , "自定义"};
//检测项目参数
struct STSCFlowData
{
    bool               bEnabled;     //是否是能
    string             strID   ;     //FlowID  
    string             strDesc ;     //描述
    SafeCheckRunStatus emStatus;     //运行状态
    string             strErrInfo;   //语法错误信息
    vector<STSCActData*> vecActions; //action列表
    //获取类型
    virtual const char* dataType() const {return "";}
    virtual const char* dataDesc() const {return "";}
    virtual ~STSCFlowData(){}

    STSCFlowData()
        :bEnabled(true)
        ,strDesc("") 
        ,strID("")
        ,emStatus(SAFECHECKRUNSTATUS_INIT)
        ,strErrInfo("")
    {
    }


    STSCFlowData(const STSCFlowData &other)
    { 
        bEnabled    = other.bEnabled   ;
        strID       = other.strID      ;  
        strDesc     = other.strDesc    ;
        emStatus    = other.emStatus   ;
        vecActions  = other.vecActions ;
        strErrInfo  = other.strErrInfo ;
    }

    STSCFlowData &operator=(const STSCFlowData &other)
    {
        if (this != &other)
        {   
            bEnabled    = other.bEnabled   ;
            strID       = other.strID      ;  
            strDesc     = other.strDesc    ;
            emStatus    = other.emStatus   ;
            vecActions  = other.vecActions ;
            strErrInfo  = other.strErrInfo ;
        }
        return *this;
    }

    bool operator!=(const STSCFlowData &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return (bEnabled    != other.bEnabled   
            || strID       != other.strID      
            || strDesc     != other.strDesc );
    }
    bool operator==(const STSCFlowData &other) const
    {
        return !(*this != other);
    }
};

//flowIO参数
struct STSCFlowIo : public STSCFlowData
{
    string         strIOname  ; //io名字
    int            iIoValue   ; //io状态

    //获取类型
    virtual const char* dataType() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_IO];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_IO];}

    STSCFlowIo()
        :strIOname("")
        ,iIoValue(0)
    {
        strDesc = g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_IO];
    }

    STSCFlowIo(const STSCFlowIo &other)
        :STSCFlowData(other)
    { 
        strIOname = other.strIOname;
        iIoValue  = other.iIoValue ;
    }

    STSCFlowIo &operator=(const STSCFlowIo &other)
    {
        if (this != &other)
        {   
            STSCFlowData::operator=(other);
            strIOname = other.strIOname;
            iIoValue  = other.iIoValue ;
        }
        return *this;
    }

    bool operator!=(const STSCFlowIo &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCFlowData::operator!=(other)
            || strIOname != other.strIOname
            || iIoValue  != other.iIoValue);
    }
    bool operator==(const STSCFlowIo &other) const
    {
        return !(*this != other);
    }
};//flowIO参数

struct STSCFlowAxisMove : public STSCFlowData
{
    string         strAxisName; //轴名字

    //获取类型
    virtual const char* dataType() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_AXISMOVE];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_AXISMOVE];};

    STSCFlowAxisMove()
        :strAxisName("")
    {
        strDesc = g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_AXISMOVE];
    }

    STSCFlowAxisMove(const STSCFlowAxisMove &other)
        :STSCFlowData(other)
    { 
        strAxisName = other.strAxisName;
    }

    STSCFlowAxisMove &operator=(const STSCFlowAxisMove &other)
    {
        if (this != &other)
        {   
            STSCFlowData::operator=(other);
            strAxisName = other.strAxisName;
        }
        return *this;
    }

    bool operator!=(const STSCFlowAxisMove &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCFlowData::operator!=(other)
            || strAxisName != other.strAxisName);
    }
    bool operator==(const STSCFlowAxisMove &other) const
    {
        return !(*this != other);
    }
};

struct STSCFlowAxisHome : public STSCFlowData
{
    string  strAxisName; //轴名字

    //获取类型
    virtual const char* dataType() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_AXISHOME];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_AXISHOME];};

    STSCFlowAxisHome()
        :strAxisName("")
    {
        strDesc = g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_AXISHOME];
    }

    STSCFlowAxisHome(const STSCFlowAxisHome &other)
        :STSCFlowData(other)
    { 
        strAxisName = other.strAxisName;
    }

    STSCFlowAxisHome &operator=(const STSCFlowAxisHome &other)
    {
        if (this != &other)
        {   
            STSCFlowData::operator=(other);
            strAxisName = other.strAxisName;
        }
        return *this;
    }

    bool operator!=(const STSCFlowAxisHome &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCFlowData::operator!=(other)
            || strAxisName != other.strAxisName);
    }
    bool operator==(const STSCFlowAxisHome &other) const
    {
        return !(*this != other);
    }
};

struct STSCFlowStationMovePos : public STSCFlowData
{
    string         strStationName; //工站名字
    string         strPosName;     //点位名字
    //获取类型
    virtual const char* dataType() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_STATIONMOVEPOS];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_STATIONMOVEPOS];};

    STSCFlowStationMovePos()
        :strStationName("")
        ,strPosName("")
    {
        strDesc = g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_STATIONMOVEPOS];
    }

    STSCFlowStationMovePos(const STSCFlowStationMovePos &other)
        :STSCFlowData(other)
    { 
        strStationName = other.strStationName;
        strPosName     = other.strPosName    ;
    }

    STSCFlowStationMovePos &operator=(const STSCFlowStationMovePos &other)
    {
        if (this != &other)
        {   
            STSCFlowData::operator=(other);
            strStationName = other.strStationName;
            strPosName     = other.strPosName    ;
        }
        return *this;
    }

    bool operator!=(const STSCFlowStationMovePos &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCFlowData::operator!=(other)
            || strStationName != other.strStationName
            || strPosName     != other.strPosName);
    }
    bool operator==(const STSCFlowStationMovePos &other) const
    {
        return !(*this != other);
    }
};

struct STSCFlowStationHome : public STSCFlowData
{
    string         strStationName; //工站名字
    //获取类型
    virtual const char* dataType() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_STATIONHOME];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_STATIONHOME];};

    STSCFlowStationHome()
        :strStationName("")
    {
        strDesc = g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_STATIONHOME];
    }

    STSCFlowStationHome(const STSCFlowStationHome &other)
        :STSCFlowData(other)
    { 
        strStationName = other.strStationName;
    }

    STSCFlowStationHome &operator=(const STSCFlowStationHome &other)
    {
        if (this != &other)
        {   
            STSCFlowData::operator=(other);
            strStationName = other.strStationName;
        }
        return *this;
    }

    bool operator!=(const STSCFlowStationHome &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCFlowData::operator!=(other)
            || strStationName != other.strStationName);
    }
    bool operator==(const STSCFlowStationHome &other) const
    {
        return !(*this != other);
    }
};

struct STSCFlowAction : public STSCFlowData
{
    string         strFlowType  ; //流程类型
    string         strWorkName  ; //流程名字
    string         strStepName  ; //步骤名字
    string         strActionName; //动作名字
    //获取类型
    virtual const char* dataType() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_FLOWACTION];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_FLOWACTION];};

    STSCFlowAction()
        :strFlowType("")
        ,strWorkName("")
        ,strStepName("")
        ,strActionName("")
    {
        strDesc = g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_FLOWACTION];
    }

    STSCFlowAction(const STSCFlowAction &other)
        :STSCFlowData(other)
    { 
        strFlowType   = other.strFlowType  ;
        strWorkName   = other.strWorkName  ;
        strStepName   = other.strStepName  ;
        strActionName = other.strActionName;
    }

    STSCFlowAction &operator=(const STSCFlowAction &other)
    {
        if (this != &other)
        {   
            STSCFlowData::operator=(other);
            strFlowType   = other.strFlowType  ;
            strWorkName   = other.strWorkName  ;
            strStepName   = other.strStepName  ;
            strActionName = other.strActionName;
        }
        return *this;
    }

    bool operator!=(const STSCFlowAction &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCFlowData::operator!=(other)
            || strFlowType   != other.strFlowType
            || strWorkName   != other.strWorkName  
            || strStepName   != other.strStepName  
            || strActionName != other.strActionName);
    }
    bool operator==(const STSCFlowAction &other) const
    {
        return !(*this != other);
    }
};

struct STSCFlowCustom : public STSCFlowData
{
    //获取类型
    virtual const char* dataType() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_CUSTOM];}
    //获取类型描述
    virtual const char* dataDesc() const {return g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_CUSTOM];};

    STSCFlowCustom()
    {
        strDesc = g_safeCheckFlowType[EM_SAFECHECKFLOWTYPE_CUSTOM];
    }

    STSCFlowCustom(const STSCFlowCustom &other)
        :STSCFlowData(other)
    { 
    }

    STSCFlowCustom &operator=(const STSCFlowCustom &other)
    {
        if (this != &other)
        {   
            STSCFlowData::operator=(other);
        }
        return *this;
    }

    bool operator!=(const STSCFlowCustom &other) const
    {
        if (this == &other)
        {
            return false;
        }
        return ( STSCFlowData::operator!=(other));
    }
    bool operator==(const STSCFlowCustom &other) const
    {
        return !(*this != other);
    }
};

#endif // ACTBASEPARAMDLG_H
