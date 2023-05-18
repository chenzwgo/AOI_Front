#ifndef ILINE_CAMERA_H
#define ILINE_CAMERA_H
#include <string>
using namespace std;

#define MAX_CHANNEL_COUNT    8

enum LINE_TRIGGER_TYPE
{
    LINE_TG_NONE = 0,
    LINE_TG_INTERNAL,
    LINE_TG_EXTERNAL,
    LINE_TG_SHAF_ENCODER
};

enum LINE_TRIGGER_DETECTION
{
    TG_ACTIVE_LOW = 0,
    TG_ACTIVE_HIGH,
    TG_RISING_EDGE,
    TG_FALLING_EDGE
};

struct LINE_CAM_PARAM
{
    bool                    bEable;
    bool                    bExtTrigger;
    bool                    bIsInvert;
	bool                    bIsTDI;
    int                     channel;  //
    unsigned long           width;
    unsigned long           height;     //有效行数
    unsigned long           snapheight;     //单帧采集时行数
    unsigned long           realheight;     //实时显示时行数
    unsigned long           exposure;   //曝光值
    unsigned long           gain;   //曝光值
    unsigned long           grabFreq;   //内部触发相机采样行频 行每秒
    unsigned long           blockSize;   //最小块大小,每次处理的最小行数
    LINE_TRIGGER_TYPE       tgType;
    LINE_TRIGGER_DETECTION  tgDetection;
    string                  strCameraName;    //相机别名
    string                  strCameraID;      //相机识别符
	string                  strCameraSerialNumber;//相机串號
    string                  strCamConfigPath;  //相机配置路径
    string                  strCamDes;         //相机描述
    string                  strCardPluginID;    //采集卡插件ID
    string                  strCamPluginID;    //相机插件ID

    LINE_CAM_PARAM()
    {
        bEable         = true;
        bExtTrigger    = false;
        bIsInvert      = false;
		bIsTDI         = false;
        channel        = 1;
        width          = 8192;
        snapheight     = 100;
        realheight     = 100;
        height         = snapheight;
        exposure       = 20;
        gain           = 0;
        grabFreq       = 10000;
        blockSize      = 512;
        tgType         = LINE_TG_NONE;
        tgDetection    = TG_FALLING_EDGE;
        strCameraID    = "";
        strCameraName  = "";
        strCamConfigPath = "";
        strCamDes = "";
        strCardPluginID = "dalsalinecamera";
        strCamPluginID = "dalsalinecamera";
		strCameraSerialNumber = "";
    }
    LINE_CAM_PARAM(LINE_CAM_PARAM &other)
    {
        if (other.channel >= MAX_CHANNEL_COUNT)
        {
            channel = 8;
        }
        else
        {
            channel        = other.channel;
        }
        bEable         = other.bEable;
        bExtTrigger    = other.bExtTrigger;
        bIsInvert      = other.bIsInvert;
		bIsTDI         = other.bIsTDI;
        width          = other.width;
        height         = other.height;
        snapheight     = other.snapheight;
        realheight     = other.realheight;
        exposure       = other.exposure;
        gain           = other.gain;
        grabFreq       = other.grabFreq;
        blockSize      = other.blockSize;
        tgType         = other.tgType;
        tgDetection    = other.tgDetection;
        strCameraID    = other.strCameraID;
        strCameraName  = other.strCameraName;
        strCamConfigPath = other.strCamConfigPath;
        strCamDes = other.strCamDes;
        strCardPluginID = other.strCardPluginID;
        strCamPluginID = other.strCamPluginID;
		strCameraSerialNumber = other.strCameraSerialNumber;
    }
    LINE_CAM_PARAM operator=(const LINE_CAM_PARAM &other)
    {
        if (other.channel >= MAX_CHANNEL_COUNT)
        {
            channel = 8;
        }
        else
        {
            channel        = other.channel;
        }
        bEable         = other.bEable;
        bExtTrigger    = other.bExtTrigger;
        bIsInvert      = other.bIsInvert;
		bIsTDI         = other.bIsTDI;
        width          = other.width;
        height         = other.height;
        snapheight     = other.snapheight;
        realheight     = other.realheight;
        exposure       = other.exposure;
        gain           = other.gain;
        grabFreq       = other.grabFreq;
        blockSize      = other.blockSize;
        tgType         = other.tgType;
        tgDetection    = other.tgDetection;
        strCameraID    = other.strCameraID;
        strCameraName  = other.strCameraName;
        strCamConfigPath = other.strCamConfigPath;
        strCamDes = other.strCamDes;
        strCardPluginID  = other.strCardPluginID;
        strCamPluginID   = other.strCamPluginID;
		strCameraSerialNumber = other.strCameraSerialNumber;
        return *this;
    }
    bool operator==(const LINE_CAM_PARAM &other)
    {
        if (channel        == other.channel
            && bEable         == other.bEable
            && bExtTrigger    == other.bExtTrigger
            && bIsInvert      == other.bIsInvert
			&& bIsTDI         == other.bIsTDI
            && width          == other.width
            && height         == other.height
            && snapheight     == other.snapheight
            && realheight     == other.realheight
            && exposure       == other.exposure
            && gain           == other.gain
            && grabFreq       == other.grabFreq
            && blockSize      == other.blockSize
            && tgType         == other.tgType
            && tgDetection    == other.tgDetection
            && strCameraID    == other.strCameraID
            && strCameraName  == other.strCameraName
            && strCamConfigPath == other.strCamConfigPath
            && strCamDes == other.strCamDes
            && strCardPluginID == other.strCardPluginID
            && strCamPluginID == other.strCamPluginID
			&& strCameraSerialNumber == other.strCameraSerialNumber)
        {
            return true;
        }
        return false;
    }
    bool operator!=(const LINE_CAM_PARAM &other)
    {
        return !(*this == other);
    }
};

typedef void (*FnLineTriggerFunction)(void *, unsigned char *, int , int);

//namespace BJ_CAM_INFO
//{
//#define STATUS_UNINIT                0x01       //未初始化
//#define STATUS_CAM_OPEN_FAILED       0x02       //打开相机失败
//#define STATUS_SET_PARAM_FAILED      0x04       //设置参数失败
//#define STATUS_GRAB_FAILED           0x08       //图像采集失败
//#define STATUS_CAM_NOT_FOUND         0x10       //没找到相机
//#define STATUS_CAM_OFF_LINE          0x80       //相机掉线
//
//#define STATUS_CARD_UNINIT           0x0100       //未初始化
//#define STATUS_CARD_OPEN_FAILED      0x0200       //打开控制卡失败
//}

class ILineCamera
{
public:
    enum X_LINE_CAM_STATUS
    {
        STATUS_UNINIT = 0,             //未初始化
        STATUS_CAM_OPEN_FAILED,        //打开相机失败
        //STATUS_SET_PARAM_FAILED,       //设置参数失败
        STATUS_GRAB_FAILED,            //图像采集失败
        STATUS_CAM_NOT_FOUND,          //没找到相机
        STATUS_CAM_OFF_LINE,           //相机掉线
        //正常状态
        STATUS_CAM_IDLE,               //相机就绪
        STATUS_CAM_READ_ABORT,         //相机采图中断
        STATUS_CAM_READING,            //相机采图中
        STATUS_CAM_READ_END,           //相机采图完成
    };
    enum X_ERROR_CODE
    {
        ERR_UNKNOW = -1,      //未知异常
        ERR_OK = 0,
        ERR_OPEN_FAILED,      //相机打开失败
        ERR_ACQ_FAILED,       //采图失败
        ERR_GRAP_RUNNING,     //采图未完成
        ERR_SET_PARAM_FAILED,       //参数设置失败
        ERR_GET_SIZE_FAILED,       //获取图像大小异常
        ERR_ABORT_FAILED,       //终止异常
        ERR_GRAB_FAILED,       //连续采集失败
        ERR_PARAM_INVALID,    //入参异常
    };

	enum
	{
		IID_CAMERA = 0x100
	};
    virtual ~ILineCamera(){}
    virtual bool InitCam(unsigned int ServerIndex, unsigned int ResourceIndex, const char *szDeviceName, const LINE_CAM_PARAM *param) = 0;
    virtual bool SingleAcq(bool bSoft = false) = 0;  //非外触发模式下， 不用传参数， 外触发模式下， 可置bSoft为true, 模拟外触发
    virtual bool StartAcq() = 0;
    //virtual bool GetImage(void *pR, void *pG, void *pB, const char *szJobName = 0, bool bSetParam=true, bool acq = true) = 0;
    //ichannel 为通道索引，  index=-1时，取整张图像，否则为返回index行的首地址, iLineNumb为要取连续多少行-即需要保证相机已经采完所需的图片
    virtual const unsigned char *GetImage() = 0; 
    //virtual const unsigned char *GetImageBufferAdr(int ichannel) = 0;
	virtual bool GetImageSize(int &w, int &h) = 0;
    virtual bool AbortAcq() = 0;
    virtual void Reset() = 0;
    virtual bool IsImplemented() { return false; };   //是否是彩色相机
    virtual bool Continuations(bool bOpen, int iSize = 1) = 0;
    virtual bool IsContinuations(){return false;}          //否在实时采图

	virtual bool Open() = 0;
    virtual bool Close() = 0;
    virtual bool IsOpen() = 0;
    virtual void GetCameraInParam(LINE_CAM_PARAM &info) {info = m_CameraParam;}  //读取相机参数
    virtual bool SetCameraInParam(LINE_CAM_PARAM &param)  = 0;//设置相机参数

    virtual void ClearBuf() {};
    //是否取像完成
    virtual bool IsSnapFinished() = 0;
    //等待取像完成
    virtual bool WaitGetImageFinished(int iTimeOut = -1) = 0;
    //重置取像完成状态
    virtual bool ResetFinishState() = 0;

    virtual int GetLastError(char *errInfo = NULL, int *iSize = NULL) = 0;
    virtual int GetCamStatus() = 0;
    virtual void SetTriggerFunction(FnLineTriggerFunction fnTriggerFunction, void *pInParam) = 0;
    virtual bool IsInitFinished() = 0;
protected:
    LINE_CAM_PARAM   m_CameraParam;
};

#endif // ICAMERA_H