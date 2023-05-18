#ifndef DEFECTLIST_H
#define DEFECTLIST_H
#include "defect/idefectlist.h"
#include "tinyxml.h"
#include "jdir.h"
#include <map>
//#include <mutilmap>
#include <vector>
#include <string>
#include "defect/idefectlistmanage.h"

using namespace std;

class IDefectListManage;

class DefectList : public iDefectList
{
public:
    DefectList();
    void readDefectCfgFile();
	void setDefectEnable(vector<pair<string ,bool > > defctEnableMap);

	vector<string> getDefectCnNameList();
	vector<string> getDefectEnNameList();
	vector<string> getDefectAliasNameList();
	vector<pair<string ,bool>> getDefectEnableList();
	vector<int> getDefectIndexList();
	vector<DefectInfo> getDefectInfo();
	vector<pair<string, int>> getBjAiDefectNameToIndexList();
private:
	void readBjAiDefectNameFile();
    vector<DefectInfo> m_defectInfoVec;
    map<string,DefectInfo> m_defectInfoMap;
	vector<pair<string, bool>> m_defectEnableVec;
	vector<pair<string, int>> m_BjAiDefectNameToIndex;

    vector<string> m_cnNameVec;
    vector<string> m_enNameVec;
    vector<string> m_aliasNameVec;
    vector<int> m_indexVec;
};

class DefectListManage : public IDefectListManage
{
public:
	static DefectListManage *GetInstance();
	static void DestroyInstance();

	virtual DefectList*  getDefectListInstance(int iStation);
	virtual void ReleaseStation(int iStation);
	
private:
	DefectListManage();
	~DefectListManage();
	static DefectListManage *m_pInstace;
	
	DefectList* m_IDefectList[10];
	
};

#endif // DEFECTLIST_H
