#include "dlArithmetic2.h"
#include "rapidjson/document.h"
#include "rapidjson/Prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "g12common.h"
#include "g12globalenv.h"
#include "filevariable/ifilevariable.h"
#include "../sendmessage.h"
#include "stringcommon.h"
#include "defect/idefectqualification.h"
#include "defect/idefectqualimanage.h"
#include "outputDataSave.h"

#include "callsdk.h"
#include "CLog.h"
#include <QFile>

using namespace rapidjson;
JMutex g_DlArithmeticMutex2;

#define  ENABLE_DV_INTERFACE  0// 屏蔽DV 的接口 0为屏蔽，1为使能
CDlArithmetic2 *CDlArithmetic2::m_pInstance = NULL;


CDlArithmetic2::CDlArithmetic2(int iStation)
{
	miStation = iStation;
	m_bIsHInit = false;

	m_bUnInitDeepLearn= false;

	m_bRoiDeepLearn = false;
	m_DVHandles.heat_handle = NULL;
	m_DVHandles.defect_handle = NULL;
	m_HLock.Init();
	//m_svmHandle = NULL;
    m_pEnableConfig = NULL;
	m_pIDefectQualification = false;
	m_pIDefectlist = false;
    m_bRun = false;

    m_bMultiThread = false;
    m_pProduct = false;
	memset(&m_hInputParam, 0, sizeof(m_hInputParam));
	memset(m_defectResult, 0, sizeof(dv::DefectResultInfo)*DV_DEFECTRESUL);
	memset(&m_rippleResultInfo, 0, sizeof(m_rippleResultInfo));
	memset(&m_DeformatrionInfo, 0, sizeof(dv::DefectResultInfo));


	if (NULL == m_pIDefectQualification)
	{
		//m_pIDefectQualification = (IDefectQualification*)GetSDKInstance("defectqualification", IDefectQualification::IDD_DEFECT_QULIFICATION);
		IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
		if (NULL != p)
		{
			m_pIDefectQualification = p->getDefectQualificationInstance(miStation);
		}
		IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
		if (p2 != NULL)
		{
			m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
			m_enNameVec    = p2->getDefectListInstance(0)->getDefectEnNameList();
			m_defectIndexVec = p2->getDefectListInstance(0)->getDefectIndexList();
			m_defectEnableVec = p2->getDefectListInstance(0)->getDefectEnableList();
			m_defectInfoList = p2->getDefectListInstance(0)->getDefectInfo();

		}
	}
}

CDlArithmetic2::~CDlArithmetic2()
{

    map<string, JThreadTP<CDlArithmetic2, string, void>* >::iterator it5;
    for (it5 = m_mapDefectLevelThreads.begin(); it5 != m_mapDefectLevelThreads.end(); it5++)
    {
        if (NULL != it5->second);
        {
            while (it5->second->IsRunning())
            {
                Sleep(5);
            }
            delete it5->second;
            it5->second = NULL;
        }
    }
    m_mapDefectLevelThreads.clear();
	unInitDeepLearnEnv();
}

IDlArithmetic * CDlArithmetic2::getInstance(int iStation)
{
    CISAPUGuard<JMutex> autoLock(g_DlArithmeticMutex2);
	if (NULL == m_pInstance)
	{
        m_pInstance = new CDlArithmetic2(iStation);
	}
	return m_pInstance;
}

void CDlArithmetic2::releaseInstance()
{
    CISAPUGuard<JMutex> autoLock(g_DlArithmeticMutex2);
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

int CDlArithmetic2::initDeepLearnEnv()
{
	CISAPUGuard<JMutex> autoLock(m_HLock);
	if (!(m_pEnableConfig->bImageDV || m_pEnableConfig->bImageSYDV) || m_bIsHInit)
	{
		return 0;
	}
	readConifg();
#if ENABLE_DV_INTERFACE
	int iRet = initLogger(m_dvConfig.log_flag, m_dvConfig.defect_log_file_path.c_str());
	if (iRet != dv::ErrorNone)
	{
		CSendMessage::GetInstance()->Send(QString("init DV log error! error code [%1]").arg(iRet), UILOG_INFO);
	}

	iRet = InitBJParam_FSK(&m_hInputParam,1, m_dvConfig.dv_product_path.c_str(),0);
	if (iRet != dv::ErrorNone)
	{
		CSendMessage::GetInstance()->Send(QString("station %1 initDVEnv error! error code [%2]").arg(miStation ).arg(iRet), UILOG_ERR);
		return -1;
	}

	m_bIsHInit = true;

	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("station %1 算法初始化完成").arg(miStation ));
#endif
	return 0;
}

void CDlArithmetic2::SetUnInitDeepLearn()
{
	m_bUnInitDeepLearn = true;
}

int CDlArithmetic2::unInitDeepLearnEnv()
{
	CISAPUGuard<JMutex> autoLock(m_HLock);
#if ENABLE_DV_INTERFACE

	if (m_bIsHInit && m_bUnInitDeepLearn)
	{
		m_bUnInitDeepLearn = false;
		m_bIsHInit = false;
		UnInitBJParam_FSK(&m_hInputParam, 1, m_defectResult, &m_rippleResultInfo);
		//UnInitBJParam(&m_hInputParam, m_defectResult, &m_rippleResultInfo, &m_DeformatrionInfo);
		//grading_uninit(&m_hGradingHandle);
		//grading_uninit(&m_hEdgeModel);
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("statin %1 算法反初始化完成").arg(miStation));
	}

#endif	
	return 0;
}

int CDlArithmetic2::updateDeepLearnEnv()
{
    int iRet = 0;
    char *json = NULL;
    QFile fileQ;

    do
    {
        fileQ.setFileName(m_dvConfig.dv_config_path.c_str());
        if (!fileQ.open(QIODevice::ReadOnly))
        {
            CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("配置文件读取失败(%1)").arg(m_dvConfig.dv_config_path.c_str()), UILOG_ERR);
            iRet = -1;
            break;
        }
    } while (0);
    if (fileQ.isOpen())
    {
        fileQ.close();
    }
    if (NULL != json)
    {
        delete[]json;
        json = NULL;
    }
    return 0;
}


InitDeepLearnState CDlArithmetic2::GetDeepLearnEnvState()
{
	if (m_pEnableConfig->bImageDV || m_pEnableConfig->bImageSYDV)
	{
		if (m_bIsHInit&& m_bIsHInit)
		{
			return DeepLearnInitOK;
		}
		return DeepLearnInit;
	}
	return DeepLearnNoNeed;
}

int CDlArithmetic2::runDeepLearn(CImageProduct* pProduct, uint64& uiElapsedTime)
{
	int iRet = 0;
    m_bRun = true;
	int iStationIndex = miStation;
	string strSceneName = SString::IntToStr((int)miStation);
#if ENABLE_DV_INTERFACE
			m_hInputParam.image = NULL;
            vector<string> vecEnableChannel;
            getEnabelChannle(vecEnableChannel, strSceneName);

            int iBlockCnt = 1;
            int iBlockLines = pProduct->getProductHeight(iStationIndex);
            int iChannelCnt = vecEnableChannel.size();
            int iOverCount = getCurrentProgramFileVariable(CAM_ATTRIBUTE_FILE_NAME)->getValue(IMAGE_PARAM_DUL_LINES, 0).toInt();
            int istep = iBlockLines - iOverCount;
            int iStartTime = 1;
		
           bool bProWhole = getCameraSetCfgFileVariable()->getValue(IMAGE_PARAM_IS_WHOLE, true).toBool();
            bProWhole = true;
			if (bProWhole)
            {
                istep = 256/8*7;
                iStartTime = 1;
            }

            for (int i = 0; i < iBlockCnt && m_bRun; i++)
            {

                int data_end = (i + 1 == iBlockCnt);
                int length = iChannelCnt;
                m_hInputParam.image = new dv::Image[length];

                for (int j = 0; j < iChannelCnt && m_bRun; j++)
                {
					int iChannelIndex = j;
					if (pProduct->getImageBuffer(QString::fromStdString(vecEnableChannel.at(j)), iStationIndex) != NULL)
					{
						stResImage stResImagetmp = pProduct->getImageBuffer(QString::fromStdString(vecEnableChannel.at(j)), iStationIndex)->rawData;
						m_hInputParam.image[iChannelIndex].data = stResImagetmp.img_data;
						m_hInputParam.image[iChannelIndex].width = stResImagetmp.img_width;
						m_hInputParam.image[iChannelIndex].height = stResImagetmp.img_height;
						m_hInputParam.image[iChannelIndex].channels = stResImagetmp.img_channels;
						m_hInputParam.image[iChannelIndex].width_step = stResImagetmp.img_width;
					}			          
                }

               // for (size_t k = 0; k < length; k++)
               // {
               //     setImageName((QString("%1_%2.bmp").arg(pProduct->getProductSn()).arg(k)).toStdString().c_str(), length, k);
               // }


                CSendMessage::GetInstance()->sendUpdateUiStus(QString::number(pProduct->getProductSn()), update_deal);
                CSendMessage::GetInstance()->Send(QString("product(%1)-station:%2 processImageDV start").arg(pProduct->getProductSn()).arg(miStation));

                // 切图
                dv::Rect rect[2] = { 0 };

                ///****************************************/
                rect[0].x = 0;
                rect[0].y = 0;
                rect[0].width = m_hInputParam.image[0].width;
                rect[0].height = m_hInputParam.image[0].height;;
                rect[1].x = 0;
                rect[1].y = 0;
                rect[1].width = m_hInputParam.image[3].width;
                rect[1].height = m_hInputParam.image[3].height;
                ///****************************************/

                uint64 iStartTime = JTime::CurrentMillisecond();
                try
                {

                    m_rippleResultInfo.rect_size = 0;
                    if(m_pEnableConfig->bImageSYDV)
                    {
						iRet = classifySilk_FSK(&m_hInputParam, 1, m_defectResult, &m_rippleResultInfo);
                    }
					else {
						return iRet;
					}
                }
                catch (...)
                {
                    CSendMessage::GetInstance()->Send(QString("product(%1)-%2 processImageDV exception   IRet = %3").arg(pProduct->getProductSn()).arg(i).arg(iRet), UILOG_ERR);
                    pProduct->setProcessErrCode(er_DV);

					return iRet;
                }
				//try
				//{
				//
				//}
				//catch (const std::exception&)
				//{
				//
				//}

                uint64 iEndTime = JTime::CurrentMillisecond();
                uint64 iElapsedTime = iEndTime - iStartTime;
                uiElapsedTime = iElapsedTime;
                CSendMessage::GetInstance()->Send(QString("h elapsed time : %2").arg(iElapsedTime));
				int iDefectNumner = 0;
                for (int i = 0; i < length; i++)
                {
                    CSendMessage::GetInstance()->Send(QString("channel %1 size %2").arg(i).arg(m_defectResult[i].len));
					iDefectNumner += m_defectResult[i].len;
                }

                if (m_hInputParam.image != NULL)
                {
                    delete []m_hInputParam.image;
                    m_hInputParam.image = NULL;
                }
				
                if (iRet != 0)
                {
                    if (iRet == dv::ErrorTimerOut)
                    {
                        CSendMessage::GetInstance()->Send(QString("product(%1)-%2 processImageDV TimeOut").arg(pProduct->getProductSn()).arg(i), UILOG_WARN);
                        pProduct->setProcessErrCode(er_DV_timeout);
						outputDataSave::GetInstance()->SaveTimeOutProduct(uiElapsedTime, pProduct->getProductSn(),"ErrorTimerOut" );//add20210505
						//
                    }
                    else
                    {
                        CSendMessage::GetInstance()->Send(QString("product(%1)-%2 processImageDV Error").arg(pProduct->getProductSn()).arg(iRet), UILOG_ERR);
                        pProduct->setProcessErrCode(er_DV);//非超时报错保存图片
						QString strms = QString(" DV Error Ret:%1").arg(iRet);
						outputDataSave::GetInstance()->SaveTimeOutProduct(uiElapsedTime, pProduct->getProductSn(), strms);//add20210505
                    }
					
					return iRet;
                }
				/*
                pProduct->m_cutRoiInfo[0].setX(m_hInputParam.roi_rect[0].x);
                pProduct->m_cutRoiInfo[0].setY(m_hInputParam.roi_rect[0].y);
                pProduct->m_cutRoiInfo[0].setWidth(m_hInputParam.roi_rect[0].width);
                pProduct->m_cutRoiInfo[0].setHeight(m_hInputParam.roi_rect[0].height);
                pProduct->m_cutRoiInfo[1].setX(m_hInputParam.roi_rect[1].x);
                pProduct->m_cutRoiInfo[1].setY(m_hInputParam.roi_rect[1].y);
                pProduct->m_cutRoiInfo[1].setWidth(m_hInputParam.roi_rect[1].width);
                pProduct->m_cutRoiInfo[1].setHeight(m_hInputParam.roi_rect[1].height);

                pProduct->m_curPolygon[0][0].clear();
                for (int i = 0; i < m_hInputParam.window_point_size[0]; i++)
                {
                    if (NULL != m_hInputParam.window_point[0])
                    {
                        pProduct->m_curPolygon[0][0] << QPoint(m_hInputParam.window_point[0][i].x, m_hInputParam.window_point[0][i].y);
                    }
                }
                pProduct->m_curPolygon[0][1].clear();
                for (int i = 0; i < m_hInputParam.edge_point_size[0]; i++)
                {
                    if (NULL != m_hInputParam.edge_point[0])
                    {
                        pProduct->m_curPolygon[0][1] << QPoint(m_hInputParam.edge_point[0][i].x, m_hInputParam.edge_point[0][i].y);
                    }
                }
                pProduct->m_curPolygon[1][0].clear();
                for (int i = 0; i < m_hInputParam.window_point_size[1]; i++)
                {
                    if (NULL != m_hInputParam.window_point[1])
                    {
                        pProduct->m_curPolygon[1][0] << QPoint(m_hInputParam.window_point[1][i].x, m_hInputParam.window_point[1][i].y);
                    }
                }
                pProduct->m_curPolygon[1][1].clear();
                for (int i = 0; i < m_hInputParam.edge_point_size[1]; i++)
                {
                    if (NULL != m_hInputParam.edge_point[1])
                    {
                        pProduct->m_curPolygon[1][1] << QPoint(m_hInputParam.edge_point[1][i].x, m_hInputParam.edge_point[1][i].y);
                    }
                }
				*/
                if (data_end)
                {
					IFileVariable* pFileVariable = getQualificationCfgFileVariable();
					if (pFileVariable != NULL)
					{
						bool bEn = pFileVariable->getValue(QUALIFICATION_ENABLE_CNT_ABORT).toBool();
						int iMaxNumber = pFileVariable->getValue(QUALIFICATION_CNT_ABORT).toInt();
						if (bEn && iDefectNumner >= iMaxNumber)
						{
							pProduct->m_stProductSummary.ResultFlag = res_MaxNumber;//缺陷过多直接退出等级计算
							CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("缺陷个数:%1! 缺陷过多!退出等级计算.").arg(iDefectNumner), UILOG_WARN);
							pProduct->setProcessErrCode(er_DV_timeout);//缺陷过多暂时使用算法超时错误码
							outputDataSave::GetInstance()->SaveTimeOutProduct(iDefectNumner, pProduct->getProductSn(),"MaxNumber");//add20210505
						}
					}

                }

				CSendMessage::GetInstance()->Send(QString("product(%1)-station:%2 processImageDV end").arg(pProduct->getProductSn()).arg(miStation));
            }
#else
      CSendMessage::GetInstance()->Send(QString::fromLocal8Bit(" station:%1 H算法已关闭").arg(miStation), UILOG_WARN);

#endif
        return iRet;
}

void CDlArithmetic2::stopDeepLearn()
{
    m_bRun = false;

}
void CDlArithmetic2::readConifg()
{
	IFileVariable *pFile = getDVCfgFileVariable();
	if (pFile != NULL)
	{

		m_dvConfig.dv_product_path = pFile->getValue(DV_CONFIG_FILE).toCString();
		m_dvConfig.dv_config_path = pFile->getValue(DV_CONFIG_FILE).toCString() + "/hconfig/config.json";
		m_dvConfig.defect_log_file_path = pFile->getValue(DV_H_LOG_PATH, "./log").toCString();
		pFile->bindValue(DV_H_LOG_PATH, &m_dvConfig.defect_log_file_path);

		//m_dvConfig.class_num = pFile->getValue(DV_CLASS_NUM, 6).toInt();
		//m_dvConfig.union_index = pFile->getValue(DV_UNION_CHANNEL_INDEX, 4).toUInt();
		//代表缺陷类别
		//for (int i = 0; i < m_dvConfig.class_num; i++)
		//{
		//	char szValueBuf[64] = { 0 };
		//	sprintf(szValueBuf, "%s%d", DV_DEFECT_INDEX_KEY, i);
		//	m_dvConfig.defect_index[i] = pFile->getValue(DV_CONFIG_GROUP_NAME, szValueBuf, i).toInt(i);
		//}
		
		//for (int i = 0; i < m_defectIndexVec.size(); ++i)
		//{
		//	m_dvConfig.defect_index[i] = m_defectIndexVec[i];
		//}
		//
		////pFile->bindValue(DV_TIMEOUT, &m_dvConfig.timeout);
		//
		//for (int iS = 0; iS < SEQUE_NUMBER; iS++)
		//{
		//	char szValueBuf[64] = { 0 };
		//	sprintf(szValueBuf, "%s%d", DV_PRO_IMAGE_TYPE, iS);
		//	m_dvConfig.image_type[iS] = (enInputImageType)pFile->getValue(DV_CONFIG_IMAGE_INPUT, szValueBuf, 0).toInt(0);
		//	memset(szValueBuf, 0, sizeof(szValueBuf));
		//	sprintf(szValueBuf, "%s%d", DV_INPUT_IMAGE_INDEX, iS);
		//	m_dvConfig.image_index[iS] = (enInputImageType)pFile->getValue(DV_CONFIG_IMAGE_INPUT, szValueBuf, iS).toInt(iS);
		//}
	}

	IFileVariable *pFileDefect = getDefectFileVariable();
	IFileVariable *pCameraFile = getCameraSetCfgFileVariable();
	double dCameramm = 0.0126;//相机精度
    string key = CAMERAPARAM;
    key = key + "_" + SString::IntToStr(miStation);
	if (pCameraFile)
	{
        dCameramm = pCameraFile->getValue(key,CAMERAPARAM_MOVEUNIT).toDouble();
	}
	double dCamerammA = dCameramm*dCameramm;
	//if (pFileDefect)
	//{
	//	m_dvConfig.enable[0] = false;
	//	m_dvConfig.enable[1] = pFileDefect->getValue(LIGHTLEAK_ENABLE).toBool();
	//	m_dvConfig.enable[2] = pFileDefect->getValue(EXCESSLNK_ENABLE).toBool();
	//	m_dvConfig.enable[3] = pFileDefect->getValue(DISCOLOURATION_ENABLE).toBool();
	//	m_dvConfig.enable[4] = pFileDefect->getValue(SAWEGDGE_ENABLE).toBool();
	//	m_dvConfig.enable[5] = pFileDefect->getValue(SMIRCH_ENABLE).toBool();
	//	m_dvConfig.enable[6] = pFileDefect->getValue(ACCULNK_ENABLE).toBool();
	//	m_dvConfig.enable[7] = pFileDefect->getValue(DISTHOLE_ENABLE).toBool();
	//	m_dvConfig.enable[8] = pFileDefect->getValue(MISPRINT_ENABLE).toBool();
	//	m_dvConfig.enable[9] = pFileDefect->getValue(SCRATCHES_ENABLE).toBool();
	//	m_dvConfig.enable[10] = pFileDefect->getValue(CHIPS_ENABLE).toBool();
	//	m_dvConfig.enable[11] = pFileDefect->getValue(DENT_ENABLE).toBool();
	//}
}
void CDlArithmetic2::readDVConifg(stEnableConfig* pEnableConfig)
{
	m_pEnableConfig = pEnableConfig;
	return;
	/*
    IFileVariable *pFile = getDVCfgFileVariable();
    if (pFile != NULL)
    {
		
		m_dvConfig.dv_product_path = pFile->getValue(DV_CONFIG_FILE).toCString();
		m_dvConfig.dv_config_path = pFile->getValue(DV_CONFIG_FILE).toCString() + "/hconfig/config.json";
        m_dvConfig.defect_log_file_path = pFile->getValue(DV_H_LOG_PATH, "./log").toCString();
        pFile->bindValue(DV_H_LOG_PATH, &m_dvConfig.defect_log_file_path);

        m_dvConfig.class_num = pFile->getValue(DV_CLASS_NUM, 6).toInt();
        m_dvConfig.union_index = pFile->getValue(DV_UNION_CHANNEL_INDEX, 4).toUInt();
        //代表缺陷类别
        for (int i = 0; i < m_dvConfig.class_num; i++)
        {
            char szValueBuf[64] = { 0 };
            sprintf(szValueBuf, "%s%d", DV_DEFECT_INDEX_KEY, i);
            m_dvConfig.defect_index[i] = pFile->getValue(DV_CONFIG_GROUP_NAME, szValueBuf, i).toInt(i);
        }

        //pFile->bindValue(DV_TIMEOUT, &m_dvConfig.timeout);

        for (int iS = 0; iS < SEQUE_NUMBER; iS++)
        {
            char szValueBuf[64] = { 0 };
            sprintf(szValueBuf, "%s%d", DV_PRO_IMAGE_TYPE, iS);
            m_dvConfig.image_type[iS] = (enInputImageType)pFile->getValue(DV_CONFIG_IMAGE_INPUT, szValueBuf, 0).toInt(0);
            memset(szValueBuf, 0, sizeof(szValueBuf));
            sprintf(szValueBuf, "%s%d", DV_INPUT_IMAGE_INDEX, iS);
            m_dvConfig.image_index[iS] = (enInputImageType)pFile->getValue(DV_CONFIG_IMAGE_INPUT, szValueBuf, iS).toInt(iS);
        }
    }

    IFileVariable *pFileDefect = getDefectFileVariable();
    IFileVariable *pCameraFile = getCameraSetCfgFileVariable();
    double dCameramm = 0.0126;//相机精度
    if (pCameraFile)
    {
        dCameramm = pCameraFile->getValue(CAMERAPARAM_MOVEUNIT).toDouble();
    }
    double dCamerammA = dCameramm*dCameramm;
    if (pFileDefect)
    {
        m_dvConfig.enable[0] = false;
        m_dvConfig.enable[1] = pFileDefect->getValue(LIGHTLEAK_ENABLE).toBool();
        m_dvConfig.enable[2] = pFileDefect->getValue(EXCESSLNK_ENABLE).toBool();
        m_dvConfig.enable[3] = pFileDefect->getValue(DISCOLOURATION_ENABLE).toBool();
        m_dvConfig.enable[4] = pFileDefect->getValue(SAWEGDGE_ENABLE).toBool();
        m_dvConfig.enable[5] = pFileDefect->getValue(SMIRCH_ENABLE).toBool();
        m_dvConfig.enable[6] = pFileDefect->getValue(ACCULNK_ENABLE).toBool();
        m_dvConfig.enable[7] = pFileDefect->getValue(DISTHOLE_ENABLE).toBool();
        m_dvConfig.enable[8] = pFileDefect->getValue(MISPRINT_ENABLE).toBool();
        m_dvConfig.enable[9] = pFileDefect->getValue(SCRATCHES_ENABLE).toBool();
        m_dvConfig.enable[10] = pFileDefect->getValue(CHIPS_ENABLE).toBool();
        m_dvConfig.enable[11] = pFileDefect->getValue(DENT_ENABLE).toBool();
    }
	*/
}
int CDlArithmetic2::getDefectType(int iType, bool &bSeverity)
{
	//缺陷索引 不能直接使用排序好的索引，比如有断层不连续的索引会崩溃的 
	// 1.2.3.4.5.6.20.21.30......
	int iNameIndex = 0;//抹默认 0 
					   //1.先找到缺陷名字
	string strDecfectName = "";
	bSeverity = false;
	for (size_t i = 0; i < m_defectInfoList.size(); i++)
	{
		if (m_defectInfoList.at(i).index == iType)
		{
			strDecfectName = m_defectInfoList.at(i).name;
			bSeverity = m_defectInfoList.at(i).severity>0?true:false;
			break;
		}
	}
	//2.由名找到对应vec的序号--名字的序号与索引序号对应
	for (int iCheck = 0; iCheck < m_enNameVec.size(); iCheck++)
	{
		if (m_enNameVec.at(iCheck) == strDecfectName)
		{
			iNameIndex = iCheck;
			break;
		}
	}
	return iNameIndex;
}
void CDlArithmetic2::DvDefectToDefectFeature(CImageProduct* pProduct, dv::DefectAttr Attr, string strchannel, int iDhLevel /*= 0*/)
{

    string channel = strchannel;
    double dCamerammArea = 0.0;
	double dCameramm = 0.0126;
	int iStation = 0;
    IFileVariable *pCameraFile = getCameraSetCfgFileVariable();
    string key = CAMERAPARAM;
    key = key + "_" + SString::IntToStr(miStation);
    if (pCameraFile)
    {
        dCameramm = pCameraFile->getValue(key,CAMERAPARAM_MOVEUNIT).toDouble();

        iStation = miStation;
        channel += QString::number(miStation).toStdString();

        dCamerammArea = dCameramm*dCameramm;
    }
    int iCameraIndex = 0;
    int iType = 0;
    //getSqueLightTypeAndCameraIndex(channel, iType, iCameraIndex);

    stDefectFeature *mark = new stDefectFeature;
    mark->strOriginate = "DV";
    mark->strChannelName = channel;
	//if (NULL != pProduct->m_pro2DCode)
	//{
	//	st2DcodeInfo* ptmp2Dcode = pProduct->m_pro2DCode;
	//	mark->str2Dcode = ptmp2Dcode->str2dCode;
	//}
    mark->sn = to_string(pProduct->getProductSn());
    mark->iChannelType = 0;
    mark->iCameraIndex = iCameraIndex;
    mark->iMarkIndex = pProduct->pDefectStrutVec->size(); 
	bool bSty = false;//是否重叠的索引
	int  iIndexVec = getDefectType(Attr.type, bSty);//对应排序的位置

	mark->iDefectType = m_defectIndexVec.at(iIndexVec);
	mark->strDefectName = m_defectNameVec[iIndexVec];
	if (bSty)
	{
		mark->strDefectName = m_defectNameVec[mark->iDefectType];
	}
    mark->iDvDefectType = Attr.type;
    //mark->iposition  = Attr.position;
	mark->iposition = Attr.position == 0 ? 0 : 1;;
    mark->iShape = Attr.shape+1;
	mark->iStation = iStation;
	mark->regionPos = Attr.grid_num+1;

    if (mark->iShape >= DEFECT_SHAPE_SIZE || mark->iShape < 0)
    {
        mark->iShape = 0;
    }
    mark->iH_DefectLevel = iDhLevel;

    mark->defectWidth = Attr.width;
    mark->defectHeight = Attr.height;

	mark->dmm_defectHeight = (double)mark->defectHeight*dCameramm;
	mark->dmm_defectWidth = (double)mark->defectWidth*dCameramm;
    mark->dWide = Attr.rect.width;
    mark->dHigh = Attr.rect.height;
    mark->SkeletonLength = mark->dWide > mark->dHigh ? mark->dWide : mark->dHigh;
    mark->iSize = Attr.num;
    mark->posx = Attr.rect.x + mark->dWide / 2;
    mark->posy = Attr.rect.y + mark->dHigh / 2;
    mark->contrast = Attr.level;
    mark->fscore = Attr.score;
    mark->fdetect_score = Attr.detect_score;
    mark->dSizeArea = mark->iSize*dCamerammArea;
    mark->idetect_flag = Attr.detect_flag;
    mark->DefectBAvgGray = Attr.average_gray;

	string channelLevel = miStation == 0 ? strchannel : channel;
	getDefectLevel(channelLevel, mark);//等级计算//
	

	m_DefectLevelLock.Lock();
#if ENABLE_DV_INTERFACE
#else
	//mark->iDefectLevel = 7;
#endif
    pProduct->pDefectStrutVec->push_back(mark);
    m_DefectLevelLock.Unlock();

    
}
void CDlArithmetic2::DvDefectToDefectFeature(CImageProduct* pProduct, stDefectFeature DefectFeature, string strchannel)
{

	string channel = strchannel;
	double dCamerammArea = 0.0;
	IFileVariable *pCameraFile = getCameraSetCfgFileVariable();
    string key = CAMERAPARAM;
    key = key + "_" + SString::IntToStr(miStation);
	if (pCameraFile)
	{
        double dCameramm = pCameraFile->getValue(key,CAMERAPARAM_MOVEUNIT).toDouble();
		dCamerammArea = dCameramm*dCameramm;
	}
	int iCameraIndex = 0;
	int iType = 0;
	getSqueLightTypeAndCameraIndex(channel, iType, iCameraIndex);

	stDefectFeature *mark = new stDefectFeature;
	mark->strOriginate = "DV";
	mark->strChannelName = strchannel;
	mark->sn = to_string(pProduct->getProductSn());
	mark->iChannelType = 0;
	mark->iCameraIndex = iCameraIndex;
	mark->iMarkIndex = pProduct->pDefectStrutVec->size();
	mark->iDefectType = DefectFeature.iDefectType;
	mark->iposition = 0;
	mark->iShape = 2;

	mark->iDefectType = DefectFeature.iDefectType;
	mark->strDefectName = m_defectNameVec[mark->iDefectType];
	

	mark->defectWidth  = DefectFeature.dWide;
	mark->defectHeight = DefectFeature.dHigh;
	mark->dWide = DefectFeature.dWide;
	mark->dHigh = DefectFeature.dHigh;
	mark->SkeletonLength = mark->dWide > mark->dHigh ? mark->dWide : mark->dHigh;
	mark->iSize = 1;
	mark->posx = DefectFeature.posx + mark->dWide / 2;
	mark->posy = DefectFeature.posy + mark->dHigh / 2;
	mark->contrast =1;
	mark->fscore = DefectFeature.fscore;
	mark->fdetect_score = DefectFeature.fdetect_score;
	mark->dSizeArea = mark->iSize*dCamerammArea;
	mark->idetect_flag = 0;
	mark->DefectBAvgGray = 0;
	//传统算法检出来的压痕 直接打到7级//20200703//
	if (DefectFeature.iDefectType == en_Impress)
	{
		mark->iH_DefectLevel = 7;
	}
	getDefectLevel(strchannel, mark);//等级计算//
	m_DefectLevelLock.Lock();
	pProduct->pDefectStrutVec->push_back(mark);
	m_DefectLevelLock.Unlock();
}
bool CDlArithmetic2::getDefectLevel(stDefectFeature *pDefectRes)
{
    if (NULL == pDefectRes || m_pIDefectQualification == NULL || !m_pEnableConfig->bDefectLevel)
    {
        return false;
    }
    int iSize = m_defectNameVec.size();
    int iDefectType = pDefectRes->iDefectType;
    if (iDefectType > iSize || iDefectType <= 0)
    {
        return false;
    }
    return m_pIDefectQualification->getDefectSeverityLevel(iDefectType, pDefectRes);
}

void CDlArithmetic2::ThreadDefectLevelProcess(string param)
{

        // 0~7 //A  --H
        int iChannel = 0 ;
        if ("SEQUENCE_A" == param)
        {
            iChannel = 0;
        }
        else if ("SEQUENCE_B" == param)
        {
            iChannel = 1;
        }
        else if ("SEQUENCE_C" == param)
        {
            iChannel = 2;
        }
        else if ("SEQUENCE_D" == param)
        {
            iChannel = 3;
        }
		else if ("SEQUENCE_E" == param)
		{
			iChannel = 4;
		}
		else if ("SEQUENCE_F" == param)
		{
			iChannel = 5;
		}
		else if ("SEQUENCE_G" == param)
		{
			iChannel = 6;
		}
		else if ("SEQUENCE_H" == param)
		{
			iChannel = 7;
		}
        else 
        {
            return;
        }

#if ENABLE_DV_INTERFACE
        if (m_pInstance->m_defectResult[iChannel].defectAttr != NULL && m_pEnableConfig->bImageSYDV)
        {

            for (int k = 0; k < m_defectResult[iChannel].len; k++)
            {
                m_pInstance->DvDefectToDefectFeature(m_pProduct, m_defectResult[iChannel].defectAttr[k], param, 0);
            }
        }

#else 


#endif

}

bool CDlArithmetic2::getDefectLevel(string strChannel, stDefectFeature *pDefectRes)
{
	if (NULL == pDefectRes || m_pIDefectQualification == NULL || !m_pEnableConfig->bDefectLevel)
	{
		return false;
	}
	int iSize = m_defectNameVec.size();
	int iDefectType = pDefectRes->iDefectType;
	if (iDefectType > iSize || iDefectType <= 0)
	{
		return false;
	}
    return m_pIDefectQualification->getDefectSeverityLevel(strChannel, iDefectType, pDefectRes);
}

void CDlArithmetic2::startDefectLevel(CImageProduct* pProduct)
{
	IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (p2 != NULL)
	{
		m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
		m_defectIndexVec = p2->getDefectListInstance(0)->getDefectIndexList();
		m_defectEnableVec = p2->getDefectListInstance(0)->getDefectEnableList();
		m_enNameVec = p2->getDefectListInstance(0)->getDefectEnNameList();
		m_defectInfoList = p2->getDefectListInstance(0)->getDefectInfo();
	}
	if (!m_pEnableConfig->bDefectLevel)
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("未使能缺陷分级"), UILOG_WARN);
		return;
	}
    m_pProduct = pProduct;
#if ENABLE_DV_INTERFACE
    m_bMultiThread = false;
    IFileVariable *pFile = getDVCfgFileVariable();
    if (pFile != NULL)
    {
        m_bMultiThread = pFile->getValue(DV_ARITHMETIC_MULTITHREAD, 0).toBool();
    }
    string sense = SString::IntToStr((int)miStation);
    vector<string> vect;
    getEnabelChannle(vect, sense);
    vector<JThreadTP<CDlArithmetic2, string, void>*> runningThreads;

    for (int i = 0; i < vect.size(); i++)
    {
        string enChannel = vect.at(i);
        if(!m_bMultiThread)
        {
            ThreadDefectLevelProcess(enChannel);
        }
        else
        {
            JThreadTP<CDlArithmetic2, string, void>* pthread = NULL;
            if (m_mapDefectLevelThreads.find(enChannel) == m_mapDefectLevelThreads.end())
            {
                pthread = new JThreadTP<CDlArithmetic2, string, void>;
                m_mapDefectLevelThreads[enChannel] = pthread;
            }

            pthread = m_mapDefectLevelThreads[enChannel];
            if (!pthread->IsRunning())
            {
                pthread->StartThread(this, &CDlArithmetic2::ThreadDefectLevelProcess, enChannel);
                runningThreads.push_back(pthread);
            }
        }
    }
	//
    if(m_bMultiThread)
    {
        do
        {
            int  icnt = 0;
            for(int i =0 ;i <runningThreads.size();i++)
            {
                if(!runningThreads.at(i)->IsRunning())
                {
                    icnt++;
                }
            }
            if(icnt == runningThreads.size())
            {
                break;
            }
            else
            {
                Sleep(10);
            }
        }while(true);
    }

#else
	int iCount = 50;
	for (int i = 0; i < iCount; i++)
	{
		dv::DefectAttr result;
		result.type = rand() % 32;
		result.rect.x = rand() % 8191;
		result.rect.y = rand() % 12800;
		result.rect.width = rand() % (8191 - result.rect.x);
		result.rect.height = rand() % (12800 - result.rect.y);
		result.position = rand() % 2;
		result.grid_num = i%8;
		result.level = rand() % 200;
		result.shape = i % 3;
		int ic = rand() % 4;
		string str = "SEQUENCE_A" ;
		switch (ic)
		{
		case 1:
			str = "SEQUENCE_B";
			break;
		case 2:
			str = "SEQUENCE_C";
			break;
		case 3:
			str = "SEQUENCE_D";
			break;
		default:
			break;
		}
		DvDefectToDefectFeature(pProduct, result, str);
	}
#endif
}

