#ifndef IMAINFLOWFUN_H
#define IMAINFLOWFUN_H

#include <string>
#include <vector>
using namespace std;

// ����ֵ��ΪSYS_ERR_OK��ʾ�ɹ�������ʧ��, parentΪע��������paramΪ����
typedef int (*pActionFun)(void* parent, string param);

class IMainFlowFun
{
public:
    enum
    {
        IDD_MAIN_FLOW_FUN = 0x003
    };

    virtual ~IMainFlowFun(){}

    /************************************************************************/
    // @brief ����ע��
    // @param funName				-- [in ]��������
    // @param fun					-- [in ]����ָ��
    // @param param					-- [in ]��������
    // @param desc					-- [in ]��������
    // @return void
    /************************************************************************/ 
	virtual void registerFun(const string &funName, pActionFun fun, void* param, string desc="") = 0;

    /************************************************************************/
    // @brief ��ȡ�����б�
    // @return vector<pair<string, string> >*    -- �����б�
    /************************************************************************/ 
	virtual vector<pair<string, string> >* getFunList() = 0;
};

#endif // IWORKFLOW

