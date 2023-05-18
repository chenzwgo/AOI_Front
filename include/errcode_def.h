#ifndef __ERROR_CODE_DEFINE_H__
#define __ERROR_CODE_DEFINE_H__

typedef int ERROR_CODE;

// #define SYS_ERR_SYSTEM_START 		 0		
// #define SYS_ERR_MAINFLOW_START 	    300						// 流程错误代码起始
// #define SYS_ERR_ACTION_START 		500		  			// 具体动作错误码起始 
// #define SYS_ERR_MOTION_START 		1000					// motion错误码起始 
// #define SYS_ERR_SAFECHECK_START      1500					// safecheck错误码起始 
// #define SYS_ERR_STATION_START 	    2000					// station误码起始 
// #define SYS_ERR_CARD_START 			2500		        // card错误码起始 
// #define SYS_ERR_BUSSINESS_START	    10000               // 业务错误码


#include "./errorcode/errcode_card.h"
#include "./errorcode/errcode_mainflow.h"
#include "./errorcode/errcode_mainflowaction.h"
#include "./errorcode/errcode_motion.h"
#include "./errorcode/errcode_safecheck.h"
#include "./errorcode/errcode_station.h"
#include "./errorcode/errcode_mainui.h"     // 业务错误码
enum EM_ERR_SYSTEM							// 系统错误码
{
   //0- 300～  为系统共用错误码
   SYS_ERR_OK = 0,                  //成功	
};

#endif
