#ifndef IDEFECTLIST_H
#define IDEFECTLIST_H
#include <vector>
#include <string>
using namespace std;

struct DefectInfo
{
	int index;
	string name;
	string alias;
	bool isEnable;
    int severity;
    string color;
	string description;
};

class iDefectList
{
public:
    virtual void readDefectCfgFile() = 0;
	virtual vector<string> getDefectCnNameList() = 0;
	virtual vector<string> getDefectEnNameList() = 0;
	virtual vector<string> getDefectAliasNameList() = 0;
	virtual vector<pair<string, bool>>  getDefectEnableList() = 0;
	virtual vector<int> getDefectIndexList() = 0;
	virtual void setDefectEnable(vector<pair<string, bool > >) = 0;
	virtual vector<DefectInfo> getDefectInfo() = 0;
	virtual vector<pair<string, int>> getBjAiDefectNameToIndexList() = 0;
};

#endif // IDEFECTLIST_H
