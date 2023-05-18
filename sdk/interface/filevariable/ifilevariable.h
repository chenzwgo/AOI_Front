#ifndef _I_FILE_VARIABLE_H_
#define _I_FILE_VARIABLE_H_
#include "cvariantvalue.h"
#include <string>
#include <vector>
using namespace std;

class IKeyVariable
{
public :
    virtual ~IKeyVariable(){}
    virtual bool setValue ( const CVariantValue & value ,bool bSyn = false) =0;
	virtual CVariantValue getValue (const CVariantValue & defaultValue = CVariantValue())  =0;
    virtual bool bindValue (const CVariantPoint & value ) = 0;
};

class IGroupVariable
{
public :
    virtual ~IGroupVariable(){}
	virtual const IKeyVariable* getKeyVariable(const string& strKeyName) =0;
    virtual bool setValue ( const string &Key, const CVariantValue & value ,bool bSyn = false) =0;
	virtual CVariantValue getValue (const string &Key, const CVariantValue & defaultValue = CVariantValue() )  =0;
    virtual bool bindValue ( const string &Key, const CVariantPoint & value ) = 0;
	virtual vector<string> childKeys() = 0;
};


///
/// \brief The IFileVariable class
///
class IFileVariable
{
public :
    virtual ~IFileVariable(){}

    ///
    /// \brief synAllValueToFile  讲所有值写入文件同步
    /// \return  true 写入成功，false 写入失败
    ///
	virtual bool synAllValueToFile() =0;

	virtual const IGroupVariable* getGroupVariable(const string& strGroupName) =0;
    ///
    /// \brief setValue  设置配置项的值
    /// \param Group     配置项组名称
    /// \param Key       配置项key名称
    /// \param value     要写入的值
    /// \param bSyn      写入文件标识， true 写入文件保存，false 仅写入内存
    /// \return  true 写入成功，false 写入失败
    ///
    virtual bool setValue (const string &Group, const string &Key, const CVariantValue & value ,bool bSyn = false) =0;
    ///
    /// \brief getValue     读取配置项的值
    /// \param Group        配置项组名称
    /// \param Key          配置项key名称
    /// \param defaultValue 要读取的值
    /// \return
    ///
	virtual CVariantValue getValue (const string &Group, const string &Key, const CVariantValue & defaultValue = CVariantValue())  =0;

    ///
    /// \brief bindValue    将外部变量的指针绑定在key 中，当用setValue进行操作时。同时改变改变量的值
    /// \param Group        配置项组名称
    /// \param Key          配置项key名称
    /// \param value        绑定的变量指针
    /// \return             true 成功，false 失败
    ///
   virtual bool bindValue(const string &Group, const string &Key, const CVariantPoint & value) = 0;


   ///
   /// \brief getAllKeys  获取分组下所有key值
   /// \param Group       配置项组名称
   /// \return  vector<string>     key列表
   ///
   virtual vector<string> getAllKeys(const string& strGroupName) = 0;

};

///
/// \brief The IFileVariableManager class
///
class IFileVariableManager
{
public:
    enum
    {
        IID_FILEVARIABLEMANAGER= 0x411
    };
    virtual ~IFileVariableManager(){}
    ///
    /// \brief getFileVariable  通过文件获取 IFileVariable 接口文件
    /// \param fileName         文件路径名字
    /// \return
    ///
    virtual IFileVariable* getFileVariable(const string& fileName) =0;
};

#endif // _I_FILE_VARIABLE_H_
