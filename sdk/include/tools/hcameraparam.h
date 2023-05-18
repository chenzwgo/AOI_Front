#ifndef _READ_JOB_CAMERA_PARAM_H_
#define _READ_JOB_CAMERA_PARAM_H_
#include <string>
#include <vector>
using namespace std;

enum EnImageFormType
{
    UNKNOWN_FROM = -1,
    IMAGE_FILE,
    IMAGE_CAMERA,

    IMAGE_TYPE_COUNT
};

class CameraParam
{
public:
    string RealCameraType;           // "AVT", "IMAGING"相机
    string ID ;                      // 离线图片就用路径标识，真实相机就是相机的GUID
    int ExposureTime;                // 曝光时间
    int Gain;                        // 增益
    //int TriggerMode;               // 是否是外触发模式
    //int iTimeout;                  // 外触发模式下取像超时时间
    CameraParam()
    {
        RealCameraType = "";
        ID ="";
        ExposureTime=0;
        Gain=0;
        //TriggerMode = false;
        //iTimeout = 0;
    }
	CameraParam(const CameraParam& other)
	{        
		RealCameraType = other.RealCameraType;
		ID             = other.ID;
		ExposureTime   = other.ExposureTime;
		Gain           = other.Gain;
        //TriggerMode   = other.TriggerMode;
        //iTimeout      = other.iTimeout;
	}
	CameraParam &operator= (const CameraParam& other)
	{
		if (this != &other)
		{
			RealCameraType = other.RealCameraType;
			ID             = other.ID;
			ExposureTime   = other.ExposureTime;
			Gain           = other.Gain;
            //TriggerMode   = other.TriggerMode;
            //iTimeout      = other.iTimeout;
		}
		return *this;
	}

    void clear()
    {    
        RealCameraType = "";
        ID ="";
        ExposureTime=0;
        Gain=0;
        //TriggerMode = false;
        //iTimeout    = 0;
    }
};

class picParam
{
public:
    string strpicpath ;               // 离线图片就用路径标识，真实相机就是相机的GUID
	
	string strpicfolder;              // 用于保存文件夹
	vector<string> vecpicpath ;       // 用于保存文件夹内的文件路径
	int    iPlaySpeed;                // 取像速率
	int    iPicIndex;                 // 当前图像索引

    bool bDir;

    picParam()
    {
		vecpicpath.clear();
        strpicpath ="";
		strpicfolder = "";
		iPlaySpeed = 2;
		iPicIndex  = 2;
        bDir = false;
    }
    void clear()
    {
		vecpicpath.clear();
        strpicpath ="";
		strpicfolder = "";
		iPlaySpeed = 2;
		iPicIndex  = 2;
        bDir = false;
    }
};

struct STImageFormInfo
{
    EnImageFormType type;
    picParam picParamInfo;
    CameraParam cameraParamInfo;
    //void *pConfig;
    STImageFormInfo()
    {
        type = IMAGE_FILE;
        picParamInfo.clear();
        cameraParamInfo.clear();
    }
    STImageFormInfo &operator = (const STImageFormInfo &other)
    {
        type = other.type;
        picParamInfo = other.picParamInfo;
        cameraParamInfo = other.cameraParamInfo;
        //pConfig = other.pConfig;
        return *this;
    }
};

//enum XTV_EN_PRE_TYPE
//{
//    ;
//};
//
//struct XTV_ST_PRE_IMAGE_PARAM
//{
//    ;
//};

#endif  //_READ_JOB_CAMERA_PARAM_H_
