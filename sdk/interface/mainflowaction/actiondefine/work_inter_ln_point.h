#ifndef WORK_INTER_LN_POINT_H
#define WORK_INTER_LN_POINT_H

#include "../actioncfgdefine.h"

struct WORK_INTER_LN_POINT : ACTION_PARAM
{
    int	 index;						//µãÎ»Ë÷Òý
    double dVel;
    double dAcc;
    double dEndVel;

    WORK_INTER_LN_POINT()
    {
        index = 0;;
        dVel = 0;
        dAcc = 0;
        dEndVel = 0;
    }

    WORK_INTER_LN_POINT& operator=(const WORK_INTER_LN_POINT &other)
    {
        if (this == &other)
        {
            return *this;
        }
        index = other.index;
        dVel = other.dVel;
        dAcc = other.dAcc;
        dEndVel = other.dEndVel;
        return *this;
    }
    bool operator==(const WORK_INTER_LN_POINT &other)
    {
        return index == other.index &&
            dVel  == other.dVel &&
            dAcc  == other.dAcc &&
            dEndVel == other.dEndVel;
    }

    void setString(const string &data){}
    string toString(){return "";}
};

#endif // WORKFLOWDEFINE
