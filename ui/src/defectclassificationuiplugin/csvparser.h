#ifndef    _CSVPARSER_H_
#define    _CSVPARSER_H_

#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <sstream>


class CCSVParser
{
    public:
        CCSVParser();
        ~CCSVParser(void);

    public:
        bool setFileName(const std::string strFileName);
        bool readFileContent();

        bool writeHearder(const std::string& strHeader);
        std::vector<std::string> getHeader(void) ;

        bool deleteRow(unsigned int row);
        bool addRow(unsigned int pos, const std::string& strRowContent);
        const std::string &getRow(unsigned int row) ;
        const std::string &getLastRow() ;
        bool addLastRow(const std::string& strRowContent);

        unsigned int rowCount(void) ;
        unsigned int columnCount(void) ;
        const std::string getItemElement(unsigned int iRow,unsigned int iCol) ;
        bool setItemElement(unsigned int iRow,unsigned int iCol,const std::string& strItem) ;

        bool synFile();

    private:
        std::string _file;
        char _sep;
        std::vector<std::string> _originalFile;
        std::vector<std::string> _header;

};


#endif /*!_CSVPARSER_HPP_*/
