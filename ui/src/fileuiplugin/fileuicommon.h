#ifndef FILEUICOMMON_H
#define FILEUICOMMON_H

#include <QObject>

#define PROGRAM_MODE "./programs/program_model"
#define PROGRAM_PATH "./programs/"

struct StProgramInfo
{
	QString strProgramName;
	QString strProgramDesc;
	StProgramInfo() {}
	StProgramInfo(const QString &name, const QString & desc)
	{
		strProgramName = name;
		strProgramDesc = desc;
	}
};

class FileUiCommon
{
public:
    FileUiCommon();
public:
	//拷贝文件夹
	static bool qCopyDirectory(const QString& fromDir, const QString& toDir, bool bCoverIfFileExists);
	//判断是否有文件丢失
	static bool isLostFile(const QString& srcDir, const QString& dstDir, QStringList& lostFileList);
};

#endif // FILEUICOMMON_H