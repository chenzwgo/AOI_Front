#ifndef ILOGREPORT_H
#define ILOGREPORT_H

#include <string>
#include <vector>
using namespace std;

typedef void (*pFunLogReport)(string logMsg,int logLevel);

class ILogReport
{
public:
    enum
    {
        IDD_LOG_REPORT = 0x1008
    };

    virtual ~ILogReport(){}

    virtual void addReproter(pFunLogReport pfun) = 0;
    virtual void removeReproter(pFunLogReport pfun ) = 0;
};

#endif // ILOGREPORT

