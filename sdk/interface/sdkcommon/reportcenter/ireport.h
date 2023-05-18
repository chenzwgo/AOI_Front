#ifndef IREPORT_H
#define IReport_H

#include "OS_DEFINE.h"

class IReport
{
public:

    virtual ~IReport(){}
    virtual bool reprot(const char* pevent, OS_LPARAM wparam, OS_LPARAM lparam) = 0;
};



#endif // IREPORT_H
