#ifndef _IMOTION_H_
#define _IMOTION_H_

#include "card/iaxis.h"
#include "card/icard.h"
#include "card/iio.h"
#include "motion/imotioncfg.h"

#include "motor_common.h"


//#define  ERROR_MOTION_OK   0
//#define  ERROR_READ_CFG      1100
//#define  ERROR_SET_AXIS_PARA 1101
//#define  ERROR_INIT_CARD     1102
//#define  ERROR_NO_CARD       1103
//#define  ERROR_NO_AXIS       1104


#include <string>
#include <map>
#include <vector>

class IAxis;
class ICard;
class IIO;
class IStation;
class IIOCtrlInterface;
class iMotion
{

public:
	enum
	{
		IDD_MOTION_MANAGE= 0x77
	};

	virtual~iMotion(){}
	virtual int Init() = 0;

	/////station//
	virtual IStation *GetStationInstance(const char* StationName) = 0;//

    virtual ICard *GetCardInstance(const char* CardName) = 0;

	virtual IAxis *GetAxisInstance(const char* AxisName) = 0;

	virtual IIOCtrlInterface* GetIOCtrlInterface() = 0;

};




#endif  //·ÀÖ¹ÖØ¸´°üº¬
