#ifndef WORK_LOG_H
#define WORK_LOG_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

#define ITEM_NAME_LEN  256             // 流程编辑各项名字长度

//Log动作属性
#define LOG_MSG   "logMsg"
#define LOG_LEVEL "logLevel"

struct WORK_LOG : ACTION_PARAM
{
    string  logMsg;    // log信息
    int   logLevel;    // log级别(0:提示(黑色) 1:警告(蓝色) 2:严重(红色) 3:弹出不标红 4弹出并标红)

    WORK_LOG()
    {
        logMsg = "";
        logLevel = 0;
    }
    WORK_LOG& operator=(const WORK_LOG &other)
    {
        if (this == &other)
        {
            return *this;
        }
        logMsg = other.logMsg;
        logLevel = other.logLevel;
        return *this;
    }

    bool operator==(const WORK_LOG &other)
    {
        return  logMsg == other.logMsg &&
                logLevel == other.logLevel;
    }

    void setString(const string &data) {}

    string toString()
    { 
        string str = "{"+ string(LOG_MSG) + string(":") + logMsg + string(",") \
                      + string(LOG_LEVEL) + string(":") + SString::IntToStr(logLevel) + "}";
        return str;
    }
};

#endif //
