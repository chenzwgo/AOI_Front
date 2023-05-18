#ifndef JTIME_H
#define JTIME_H

#include <sys/timeb.h>
#include <time.h>

#define uint64 unsigned long long
class JTime
{
public:
    #define uint64 unsigned long long
    JTime(int year=0, int month=0, int day=0, int h=0, int m=0, int s=0, int ms = 0);
    ~JTime();
    static uint64 CurrentMillisecond(); //return msec from 1970.01.01 00::00::00:00
    static JTime CurrentUTCDateTime();
    static JTime CurrentLocalDateTime();
    int year() const;
    int month() const;
    int day() const;
    int hour() const;
    int minute() const;
    int second() const;
    int msec() const;
    time_t toTime_t();
private:
    bool isValidDate(int year, int month, int day);
    bool isValidTime(int h, int m, int s, int ms);
    bool setDate(int year, int month, int day);
    bool setHMS(int h, int m, int s, int ms = 0);
    bool isLeapYear(int year);
private:
    int m_year;
    int m_mon;
    int m_day;
    int m_hour;
    int m_min;
    int m_sec;
    int m_msec;
};

#endif // JTIME_H
