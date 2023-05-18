#ifndef _ARITHMETIC_MEASURESIZE_H_
#define _ARITHMETIC_MEASURESIZE_H_
#include "g12arithmetic\iarithmeticmeasuresize.h"
#include <map>
#include "jmutex.h"
#include "measuresize.h"

class ArithmeticMeasureSize :public IArithmeticMeasureSize
{
public:
    ArithmeticMeasureSize();
    ~ArithmeticMeasureSize();
    string getArithmeticName();
    void uninit();
    int init(int imageWidth, int imageHeight, double mmPerX=1, double mmPerY=1);
	int calcSize(unsigned char* srcImage);
    int getResultPix(map<string,double> &result);
	int getResultmm(map<string, double> &result);
	int getResultImage(unsigned char* pImgResult);

private:
    int m_iwidth;
    int m_iHeight;
    bool m_bInit;
    JMutex m_mutex;
	MeasureSize *m_CMeasureSize;
};

#endif //_ARITHMETIC_MEASURESIZE_H_

