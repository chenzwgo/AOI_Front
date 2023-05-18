
#include "gts_common.h"


string GTS_Common::GetGtsErrStr(int ErrType)
{
	GTS_ERROR_CODE type = (GTS_ERROR_CODE) ErrType;
	string errorstr;
	switch (type)
	{
	case GTS_SUCCESS:
		{
			errorstr =  "";
		}
		break;
	case GTS_EXEC_ERROR:
		{
			errorstr =  "Error-Executed error !";
		}
		break;
	case GTS_LICENSE_WRONG:
		{
			errorstr = "Error-License not supported!";
		}
		break;
	case GTS_PARAM_ERROR:
		{
			errorstr = "Error-Param wrong!";
		}
		break;
	case GTS_LOST_COMM:
		{
			errorstr = "Error-Lost Commucation ! ";
		}
		break;
	case GTS_OPEN_FAIL:
		{
			errorstr = "Error-Open Controller failed !";
		}
		break;
	case GTS_NO_RESP:
		{
			errorstr =  "Error-No Response! ";
		}
		break;
	default:
		{
			errorstr =  "Return Invalided !";
		}
		break;
	}
	if (GTS_SUCCESS !=type )
	{
		//Log::Out() << errorstr <<endl;
	}

	return errorstr ;
}