#ifndef IMAINFLOWFUN_H
#define IMAINFLOWFUN_H

#include <string>
#include <vector>
using namespace std;

// 返回值，为SYS_ERR_OK表示成功，否则失败, parent为注册的类对象，param为参数
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
    // @brief 函数注册
    // @param funName				-- [in ]函数名称
    // @param fun					-- [in ]函数指针
    // @param param					-- [in ]函数参数
    // @param desc					-- [in ]函数描述
    // @return void
    /************************************************************************/ 
	virtual void registerFun(const string &funName, pActionFun fun, void* param, string desc="") = 0;

    /************************************************************************/
    // @brief 获取函数列表
    // @return vector<pair<string, string> >*    -- 函数列表
    /************************************************************************/ 
	virtual vector<pair<string, string> >* getFunList() = 0;
};

#endif // IWORKFLOW

