#include "dlArithmetic.h"
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
JMutex g_DlArithmeticMutex;

#define  ENABLE_DV_INTERFACE  1// 屏蔽DV 的接口 0为屏蔽，1为使能
CDlArithmetic *CDlArithmetic::m_pInstance = NULL;


CDlArithmetic::CDlArithmetic(int iStation)
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


CDlArithmetic::~CDlArithmetic()
{

    map<string, JThreadTP<CDlArithmetic, string, void>* >::iterator it5;
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

IDlArithmetic * CDlArithmetic::getInstance(int iStation)
{
    CISAPUGuard<JMutex> autoLock(g_DlArithmeticMutex);
	if (NULL == m_pInstance)
	{
        m_pInstance = new CDlArithmetic(iStation);
	}
	return m_pInstance;
}

void CDlArithmetic::initDefectIndex()
{
	/*------取消 2023-1-6 by lucus
	IFileVariable* file = getdefectIndex();
	int type = 0;
	if (file != NULL)
	{
		type = file->getValue("defindex","OK").toInt();
		m_defectIndex.push_back(pair<string,int>("OK", type));
		type = file->getValue("defindex", "HS").toInt();
		m_defectIndex.push_back(pair<string, int>("HS", type));
		type = file->getValue("defindex", "BB").toInt();
		m_defectIndex.push_back(pair<string, int>("BB", type));
		type = file->getValue("defindex", "CB").toInt();
		m_defectIndex.push_back(pair<string, int>("CB", type));
		type = file->getValue("defindex", "CH").toInt();
		m_defectIndex.push_back(pair<string, int>("CH", type));
		type = file->getValue("defindex", "YH").toInt();
		m_defectIndex.push_back(pair<string, int>("YH", type));
		type = file->getValue("defindex", "ZY").toInt();
		m_defectIndex.push_back(pair<string, int>("ZY", type));
		type = file->getValue("defindex", "CI").toInt();
		m_defectIndex.push_back(pair<string, int>("CI", type));
		type = file->getValue("defindex", "YM").toInt();
		m_defectIndex.push_back(pair<string, int>("YM", type));
		type = file->getValue("defindex", "ZW").toInt();
		m_defectIndex.push_back(pair<string, int>("ZW", type));
	}*/
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

void CDlArithmetic::releaseInstance()
{
    CISAPUGuard<JMutex> autoLock(g_DlArithmeticMutex);
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
int CDlArithmetic::initDeepLearnEnv()
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

void CDlArithmetic::SetUnInitDeepLearn()
{
	if (m_bIsHInit)
	{
		m_bUnInitDeepLearn = true;
	}
}
int CDlArithmetic::updateDeepLearnEnv()
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

int CDlArithmetic::unInitDeepLearnEnv()
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
InitDeepLearnState CDlArithmetic::GetDeepLearnEnvState()
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
int CDlArithmetic::runDeepLearn(CImageProduct* pProduct, uint64& uiElapsedTime)
{
	int iRet = 0;
    m_bRun = true;
#if ENABLE_DV_INTERFACE
    vector<string> vecEnableChannel;
	getEnabelChannle(vecEnableChannel, to_string(miStation));
    int num = vecEnableChannel.size();
	int isn = pProduct->getProductSn();
	
    bjcv::Image*  imgs_ptr = new bjcv::Image[num];
    for (size_t i = 0; i < num; i++)
    {
        ImageBuffer* pImagebuffer = pProduct->getImageBuffer(QString::fromStdString(vecEnableChannel.at(i)), miStation);
        if (pImagebuffer != NULL)
        {
            imgs_ptr[i].channels = 1;
            imgs_ptr[i].data= pImagebuffer->rawData.img_data;
            imgs_ptr[i].height= pImagebuffer->rawData.img_height;
            imgs_ptr[i].width = pImagebuffer->rawData.img_width;
            imgs_ptr[i].width_step = imgs_ptr[i].width;
        }
    }

    //bjai::DefectResultInfo results_ptr;
	memset(&m_results_ptr, 0, sizeof(m_results_ptr));
	if (pProduct->pDefectStrutVec != NULL)
	{
		for (int i = 0; i < pProduct->pDefectStrutVec->size(); i++)
		{
			if (pProduct->pDefectStrutVec->at(i) != NULL)
			{
				delete pProduct->pDefectStrutVec->at(i);
			}
		}
		pProduct->pDefectStrutVec->clear();
	}
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("Produc:%1;inspect开始").arg(isn));
	uint64 RunTime = JTime::CurrentMillisecond();
    bjcv::ErrorCode err = bjcv::inspect(imgs_ptr, num, &m_results_ptr, model_handle_ptr);
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("Produc:%3;inspect结束 time :%1ms").arg(JTime::CurrentMillisecond() - RunTime).arg(isn));
	//
	delete[]imgs_ptr;
	imgs_ptr = NULL;
	
    if(bjcv::ErrorCode::Success == err)
    {
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("Produc:%2;runAI成功(%1)").arg((int)err).arg(isn));
        int iDefectTotalCnt = m_results_ptr.defect_num;
		//for (int k = 0; k <num && k < m_results_ptr.img_num; k++)
        for (int k = 0; k <m_results_ptr.result_num; k++)
		{
			                                                                                       //m_results_ptr.all_results[iChannel].part_results[0]
            CSendMessage::GetInstance()->Send(QString("Produc:%3;Channel:%1  szie :%2").arg(k).arg(m_results_ptr.all_results[k].part_results[0].defect_num).arg(isn));

            //iDefectTotalCnt += m_results_ptr.all_results[k].count;
		}
		IFileVariable* pFileVariable = getQualificationCfgFileVariable();
		if (pFileVariable != NULL)
		{
			bool bEn = pFileVariable->getValue(QUALIFICATION_ENABLE_CNT_ABORT).toBool();
			int iMaxNumber = pFileVariable->getValue(QUALIFICATION_CNT_ABORT).toInt();
			if (bEn && iDefectTotalCnt >= iMaxNumber)
			{
				pProduct->setProcessErrCode(er_DV);;//缺陷过多直接退出等级计算
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("缺陷个数:%1!退出等级计算.").arg(QString::number(iDefectTotalCnt)), UILOG_WARN);
			}
			else
			{
				CSendMessage::GetInstance()->Send(QString("Produc:%2;DefectLevel(size:%1)  Start !").arg(QString::number(pProduct->pDefectStrutVec->size())).arg(isn));
				RunTime = JTime::CurrentMillisecond();
				startDefectLevel(pProduct);
				CSendMessage::GetInstance()->Send(QString("Produc:%3;DefectLevel(size:%1) End  time:%2ms!").arg(QString::number(pProduct->pDefectStrutVec->size())).arg(JTime::CurrentMillisecond() - RunTime).arg(isn));
			}
		}
        //startDefectLevel(pProduct);
    }
    else
    {
		pProduct->setProcessErrCode(er_DV);
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("Produc:%2;runAI失败(%1)").arg((int)err).arg(isn));
        if (err == bjcv::ErrorCode::ErrorTimerOut)
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("Produc:%2;算法超时返回(%1)").arg((int)err).arg(isn),UILOG_ERR);
		}
        return (int)err;
    }

#else
        CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("H算法已关闭"), UILOG_WARN);

#endif
        return iRet;
}

void CDlArithmetic::stopDeepLearn()
{
    m_bRun = false;

}
void CDlArithmetic::readConifg()
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
void CDlArithmetic::readDVConifg(stEnableConfig* pEnableConfig)
{
	m_pEnableConfig = pEnableConfig;
	return;
}
int CDlArithmetic::getDefectType(int iType, bool &bSeverity)
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

void CDlArithmetic::BjaiDvDefectToDefectFeature(CImageProduct* pProduct, stDefectFeature DefectFeature, string strchannel)
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

void CDlArithmetic::DvDefectToDefectFeature(CImageProduct* pProduct, stDefectFeature DefectFeature, string strchannel)
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
bool CDlArithmetic::getDefectLevel(stDefectFeature *pDefectRes)
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

void CDlArithmetic::ThreadDefectLevelProcess(string param)
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

bool CDlArithmetic::getDefectLevel(string strChannel, stDefectFeature *pDefectRes)
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

void CDlArithmetic::startDefectLevel(CImageProduct* pProduct)
{
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

    int iStationIndex = miStation;
    string sense = SString::IntToStr((int)miStation);

    vector<string> vect;
    getEnabelChannle(vect, sense);
    vector<JThreadTP<CDlArithmetic, string, void>*> runningThreads;

    for (int i = 0; i < vect.size(); i++)
    {
        string enChannel = vect.at(i);

        if(!m_bMultiThread)
        {
            ThreadDefectLevelProcess(enChannel);
        }
        else
        {
            JThreadTP<CDlArithmetic, string, void>* pthread = NULL;
            if (m_mapDefectLevelThreads.find(enChannel) == m_mapDefectLevelThreads.end())
            {
                pthread = new JThreadTP<CDlArithmetic, string, void>;
                m_mapDefectLevelThreads[enChannel] = pthread;
            }

            pthread = m_mapDefectLevelThreads[enChannel];
            if (!pthread->IsRunning())
            {
                pthread->StartThread(this, &CDlArithmetic::ThreadDefectLevelProcess, enChannel);
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
	int iCount = rand() % 100;
	//for (int i = 0; i < iCount; i++)
	//{
	//	dv::DefectAttr result;
	//	result.type = rand() % 12;
	//	result.rect.x = rand() % 8191;
	//	result.rect.y = rand() % 12800;
	//	result.rect.width = rand() % (8191 - result.rect.x);
	//	result.rect.height = rand() % (12800 - result.rect.y);
	//	result.position = 0;
	//	DvDefectToDefectFeature(pProduct, result, "SEQUENCE_ABC");
	//}
#endif
}

