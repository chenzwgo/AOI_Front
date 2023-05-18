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
    string RealCameraType;           // "AVT", "IMAGING"���
    string ID ;                      // ����ͼƬ����·����ʶ����ʵ������������GUID
    int ExposureTime;                // �ع�ʱ��
    int Gain;                        // ����
    //int TriggerMode;               // �Ƿ����ⴥ��ģʽ
    //int iTimeout;                  // �ⴥ��ģʽ��ȡ��ʱʱ��
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
    string strpicpath ;               // ����ͼƬ����·����ʶ����ʵ������������GUID
	
	string strpicfolder;              // ���ڱ����ļ���
	vector<string> vecpicpath ;       // ���ڱ����ļ����ڵ��ļ�·��
	int    iPlaySpeed;                // ȡ������
	int    iPicIndex;                 // ��ǰͼ������

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
