#include <fstream>
#include <sstream>
#include <iomanip>
#include "stringcommon.h"
#include "csvparser.h"

CCSVParser::CCSVParser()
{

}

CCSVParser::~CCSVParser(void)
{
    _originalFile.clear();
}


bool CCSVParser::setFileName( const std::string strFileName )
{
    _file = strFileName;
    return true;
}


bool CCSVParser::readFileContent()
{
    std::string line;
    std::ifstream ifile(_file.c_str());
    if (ifile.is_open())
    {
        _originalFile.clear();
        while (ifile.good())
        {
            getline(ifile, line);
            if (line != "")
                _originalFile.push_back(line);
        }
        ifile.close();
    }
    return true;
}


bool CCSVParser::writeHearder( const std::string& strHeader )
{
    std::ofstream f;
    f.open(_file.c_str(), std::ios::out | std::ios::trunc);
    f << strHeader << std::endl;
    f.close();
    return true;
}

std::vector<std::string> CCSVParser::getHeader(void) 
{
    return _header;
}

bool CCSVParser::deleteRow(unsigned int pos)
{
    if (pos < _originalFile.size())
    {
        _originalFile.erase(_originalFile.begin() + pos);
        return true;
    }
    return false;
}

bool CCSVParser::addRow(unsigned int pos, const std::string& strRowContent)
{
    if (pos <= _originalFile.size())
    {
        _originalFile.insert(_originalFile.begin() + pos,strRowContent);
        return true;
    }
    return false;
}

bool CCSVParser::addLastRow(const std::string& strRowContent)
{
    unsigned int pos =  _originalFile.size();  
    if (pos >0)
    {
        _originalFile.insert(_originalFile.begin() + pos,strRowContent);
        return true;
    }
    return false;
}


unsigned int CCSVParser::rowCount(void) 
{
    return _originalFile.size();
}

unsigned int CCSVParser::columnCount(void) 
{
    return _header.size();
}

const std::string& CCSVParser::getRow(unsigned int row) 
{
    if (row < _originalFile.size())
    {
        return _originalFile[row];
    }
    return "";         
}

const std::string& CCSVParser::getLastRow() 
{
    unsigned int row = _originalFile.size();
    if (row >0)
    {
        return _originalFile[row-1];
    }
    return "";         
}

const std::string CCSVParser::getItemElement( unsigned int iRow,unsigned int iCol )
{
    if (iRow > _originalFile.size())
    {
        return "";
    }
    std::string strRow = _originalFile[iRow-1];
    std::vector<std::string> collist = SString::split(strRow,",");
    if (iCol > collist.size())
    {
        return "";
    }
    return collist[iCol -1];
}

bool CCSVParser::setItemElement( unsigned int iRow,unsigned int iCol,const std::string& strItem )
{
    if (iRow > _originalFile.size())
    {
        return "";
    }
    std::string strRow = _originalFile[iRow-1];
    std::vector<std::string> collist = SString::split(strRow,",");
    if (iCol > collist.size())
    {
        return "";
    }
    collist[iCol] = strItem;
    int count = collist.size();
    std::stringstream newstr;
    for (int i=0; i<count; i++)
    {
        newstr<<collist[i];
        if (i != count-1 )
        {
            newstr<<",";
        }
    }
    _originalFile[iRow-1] = newstr.str();
    return true;
}

bool CCSVParser::synFile()
{
    std::ofstream f;
    f.open(_file.c_str(), std::ios::out | std::ios::trunc);
    for (auto it = _originalFile.begin(); it != _originalFile.end(); it++)
        f << *it << std::endl;
    f.close();
    return true;
}