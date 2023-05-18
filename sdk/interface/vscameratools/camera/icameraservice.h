#pragma once

class ICameraService
{
public:
	enum
	{
		IID_CAMERASERVICE = 0x100
	};
	virtual ~ICameraService(){}
	// �����
	virtual bool Open(const char *pszGUID) = 0;
    // �ر����
    virtual bool Close(const char *pszGUID) = 0;
    // ��ȡ����б�
    virtual bool GetCamList(vector<string> vecGUID,unsigned long MaxCnt) = 0;
    // ����һ���������
    virtual bool CreateCamera(const char *pszGUID, unsigned int index) = 0;
	// ��ȡ������
	//virtual bool GetImageSize(const char *pszGUID, int &w, int &h) = 0;
	
	// �ӵ�index�������ȡͼƬ
	//virtual bool GetImage(const char *pszGUID, void *pImage) = 0;

	//// �����������
	//virtual bool SetSetGain(const char *pszGUID,unsigned int GainValue) = 0;

	//// ��������ع�ʱ��
	//virtual bool SetExposureTime(const char *pszGUID,unsigned int ExpTimeValue) = 0;

	
protected:
private:
};