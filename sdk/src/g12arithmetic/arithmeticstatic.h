#ifndef _ARITHMETICSTATIC_H_
#define _ARITHMETICSTATIC_H_
#include "g12arithmetic\iarithmeticStatic.h"
#include <map>
#include "jmutex.h"
/*
*    静态校准
*    standardGray ：设置为该通道基准值 
*    （TFB,RFB 一般为128）
*    （FFDD 暗场一般为15）
*/

class CFastStaticCorrection;
class ArithmeticStatic :public IArithmeticStatic
{
public:
    ArithmeticStatic(string strChannel);
    ~ArithmeticStatic();
    string getArithmeticName();
    void uninit();
	int Teaching(unsigned char *data, int imageWidth, int imageHeight, int standardGv, int startLine, int endLine, int startCol, int endCol, float *factors);
    int iniStatCorr(int imageWidth,int icorrectHeight);
	int StatCorr(unsigned char *image, unsigned char *correctImage);
    int GetStatParam(unsigned char *image,int imageWidth,int imageHeight,unsigned char &maxGray,unsigned char &minGray,double &mean,double &devi);

private:
    bool readFactorFile();
private:
    CFastStaticCorrection* m_pCFastStaticCorrection;
    string m_strChannel;
	float *m_pFactor;
    float *m_pfcorrectImage;
    int m_iwidth;
    int m_iHeight;
    bool m_bInit;
	bool m_DisEnable;
	bool m_bFloatFlag;
    JMutex m_mutex;
};


class ArithmeticStaticMgr:public IArithmeticStaticMgr
{
public:
    ArithmeticStaticMgr();
    ~ArithmeticStaticMgr();
    IArithmeticStatic* getArithmeticStatic(const string& strChannel) ;
private:
    map<string ,ArithmeticStatic*> m_map;
};

#endif //_ARITHMETICSTATIC_H_

