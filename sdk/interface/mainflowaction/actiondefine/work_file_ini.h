#ifndef WORK_FILE_INI_H
#define WORK_FILE_INI_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

#define  ITEM_NAME_LEN  256             // 流程编辑各项名字长度

#define  File_Ini_FileName				"FileName"
#define  File_Ini_GroupName			    "GroupName"
#define  File_Ini_KeyName				"KeyName"
#define  File_Ini_VariableName		    "VariableName"
#define  File_Ini_RWMode				"RWMode"


struct WORK_FILE_INI : ACTION_PARAM
{
    string FileName;   	 // 文件路径
    string GroupName;    // Group 名字
    string KeyName;  		 // key 名字
    string VariableName; // 变量 名字
    int iRWMode;  			// 0 读， 1 写

    WORK_FILE_INI()
    {
        FileName = "";
        GroupName = "";
        KeyName = "";
        VariableName = "";
        iRWMode = -1;
    }

    WORK_FILE_INI& operator=(const WORK_FILE_INI &other)
    {
        if (this == &other)
        {
            return *this;
        }
        FileName = other.FileName;
        GroupName = other.GroupName;
        KeyName = other.KeyName;
        VariableName = other.VariableName;
        iRWMode = other.iRWMode;
        return *this;
    }
    bool operator==(const WORK_FILE_INI &other)
    {
        return (FileName == other.FileName 
            && GroupName == other.GroupName 
            && KeyName == other.KeyName 
            && VariableName == other.VariableName
            && iRWMode == other.iRWMode);
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ \
            string(File_Ini_FileName) + string(":") + FileName + "," + \
            string(File_Ini_GroupName) + string(":") + GroupName + "," + \
            string(File_Ini_KeyName) + string(":") + KeyName + "," + \
            string(File_Ini_VariableName) + string(":") + VariableName + "," + \
            string(File_Ini_RWMode) + string(":") + SString::IntToStr(iRWMode) + "," + \
            "}";
        return str;
    }
};


#endif // WORK_FILE_INI
