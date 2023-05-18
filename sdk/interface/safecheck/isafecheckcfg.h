#ifndef I_SAFECHECKCFG_H
#define I_SAFECHECKCFG_H
#include <vector>

using namespace std;
struct STSCFlowData;
struct STSCActData;
class ISafeCheckCfg
{
public:
    enum
    {
        I_SAFECHECKCFG_IID = 0x10
    };
    //获取流程数据
    virtual vector<STSCFlowData*> GetSafeCheckFlow() = 0;
    //保存流程数据
    virtual int SaveSafeCheckFlow(const vector<STSCFlowData*>& vecSTSCFlowData, const char* szFilePath = NULL) = 0;
    //加载流程
    virtual int LoadSafeCheckFlow(const char* szFilePath = NULL) = 0;
    //判断流程是否发生修改
    virtual bool IsSafeCheckFlowAlter(const vector<STSCFlowData*>& vecSTSCFlowData) = 0;
    //获取当前文件路径
    virtual string GetCurFilePath() = 0;
    //检查数据是否合法
    virtual bool CheckSafeLegel(STSCFlowData* FlowData) = 0;
};

#endif
