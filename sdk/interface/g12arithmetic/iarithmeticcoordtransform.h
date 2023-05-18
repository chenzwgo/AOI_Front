#ifndef _IARITHMETICCOORDTRANSFORM_H_
#define _IARITHMETICCOORDTRANSFORM_H_
#include <string>
#include <map>
#include "common_data.h"
using namespace std;

class IArithmeticCoordTransform
{
public:
	enum
	{
		IID_MEASURE_SIZE = 0x101,//
	};

	virtual ~IArithmeticCoordTransform() {}
    virtual string getArithmeticName() = 0;
    virtual void uninit() = 0;
	// ¼ÆËã×ø±êÆ«ÒÆ
	virtual int calcOffset(unsigned char* pSrc, int width, int height, int &x, int &y, double &angle) = 0;
	// ³õÊ¼»¯×ø±êÆ«ÒÆ
	virtual int initOffset(int x, int y, double angle) = 0;
	// ×ø±ê»»Ëã
	virtual void map(int x, int y, int &tx, int &ty) = 0;
};

#endif //_IARITHMETICCOORDTRANSFORM_H_

