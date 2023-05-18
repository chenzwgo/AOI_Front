#ifndef WORK_JUMP_H
#define WORK_JUMP_H

#include "../actioncfgdefine.h"
#include <vector>

//io动作属性
#define  IO_NAME          "ioName"           
#define  IO_OPEN          "bOpen"            // 开关

//variable
#define  VAR_ELEM_NAME        "variable"
#define  VAR_COMPARE          "compare"
#define  VAR_NAME             "name"
#define  VAR_VALUE            "value"
#define  VAR_OPERATE          "operate"

#include <map>
using namespace std;

#ifndef stVarCondition
struct stVarCondition
{
    // 第一项为变量名称，第二项位操作符，第三项为值
    string name;     // 变量
    string operate;  // 操作符
    string value;    // 值

    stVarCondition()
    {
    }

    stVarCondition& operator=(const stVarCondition &other)
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

    bool operator==(const stVarCondition &other)
    {
        return name == other.name 
            && operate == other.operate
            && value == other.value;
    }
};

#endif

struct WORK_JUMP : ACTION_PARAM
{
    map<string, bool>  mapIo;   // IO条件列表
    vector<stVarCondition>  vecVar; // 变量条件列表
    string logicOper;       // 逻辑操作符，取值"&&"或"||"

    WORK_JUMP()
    {
        mapIo.clear();
        vecVar.clear();
        logicOper = "&&";
    }

    WORK_JUMP& operator=(const WORK_JUMP &other)
    {
        if (this == &other)
        {
            return *this;
        }

        logicOper = other.logicOper;
        mapIo.clear();
        map<string, bool>::const_iterator iter = other.mapIo.begin();
        for (iter; iter != other.mapIo.end(); iter++)
        {
            mapIo.insert(pair<string, bool>(iter->first, iter->second));
        }

        vecVar.clear();
        for (int i=0; i<other.vecVar.size(); i++)
        {
            vecVar.push_back(other.vecVar.at(i));
        }

        return *this;
    }

    bool operator==(const WORK_JUMP &other)
    {
        if(mapIo.size() != other.mapIo.size())
        {
            return false;
        }

        if(vecVar.size() != other.vecVar.size())
        {
            return false;
        }

        if (0 != logicOper.compare(other.logicOper))
        {
            return false;
        }

        map<string, bool>::const_iterator iter = other.mapIo.begin();
        for (iter; iter != other.mapIo.end(); iter++)
        {
            if(mapIo[iter->first] != iter->second)
            {
                return false;
            }
        }

        for (int i=0; i<other.vecVar.size(); i++)
        {
            if(!(vecVar.at(i) == other.vecVar.at(i)))
            {
                return false;
            }
        }
        return true;
    }

    void setString(const string &data) {}

    string toString()
    { 
        string str;
        map<string, bool>::const_iterator iter = mapIo.begin();
        for (iter; iter != mapIo.end(); iter++)
        {
            str += "{"+ string(IO_NAME) + string(":") + iter->first + string(",") + string(IO_OPEN) + string(":") + string(iter->second?"1":"0") + "}";
            if (iter != mapIo.end()--)
            {
                str += ",";
            }
        }
        for (int i=0; i<vecVar.size(); i++)
        {
            str += "{"+ string(VAR_NAME) + string(":") + vecVar.at(i).name + string(",") + string(VAR_OPERATE) + string(":") + vecVar.at(i).operate + string(",") + string(VAR_VALUE) + string(":") + vecVar.at(i).value + "}";
            if (i != vecVar.size())
            {
                str += ",";
            }
        }
        return str;
    }
};


#endif // WORK_JUMP_H
