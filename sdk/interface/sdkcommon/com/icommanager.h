#ifndef _ICOM_MANAGER_H_
#define _ICOM_MANAGER_H_

class ICom;
class IComManager
{
public:
		enum
		{
			IID_COMMANAGER= 0x200
		};
    virtual ~IComManager(){}

    virtual ICom* CreateCom(const char* pName) = 0;
    virtual void ReleaseCom(ICom* pCom) = 0;
};
#endif
