#pragma once

#include "tisudshl.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

using namespace DShowLib;

//加载相机DLL
bool ImgCameraInitLibrary();
void ImgCameraExitLibrary();

class CIMGCamBase
{
public:
	CIMGCamBase(void);
	~CIMGCamBase(void);

	//初始化相机
	virtual bool InitCam(unsigned long ulSN);
	virtual bool UnInitCam();

	int GetImageWidth();				// 获取图片宽度
	int GetImageHeight();				// 获取图片高度

	virtual void SetBuffer(BYTE* pBuffer);		// 设置图像缓冲区
	virtual bool SingleSnap(bool bDisplay = true);	// 单帧采集

	bool SetTriggerMode(bool bSoft);		//设置触发模式	
	bool SetExposureTime(double dValue);	//设置曝光时间	
	bool SetGain(double dValue);			//设置增益

	double GetExposureTime();		//获取曝光时间
	double GetGain();				//获取增益

	bool IsOpen();

    //获取相机SN列表
    static bool GetCamList(vector<string> &vecSN,unsigned long MaxCnt);

protected:
	void SoftTrigger();

	Grabber m_Grabber;
	FrameHandlerSink::tFHSPtr m_pSink;

	BYTE* m_pImageBuffer;	
	UINT32 m_iImageWidth;
	UINT32 m_iImageHeight;

	unsigned long m_iSN;								 //相机SN，例如0x40214390
	bool m_bOpen;										 //相机是否打开
	bool m_bSoftTrigger;								 //是否软触发

	tIVCDPropertyItemsPtr		 m_pPropertyItems;		 //配置属性项，如增益，曝光等
	tIVCDButtonPropertyPtr		 m_pSoftTrigger;		 //软触发按钮属性
	tIVCDSwitchPropertyPtr		 m_pTriggerSwitch;		 //软触发enable属性

	static bool m_bInitLib;
};
