#ifndef WORK_SCRIPT_H
#define WORK_SCRIPT_H

#include "../actioncfgdefine.h"

#define  SCRIPT_ACTION_NAME        "scriptName"

struct WORK_SCRIPT : ACTION_PARAM
{
    string cScriptName;
    WORK_SCRIPT()
    {
        cScriptName = "";
    }

    WORK_SCRIPT& operator=(const WORK_SCRIPT &other)
    {
        if (this == &other)
        {
            return *this;
        }
        cScriptName =other.cScriptName;
        return *this;
    }

    bool operator==(const WORK_SCRIPT &other)
    {
        return (cScriptName == other.cScriptName);
    }

    void setString(const string &data){}
    string toString()
    { 
        string str = "{"+ string(SCRIPT_ACTION_NAME) + string(":") + cScriptName + "}";
        return str;
    }
};


#endif // WORKFLOWDEFINE
