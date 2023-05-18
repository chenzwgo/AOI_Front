#ifndef _PROCESSBASE_H_
#define _PROCESSBASE_H_

#include <list>
#include <string>
#include <vector>

using std::list;
using std::string;
using std::vector;

enum EnProcessParamType
{
    UNKNOWN_PROCESS_PARAM = -1,
    IN_PROCESS_PARAM,       //入参
    OUT_PROCESS_PARAM,      //出参
};

enum EnDataType
{
    DATATYPE_UNKNOWN = -1,
    DATATYPE_IMAGE = 0x100,
    DATATYPE_LINE,
    DATATYPE_CIRCLE,
    DATATYPE_POINT,
    DATATYPE_POINTS,
    DATATYPE_ROI,
    DATATYPE_CALIB_FILE,
    DATATYPE_DOUBLE,
    DATATYPE_BOOL,
    DATATYPE_STRING
};

struct stMethodParam
{
public:
    string             strName;           //参数名称
    EnProcessParamType enParamType;       //参数类型，输入/输出
    EnDataType         enDataType;

    stMethodParam() : enParamType(UNKNOWN_PROCESS_PARAM) ,enDataType(DATATYPE_UNKNOWN) { }

    stMethodParam(string Name,EnProcessParamType paramtype,EnDataType datatype)
        : strName(Name)
        , enParamType(paramtype)
        , enDataType(datatype)
    {
    }

    stMethodParam(const stMethodParam &other)
    {
        strName     = other.strName;
        enParamType = other.enParamType;
        enDataType  = other.enDataType;
    }

    stMethodParam & operator= (const stMethodParam &other)
    {
        if (this != &other)
        {
            strName     = other.strName;
            enParamType = other.enParamType;
            enDataType  = other.enDataType;
        }
        return *this;
    }

    bool operator!= (stMethodParam &other)
    {
        return (strName    != other.strName
            || enParamType != other.enParamType);
    }

    bool operator== (stMethodParam &other)
    {
        return !operator!=(other);
    }
};

//流程类型参数
struct ProcessParam
{
    string             strName;          //参数名称
    EnProcessParamType iParamType;       //参数类型，输入/输出
    string             strDataType;      //数据类型：image,line,circle,double

    ProcessParam(): iParamType(UNKNOWN_PROCESS_PARAM) { }

    ProcessParam(const ProcessParam &other)
    {
        strName     = other.strName;
        iParamType  = other.iParamType;
        strDataType = other.strDataType;
    }

    ProcessParam & operator= (const ProcessParam &other)
    {
        if (this != &other)
        {
            strName = other.strName;
            iParamType = other.iParamType;
            strDataType = other.strDataType;
        }
        return *this;
    }

    bool operator!= (ProcessParam &other)
    {
        return (strName    != other.strName
            || iParamType  != other.iParamType
            || strDataType != other.strDataType);
    }

    bool operator == (ProcessParam &other)
    {
        return !operator!=(other);
    }
};

typedef vector<ProcessParam>             ProcessParamList;
typedef ProcessParamList::iterator       ProcessParamIter;
typedef ProcessParamList::const_iterator ProcessParamConstIter;

//流程类型
struct ProcessItem
{
    string strMethod;                   //流程方法，例如findline
    string strDesc;                     //描述，例如找圆、找线等
    string strSdkGuidName;
    string strUiGuidName;
    vector<ProcessParam> vecParams;     //输入、输出参数
};

typedef vector<ProcessItem>             ProcessItemList;
typedef ProcessItemList::iterator       ProcessItemIter;
typedef ProcessItemList::const_iterator ProcessItemConstIter;

//流程类型集合，例如图像输入、检测等
struct ProcessSet
{
    string strName;
    string strDesc;
    vector<ProcessItem> vecProcessItem;
};


//流程子项入参
struct StJobItemParam
{
	string strIn;                // 输入参数名称
	string strInItem;            // 入参来源项
	string strInItemOut;         // 入参来源项的输出参数名称

    string strOut;               // 输出参数名称
    string strType;              // 参数类型

    StJobItemParam() :
	strIn(""), 
	strInItem(""),     
	strInItemOut(""),   
	strOut(""),
	strType("")  
	{ }

    StJobItemParam(const StJobItemParam &other)
        : strIn(other.strIn)
        , strInItem(other.strInItem)
        , strInItemOut(other.strInItemOut)
        , strOut(other.strOut)
        , strType(other.strType)
    {
    }

    StJobItemParam & operator= (const StJobItemParam &other)
    {
        if (this != &other)
        {
            strIn   = other.strIn;
            strInItem      = other.strInItem;
            strInItemOut = other.strInItemOut;
            strOut       = other.strOut;
            strType  = other.strType;
        }
        return *this;
    }

    bool operator!= (const StJobItemParam &other)
    {
        return (strIn  != other.strIn 
            || strInItem      != other.strInItem 
            || strInItemOut != other.strInItemOut
            || strOut       != other.strOut
            || strType   != other.strType);
    }

    bool operator == (const StJobItemParam &other)
    {
        return !operator!=(other);
    }
};

typedef vector<StJobItemParam>             StJobItemParamList;
typedef StJobItemParamList::iterator       StJobItemParamIter;
typedef StJobItemParamList::const_iterator StJobItemParamConstIter;

//流程子项
struct StJobItem
{
    string strName;                     //名称，用于保存参数的文件夹名称
    string strJobType;
    string strDesc;                     //描述，用于UI显示
    vector<StJobItemParam> vecParams;   //流程参数

    StJobItem():strName("")
				,strJobType("")
				,strDesc("")
	{ }

    StJobItem(const StJobItem &other)
        : strName(other.strName)
        , strJobType(other.strJobType)
        , strDesc(other.strDesc)
        , vecParams(other.vecParams)
    {
    }

    StJobItem & operator= (const StJobItem &other)
    {
        if (this != &other)
        {
            strName = other.strName;
            strJobType = other.strJobType;
            strDesc = other.strDesc;
            vecParams = other.vecParams;
        }
        return *this;
    }

    bool operator!= (const StJobItem &other)
    {
        if (strName             != other.strName
            || strJobType       != other.strJobType
            || strDesc          != other.strDesc
            || vecParams.size() != other.vecParams.size())
        {
            return true;
        }

        for (size_t i=0; i<vecParams.size(); i++)
        {
            if(vecParams.at(i) != other.vecParams.at(i))
            {
                return true;
            }
        }
        return false;
    }

    bool operator== (const StJobItem &other)
    {
        return !operator!=(other);
    }
};

typedef vector<StJobItem>             StJobItemList;
typedef StJobItemList::iterator       StJobItemIter;
typedef StJobItemList::const_iterator StJobItemConstIter;

//工作流程
struct StJob
{
    string strName;                 //名称
    vector<StJobItem> vecJobItem;   //工作明细项

    StJob():strName("") 
	{ }

    StJob(const StJob &other) : strName(other.strName), vecJobItem(other.vecJobItem) { }

    StJob & operator= (const StJob &other)
    {
        if (this != &other)
        {
            strName = other.strName;
            vecJobItem = other.vecJobItem;
        }
        return *this;
    }

    bool operator!= (const StJob &other)
    {
        if (strName              != other.strName
            || vecJobItem.size() != other.vecJobItem.size())
        {
            return true;
        }

        for (size_t i=0; i<vecJobItem.size(); i++)
        {
            if(vecJobItem.at(i) != other.vecJobItem.at(i))
            {
                return true;
            }
        }
        return false;
    }

    bool operator== (const StJob &other)
    {
        return !operator!=(other);
    }
};

#endif // _PROCESSBASE_H_