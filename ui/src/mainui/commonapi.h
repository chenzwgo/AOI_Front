#ifndef COMMONAPI_H
#define COMMONAPI_H

#include <Windows.h>
#include <QString>

unsigned long long getDiskFreeSpace(const unsigned short *driver);

int UsSleep(int us);//΢����ʱ����ʵ��

QString getSaveResultFilePath();
//ͳһ���ڸ�ʽ 2020-07-20  ��yyyy-mm-dd
QString getCurrentLocalDateTimeStrPath();
QString CalculatorYesterday(int y, int m, int d);
/*ͳһ��ǰ����������ڣ�
    ��װ� �ϰ�ʱ�䵽�°�ʱ�� Ϊͬһ�죨����8���ϰൽ����20�� Ϊͬһ�� 2020-08-05����
	ҹ��Ҳ ���                       ������20���ϰൽ�ڶ���8���°࣬Ϊ��װ�ͬһ�� 2020-08-05��
*/
QString getCurrentProducsTimeStrPath();
int getCurrentWorktimes();
/*��ȡ��ǰ�������Ŀ¼*/
QString getCurrenTimeDayOrNightPath();
 QString getSaveResultFilePathNew();
 QString getResByResultFlag(int ResultFlag);
unsigned int  getCurrentDaySn();
#endif // COMMONAPI_H
