#pragma once

#include <string>
using std::string;

class IAutoJobItem;

class ISCWorkFlow
{
public:
    enum
    {
        IID_SC_WORKFLOW = 0x103,    //�Ӿ�����ִ�нӿ�
    };

    virtual ~ISCWorkFlow(){};

    /****************************************************
    *Function: ��ʼ������
    *Intput: NULL
    *Output: NULL
    *Return: �ɹ�����true ��ʧ�ܷ���false
    *Remark: ��ʼ���ڲ�������
    ******************************************************/
    virtual bool Init() = 0;

    /****************************************************
    *Function: ȥ��ʼ������
    *Intput: NULL
    *Output: NULL
    *Return: �ɹ�����true ��ʧ�ܷ���false
    *Remark: �ͷ���Դ��
    ******************************************************/
    virtual bool UnInit() = 0;

    /****************************************************
    *Function: ��ʼ��Job
    *Intput: jobName����������
    *Output: NULL
    *Return: �ɹ�����true ��ʧ�ܷ���false
    *Remark: ���̵�һ��ִ��֮ǰ������������������ӡ��޸ġ�ɾ��֮�����
    ******************************************************/
    virtual bool InitJob(const char *jobName) = 0;

    /****************************************************
    *Function: �ж�Job�Ƿ��ʼ��
    *Intput: jobName����������
    *Output: NULL
    *Return: �Ѿ���ʼ���򷵻�true�����򷵻�false
    ******************************************************/
    virtual bool IsJobInit(const char *jobName) = 0;

    /****************************************************
    *Function: ִ��Job
    *Intput: jobName����������
            strFailedItem: ʧ�ܵ������������ƣ��ɹ��Ļ�Ϊ��
            bFailedExit:�����������ʧ�����˳�����ִ�к��������������ִ��������������
    *Output: NULL
    *Return: �ɹ�����true ��ʧ�ܷ���false
    ******************************************************/
    virtual bool Execute(const char *jobName, string &strFailedItem, bool bFailedExit = true) = 0;
    
    /****************************************************
    *Function: ���ݴ���JobName->itemNameִ�ж�Ӧitem��
    *Intput: jobName����������
            szItem: item��
            winID: ����ID�� �ô�����������ڵ��Դ���
            pInnerParam��item���ܵ��ڲ����������ڵ��ԣ� ����ʱ��Ҫ����Save�ӿ�
    *Output: NULL
    *Return: �ɹ�����true ��ʧ�ܷ���false
    ******************************************************/
    virtual bool ExecuteItem(const char *jobName, const char *szItem, long winID, void *pInnerParam) = 0;
    
    /****************************************************
    *Function: ��ȡ��Ӧitem����ڲ����������ڽ����������
    *Intput: jobName����������
            szItem: item��
    *Output: pInnerParam��item��������ڲ����صĲ���
    *Return: �ɹ�����true ��ʧ�ܷ���false
    ******************************************************/
    virtual bool GetItemParam(const char *jobName, const char *szItem, void *pInnerParam) = 0;
    
    /****************************************************
    *Function: �����Ӧitem����ڲ�����
    *Intput: jobName����������
            szItem: item��
            pInnerParam��item���ܵ��ڲ�����
    *Output: NULL
    *Return: �ɹ�����true ��ʧ�ܷ���false
    ******************************************************/
    virtual bool SaveItemParam(const char *jobName, const char *szItem, void *pInnerParam) = 0;
    /****************************************************
    *Function: ��ȡ��������ִ�н��
    *Intput: jobName����������
            jobItemName����������
            pName����������
    *Output: pValue��������
    *Return: �ɹ�����true ��ʧ�ܷ���false
    ******************************************************/
    virtual bool GetItemResult(const char *jobName, const char *jobItemName, const char *pName, void **pValue) = 0;

    /****************************************************
    *Function: ��ȡ����ִ�н��
    *Intput: jobName����������
            pName����������
    *Output: pValue��������
    *Return: �ɹ�����true ��ʧ�ܷ���false
    *Remark����ȡ����ִ�н�����������ִ����ɣ��򷵻����һ��item�Ľ����
            ���򷵻�ִ�г����item�Ľ��������LOG ͼƬ��
    ******************************************************/
    virtual bool GetResult(const char *jobName, const char *pName, void **pValue) = 0;

    /****************************************************
    *Function: ��ʾ����ִ�н��
    *Intput: jobName����������
            jobItemName����������
            nWinID������ID
    *Output: NULL
    *Return: �ɹ�����true ��ʧ�ܷ���false
    ******************************************************/
    virtual bool DisplayResult(const char *jobName, const char *jobItemName, int nWinID) = 0;

    /****************************************************
    *Function: ��ʾ����ִ�н��
    *Intput: jobName����������
            nWinID������ID
    *Output: NULL
    *Return: �ɹ�����true ��ʧ�ܷ���false
    *Remark: ���ִ�гɹ�����ʾ���һ��Item�����������ʾִ��ʧ�ܵ�Item�Ľ��
    ******************************************************/
    virtual bool DisplayResult(const char *jobName, int nWinID) = 0;

    /****************************************************
    *Function: ��ȡ����·��
    *Intput: pJobName����������
            jobItemName����������
    *Output: NULL
    *Return: ���ز����ľ���·��
    ******************************************************/
    virtual string GetParamPath(const char *pJobName, const char *jobItemName) = 0;

    /****************************************************
    *Function: ��ȡIAutoJobItemָ��
    *Intput: pJobName����������
            jobItemName����������
            iType: JobItem����
    *Output: NULL
    *Return: ����IAutoJobItemָ��
    *Remark: ���iType���Զ���halcon���̣����������JobItem���򴴽�JobItem���������ͷ���NULL
    ******************************************************/
    virtual IAutoJobItem* GetJobItem(const char *pJobName, const char *jobItemName, const char *iType = NULL) = 0;

	/****************************************************
    *Function: ��ȡ����������
    *Intput: pJobName����������
            jobItemName����������
    *Output: NULL
    *Return: ���ع���������
    ******************************************************/
	virtual string GetFactoryObjectName(const char *pJobName, const char *jobItemName) = 0;
};
