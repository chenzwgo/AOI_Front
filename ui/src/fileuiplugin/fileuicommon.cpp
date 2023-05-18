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

		//������Ŀ¼
		if (fileInfo.isDir())
		{
			//�ݹ���ÿ���
			if (!qCopyDirectory(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName()), true))
				return false;
		}
		//�������ļ�
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
			//����ļ��Ѵ��ڣ��ж��Ƿ񸲸�
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

		//�ж��ļ��Ƿ����
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
		//����ļ��Ѷ�ʧ�����¼����
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
