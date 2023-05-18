#pragma once

class ICameraService
{
public:
	enum
	{
		IID_CAMERASERVICE = 0x100
	};
	virtual ~ICameraService(){}
	// 打开相机
	virtual bool Open(const char *pszGUID) = 0;
    // 关闭相机
    virtual bool Close(const char *pszGUID) = 0;
    // 获取相机列表
    virtual bool GetCamList(vector<string> vecGUID,unsigned long MaxCnt) = 0;
    // 创建一个相机对象
    virtual bool CreateCamera(const char *pszGUID, unsigned int index) = 0;
	// 获取相机宽高
	//virtual bool GetImageSize(const char *pszGUID, int &w, int &h) = 0;
	
	// 从第index个相机获取图片
	//virtual bool GetImage(const char *pszGUID, void *pImage) = 0;

	//// 设置相机增益
	//virtual bool SetSetGain(const char *pszGUID,unsigned int GainValue) = 0;

	//// 设置相机曝光时间
	//virtual bool SetExposureTime(const char *pszGUID,unsigned int ExpTimeValue) = 0;

	
protected:
private:
};