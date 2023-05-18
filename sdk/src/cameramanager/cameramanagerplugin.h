#ifndef _CAMERAMANAGERPLUGIN_PLUGIN_H_
#define _CAMERAMANAGERPLUGIN_PLUGIN_H_

#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "jmutex.h"

#include <string>
#include <map>
using namespace std;

class CCameraManagerPlugin : public ICameraFactoryManage
{
public:
    CCameraManagerPlugin(void);
    ~CCameraManagerPlugin(void);
    
    static CCameraManagerPlugin *getInstance();
    static void ReleaseInstance();

    //virtual ICameraFactory* GetCameraFactory(int iCamType);
    virtual void EnumCameraDesc(vector<string> &vecCamDesc);
    virtual string GetCamDesc(int iCamType);
    virtual int GetCamType(const string &strCamDesc);
    
    //通过ID， 相机类型获取相机实例， 类型不确定则只用ID匹配
    virtual ICamera *GetCameraInstance(string id, int iCamType = UNKNOWN_CAM);
    virtual string GetCameraNameWithGuid(string ID);  //通过别名获取相机ID
    /*获取枚举到的所有类型相机的GUID
    * 输出 ： 相机GUID列表
    * 返回 ： 列表个数*/
    virtual int GetCameraNameList(vector<string> &vecName, int iCamType = UNKNOWN_CAM);
    //获取枚举到的有效相机
    virtual int GetValidCameraList(map<string, ICamera *> &mapCamera);
    virtual int GetLastError() {return m_lastError;};
    void Init();
    void UnInit();

    virtual bool GetCameraInfo(string strName, VS_CAMERA_INFO &info);  //读取相机参数
    virtual bool SetCameraInParam(VS_CAMERA_INFO &param);//设置相机参数
    virtual bool UpdateCameraName(VS_CAMERA_INFO &info, string strOldName);//更新相机名称
    virtual bool CreateCameraConfig(string strName, VS_CAMERA_INFO &info);//创建相机
    virtual bool DeleteCameraConfig(string strName);//删除相机

    virtual bool CameraAcqImage(const char *szJobName, const char *szItemName, void *param = NULL);
    //刷新连接的相机列表
    virtual bool RefreshValidCameraList();

private:
    //读配置文件
    bool ReadConfig(void);
    //写配置文件
    bool SaveConfig();
private:
    //vector<ICameraFactory*> m_vecCameraFactory;
    static CCameraManagerPlugin *m_pInstance;
    int  m_lastError;
    map<string, ICamera *> m_camList;    //(SN, ICamera) 
    map<string, VS_CAMERA_INFO> m_camConfigList;  //(CameraName, Config)
    JMutex    m_lock;
};


#endif _CAMERAMANAGERPLUGIN_PLUGIN_H_

