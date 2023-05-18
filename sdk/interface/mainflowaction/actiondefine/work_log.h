#ifndef WORK_LOG_H
#define WORK_LOG_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

#define ITEM_NAME_LEN  256             // ���̱༭�������ֳ���

//Log��������
#define LOG_MSG   "logMsg"
#define LOG_LEVEL "logLevel"

struct WORK_LOG : ACTION_PARAM
{
    string  logMsg;    // log��Ϣ
    int   logLevel;    // log����(0:��ʾ(��ɫ) 1:����(��ɫ) 2:����(��ɫ) 3:��������� 4���������)

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
