#ifndef IACTION_CFG_H
#define IACTION_CFG_H

class TiXmlElement;
class IActionCfg
{
public:
    virtual ~IActionCfg(){}

    /************************************************************************/
    // @brief 创建动作参数结构体
    // @return void*                 -- 返回动作参数的结构体实例
    /************************************************************************/ 
    virtual void* CreateActionParamData() = 0;

    /************************************************************************/
    // @brief 动作参数是否相同(用于判断配置是否修改)
    // @return bool
    /************************************************************************/ 
    virtual bool IsActionParamSame(void* srcData,void* desData) = 0;

    /************************************************************************/
    // @brief 动作参数复制(用于判断配置是否修改)
    // @param desData				-- [in ]目标参数
    // @param srcData				-- [in ]原参数
    // @return void
    /************************************************************************/ 
	virtual void ActionParamCopy(void*desData ,void* srcData) = 0;

    /************************************************************************/
    // @brief 动作参数是否OK
    // @param srcData				-- [in ]动作参数指针
    // @return bool
    /************************************************************************/ 
    virtual bool IsActionParamReady(void* srcData) = 0;  

    /************************************************************************/
    // @brief 从配置中解析动作参数
    // @param actioElem				    -- [in ]xml文件节点
    // @param pParamData				-- [out]动作参数指针
    // @return void
    /************************************************************************/ 
    virtual void parseActionParam( TiXmlElement *actioElem,void* pParamData ) = 0;

    /************************************************************************/
    // @brief 动作参数写入配置
    // @param actioElem				    -- [in ]xml文件节点
    // @param pParamData				-- [in ]动作参数指针
    // @return bool
    /************************************************************************/ 
    virtual void writeActionParam( TiXmlElement *actioElem,void* pParamData ) = 0;
};

#endif // IACTION_CFG_H

