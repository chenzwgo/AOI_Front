#ifndef _IARITHMETICCUTIMAGE_H_
#define _IARITHMETICCUTIMAGE_H_
#include <string>
#include <map>
#include "common_data.h"
using namespace std;

class IArithmeticMeasureSize
{
public:
	enum
	{
		IID_MEASURE_SIZE = 0x101,//
	};

	virtual ~IArithmeticMeasureSize() {}
    virtual string getArithmeticName() = 0;
    virtual void uninit() = 0;
	virtual int init(int imageWidth, int imageHeight, double mmPerX = 1, double mmPerY = 1) = 0;
	virtual int calcSize(unsigned char* srcImage) = 0;
	virtual int getResultPix(map<string,double> &result) = 0;
	virtual int getResultmm(map<string, double> &result) = 0;
	virtual int getResultImage(unsigned char* pImgResult) = 0;
};

#endif //_IARITHMETICCUTIMAGE_H_

