#ifndef  _I_MAINFLOWACITONFAC_H_
#define  _I_MAINFLOWACITONFAC_H_

class IAction;
class IActionCfg;

typedef IAction* CreatorFunctionCB();
template<class T> IAction* CreateActionObject() { return new T(); };

class IMainFlowAcitonFac
{
public:
    enum
    {
        IDD_MAIN_FLOW_ACTION_FAC = 0x005
    };
    virtual ~IMainFlowAcitonFac(){}
    
	virtual void registerCreateActionClass(const char* typeName, CreatorFunctionCB* pActionCreator = NULL) =0;
	virtual void registerctionParamClass(const char* typeName,IActionCfg* pIActionCfg) =0;

	virtual IAction* createAcitonClass(const char* typeName) =0;

	virtual IActionCfg* GetAcitonParamClass(const char* typeName) =0;

};

#endif // _I_MAINFLOWACITONFAC_H_