#ifndef JFILE_H
#define JFILE_H

#include <string>
using std::string;

#include <stdio.h>

class JFile //not thread safe
{
public:
    enum Permissions {
        ReadOwner = 0x4000, WriteOwner = 0x2000, ExeOwner = 0x1000,
        ReadUser  = 0x0400, WriteUser  = 0x0200, ExeUser  = 0x0100,
        ReadGroup = 0x0040, WriteGroup = 0x0020, ExeGroup = 0x0010,
        ReadOther = 0x0004, WriteOther = 0x0002, ExeOther = 0x0001
                                                        };

    JFile();
    JFile(const string & filepath); // use  AbsolutePath
    ~JFile();
    static bool exists(const string & filepath);
    bool exists();
    void setFileName(const string & filepath); // use  AbsolutePath
    bool setPermissions ( unsigned int  permissions );
    static bool setPermissions ( const string & newfile, unsigned int permissions );
    bool open (const char * mode); //the mode is as same as fopen()
    bool isOpen();
    bool close();
    size_t read(void * buf, size_t max_size); //read by bytes
    size_t write(void * buf, size_t max_size); //write by bytes
    bool remove();
    static bool copyFile(const char *dst, const char *src, int iLength); //iLength buffer size
    int readend();
    size_t size();
    int   setCurPos(size_t pos);
private:
    string m_filepath;// use  AbsolutePath
    FILE * m_filehandle;
};

#endif // JFILE_H
