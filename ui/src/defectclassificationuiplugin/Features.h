#ifndef  _CFEATURES_H_
#define  _CFEATURES_H_

#include <string>
#include <map>

using namespace  std;

typedef struct stFeaturesInfo
{
    stFeaturesInfo(){}

    stFeaturesInfo(const string& Name,const string& Unit,const string& Value,const string& Desc)
    {
        strFeaturesName = Name;
        strFeaturesUnit = Unit;
        strFeaturesDefaultValue = Value;
        strFeaturesDescription = Desc;
    }

    stFeaturesInfo(const stFeaturesInfo& other)
    {
         strFeaturesName = other.strFeaturesName;
         strFeaturesUnit = other.strFeaturesUnit;
         strFeaturesDefaultValue = other.strFeaturesDefaultValue;
         strFeaturesDescription = other.strFeaturesDescription;
    }

    const stFeaturesInfo& operator=(const stFeaturesInfo& other)
    {
        if (&other == this)
        {
            return *this;
        }
        strFeaturesName = other.strFeaturesName;
        strFeaturesUnit = other.strFeaturesUnit;
        strFeaturesDefaultValue = other.strFeaturesDefaultValue;
        strFeaturesDescription = other.strFeaturesDescription;
    }

    string strFeaturesName;
    string strFeaturesUnit;
    string strFeaturesDefaultValue;
    string strFeaturesDescription;
};

class CCSVParser;

class CFeatures
{
public:


    static CFeatures* getInstance();
    static void releaseInstance();
    CFeatures(void);
    ~CFeatures(void);

    bool isBaseFeatureExist(const string& FeaturesName);

    bool isDerivedFeatureExist(const string& FeaturesName);
    
    stFeaturesInfo getBaseFeature(const string& FeaturesName);

    stFeaturesInfo getDerivedFeature(const string& FeaturesName);

    const map<string,stFeaturesInfo>* getBaseFeaturesMap();
    
    const map<string,stFeaturesInfo>* getDerivedFeaturesMap();

private:

    bool readBaseFeatureData();

    bool readDerivedFeatureData();

private:

    CCSVParser* m_pCCSVParserBaseFeatures;

    CCSVParser* m_pCCSVParserDerivedFeatures;

    map<string,stFeaturesInfo> m_mapBaseFeatures;

    map<string,stFeaturesInfo> m_mapDerivedFeatures;

    static CFeatures* m_pInstance;

};


#endif  