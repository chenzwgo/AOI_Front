#include "filepathvariable.h"
#include "jdir.h"
#include "jfile.h"
#include "json/json.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define  FILEVARIABLE_ROOT_PATH   "./plugin/VariableFile/"

CFilePathVariable::CFilePathVariable(const string& strfileName)
{
    m_fileName=strfileName;
	createVariablemap();

}

unsigned int CFilePathVariable::getChildrenNum(const string &ValuePath)
{
    return -1;
}

string CFilePathVariable::getChildrenName(const string &ValuePath, unsigned int index)
{
    return "";
}

CVariantValue CFilePathVariable::getValue(const string &ValuePath, const CVariantValue &defaultValue)
{
    if(m_mapGroupVariable.find(ValuePath) == m_mapGroupVariable.end())
    {
        return defaultValue;
    }
    return m_mapGroupVariable[ValuePath];
}

void CFilePathVariable::setValue(const string &ValuePath, const CVariantValue &value, bool bSyn)
{
    if(ValuePath.empty())
    {
        return ;
    }
    m_mapGroupVariable[ValuePath] = value;
	return;
    if(!bSyn)
    {
        return ;
    }
    stack<string> stackKey = ValuePathParse(ValuePath);

    Json::Value TmpJsonValue = m_Root;
    while(stackKey.size() >1)
    {
		bool bFound = false;
		string strTopKey = stackKey.top();

		vector<string> keys = TmpJsonValue.getMemberNames();
		for (int i = 0;i <keys.size();i++)
		{
			if(keys[i] == strTopKey)
			{
				//TmpJsonValue = TmpJsonValue.get(strTopKey,"");
				bFound = true; 
				break;
			}
		}
		if (!bFound)
		{
			TmpJsonValue.append(Json::Value(strTopKey));
		}
		TmpJsonValue = TmpJsonValue.get(strTopKey,"");
    }
	string strTopKey = stackKey.top();
	TmpJsonValue[strTopKey] = value.getValueString();

	Json::FastWriter writer;
	string json_file = writer.write(m_Root);
	string path = FILEVARIABLE_ROOT_PATH;
	path += m_fileName;
	ofstream ofs;
	ofs.open(path);
	ofs<<json_file;
	ofs.close();
	return ;
}
void CFilePathVariable::createVariablemap()
{
	string path = FILEVARIABLE_ROOT_PATH;
	path += m_fileName;
	JDir tmpdir(FILEVARIABLE_ROOT_PATH);
	if (!tmpdir.exists())
	{
		tmpdir.mkdir(FILEVARIABLE_ROOT_PATH);
	}

    JFile FileTmp(path);
    if(!FileTmp.exists())
    {
        FileTmp.open("w");
        FileTmp.close();
    }


    //从文件中读取，保证当前文件有test.json文件
    ifstream in;
    in.open (path, std::ios::binary );
    if( !in.is_open() )
    {
        return;
    }
    Json::Reader Read(Json::Features::strictMode());

    if (Read.parse(in,m_Root))
    {
        Loop_json(string(""),m_Root);
    }
    in.close();
    return ;

}


string CFilePathVariable::GetJsonStringValue(const string& strKey,Json::Value& v)
{
    if (!strKey.empty())
    {
        return string("");
    }

    Json::Value::Members mem = v.getMemberNames();
    Json::Value::Members::iterator it;
    //遍历所有key广度搜索
    for (it = mem.begin(); it != mem.end(); it++)
    {
        if (*it == strKey)
        {
            return jsonVaultToString(v[*it]);
        }
        else if(v[*it].type() == Json::objectValue)
        {
            for (int i = 0; i < v[*it].size(); i++)
            {
                //如果在递归的过程中已找到，则返回
                string strvalue = GetJsonStringValue(strKey, v[*it][i]);
                if (!strvalue.empty())
                {
                    return strvalue;
                }
            }
        }
    }

    return string("");
}

bool CFilePathVariable::setJsonStringValue(const string &strKey, const string &strValue,Json::Value& v)
{
    vector<string> keys = v.getMemberNames();
    for (int i = 0;i <keys.size();i++)
    {
        if(keys[i] == strKey)
        {
            v[strKey] = strValue;
            return true;
        }
    }
    return false;
}

string CFilePathVariable::jsonVaultToString(Json::Value data)
{
    if (data.type() == Json::stringValue)
    {
        return data.asString();
    }
    else if (data.type() == Json::arrayValue)
    {
        string strList = "";
        int cnt = data.size();
        for (int i = 0; i < cnt; i++)
        {
            string tmp = jsonVaultToString(data[i]);
            strList.append(tmp);
            if(i != cnt -1)
            {
                strList.append(",");
            }
        }
    }
    else if (data.type() == Json::intValue)
    {
        ostringstream convert;
        convert << data.asInt();
        return   convert.str();
    }
    else if (data.type() == Json::realValue)
    {
        ostringstream convert;
        convert << data.asDouble();
        return   convert.str();
    }
    else if (data.type() == Json::uintValue)
    {
        ostringstream convert;
        convert << data.asUInt();
        return   convert.str();
    }
    else if(data.type() == Json::booleanValue)
    {

        ostringstream convert;
        convert << data.asBool();
        return   convert.str();
    }
    return "";
}


void CFilePathVariable::Loop_json(string& key,Json::Value data)
{
    if(!key.empty())
    {
        key.append("/");
    }
    Json::Value::Members mem = data.getMemberNames();
    for (auto iter = mem.begin(); iter != mem.end(); iter++)
    {
        if (data[*iter].type() == Json::objectValue)
        {
            Loop_json(key.append(*iter),data[*iter]);
        }
        else
        {
            string strValue = jsonVaultToString(data[*iter]);
            m_mapGroupVariable.insert(pair<string,CVariantValue*>(key.append(*iter),new CVariantValue(strValue)));
        }
    }
    return;
}

stack<string> CFilePathVariable::ValuePathParse(const string &ValuePath)
{
    string str = ValuePath;
    string pattern = "/";
    std::string::size_type pos;
    std::stack<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    string::size_type size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;

}
