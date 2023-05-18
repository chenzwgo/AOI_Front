#include "cardmanagerservice.h"
#include "card/icardmanagerinterface.h"
#include "rtdefine.h"
#include "card/iaxis.h"
#include "card/iio.h"

#ifndef SIMULATE_CARD
    #include "card.h"
#else
    #include "simulate/simcard.h"
#endif

#define CADR_MANAGER CCardManagerService::getInstance()

ICardManagerInterface* CCardManagerService::m_pInstance = NULL;


CCardManagerService::CCardManagerService()
{

}

CCardManagerService::~CCardManagerService()
{

}

ICardManagerInterface* CCardManagerService::GetInstance()
{
	RETURN_V_IF(m_pInstance,m_pInstance); 
	m_pInstance = new CCardManagerService;
	return m_pInstance;
}

void CCardManagerService::DestoryManagerService()
{
	if (NULL != m_pInstance)
	{
		SAFE_DELETE(m_pInstance);
	}
}

ICard* CCardManagerService::getCardInstance(int iCardIndex)
{
	RETURN_V_IF(iCardIndex < 0 || iCardIndex >= 8,NULL);
	return m_cardfac.CreateObject(iCardIndex);
}

IAxis* CCardManagerService::getAxisInstance(int iCardIndex, int iAxisIndex)
{
	RETURN_V_IF(iCardIndex < 0 || iCardIndex >= 8,NULL);
	ICard *pCard = m_cardfac.FindObject(iCardIndex);
	RETURN_V_IF(!pCard, NULL);
	return pCard->GetAxis(iAxisIndex);
}



IIO* CCardManagerService::getIOInstance(int iCardIndex, int iModelNO)
{
	RETURN_V_IF(iCardIndex < 0 || iCardIndex >= 8,NULL);
	ICard *pCard = m_cardfac.CreateObject(iCardIndex);
	RETURN_V_IF(!pCard, NULL);
	return pCard->GetIO(iModelNO);
}


