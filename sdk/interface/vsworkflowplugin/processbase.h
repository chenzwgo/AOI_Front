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
    IN_PROCESS_PARAM,       //���
    OUT_PROCESS_PARAM,      //����
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
    string             strName;           //��������
    EnProcessParamType enParamType;       //�������ͣ�����/���
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

//�������Ͳ���
struct ProcessParam
{
    string             strName;          //��������
    EnProcessParamType iParamType;       //�������ͣ�����/���
    string             strDataType;      //�������ͣ�image,line,circle,double

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

//��������
struct ProcessItem
{
    string strMethod;                   //���̷���������findline
    string strDesc;                     //������������Բ�����ߵ�
    string strSdkGuidName;
    string strUiGuidName;
    vector<ProcessParam> vecParams;     //���롢�������
};

typedef vector<ProcessItem>             ProcessItemList;
typedef ProcessItemList::iterator       ProcessItemIter;
typedef ProcessItemList::const_iterator ProcessItemConstIter;

//�������ͼ��ϣ�����ͼ�����롢����
struct ProcessSet
{
    string strName;
    string strDesc;
    vector<ProcessItem> vecProcessItem;
};


//�����������
struct StJobItemParam
{
	string strIn;                // �����������
	string strInItem;            // �����Դ��
	string strInItemOut;         // �����Դ��������������

    string strOut;               // �����������
    string strType;              // ��������

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

//��������
struct StJobItem
{
    string strName;                     //���ƣ����ڱ���������ļ�������
    string strJobType;
    string strDesc;                     //����������UI��ʾ
    vector<StJobItemParam> vecParams;   //���̲���

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

//��������
struct StJob
{
    string strName;                 //����
    vector<StJobItem> vecJobItem;   //������ϸ��

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