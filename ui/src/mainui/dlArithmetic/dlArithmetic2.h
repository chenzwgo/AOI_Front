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

	bool getDefectLevel(string strChannel, stDefectFeature *pDefectRes);		//对缺陷等级进行判定

    bool getDefectLevel(stDefectFeature *pDefectRes);
    static CDlArithmetic2 *m_pInstance;

    void ThreadDefectLevelProcess(string param);

	int getDefectType(int iType,bool &bSeverity);

	JMutex  m_HLock;
    JMutex  m_DefectLevelLock;
	dv::BJInputParam     m_hInputParam;   //H算法入参
	dv::DefectResultInfo m_defectResult[DV_DEFECTRESUL];
	dv::RippleResultInfo m_rippleResultInfo;//水波纹结构体，现在没有使用，可能后面会使用。
	//PredictorHandle m_svmHandle;

	dv::DeformationInfo m_DeformatrionInfo;
	stDVHandles m_DVHandles;

	bool m_bIsHInit;      //H算法是否初始化

	bool m_bUnInitDeepLearn;//算法反初始化标志
	bool m_bRoiDeepLearn;//Roi检测
    bool m_bRun;
	stDvConfig m_dvConfig; // H算法配置信息
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
