#include "commanager.h"

ComManager* ComManager::m_pInstance = NULL;

ComManager::ComManager()
{

}

ComManager::~ComManager()
{

}

ComManager *ComManager::GetInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new ComManager();
    }
    return m_pInstance;
}

void ComManager::DestroyInstance()
{
    if(m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

ICom *ComManager::CreateCom(const char *pName)
{
    if(pName == NULL || strlen(pName) == 0)
    {
        return NULL;
    }
    return m_comfac.CreateObject(pName);
}

void ComManager::ReleaseCom(ICom *pCom)
{
    if(pCom == NULL)
    {
        return ;
    }
    ComControl* ptemp = dynamic_cast<ComControl*>(pCom);
    if(ptemp == NULL)
    {
        return ;
    }
    m_comfac.DestroyObject(ptemp);
}

