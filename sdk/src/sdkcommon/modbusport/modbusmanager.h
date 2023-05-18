#ifndef _MODBUSMANAGER_H
#define _MODBUSMANAGER_H
#include "objectfactory.h"
#include "modbusport.h"
#include "sdkcommon/modbusport/imodbusmanager.h"
#include <string>
using namespace  std;


class ModbusManager: public IModbusManager
{
public:
    static  ModbusManager* GetInstance();
    static void DestroyInstance();
    IModbusMsg* CreateModbus(const char* pName);
     void ReleaseModbus(IModbusMsg* pModbus);
private:
    ModbusManager();
    ~ModbusManager();
    objectFactory<string,CModbusMsg> m_modbusfac;
    static ModbusManager* m_pInstance;
};

#endif // _MODBUSMANAGER_H
