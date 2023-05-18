#ifndef WORK_VARIABLE_H
#define WORK_VARIABLE_H

#include "../actioncfgdefine.h"
#include "rtdefine.h"

using namespace std;

#define ITEM_NAME_LEN  256             // 流程编辑各项名字长度


//比较
#define OP_BIGGER             ">"
#define OP_SMALLER            "<"
#define OP_EQUAL              "="
#define OP_NOT_EQUAL          "!="
#define OP_BIGGER_OR_EQUAL    ">="
#define OP_SMALLER_OR_EQUAL   "<="

// 赋值
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
    // 第一项为变量名称，第二项位操作符，第三项为值
    string name;     // 变量
    string operate;  // 操作符
    string value;    // 值

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
    string name;         // 变量名
    string operate;  // 操作符
    string value;        // 变量值
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
