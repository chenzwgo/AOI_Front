#ifndef _IPROCESSLIBCONFIG_H_
#define _IPROCESSLIBCONFIG_H_

#include "ProcessBase.h"

#include <list>
using std::list;

//�������ͽ���
class IProcessLibConfig
{
public:
    enum
    {
        IID_PROCESS_LIB_CONFIG = 0x102,    //������������ļ������ӿ�
    };

    virtual ~IProcessLibConfig() {};

    /****************************************************
    *Function: ��ȡ��������
    *Intput: NULL
    *Output: NULL
    *Return: ���������б�
    ******************************************************/
    virtual list<ProcessSet*> *GetProcessInfo() = 0;

    /****************************************************
    *Function: ��ȡ�����б�
    *Intput: szMethod����������
    *Output: NULL
    *Return: ���ز����б�
    ******************************************************/
    virtual const vector<ProcessParam> *GetProcessParams(const char *szMethod) = 0;

    /****************************************************
    *Function: ��ȡ��������������
    *Intput: szMethod����������
            szParamName:��������
            iParamType����Ρ�����
    *Output: NULL
    *Return: ���ز�����������
    ******************************************************/
    virtual string GetParamDataType(const char *szMethod, const char *szParamName, int iParamType) = 0;


    virtual string GetMethodUIGuidName(const char *szMethod) = 0;
};

#endif // _IPROCESSLIBCONFIG_H_