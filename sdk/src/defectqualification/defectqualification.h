#ifndef DEFECTQUALIFICATION_H
#define DEFECTQUALIFICATION_H

#include "defect/idefectqualification.h"
#include "defect/idefectqualimanage.h"

#include "jmutex.h"

class ScriptLexer;
class CFeatures;
#define  DEFECTTYPENUMBER_TYPE_SIZE	    15		//��ǰ����������
class DefectQualification : public IDefectQualification
{
public:
    DefectQualification(int iStation =0);
    ~DefectQualification();

public:
    //static DefectQualification *GetInstance();
   // static void DestroyInstance();
public:
    /*********************************************************************************
    *����   : ��ȡָ��ȱ�������жϵ�����
    *���   ��defectName  - ȱ�����
    *����   ��N/A
    *����ֵ ����ȱ�ݵ����ؼ���
    *��ע   ��N/A
    *********************************************************************************/
    virtual vector<string> getDefectSeverityType(const string &defectName);


	/*********************************************************************************
	*����   : ��ȡ��������ֵ��ָ��ȱ���ڵ����ؼ���
	*���   ��defectName  - ȱ�����		"LightLeak","ExcessInk","Discolouration","SawEgdge","Smirch",
	*										"AccuLnk","DistHole", "MisPrint","Scratches","Chips","Dent"
	*         nature      - ȱ�ݵ���������	"width,size,length,DiagSize,Aranking,Aarea"
	*         dValue      - ȱ������ֵ
	*����   ��N/A
	*����ֵ ����ȱ�ݵ����ؼ���, �����1��ʼ
	*��ע   ��N/A
	*********************************************************************************/
	virtual bool getDefectSeverityLevel(const int defectType, stDefectFeature* pDefectFeature);

	virtual bool getDefectSeverityLevel(string strChannel,const int defectType, stDefectFeature* pDefectFeature);

    virtual const map<string, Defect_Qualification_Param> *getQualificationParam(string strProduct = "");
    virtual bool setQualificationParam(const map<string, Defect_Qualification_Param> *pParam, string strProduct = "");

	virtual map<string, vector<stdefectlevelRegion>* >* getDefectLevelParamMap(string strChannel = "");
	virtual bool saveDefectLevelParam(string strChannel = "");

    virtual enResFlag getProductFinalRes(stProductSummary* pProductSummary,int  iqualifyIndex = 0);

	virtual string getDefectDesByName(const string& defectName);

	virtual const map<string, stVarExpression> *getVarExpressionParam(string strChannel = "");
	virtual bool setVarExpressionParam(const map<string, stVarExpression> *pParam, string strChannel = "");

	virtual stVarExpression getVarExpression(const string& VartName, string strChannel = "");

	virtual bool isVarExpression(const string& VartName, string strChannel = "");
private:
    void init();

    bool readConfig(string strProduct, map<string, Defect_Qualification_Param>*param);
    bool saveConfig(string strProduct, const map<string, Defect_Qualification_Param> *param);

	bool readDefectLevelXml(string strChannel, map<string, vector<stdefectlevelRegion>* >*param);
	bool saveDefectLevelXml(string strChannel, map<string, vector<stdefectlevelRegion>* >*param);

	const Defect_Result_Param* getCurrDefectQualificationCfg( string ResType,string defectType);

	void initDefectEnableCfg();

	vector<stdefectlevel>* getDefectLevelParamByType(string strChannel = "",string strDefectType = "",int iRegion = 0);
	bool readVarExpressionConfig(string strChannel, map<string, stVarExpression> *pVarExpressionMap);
	bool saveVarExpressionConfig(string strChannel, map<string, stVarExpression> *pVarExpressionMap);

private:
    //static DefectQualification *m_pInstace;
	ScriptLexer* m_pScriptLexer ;
	ScriptLexer* m_pScriptLexerLevel;
	CFeatures*	m_pCFeatures ;
    map<string, map<string, Defect_Qualification_Param> *> m_DefectQualificationParam;

	//  product,defecttype(1~11),level()1~7,item(������÷֣��Աȶ�)      
	//map<string, map<string, map<string, vector<stdefectlevelItem*>>*>*> m_DefectLevel;
	//  product,defecttype(1~11),level()1~7 
	map<string, map<string,vector<stdefectlevelRegion>* >* > m_mapAllDefectLevel;
	map<string, string> m_mapDefectDes;
	map<string, map<string, ScriptLexer*>*> *m_mapScriptLexer;
	map<string, map<string, map<string, ScriptLexer*>*> *> m_mapMultiChannelScriptLexer;

	map<string, map<string, stVarExpression>* > m_mapVarExpression;
	string m_defectAttributeCategory;
	stdefectLevelEnableCfg m_LevelEnableCfg;
    JMutex m_DefectLexerMapMutex;
	int m_defectTypeNumber[DEFECTTYPENUMBER_TYPE_SIZE];
	string m_strCurProductName;
	int m_iStation;
	vector<string> m_defectNameVec;
	vector<string> m_defectEnNameVec;
	vector<pair<string,bool> > m_defectEnableVec;
};


//
class DefectQualimanage : public IDefectQualimanage
{
public:
	static DefectQualimanage *GetInstance();
	static void DestroyInstance();

	//
	virtual IDefectQualification*   getDefectQualificationInstance(int iStation);
	virtual void ReleaseStation(int iStation);
	//

private:
	DefectQualimanage();
	~DefectQualimanage();
	static DefectQualimanage *m_pInstace;
	//
	IDefectQualification* m_IDefectQualification[10];
	//
};
//
#endif // DEFECTQUALIFICATION_H
