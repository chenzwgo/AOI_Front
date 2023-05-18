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
    //��ȡ��������
    virtual vector<STSCFlowData*> GetSafeCheckFlow() = 0;
    //������������
    virtual int SaveSafeCheckFlow(const vector<STSCFlowData*>& vecSTSCFlowData, const char* szFilePath = NULL) = 0;
    //��������
    virtual int LoadSafeCheckFlow(const char* szFilePath = NULL) = 0;
    //�ж������Ƿ����޸�
    virtual bool IsSafeCheckFlowAlter(const vector<STSCFlowData*>& vecSTSCFlowData) = 0;
    //��ȡ��ǰ�ļ�·��
    virtual string GetCurFilePath() = 0;
    //��������Ƿ�Ϸ�
    virtual bool CheckSafeLegel(STSCFlowData* FlowData) = 0;
};

#endif
