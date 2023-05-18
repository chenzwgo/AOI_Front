#ifndef WORK_SET_EVENT_H
#define WORK_SET_EVENT_H

#include "../actioncfgdefine.h"


//event
#define  EVENT_ELEM_NAME        "workevent"
#define  EVENT_NAME             "name"
#define  EVENT_STOP_VAR         "varstop"


struct WORK_SET_EVENT : ACTION_PARAM
{
    string cEventName;

    WORK_SET_EVENT()
    {
        cEventName = "";
    }

    WORK_SET_EVENT& operator=(const WORK_SET_EVENT &other)
    {
        if (this == &other)
        {
            return *this;
        }
        cEventName = other.cEventName;
        return *this;
    }

    bool operator==(const WORK_SET_EVENT &other)
    {
        return (cEventName ==other.cEventName);
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ \
                          string(EVENT_NAME) + string(":") + cEventName + \
                     "}";
        return str;
    }
};


struct WORK_WAIT_EVENT : ACTION_PARAM
{
    string cEventName;
    string cVarStopWait;  // 停止等待的条件(变量)
    WORK_WAIT_EVENT()
    {
        cEventName = "";
        cVarStopWait = "None";
    }

    WORK_WAIT_EVENT& operator=(const WORK_WAIT_EVENT &other)
    {
        if (this == &other)
        {
            return *this;
        }
        cEventName = other.cEventName;
        cVarStopWait = other.cVarStopWait;
        return *this;
    }

    bool operator==(const WORK_WAIT_EVENT &other)
    {
        return (cEventName == other.cEventName) && (cVarStopWait == other.cVarStopWait);
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ \
                          string(EVENT_NAME) + string(":") + cEventName + \
                          string(EVENT_STOP_VAR) + string(":") + cVarStopWait + \
                     "}";
        return str;
    }
};

#endif // WORKFLOWDEFINE
