#ifndef WORK_COUNT_H
#define WORK_COUNT_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

//io��������
#define  IO_NAME          "ioName"           
#define  IO_OPEN          "bOpen"            // ����

//variable
#define  COUNT_INDEX      "index"
#define  COUNT_MAX        "max"
#define  COUNT_TYPE       "type"

struct WORK_COUNT : ACTION_PARAM
{
    int countIndex;   // ������
    int countMax;     // ���ֵ,���ڵ������ֵ����false
    int type;         // 0��ʾ���� 1��ʾ����(�ۼ�)
    int currentCount; // ��ǰ����ֵ(����ʱ��������д������)

    WORK_COUNT()
    {
        countIndex = 0;
        countMax = 0;
        type = 0;
        currentCount = 0;
    }

    WORK_COUNT& operator=(const WORK_COUNT &other)
    {
        if (this == &other)
        {
            return *this;
        }

        countIndex = other.countIndex;
        countMax = other.countMax;
        type = other.type;

        return *this;
    }

    bool operator==(const WORK_COUNT &other)
    {
        return countIndex==other.countIndex && countMax==other.countMax && type==other.type;
    }

    void setString(const string &data) {}

    string toString()
    { 
        string str = "{"+ string(COUNT_INDEX) + string(":") + SString::IntToStr(countIndex) \
            + string(",") + string(COUNT_MAX) + string(":") + SString::IntToStr(countMax) \
            + string(",") + string(COUNT_TYPE) + string(":") + SString::IntToStr(type) + "}";
        return str;
    }
};


#endif // WORK_COUNT_H
