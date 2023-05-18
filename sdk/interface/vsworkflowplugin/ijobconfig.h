#ifndef _IJobConfig_h_
#define _IJobConfig_h_

#include "processbase.h"

class IJobConfig
{
public:
    enum
    {
        IID_JOB_CONFIG = 0x101,    //���������ý����ӿ�
    };

    virtual ~IJobConfig(){};

    ///****************************************************
    //*Function: ��ȡ��������
    //*Intput: iType: ��������
    //*Output: NULL
    //*Return: �����ַ���������
    //******************************************************/
    //virtual string GetJobType(int iType) = 0;

    ///****************************************************
    //*Function: ��ȡ��������
    //*Intput: pszType:  
    //*Output: NULL
    //*Return: ������������
    //******************************************************/
    //virtual int GetJobType(const char *pszType) = 0;

    /****************************************************
    *Function: ��ѯ����������Ϣ
    *Intput: NULL
    *Output: NULL
    *Return: ���������б�
    ******************************************************/
    virtual list<StJob*> *QueryJobs(void) = 0;

    /****************************************************
    *Function: �������̣����ڴ�
    *Intput: szJobName����������
    *Output: job������
    *Return: �ҵ��򷵻�true�����򷵻�false
    ******************************************************/
    virtual bool QueryJob(const char * szJobName, StJob **job) = 0;

    /****************************************************
    *Function: ��������������ڴ�
    *Intput: pszJobName���������� pszJobItemName ������������
    *Output: StJobItem����������
    *Return: �ҵ��򷵻�true�����򷵻�false
    ******************************************************/
    virtual bool QueryJobItem(const char * pszJobName, const char * pszJobItemName, StJobItem **jobItem) = 0;

    /****************************************************
    *Function: ��ȡ����job,�������ļ�
    *Return:  ���������б�
    ******************************************************/
    virtual list<StJob*> *ReadAllJobsFromCfg() = 0;
    
    /****************************************************
    *Function: ����˵��job��д�������ļ�
    *Return: �ɹ��򷵻�true�����򷵻�false
    ******************************************************/
    virtual bool SaveAllJobsToCfg() = 0;

    
    /****************************************************
    *Function: ������,ֱ����Ч��д���ļ�
    *Input:  pszOldJobName: ����������
    *Input:  szJobName����������
    *Output: vecParam�����̳�����Ϣ
    *Return: �ҵ��򷵻�true�����򷵻�false
    ******************************************************/
    virtual bool RenameJob(const char *pszOldJobName, const char *szJobName) = 0;

    /****************************************************
    *Function: ��ѯ���̳���(���һ��item�ĳ���)
    *Intput: szJobName����������
    *Output: vecParam�����̳�����Ϣ
    *Return: �ҵ��򷵻�true�����򷵻�false
    ******************************************************/
    // û�õ� scj
    //virtual bool QueryJobOutputParam(const char * szJobName, vector<ProcessParam>& vecParam) = 0;

    /****************************************************
    *Function: �жϹ������Ƿ����
    *Intput: szJobName������������
    *Output: NULL
    *Return: �����򷵻�true�����򷵻�false
    ******************************************************/
    virtual bool IsJobExist(const char * szJobName) = 0;

    /****************************************************
    *Function: ��ȡ����������
    *Intput: NULL
    *Output: NULL
    *Return: ���ع���������
    ******************************************************/
    virtual int GetJobCount() = 0;


    /****************************************************
    *Function: �����µĹ�����
    *Intput: NULL
    *Output: NULL
    *Return: ���ع���������
    ******************************************************/
    virtual bool InsertJob(const StJob &job) = 0;

    /****************************************************
    *Function: �޸�����
    *Intput: szJobName����������
             job���޸�֮�������
    *Output: NULL
    *Return: �����Ƿ��޸ĳɹ�
    ******************************************************/
    virtual bool UpdateJob(const char *szJobName, const StJob &job) = 0;

    /****************************************************
    *Function: ɾ������
    *Intput: szJobName����������
    *Output: NULL
    *Return: �����Ƿ�ɾ���ɹ�
    ******************************************************/
    virtual bool DeleteJob(const char *szJobName) = 0;

     /****************************************************
    *Function: �����µĹ�����
    *Intput: szJobName����������
    *Intput: szPreItemName����һ��������֣�=NULL ��ʾ���뵽������
    *Intput: jobItem��Ҫ�������
    *Output: NULL
    *Return: �����Ƿ����ɹ�
    ******************************************************/
    virtual bool InsertJobItem(const char *szJobName, const char *szPreItemName, const StJobItem &jobItem) = 0;

    /****************************************************
    *Function: ɾ��������
    *Intput: szJobName����������
    *Intput: jobItem��Ҫɾ������
    *Output: NULL
    ******************************************************/
    virtual void DeleteJobItem(const char *szJobName, const char *pszItemName) = 0;

    /****************************************************
    *Function: �ƶ������
    *Intput: szJobName���������ƣ�
             szItemName������������
             bUp�����ƻ�������
    *Output: NULL
    *Return: �����Ƿ��ƶ��ɹ�
    ******************************************************/
    bool MoveJobItem(const char *szJobName, const char *szItemName, bool bUp);

    //�༭������
    /****************************************************
    *Function: �޸�����
    *Intput: szJobName����������
             szItemName������������
             jobItem���޸�֮��Ĺ�����
    *Output: NULL
    *Return: �����Ƿ��޸ĳɹ�
    ******************************************************/
    bool UpdateJobItem(const char *szJobName, const char *szItemName, const StJobItem &jobItem);

    /****************************************************
    *Function: �ƶ����̣�
    *Intput: szJobName���������ƣ�
            bUp�����ƻ�������
    *Output: NULL
    *Return: �����Ƿ��ƶ��ɹ�
    ******************************************************/
    virtual bool MoveJob(const char *szJobName, bool bUp) = 0;

    /****************************************************
    *Function: �������̣�
    *Intput: pszJobPath������·����
    *Output: NULL
    *Return: �����Ƿ���سɹ�
    ******************************************************/
    virtual bool ReadConfig(const char *pszJobPath) = 0;
};

#endif // _IJobConfig_h_