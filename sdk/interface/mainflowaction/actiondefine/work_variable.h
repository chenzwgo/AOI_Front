#ifndef WORK_VARIABLE_H
#define WORK_VARIABLE_H

#include "../actioncfgdefine.h"
#include "rtdefine.h"

using namespace std;

#define ITEM_NAME_LEN  256             // ���̱༭�������ֳ���


//�Ƚ�
#define OP_BIGGER             ">"
#define OP_SMALLER            "<"
#define OP_EQUAL              "="
#define OP_NOT_EQUAL          "!="
#define OP_BIGGER_OR_EQUAL    ">="
#define OP_SMALLER_OR_EQUAL   "<="

// ��ֵ
#define OP_ASSIGN             "="
#define OP_SUB_ASSIGN         "-="
#define OP_ADD_ASSIGN         "+="

//variable
#define  VAR_ELEM_NAME        "variable"
#define  VAR_COMPARE          "compare"
#define  VAR_NAME             "name"
#define  VAR_VALUE            "value"
#define  VAR_OPERATE          "operate"

struct WORK_VARIABLE_READ : ACTION_PARAM
{
    // ��һ��Ϊ�������ƣ��ڶ���λ��������������Ϊֵ
    string name;     // ����
    string operate;  // ������
    string value;    // ֵ

    WORK_VARIABLE_READ()
    {
    }

    WORK_VARIABLE_READ& operator=(const WORK_VARIABLE_READ &other)
    {
        if (this == &other)
        {
            return *this;
        }

        name = other.name;
        operate = other.operate;
        value = other.value;
        return *this;
    }

    bool operator==(const WORK_VARIABLE_READ &other)
    {
        return name == other.name 
            && operate == other.operate
            && value == other.value;
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ string(VAR_ELEM_NAME) + string(":") + name + string(",") + string(VAR_OPERATE) + string(":") + operate+ string(",") + string(VAR_VALUE) + string(":") + value + "}";
        return str;
    }
};


struct WORK_VARIABLE_WRITE : ACTION_PARAM
{
    string name;         // ������
    string operate;  // ������
    string value;        // ����ֵ
    WORK_VARIABLE_WRITE()
    {
    }

    WORK_VARIABLE_WRITE& operator=(const WORK_VARIABLE_WRITE &other)
    {
        if (this == &other)
        {
            return *this;
        }

        name = other.name;
        operate = other.operate;
        value = other.value;
        return *this;
    }

    bool operator==(const WORK_VARIABLE_WRITE &other)
    {
        return name == other.name && operate == other.operate && value == other.value;
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ string(VAR_ELEM_NAME) + string(":") + name + string(",") + string(VAR_OPERATE) + string(":") + operate+ string(",") + string(VAR_VALUE) + string(":") + value + "}";
        return str;
    }
};

#endif // WORKFLOWDEFINE
