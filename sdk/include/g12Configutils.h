#ifndef _G12_CONFIG_UTILS_H_
#define _G12_CONFIG_UTILS_H_
#include "g12commonexport.h"
#include "../ui/include/maindefine.h"
#include <string>
#include <vector>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

    RESOURCE_LIBRARY_EXPORT  stMutilCamCfg* getMutilCamCfg();

	RESOURCE_LIBRARY_EXPORT  ChannelCamCfg* getUpCamCfg();

	RESOURCE_LIBRARY_EXPORT  ChannelCamCfg* getDownCamCfg();

#ifdef __cplusplus
}
#endif

#endif _G12_CONFIG_UTILS_H_

