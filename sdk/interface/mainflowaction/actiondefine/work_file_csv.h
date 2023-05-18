#ifndef WORK_FILE_CSV_H
#define WORK_FILE_CSV_H

#include "../actioncfgdefine.h"
#include "stringcommon.h"

#define  ITEM_NAME_LEN  256             // 流程编辑各项名字长度

#define  File_CSV_FileDir				"FileDir"
#define  File_CSV_FileNamePre			"FileNamePre"
#define  File_CSV_FileNameSuffix		"FileNameSuffix"
#define  File_CSV_FileHeader		    "FileHeader"
#define  File_CSV_VariableName		    "VariableName"
#define  File_CSV_RWMode				"RWMode"
#define  File_CSV_ROW				    "ROW"
#define  File_CSV_COL				    "COL"

struct WORK_FILE_CSV : ACTION_PARAM
{
    string FileDir;   	        // 文件路径
    string FileNamePre;         // 文件名字
    int bAddFileNameSuffix;     // 自动增加时间后缀 0 不增加，1为增加时间后缀
    string strHeaders;          // CSV 标题头部
    string VariableName;        // 变量 名字
    int iRWMode;  		        // 0 写整行,1 读整行,2 写单项,3 读单项,4 读取行号,5 读取列号
    int iRow;                   // 行号
    int iCol;                   // 列号

    WORK_FILE_CSV()
    {
        FileDir = "";   	    
        FileNamePre = "";     
        bAddFileNameSuffix= false;
        strHeaders = "";      
        VariableName="";    
        iRWMode = 0;  		    
        iRow = 0;               
        iCol = 0;               
    }

    WORK_FILE_CSV& operator=(const WORK_FILE_CSV &other)
    {
        if (this == &other)
        {
            return *this;
        }
        FileDir = other.FileDir;
        FileNamePre = other.FileNamePre;
        bAddFileNameSuffix = other.bAddFileNameSuffix;
        VariableName = other.VariableName;
        strHeaders = other.strHeaders;
        iRWMode = other.iRWMode;
        iRow = other.iRow;                
        iCol = other.iCol;        
        return *this;
    }
    bool operator==(const WORK_FILE_CSV &other)
    {
        return (
            FileDir == other.FileDir 
            && FileNamePre == other.FileNamePre 
            && bAddFileNameSuffix == other.bAddFileNameSuffix 
            && VariableName == other.VariableName
            && strHeaders == other.strHeaders
            && iRow == other.iRow                
            && iCol == other.iCol  
            && iRWMode == other.iRWMode);
    }

    void setString(const string &data){}
    string toString()
    {
        string str = "{"+ \
            string(File_CSV_FileDir) + string(":") + FileDir + "," + \
            string(File_CSV_FileNamePre) + string(":") + FileNamePre + "," + \
            string(File_CSV_FileNameSuffix) + string(":") + SString::IntToStr(bAddFileNameSuffix) + "," + \
            string(File_CSV_FileHeader) + string(":") + strHeaders + "," + \
            string(File_CSV_VariableName) + string(":") + VariableName + "," + \
            string(File_CSV_RWMode) + string(":") + SString::IntToStr(iRWMode) + "," + \
            string(File_CSV_ROW) + string(":") + SString::IntToStr(iRow) + "," + \
            string(File_CSV_COL) + string(":") + SString::IntToStr(iCol) + "," + \
            "}";
        return str;
    }
};


#endif // WORK_FILE_CSV_H
