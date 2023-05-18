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
	//�����ļ���
	static bool qCopyDirectory(const QString& fromDir, const QString& toDir, bool bCoverIfFileExists);
	//�ж��Ƿ����ļ���ʧ
	static bool isLostFile(const QString& srcDir, const QString& dstDir, QStringList& lostFileList);
};

#endif // FILEUICOMMON_H