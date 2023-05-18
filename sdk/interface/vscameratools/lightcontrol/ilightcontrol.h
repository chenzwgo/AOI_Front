#ifndef __ILIGHTCONTROL_H__
#define __ILIGHTCONTROL_H__
#include "sdkcommon/com/icom.h"
class ICom;
class ILightControl
{
public:
	enum
	{
		IDD_LIGHT_OPT = 0x77
	};
    virtual ~ILightControl(){};

	virtual bool Init(const char* Comname) =0;
	virtual bool OpenLightContol() =0;
	virtual bool LigControllerIsOpen() =0;
	virtual bool CloseCom() = 0;

    virtual int SetSingleCh(int ChannalNum,unsigned int BOpen,unsigned  int ChValue) = 0;
    virtual int GetSingleCh(int ChannalNum) = 0;

	virtual bool OpenLightChannel(unsigned short ChannalNum,unsigned short ChannelValue)	= 0;
	virtual bool SetLightChannelValue(unsigned short ChannalNum,unsigned short ChannelValue) = 0;
	virtual bool GetLightChannelValue(unsigned short ChannalNum,unsigned short *ChannelValue) = 0;
	virtual bool CloseLightChannel(unsigned short ChannalNum,unsigned short ChannelValue) = 0;
};
#endif
