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
    // @brief 增加变量
    // @param varName					-- [in ]变量名称
    // @param type					    -- [in ]变量类型
    // @param desc					    -- [in ]变量描述
    // @param defaultVal                -- [in ]默认值
    // @param bReadOnly                 -- [in ]是否只读变量
    // @return bool                     -- 操作是否成功
    /************************************************************************/ 
    virtual bool addVariable( const string &varName, const string &type, const string &desc = "", const string &defaultVal = "", bool bReadOnly=false ) = 0;

    /************************************************************************/
    // @brief 修改变量
    // @param varName					-- [in ]变量名称
    // @param type					    -- [in ]变量类型
    // @param desc					    -- [in ]变量描述
    // @return bool                     -- 操作是否成功
    /************************************************************************/ 
    virtual bool modifyDesc( const string &varName, const string &desc ) = 0;

    /************************************************************************/
    // @brief 删除变量
    // @param varName					-- [in ]变量名称
    // @return bool                     -- 操作是否成功
    /************************************************************************/ 
    virtual bool delVariable( const string &varName ) = 0;

    /************************************************************************/
    // @brief 读变量值
    // @param varName					-- [in ]变量名称
    // @param rValue					-- [out]变量值
    // @return bool                     -- 操作是否成功
    /************************************************************************/ 
	virtual bool readVarialbleValue(const string &varName, any & rValue) = 0;

    /************************************************************************/
    // @brief 写变量值
    // @param varName					-- [in ]变量名称
    // @param wValue					-- [in ]变量值
    // @return bool                     -- 操作是否成功
    /************************************************************************/ 
	virtual bool writeVarialbleValue(const string &varName, const any &wValue) = 0;

    /************************************************************************/
    // @brief 获取变量描述
    // @param varName					-- [in ]变量名称
    // @param strDes					-- [out ]描述
    // @param defaultVal				-- [out ]默认值
    // @param bReadOnly				    -- [out ]是否只读变量
    // @return bool                     -- 操作是否成功
    /************************************************************************/ 
	virtual bool readVarialbleDes(const string &varName, string &strDes, string &defaultVal, bool &bReadOnly) = 0;

    /************************************************************************/
    // @brief 获取变量类型
    // @param varName					-- [in ]变量名称
    // @param type					    -- [out]变量类型
    // @return bool                     -- 操作是否成功
    /************************************************************************/ 
    virtual bool getVarialbleType(const string &varName, string & type) = 0;

    /************************************************************************/
    // @brief 获取变量列表
    // @param vectVariables			    -- [out]变量列表
    // @return void
    /************************************************************************/ 
    virtual void  getVariableList(vector<string>  &vectVariables) = 0;

    /************************************************************************/
    // @brief 清零所有变量值
    // @return void
    /************************************************************************/ 
    virtual void ResetAllVariable() = 0;

    /************************************************************************/
    // @brief 销毁实例（用于UI析构前,解决UI先于any变量销毁会崩溃的bug）
    // @return void
    /************************************************************************/ 
    virtual void DestroyInstanceEx() = 0;

};
#endif // _IINOUTVARIABLE_H_
