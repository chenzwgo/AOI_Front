#ifndef WORK_VISION_H
#define WORK_VISION_H

#include "../actioncfgdefine.h"

#define ITEM_NAME_LEN  256             // 流程编辑各项名字长度

#define  VISION_ACTION_NAME         "jobName"
#define  VISION_ACTION_ITEM_NAME    "itemName"

struct WORK_VISION : ACTION_PARAM
{
    string VisonJobName;
	string VisonItemName;

    WORK_VISION()
    {
		VisonJobName = "";
        VisonItemName = "";
    }

    WORK_VISION& operator=(const WORK_VISION &other)
    {
        if (this == &other)
        {
            return *this;
        }
        VisonJobName = other.VisonJobName;
		VisonItemName = other.VisonItemName;
        return *this;
    }

    bool operator==(const WORK_VISION &other)
    {
        return (VisonJobName == other.VisonJobName && 
			VisonItemName == other.VisonItemName);
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ string(VISION_ACTION_NAME) + string(":") + VisonJobName + string(",") + string(VISION_ACTION_ITEM_NAME) + string(":") + VisonItemName + "}";
        return str;
    }
};


#endif // WORKFLOWDEFINE
