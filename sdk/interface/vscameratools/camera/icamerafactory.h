#pragma once

#include <vector>
#include <string>
using namespace std;



class ICamera;
class ILineCamera;
class CProxyCamera;

struct LINE_CAM_PARAM;

typedef struct _STCameraParam
{
    unsigned int index;
    unsigned int exposure;
    unsigned int gain;
    _STCameraParam()
    {
        index = -1;
        exposure = 100;
        gain = 0;
    };
}STCameraParam;

typedef struct _STCameraInfo
{
    CProxyCamera *pProxyCamera;
    bool bInit;
    vector<ICamera *> vecCamera;
    _STCameraInfo()
    {
        pProxyCamera = NULL;
        bInit = false;
    };
}STCameraInfo;

class ICameraFactory
{
public:
    enum
    {
        IID_CAMERAFACTORY = 0x100,//
    };
    virtual ~ICameraFactory(){}
    // ��ȡ����б�
    virtual bool GetCamList(vector<string> &vecGUID,unsigned long MaxCnt) = 0;
    // �������-ͬһ��id���Դ����������
    // indexΪ���������������Χ��0-64,���������������򷵻����еģ����û���򴴽�
    // bCreateNewCameraObj����true,���Ǵ���һ���µ��������
    // ������û�г�ʼ�������ȳ�ʼ��
    virtual ICamera *CreateCamera(const char *pszGUID, unsigned int index) = 0;
protected:
private:
};

class ILineCameraFactory
{
public:
    enum
    {
        IID_LINE_CAMERAFACTORY = 0x101,//
    };
    virtual ~ILineCameraFactory(){}
    // ��ȡ����б�
    virtual bool GetCamList(vector<string> &vecGUID,unsigned long MaxCnt) = 0;
    // �������-ͬһ��id���Դ����������
    // indexΪ���������������Χ��0-64,���������������򷵻����еģ����û���򴴽�
    // bCreateNewCameraObj����true,���Ǵ���һ���µ��������
    // ������û�г�ʼ�������ȳ�ʼ��
    virtual ILineCamera *CreateCamera(LINE_CAM_PARAM *param) = 0;

    virtual bool isCameraInitFinished() = 0;
protected:
private:
};