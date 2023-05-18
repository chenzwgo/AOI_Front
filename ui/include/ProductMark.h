#pragma once
#include <QList>
#include <vector>
using namespace std;
#include "maindefine.h"
#include "g12globalenv.h"
#include "jmutex.h"
#include <QTime>
#include <QPolygon>
#include "maindefine.h"
#include "jthreadt.h"

//

struct stPrMark
{
	unsigned long long uSn;          //sn
	enResFlag          eFlag;        //���
	QStringList            strPrinterMsg;//��������
	stPrMark()
	{
		uSn           = 0;
		eFlag         = res_unknow;
	};
	stPrMark& operator=(const stPrMark& other)
	{
		uSn           = other.uSn;
		eFlag         = other.eFlag;
		strPrinterMsg = other.strPrinterMsg;
		return *this;
	}
	void init()
	{
		uSn           = 0;
		eFlag         = res_Ng;
	}
};

//
class CProductMark
{
public:
	CProductMark(unsigned long long ullsn);
    ~CProductMark();
	//����Ʒȱ����Ϣ��������������
	void setDefectsVecToPrinter(vector<stDefectFeature*>* pDefectStrutVec, enResFlag rstFlag);
	void setMsgToPrinter(QStringList strListMsg, enResFlag rstFlag);
	//��ȡ��������
	stPrMark getMarkFlag();
	unsigned long long getSn();
	vector<string> m_defectAliasNameVec;
private:
	stPrMark  mMarkFlag;
};

class CPrMarkUnion
{

public:
    static CPrMarkUnion *getInstance();
    static void releaseInstance();
	void createPrMark(unsigned long long ullsn);
	
	CProductMark* getFirstProductMark();

	unsigned long long getFirstSn();
    void releaseFirstPrMark();
	void releasePrMark(CProductMark* pPrMark);
    void releaseAllPrMark();

    //��ȡ��ǰ��Ʒ����
    int getPrMarkCnt();

	//����Ϣ����Ӧ��Ʒsn����������������
	void setUnionDefectsVecToPrinter(unsigned long long ullsn, vector<stDefectFeature*>* pDefectStrutVec, enResFlag rstFlag);
	void setMsgToPrinter(unsigned long long ullsn, QStringList strListMsg, enResFlag rstFlag);
	//��ȡ��Ӧsn����������
	stPrMark getUnionMarkFlag(unsigned long long ullsn);

	//��ȡ��һ����������
	stPrMark getFirstPrMarkFlag();
	//
	void SnToPenmaData(long sn, QStringList& Penmadata);
	void  DefectToPenmaData(stDefectFeature *pDefectRes , string strLine, QStringList& Penmadata);
	QString snToStr(long sn);
	//
private:

    static CPrMarkUnion *m_pInstance;
	CPrMarkUnion(void);
    ~CPrMarkUnion(void);
    QList<CProductMark*> m_listPrMark;
    JMutex   m_mutexPrMarkList;
	stPrMark mMark;
    vector<string> m_defectAliasNameVec;
};
//
///
class ICom;
class IIOCtrlInterface;
class CPrPrinterCtr
{

public:
	static CPrPrinterCtr *getInstance();
	static void releaseInstance();
	//����������Ϣ ����true��ʼ�������� false ����δ����
	bool startPrinterCtr(stPrMark eFlag);

private:
	CPrPrinterCtr(void);
	~CPrPrinterCtr(void);

	void ThreadPrinterCtr(void* param);
	void toPenmaMsg(QStringList& msgList,char penmsg[4][64]);

private:	
	static CPrPrinterCtr *m_pInstance;
	JThreadTP<CPrPrinterCtr, void*, void>* m_pthread;// 

	bool      m_bRun;
	int       m_itimeMax;
	enResFlag m_ResultFlag;
	stPrMark  mMark;

	bool m_bEnalbe;
	IIOCtrlInterface* m_pIo;
	int m_idelayTime;
	string m_strComName;
	
	//
};


