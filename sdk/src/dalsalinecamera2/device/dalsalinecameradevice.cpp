#include "dalsalinecameradevice.h"
#include "rtdefine.h"
#include "trace.h"
#include "vscameratools/camera/ilinecameradevice.h"
#include "sdkcore.h"
#include "jdir.h"
#include "tinyxml.h"
#include "vscameratools/camera/icamerafactory.h"

dalsalinecameradevice::dalsalinecameradevice()
    : m_AcqDevice(NULL)
    , m_pFeature(NULL)
    , m_pOtherCamDevice(NULL)
    , m_bIsUnknown(false)
    , m_strCamPluginName("plugin")
{
    //readConfig();
}

bool dalsalinecameradevice::init(const char *szDeviceName)
{
    m_strDeviceName = szDeviceName;
    return true;
}

bool dalsalinecameradevice::openDevice(SapLocation location, const char *configFile)
{
    bool bRet = false;
    do 
    {
        RETURN_V_IF(m_bIsUnknown, true); //相机未知不操作相机直接返回正常
        RETURN_V_IF(m_strDeviceName.empty(), false); //设备名称枚举不到时打开相机失败

        if (NULL != m_pOtherCamDevice)
        {
            bRet = m_pOtherCamDevice->openDevice();
            break;
        }
        m_AcqDevice = new SapAcqDevice(m_strDeviceName.c_str());
        if (NULL == m_AcqDevice || !m_AcqDevice->Create())
            break;
        m_pFeature = new SapFeature(m_strDeviceName.c_str());
        if (NULL == m_pFeature || !m_pFeature->Create())
            break;
        bRet = true;
    } while (0);
    return bRet;
}

bool dalsalinecameradevice::closeDevice()
{
    bool bRet = false;
    do 
    {
        RETURN_V_IF(m_bIsUnknown, true); //相机未知不操作相机直接返回正常
        if (NULL != m_pOtherCamDevice)
        {
            bRet = m_pOtherCamDevice->closeDevice();
            break;
        }
        if (NULL != m_AcqDevice && *m_AcqDevice)
        {
            if (*m_AcqDevice)
            {
                m_AcqDevice->Destroy();
            }
			//Win10 下 delete 会崩溃
            //delete m_AcqDevice;
            //m_AcqDevice = NULL;
        }
        bRet = true;
    } while (0);
    return bRet;
}

bool dalsalinecameradevice::IsOpen()
{
    RETURN_V_IF(m_bIsUnknown, true); //相机未知不操作相机直接返回正常
    return true;
}

bool dalsalinecameradevice::setTDIFeature(bool open)
{
    bool bRet = false;
    do 
    {
        RETURN_V_IF(m_bIsUnknown, true); //相机未知不操作相机直接返回正常
        if (NULL != m_pOtherCamDevice)
        {
            bRet = m_pOtherCamDevice->setTDIFeature(open);
            break;
        }
		if (!open)
		{
			int value = 0;
			m_AcqDevice->GetFeatureValue("sensorTDIStagesSelection", &value);
			if (value != 1)
			{
				m_AcqDevice->SetFeatureValue("sensorTDIStagesSelection", 1);  //TDI设置为1,防止干扰到多通道采图 1是关，2是开
			}
		}
		else
		{
			int value = 0;
			m_AcqDevice->GetFeatureValue("sensorTDIStagesSelection", &value);
			if (value != 2)
			{
				m_AcqDevice->SetFeatureValue("sensorTDIStagesSelection", 2);  //TDI设置 1是关，2是开
			}
		}

        bRet = true;
    } while (0);
    return true;
}

bool dalsalinecameradevice::SetGain(double GainValue)
{
    bool bRet = false;
    try
    {
        do
        {
            RETURN_V_IF(m_bIsUnknown, true); //相机未知不操作相机直接返回正常
            if (NULL != m_pOtherCamDevice)
            {
                bRet = m_pOtherCamDevice->SetGain(GainValue);
                break;
            }

            BREAK_IF(NULL == m_AcqDevice);
            double dMin = 0;
            double dMax = 99999;
            if (NULL != m_pFeature && m_AcqDevice->GetFeatureInfo("Gain", m_pFeature))
            {
                m_pFeature->GetMin(&dMin);
                m_pFeature->GetMax(&dMax);
            }
            BREAK_IF(GainValue < dMin || GainValue > dMax);
            double curValue;
            BREAK_IF(!m_AcqDevice->GetFeatureValue("Gain", &curValue));
            if (abs(curValue - GainValue) > 0.01)
            {
                BREAK_IF(!m_AcqDevice->SetFeatureValue("Gain", GainValue));
            }
            bRet = true;
        } while (0);
    }
    catch (...)
    {
        LOG_WARN("===============[cam]set gain param failed.===============");
        bRet = false;
    }

    //m_CameraParam.gain = GainValue;
    return bRet;
}

bool dalsalinecameradevice::SetExposureTime(double ExpTimeValue)
{
    bool bRet = false;
    try
    {
        do
        {
            RETURN_V_IF(m_bIsUnknown, true); //相机未知不操作相机直接返回正常
            if (NULL != m_pOtherCamDevice)
            {
                bRet = m_pOtherCamDevice->SetCameraFrequency(ExpTimeValue);
                break;
            }

            BREAK_IF(NULL == m_AcqDevice);

            double dMin = 0;
            double dMax = 99999;
            if (NULL != m_pFeature && m_AcqDevice->GetFeatureInfo("ExposureTime", m_pFeature))
            {
                m_pFeature->GetMin(&dMin);
                m_pFeature->GetMax(&dMax);
            }
            BREAK_IF(ExpTimeValue < dMin || ExpTimeValue > dMax);
            double curValue;
            BREAK_IF(!m_AcqDevice->GetFeatureValue("ExposureTime", &curValue));
            if (abs(curValue - ExpTimeValue) > 0.01)
            {
                BREAK_IF(!m_AcqDevice->SetFeatureValue("ExposureTime", ExpTimeValue));
            }
            bRet = true;
        } while (0);
    }
    catch (...)
    {
        try
        {
            if (NULL != m_pFeature && m_AcqDevice->GetFeatureInfo("ExposureTime", m_pFeature))
            {
                double curValue;
                if (m_AcqDevice->GetFeatureValue("ExposureTime", &curValue))
                {
                    bRet = true;
                }
            }
        }
        catch (...)
        {
        }
        LOG_WARN("===============[cam]set ExposureTime param failed.===============");
        bRet = false;
    }
    return bRet;
}

bool dalsalinecameradevice::SetCameraFrequency(unsigned int Freq)
{
    bool bRet = false;
    if (Freq < 1)
    {
        return false;
    }
    try
    {
        do
        {
            RETURN_V_IF(m_bIsUnknown, true); //相机未知不操作相机直接返回正常
            if (NULL != m_pOtherCamDevice)
            {
                bRet = m_pOtherCamDevice->SetCameraFrequency(Freq);
                break;
            }

            SapFeature::AccessMode myAccess;
            BREAK_IF(NULL == m_AcqDevice);
            BREAK_IF(!m_pFeature->GetAccessMode(&myAccess));
            BREAK_IF(CORFEATURE_VAL_ACCESS_MODE_RW != myAccess)
                unsigned int iMin = 0;
            unsigned int iMax = 99999;
            if (NULL != m_pFeature && m_AcqDevice->GetFeatureInfo("AcquisitionLineRate", m_pFeature))
            {
                m_pFeature->GetMin(&iMin);
                m_pFeature->GetMax(&iMax);
            }
            BREAK_IF(Freq < iMin || Freq > iMax);
            unsigned int curValue = 0;
            BREAK_IF(!m_AcqDevice->GetFeatureValue("AcquisitionLineRate", &curValue));
            if (curValue != Freq)
            {
                BREAK_IF(!m_AcqDevice->SetFeatureValue("AcquisitionLineRate", Freq));
                //m_CameraParam.grabFreq = Freq;
            }
            bRet = true;
        } while (0);
    }
    catch (...)
    {
        LOG_WARN("===============[cam]set AcquisitionLineRate param failed.===============");
        bRet = false;
    }
    return bRet;
}

bool dalsalinecameradevice::SetTriggerMode(bool triger)
{
    bool bRet = false;
    do
    {
        RETURN_V_IF(m_bIsUnknown, true); //相机未知不操作相机直接返回正常
        if (NULL != m_pOtherCamDevice)
        {
            bRet = m_pOtherCamDevice->SetTriggerMode(triger);
            break;
        }
        BOOL bIsTriger;
        int emTrigger;
        //BREAK_IF(!m_Acq->GetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, &bIsTriger));
        BREAK_IF(!m_AcqDevice->GetFeatureValue("TriggerMode", &emTrigger));
        if (emTrigger != triger/* || bIsTriger != triger*/)
        {
            if (triger)
            {
                BREAK_IF(!m_AcqDevice->SetFeatureValue("TriggerMode", 1));
                //BREAK_IF(!m_Acq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, 1));
            }
            else
            {
                BREAK_IF(!m_AcqDevice->SetFeatureValue("TriggerMode", 0));
                //BREAK_IF(!m_Acq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, 0));
            }
            //m_CameraParam.bExtTrigger = triger;
        }
        bRet = true;
    } while (0);
    return bRet;
}

bool dalsalinecameradevice::setCamPluginId(string strCamPluginID)
{
    if (0 == m_strCamPluginName.compare(strCamPluginID))
    {
        return true;
    }
    m_strCamPluginName = strCamPluginID;
    if (0 != m_strCamPluginName.compare("dalsalinecamera") && !m_strCamPluginName.empty())
    {
        m_pOtherCamDevice = (ILineCameraDevice *)sdkcore::queryPriInterface(strCamPluginID.c_str(), ILineCameraDevice::IID_LINE_CAMERA_DEVICE);
        if (NULL != m_pOtherCamDevice)
        {
            m_pOtherCamDevice->init(m_strDeviceName.c_str());
        }
        else
        {
            m_bIsUnknown = true;
        }
    }
    return true;
}

//bool dalsalinecameradevice::readConfig()
//{
//    int index = 0;
//    int id = 0;
//    string strPluginName = "";
//    if (index > 0)
//    {
//        m_pOtherCamDevice = (ILineCameraDevice *)sdkcore::queryPriInterface(strPluginName.c_str(), id);
//    }
//    return true;
//}
