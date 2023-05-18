
#include "IMGCamBase.h"
#include "ImgCameraFactoryPlugin.h"

//software trigger
static const _GUID VCDElement_ST =	{ 0xFDB4003C, 0x552C, 0x4FAA, { 0xB8, 0x7B, 0x42, 0xE8, 0x88, 0xD5, 0x41, 0x47 } };

static bool m_bInitLib = false;

CIMGCamBase::CIMGCamBase(void): m_pSink(NULL) 
	,m_pImageBuffer(NULL)
	, m_iImageWidth(0)
	, m_iImageHeight(0)
	, m_iSN(0)
	, m_bOpen(false)
	, m_pPropertyItems(NULL)
	, m_pSoftTrigger(NULL)
	, m_pTriggerSwitch(NULL)
{
}


CIMGCamBase::~CIMGCamBase(void)
{
}

bool CIMGCamBase::InitCam(unsigned long ulSN)
{
	bool bRet = false;
	bRet = ImgCameraInitLibrary();
	if (!bRet)
	{
		return false;
	}

	return bRet;
}

bool CIMGCamBase::UnInitCam()
{
	//if (m_Grabber.isLive())
	//{
	//	m_Grabber.stopLive();
	//}

	//if (m_Grabber.isDevOpen())
	//{
	//	m_Grabber.closeDev();
	//}

	//m_bOpen = false;
	////ExitLibrary();

	return true;
}

int CIMGCamBase::GetImageWidth()
{  
	return m_iImageWidth;
}

int CIMGCamBase::GetImageHeight()
{
	return m_iImageHeight;
}

void CIMGCamBase::SetBuffer(BYTE* pBuffer)
{
}

bool CIMGCamBase::SingleSnap(bool bDisplay)
{
	bool bRet = false;
	if (m_pSink != NULL)
	{
		if (m_Grabber.isLive())
		{
			if (m_bSoftTrigger)
			{
				OutputDebugString("begin SoftTrigger\n");
				SoftTrigger();
				OutputDebugString("End SoftTrigger\n");
			}

			Error Err = m_pSink->snapImages(1, 1000);
			int iErr = Err.getVal();
			if (eNOERROR == iErr)
			{
				//m_pSink->getLastAcqMemBuffer()->save("E:\\test.bmp");
				//saveToFileJPEG(*(m_pSink->getLastAcqMemBuffer()), "E:\\test.jpg");
				bRet = true;
			}
			else
			{
				OutputDebugString(Err);
				LOG_ERRO("SingleSnap failed.(sn:%x) error:%s", m_iSN, Err);
			}
		}
	}

	return bRet;
}

bool CIMGCamBase::SetExposureTime(double dValue)
{
	bool bRet = false;
	if( m_Grabber.isDevValid() )
	{
		m_pPropertyItems = m_Grabber.getAvailableVCDProperties();
		if( m_pPropertyItems != NULL)
		{
			tIVCDPropertyInterfacePtr pPropertyInterface = NULL;
			tIVCDSwitchPropertyPtr pSwitchProperty = NULL;
			pPropertyInterface = m_pPropertyItems->findInterface( VCDID_Exposure, VCDElement_Auto, VCDInterface_Switch );
			if (pPropertyInterface != NULL)
			{
				pPropertyInterface->QueryInterface( pSwitchProperty );
				if (pSwitchProperty != NULL)
				{
					pSwitchProperty->setSwitch(false);
				}
			}

			tIVCDAbsoluteValuePropertyPtr pAbsValueProperty = NULL;
			pPropertyInterface = m_pPropertyItems->findInterface( VCDID_Exposure, VCDElement_Value, VCDInterface_AbsoluteValue );
			if (pPropertyInterface != NULL)
			{
				pPropertyInterface->QueryInterface( pAbsValueProperty );
				if (pAbsValueProperty != NULL)
				{
					pAbsValueProperty->setValue(dValue);
					bRet = true;
				}
			}
		}
	}

	return bRet;
}

bool CIMGCamBase::SetGain(double dValue)
{
	bool bRet = false;
	if( m_Grabber.isDevValid() )
	{
		m_pPropertyItems = m_Grabber.getAvailableVCDProperties();
		if( m_pPropertyItems != NULL)
		{
			tIVCDPropertyInterfacePtr pPropertyInterface = NULL;
			tIVCDSwitchPropertyPtr pSwitchProperty = NULL;
			pPropertyInterface = m_pPropertyItems->findInterface( VCDID_Gain, VCDElement_Auto, VCDInterface_Switch );
			if (pPropertyInterface != NULL)
			{
				pPropertyInterface->QueryInterface( pSwitchProperty );
				if (pSwitchProperty != NULL)
				{
					pSwitchProperty->setSwitch(false);
				}
			}

			tIVCDAbsoluteValuePropertyPtr pAbsValueProperty = NULL;
			pPropertyInterface = m_pPropertyItems->findInterface( VCDID_Gain, VCDElement_Value, VCDInterface_AbsoluteValue );
			if (pPropertyInterface != NULL)
			{
				pPropertyInterface->QueryInterface( pAbsValueProperty );
				if (pAbsValueProperty != NULL)
				{
					pAbsValueProperty->setValue(dValue);
					bRet = true;
				}
			}
		}
	}

	return bRet;
}

bool CIMGCamBase::SetTriggerMode(bool bSoft)
{
	bool bRet = false;
	if( m_Grabber.isDevValid() )
	{
		m_pPropertyItems = m_Grabber.getAvailableVCDProperties();
		if( m_pPropertyItems != NULL)
		{
			tIVCDPropertyInterfacePtr pInterface = NULL;
			pInterface = m_pPropertyItems->findInterface( VCDID_TriggerMode, VCDElement_ST, VCDInterface_Button );
			if (pInterface != NULL)
			{
				pInterface->QueryInterface( m_pSoftTrigger );
			}

			pInterface = m_pPropertyItems->findInterface( VCDID_TriggerMode, VCDElement_Value, VCDInterface_Switch );
			if (pInterface != NULL)
			{    
				pInterface->QueryInterface( m_pTriggerSwitch );
				if (m_pTriggerSwitch != NULL)
				{
					m_pTriggerSwitch->setSwitch( bSoft );
					m_bSoftTrigger = bSoft;
					bRet = true;
				}
			}
		}
	}
	
	return bRet;
}

void CIMGCamBase::SoftTrigger()
{
	if (m_pSoftTrigger != NULL)
	{
		m_pSoftTrigger->push();
	}
}

double CIMGCamBase::GetExposureTime()
{
	double dValue = 0;
	if( m_Grabber.isDevValid() )
	{
		m_pPropertyItems = m_Grabber.getAvailableVCDProperties();
		if( m_pPropertyItems != NULL)
		{
			tIVCDPropertyInterfacePtr pPropertyInterface = NULL;
			//tIVCDRangePropertyPtr pRangeInterface = NULL;
			//pPropertyInterface = m_pPropertyItems->findInterface( VCDID_Exposure, VCDElement_Value, VCDInterface_Range );
			//if (pPropertyInterface != NULL)
			//{
			//	pPropertyInterface->QueryInterface( pRangeInterface );
			//	if (pRangeInterface != NULL)
			//	{
			//		iValue = pRangeInterface->getValue();
			//	}
			//}

			tIVCDAbsoluteValuePropertyPtr pAbsValueProperty = NULL;
			pPropertyInterface = m_pPropertyItems->findInterface( VCDID_Exposure, VCDElement_Value, VCDInterface_AbsoluteValue );
			if (pPropertyInterface != NULL)
			{
				pPropertyInterface->QueryInterface( pAbsValueProperty );
				if (pAbsValueProperty != NULL)
				{
					dValue = pAbsValueProperty->getValue();
				}
			}
		}
	}
	return dValue;
}

double CIMGCamBase::GetGain()
{
	double dValue = 0;
	if( m_Grabber.isDevValid() )
	{
		m_pPropertyItems = m_Grabber.getAvailableVCDProperties();
		if( m_pPropertyItems != NULL)
		{
			tIVCDPropertyInterfacePtr pPropertyInterface = NULL;
			tIVCDAbsoluteValuePropertyPtr pAbsValueProperty = NULL;
			pPropertyInterface = m_pPropertyItems->findInterface( VCDID_Gain, VCDElement_Value, VCDInterface_AbsoluteValue );
			if (pPropertyInterface != NULL)
			{
				pPropertyInterface->QueryInterface( pAbsValueProperty );
				if (pAbsValueProperty != NULL)
				{
					dValue = pAbsValueProperty->getValue();
				}
			}
		}
	}
	return dValue;
}

bool CIMGCamBase::IsOpen()
{
	return m_bOpen;
}

//加载相机DLL
bool ImgCameraInitLibrary()
{
	if (!m_bInitLib)
	{
		m_bInitLib = DShowLib::InitLibrary();
		if (!m_bInitLib)
		{
			LOG_ERRO("IMG camera library init failed.");
			return false;
		}
	}

	return m_bInitLib;
}

void ImgCameraExitLibrary()
{
    DShowLib::ExitLibrary();
}

//获取相机SN列表
bool CIMGCamBase::GetCamList(vector<string> &vecSN, unsigned long MaxCnt)
{
    bool bRet = ImgCameraInitLibrary();
    if (bRet)
    {
        char szBuf[20] = {0};
        long long iSN = 0;

        Grabber tmpGrabber;
        Grabber::tVidCapDevListPtr pDevList = tmpGrabber.getAvailableVideoCaptureDevices();
        Grabber::tVidCapDevList::const_iterator iter = pDevList->begin();
        for (; iter != pDevList->end(); iter++)
        {
            bRet = iter->getSerialNumber(iSN);
            if (bRet)
            {
                ultoa((unsigned long)iSN, szBuf, 16);
                vecSN.push_back(szBuf);
            }

            if (MaxCnt == vecSN.size())
            {
                break;
            }
        }
    }

    return bRet;
}
