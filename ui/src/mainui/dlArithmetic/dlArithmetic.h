#ifndef _DLARITHMETIC_H_
#define _DLARITHMETIC_H_
#include "idlArithmetic.h"
#include "jthread.h"
#include "jthreadt.h"
#include "bjcv.h"
#include "defect/idefectlist.h"
#include "defect/idefectlistmanage.h"
#define  DV_DEFECTRESUL_1     8

class IDefectQualification;
class CDlArithmetic : public IDlArithmetic
{
public:
	static IDlArithmetic *getInstance(int iStation);
	static void releaseInstance();

    int initDeepLearnEnv(); 

	int unInitDeepLearnEnv(); 

    void SetUnInitDeepLearn();

	InitDeepLearnState GetDeepLearnEnvState();

	void readDVConifg(stEnableConfig* pEnableConfig);

    int runDeepLearn(CImageProduct* pProduct, uint64& uiElapsedTime); //一个图像

    void stopDeepLearn();

	void startDefectLevel(CImageProduct* pProduct);

private:
    CDlArithmetic(int iStation=0);
    ~CDlArithmetic();
	void readConifg();
	int updateDeepLearnEnv();

	void DvDefectToDefectFeature(CImageProduct* pProduct, stDefectFeature DefectFeature, string strchannel);

	void BjaiDvDefectToDefectFeature(CImageProduct* pProduct, stDefectFeature DefectFeature, string strchannel);

	bool getDefectLevel(string strChannel, stDefectFeature *pDefectRes);		//对缺陷等级进行判定

    bool getDefectLevel(stDefectFeature *pDefectRes);
    static CDlArithmetic *m_pInstance;

	void initDefectIndex();
	int getDefectIndex(string name);

    void ThreadDefectLevelProcess(string param);
	int getDefectType(int iType, bool &bSeverity);
	JMutex  m_HLock;
    JMutex  m_DefectLevelLock;


	bool m_bIsHInit;      //H算法是否初始化

	bool m_bUnInitDeepLearn;//算法反初始化标志
	bool m_bRoiDeepLearn;//Roi检测
    bool m_bRun;
	stDvConfig m_dvConfig; // H算法配置信息
	stEnableConfig* m_pEnableConfig;
	IDefectQualification *m_pIDefectQualification;
	iDefectList *m_pIDefectlist;
    map<string, JThreadTP<CDlArithmetic, string, void>* > m_mapDefectLevelThreads;

    bool m_bMultiThread;
    CImageProduct* m_pProduct;
	int miStation;
	vector<string> m_defectNameVec;
	vector<string> m_enNameVec;
	vector<int> m_defectIndexVec;
	vector<pair<string, bool>> m_defectEnableVec;
	vector<DefectInfo> m_defectInfoList;
	vector<pair<string, int>> m_defectIndex;
    bjcv::ModelHandle model_handle_ptr;
    bjcv::BJCVResults m_results_ptr;
};

#endif
