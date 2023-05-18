#ifndef _LINE_CAMERAMANAGERPLUGIN_PLUGIN_H_
#define _LINE_CAMERAMANAGERPLUGIN_PLUGIN_H_

#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "vscameratools/camera/icamerafactory.h"
#include "jmutex.h"
#include "jthreadt.h"

//#include <string>
//#include <map>
//using namespace std;

class CLineCamManagerPlugin : public ILineCameraFactoryManage
{
public:
    CLineCamManagerPlugin(void);
    ~CLineCamManagerPlugin(void);
    
    static CLineCamManagerPlugin *getInstance();
    static void ReleaseInstance();

    //virtual ICameraFactory* GetCameraFactory(int iCamType);
    virtual void EnumCameraDesc(vector<string> &vecCamDesc);
    virtual string GetCamDesc(int iCamType);
    virtual int GetCamType(const string &strCamDesc);
    
    //通过ID， 相机类型获取相机实例， 类型不确定则只用ID匹配
    virtual ILineCamera *GetCameraInstance(string strCamName, int iCamType = UNKNOWN_CAM);
    virtual string GetCameraNameWithGuid(string ID);  //通过别名获取相机ID
    /*获取枚举到的所有类型相机的GUID
    * 输出 ： 相机GUID列表
    * 返回 ： 列表个数*/
    virtual int GetCameraNameList(vector<string> &vecName, int iCamType = UNKNOWN_CAM);
    //获取枚举到的有效相机
    virtual int GetValidCameraList(map<string, ILineCamera *> &mapCamera);
    virtual int GetLastError() { return m_lastError; };
    virtual int GetCamStatus(const char *szCamName = NULL);
    virtual void ReInit();
    void Init();
    void UnInit();

    virtual bool GetCameraInParam(string strName, LINE_CAM_PARAM &info);  //读取相机参数
    virtual bool SetCameraInParam(LINE_CAM_PARAM &param);//设置相机参数
    virtual bool UpdateCameraName(LINE_CAM_PARAM &info, string strOldName);//更新相机名称
    virtual bool CreateCameraConfig(string strName, LINE_CAM_PARAM &info);//创建相机
    virtual bool DeleteCameraConfig(string strName);//删除相机

    virtual bool CameraAcqImage(const char *szJobName, const char *szItemName, void *param = NULL);
    //刷新连接的相机列表
    virtual bool RefreshValidCameraList();
    virtual bool IsInitFinished();
    //根据索引或名称获取对应通道图像
 //   //img 输出，图像对象地址，chid 通道ID，  index 通道下对应相机索引
 //   virtual bool getSequenceImageBuffer(stImageInfo *img, int chid, int index = 0);
 //   //img 输出，图像对象地址，chName 通道名称，  index 通道下对应相机索引
 //   virtual bool getSequenceImageBuffer(stImageInfo *img, string chName, int index = 0);
	//virtual ILineCamera *getSequenceImageInstance(int chid, int index = 0);
	//virtual int getSequenceInCamIndex(int index); //通过通道索引， 获取相机在该通道下对应的内部索引， 用于取图等操作
    virtual void SaveCameraInParam();
private:
    //读配置文件
    bool ReadConfig(void);
    //写配置文件
    bool SaveConfig();
    void ThreadInitCam(void* param);
private:
    bool     m_bIsInit;
    JThreadTP<CLineCamManagerPlugin,void*,void>*    m_pThreadInit;
    //vector<ICameraFactory*> m_vecCameraFactory;
    static CLineCamManagerPlugin *m_pInstance;
    int  m_lastError;
    map<string, ILineCamera *> m_camList;    //(SN, ICamera) 
    map<string, LINE_CAM_PARAM> m_camConfigList;  //(CameraName, Config)
    ILineCameraFactory *m_pCamFactory[ILineCameraFactoryManage::LINE_CAM_TYPE_COUNT];
    JMutex    m_lock;
    bool      m_bIsRunning;
};


#endif _CAMERAMANAGERPLUGIN_PLUGIN_H_

