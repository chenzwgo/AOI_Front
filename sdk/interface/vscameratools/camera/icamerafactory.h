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
    // 获取相机列表
    virtual bool GetCamList(vector<string> &vecGUID,unsigned long MaxCnt) = 0;
    // 创建相机-同一个id可以创建多个对象
    // index为相机对象索引，范围在0-64,如果已有相机对象，则返回已有的，如果没有则创建
    // bCreateNewCameraObj等于true,总是创建一个新的相机对象
    // 如果相机没有初始化，则先初始化
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
    // 获取相机列表
    virtual bool GetCamList(vector<string> &vecGUID,unsigned long MaxCnt) = 0;
    // 创建相机-同一个id可以创建多个对象
    // index为相机对象索引，范围在0-64,如果已有相机对象，则返回已有的，如果没有则创建
    // bCreateNewCameraObj等于true,总是创建一个新的相机对象
    // 如果相机没有初始化，则先初始化
    virtual ILineCamera *CreateCamera(LINE_CAM_PARAM *param) = 0;

    virtual bool isCameraInitFinished() = 0;
protected:
private:
};