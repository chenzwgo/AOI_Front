#include "commonapi.h"
#include "g12utils.h"
#include <QApplication>
#include "filevariable/ifilevariable.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "jtime.h"
#include <QDir>
#include "maindefine.h"
int UsSleep(int us)
{
    return usSleep(us);
}

unsigned long long getDiskFreeSpace(const unsigned short *driver) {

    LPCWSTR lpcwstrDriver = (LPCWSTR)driver;

    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;

    if (!GetDiskFreeSpaceEx(lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes))
    {
        return 0;
    }
    return (unsigned long long)liTotalFreeBytes.QuadPart / (1024 * 1024 * 1024);
}


QString getSaveResultFilePath()
{
	QString strDir = QApplication::applicationDirPath();
	//
	IFileVariable *pFile = getDVCfgFileVariable();
	if (NULL != pFile)
	{
		QString path;
		int iflag = pFile->getValue(G12_DATA_UI_LOG_FLAGS).toInt();
		if (iflag & 0x02)
		{
			path = pFile->getValue(G12_DATA_PRODUCTRESULT).toCString().c_str();
		}
		strDir = path.length() > 0 ? path : QApplication::applicationDirPath();
	}
	//
	strDir += "/ProductResult";

	QDir dir;
	if (!dir.exists(strDir))
	{
		dir.mkdir(strDir);
	};
	return strDir;
}
QString getCurrentLocalDateTimeStrPath()
{
	JTime curTime = JTime::CurrentLocalDateTime();
	QString str = QString("%1-%2-%3").arg(curTime.year()).arg(curTime.month(), 2, 10, QChar('0')).arg(curTime.day(), 2, 10, QChar('0'));
	return str;
}
/***
* 计算前一天日期 
* @param y :年，负数表示公元前
* @param m :月
* @param d :日
* @return
*/
 QString CalculatorYesterday(int y, int m, int d)
 {
	int am[] = { 12,1,2,3,4,5,6,7,8,9,10,11,12 };          //month am[1] 一月
	int ad[] = { 31,31,28,31,30,31,30,31,31,30,31,30,31 };//day   ad[1] 一月，31天

	if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {              // 判断是否是闰年
		ad[2] = 29;
	}

	if (m < 1 || m>12 || d < 1 || (d > ad[m]))               //判断输入日期是否合法
	{
		return "Input error:Invalid date";
	}
		
	if (m == 1 && d == 1)
	{
		y = y - 1;    //年
	}
		
	if (d == 1) {
		d = ad[m - 1];
		m = am[m - 1];//月
	}
	else
	{
		d = d - 1;   //日
	}
		
	QString str = QString("%1-%2-%3").arg(y).arg(m, 2, 10, QChar('0')).arg(d, 2, 10, QChar('0'));

	return str;
}
 QString getCurrentProducsTimeStrPath()
 {
	 QString strPath = getCurrentLocalDateTimeStrPath();
	 IFileVariable *pFile = getCameraSetCfgFileVariable();
	 int iSartTime = 8;
	 if (pFile)
	 {
		 iSartTime = pFile->getValue(WORK_MODE_AM_STRAT_TIME).toInt(8);;//
		 JTime curTime = JTime::CurrentLocalDateTime();
		 if (curTime.hour()<iSartTime)
		 {
			 strPath = CalculatorYesterday(curTime.year(), curTime.month(), curTime.day());
		 }
	 }
	 return strPath;
 }

 //白夜班判断
 int getCurrentWorktimes()
 {
     int iTimes = 2; //夜班
	 IFileVariable *pFile = getCameraSetCfgFileVariable();
	 int iSartTime = 8;
	 if (pFile)
	 {
		 iSartTime = pFile->getValue(WORK_MODE_AM_STRAT_TIME).toInt(8);
	 }
	 JTime curTime = JTime::CurrentLocalDateTime();
	 if (curTime.hour() >= iSartTime && curTime.hour() < (iSartTime + 12))
	 {
		 iTimes = 1 ;	//白班
	 }
	 return iTimes;	
	 
 }
 QString getCurrenTimeDayOrNightPath()
 {
	 QString strDir = getSaveResultFilePath();

	 QString strSavePath = QString("%1/%2").arg(strDir).arg(getCurrentProducsTimeStrPath());
	 QDir dir2(strSavePath);
	 if (!dir2.exists(strSavePath))
	 {
		 dir2.mkdir(strSavePath);
	 }
	 strSavePath += "/" + QString::fromLocal8Bit(szDayPath[getCurrentWorktimes()]);
	 QDir dir3(strSavePath);
	 if (!dir3.exists(strSavePath))
	 {
		 dir3.mkdir(strSavePath);
	 }
	 return strSavePath;
 }

 QString getSaveResultFilePathNew()
 {
     QString strDir = QApplication::applicationDirPath();
     //
     IFileVariable *pFile = getDVCfgFileVariable();
     if (NULL != pFile)
     {
         QString path;
         int iflag = pFile->getValue(G12_DATA_UI_LOG_FLAGS).toInt();
         if (iflag & 0x02)
         {
             path = pFile->getValue(G12_DATA_PRODUCTRESULT).toCString().c_str();
         }
         strDir = path.length() > 0 ? path : QApplication::applicationDirPath();
     }
     //
     strDir += "/ProductResultNew";

     QDir dir;
     if (!dir.exists(strDir))
     {
         dir.mkdir(strDir);
     };
     return strDir;
 }
 QString getResByResultFlag(int ResultFlag)
 {
	 QString str = "RI";
	 switch ((productResFlag)ResultFlag)
	 {
	 case res_OK:
	 case res_OK2:
		 str = "OK";
		 break;
	 case res_Ng:
		 str = "NG";
		 break;
	 default:
		 break;
	 }
	 return str;
 }
 unsigned int  getCurrentDaySn()
 {
	 IFileVariable *pFileVar = getGlobalSNFileVariable();
	 if (pFileVar != NULL)
	 {
		 QString strDate = getCurrentProducsTimeStrPath();
		 QString m_strMarkDay = strDate.remove(QChar('-'), Qt::CaseInsensitive);
		 return pFileVar->getValue("ProductSN", m_strMarkDay.toStdString(), 0).toUInt();
	 }
 }