#ifndef _CARITHMETIC_RESOUCE_H_
#define _CARITHMETIC_RESOUCE_H_
#include "isdkresource.h"

class ArithmeticStaticMgr;
class ArithmeticMeasureSize;

class CArithmeticResource :public ISDKResource
{
public:
    CArithmeticResource(void);
    ~CArithmeticResource(void);

    bool Init() ;
    bool UnInit() ;

    void* queryScriptObject(const int iid) ;

    void* queryPriInterface(const int iid);

private:
    bool m_bInit;
    ArithmeticStaticMgr* m_pArithmeticStaticMgr;
	ArithmeticMeasureSize* m_pArithmeticMeasureSize;
};



#endif  //_CARITHMETIC_RESOUCE_H_

