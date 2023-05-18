#ifndef _IINOUTVARIABLE_H_
#define _IINOUTVARIABLE_H_
#include <string>
#include <vector>
#include "any.h"
using namespace std;

#define TYPE_INT   "int"
#define TYPE_LONG  "long"
#define TYPE_FLOAT "float"
#define TYPE_DOUBLE "double"
#define TYPE_BOOL   "bool"
#define TYPE_STRING "string"

class IInOutVariable
{
public:
    enum
    {
        IID_INOUTVARIABLE= 0x206
    };
	virtual ~IInOutVariable() { }


    /************************************************************************/
    // @brief ���ӱ���
    // @param varName					-- [in ]��������
    // @param type					    -- [in ]��������
    // @param desc					    -- [in ]��������
    // @param defaultVal                -- [in ]Ĭ��ֵ
    // @param bReadOnly                 -- [in ]�Ƿ�ֻ������
    // @return bool                     -- �����Ƿ�ɹ�
    /************************************************************************/ 
    virtual bool addVariable( const string &varName, const string &type, const string &desc = "", const string &defaultVal = "", bool bReadOnly=false ) = 0;

    /************************************************************************/
    // @brief �޸ı���
    // @param varName					-- [in ]��������
    // @param type					    -- [in ]��������
    // @param desc					    -- [in ]��������
    // @return bool                     -- �����Ƿ�ɹ�
    /************************************************************************/ 
    virtual bool modifyDesc( const string &varName, const string &desc ) = 0;

    /************************************************************************/
    // @brief ɾ������
    // @param varName					-- [in ]��������
    // @return bool                     -- �����Ƿ�ɹ�
    /************************************************************************/ 
    virtual bool delVariable( const string &varName ) = 0;

    /************************************************************************/
    // @brief ������ֵ
    // @param varName					-- [in ]��������
    // @param rValue					-- [out]����ֵ
    // @return bool                     -- �����Ƿ�ɹ�
    /************************************************************************/ 
	virtual bool readVarialbleValue(const string &varName, any & rValue) = 0;

    /************************************************************************/
    // @brief д����ֵ
    // @param varName					-- [in ]��������
    // @param wValue					-- [in ]����ֵ
    // @return bool                     -- �����Ƿ�ɹ�
    /************************************************************************/ 
	virtual bool writeVarialbleValue(const string &varName, const any &wValue) = 0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param varName					-- [in ]��������
    // @param strDes					-- [out ]����
    // @param defaultVal				-- [out ]Ĭ��ֵ
    // @param bReadOnly				    -- [out ]�Ƿ�ֻ������
    // @return bool                     -- �����Ƿ�ɹ�
    /************************************************************************/ 
	virtual bool readVarialbleDes(const string &varName, string &strDes, string &defaultVal, bool &bReadOnly) = 0;

    /************************************************************************/
    // @brief ��ȡ��������
    // @param varName					-- [in ]��������
    // @param type					    -- [out]��������
    // @return bool                     -- �����Ƿ�ɹ�
    /************************************************************************/ 
    virtual bool getVarialbleType(const string &varName, string & type) = 0;

    /************************************************************************/
    // @brief ��ȡ�����б�
    // @param vectVariables			    -- [out]�����б�
    // @return void
    /************************************************************************/ 
    virtual void  getVariableList(vector<string>  &vectVariables) = 0;

    /************************************************************************/
    // @brief �������б���ֵ
    // @return void
    /************************************************************************/ 
    virtual void ResetAllVariable() = 0;

    /************************************************************************/
    // @brief ����ʵ��������UI����ǰ,���UI����any�������ٻ������bug��
    // @return void
    /************************************************************************/ 
    virtual void DestroyInstanceEx() = 0;

};
#endif // _IINOUTVARIABLE_H_
