#include "dlArithmetic0.h"
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
#include "bjcv.h"
#include <QFile>
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"

using namespace rapidjson;
JMutex g_DlArithmeticMutex0;

#define  ENABLE_DV_INTERFACE  0// 屏蔽DV 的接口 0为屏蔽，1为使能
CDlArithmetic0 *CDlArithmetic0::m_pInstance = NULL;


CDlArithmetic0::CDlArithmetic0(int iStation)
{
	miStation = iStation;
	m_bIsHInit = false;
	m_bUnInitDeepLearn = false;
    m_bRoiDeepLearn = false;

	m_HLock.Init();
	//m_svmHandle = NULL;
    m_pEnableConfig = NULL;
	m_pIDefectQualification = false;
	m_pIDefectlist = false;
    m_bRun = false;
    m_bMultiThread = false;
    m_pProduct = false;
	memset(&m_results_ptr, 0, sizeof(m_results_ptr));
	
	if (NULL == m_pIDefectQualification)
	{
		//m_pIDefectQualification = (IDefectQualification*)GetSDKInstance("defectqualification", IDefectQualification::IDD_DEFECT_QULIFICATION);
		IDefectQualimanage* p = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
		if (NULL != p)
		{
			m_pIDefectQualification = p->getDefectQualificationInstance(miStation);
		}
	}
	IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p2)
	{
		m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
		m_enNameVec = p2->getDefectListInstance(0)->getDefectEnNameList();
		m_defectIndexVec = p2->getDefectListInstance(0)->getDefectIndexList();
		m_defectEnableVec = p2->getDefectListInstance(0)->getDefectEnableList();
		m_defectInfoList = p2->getDefectListInstance(0)->getDefectInfo();
		m_defectIndex = p2->getDefectListInstance(0)->getBjAiDefectNameToIndexList();
	}
}

CDlArithmetic0::~CDlArithmetic0()
{

    map<string, JThreadTP<CDlArithmetic0, string, void>* >::iterator it5;
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

IDlArithmetic * CDlArithmetic0::getInstance(int iStation)
{
    CISAPUGuard<JMutex> autoLock(g_DlArithmeticMutex0);
	if (NULL == m_pInstance)
	{
        m_pInstance = new CDlArithmetic0(iStation);
	}
	return m_pInstance;
}

int CDlArithmetic::getDefectIndex(string name)
{
	if (m_defectIndex.size() <= 0)
	{
		return 0;
	}
	for (int i = 0; i < m_defectIndex.size(); i++)
	{
		if (m_defectIndex[i].first == name)
		{
			return m_defectIndex[i].second;
		}
	}
	return 0;
}
void CDlArithmetic0::releaseInstance()
{
    CISAPUGuard<JMutex> autoLock(g_DlArithmeticMutex0);
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
int CDlArithmetic0::initDeepLearnEnv()
{
	CISAPUGuard<JMutex> autoLock(m_HLock);
	if (!(m_pEnableConfig->bImageDV || m_pEnableConfig->bImageSYDV) || m_bIsHInit)
	{
		return 0;
	}
	if (m_bIsHInit)
	{
		return 0;
	}
	IFileVariable *pFile = getDVCfgFileVariable();
	if (pFile != NULL)
	{

		m_dvConfig.dv_product_path = pFile->getValue(DV_CONFIG_FILE).toCString();
		m_dvConfig.dv_config_path = m_dvConfig.dv_product_path + "/hconfig/config.json";
	}
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("AI模型:%1").arg(m_dvConfig.dv_product_path.c_str()));

#if ENABLE_DV_INTERFACE
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("initAI 算法初始化开始"));
    string  strPath = m_dvConfig.dv_product_path;
    bjcv::ErrorCode err = bjcv::init_model(strPath.c_str(), &model_handle_ptr);
    if(bjcv::ErrorCode::Success != err)
    {
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("initAI失败(%1)").arg((int)err));
        return -1;
    }

    CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("initAI 算法初始化完成"));
#else
    CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("station %1 算法已经屏蔽").arg(miStation ));
#endif
	m_bIsHInit = true;
	return 0;
}

void CDlArithmetic0::SetUnInitDeepLearn()
{
	if (m_bIsHInit)
	{
		m_bUnInitDeepLearn = true;
	}
}
int CDlArithmetic0::updateDeepLearnEnv()
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

int CDlArithmetic0::unInitDeepLearnEnv()
{
	CISAPUGuard<JMutex> autoLock(m_HLock);
#if ENABLE_DV_INTERFACE

	if (m_bIsHInit && m_bUnInitDeepLearn)
	{
		m_bUnInitDeepLearn = false;
		m_bIsHInit = false;
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("unInitAI开始"));
        bjcv::ErrorCode  err = bjcv::uninit_model(&model_handle_ptr);
        if (bjcv::ErrorCode::Success != err)
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("unInitAI失败(%1)").arg((int)err));
		}
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("unInitAI完成"));
	}

#endif	
	return 0;
}
InitDeepLearnState CDlArithmetic0::GetDeepLearnEnvState()
{
	if (m_pEnableConfig->bImageDV || m_pEnableConfig->bImageSYDV)
	{
		if (m_bIsHInit)
		{
			return DeepLearnInitOK;
		}
		return DeepLearnInit;
	}
	return DeepLearnNoNeed;
}
void CDlArithmetic0::SetUnInitDeepLearn()
{
	m_bUnInitDeepLearn = true;
}

void CDlArithmetic0::stopDeepLearn()
{
    m_bRun = false;

}
void CDlArithmetic0::readConifg()
{
	IFileVariable *pFile = getDVCfgFileVariable();
	if (pFile != NULL)
	{

		m_dvConfig.dv_product_path = pFile->getValue(DV_CONFIG_FILE).toCString();
		m_dvConfig.dv_config_path = pFile->getValue(DV_CONFIG_FILE).toCString() + "/hconfig/config.json";
		m_dvConfig.defect_log_file_path = pFile->getValue(DV_H_LOG_PATH, "./log").toCString();
		pFile->bindValue(DV_H_LOG_PATH, &m_dvConfig.defect_log_file_path);

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
}
void CDlArithmetic0::readDVConifg(stEnableConfig* pEnableConfig)
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
int CDlArithmetic0::getDefectType(int iType, bool &bSeverity)
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
			bSeverity = m_defectInfoList.at(i).severity>0 ? true : false;
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

void CDlArithmetic0::BjaiDvDefectToDefectFeature(CImageProduct* pProduct, stDefectFeature DefectFeature, string strchannel)
{

	string channel = strchannel;

	double dCamerammArea = 0.0;
	double dCameramm = 0.0;
	IFileVariable *pCameraFile = getCameraSetCfgFileVariable();
	if (pCameraFile)
	{

		string key = CAMERAPARAM;
        key = key + "_" + SString::IntToStr(DefectFeature.iStation);
        dCameramm = pCameraFile->getValue(key,CAMERAPARAM_MOVEUNIT).toDouble();
        
        channel += QString::number(DefectFeature.iStation).toStdString();
		dCamerammArea = dCameramm*dCameramm;
	}
	int iCameraIndex = 0;
	int iType = 0;
	//getSqueLightTypeAndCameraIndex(channel, iType, iCameraIndex);

	stDefectFeature *mark = new stDefectFeature;
	mark->strOriginate = DefectFeature.strOriginate;
	mark->strChannelName = channel;
	mark->sn = to_string(pProduct->getProductSn());
	mark->iChannelType = 0;
	mark->iCameraIndex = iCameraIndex;
    mark->iStation = DefectFeature.iStation;
	mark->iMarkIndex = pProduct->pDefectStrutVec->size();
	mark->iDvDefectType = DefectFeature.iDvDefectType;
	mark->iDefectType = DefectFeature.iDvDefectType;
	mark->iposition = DefectFeature.iposition;
	mark->iShape = DefectFeature.iShape;
	if (mark->iShape >= DEFECT_SHAPE_SIZE || mark->iShape < 0)
	{
		mark->iShape = 0;
	}
	//
    if(mark->iDefectType < m_defectNameVec.size())
    {
        mark->strDefectName = m_defectNameVec[mark->iDefectType];
    }
	else {
		mark->iDefectType = 0;
		mark->strDefectName = m_defectNameVec[mark->iDefectType];
	}

	mark->defectWidth = DefectFeature.defectWidth;
	mark->defectHeight = DefectFeature.defectHeight;
	mark->dmm_defectHeight = (double)mark->defectHeight*dCameramm;
	mark->dmm_defectWidth = (double)mark->defectWidth*dCameramm;
	mark->dWide = DefectFeature.dWide;
    mark->dHigh = DefectFeature.dHigh;
	mark->SkeletonLength = mark->dmm_defectWidth > mark->dmm_defectHeight ? mark->dmm_defectWidth : mark->dmm_defectHeight;
	mark->iSize = DefectFeature.iSize;
	mark->posx = DefectFeature.posx;
	mark->posy = DefectFeature.posy;
	mark->contrast = DefectFeature.contrast;
	mark->fscore = DefectFeature.fscore;
	mark->fdetect_score = DefectFeature.fdetect_score;
	mark->dSizeArea = mark->iSize*dCamerammArea;
	mark->idetect_flag = DefectFeature.idetect_flag;
	mark->DefectBAvgGray = DefectFeature.DefectBAvgGray;

	string channelLevel = miStation == 0 ? strchannel : channel;

	getDefectLevel(channelLevel, mark);//等级计算//

    m_DefectLevelLock.Lock();
    pProduct->pDefectStrutVec->push_back(mark);
    m_DefectLevelLock.Unlock();
}
void CDlArithmetic0::DvDefectToDefectFeature(CImageProduct* pProduct, stDefectFeature DefectFeature, string strchannel)
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
		 channel += QString::number(miStation).toStdString();
	}
	int iCameraIndex = 0;
	int iType = 0;
	getSqueLightTypeAndCameraIndex(channel, iType, iCameraIndex);

	stDefectFeature *mark = new stDefectFeature;
	mark->strOriginate = "DV";
	mark->strChannelName = channel;
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
	string channelLevel = miStation == 0 ? strchannel : channel;

	getDefectLevel(channelLevel, mark);//等级计算//

	m_DefectLevelLock.Lock();
	pProduct->pDefectStrutVec->push_back(mark);
	m_DefectLevelLock.Unlock();
}
bool CDlArithmetic0::getDefectLevel(stDefectFeature *pDefectRes)
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

void CDlArithmetic0::ThreadDefectLevelProcess(string param)
{
	  vector<string> vect;
	  getEnabelChannle(vect);
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
		else {
			return;
		}


#if ENABLE_DV_INTERFACE

		if (m_pEnableConfig->bImageDV || m_pEnableConfig->bImageSYDV)
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("AI分级开始: %1: defect_num: %2").arg(param.c_str())
				.arg(m_results_ptr.all_results[iChannel].part_results[0].defect_num));
            for (int k = 0; k < m_results_ptr.all_results[iChannel].part_results[0].defect_num; k++)
			//for (int k = 0; k < m_results_ptr.all_results->part_results[iChannel].defect_num; k++)
			{
				stDefectFeature pMarkStrut;
				//int n = m_results_ptr.all_results[iChannel].part_results[0].defects[k].channel;
               // int iv = n < vect.size() ? n : 0;
				
				pMarkStrut.strChannelName = vect.at(iChannel);

                pMarkStrut.dWide = m_results_ptr.all_results[iChannel].part_results[0].defects[k].rect.width;
                pMarkStrut.dHigh = m_results_ptr.all_results[iChannel].part_results[0].defects[k].rect.height;
				
                pMarkStrut.defectWidth  = m_results_ptr.all_results[iChannel].part_results[0].defects[k].incline_width;
                pMarkStrut.defectHeight = m_results_ptr.all_results[iChannel].part_results[0].defects[k].incline_length;
                pMarkStrut.iSize = m_results_ptr.all_results[iChannel].part_results[0].defects[k].area;

                pMarkStrut.posx = m_results_ptr.all_results[iChannel].part_results[0].defects[k].rect.x+ pMarkStrut.dWide /2;
                pMarkStrut.posy = m_results_ptr.all_results[iChannel].part_results[0].defects[k].rect.y+pMarkStrut.dHigh/2;
				
				pMarkStrut.iDvDefectType = getDefectIndex(m_results_ptr.all_results[iChannel].part_results[0].defects[k].name);

                pMarkStrut.strOriginate = m_results_ptr.all_results[iChannel].part_results[0].defects[k].name;
				
                pMarkStrut.contrast = m_results_ptr.all_results[iChannel].part_results[0].defects[k].contrast;
				pMarkStrut.DefectBAvgGray = m_results_ptr.all_results[iChannel].part_results[0].defects[k].avg;
				pMarkStrut.fdetect_score = m_results_ptr.all_results[iChannel].part_results[0].defects[k].score;

				m_pInstance->BjaiDvDefectToDefectFeature(m_pProduct, pMarkStrut, pMarkStrut.strChannelName);

			}
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("AI分级结束: %1").arg(param.c_str()));
		}
		//
		//    int count = results_ptr.imgs_defect_results[i].count;
		//    for (size_t j = 0; j < count; j++)
		//    {
		//        if (pMarkStrutVec != NULL)
		//        {
		//            stDefectFeature* pMarkStrut = new stDefectFeature();
		//            pMarkStrut->strChannelName = vecEnableChannel.at(0);
		//            pMarkStrut->iMarkIndex = pMarkStrutVec->size();
		//            pMarkStrut->dHigh = results_ptr.imgs_defect_results[i].defectAttr[j].rect.height;
		//            pMarkStrut->dWide = results_ptr.imgs_defect_results[i].defectAttr[j].rect.width;
		//            pMarkStrut->iDvDefectType = results_ptr.imgs_defect_results[i].defectAttr[j].type;
		//            pMarkStrut->iSize = results_ptr.imgs_defect_results[i].defectAttr[j].area;
		//            pMarkStrut->posx = results_ptr.imgs_defect_results[i].defectAttr[j].rect.x + pMarkStrut->dWide / 2;
		//			  pMarkStrut->posy = results_ptr.imgs_defect_results[i].defectAttr[j].rect.y + pMarkStrut->dHigh / 2;
		//            pMarkStrut->strDefectName = results_ptr.imgs_defect_results[i].defectAttr[j].name;
		//            pMarkStrutVec->push_back(pMarkStrut);
		//        }
		//    }
		//

#endif

}

bool CDlArithmetic0::getDefectLevel(string strChannel, stDefectFeature *pDefectRes)
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
    return m_pIDefectQualification->getDefectSeverityLevel(strChannel,iDefectType, pDefectRes);
}

void CDlArithmetic0::startDefectLevel(CImageProduct* pProduct)
{
	if (!m_pEnableConfig->bDefectLevel)
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("未使能缺陷分级"), UILOG_WARN);
		return;
	}
	IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (p2 != NULL)
	{
		m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
		m_defectIndexVec = p2->getDefectListInstance(0)->getDefectIndexList();
		m_defectEnableVec = p2->getDefectListInstance(0)->getDefectEnableList();
		m_enNameVec = p2->getDefectListInstance(0)->getDefectEnNameList();
		m_defectInfoList = p2->getDefectListInstance(0)->getDefectInfo();
	}
    m_pProduct = pProduct;
#if ENABLE_DV_INTERFACE
    m_bMultiThread = false;
    IFileVariable *pFile = getDVCfgFileVariable();
    if (pFile != NULL)
    {
        m_bMultiThread = pFile->getValue(DV_ARITHMETIC_MULTITHREAD, 0).toBool();
    }

    int iStationIndex = miStation;
    string sense = SString::IntToStr((int)miStation);

    vector<string> vect;
    getEnabelChannle(vect, sense);
    vector<JThreadTP<CDlArithmetic0, string, void>*> runningThreads;

    for (int i = 0; i < vect.size(); i++)
    {
        string enChannel = vect.at(i);

        if(!m_bMultiThread)
        {
            ThreadDefectLevelProcess(enChannel);
        }
        else
        {
            JThreadTP<CDlArithmetic0, string, void>* pthread = NULL;
            if (m_mapDefectLevelThreads.find(enChannel) == m_mapDefectLevelThreads.end())
            {
                pthread = new JThreadTP<CDlArithmetic0, string, void>;
                m_mapDefectLevelThreads[enChannel] = pthread;
            }

            pthread = m_mapDefectLevelThreads[enChannel];
            if (!pthread->IsRunning())
            {
                pthread->StartThread(this, &CDlArithmetic0::ThreadDefectLevelProcess, enChannel);
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
	/*
	int iCount = 20;
	for (int i = 0; i < iCount; i++)
	{
		dv::DefectAttr result;
		result.type = rand() % 37;
		result.rect.x = rand() % 8191;
		result.rect.y = rand() % 12800;
		result.rect.width = rand() % (8191 - result.rect.x);
		result.rect.height = rand() % (12800 - result.rect.y);
		result.position = rand() % 2;
		result.grid_num = i % 8;
		result.level = rand() % 200;
		result.shape = i % 3;

		string str = "SEQUENCE_A";

		DvDefectToDefectFeature(pProduct, result, str);
	}*/
#endif
}
int CDlArithmetic0::initDeepLearnEnv()
{
	
	CISAPUGuard<JMutex> autoLock(m_HLock);
	if (!m_pEnableConfig->bImageSYDV || m_bIsHInit)
	{
		return 0;
	}
	readConifg();
#if ENABLE_DV_INTERFACE
	IFileVariable *pFile = getDVCfgFileVariable();
	if (pFile != NULL)
	{
	
		m_dvConfig.dv_product_path = pFile->getValue(DV_CONFIG_FILE).toCString();
		m_dvConfig.dv_config_path = m_dvConfig.dv_product_path + "/hconfig/config.json";
	}
	int iRet = InitBJParam_FSK(&m_hInputParam, 1, m_dvConfig.dv_product_path.c_str(),1);
	if (iRet != dv::ErrorNone)
	{
		CSendMessage::GetInstance()->Send(QString(" station %1 initDVEnv error! error code [%1]").arg(miStation).arg(iRet), UILOG_ERR);
		return -1;
	}
	//std::string  grading_path = m_dvConfig.dv_product_path + "/rankModel.dat";
	//m_hGradingHandle = grading_init(grading_path.c_str());
	//
	//if (0 == m_hGradingHandle)
	//{
	//	string str = "init grading_dll ERROR  path:" + grading_path;
	//	CSendMessage::GetInstance()->Send(QString::fromStdString(str), UILOG_ERR);
	//	return -1;
	//}
	//std::string  rankEdgeModel_path = m_dvConfig.dv_product_path + "/rankEdgeModel.dat";
	//m_hEdgeModel = grading_init(rankEdgeModel_path.c_str());
	//
	//if (0 == m_hEdgeModel)
	//{
	//	string str = "init grading_dll ERROR  path:" + rankEdgeModel_path;
	//	CSendMessage::GetInstance()->Send(QString::fromStdString(str), UILOG_ERR);
	//	return -1;
	//}

	m_bIsHInit = true;

	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("station %1 算法初始化完成").arg(miStation));
#endif

	return 0;
	
}
int CDlArithmetic0::unInitDeepLearnEnv()
{

	CISAPUGuard<JMutex> autoLock(m_HLock);
#if ENABLE_DV_INTERFACE

	if (m_bIsHInit && m_bUnInitDeepLearn)
	{
	    m_bIsHInit = false;
		m_bUnInitDeepLearn = false;
		UnInitBJParam_FSK(&m_hInputParam, 1, m_defectResult, &m_rippleResultInfo);
		//UnInitBJParam(&m_hInputParam, m_defectResult, &m_rippleResultInfo, &m_DeformatrionInfo);
		//grading_uninit(&m_hGradingHandle);
		//grading_uninit(&m_hEdgeModel);
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("statin %1 算法反初始化完成").arg(miStation));
	}

#endif	

	return 0;
}
int CDlArithmetic0::runDeepLearn(CImageProduct* pProduct, uint64& uiElapsedTime)
{
	int iRet = -1;
	m_bRun = true;
#if ENABLE_DV_INTERFACE

    int iStationIndex = miStation;
    string sense = SString::IntToStr((int)miStation);

	m_hInputParam.image = NULL;

	vector<string> vecEnableChannel;
    getEnabelChannle(vecEnableChannel, sense);

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
		istep = 256 / 8 * 7;
		iStartTime = 1;
	}
	int iImageCnt = 0;
	for (int i = 0; i < iBlockCnt && m_bRun; i++)
	{
		int data_end = (i + 1 == iBlockCnt);
		iImageCnt = vecEnableChannel.size();
		m_hInputParam.image = new dv::Image[iImageCnt];;
		for (int k = 0; k < iImageCnt && m_bRun; k++)
		{
			ImageBuffer *buff = pProduct->getImageBuffer(QString::fromStdString(vecEnableChannel.at(k)), iStationIndex);
			if (NULL == buff)
			{
				pProduct->setProcessErrCode(er_DV);
				CSendMessage::GetInstance()->Send(QString("product(%1)-%2 station img_data is NULL").arg(pProduct->getProductSn()).arg(iStationIndex), UILOG_ERR);
				return iRet;
			}
			stResImage stResImagetmp = pProduct->getImageBuffer(QString::fromStdString(vecEnableChannel.at(k)), iStationIndex)->rawData;
			if (NULL == stResImagetmp.img_data)
			{
				pProduct->setProcessErrCode(er_DV);
				CSendMessage::GetInstance()->Send(QString("product(%1)-%2 station img_data is NULL").arg(pProduct->getProductSn()).arg(iStationIndex), UILOG_ERR);
				return iRet;
			}
			m_hInputParam.image[k].data = stResImagetmp.img_data;
			m_hInputParam.image[k].width = stResImagetmp.img_width;
			m_hInputParam.image[k].height = stResImagetmp.img_height;
			m_hInputParam.image[k].channels = stResImagetmp.img_channels;
			m_hInputParam.image[k].width_step = stResImagetmp.img_width;

		}

		//setImageName((QString("%1_%2.bmp").arg(pProduct->getProductSn())).toStdString().c_str(), 1,i);
		CSendMessage::GetInstance()->sendUpdateUiStus(QString::number(pProduct->getProductSn()), update_deal);
		CSendMessage::GetInstance()->Send(QString("product(%1)-station:%2 processImageDV start").arg(pProduct->getProductSn()).arg(iStationIndex));

		///****************************************/

		uint64 iStartTime = JTime::CurrentMillisecond();
		try
		{

			m_rippleResultInfo.rect_size = 0;
            if (m_pEnableConfig->bImageSYDV)
			{
				iRet = classifySilkSingle_FSK(&m_hInputParam, 1, m_defectResult, &m_rippleResultInfo);
			
			}
			else {

				m_hInputParam.image = NULL;
				pProduct->setProcessErrCode(er_DV);
				return iRet;
			}

		}
		catch (...)
		{
			CSendMessage::GetInstance()->Send(QString("product(%1)-station %2 processImageDV exception   IRet = %3").arg(pProduct->getProductSn()).arg(iStationIndex).arg(iRet), UILOG_ERR);
			pProduct->setProcessErrCode(er_DV);
			m_hInputParam.image = NULL;
			return iRet;
		}

		uint64 iEndTime = JTime::CurrentMillisecond();
		uint64 iElapsedTime = iEndTime - iStartTime;
		uiElapsedTime = iElapsedTime;
		CSendMessage::GetInstance()->Send(QString("station %1 h elapsed time : %2").arg(iStationIndex).arg(iElapsedTime));

		CSendMessage::GetInstance()->Send(QString("station %1 DvtResult %2 size %3").arg(iStationIndex).arg(i).arg(m_defectResult[i].len));
		
		m_hInputParam.image = NULL;
		
		if (iRet != 0)
		{
			if (iRet == dv::ErrorTimerOut)
			{
				CSendMessage::GetInstance()->Send(QString("product(%1)-%2 processImageDV TimeOut").arg(pProduct->getProductSn()).arg(i), UILOG_WARN);
				pProduct->setProcessErrCode(er_DV_timeout);
			}
			else
			{
				CSendMessage::GetInstance()->Send(QString("product(%1)-%2 processImageDV Error").arg(pProduct->getProductSn()).arg(iRet), UILOG_ERR);
				pProduct->setProcessErrCode(er_DV);//非超时报错保存图片
			}

			return iRet;
		}
		CSendMessage::GetInstance()->Send(QString("product(%1)-station %2 processImageDV end").arg(pProduct->getProductSn()).arg(iStationIndex));
	}
#else
    CSendMessage::GetInstance()->Send(QString::fromLocal8Bit(" station:%1 H算法已关闭").arg(miStation), UILOG_WARN);

#endif
	return iRet;
}
