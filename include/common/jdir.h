
#ifndef JDIR_H
#define JDIR_H

#include <string>
using std::string;

#ifdef Q_OS_WIN
	#ifndef MAX_PATH
	#define MAX_PATH 260
	#endif
#else	//posix
	#ifndef MAX_PATH
	#include <sys/param.h>
	#define MAX_PATH MAXPATHLEN
	#endif
#endif

class JDir //not thread safe
{
public:
    JDir();//default is the current path of process
    JDir(const string & dirPath);
    ~JDir();
    bool mkdir(const string & dirPath); //not support multi dir ,if not using AbsolutePath, the subdir will be create under  CurrentPath
    bool exists(const string & dirPath);//if not using AbsolutePath, the subdir will be tested under  CurrentPath
    bool exists();
    static string toNativeSeparators(const string & dirPath);
    static char separator();
    string getCurrentPath();
private:
    bool isAbsolutePath(const string & dirPath);
private:
    string m_dirpath; //should be AbsolutePath
};

#endif // JDIR_H
