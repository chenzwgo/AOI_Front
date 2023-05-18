#ifndef _ARITHMETIC_COORDTRANSFORM_H_
#define _ARITHMETIC_COORDTRANSFORM_H_
#include "g12arithmetic\iarithmeticcoordtransform.h"
#include <map>
#include "jmutex.h"
#include "coordtransform.h"

class ArithmeticCoordTransform :public IArithmeticCoordTransform
{
public:
    ArithmeticCoordTransform();
    ~ArithmeticCoordTransform();
    string getArithmeticName();
    void uninit();

	// ��������ƫ��
	int calcOffset(unsigned char* pSrc, int width, int height, int &x, int &y, double &angle);
	// ��ʼ������ƫ��
    int initOffset(int x, int y, double angle);
	// ���껻��
	void map(int x, int y, int &tx, int &ty);

private:
    int m_iwidth;
    int m_iHeight;
    bool m_bInit;
    JMutex m_mutex;
	CoordTransform *m_coordTransform;
};

#endif //_ARITHMETIC_COORDTRANSFORM_H_

