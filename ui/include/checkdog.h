#ifndef _CHECKDOG_PLUGIN_H_
#define _CHECKDOG_PLUGIN_H_
#include "dllexport.h"
#include "uidefine.h"

class RT_API CCheckDog
{
public:
    static bool hasHardwareDog();
    static bool checkHardDogLicenseValid();
    static bool checkSoftLicenseValid(int &iexpireDays);


};


#endif _CHECKDOG_PLUGIN_H_

