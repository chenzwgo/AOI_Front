#ifndef WORK_INTER_DELAY_H
#define WORK_INTER_DELAY_H

#include "../actioncfgdefine.h"

struct WORK_INTER_DELAY : ACTION_PARAM
{
    int time;           // ms
    WORK_INTER_DELAY()
    {
        time = 0;
    }

    WORK_INTER_DELAY& operator=(const WORK_INTER_DELAY &other)
    {
        if (this == &other)
        {
            return *this;
        }
        time = other.time;
        return *this;
    }
    bool operator==(const WORK_INTER_DELAY &other)
    {
        return time == other.time;
    }

    void setString(const string &data){}
    string toString(){return "";}
};



#endif // WORKFLOWDEFINE
