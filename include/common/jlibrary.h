#ifndef JLIBRARY_H
#define JLIBRARY_H

#ifdef Q_OS_WIN
#include <Windows.h>
#else //Linux,Mac
#include <dlfcn.h>
#endif

#include <string>
using std::string;

class JLibrary
{
public:
    JLibrary(const char * pszFileName);
    ~JLibrary();
    bool load();
    bool unload();
    bool isLoaded() const;
    void *  resolve(const char * pszProcName);
private:
    bool m_isloaded;
    void * m_handle;
    string m_filename;
};

#endif // JLIBRARY_H
