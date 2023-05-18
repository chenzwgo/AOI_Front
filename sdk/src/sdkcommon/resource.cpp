#include "resource.h"
#include "dllexport.h"
#include "./com/commanager.h"
#include "./modbusport/modbusmanager.h"
#include "./notificationcenter/notificationcenter.h"
#include "./socket/socketclientmanager.h"
#include "./socket/socketservermanager.h"
#include "./errorcode/parseerrormessage.h"
#include "./socket/udpsocketmanager.h"
#include "./socket/udpsocket.h"
#include "./reportcenter/reportcenter.h"

CSdkCommonResource::CSdkCommonResource(void):
    m_bInit(false)
{
    
}


CSdkCommonResource::~CSdkCommonResource(void)
{
    ComManager::DestroyInstance();
    ModbusManager::DestroyInstance();
    CReportCenter::DestroyInstance();
    CNotificationCenter::DestroyInstance();
    SocketClientManager::DestroyInstance();
    SocketServerManager::DestroyInstance();
    ParseErrorMessage::DestroyInstance();
}

bool CSdkCommonResource::Init()
{
    if (m_bInit)
    {
        return true;
    }

    m_bInit = true;
    return true;
}

bool CSdkCommonResource::UnInit()
{
    m_bInit = false;
    return true;
}

void *CSdkCommonResource::queryScriptObject(const int iid)
{
    return NULL;
}

void *CSdkCommonResource::queryPriInterface(const int iid)
{
    if(!m_bInit)
    {
        return NULL;
    }
    if (iid == IComManager::IID_COMMANAGER)
    {
        return ComManager::GetInstance();
    }
    else if(iid == IModbusManager::IID_MODBUSMANAGER)
    {
        return ModbusManager::GetInstance();
    }
    else if(iid == INotificationCenter::IID_NOTIFICATIONCENTER)
    {
        return CNotificationCenter::GetInstance();
    }
    else if(iid == ISocketClientManager::IID_SOCKETCLIENTMANAGER)
    {
        return SocketClientManager::GetInstance();
    }
    else if(iid == ISocketServerManager::IID_SOCKETSERVERMANAGER)
    {
        return SocketServerManager::GetInstance();
    }
    else if(iid == IParseErrorMessage::IID_PARSEERRORMESSAGE)
    {
        return ParseErrorMessage::GetInstance();
    }
    else if(iid == IUdpSocketManager::IID_UDP_SOCKETMANAGER)
    {
        return UdpSocketManager::GetInstance();
    }
    else if (iid == IReportCenter::IID_REPORTCENTER)
    {
        return CReportCenter::GetInstance();
    }
    return NULL;
}

#if defined(SDKCOMMON_RESOURCE_LIBRARY)
#  define RESOURCE_LIBRARY_EXPORT  Q_DECL_EXPORT
#else
#  define RESOURCE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

ISDKResource* m_pResource = 0;
extern "C" RESOURCE_LIBRARY_EXPORT ISDKResource* CreateResource(void)
{
    if (m_pResource == 0)
    {
        m_pResource = new CSdkCommonResource();
    }
    return m_pResource;
}
extern "C" RESOURCE_LIBRARY_EXPORT void ReleaseResource(void)
{
    if (m_pResource != 0)
    {
        delete m_pResource;
        m_pResource = 0;
    }
}

