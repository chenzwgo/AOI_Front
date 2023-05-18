#ifndef _I_DLARITHMETIC_H_
#define _I_DLARITHMETIC_H_

#include "maindefine.h"
#include "ProductUnion.h"

class IDlArithmetic
{
public:
	~IDlArithmetic(){}

    virtual int initDeepLearnEnv() = 0;

	virtual int unInitDeepLearnEnv() = 0;

    virtual void SetUnInitDeepLearn() = 0;

	virtual InitDeepLearnState GetDeepLearnEnvState() = 0;

	virtual void readDVConifg(stEnableConfig* pEnableConfig) = 0;

    virtual int runDeepLearn(CImageProduct* pProduct, uint64& uiElapsedTime) = 0;

	virtual void startDefectLevel(CImageProduct* pProduct) = 0;
};

#endif
