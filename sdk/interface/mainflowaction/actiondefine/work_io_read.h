#ifndef WORK_IO_READ_H
#define WORK_IO_READ_H

#include "../actioncfgdefine.h"

//io动作属性
#ifndef IO_CFG
#define  IO_NAME          "ioName"           
#define  IO_RW            "ioRW"             // io读写
#define  IO_OPEN          "bOpen"            // 开关
#else
#define  IO_CFG
#endif

#include <map>
using namespace std;

struct WORK_IO_READ : ACTION_PARAM
{
    map<string, bool>  mapIo;

    WORK_IO_READ()
    {
        mapIo.clear();
    }

    WORK_IO_READ& operator=(const WORK_IO_READ &other)
    {
        if (this == &other)
        {
            return *this;
        }

		mapIo.clear();
		map<string, bool>::const_iterator iter = other.mapIo.begin();
		for (iter; iter != other.mapIo.end(); iter++)
		{
			mapIo.insert(pair<string, bool>(iter->first, iter->second));
		}

        return *this;
    }

    bool operator==(const WORK_IO_READ &other)
    {
		if(mapIo.size() != other.mapIo.size())
		{
			return false;
		}

		map<string, bool>::const_iterator iter = other.mapIo.begin();
		for (iter; iter != other.mapIo.end(); iter++)
		{
			if(mapIo[iter->first] != iter->second)
			{
				return false;
			}
		}

		return true;
    }

    void setString(const string &data) {}

    string toString()
    { 
        string str;
        map<string, bool>::const_iterator iter = mapIo.begin();
        for (iter; iter != mapIo.end(); iter++)
        {
            str += "{"+ string(IO_NAME) + string(":") + iter->first + string(",") + string(IO_OPEN) + string(":") + string(iter->second?"1":"0") + "}";
            if (iter != mapIo.end()--)
            {
                str += ",";
            }
        }
        
        return str;
    }
};


#endif // WORKFLOWDEFINE
