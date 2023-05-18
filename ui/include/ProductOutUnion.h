#pragma once
#include <QList>
#include <vector>

using namespace std;

#include "maindefine.h"
#include "g12globalenv.h"
#include "jmutex.h"
//#include "classification_interface.h"
#include <QTime>
#include <QPolygon>
#include "maindefine.h"

enum enPrOutFlag
{
	en_Pr_Wait = 0,   // 等待信号
    en_Pr_Moving,     // 开始移动
    en_Pr_OutBegin,   // 到出料口
};

#include "jthreadt.h"
class CProductOut
{
public:
	CProductOut(unsigned long long ullsn);
    ~CProductOut();
	
	enPrOutFlag getFlag();
	void setResultFlag(enResFlag rstFlag);
	void setResultMsgToPenma(QStringList strMsg);
	enResFlag getResultFlag();
	unsigned long long getSn();
	unsigned long long getStartTime();
	unsigned long getMoveTimeBySet();
	unsigned long getOffTimeBySet();
	unsigned long getWcTimeBySet();
	QStringList getPenmaMsg();
	bool isCurrentProduct();
	void setResultDataFlag(stProductDataRes* dataFlag);
	stProductDataRes* getResultDataFlag();
private:
	void ThreadTimeOut(void* param);   
	JThreadTP<CProductOut, void*, void>* m_pthread;// 
	void startTimeOut();
    JMutex m_mutexStatus;
	JMutex m_mutexTime;   
	bool   m_bRun;
	long   m_Time;//预定移动时间
	unsigned long   m_OffTime;//玻璃在光纤上移动时间
	unsigned long   m_WcTime;//误差时间
	enPrOutFlag m_flag;
	enResFlag ResultFlag;
	unsigned long long  m_proSn;
	unsigned long long  m_starTime;
	QStringList m_strQListMsg;
	stProductDataRes m_stProductDataRes;
};
struct productResData
{
	int Sn;
	enResFlag result;
	QTime time;
};

class COutUnion
{

public:
    static COutUnion *getInstance();
    static void releaseInstance();
	void createPrOut(unsigned long long ullsn);
	enPrOutFlag getFirstFlag();
	CProductOut* getFirstProductOut();
	unsigned long long getFirstSn();
	unsigned long long getProductOutSn(int index);
    void releaseFirstPrOut();
	void releasePrOut(CProductOut* pProOut);
    void releaseAllPrOut();

    //获取当前产品数量
    int getPrOutCnt();
	void setResultFlag(unsigned long long ullsn,enResFlag ResultFlag);
	void setResultMsgToPenma(unsigned long long ullsn, QStringList strMsg);
	enResFlag getResultFlag(unsigned long long ullsn);
	void setResultDataFlag(unsigned long long ullsn, stProductDataRes* dataFlag);
	
private:

    static COutUnion *m_pInstance;
	COutUnion(void);
    ~COutUnion(void);
    QList<CProductOut*> m_listPrOut;
    JMutex m_mutexPrOutList;
	
};

///
class CProductOutLightCtr
{

public:
	static CProductOutLightCtr *getInstance();
	static void releaseInstance();
    bool startOutLightCtr(int iSn,enResFlag ResultFlag,QString strRed, QString strYellow, QString strGreen,bool Robot=true,int iIoDelay=0);
private:

	static CProductOutLightCtr *m_pInstance;
	CProductOutLightCtr(void);
	~CProductOutLightCtr(void);
	bool m_bRun;
	void ThreadLightCtr(void* param);
	JThreadTP<CProductOutLightCtr, void*, void>* m_pthread;// 
	QString m_strRed;
	QString m_strYellow;
	QString m_strGreen;
	int m_itimeMax;
	enResFlag m_ResultFlag;
	bool m_bRobot;
	int m_iIodelay;
	int m_iSn;
	bool m_bRobotCheck;
	list<productResData* > m_ListRobotResData;
	JMutex m_RobotResDataLock;
	//
};
