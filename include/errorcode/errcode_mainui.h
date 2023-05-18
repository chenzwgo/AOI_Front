#ifndef __ERROR_CODE_BUSSINESS_DEFINE_H__
#define __ERROR_CODE_BUSSINESS_DEFINE_H__

#ifndef SYS_ERR_BUSSINESS_START
#define SYS_ERR_BUSSINESS_START    10000					 // 业务错误码
#endif

enum EM_ERR_BUSSINESS										// 业务错误码
{
 
  // 业务错误码
    ERR_BUSSINESS_PARAM = SYS_ERR_BUSSINESS_START,			//回调函数入参错误
	ERR_BUSSINESS_STATION_INSTANCE ,			            //回调函数内使用的工站指针为空
	ERR_BUSSINESS_CARD_INSTANCE ,			                 //回调函数内使用的卡指针为空
};
#endif
