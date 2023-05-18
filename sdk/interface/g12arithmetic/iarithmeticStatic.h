#ifndef _IARITHMETICSTATIC_H_
#define _IARITHMETICSTATIC_H_
#include <string>
using namespace std;

class IArithmeticStatic
{
public:
    virtual ~IArithmeticStatic(){};

    virtual string getArithmeticName() = 0;
    virtual void uninit() = 0;
	virtual int Teaching(unsigned char *data, int imageWidth, int imageHeight, int standardGv, int startLine, int endLine, int startCol, int endCol, float *factors) = 0;
    virtual int iniStatCorr(int imageWidth,int icorrectHeight) = 0;
	virtual int StatCorr(unsigned char *image,unsigned char *correctImage)=0;
    virtual int GetStatParam(unsigned char *image,int imageWidth,int imageHeight,unsigned char &maxGray,unsigned char &minGray,double &mean,double &devi) = 0;
};

class IArithmeticStaticMgr
{
public:
    enum{IDD_ARITHMETICSTATICMGR = 0x22};
    virtual ~IArithmeticStaticMgr(){}
    virtual IArithmeticStatic* getArithmeticStatic(const string& strChannel) = 0;
};

#endif //_IARITHMETICSTATIC_H_

