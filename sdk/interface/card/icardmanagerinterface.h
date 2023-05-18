#ifndef _CARD_MANAGER_INTERFACE_H_
#define _CARD_MANAGER_INTERFACE_H_

class ICard;
class IAxis;
class IIO;

class ICardManagerInterface
{
public:
	enum
	{
		IID_CARDMANAGERINTERFACE = 0x88,
	};
    virtual ~ICardManagerInterface(){}

    virtual ICard* getCardInstance(int iCardIndex) = 0;
    virtual IAxis* getAxisInstance(int iCardIndex, int iAxisIndex) = 0;
    virtual IIO* getIOInstance(int iCardIndex, int iModelNO) = 0;//modelNO 0为本卡，1递增后为扩展卡
};


#endif
