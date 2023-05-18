#ifndef ACTIONPARSEBUF_DEFINE_H
#define ACTIONPARSEBUF_DEFINE_H

struct StBuf
{
    StBuf(int nType, const string& strName)
    {
        type = nType;
        name = strName;
        memset(value, 0, sizeof(value));
    }

    int type;    // 0点位 1变量 2不接收
    string name; // 点位名称或变量名称
    char value[128]; // 值
};

#endif