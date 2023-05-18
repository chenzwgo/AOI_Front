#ifndef  _CFEATURES_H_
#define  _CFEATURES_H_

#include <string>
#include <map>
#include <vector>
#include "jmutex.h"
#include "g12commonexport.h"
#include "filevariable/ifilevariable.h"
using namespace  std;

typedef struct stFeaturesInfo
{
    stFeaturesInfo(){}

    stFeaturesInfo(const string& Name,const string& Unit,const string& Value,const string& Desc)
    {
        strName = Name;
        strUnit = Unit;
        strDefaultValue = Value;
        strDescription = Desc;
    }

    stFeaturesInfo(const stFeaturesInfo& other)
    {
         strName = other.strName;
         strUnit = other.strUnit;
         strDefaultValue = other.strDefaultValue;
         strDescription = other.strDescription;
         strFeaturetype = other.strFeaturetype;
         strBlockorder = other.strBlockorder;
         strShape = other.strShape;
         strIntensity = other.strIntensity;
         strSignal = other.strSignal;
         strChannele = other.strChannele;
    }

    const stFeaturesInfo& operator=(const stFeaturesInfo& other)
    {
        if (&other == this)
        {
            return *this;
        }
        strName = other.strName;
        strUnit = other.strUnit;
        strDefaultValue = other.strDefaultValue;
        strDescription = other.strDescription;
		strFeaturetype = other.strFeaturetype;
		strBlockorder = other.strBlockorder;
		strShape = other.strShape;
		strIntensity = other.strIntensity;
		strSignal = other.strSignal;
		strChannele = other.strChannele;
    }

    string strName;
    string strUnit;
    string strDefaultValue;
    string strDescription;
	string strFeaturetype;
	string strBlockorder;
	string strShape;
	string strIntensity;
	string strSignal;
	string strChannele;
};

typedef struct stDerivedFeature
{
    stDerivedFeature() {}

    stDerivedFeature(const string& Name, const string& Unit, const string& Value, const string& Desc)
    {
        strFeaturesName = Name;
        strFeaturescategory = Unit;
        strFeaturesexpression = Value;
        strFeaturesDescription = Desc;
    }

    stDerivedFeature(const stDerivedFeature& other)
    {
        strFeaturesName = other.strFeaturesName;
        strFeaturescategory = other.strFeaturescategory;
        strFeaturesexpression = other.strFeaturesexpression;
        strFeaturesDescription = other.strFeaturesDescription;
		strWidth = other.strWidth;
		strSize = other.strSize;
		strLength = other.strLength;
		strDiagSize = other.strDiagSize;
		strAranking = other.strAranking;
		strAarea = other.strAarea;
    }

    const stDerivedFeature& operator=(const stDerivedFeature& other)
    {
        if (&other == this)
        {
            return *this;
        }
        strFeaturesName = other.strFeaturesName;
        strFeaturescategory = other.strFeaturescategory;
        strFeaturesexpression = other.strFeaturesexpression;
        strFeaturesDescription = other.strFeaturesDescription;
		strWidth = other.strWidth;
		strSize = other.strSize;
		strLength = other.strLength;
		strDiagSize = other.strDiagSize;
		strAranking = other.strAranking;
		strAarea = other.strAarea;
		return *this;
    }

    string strFeaturesName;         //名字
    string strFeaturescategory;     // 类别
    string strFeaturesexpression;   // 表达式
    string strFeaturesDescription;  // 描述
	string strWidth;
	string strSize;
	string strLength;
	string strDiagSize;
	string strAranking;
	string strAarea;
};

typedef struct stDefectSeverityLevel
{
    stDefectSeverityLevel()
    {
        strFeaturesName = "";
        strFeaturesDescription = "";
        strFeaturesNature = "";
        memset(dSeverityLevel, 0, sizeof(double)*4);
    }
    stDefectSeverityLevel(const string& Name, const string& Description, const string& Nature, const double *Level)
    {
        strFeaturesName = Name;
        strFeaturesDescription = Description;
        strFeaturesNature = Nature;
        memcpy(dSeverityLevel, Level, sizeof(double) * 4);
    }
    stDefectSeverityLevel(const stDefectSeverityLevel& other)
    {
        strFeaturesName = other.strFeaturesName;
        strFeaturesDescription = other.strFeaturesDescription;
        strFeaturesNature = other.strFeaturesNature;
        memcpy(dSeverityLevel, other.dSeverityLevel, sizeof(double) * 4);
    }
    const stDefectSeverityLevel &operator=(const stDefectSeverityLevel& other)
    {
        strFeaturesName = other.strFeaturesName;
        strFeaturesDescription = other.strFeaturesDescription;
        strFeaturesNature = other.strFeaturesNature;
        memcpy(dSeverityLevel, other.dSeverityLevel, sizeof(double) * 4);
        return *this;
    }
    string strFeaturesName;         //名字
    string strFeaturesDescription;  // 描述
    string strFeaturesNature;       // 类别
    double dSeverityLevel[4];       // 5个级别的分界值
};
class CCSVParser;

class RESOURCE_LIBRARY_EXPORT CFeatures
{
public:

    CFeatures(string strChannel);
    ~CFeatures(void);

    bool isBaseFeatureExist(const string& FeaturesName);

    bool isDerivedFeatureExist(const string& FeaturesName);
    
    stFeaturesInfo getBaseFeature(const string& FeaturesName);

    stDerivedFeature getDerivedFeature(const string& FeaturesName);

    bool delDerivedFeature(const string& FeaturesName);

    bool addDerivedFeature(const string& FeaturesName, stDerivedFeature& Feature);

    bool modifyDerivedFeature(const string& FeaturesName, stDerivedFeature& Feature);

    bool saveDerivedFeature();

    const map<string,stFeaturesInfo>* getBaseFeaturesMap();
    
    const map<string, vector<stDerivedFeature>* > * getDerivedFeaturesMap();

	void getDerivedFeatureNames(vector<string>& DerivedFeatures);

	string getCategorysString();

private:

    bool readBaseFeatureData();

    bool readDerivedFeatureData();

    bool writeDerivedFeatureData();

	void clearDerivedFeaturesMap();

private:

    CCSVParser* m_pCCSVParserBaseFeatures;
	
    map<string,stFeaturesInfo> m_mapBaseFeatures;

    map<string, vector<stDerivedFeature>* > m_mapDerivedFeatures;

    string m_strChannel;

	string m_strCategorys;

	JMutex m_FeaturesMapMutex;
	IFileVariable* m_pdataBuffer;

};

class RESOURCE_LIBRARY_EXPORT CFeaturesMgr
{
public:
    static CFeaturesMgr* getCFeaturesMgr();
    static void releaseCFeaturesMgr();
    CFeaturesMgr();
    ~CFeaturesMgr();
     CFeatures* getCurrentCFeatures();
     CFeatures* getCFeatures(const string& strChannel);
private:
    static   CFeaturesMgr* m_pInstance;
    map<string, CFeatures*> m_map;
};

#endif  
