#ifndef COMMANAGER_H
#define COMMANAGER_H
#include "objectfactory.h"
#include "comcontrol.h"
#include "sdkcommon/com/icommanager.h"

class ComManager: public IComManager
{
public:
    static  ComManager* GetInstance();
    static void DestroyInstance();
    ICom* CreateCom(const char* pName);
     void ReleaseCom(ICom* pCom);
private:
    ComManager();
    ~ComManager();
    objectFactory<string,ComControl> m_comfac;
    static ComManager* m_pInstance;
};

#endif // COMMANAGER_H
