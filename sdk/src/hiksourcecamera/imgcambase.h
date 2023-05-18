#pragma once

#include "tisudshl.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

using namespace DShowLib;

//�������DLL
bool ImgCameraInitLibrary();
void ImgCameraExitLibrary();

class CIMGCamBase
{
public:
	CIMGCamBase(void);
	~CIMGCamBase(void);

	//��ʼ�����
	virtual bool InitCam(unsigned long ulSN);
	virtual bool UnInitCam();

	int GetImageWidth();				// ��ȡͼƬ���
	int GetImageHeight();				// ��ȡͼƬ�߶�

	virtual void SetBuffer(BYTE* pBuffer);		// ����ͼ�񻺳���
	virtual bool SingleSnap(bool bDisplay = true);	// ��֡�ɼ�

	bool SetTriggerMode(bool bSoft);		//���ô���ģʽ	
	bool SetExposureTime(double dValue);	//�����ع�ʱ��	
	bool SetGain(double dValue);			//��������

	double GetExposureTime();		//��ȡ�ع�ʱ��
	double GetGain();				//��ȡ����

	bool IsOpen();

    //��ȡ���SN�б�
    static bool GetCamList(vector<string> &vecSN,unsigned long MaxCnt);

protected:
	void SoftTrigger();

	Grabber m_Grabber;
	FrameHandlerSink::tFHSPtr m_pSink;

	BYTE* m_pImageBuffer;	
	UINT32 m_iImageWidth;
	UINT32 m_iImageHeight;

	unsigned long m_iSN;								 //���SN������0x40214390
	bool m_bOpen;										 //����Ƿ��
	bool m_bSoftTrigger;								 //�Ƿ�����

	tIVCDPropertyItemsPtr		 m_pPropertyItems;		 //��������������棬�ع��
	tIVCDButtonPropertyPtr		 m_pSoftTrigger;		 //������ť����
	tIVCDSwitchPropertyPtr		 m_pTriggerSwitch;		 //����enable����

	static bool m_bInitLib;
};
