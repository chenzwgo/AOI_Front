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

//�������

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
    //ͨ��ID�� ������ͻ�ȡ���ʵ���� ���Ͳ�ȷ����ֻ��IDƥ��
    virtual ICamera *GetCameraInstance(string strCamName, int iCamType = UNKNOWN_CAM) = 0;
    /*��ȡö�ٵ����������������GUID
    * ��� �� ���GUID�б�
    * ���� �� �б����*/
    virtual int GetCameraNameList(vector<string> &vecName, int iCamType = UNKNOWN_CAM) = 0;
    //��ȡö�ٵ�����Ч���
    virtual int GetValidCameraList(map<string, ICamera *> &mapCamera) = 0;
    virtual int GetLastError() = 0;
    virtual string GetCameraNameWithGuid(string ID) = 0;  //ͨ��������ȡʵ��

    virtual bool GetCameraInfo(string strName, VS_CAMERA_INFO &info) = 0;  //��ȡ�������
    virtual bool SetCameraInParam(VS_CAMERA_INFO &param) = 0;//�����������
    virtual bool UpdateCameraName(VS_CAMERA_INFO &info, string strOldName) = 0;//�����������
    virtual bool CreateCameraConfig(string strName, VS_CAMERA_INFO &info) = 0;//�������
    virtual bool DeleteCameraConfig(string strName) = 0;//ɾ�����

    virtual bool CameraAcqImage(const char *szJobName, const char *szItemName, void *param = NULL) = 0;  //�����ɼ�

    //ˢ�����ӵ�����б�
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
    //ͨ��ID�� ������ͻ�ȡ���ʵ���� ���Ͳ�ȷ����ֻ��IDƥ��
    virtual ILineCamera *GetCameraInstance(string id, int iCamType = UNKNOWN_CAM) = 0;
    /*��ȡö�ٵ����������������GUID
    * ��� �� ���GUID�б�
    * ���� �� �б����*/
    virtual int GetCameraNameList(vector<string> &vecName, int iCamType = UNKNOWN_CAM) = 0;
    //��ȡö�ٵ�����Ч���
    virtual int GetValidCameraList(map<string, ILineCamera *> &mapCamera) = 0;
    virtual int GetLastError() = 0;
    virtual int GetCamStatus(const char *szCamName = NULL) = 0;
    virtual string GetCameraNameWithGuid(string ID) = 0;  //ͨ��������ȡʵ��

    virtual bool GetCameraInParam(string strName, LINE_CAM_PARAM &info) = 0;  //��ȡ�������
    virtual bool SetCameraInParam(LINE_CAM_PARAM &param) = 0;//�����������
    virtual bool UpdateCameraName(LINE_CAM_PARAM &info, string strOldName) = 0;//�����������
    virtual bool CreateCameraConfig(string strName, LINE_CAM_PARAM &info) = 0;//�������
    virtual bool DeleteCameraConfig(string strName) = 0;//ɾ�����

    virtual bool CameraAcqImage(const char *szJobName, const char *szItemName, void *param = NULL) = 0;  //�����ɼ�

    //ˢ�����ӵ�����б�
    virtual bool RefreshValidCameraList() = 0;
    virtual bool IsInitFinished() { return true; };
 //   //img �����ͼ������ַ��chid ͨ��ID��  index ͨ���¶�Ӧ�������
 //   virtual bool getSequenceImageBuffer(stImageInfo *img, int chid, int index = 0) = 0;
 //   //img �����ͼ������ַ��chName ͨ�����ƣ�  index ͨ���¶�Ӧ�������
 //   virtual bool getSequenceImageBuffer(stImageInfo *img, string chName, int index = 0) = 0;
 //   virtual ILineCamera *getSequenceImageInstance(int chid, int index = 0) = 0;
	//virtual int getSequenceInCamIndex(int index) = 0; //ͨ��ͨ�������� ��ȡ����ڸ�ͨ���¶�Ӧ���ڲ������� ����ȡͼ�Ȳ���
    virtual void ReInit() = 0;  //��Ʒ���º� ���³�ʼ������
    virtual void SaveCameraInParam() = 0;
};
#endif