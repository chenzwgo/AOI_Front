#ifndef _FILE_PATHVARIABLE_H_
#define _FILE_PATHVARIABLE_H_
#include "filevariable/ifilepathvariable.h"
#include "json/json.h"
#include <string>
#include <map>
#include <stack>
#include "jmutex.h"
using namespace std;

class CFilePathVariable:public IFilePathVariable
{
public :
    CFilePathVariable(const string& strfileName);
    unsigned int getChildrenNum(const string &ValuePath);
    string getChildrenName(const string &ValuePath,unsigned int index);
     void setValue ( const string &ValuePath, const CVariantValue & value ,bool bSyn = false) ;
     CVariantValue getValue ( const string &ValuePath, const CVariantValue & defaultValue = CVariantValue()) ;
private:
    bool synAllValueToFile();
    void createVariablemap();
    string GetJsonStringValue(const string& strKey, Json::Value& v);
    bool setJsonStringValue(const string& strKey, const string& strValue,Json::Value& v);
    string jsonVaultToString(Json::Value data);
    void Loop_json(string& key,Json::Value data);
    stack<string> ValuePathParse(const string &ValuePath);
    string m_fileName;
    Json::Value m_Root;
    map<string,CVariantValue> m_mapGroupVariable;
};


#endif  //_FILE_PATHVARIABLE_H_
