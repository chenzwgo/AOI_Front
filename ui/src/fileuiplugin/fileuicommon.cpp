#include "fileuicommon.h"
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>

FileUiCommon::FileUiCommon()
{

}

bool FileUiCommon::qCopyDirectory(const QString& fromDir, const QString& toDir, bool bCoverIfFileExists)
{
	QDir formDir_(fromDir);
	QDir toDir_(toDir);

	if (!toDir_.exists())
	{
		if (!toDir_.mkdir(toDir_.absolutePath()))
			return false;
	}

	QFileInfoList fileInfoList = formDir_.entryInfoList();
	foreach(QFileInfo fileInfo, fileInfoList)
	{
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		//拷贝子目录
		if (fileInfo.isDir())
		{
			//递归调用拷贝
			if (!qCopyDirectory(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName()), true))
				return false;
		}
		//拷贝子文件
		else
		{
			/*if(bCoverIfFileExists && toDir_.exists(fileInfo.fileName()))
			{
			toDir_.remove(fileInfo.fileName());
			}
			if(!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
			{
			return false;
			}*/
			//如果文件已存在，判断是否覆盖
			if (toDir_.exists(fileInfo.fileName()))
			{
				if (bCoverIfFileExists)
				{
					toDir_.remove(fileInfo.fileName());
					if (!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
					{
						return false;
					}
				}
			}
			else
			{
				if (!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool FileUiCommon::isLostFile(const QString& srcDir, const QString& dstDir, QStringList& lostFileList)
{
	QDir srcDir_(srcDir);
	QDir dstDir_(dstDir);
	if (!srcDir_.exists() || !dstDir_.exists())
	{
		return false;
	}

	QFileInfoList srcFileInfoList = srcDir_.entryInfoList();
	QFileInfoList dstFileInfoList = dstDir_.entryInfoList();
	foreach(QFileInfo srcFileInfo, srcFileInfoList)
	{
		if (srcFileInfo.fileName() == "." || srcFileInfo.fileName() == "..")
			continue;

		//判断文件是否存在
		QString strSrcFileName = srcFileInfo.fileName();
		bool bIsLost = true;
		foreach(QFileInfo dstFileInfo, dstFileInfoList)
		{
			if (strSrcFileName == dstFileInfo.fileName())
			{
				bIsLost = false;
				break;
			}
		}
		//如果文件已丢失，则记录下来
		if (bIsLost)
		{
			lostFileList.push_back(srcFileInfo.filePath());
		}
		else if (srcFileInfo.isDir())
		{
			if (!isLostFile(srcFileInfo.filePath(), dstDir_.filePath(srcFileInfo.fileName()), lostFileList))
				return false;
		}
	}
	return true;
}
