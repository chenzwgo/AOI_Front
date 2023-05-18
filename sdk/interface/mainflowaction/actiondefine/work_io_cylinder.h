#ifndef WORK_IO_CYLINDER_H
#define WORK_IO_CYLINDER_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

//气缸动作属性
#define  CYLINDER_IO_ORIGIN_NAME        "ioOriginName"    
#define  CYLINDER_IO_TERMINAL_NAME      "ioTerminalName"  
#define  CYLINDER_IO_OPEN_NAME          "ioOpenName"
#define  CYLINDER_IO_CLOSE_NAME         "ioCloseName"
#define  CYLINDER_OPEN                  "open"

struct WORK_IO_CYLINDER : ACTION_PARAM
{
    string  cOriginIoName;   // 原位IO
    string  cTerminalIoName; // 到位IO
    string  cOpenIoName;     // 打开IO
    string  cCloseIoName;    // 关闭IO
    bool  bOpen;                          // 打开/关闭气缸

    WORK_IO_CYLINDER()
    {
        cOriginIoName = "";
        cTerminalIoName = "";
        cOpenIoName = "";
        cCloseIoName = "";
        bOpen = true;
    }
    WORK_IO_CYLINDER& operator=(const WORK_IO_CYLINDER &other)
    {
        if (this == &other)
        {
            return *this;
        }
        cOriginIoName = other.cOriginIoName;
        cTerminalIoName = other.cTerminalIoName;
        cOpenIoName = other.cOpenIoName;
        cCloseIoName = other.cCloseIoName;
        bOpen = other.bOpen;
        return *this;
    }

    bool operator==(const WORK_IO_CYLINDER &other)
    {
        return cOriginIoName == other.cOriginIoName&&
            cTerminalIoName == other.cTerminalIoName&&
            cOpenIoName == other.cOpenIoName&&
            cCloseIoName == other.cCloseIoName&&
            bOpen == other.bOpen;
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+\
                         string(CYLINDER_IO_ORIGIN_NAME) + string(":") + cOriginIoName + string(",") + \
                         string(CYLINDER_IO_TERMINAL_NAME) + string(":") + cTerminalIoName  + string(",") + \
                         string(CYLINDER_IO_OPEN_NAME) + string(":") + cOpenIoName  + string(",") + \
                         string(CYLINDER_IO_CLOSE_NAME) + string(":") + cCloseIoName  + string(",") + \
                         string(CYLINDER_OPEN) + string(":") + SString::IntToStr(bOpen)  + string(",") + \
                      "}";
        return str;
    }
};


#endif // WORKFLOWDEFINE
