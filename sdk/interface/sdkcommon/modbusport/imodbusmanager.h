#ifndef _IMODBUS_MANAGER_H_
#define _IMODBUS_MANAGER_H_

class IModbusMsg;
class IModbusManager
{
public:
		enum
		{
			IID_MODBUSMANAGER= 0x201
		};
    virtual ~IModbusManager(){}

    virtual IModbusMsg* CreateModbus(const char* pName)= 0;
    virtual void ReleaseModbus(IModbusMsg* pModbus)= 0;
};
#endif
