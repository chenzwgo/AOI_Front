#ifndef _I_AUTO_JOB_ITEM_H_
#define _I_AUTO_JOB_ITEM_H_
//��������ӿ�
class IAutoJobItem 
{
public:
    virtual ~IAutoJobItem(){};

    /****************************************************
    *Function: ����������������
    *Intput: pszObjName��������������
    *Output: NULL
    *Return: NULL
    *Remark: ���������֮��ʹ�ã��������
    ***************************************************** */
    virtual void SetName(const char *pszObjName) = 0;

    /****************************************************
    *Function: �������������ڲ������ļ�·��
    *Intput: pszPath�����������ڲ������ļ�·��
    *Output: NULL
    *Return: NULL
    *Remark: ��ȡ�������ڲ�����ʱʹ��
    ***************************************************** */
    virtual void SetPath(const char *pszPath) = 0;

    /****************************************************
    *Function: �޸��ұ������������ڲ�����
    *Intput: NULL
    *Output: NULL
    *Return: �޸��Ƿ�ɹ�
    *Remark: ��������UI�����ʾ�޸Ľ���
    ***************************************************** */
    virtual bool SaveInnerParam(void *pInnerParam) = 0; 

    /****************************************************
    *Function: ��ȡ���������ڲ�����
    *Intput: NULL
    *Output: pInnerParam�� ��������
    *Return: �Ƿ�ɹ�
    *Remark: 
    ***************************************************** */
    virtual bool GetInnerParam(void *pInnerParam) = 0; 

    /****************************************************
    *Function: ����ϴ�ִ�н������ʼ���ڲ�������
    *Intput: NULL
    *Output: NULL
    *Return: NULL
    *Remark: ���ò���֮ǰ����
    ***************************************************** */
    virtual void Clear() = 0;

    /****************************************************
    *Function: �������������֮����������
    *Intput: pName���������ƣ����磺image�� line1��
             pData������ָ��
    *Output: NULL
    *Return: ���������Ƿ�ɹ�
    *Remark: ���Դ���֮ǰ������������
    ***************************************************** */
    virtual bool SetInputParam(const char *pName, void *pData) = 0; 

    /****************************************************
    *Function: ���¼������������ڲ�����
    *Intput: NULL
    *Output: NULL
    *Return: ���������Ƿ�ɹ�
    *Remark: ͨ��SetInnerParam()����ʵ���޸ģ�·��������"./Process/��������/��������/"Ŀ¼��
    ***************************************************** */
    virtual bool ReloadInnerParam() = 0;  

    /****************************************************
    *Function: ִ�����������㷨
    *Intput: NULL
    *Output: NULL
    *Return: ����ִ���Ƿ�ɹ�
    *Remark: ���ִ��ʧ�ܣ���Ҫ�������Ƿ����ã�ִ�й����Ƿ��쳣��
    ***************************************************** */
    virtual bool Execute(const void *pInParam = 0, long halconWID = -1) = 0; 

    /****************************************************
    *Function: ��ȡִ�н��
    *Intput: name���������
    *Output: pResult�� �������ָ��
    *Return: ���ػ�ȡ�Ƿ�ɹ�
    ***************************************************** */
    virtual bool GetResult(const char *name, void **pResult) = 0; //�޸�

    /****************************************************
    *Function: ��ʾִ�н��
    *Intput: nWinID������ID��halconͨ��open_window�򿪵Ĵ���ID������3600
    *Output: NULL
    *Return: ������ʾ�Ƿ�ɹ�
    ***************************************************** */
    virtual bool DisplayResult(int nWinID) = 0;  //�޸�


	    /****************************************************
    *Function: ���ش�����Ϣ
    *Intput: ��
    *Output: NULL
    *Return: ���ش�����
    ***************************************************** */
	virtual int GetLastError() = 0;
};

#endif