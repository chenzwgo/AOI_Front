#ifndef COMMONAPI_H
#define COMMONAPI_H

#include <Windows.h>
#include <QString>

unsigned long long getDiskFreeSpace(const unsigned short *driver);

int UsSleep(int us);//微秒延时函数实现

QString getSaveResultFilePath();
//统一日期格式 2020-07-20  年yyyy-mm-dd
QString getCurrentLocalDateTimeStrPath();
QString CalculatorYesterday(int y, int m, int d);
/*统一当前生产班次日期：
    如白班 上班时间到下班时间 为同一天（例：8点上班到晚上20点 为同一天 2020-08-05），
	夜班也 如此                       （例：20点上班到第二天8点下班，为与白班同一天 2020-08-05）
*/
QString getCurrentProducsTimeStrPath();
int getCurrentWorktimes();
/*获取当前生产班次目录*/
QString getCurrenTimeDayOrNightPath();
 QString getSaveResultFilePathNew();
 QString getResByResultFlag(int ResultFlag);
unsigned int  getCurrentDaySn();
#endif // COMMONAPI_H
