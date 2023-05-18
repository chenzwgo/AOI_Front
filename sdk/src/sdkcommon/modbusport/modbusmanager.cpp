#include "modbusmanager.h"

ModbusManager* ModbusManager::m_pInstance = NULL;

ModbusManager::ModbusManager()
{

}

ModbusManager::~ModbusManager()
{

}

ModbusManager *ModbusManager::GetInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new ModbusManager();
    }
    return m_pInstance;
}

void ModbusManager::DestroyInstance()
{
    if(m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

IModbusMsg *ModbusManager::CreateModbus(const char *pName)
{
    if(pName == NULL || strlen(pName) == 0)
    {
        return NULL;
    }
    return m_modbusfac.CreateObject(pName);
}

void ModbusManager::ReleaseModbus(IModbusMsg *pModbus)
{
    if(pModbus == NULL)
    {
        return ;
    }
    CModbusMsg* ptemp = dynamic_cast<CModbusMsg*>(pModbus);
    if(ptemp == NULL)
    {
        return ;
    }
    m_modbusfac.DestroyObject(ptemp);
}

