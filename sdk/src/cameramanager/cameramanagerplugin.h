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
    
    //ͨ��ID�� ������ͻ�ȡ���ʵ���� ���Ͳ�ȷ����ֻ��IDƥ��
    virtual ICamera *GetCameraInstance(string id, int iCamType = UNKNOWN_CAM);
    virtual string GetCameraNameWithGuid(string ID);  //ͨ��������ȡ���ID
    /*��ȡö�ٵ����������������GUID
    * ��� �� ���GUID�б�
    * ���� �� �б����*/
    virtual int GetCameraNameList(vector<string> &vecName, int iCamType = UNKNOWN_CAM);
    //��ȡö�ٵ�����Ч���
    virtual int GetValidCameraList(map<string, ICamera *> &mapCamera);
    virtual int GetLastError() {return m_lastError;};
    void Init();
    void UnInit();

    virtual bool GetCameraInfo(string strName, VS_CAMERA_INFO &info);  //��ȡ�������
    virtual bool SetCameraInParam(VS_CAMERA_INFO &param);//�����������
    virtual bool UpdateCameraName(VS_CAMERA_INFO &info, string strOldName);//�����������
    virtual bool CreateCameraConfig(string strName, VS_CAMERA_INFO &info);//�������
    virtual bool DeleteCameraConfig(string strName);//ɾ�����

    virtual bool CameraAcqImage(const char *szJobName, const char *szItemName, void *param = NULL);
    //ˢ�����ӵ�����б�
    virtual bool RefreshValidCameraList();

private:
    //�������ļ�
    bool ReadConfig(void);
    //д�����ļ�
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

