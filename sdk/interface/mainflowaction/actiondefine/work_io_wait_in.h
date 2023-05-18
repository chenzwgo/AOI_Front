#ifndef WORK_IO_WAIT_IN_H
#define WORK_IO_WAIT_IN_H

#include "../actioncfgdefine.h"

//io动作属性
#ifndef IO_CFG
#define  IO_NAME          "ioName"           
#define  IO_RW            "ioRW"             // io读写
#define  IO_OPEN          "bOpen"            // 开关
#else
#define  IO_CFG
#endif

struct WORK_IO_WAIT_IN : ACTION_PARAM
{
    string  cIoName;
    bool  bIoOpen;                          // 读取IO，直到值为bIoOpen 
    //int   timeOut;               // 超时时间ms

    WORK_IO_WAIT_IN()
    {
        cIoName = "";
        bIoOpen  = false;
        //timeOut = 0;
    }
    WORK_IO_WAIT_IN& operator=(const WORK_IO_WAIT_IN &other)
    {
        if (this == &other)
        {
            return *this;
        }
        cIoName = other.cIoName;
        bIoOpen = other.bIoOpen;
        //timeOut = other.timeOut;
        return *this;
    }

    bool operator==(const WORK_IO_WAIT_IN &other)
    {
        return cIoName == other.cIoName &&
            bIoOpen == other.bIoOpen; //&&
            //timeOut == other.timeOut;
    }

    void setString(const string &data){}
    string toString()
    { 
        string str = "{"+ string(IO_NAME) + string(":") + cIoName + string(",") + string(IO_OPEN) + string(":") + string(bIoOpen?"1":"0") + "}";
        return str;
    }
};

#endif // WORKFLOWDEFINE
