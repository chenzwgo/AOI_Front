#ifndef _I_DEFECTQUALIFICATION_H_
#define _I_DEFECTQUALIFICATION_H_
#include <string>
#include "../../../ui/include/maindefine.h"
#include <map>
#include <vector>
using namespace std;

struct Defect_Result_Param
{
    bool    bEnableQualification; //�Ƿ�ʹ��ȱ���ж�
    //���ȼ��µĸ����ٽ���
	/*
	int  iLevel7Count;
	int  iLevel6Count;
    int  iLevel5Count;
    int  iLevel4Count;
    int  iLevel3Count;
    int  iLevel2Count;
    int  iLevel1Count;
	*/
	int  iLevelCountArr[DEFECT_LEVEL_SIZE];
    Defect_Result_Param()
    {
        bEnableQualification = true;
		/*
		iLevel7Count = 99999;
		iLevel6Count = 99999;
        iLevel5Count = 99999;
        iLevel4Count = 99999;
        iLevel3Count = 99999;
        iLevel2Count = 99999;
        iLevel1Count = 99999;
		*/
		memset(iLevelCountArr, 9999, sizeof(iLevelCountArr));
    }
    Defect_Result_Param(const Defect_Result_Param& other)
    {
        bEnableQualification = other.bEnableQualification;
		/*
		iLevel7Count = other.iLevel7Count;
		iLevel6Count = other.iLevel6Count;
        iLevel5Count = other.iLevel5Count;
        iLevel4Count = other.iLevel4Count;
        iLevel3Count = other.iLevel3Count;
        iLevel2Count = other.iLevel2Count;
        iLevel1Count = other.iLevel1Count;
		*/
		memcpy(iLevelCountArr, other.iLevelCountArr, sizeof(iLevelCountArr));
    }
    Defect_Result_Param &operator=(const Defect_Result_Param& other)
    {
        bEnableQualification = other.bEnableQualification;
		/*
		iLevel7Count = other.iLevel7Count;
		iLevel6Count = other.iLevel6Count;
        iLevel5Count = other.iLevel5Count;
        iLevel4Count = other.iLevel4Count;
        iLevel3Count = other.iLevel3Count;
        iLevel2Count = other.iLevel2Count;
        iLevel1Count = other.iLevel1Count;
		*/
		memcpy(iLevelCountArr, other.iLevelCountArr, sizeof(iLevelCountArr));
        return *this;
    }
};
struct Defect_Severity_Param
{
    bool    bEnableSeverity;  //�Ƿ�ʹ�ּܷ��ж�
    //���ȼ��ж��ٽ���
    double  dLevel5;
    double  dLevel4;
    double  dLevel3;
    double  dLevel2;
    Defect_Severity_Param()
    {
        bEnableSeverity = true;
        dLevel5 = 0;
        dLevel4 = 0;
        dLevel3 = 0;
        dLevel2 = 0;
    }
    Defect_Severity_Param(const Defect_Severity_Param &other)
    {
        bEnableSeverity = other.bEnableSeverity;
        dLevel5 = other.dLevel5;
        dLevel4 = other.dLevel4;
        dLevel3 = other.dLevel3;
        dLevel2 = other.dLevel2;
    }
    Defect_Severity_Param &operator=(const Defect_Severity_Param &other)
    {
        bEnableSeverity = other.bEnableSeverity;
        dLevel5 = other.dLevel5;
        dLevel4 = other.dLevel4;
        dLevel3 = other.dLevel3;
        dLevel2 = other.dLevel2;
        return *this;
    }
};
struct Defect_Qualification_Param
{
    string  strName;  //ȱ������
    string  strDes;       //ȱ������
    map<string, Defect_Severity_Param> mapSeverityParam;
    map<string, Defect_Result_Param> mapResultParam;
    Defect_Qualification_Param()
    {
        strName = "";
        strDes = "";
        mapSeverityParam.clear();
        mapResultParam.clear();
    }
    Defect_Qualification_Param(const Defect_Qualification_Param &other)
    {
        strName     = other.strName    ;
        strDes = other.strDes;
        mapSeverityParam = other.mapSeverityParam;
        mapResultParam = other.mapResultParam;
    }
    Defect_Qualification_Param &operator=(const Defect_Qualification_Param &other)
    {
        strName = other.strName;
        strDes = other.strDes;
        mapSeverityParam = other.mapSeverityParam;
        mapResultParam = other.mapResultParam;
        return *this;
    }
};

class IDefectQualification
{
public:
    //enum { IDD_DEFECT_QULIFICATION = 0x244 };
    virtual ~IDefectQualification() {}
    /*********************************************************************************
    *����   : ��ȡָ��ȱ�������жϵ�����
    *���   ��defectName  - ȱ�����
    *����   ��N/A
    *����ֵ ����ȱ�ݵ����ؼ���
    *��ע   ��N/A
    *********************************************************************************/
    virtual vector<string> getDefectSeverityType(const string &defectName) = 0;

    /*********************************************************************************
    *����   : ��ȡ��������ֵ��ָ��ȱ���ڵ����ؼ���
    *���   ��defectName  - ȱ�����		"LightLeak","ExcessInk","Discolouration","SawEgdge","Smirch",
	*										"AccuLnk","DistHole", "MisPrint","Scratches","Chips","Dent" 
    *         stDefectFeature      - ȱ�ݵ�����
    *����   ��N/A
    *����ֵ �����óɹ�/ʧ��
    *��ע   ��N/A
    *********************************************************************************/
    virtual bool getDefectSeverityLevel(const int defectType, stDefectFeature* pDefectFeature) = 0;

	virtual bool getDefectSeverityLevel(string strChannel, const int defectType, stDefectFeature* pDefectFeature) = 0;
    /*********************************************************************************
    *����   : ��ȡ/����ȱ�ݲ���
    *���   ��N/A
    *����   ��N/A
    *����ֵ ��N/A
    *��ע   ��N/A
    *********************************************************************************/
    virtual const map<string, Defect_Qualification_Param> *getQualificationParam(string strProduct = "") = 0;
    virtual bool setQualificationParam(const map<string, Defect_Qualification_Param> *pParam, string strProduct = "") = 0;

	virtual map<string, vector<stdefectlevelRegion>* >* getDefectLevelParamMap(string strChannel = "") =0;
	virtual bool saveDefectLevelParam( string strChannel = "")=0;


    virtual enResFlag getProductFinalRes(stProductSummary* pProductSummary,int  iqualifyIndex = 0) = 0;

	virtual string getDefectDesByName(const string& defectName) = 0;
	virtual const map<string, stVarExpression> *getVarExpressionParam(string strChannel = "") =0;
	virtual bool setVarExpressionParam(const map<string, stVarExpression> *pParam, string strChannel = "") =0;

	virtual stVarExpression getVarExpression(const string& VartName, string strChannel = "") = 0;

	virtual bool isVarExpression(const string& VartName, string strChannel = "") = 0;
};

#endif // _I_DEFECTQUALIFICATION_H_
