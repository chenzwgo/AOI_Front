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


    int SendProductSN(unsigned int proSN );				//���Ͳ�Ʒsn        0 ���ͳɹ� 1 δʹ��ǰ���Ѷ  2.����ʧ�ܣ�3 ����ʧ��
    int queryBackPcStatus(int& iStatus);							//��ѯ��̨PC��״̬  0 ��ѯ�ɹ� 1 δʹ��ǰ���Ѷ  2.����ʧ�ܣ�3 ��ѯʧ��
    int queryBackPcProductSN(unsigned int& proSN);				//��ѯ��̨PC��״̬  0 ��ѯ�ɹ� 1 δʹ��ǰ���Ѷ  2.����ʧ�ܣ�3 ��ѯʧ��
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
