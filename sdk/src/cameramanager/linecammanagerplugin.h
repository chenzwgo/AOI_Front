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
    
    //ͨ��ID�� ������ͻ�ȡ���ʵ���� ���Ͳ�ȷ����ֻ��IDƥ��
    virtual ILineCamera *GetCameraInstance(string strCamName, int iCamType = UNKNOWN_CAM);
    virtual string GetCameraNameWithGuid(string ID);  //ͨ��������ȡ���ID
    /*��ȡö�ٵ����������������GUID
    * ��� �� ���GUID�б�
    * ���� �� �б����*/
    virtual int GetCameraNameList(vector<string> &vecName, int iCamType = UNKNOWN_CAM);
    //��ȡö�ٵ�����Ч���
    virtual int GetValidCameraList(map<string, ILineCamera *> &mapCamera);
    virtual int GetLastError() { return m_lastError; };
    virtual int GetCamStatus(const char *szCamName = NULL);
    virtual void ReInit();
    void Init();
    void UnInit();

    virtual bool GetCameraInParam(string strName, LINE_CAM_PARAM &info);  //��ȡ�������
    virtual bool SetCameraInParam(LINE_CAM_PARAM &param);//�����������
    virtual bool UpdateCameraName(LINE_CAM_PARAM &info, string strOldName);//�����������
    virtual bool CreateCameraConfig(string strName, LINE_CAM_PARAM &info);//�������
    virtual bool DeleteCameraConfig(string strName);//ɾ�����

    virtual bool CameraAcqImage(const char *szJobName, const char *szItemName, void *param = NULL);
    //ˢ�����ӵ�����б�
    virtual bool RefreshValidCameraList();
    virtual bool IsInitFinished();
    //�������������ƻ�ȡ��Ӧͨ��ͼ��
 //   //img �����ͼ������ַ��chid ͨ��ID��  index ͨ���¶�Ӧ�������
 //   virtual bool getSequenceImageBuffer(stImageInfo *img, int chid, int index = 0);
 //   //img �����ͼ������ַ��chName ͨ�����ƣ�  index ͨ���¶�Ӧ�������
 //   virtual bool getSequenceImageBuffer(stImageInfo *img, string chName, int index = 0);
	//virtual ILineCamera *getSequenceImageInstance(int chid, int index = 0);
	//virtual int getSequenceInCamIndex(int index); //ͨ��ͨ�������� ��ȡ����ڸ�ͨ���¶�Ӧ���ڲ������� ����ȡͼ�Ȳ���
    virtual void SaveCameraInParam();
private:
    //�������ļ�
    bool ReadConfig(void);
    //д�����ļ�
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

