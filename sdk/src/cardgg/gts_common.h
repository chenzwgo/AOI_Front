#ifndef _GG_GTS_COMMON_H_
#define _GG_GTS_COMMON_H_

#include "gts.h"

#include <string>
#include <iostream>
using namespace std;

#define GTS_PORT_OPEN    0
#define GTS_PORT_CLOSE    1


enum GTS_ERROR_CODE
{
	GTS_SUCCESS = 0,
	GTS_EXEC_ERROR = 1,
	GTS_LICENSE_WRONG = 2,
	GTS_PARAM_ERROR = 7,
	GTS_LOST_COMM = -1,
	GTS_OPEN_FAIL = -6,
	GTS_NO_RESP = -7
};

class GTS_Common
{
public:
	enum GTS_ERROR_CODE
	{
		GTS_SUCCESS = 0,
		GTS_EXEC_ERROR = 1,
		GTS_LICENSE_WRONG = 2,
		GTS_PARAM_ERROR = 7,
		GTS_LOST_COMM = -1,
		GTS_OPEN_FAIL = -6,
		GTS_NO_RESP = -7
	};

	static string GetGtsErrStr(int  ErrType);
};
#endif
