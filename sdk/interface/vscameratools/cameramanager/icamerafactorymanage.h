#ifndef _I_CAMERA_FACTORY_MANAGE_H_
#define _I_CAMERA_FACTORY_MANAGE_H_

#include <string>
#include <vector>
#include <map>
#include "common_data.h"

using std::string;
using std::vector;
using std::map;

class ICameraFactory;
class ICamera;
class ILineCamera;

struct VS_CAMERA_INFO;
struct LINE_CAM_PARAM;

//相机类型

class ICameraFactoryManage
{
public:
    enum EnCameraType
    {
        UNKNOWN_CAM = -1,
        AVT_CAM,
        IMG_CAM,
        BASLER_CAM,
        BAUMER_CAM,
        DHMER_CAM,
        OTHER_CAM,

        CAM_TYPE_COUNT
    };
	enum
	{
		IID_CAMERA_FACTORY_MANAGE = 0x100
	};

    virtual ~ICameraFactoryManage(){};

    //virtual ICameraFactory* GetCameraFactory(int iCamType) = 0;
    virtual void EnumCameraDesc(vector<string> &vecCamDesc) = 0;
    virtual string GetCamDesc(int iCamType) = 0;
    virtual int GetCamType(const string &strCamDesc) = 0;
    //通过ID， 相机类型获取相机实例， 类型不确定则只用ID匹配
    virtual ICamera *GetCameraInstance(string strCamName, int iCamType = UNKNOWN_CAM) = 0;
    /*获取枚举到的所有类型相机的GUID
    * 输出 ： 相机GUID列表
    * 返回 ： 列表个数*/
    virtual int GetCameraNameList(vector<string> &vecName, int iCamType = UNKNOWN_CAM) = 0;
    //获取枚举到的有效相机
    virtual int GetValidCameraList(map<string, ICamera *> &mapCamera) = 0;
    virtual int GetLastError() = 0;
    virtual string GetCameraNameWithGuid(string ID) = 0;  //通过别名获取实例

    virtual bool GetCameraInfo(string strName, VS_CAMERA_INFO &info) = 0;  //读取相机参数
    virtual bool SetCameraInParam(VS_CAMERA_INFO &param) = 0;//设置相机参数
    virtual bool UpdateCameraName(VS_CAMERA_INFO &info, string strOldName) = 0;//更新相机名称
    virtual bool CreateCameraConfig(string strName, VS_CAMERA_INFO &info) = 0;//创建相机
    virtual bool DeleteCameraConfig(string strName) = 0;//删除相机

    virtual bool CameraAcqImage(const char *szJobName, const char *szItemName, void *param = NULL) = 0;  //触发采集

    //刷新连接的相机列表
    virtual bool RefreshValidCameraList() = 0;
};

class ILineCameraFactoryManage
{
public:
    enum EnLineCameraType
    {
        UNKNOWN_CAM = -1,
        DALSA_CAM = 0,
        INDIGO_CAM,
        LINE_CAM_TYPE_COUNT
    };
	enum
	{
		IID_LINE_CAMERA_FACTORY_MANAGE = 0x101
	};
    enum EnErrorCode
    {
        ERR_UNINIT = -1,
        ERR_OK = 0,
        ERR_OPEN_CARD_FAILED,
        ERR_OPEN_CAM_FAILED,
    };

    virtual ~ILineCameraFactoryManage(){};

    //virtual ICameraFactory* GetCameraFactory(int iCamType) = 0;
    virtual void EnumCameraDesc(vector<string> &vecCamDesc) = 0;
    virtual string GetCamDesc(int iCamType) = 0;
    virtual int GetCamType(const string &strCamDesc) = 0;
    //通过ID， 相机类型获取相机实例， 类型不确定则只用ID匹配
    virtual ILineCamera *GetCameraInstance(string id, int iCamType = UNKNOWN_CAM) = 0;
    /*获取枚举到的所有类型相机的GUID
    * 输出 ： 相机GUID列表
    * 返回 ： 列表个数*/
    virtual int GetCameraNameList(vector<string> &vecName, int iCamType = UNKNOWN_CAM) = 0;
    //获取枚举到的有效相机
    virtual int GetValidCameraList(map<string, ILineCamera *> &mapCamera) = 0;
    virtual int GetLastError() = 0;
    virtual int GetCamStatus(const char *szCamName = NULL) = 0;
    virtual string GetCameraNameWithGuid(string ID) = 0;  //通过别名获取实例

    virtual bool GetCameraInParam(string strName, LINE_CAM_PARAM &info) = 0;  //读取相机参数
    virtual bool SetCameraInParam(LINE_CAM_PARAM &param) = 0;//设置相机参数
    virtual bool UpdateCameraName(LINE_CAM_PARAM &info, string strOldName) = 0;//更新相机名称
    virtual bool CreateCameraConfig(string strName, LINE_CAM_PARAM &info) = 0;//创建相机
    virtual bool DeleteCameraConfig(string strName) = 0;//删除相机

    virtual bool CameraAcqImage(const char *szJobName, const char *szItemName, void *param = NULL) = 0;  //触发采集

    //刷新连接的相机列表
    virtual bool RefreshValidCameraList() = 0;
    virtual bool IsInitFinished() { return true; };
 //   //img 输出，图像对象地址，chid 通道ID，  index 通道下对应相机索引
 //   virtual bool getSequenceImageBuffer(stImageInfo *img, int chid, int index = 0) = 0;
 //   //img 输出，图像对象地址，chName 通道名称，  index 通道下对应相机索引
 //   virtual bool getSequenceImageBuffer(stImageInfo *img, string chName, int index = 0) = 0;
 //   virtual ILineCamera *getSequenceImageInstance(int chid, int index = 0) = 0;
	//virtual int getSequenceInCamIndex(int index) = 0; //通过通道索引， 获取相机在该通道下对应的内部索引， 用于取图等操作
    virtual void ReInit() = 0;  //产品更新后， 重新初始化配置
    virtual void SaveCameraInParam() = 0;
};
#endif