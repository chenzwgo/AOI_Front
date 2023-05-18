#ifndef IACTION_CFG_H
#define IACTION_CFG_H

class TiXmlElement;
class IActionCfg
{
public:
    virtual ~IActionCfg(){}

    /************************************************************************/
    // @brief �������������ṹ��
    // @return void*                 -- ���ض��������Ľṹ��ʵ��
    /************************************************************************/ 
    virtual void* CreateActionParamData() = 0;

    /************************************************************************/
    // @brief ���������Ƿ���ͬ(�����ж������Ƿ��޸�)
    // @return bool
    /************************************************************************/ 
    virtual bool IsActionParamSame(void* srcData,void* desData) = 0;

    /************************************************************************/
    // @brief ������������(�����ж������Ƿ��޸�)
    // @param desData				-- [in ]Ŀ�����
    // @param srcData				-- [in ]ԭ����
    // @return void
    /************************************************************************/ 
	virtual void ActionParamCopy(void*desData ,void* srcData) = 0;

    /************************************************************************/
    // @brief ���������Ƿ�OK
    // @param srcData				-- [in ]��������ָ��
    // @return bool
    /************************************************************************/ 
    virtual bool IsActionParamReady(void* srcData) = 0;  

    /************************************************************************/
    // @brief �������н�����������
    // @param actioElem				    -- [in ]xml�ļ��ڵ�
    // @param pParamData				-- [out]��������ָ��
    // @return void
    /************************************************************************/ 
    virtual void parseActionParam( TiXmlElement *actioElem,void* pParamData ) = 0;

    /************************************************************************/
    // @brief ��������д������
    // @param actioElem				    -- [in ]xml�ļ��ڵ�
    // @param pParamData				-- [in ]��������ָ��
    // @return bool
    /************************************************************************/ 
    virtual void writeActionParam( TiXmlElement *actioElem,void* pParamData ) = 0;
};

#endif // IACTION_CFG_H

