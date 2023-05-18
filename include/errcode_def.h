#ifndef __ERROR_CODE_DEFINE_H__
#define __ERROR_CODE_DEFINE_H__

typedef int ERROR_CODE;

// #define SYS_ERR_SYSTEM_START 		 0		
// #define SYS_ERR_MAINFLOW_START 	    300						// ���̴��������ʼ
// #define SYS_ERR_ACTION_START 		500		  			// ���嶯����������ʼ 
// #define SYS_ERR_MOTION_START 		1000					// motion��������ʼ 
// #define SYS_ERR_SAFECHECK_START      1500					// safecheck��������ʼ 
// #define SYS_ERR_STATION_START 	    2000					// station������ʼ 
// #define SYS_ERR_CARD_START 			2500		        // card��������ʼ 
// #define SYS_ERR_BUSSINESS_START	    10000               // ҵ�������


#include "./errorcode/errcode_card.h"
#include "./errorcode/errcode_mainflow.h"
#include "./errorcode/errcode_mainflowaction.h"
#include "./errorcode/errcode_motion.h"
#include "./errorcode/errcode_safecheck.h"
#include "./errorcode/errcode_station.h"
#include "./errorcode/errcode_mainui.h"     // ҵ�������
enum EM_ERR_SYSTEM							// ϵͳ������
{
   //0- 300��  Ϊϵͳ���ô�����
   SYS_ERR_OK = 0,                  //�ɹ�	
};

#endif
