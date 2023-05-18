#ifndef WORK_FUNCTION_H
#define WORK_FUNCTION_H

#include "../actioncfgdefine.h"

// FUN
#define  FUN_NAME          "cFunName"
#define  FUN_PARAM         "cParam"

struct WORK_FUNCTION : ACTION_PARAM
{
    string cFunName;
    string cParam;

    WORK_FUNCTION()
    {
        cFunName = "";
        cParam = "";
    }

    WORK_FUNCTION& operator=(const WORK_FUNCTION &other)
    {
        if (this == &other)
        {
            return *this;
        }
        cFunName = other.cFunName;
        cParam = other.cParam;
        return *this;
    }


    bool operator==(const WORK_FUNCTION &other)
    {
        return (cFunName == other.cFunName
            && cParam == other.cParam);
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ \
                        string(FUN_NAME) + string(":") + cFunName + "," + \
                        string(FUN_PARAM) + string(":") + cParam + "," + \
                      "}";
        return str;
    }
};

#endif // WORKFLOWDEFINE
