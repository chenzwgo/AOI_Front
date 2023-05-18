#ifndef WORK_IO_OUTPUT_H
#define WORK_IO_OUTPUT_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

#define ITEM_NAME_LEN  256             // 流程编辑各项名字长度

//io动作属性
#ifndef IO_CFG
#define  IO_NAME          "ioName"           
#define  IO_RW            "ioRW"             // io读写
#define  IO_OPEN          "bOpen"            // 开关
#define  IO_REVERSEDELAY  "ioReverseDelay"   // io反转延时
#else
#define  IO_CFG
#endif

struct WORK_IO_OUTPUT : ACTION_PARAM
{
    string  cIoName;
    bool  bIoOpen;                          // 写入或读取的值
    int   reverseDelay;                     // 反转延时

    WORK_IO_OUTPUT()
    {
        cIoName = "";
        bIoOpen  = false;
        reverseDelay = -1;
    }
    WORK_IO_OUTPUT& operator=(const WORK_IO_OUTPUT &other)
    {
        if (this == &other)
        {
            return *this;
        }
        cIoName = other.cIoName;
        bIoOpen = other.bIoOpen;
        reverseDelay = other.reverseDelay;
        return *this;
    }

    bool operator==(const WORK_IO_OUTPUT &other)
    {
        return  cIoName == other.cIoName &&
            bIoOpen == other.bIoOpen &&
            reverseDelay == other.reverseDelay;
    }

    void setString(const string &data) {}

    string toString()
    { 
        string str = "{"+ string(IO_NAME) + string(":") + cIoName + string(",") \
                      + string(IO_OPEN) + string(":") + string(bIoOpen?"1":"0") + string(",")  \
                      + string(IO_REVERSEDELAY) + string(":") + SString::IntToStr(reverseDelay) + "}";
        return str;
    }
};

#endif // WORKFLOWDEFINE
