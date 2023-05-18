#ifndef _DLARITHMETIC2_H_
#define _DLARITHMETIC2_H_
#include "idlArithmetic.h"
#include "classification_interface.h"
#include "libsvm_interface.h"
#include "grading_interface.h"
#include "jthread.h"
#include "jthreadt.h"

#include "defect/idefectlist.h"
#include "defect/idefectlistmanage.h"

#define  DV_DEFECTRESUL     8

class IDefectQualification;
class iDefectList;
class CDlArithmetic2 : public IDlArithmetic
{
public:
	static IDlArithmetic *getInstance(int iStation);
	static void releaseInstance();

    int initDeepLearnEnv(); 

	int unInitDeepLearnEnv(); 

    void SetUnInitDeepLearn();

	InitDeepLearnState GetDeepLearnEnvState();

	void readDVConifg(stEnableConfig* pEnableConfig);

    int runDeepLearn(CImageProduct* pProduct, uint64& uiElapsedTime); 

    void stopDeepLearn();

	void startDefectLevel(CImageProduct* pProduct);

	//
private:
    CDlArithmetic2(int iStation =0);
    ~CDlArithmetic2();
	void readConifg();
	int updateDeepLearnEnv();

	void DvDefectToDefectFeature(CImageProduct* pProduct, dv::DefectAttr Attr, string strchannel, int iDhLevel = 0);
	void DvDefectToDefectFeature(CImageProduct* pProduct, stDefectFeature DefectFeature, string strchannel);

	bool getDefectLevel(string strChannel, stDefectFeature *pDefectRes);		//��ȱ�ݵȼ������ж�

    bool getDefectLevel(stDefectFeature *pDefectRes);
    static CDlArithmetic2 *m_pInstance;

    void ThreadDefectLevelProcess(string param);

	int getDefectType(int iType,bool &bSeverity);

	JMutex  m_HLock;
    JMutex  m_DefectLevelLock;
	dv::BJInputParam     m_hInputParam;   //H�㷨���
	dv::DefectResultInfo m_defectResult[DV_DEFECTRESUL];
	dv::RippleResultInfo m_rippleResultInfo;//ˮ���ƽṹ�壬����û��ʹ�ã����ܺ����ʹ�á�
	//PredictorHandle m_svmHandle;

	dv::DeformationInfo m_DeformatrionInfo;
	stDVHandles m_DVHandles;

	bool m_bIsHInit;      //H�㷨�Ƿ��ʼ��

	bool m_bUnInitDeepLearn;//�㷨����ʼ����־
	bool m_bRoiDeepLearn;//Roi���
    bool m_bRun;
	stDvConfig m_dvConfig; // H�㷨������Ϣ
	stEnableConfig* m_pEnableConfig;
	IDefectQualification *m_pIDefectQualification;
	iDefectList *m_pIDefectlist;
    map<string, JThreadTP<CDlArithmetic2, string, void>* > m_mapDefectLevelThreads;

    bool m_bMultiThread;
    CImageProduct* m_pProduct;
	GradingHandle  m_hGradingHandle;
	GradingHandle  m_hEdgeModel;
	int miStation;

	vector<string> m_defectNameVec;
	vector<string> m_enNameVec;
	vector<int> m_defectIndexVec;
	vector<pair<string, bool>> m_defectEnableVec;
	vector<DefectInfo> m_defectInfoList;
};

#endif
