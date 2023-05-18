#ifndef _GG_CARD_MANAGER_SERVER_H_
#define _GG_CARD_MANAGER_SERVER_H_

#include "card/icardmanagerinterface.h"
#include "isdkresource.h"
#include "objectfactory.h"

class IIO;
class IAxis;
class CCard;

class CCardManagerService : public ICardManagerInterface
{
public:
	CCardManagerService();
	~CCardManagerService();

	ICard* getCardInstance(int iCardIndex);
	IAxis* getAxisInstance(int iCardIndex, int iAxisIndex);
	IIO* getIOInstance(int iCardIndex, int iModelNO);//modelNO 0为本卡，1递增后为扩展卡

	static ICardManagerInterface* GetInstance();
	static void DestoryManagerService();
private:
	static ICardManagerInterface *m_pInstance;
	objectFactory<int ,CCard> m_cardfac;
};

#endif
