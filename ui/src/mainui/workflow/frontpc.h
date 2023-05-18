#ifndef FRONTPC_H
#define FRONTPC_H
#include "maindefine.h"
#include <string>
#include <QMutex>
#include<QList>
using namespace std;

class CFrontPCtoBack
{
public :
    static CFrontPCtoBack *getInstance();
    static void releaseInstance();


    int SendProductSN(unsigned int proSN );				//发送产品sn        0 发送成功 1 未使能前后道讯  2.连接失败，3 发送失败
    int queryBackPcStatus(int& iStatus);							//查询下台PC的状态  0 查询成功 1 未使能前后道讯  2.连接失败，3 查询失败
    int queryBackPcProductSN(unsigned int& proSN);				//查询下台PC的状态  0 查询成功 1 未使能前后道讯  2.连接失败，3 查询失败
    int sendProductInfo(unsigned int proSN, int iProductRes);

	//
	//void pushBackSynResult(unsigned int iSn, int iRest, unsigned long long ulStartTime=0);
	void pushBackSynResult(unsigned int iSn, int iRest, int iNgOrRiDefectType,string str2Dcode);
	bool getForntMsgBySn(int iSn,stProductDataRes &PrMsg);
	void ClearFrontMsg();
private:
    CFrontPCtoBack();
    ~CFrontPCtoBack();

private:
    static CFrontPCtoBack *m_pInstance;
    bool m_bEnable;
	QMutex		m_synResultocker;
	QList<stProductDataRes> mQListFrontRes;
	QMap<unsigned int, stProductDataRes> m_proResult;
};

#endif // FRONTPC_H
