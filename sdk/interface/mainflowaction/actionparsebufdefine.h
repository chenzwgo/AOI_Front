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

    int type;    // 0��λ 1���� 2������
    string name; // ��λ���ƻ��������
    char value[128]; // ֵ
};

#endif