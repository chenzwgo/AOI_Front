#include "InvokeArithmetic.h"
#include "g12common.h"
#include "g12globalenv.h"
#include "filevariable/ifilevariable.h"
#include "g12arithmetic/iarithmeticStatic.h"
#include "callsdk.h"
#include "workflowfun.h"
#include "../sendmessage.h"
#include "ProductUnion.h"
#include "cimageprocess.h"
#include "stringcommon.h"
#include "iniFile.h"
#include "workflow/defectlevelthreads.h"
#include "rapidjson/document.h"
#include "rapidjson/Prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "jtime.h"
#include <QApplication>
#include <QFile>
#include "dlArithmetic/dlArithmetic.h"
#include "dlArithmetic/dlArithmetic0.h"
#include "inotifyservice.h"
#include "uicommon/commonutils.h"
#include "signalcheck.h"
#include "workflow.h"
#include "g12arithmetic/iarithmeticmeasuresize.h"
#include "commonapi.h"
#include "defect/idefectlist.h"
#include "defect/idefectlistmanage.h"
#include "defect/idefectqualimanage.h"
#include "workflow/backpc.h"
#include "frontpc.h"
CInvokeArithmetic *CInvokeArithmetic::m_pInstance = NULL;
#define  IMAGE_8K_WIDTH   8192
JMutex g_instanceMutex;

using namespace rapidjson;

CInvokeArithmetic::CInvokeArithmetic(void)
{
    m_bPreProcessThread = false;
    m_bProcessThread = false;
    m_bDeepLearnThread = false;
    m_bUpdateResultThread = false;
    m_bSaveImageThread = false;
    m_bRecyclingThread = false;
	m_bWaitGrapFinishthread = false;

    m_bIsFastGrab = false;
    m_bAotoTesthread = false;
    m_bFinishFlag = true;
    //m_DVHandles.filterHandle = NULL;
    //m_DVHandles.refiner = NULL;
    m_pDeepLearnThread = NULL;
    m_pUpdateResultThread = NULL;
    m_pRecyclingThread = NULL;
    m_csvFile = NULL;
    m_pIDefectQualification = NULL;
    m_pGrapLocalImageThread = NULL;
	m_pAotoTesthread = NULL;
	m_pWaitGrapFinishthread = NULL;
	m_iIndexDlArithmetic  =0;
	m_iIndexDlArithmetic2 =0;
	m_iIndexDlArithmetic0 =0;
    m_ElapsedTimeLock.Init();
    m_pIDlArithmetic = NULL;
	m_pIDlArithmetic0 = NULL;
	init();
}


CInvokeArithmetic::~CInvokeArithmetic(void)
{
    unInit();
}

CInvokeArithmetic * CInvokeArithmetic::getInstance()
{
    CISAPUGuard<JMutex> autoLock(g_instanceMutex);
    if (NULL == m_pInstance)
    {
        m_pInstance = new CInvokeArithmetic;
    }
    return m_pInstance;
}

void CInvokeArithmetic::releaseInstance()
{
    CISAPUGuard<JMutex> autoLock(g_instanceMutex);
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

bool CInvokeArithmetic::init()
{
	
	IFileVariable* pFile = getStationCfgVariable();

	if (pFile != NULL)
	{
		m_iIndexDlArithmetic0 = pFile->getValue("ImageStation", "BIAN").toInt();
		m_iIndexDlArithmetic = pFile->getValue("ImageStation", "MAIN").toInt();
		//m_iIndexDlArithmetic = pFile->getValue("ImageStation", "VERT").toInt();
        //m_iIndexDlArithmetic2 = pFile->getValue("ImageStation", "MAIN").toInt();
		//
    }
    m_pIDlArithmetic0 = CDlArithmetic0::getInstance(m_iIndexDlArithmetic0); //BJAI
	m_pIDlArithmetic = CDlArithmetic::getInstance(m_iIndexDlArithmetic); //BJAI

//    m_pIDlArithmetic = CDlArithmetic0::getInstance(0); //ShenShiAI

	if (m_pDeepLearnThread == NULL)
	{ 
		m_pDeepLearnThread = new JThreadTP<CInvokeArithmetic, void*, void>;
	}
	if (m_pUpdateResultThread == NULL)
	{
		m_pUpdateResultThread = new JThreadTP<CInvokeArithmetic, void*, void>;
	}
	if (m_pRecyclingThread == NULL)
	{
		m_pRecyclingThread = new JThreadTP<CInvokeArithmetic, void*, void>;
	}

    for(int i=0 ;i < getImageStationCnt();i++)
    {
        m_pProcessThreads[i] = new JThreadTP<CInvokeArithmetic, int, void>();

    }

    for(int i=0 ;i < getImageStationCnt();i++)
    {
		m_mapSaveImageThreads[i] = new JThreadTP<CInvokeArithmetic, int, void>();

    }
    if (m_csvFile == NULL)
    {
		m_csvFile = new QFile();
	}

	IDefectListManage* p2 = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p2)
	{
		m_defectNameVec = p2->getDefectListInstance(0)->getDefectCnNameList();
		m_defectEnNameVec = p2->getDefectListInstance(0)->getDefectEnNameList();
		m_defectEnableVec = p2->getDefectListInstance(0)->getDefectEnableList();
	}

	if (m_pGrapLocalImageThread == NULL)
	{
		m_pGrapLocalImageThread = new JThreadTP<CInvokeArithmetic, void *, void>;
	}

	IDefectQualimanage* pd = (IDefectQualimanage*)GetSDKInstance("defectqualification", IDefectQualimanage::IDD_DEFECT_QULIFICATION_MANAGE);
	if (NULL != pd)
	{
		m_pIDefectQualification = pd->getDefectQualificationInstance(0);
	}
	readEnableConfig();

	if (m_pIDlArithmetic)
	{
		m_pIDlArithmetic->readDVConifg(&m_enableConfig);
	}
	if (m_pIDlArithmetic0)
	{
		m_pIDlArithmetic0->readDVConifg(&m_enableConfig);
	}
    readOtherConifg();

	startImageProcess();
    return true;
}

bool CInvokeArithmetic::unInit()
{
	m_bProcessThread = false;
    m_bDeepLearnThread = false;
    m_bUpdateResultThread = false;
    m_bSaveImageThread = false;
	m_bRecyclingThread = false;
    m_bIsFastGrab = false;
    m_bAotoTesthread = false;
	m_bPreProcessThread = false;
	m_bWaitGrapFinishthread = false;
	if (m_csvFile !=NULL)
	{
		delete m_csvFile;
		m_csvFile = NULL;
	}

    for(int i=0 ;i < getImageStationCnt();i++)
    {
        if(m_pProcessThreads[i] != NULL)
        {
            while (m_pProcessThreads[i]->IsRunning())
            {
                Sleep(5);
            }
            delete m_pProcessThreads[i];
            m_pProcessThreads[i] = NULL;
        }
    }

    for(int i=0 ;i < getImageStationCnt();i++)
    {
        if(m_mapSaveImageThreads[i] != NULL)
        {
            while (m_mapSaveImageThreads[i]->IsRunning())
            {
                Sleep(5);
            }
            delete m_mapSaveImageThreads[i];
			m_mapSaveImageThreads[i] = NULL;
        }
    }

    if (m_pDeepLearnThread != NULL)
    {
        while (m_pDeepLearnThread->IsRunning())
        {
            Sleep(5);
        }
        delete m_pDeepLearnThread;
        m_pDeepLearnThread = NULL;
    }

    if (m_pUpdateResultThread != NULL)
    {
        while (m_pUpdateResultThread->IsRunning())
        {
            Sleep(5);
        }
        delete m_pUpdateResultThread;
        m_pUpdateResultThread = NULL;
    }

    if (m_pRecyclingThread != NULL)
    {
        while (m_pRecyclingThread->IsRunning())
        {
            Sleep(5);
        }
        delete m_pRecyclingThread;
        m_pRecyclingThread = NULL;
    }
	
	if (m_pGrapLocalImageThread != NULL)
	{
		while (m_pGrapLocalImageThread->IsRunning())
		{
			Sleep(5);
		}
		delete m_pGrapLocalImageThread;
		m_pGrapLocalImageThread = NULL;
	}

	if (m_pWaitGrapFinishthread != NULL)
	{
		while (m_pWaitGrapFinishthread->IsRunning())
		{
			Sleep(5);
		}
		delete m_pWaitGrapFinishthread;
		m_pWaitGrapFinishthread = NULL;
	}
	if (m_pAotoTesthread != NULL)
	{
		while (m_pAotoTesthread->IsRunning())
		{
			Sleep(5);
		}
		delete m_pAotoTesthread;
		m_pAotoTesthread = NULL;
	}
	if (m_pIDlArithmetic)
	{
		m_pIDlArithmetic->unInitDeepLearnEnv();
	}
	
    CProductUnion::getInstance()->releaseAllProduct();
    return true;
}

void CInvokeArithmetic::startDeepLearnProcess()
{
    if (m_pDeepLearnThread != NULL)
    {
        if (!m_pDeepLearnThread->IsRunning())
        {
            m_bDeepLearnThread = true;
            m_pDeepLearnThread->StartThread(this,&CInvokeArithmetic::ThreadImageDeepLearnProcess,this);
        }
    }
}

void CInvokeArithmetic::startPocess()
{
    //();
	if (m_pIDlArithmetic)
	{
		m_pIDlArithmetic->readDVConifg(&m_enableConfig);
	}

    startImageProcess();
    startDeepLearnProcess();
	startUpdataToUI();
	startSaveImage();
	startRecycling();
}

void CInvokeArithmetic::startGrapLocalImage()
{
	if (m_pGrapLocalImageThread != NULL)
	{
		if (!m_pGrapLocalImageThread->IsRunning())
		{
			IFileVariable *pFile = getCameraSetCfgFileVariable();
			CImageProduct*  proInfo = CProductUnion::getInstance()->createProduct();
			if (pFile  != NULL)
			{
				bool bUseHistoryData = pFile->getValue(IMAGE_HISTORY_DATA_ENABLE).toBool();
				if (bUseHistoryData)  //如果使用历史数据则产品sn与图像sn一致
				{
					unsigned long lSN = pFile->getValue(WORK_PRODUCT_SN).toULong();
					proInfo->setProductSn(lSN);
				}
				
			}
			proInfo->setProductAllStatus(en_preGrap);
			m_pGrapLocalImageThread->StartThread(this, &CInvokeArithmetic::ThreadGrapLocalImage, proInfo);
			startPocess();
		}
		else
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("取图处理未完成"));
		}
	}
	
}
void CInvokeArithmetic::startAotoTest(int imax)
{
	if (NULL == m_pAotoTesthread)
	{
		m_pAotoTesthread = new JThreadTP<CInvokeArithmetic, void*, void>;
	}
	if (!m_pAotoTesthread->IsRunning())
	{
		m_bAotoTesthread = true;
		m_iAotoTestMax = imax;
		m_pAotoTesthread->StartThread(this, &CInvokeArithmetic::ThreadAotoTestProcess, NULL);
	}
}


void CInvokeArithmetic::startImageProcess()
{
	int iImageStationCnt = getImageStationCnt();
    for(int i=0 ;i < iImageStationCnt;i++)
    {
        if (!m_pProcessThreads[i]->IsRunning())
        {
            m_bProcessThread = true;
            m_pProcessThreads[i]->StartThread(this, &CInvokeArithmetic::ThreadImageProcess, i);
        }
    }


}

void CInvokeArithmetic::startSaveImage()
{
   
	for (int i = 0;i < getImageStationCnt();i++)
	{
		if (!m_mapSaveImageThreads[i]->IsRunning())
		{
			m_bSaveImageThread = true;
			m_mapSaveImageThreads[i]->StartThread(this, &CInvokeArithmetic::ThreadSaveImage, i);
		}
	}
}

void CInvokeArithmetic::startRecycling()
{
    if (!m_pRecyclingThread->IsRunning())
    {
		m_bRecyclingThread = true;
        m_pRecyclingThread->StartThread(this,&CInvokeArithmetic::ThreadRecyclingProcess,this);
    }    
}


int CInvokeArithmetic::updataResultToUI(CImageProduct* pProduct)
{
	pProduct->setProductAllStatus(en_updateui);
	pProduct->setUpdateUIFinish(false);
    int elapsedTime = pProduct->processElapsedTime();
   // CSendMessage::GetInstance()->Send(QString("Product(%1) : elapsed %2 ms").arg(pProduct->getProductSn()).arg(elapsedTime));
    CSendMessage::GetInstance()->sendUpdateUiStus(QString::number(elapsedTime),update_dealtime);   
    CSendMessage::GetInstance()->sendProductInfo(pProduct);
	pProduct->setProductAllStatus(en_updateuifinish);
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("更新UI完成,Sn(%1)").arg(pProduct->getProductSn()));
    return 0;
}
void  CInvokeArithmetic::setAotoTestFlag(bool bFinish)
{
	m_bFinishFlag = bFinish;
}
void  CInvokeArithmetic::setStopAotoTest()
{
	m_bAotoTesthread = false;
}
void  CInvokeArithmetic::ThreadAotoTestProcess(void* param)
{
	//
	int i = 0;
	while (m_bAotoTesthread && m_iAotoTestMax>i)
	{

		// 个数左于10000以上默认为 压力测试-使用固定时间加载图片
		if (m_iAotoTestMax>=10000)
		{
			IFileVariable *pfile = getFileVariable("./programs/globalenv.ini");
			int iTimeSet = 2800;
			if (pfile != NULL)
			{
				iTimeSet =pfile->getValue("runTime", "time", 2800).toUInt();
			}
			if (0 < i)
			{
				INotifyService* pNotifyService = CommonUtils::getNotifyService();
				if (NULL != pNotifyService)
				{
					pNotifyService->send("customsettingplugin", "next_product", 0, NULL);
				}
			}
			startGrapLocalImage();//启动一个产品测试
			i++;
			uint64 iStartTime = JTime::CurrentMillisecond();
			do
			{
				Sleep(50);

			} while (m_bAotoTesthread&&(JTime::CurrentMillisecond() - iStartTime) < iTimeSet);
		}
        // 自动测试模式-测试完一个才下一个
		else {
			if (m_bFinishFlag)//测试完成--   一个一个的测试
			{
				//更新获取下一个产品图像
				if (0 < i)
				{
					INotifyService* pNotifyService = CommonUtils::getNotifyService();
					if (NULL != pNotifyService)
					{
						pNotifyService->send("customsettingplugin", "next_product", 0, NULL);
					}
				}
				m_bFinishFlag = false;
				startGrapLocalImage();//启动一个产品测试
				i++;

			}
			Sleep(50);
		}
	}
	if (i == m_iAotoTestMax)
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("自动连续测试结束"),UILOG_ERR);
	}
	else
	{
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("停止自动连续测试"), UILOG_ERR);
	}
}

void CInvokeArithmetic::ThreadGrapLocalImage(void* param)
{
	IFileVariable* pAcquisitionFile = getAcquisitionFileVariable(getCurrentProgramName(), "1");
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	bool bUseHistoryData = false;
	QString strName = "";
	//int idextN = 2;
	if (pFile != NULL)
	{
		bUseHistoryData = pFile->getValue(IMAGE_HISTORY_DATA_ENABLE).toBool();
	}
	CImageProduct* pProduct = (CImageProduct*)(param);
	if (pProduct == NULL)
	{
		return;
	}
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("开始取图处理"));
	pProduct->setStartGrapImageTime();
	pProduct->setProductAllStatus(en_graping);
	vector<string> vect;
	//
	int iImageStationCnt = getImageStationCnt();
	for (size_t iIndex = 0; iIndex < iImageStationCnt; iIndex++)
	{
		if (isImageStationEnable(iIndex))
		{
			vect.clear();
			getEnabelChannle(vect, to_string(iIndex));//
			for (size_t i = 0; i < vect.size(); i++)
			{
				ThreadLocalImage(pProduct, vect.at(i), iIndex);
			}
			if (vect.size()>0)
			{
				pProduct->setProductAllStatus(en_grapfinish, iIndex);
			}
		}

	}
	/*
	getEnabelChannle(vect, "0");//
	for (int i = 0; i < vect.size(); i++)
	{
		ThreadLocalImage(pProduct, vect.at(i), 0);
	}
	if (vect.size()>0)
	{
		pProduct->setProductAllStatus(en_grapfinish, 0);
	}
	vect.clear();
	getEnabelChannle(vect, "1");//
	for (int i = 0; i < vect.size(); i++)
	{
		ThreadLocalImage(pProduct, vect.at(i),1);
	}
	pProduct->setProductAllStatus(en_grapfinish, 1);
	pAcquisitionFile = getAcquisitionFileVariable(getCurrentProgramName(), "2");
	if (pAcquisitionFile != NULL)
	{
		if (pAcquisitionFile->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_GRAP_ENABLE, true).toBool(true))
		{
			vect.clear();
			getEnabelChannle(vect, "2");//
			for (int i = 0; i < vect.size(); i++)
			{
				ThreadLocalImage(pProduct, vect.at(i), 2);
			}
		}
	}
	pProduct->setProductAllStatus(en_grapfinish, 2);*/
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("结束取图处理"));
	pProduct->setEndGrapImageTime();
	//if (bUseHistoryData)
	//{
	//	productFillWithHistoryData(pProduct);
	//}
	//else
	//{
	//	pProduct->setProductAllStatus(en_grapfinish);
	//}

}


void CInvokeArithmetic::ThreadLocalImage(void* param, string strChannel, int Image_station)
{
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	string station = "";
	if (Image_station == 0)
	{
		station = "0";
	}
	else if (Image_station == 1)
	{
		station = "1";
	}
	else if (Image_station == 2)
	{
		station = "2";
	}
	IFileVariable* pAcquisitionFile = getAcquisitionFileVariable(getCurrentProgramName(), station);
	//int idextN = 0;
	//if (pAcquisitionFile)
	//{
	//	idextN = pAcquisitionFile->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_IMAGE_NAME_CH, 2).toInt();
	//}

	CImageProduct* pProduct = (CImageProduct*)param;
	if (pProduct == NULL)
	{
		return;
	}
	do
	{
		pProduct->setChannelAllStatus(QString::fromStdString(strChannel), en_graping, Image_station);
		string pathName = strChannel;
		string  strPath = "D:\\IMAGE\\offline";
		string  strFileName = pathName;
		QString strTmp = QString::fromStdString(strChannel);
		strPath = pFile->getValue(IMAGE_OFFLINE_PATH, strPath.c_str()).toCString();
		int strSn = pFile->getValue(WORK_PRODUCT_SN, "1").toInt();
		//"procuct(12)-A2-rawImge.bmp");
		//procuct(7)-DOWN_A-rawImge.bmp
		QString fileName = QString::fromLocal8Bit("procuct(%1)-%2_%3-rawImge").arg(strSn).arg(QString::fromStdString(Utils::getCamSuffix(Image_station))).arg(strChannel.c_str());
		strFileName = fileName.toStdString();

		strPath = strPath + "\\" + strFileName;
		if (strFileName.find(".") == string::npos)
		{
			strPath += ".bmp";
		}
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("取图：") + QString::fromLocal8Bit(strPath.c_str()));

		if (!QFile::exists(QString::fromLocal8Bit(strPath.c_str())))
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("离线图片路径不存在[%1]!").arg(QString::fromLocal8Bit(strPath.c_str())), UILOG_ERR);
			break;
		}
		QImage img;
		if (!img.load(QString::fromLocal8Bit(strPath.c_str()))) //加载图像
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("离线图片加载失败[%1]!").arg(QString::fromLocal8Bit(strPath.c_str())), UILOG_ERR);
			break;;
		}

		int iwidth = img.width();
		int iheight = img.height();

		int index = pFile->getValue(IMAGE_OFFLINE_IMAGETYPE, 0).toInt(0); ;
		if (index == 3) //图像减半算法
		{
			pProduct->setProductImageSize(iwidth, iheight / 2, Image_station);
		}
		else
		{
			try
			{
				pProduct->setProductImageSize(iwidth, iheight, Image_station);//
			}
			catch (const std::exception& m_exception)
			{

				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("产品积累导致运行内存不足！"), UILOG_POPRED);
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit(m_exception.what()), UILOG_WARN);
				SignalCheck::SetSignal(SYS_STOP);
			}
		}


		ImageBuffer *pBuffer = pProduct->getImageBuffer(QString::fromStdString(pathName), Image_station);//20200703后端端暂用
		if (pBuffer == NULL)
		{
			break;
		}
		unsigned char *img_data = pBuffer->rawData.img_data;

		if (index == 0)			//原图处理
		{
			for (int j = 0; j < iheight; j++)
			{
				memcpy(img_data + iwidth*j, img.bits() + img.bytesPerLine() * j, iwidth);
			}
		}
		else if (index == 1)	//插值算法
		{
			for (int j = 0; j < iheight; )
			{
				memcpy(img_data + iwidth*j, img.bits() + img.bytesPerLine() * j, iwidth);
				memcpy(img_data + iwidth*(j + 1), img.bits() + img.bytesPerLine() * j, iwidth);
				j += 2;
			}
		}
		else if (index == 2)	//均值算法
		{
			for (int j = 0; j < iheight; j++)
			{
				unsigned char* pCur = img.bits() + img.bytesPerLine() * j;
				unsigned char* pNext = img.bits() + img.bytesPerLine() * j;
				if (j + 1 < iheight)
				{
					pNext = img.bits() + img.bytesPerLine() * (j + 1);
				}
				for (int k = 0; k < iwidth; k++)
				{
					*(img_data + iwidth*j + k) = (*(pCur + k) + *(pNext + k)) / 2;
				}
			}
		}
		else if (index == 3)	//图像减半算法
		{
			for (int j = 0; j < iheight / 2; j++)
			{
				memcpy(img_data + iwidth*j, img.bits() + img.bytesPerLine() * j * 2, iwidth);
			}
		}
		else
		{

		}
	} while (0);

//	pProduct->setChannelAllStatus(QString::fromStdString(strChannel), en_grapfinish);
}


stResImage CInvokeArithmetic::getPreProcessImage(string& strChannel, CImageProduct* pProduct)
{
	if (pProduct == NULL || strChannel.empty())
	{
		return stResImage();
	}
	stResImage  buff ;
	buff  = pProduct->getImageBuffer(QString::fromStdString(strChannel))->rawData;
	return  buff;
}

void CInvokeArithmetic::ThreadImageProcess(int param)
{
    int  Index= (int)param;
	bool benable = isImageStationEnable(Index);
	while (m_bProcessThread && benable)
	{

		benable = isImageStationEnable(Index);
		
        if (Index == m_iIndexDlArithmetic)//算法需要在同一个线程初始化与计算
		{
            if (m_enableConfig.bImageSYDV && m_pIDlArithmetic != NULL)
			{
				if (0 != m_pIDlArithmetic->initDeepLearnEnv())
				{
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("工站%1 算法线程退出").arg(Index), UILOG_WARN);
					return;
				}
			}
		}
		if (Index == m_iIndexDlArithmetic0)
		{
			if (m_enableConfig.bImageSYDV && m_pIDlArithmetic0 != NULL)
			{
				if (0 != m_pIDlArithmetic0->initDeepLearnEnv())
				{
					CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("工站%1 算法线程退出").arg(Index), UILOG_WARN);
					return;
				}
			}
		}
        CImageProduct* pProduct = NULL;
        pProduct = CProductUnion::getInstance()->getProductAllOnStatus(en_grapfinish,Index);
		if (pProduct == NULL)
		{
            Sleep(5);
			continue;
		}
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("获取采图完成状态产品成功，工站%1,Sn(%2)").arg(Index).arg(pProduct->getProductSn()));
        int iRet = 0;
      
       if(Index == m_iIndexDlArithmetic) // 采图结束后，调用AI算法进行缺陷检测
       {
		   pProduct->setEndGrapImageTime();
		  ImageForAi(pProduct, Index);
			//pProduct->setStartProcessTime();
			if (m_enableConfig.bImageSYDV)//垂直工位暂时不计算时间
			{
				uint64 iElapsedTime;
				iRet = m_pIDlArithmetic->runDeepLearn(pProduct, iElapsedTime);
               //m_pIDlArithmetic->startDefectLevel(pProduct);
				//pushDvElapsedTime(iElapsedTime);
			}
			else {
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("算法使用已关使能"),UILOG_WARN);
			}
			//pProduct->setEndProcessTime();
       }
	   else if (Index == m_iIndexDlArithmetic0)
	   {
			if (m_enableConfig.bImageSYDV)//
			{
				uint64 iElapsedTime;
				iRet = m_pIDlArithmetic0->runDeepLearn(pProduct, iElapsedTime);
			}
			else {
				CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("算法使用已关使能"), UILOG_WARN);
			}
		}
        pProduct->setProductAllStatus(en_extractfinish, Index);
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("算法缺陷提取完成，工站%1,Sn(%2)").arg(Index).arg(pProduct->getProductSn()));
	}

	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("工站%1 算法线程退出").arg(Index), UILOG_WARN);
}


void CInvokeArithmetic::ThreadImageDeepLearnProcess( void* param )
{
	
//   CInvokeArithmetic* pWorkFlowFun = (CInvokeArithmetic*)param;
//   int iRet = 0;  
//   while (m_bDeepLearnThread)
//   {     
//       if(m_enableConfig.bImageDV || m_enableConfig.bImageSYDV)
//       {
//			if (0 != getDlArithmetic()->unInitDeepLearnEnv())
//			{
//				CSendMessage::GetInstance()->Send("UnInitDeepLearn error!", UILOG_ERR);
//				return;
//			}
//
//           iRet = getDlArithmetic()->initDeepLearnEnv();
//           if (iRet < 0)
//           {
//               CSendMessage::GetInstance()->Send("initDVEnv error!", UILOG_ERR);
//               return;
//           }
//           return;
//		}
//        CImageProduct* pProduct  = NULL;
//		pProduct = CProductUnion::getInstance()->getProductAllOnStatus(en_multiChannelfinish);
//        if (pProduct == NULL)
//        {
//			Sleep(5);
//            continue;
//        }

//        if (er_no_error != pProduct->getProcessError())
//        {
//            pProduct->setProductAllStatus(en_extractfinish);
//            continue;
//        }
//		pProduct->setStartProcessTime();
//		if (m_enableConfig.bImageDV || m_enableConfig.bImageSYDV)
//		{
//			uint64 iElapsedTime;
//			iRet = getDlArithmetic()->runDeepLearn(pProduct, iElapsedTime);
//			pushDvElapsedTime(iElapsedTime);
//		}
//		pProduct->setEndProcessTime();
//		pProduct->setProductAllStatus(en_extractfinish);
//    }
}

void CInvokeArithmetic::ThreadUpdateResult( void* param )
{
    CInvokeArithmetic* pWorkFlowFun = (CInvokeArithmetic*)param;
    int iRet = 0;

    while (m_bUpdateResultThread)
    {     
        CImageProduct* pProduct  = NULL;
		pProduct =  CProductUnion::getInstance()->getProductAllOnStatus(en_extractfinish);//union
		
		if (pProduct == NULL)
		{
			Sleep(5);
			continue;;
		}
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("获取算法缺陷提取完成状态产品成功,Sn(%1)").arg(pProduct->getProductSn()));
		// 數據統計
		defectLevelAndSaveFile(pProduct);
		//  更新结果到UI
        updataResultToUI(pProduct);
    }
}
void CInvokeArithmetic::SaveCuseImage(CImageProduct* pProduct)
{
	if (NULL == pProduct)
	{
		return;
	}
	vector<string> vect;
	getEnabelChannle(vect);
	QString imageName = "";
	for (int i = 0; i < vect.size(); i++)
	{
		string enChannel = vect.at(i);
		QString strChannelName = QString::fromStdString(enChannel);
		//procuct(7)-DOWN_A-rawImge.bmp
		//QString fileName = QString::fromLocal8Bit("procuct(%1)-%2_%3-rawImge").arg(strSn).arg(Utils::getCamSuffix(Image_station)).arg(strChannel.c_str());
		imageName = QString("procuct(%1)-%2-%3").arg(pProduct->getProductSn()).arg(strChannelName).arg("rawImge");
		
		if (NULL != pProduct->getImageBuffer(strChannelName)->rawData.img_data)
		{
			saveRawImgage(pProduct->getImageBuffer(strChannelName)->rawData.img_data, pProduct->getProductWidth(), pProduct->getProductHeight(), imageName.toStdString());

		}
	}
	
}
void CInvokeArithmetic::ThreadSaveImage(int param)
{
	int  Index = (int)param;
	bool benable = isImageStationEnable(Index);
	while (m_bSaveImageThread && benable)
	{
		CImageProduct* pProduct = NULL;
		pProduct = CProductUnion::getInstance()->getProductAllOnStatus(en_updateuifinish, Index);
		if (pProduct == NULL)
		{
			Sleep(5);
			continue;
		}
		//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("获取更新UI完成产品成功,Sn(%1)").arg(pProduct->getProductSn()), UILOG_WARN);
		eProcessErroeCode LastError = pProduct->getProcessError();
		// 算法报错的图像保存 add20200722 start
		bool resSaveErrorImage = false;
		if (m_enableConfig.bSaveDvErRawImge && (er_DV_timeout == LastError || er_DV == LastError))
		{
			resSaveErrorImage = true;
		}
		if (m_enableConfig.bSaveRawImge || resSaveErrorImage)
		{
			CImageStation *pCImageStation  =CImageStationManager::GetInstance()->getImageStation(Index);
			if (pCImageStation != NULL)
			{
				pCImageStation->saveRawImage(pProduct);
			}
		}
		pProduct->setProductAllStatus(en_saveResImagefinish, Index);
		//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("保存结果图片完成，工站%1，Sn(%2)").arg(Index).arg(pProduct->getProductSn()), UILOG_WARN);
	}
}

void CInvokeArithmetic::ThreadRecyclingProcess( void* param )
{
    CInvokeArithmetic* pWorkFlowFun = (CInvokeArithmetic*)param;
    int iRet = 0;
	while (m_bRecyclingThread)
	{
		CImageProduct* pProduct = NULL;
		pProduct = CProductUnion::getInstance()->getProductAllOnStatus(en_saveResImagefinish);
		if (pProduct != NULL)
		{
			//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("获取保存结果完成产品成功,Sn(%1)").arg(pProduct->getProductSn()), UILOG_WARN);
			pProduct->setProductAllStatus(en_processfinish);
			//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("处理完成,Sn(%1)").arg(pProduct->getProductSn()), UILOG_WARN);
		}
		pProduct = CProductUnion::getInstance()->getProductAllOnStatus(en_processfinish);
		if (pProduct != NULL && pProduct->getUpdateUIFlag() && !pProduct->isFree())
		{
			//CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("获取处理完成产品成功,Sn(%1，产品状态可释放)").arg(pProduct->getProductSn()), UILOG_WARN);
			CProductUnion::getInstance()->releaseProduct(pProduct);
		}
		else
		{
			Sleep(5);
		}

    }
}

void CInvokeArithmetic::readEnableConfig()
{
	IFileVariable *pCameraSetCfgFile = getCameraSetCfgFileVariable();
	if (pCameraSetCfgFile == NULL)
	{
		return;
	}

    m_enableConfig.bImageDV = pCameraSetCfgFile->getValue(IMAGE_PARAM_DEFECT_DETECT_ENABLE,false).toBool();
	m_enableConfig.bImageSYDV = pCameraSetCfgFile->getValue(IMAGE_PARAM_H_SY_DETECT_ENABLE,false).toBool();
	m_enableConfig.bNeedCutImage = pCameraSetCfgFile->getValue(IMAGE_PARAM_CUT_IMAGE).toBool();
	m_enableConfig.bStaticShading = pCameraSetCfgFile->getValue(IMAGE_PARAM_STATIC_ENABLE, false).toBool();
	m_enableConfig.bDynamicShading = pCameraSetCfgFile->getValue(IMAGE_PARAM_DYNAMIC_ENABLE, false).toBool();
	m_enableConfig.bSignalFilter = pCameraSetCfgFile->getValue(IMAGE_PARAM_FILTER_ENABLE, false).toBool();
	m_enableConfig.bThreshold = pCameraSetCfgFile->getValue(IMAGE_PARAM_THRESHOLD_ENABLE, false).toBool();
	m_enableConfig.bInner = pCameraSetCfgFile->getValue(IMAGE_PARAM_INNER_ENABLE, false).toBool();
	m_enableConfig.bEdgeDetect = pCameraSetCfgFile->getValue(IMAGE_PARAM_EDGEDETECT_ENABLE, false).toBool();
	m_enableConfig.bCornerDetect = pCameraSetCfgFile->getValue(IMAGE_PARAM_CORNERDETECT_ENABLE, false).toBool();
	m_enableConfig.bListionHole = pCameraSetCfgFile->getValue(IMAGE_PARAM_LISTIONHOLE_ENABLE, false).toBool();
	m_enableConfig.bClassification = pCameraSetCfgFile->getValue(IMAGE_PARAM_CLASSIFICATION_ENABLE, false).toBool();
	m_enableConfig.bSaveRawImge = pCameraSetCfgFile->getValue(IMAGE_PARAM_SAVE_SIMAGE_ENABLE,false).toBool();
	m_enableConfig.bSaveDvErRawImge = pCameraSetCfgFile->getValue(IMAGE_PARAM_SAVE_DV_ER_SIMAGE_ENABLE, false).toBool();
	m_enableConfig.bSaveCutcImage = pCameraSetCfgFile->getValue(IMAGE_PARAM_CUT_SAVE_IMAGE,false).toBool();
	m_enableConfig.bSaveStaticImage = pCameraSetCfgFile->getValue(IMAGE_PARAM_STATIC_SAVE_ENABLE,false).toBool();
	m_enableConfig.bSaveDynImage = pCameraSetCfgFile->getValue(IMAGE_PARAM_DYNAMIC_SAVE_ENABLE,false).toBool();
	m_enableConfig.bSaveFiltImage = pCameraSetCfgFile->getValue(IMAGE_PARAM_FILTER_SAVE_ENABLE, false).toBool();
	m_enableConfig.bSavebThresholdImage = pCameraSetCfgFile->getValue(IMAGE_PARAM_THRESHOLD_SAVE_ENABLE, false).toBool();
	m_enableConfig.bSaveResultFile = pCameraSetCfgFile->getValue(IMAGE_PARAM_SAVE_FILE_ENABLE, true).toBool();
	m_enableConfig.bDefectLevel = pCameraSetCfgFile->getValue(IMAGE_PARAM_DEFECT_LEVEL_ENABLE, true).toBool();
    m_enableConfig.bDvbUnitEF = pCameraSetCfgFile->getValue(IMAGE_PARAM_H_ENABLEBUNITE, true).toBool();
    m_enableConfig.bMeasrue = pCameraSetCfgFile->getValue(IMAGE_PARAM_MEASURE_ENABLE, true).toBool();

    pCameraSetCfgFile->bindValue(IMAGE_PARAM_DEFECT_DETECT_ENABLE, &m_enableConfig.bImageDV);
    pCameraSetCfgFile->bindValue(IMAGE_PARAM_H_SY_DETECT_ENABLE, &m_enableConfig.bImageSYDV);
    pCameraSetCfgFile->bindValue(IMAGE_PARAM_H_ENABLEBUNITE, &m_enableConfig.bDvbUnitEF);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_CUT_IMAGE, &m_enableConfig.bNeedCutImage);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_STATIC_ENABLE, &m_enableConfig.bStaticShading);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_DYNAMIC_ENABLE, &m_enableConfig.bDynamicShading);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_FILTER_ENABLE, &m_enableConfig.bSignalFilter);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_THRESHOLD_ENABLE, &m_enableConfig.bThreshold);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_INNER_ENABLE, &m_enableConfig.bInner);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_EDGEDETECT_ENABLE, &m_enableConfig.bEdgeDetect);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_CORNERDETECT_ENABLE, &m_enableConfig.bCornerDetect);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_LISTIONHOLE_ENABLE, &m_enableConfig.bListionHole);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_CLASSIFICATION_ENABLE, &m_enableConfig.bClassification);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_SAVE_SIMAGE_ENABLE, &m_enableConfig.bSaveRawImge);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_SAVE_DV_ER_SIMAGE_ENABLE, &m_enableConfig.bSaveDvErRawImge);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_CUT_SAVE_IMAGE, &m_enableConfig.bSaveCutcImage);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_STATIC_SAVE_ENABLE, &m_enableConfig.bSaveStaticImage);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_DYNAMIC_SAVE_ENABLE, &m_enableConfig.bSaveDynImage);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_FILTER_SAVE_ENABLE, &m_enableConfig.bSaveFiltImage);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_THRESHOLD_SAVE_ENABLE, &m_enableConfig.bSavebThresholdImage);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_SAVE_FILE_ENABLE, &m_enableConfig.bSaveResultFile);
	pCameraSetCfgFile->bindValue(IMAGE_PARAM_DEFECT_LEVEL_ENABLE, &m_enableConfig.bDefectLevel);
    pCameraSetCfgFile->bindValue(IMAGE_PARAM_MEASURE_ENABLE, &m_enableConfig.bMeasrue);

    m_bIsFastGrab = pCameraSetCfgFile->getValue(IMAGE_PARAM_FAST_GRAB_ENABLE, false).toBool();
    pCameraSetCfgFile->bindValue(IMAGE_PARAM_FAST_GRAB_ENABLE, &m_bIsFastGrab);
}

void CInvokeArithmetic::readOtherConifg()
{
    IFileVariable *pUserSettingFile = getCurrentProgramFileVariable(USERSETTING_CFG_FILE);
    if (NULL != pUserSettingFile)
    {
        m_catoutParam.thresholdE[0] = pUserSettingFile->getValue(CUT_OUT_THRESHOLD_E1, 50).toInt();
        m_catoutParam.thresholdE[1] = pUserSettingFile->getValue(CUT_OUT_THRESHOLD_E2, 200).toInt();
        m_catoutParam.thresholdF[0] = pUserSettingFile->getValue(CUT_OUT_THRESHOLD_F1, 100).toInt();
        m_catoutParam.thresholdF[1] = pUserSettingFile->getValue(CUT_OUT_THRESHOLD_F2, 110).toInt();
        m_catoutParam.diliation1[0] = pUserSettingFile->getValue(CUT_OUT_DILIATION11, 1).toInt();
        m_catoutParam.diliation1[1] = pUserSettingFile->getValue(CUT_OUT_DILIATION12, 5).toInt();
        m_catoutParam.diliation2[0] = pUserSettingFile->getValue(CUT_OUT_DILIATION21, 21).toInt();
        m_catoutParam.diliation2[1] = pUserSettingFile->getValue(CUT_OUT_DILIATION22, 15).toInt();
        m_catoutParam.addValue[0] = pUserSettingFile->getValue(CUT_OUT_ADD_VAL1, 3).toInt();
        m_catoutParam.addValue[1] = pUserSettingFile->getValue(CUT_OUT_ADD_VAL2, 0).toInt();

        pUserSettingFile->bindValue(CUT_OUT_THRESHOLD_E1, &m_catoutParam.thresholdE[0]);
        pUserSettingFile->bindValue(CUT_OUT_THRESHOLD_E2, &m_catoutParam.thresholdE[1]);
        pUserSettingFile->bindValue(CUT_OUT_THRESHOLD_F1, &m_catoutParam.thresholdF[0]);
        pUserSettingFile->bindValue(CUT_OUT_THRESHOLD_F2, &m_catoutParam.thresholdF[1]);

        pUserSettingFile->bindValue(CUT_OUT_DILIATION11, &m_catoutParam.diliation1[0]);
        pUserSettingFile->bindValue(CUT_OUT_DILIATION12, &m_catoutParam.diliation1[1]);
        pUserSettingFile->bindValue(CUT_OUT_DILIATION21, &m_catoutParam.diliation2[0]);
        pUserSettingFile->bindValue(CUT_OUT_DILIATION22, &m_catoutParam.diliation2[1]);

        pUserSettingFile->bindValue(CUT_OUT_ADD_VAL1, &m_catoutParam.addValue[0]);
        pUserSettingFile->bindValue(CUT_OUT_ADD_VAL2, &m_catoutParam.addValue[1]);

    }

    QString strChannelNameList = getCurrentProgramRecipesList().c_str();
    if (strChannelNameList.isEmpty())
    {
        strChannelNameList = "SEQUENCE_A,SEQUENCE_B,SEQUENCE_C,SEQUENCE_D,SEQUENCE_E,SEQUENCE_F,SEQUENCE_G,SEQUENCE_H";
    }
    QStringList strNameList = strChannelNameList.split(",");
    for (int i = 0; i < strNameList.size() && strNameList.size() <= SEQUE_NUMBER; i++)
    {
        string strChannelName = strNameList[i].toStdString();
        IFileVariable* pFileVariable = getCurrentThresholdVariable();
        m_InterpolationParam[i].thresholdMin = pFileVariable->getValue(strChannelName, INTERPOLATION_MIN_THRESHOLD, 0).toInt();
        pFileVariable->bindValue(strChannelName, INTERPOLATION_MIN_THRESHOLD, &m_InterpolationParam[i].thresholdMin);
        m_InterpolationParam[i].thresholdMax = pFileVariable->getValue(strChannelName, INTERPOLATION_MAX_THRESHOLD, 30).toInt();
        pFileVariable->bindValue(strChannelName, INTERPOLATION_MAX_THRESHOLD, &m_InterpolationParam[i].thresholdMax);
    }
    m_catoutParam;
}

void CInvokeArithmetic::defectLevelAndSaveFile(CImageProduct* pProduct)
{

	//union end
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	bool bEnH = false;
	if (pFile)
	{
		bEnH = pFile->getValue(IMAGE_PARAM_ENABLE_H_LEVEL, true).toBool();
	}
	// 數據統計
	
	QString addData = "";
	for (int k = 0; k < pProduct->pDefectStrutVec->size(); k++)
	{
		stDefectFeature *mark = pProduct->pDefectStrutVec->at(k);

		 //等级分类信息统计
		mark->iIndex = k;
		int iDefectLevel = mark->iDefectLevel;
		if (bEnH&&mark->iH_DefectLevel > 0)
		{
			iDefectLevel = mark->iH_DefectLevel;
		}

		int iDefectType = mark->iDefectType;
		if (iDefectLevel > 0 && iDefectLevel < DEFECT_LEVEL_SIZE)
		{
			// 每一种缺陷的个数,共 11 种缺陷
			pProduct->m_stProductSummary.defectSummaryArr[iDefectType] ++;
			// 每一种等级的个数,共 7个等级
			pProduct->m_stProductSummary.defectLevelSummaryArr[iDefectLevel]++;
			// 每一种缺陷,每一种等级的个数.
			pProduct->m_stProductSummary.defectSingleLevelCntArr[iDefectType][iDefectLevel]++;
			// 把脏污按照  1 脏污 ，2 毛发，3 尘点 进行细分统计个数
			if (mark->iDvDefectType < m_defectNameVec.size())
			{
				pProduct->m_stProductSummary.dvdefectSummaryArr[mark->iDvDefectType]++;
				pProduct->m_stProductSummary.dvdefectSummaryAreaArr[mark->iDvDefectType] += mark->dSizeArea;
			}
		}
		mark->absposx = mark->posx;
		mark->absposy = mark->posy;
		//mark->regionPos = DefectPosToRegion(pProduct->getProductWidth(CImageProduct::Image_second), pProduct->getProductHeight(CImageProduct::Image_second), mark->posx, mark->posy);
		//mark->regionPos = DefectPosToRegion(pProduct->getProductWidth(CImageProduct::Image_first), pProduct->getProductHeight(CImageProduct::Image_first), mark->posx, mark->posy);
		pProduct->m_stProductSummary.dvdefectRegionCnt[mark->regionPos] += 1;
		//
		QString data = "";
		SaveProductResult(pProduct->getProductSn(), mark, data);
		addData += data;
	}
	//SaveOneProductResult(pProduct->getProductSn(), addData);
	pProduct->m_stProductSummary.iDefectNumber = pProduct->pDefectStrutVec->size();
	//SaveProductSummaryResult(pProduct->getProductSn(), &pProduct->m_stProductSummary);
	if (m_csvFile != NULL)
	{
		m_csvFile->close();
	}
	//
	int isize = pProduct->pDefectStrutVec->size();
	pProduct->m_stProductSummary.iDefectNumber = isize;
	
	eProcessErroeCode flagEcode = pProduct->getProcessError();
	pProduct->m_stProductSummary.ResultFlag = res_OK3; //默认 RI
	if (flagEcode == er_no_error)//没有错误
	{
		if (0 == isize)
		{
			pProduct->m_stProductSummary.ResultFlag = res_OK;
		}
		else if (m_pIDefectQualification != NULL)
		{
			pProduct->m_stProductSummary.ResultFlag = m_pIDefectQualification->getProductFinalRes(&pProduct->m_stProductSummary);
		}

	}
	else if (flagEcode == er_DV_timeout || er_DV == flagEcode)
	{
		pProduct->m_stProductSummary.ResultFlag = res_recheck;
	}
	
	//------------- OK NG 与前端机台结果计算
	if (getEnableFrontSig())
	{
		if (!CFrontPCtoBack::getInstance()->getForntMsgBySn(pProduct->getProductSn(), pProduct->m_MarkFrontRes))
		{
			CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("Sn:1% 获取前端数据失败，异常产品！").arg(pProduct->getProductSn()), UILOG_ERR);
			pProduct->m_MarkFrontRes.ResultFlag = res_Ng;
		}
		pProduct->m_stProductSummary.str2Dcode= pProduct->m_MarkFrontRes.str2Dcode;
		unsigned int iSn = pProduct->getProductSn();
		stProductDataRes  SendPcRes = pProduct->m_MarkFrontRes;
		enResFlag MyRes = pProduct->m_stProductSummary.ResultFlag;
		// 默认前端结果OK
		enResFlag iFinalRes = MyRes;
		enResFlag iFrontRes = pProduct->m_MarkFrontRes.ResultFlag;
		if (!pProduct->m_MarkFrontRes.IsOkFlag())//前端不OK
		{
			iFinalRes = iFrontRes;
			if (res_Ng==pProduct->m_MarkFrontRes.ResultFlag)
			{
				pProduct->m_stProductSummary.iProductNgDefectType = pProduct->m_MarkFrontRes.iNgOrRiDefectType;
			}
			else {
				pProduct->m_stProductSummary.iProductRiDefectType = pProduct->m_MarkFrontRes.iNgOrRiDefectType;
			}
			
		}
		pProduct->m_stProductSummary.ResultFlag = iFinalRes;
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("产品(%1)前机结果 %2 ，本机结果 %3，最终结果 %4").arg(iSn).arg(resultFlagToStr(iFrontRes)).arg(resultFlagToStr(MyRes)).arg(resultFlagToStr(iFinalRes)));
	}

	enResFlag ResultFlag = res_unknow;
	bool enableRandomSend = CBackPCtoFront::getInstance()->randomResult((int*)&ResultFlag);
	if (enableRandomSend)
	{
		pProduct->m_stProductSummary.ResultFlag = ResultFlag;
		CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("使能随机结果！"), UILOG_WARN);
	}
	//
}
QString CInvokeArithmetic::resultFlagToStr(enResFlag flag)
{
	QString flagStr = "RI";
	if (flag == enResFlag::res_OK || flag == enResFlag::res_OK2)
	{
		flagStr = "OK";
	}
	else if (flag == enResFlag::res_Ng)
	{
		flagStr = "NG";
	}
	return flagStr;
}
QString CInvokeArithmetic::GetSaveResultSavePath(QString strPath)
{
	QString strDir = getSaveResultFilePath();

	QString strtDatePth = getCurrentProducsTimeStrPath();
	
	QString strSavePath = QString("%1/%2").arg(strDir).arg(strtDatePth);
	// 路径不存在，创建路径
	QDir dir2(strSavePath);
	if (!dir2.exists(strSavePath))
	{
		dir2.mkdir(strSavePath);
	}
	//
	if (!strPath.isEmpty())
	{
		QString strAddSavePath = QString("%1/%2").arg(strSavePath).arg(strPath);
		// 路径不存在，创建路径
		QDir dir2(strAddSavePath);
		if (!dir2.exists(strAddSavePath))
		{
			dir2.mkdir(strAddSavePath);
		}
		return strAddSavePath;
	}
	//
	return strSavePath;
}
#define CSV_FILE_SN_DEFECT_HEADER "通道,序列,等级,H等级,形状,Size,类型,区域,对比度,得分,灰度,宽度,高度,X,Y,缺陷来源,绝对位置X,绝对位置Y,区域位置,H类型"
void CInvokeArithmetic::SaveProductResult(unsigned long long proSN, stDefectFeature *mark,QString &data)
{
	if (m_csvFile == NULL || mark == NULL || !m_enableConfig.bSaveResultFile)
	{
		return;
	}
	QString strDir = getSaveResultFilePath();
	JTime curTime = JTime::CurrentLocalDateTime();
	QString strSavePath = GetSaveResultSavePath(szDayPath[getCurrentWorktimes()]);
	QString strtDatePth = getCurrentProducsTimeStrPath();
	QString strTitleValue = QString::fromLocal8Bit(CSV_FILE_SN_DEFECT_HEADER);
	if (!m_csvFile->isOpen())
	{
		QString csvFileName = strSavePath + "/" + strtDatePth+ "_"+ QString::number(proSN) + "_defectlist.csv";
		m_csvFile->setFileName(csvFileName);
		if (!m_csvFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
		{
			return;
		}
	
		m_csvFile->write(strTitleValue.toLocal8Bit().data());
	} 
		
	QString strValue = QString("%1%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20").arg("\n") \
		.arg(mark->strChannelName.c_str()) \
		.arg(mark->iIndex + 1) \
		.arg(QString("%1,%2").arg(mark->iDefectLevel).arg(mark->iH_DefectLevel)) \
		.arg(QString::fromLocal8Bit(g_ShapeFlag[mark->iShape < 0 ? 0 : mark->iShape])) \
		.arg(QString("%1/%2(%3/%4/%5/%6/%7/%8/%9)").arg(mark->iSize).arg(mark->dSizeArea, 0, 'f', 3).arg(mark->iSAreaSize[0]).arg(mark->iSAreaSize[1]).arg(mark->iSAreaSize[2]).arg(mark->iSAreaSize[3]).arg(mark->iSAreaSize[4]).arg(mark->iSAreaSize[5]).arg(mark->iSAreaSize[6])) \
		.arg(QString::fromStdString(m_defectNameVec[mark->iDefectType])) \
		.arg(QString::fromLocal8Bit(g_strFlag[mark->iposition])) \
		.arg(QString("%1(%2/%3/%4/%5/%6/%7/%8)").arg(mark->contrast).arg(mark->iScontrast[0]).arg(mark->iScontrast[1]).arg(mark->iScontrast[2]).arg(mark->iScontrast[3]).arg(mark->iScontrast[4]).arg(mark->iScontrast[5]).arg(mark->iScontrast[6])) \
		.arg(mark->fdetect_score) \
		.arg(QString("%1/%2/%3/%4").arg(mark->iSGray[0]).arg(mark->iSGray[1]).arg(mark->iSGray[2]).arg(mark->iSGray[3]))  \
		.arg(mark->dWide) \
		.arg(mark->dHigh) \
		.arg(mark->posx) \
		.arg(mark->posy) \
		.arg(mark->strOriginate.c_str())	\
		.arg(mark->absposx)	\
		.arg(mark->absposy)	\
		.arg(mark->regionPos)\
	    .arg(mark->iDvDefectType);

	m_csvFile->write(strValue.toLocal8Bit());
	data = strValue;
	//
    return ;
}
void CInvokeArithmetic::SaveOneProductResult(unsigned long long proSN, QString &data)
{
	//需要一个总表//
	QString strDir = getSaveResultFilePath();
	JTime curTime = JTime::CurrentLocalDateTime();
	QString strSavePath = GetSaveResultSavePath();
	QString strDatePth = getCurrentProducsTimeStrPath();
	QString strTitleValue = QString::fromLocal8Bit(CSV_FILE_SN_DEFECT_HEADER);

	QString csvFileName = strSavePath + "/" + strDatePth + "_defectAll.csv";
	QString addData = "";
	if (!QFile::exists(csvFileName))
	{
		addData = strTitleValue;
	}
	QFile csvFile(csvFileName);

	if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		return;
	}
	QString strTime = QString("time %1:%2:%3").arg(curTime.hour()).arg(curTime.minute()).arg(curTime.second());
	QString strSn = strTime + " SN:" + QString::number(proSN);
	addData += strSn+data + "\n";
	csvFile.write(addData.toLocal8Bit());
	csvFile.close();
}
void CInvokeArithmetic::SaveProductSummaryResult(unsigned long long proSN, stProductSummary *pProductSummary)
{

	if (pProductSummary == NULL || !m_enableConfig.bSaveResultFile)
	{
		return;
	}
	/*
	[defectSummary]
	defectCnt1=0
	defectCnt2=0
	defectCnt3=0
	defectCnt4=0
	defectCnt5=0
	defectCnt6=0
	defectCnt7=0
	defectCnt8=0
	defectCnt9=0
	defectCnt10=0
	defectCnt11=0

	[defectLevelSummary]
	LevelCnt1=0
	LevelCnt2=0
	LevelCnt3=0
	LevelCnt4=0
	LevelCnt5=0
	LevelCnt6=0
	LevelCnt7=0

	[singleDefectLevelCnt1]
	LevelCnt1=0
	LevelCnt2=0
	LevelCnt3=0
	LevelCnt4=0
	LevelCnt5=0
	LevelCnt6=0
	LevelCnt7=0

	[singleDefectLevelCnt2]
	LevelCnt1=0
	LevelCnt2=0
	LevelCnt3=0
	LevelCnt4=0
	LevelCnt5=0
	LevelCnt6=0
	LevelCnt7=0

	*/
	CIniFile iniFile;
	

	// 路径不存在，创建路径
	//QString strDir = getSaveResultFilePath();
	
	//JTime curTime = JTime::CurrentLocalDateTime();
	QString strSavePath = GetSaveResultSavePath(szDayPath[getCurrentWorktimes()]);
	QString strDatePth = getCurrentProducsTimeStrPath();
	QString csvFileName = strSavePath + "/" + strDatePth + "_" + QString::number(proSN) + "_Summary.ini";
	iniFile.Initialize(csvFileName.toStdString());

    for (int i = 1; i < m_defectNameVec.size();i++)
	{
		QString keyname = "defectSummary";
		QString valuename = "defectCnt" + QString::number(i);
		iniFile.SetValueI(keyname.toStdString(), valuename.toStdString(), pProductSummary->defectSummaryArr[i]);
	}
	
	for (int i = 1; i < DEFECT_LEVEL_SIZE; i++)
	{
		QString keyname = "defectLevelSummary";
		QString valuename = "LevelCnt" + QString::number(i);
		iniFile.SetValueI(keyname.toStdString(), valuename.toStdString(), pProductSummary->defectLevelSummaryArr[i]);
	}

    for (int i = 1; i < m_defectNameVec.size(); i++)
	{
		for (int j = 1; j < DEFECT_LEVEL_SIZE; j++)
		{
			QString keyname = "singleDefectLevelCnt" + QString::number(i);
			QString valuename = "LevelCnt" + QString::number(j);
			iniFile.SetValueI(keyname.toStdString(), valuename.toStdString(), pProductSummary->defectSingleLevelCntArr[i][j]);
		}

	}

    for (int i = 1; i < m_defectNameVec.size(); i++)
	{
		QString keyname = "dvdefectSummary";
		QString valuename = "dvdefectCnt" + QString::number(i);
		QString valuename2 = "dvdefectArea" + QString::number(i);
		iniFile.SetValueI(keyname.toStdString(), valuename.toStdString(), pProductSummary->dvdefectSummaryArr[i]);
		iniFile.SetValueF(keyname.toStdString(), valuename2.toStdString(), pProductSummary->dvdefectSummaryAreaArr[i]);
	}

	for (int i = 1; i < DEFECT_REGION_SIZE; i++)
	{
		QString keyname = "RegionSummary";
		QString valuename = "Region" + QString::number(i);
		iniFile.SetValueI(keyname.toStdString(), valuename.toStdString(), pProductSummary->dvdefectRegionCnt[i]);
	}

	iniFile.WriteFile();
	return;

}

void CInvokeArithmetic::saveProcessImgage( unsigned char* pImageData,unsigned int iwidth,unsigned iHeight,string name )
{
	if (pImageData == NULL)
	{
		return;
	}
    string  strPath = "D:\\IMAGE";
    IFileVariable* pfileVariable= getCameraSetCfgFileVariable();
    if (pfileVariable != NULL)
    {
        strPath = pfileVariable->getValue("image", "savepath", strPath.c_str()).toCString();
    }
    //JTime curTime = JTime::CurrentLocalDateTime();
	QString strDatePth = getCurrentProducsTimeStrPath();
    QString strSavePath = QString("%1\\%2").arg(strPath.c_str()).arg(strDatePth);
    QDir dir(strSavePath);
    if (!dir.exists(strSavePath))
    {
        dir.mkdir(strSavePath);
    }
    strSavePath = strSavePath + "\\" +"processimage";
    QDir dir2(strSavePath);
    if (!dir2.exists(strSavePath))
    {
        dir2.mkdir(strSavePath);
    }
    strSavePath = strSavePath + "\\" + QString::fromStdString(name);
    strSavePath+=".bmp";
    CImageProcess::SaveImageToFile(pImageData, iwidth, iHeight, strSavePath);
}

void CInvokeArithmetic::saveRawImgage(unsigned char* pImageData, unsigned int iwidth, unsigned iHeight, string name)
{
	string  strPath = "D:\\IMAGE";
	IFileVariable* pfileVariable = getCameraSetCfgFileVariable();
	if (pfileVariable != NULL)
	{
		strPath = pfileVariable->getValue("image", "savepath", strPath.c_str()).toCString();
	}

	//JTime curTime = JTime::CurrentLocalDateTime();
	QString strDatePth = getCurrentProducsTimeStrPath();
	QString strSavePath = QString("%1\\%2").arg(QString::fromLocal8Bit(strPath.c_str())).arg(strDatePth);
	QDir dir(strSavePath);
	if (!dir.exists(strSavePath))
	{
		dir.mkpath(strSavePath);
	}

	strSavePath = strSavePath + "\\" + QString::fromStdString(name);
	strSavePath += ".bmp";
	if (!QFile::exists(strSavePath))
	{
		CImageProcess::SaveImageToFile(pImageData, iwidth, iHeight, strSavePath);
	}

}

void CInvokeArithmetic::resetProcess()
{

    m_bUpdateResultThread = false;
    m_bSaveImageThread = false;
    m_bRecyclingThread = false;
	m_bWaitGrapFinishthread = false;

	/* 算法綫程需要常駐
	//m_bProcessThread = false;
    for(int i=0 ;i < getImageStationCnt();i++)
    {
        if(m_pProcessThreads[i] != NULL)
        {
            while (m_pProcessThreads[i]->IsRunning())
            {
                Sleep(5);
            }
        }
    }*/

    for(int i=0 ;i < getImageStationCnt();i++)
    {
        if(m_mapSaveImageThreads[i] != NULL)
        {
            while (m_mapSaveImageThreads[i]->IsRunning())
            {
                Sleep(5);
            }
        }
    }


	while (m_pUpdateResultThread != NULL && m_pUpdateResultThread->IsRunning())
	{
		Sleep(5);
	}

	while (m_pRecyclingThread != NULL && m_pRecyclingThread->IsRunning())
	{
		Sleep(5);
	}

	if (m_pIDlArithmetic)
	{
		m_pIDlArithmetic->readDVConifg(&m_enableConfig);
	}
	if (m_pIDlArithmetic0)
	{
		m_pIDlArithmetic0->readDVConifg(&m_enableConfig);
	}

	CProductUnion::getInstance()->releaseAllProduct();
	clearDvElapsedTime();
	startImageProcess();//若算法初始化失败后，复位重新启动算法线程
}

bool CInvokeArithmetic::isResetFinish()
{


	for (int i = 0;i < getImageStationCnt();++i)
	{
		while (m_mapSaveImageThreads[i] != NULL && m_mapSaveImageThreads[i]->IsRunning())
		{
			return false;
		}
	}

	if (m_pDeepLearnThread != NULL && m_pDeepLearnThread->IsRunning())
	{
		return false;
	}

	if (m_pUpdateResultThread != NULL && m_pUpdateResultThread->IsRunning())
	{
		return false;
	}

	if (m_pRecyclingThread != NULL && m_pRecyclingThread->IsRunning())
	{
		return false;
	}

	return true;
}

int CInvokeArithmetic::reProcessSpecialProduct(unsigned long long sn)
{
	CImageProduct* pProduct = CProductUnion::getInstance()->getProductBySn(sn);
	if (pProduct== NULL || !pProduct->isFree())
	{
		return -1;
	}
	pProduct->resetFlagStatus();
	pProduct->setStartGrapImageTime();
	pProduct->setProductAllStatus(en_grapfinish);

	return 0;
}

void CInvokeArithmetic::startUpdataToUI()
{
    if (!m_pUpdateResultThread->IsRunning())
    {
        m_bUpdateResultThread = true;
        m_pUpdateResultThread->StartThread(this,&CInvokeArithmetic::ThreadUpdateResult,this);
    }
}


uint64 CInvokeArithmetic::getDvElapsedTime()
{
	CISAPUGuard<JMutex> autoLock(m_ElapsedTimeLock);
	uint64 uiElapsedTime = 0;
	if (m_DVElapsedTimestack.size() ==0)
	{
		return 0;
	}
	uiElapsedTime = m_DVElapsedTimestack.top();
	m_DVElapsedTimestack.pop();
	return uiElapsedTime;
}


void CInvokeArithmetic::pushDvElapsedTime(uint64 uiElapsedTime)
{
	CISAPUGuard<JMutex> autoLock(m_ElapsedTimeLock);
	m_DVElapsedTimestack.push(uiElapsedTime);
}

void CInvokeArithmetic::clearDvElapsedTime()
{
	CISAPUGuard<JMutex> autoLock(m_ElapsedTimeLock);
	while (!m_DVElapsedTimestack.empty())
	{
		m_DVElapsedTimestack.pop();
    }
}

void CInvokeArithmetic::writeClassSizeCsv(const QString &strData)
{
	if (strData.isEmpty())
	{
		return;
	}
	QString strDir = getSaveResultFilePath();

	//JTime curTime = JTime::CurrentLocalDateTime();
	QString strDatePth = getCurrentProducsTimeStrPath();
	QString strSavePath = getCurrenTimeDayOrNightPath();
	QString csvFileName = strSavePath + "/" + strDatePth + "_glassSize.csv";

	//
	QString addData = "";
	if (!QFile::exists(csvFileName))
	{
		addData = QString::fromLocal8Bit("SN,A,B,C,D,E,F,G,H,I,J,K\n");
	}
	//
	QFile csvFile(csvFileName);

	if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		return;
	}
	addData += strData + "\n";

	csvFile.write(addData.toLocal8Bit());
	csvFile.close();
}

void CInvokeArithmetic::productFillWithHistoryData(CImageProduct* pProduct)
{
	readClassSizeCsv(pProduct);		// 读取玻璃尺寸
	readDefectLevel(pProduct);		// 读取产品缺陷等级信息
	readProductSummary(pProduct);	// 读取产品的统计信息
	// 填充
	updataResultToUI(pProduct);
}

void CInvokeArithmetic::readClassSizeCsv(CImageProduct* pProduct)
{
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile == NULL)
	{
		return;
	}

	string strPath = pFile->getValue(IMAGE_HISTORY_DATA_PATH).toCString();
	QDir dir2(QString::fromLocal8Bit(strPath.c_str()));
	if (!dir2.exists(QString::fromLocal8Bit(strPath.c_str())))
	{
		return;
	}
	dir2.cdUp();
	QString strDirName = dir2.dirName();
	QString csvFileName = QString::fromStdString(strPath)+"/" +strDirName + "_glassSize.csv";

	QFile csvFile(csvFileName);
	if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	QString title = csvFile.readLine();
	while (!csvFile.atEnd())
	{
		QString line = csvFile.readLine();
		QList<QString> attrList = line.split(",");
		if (attrList.size() != 11)
		{
			continue;
		}
		QString Sn = attrList.at(0);
		if (Sn.toULong() == pProduct->getProductSn())
		{
			pProduct->m_glassSize.insert(std::pair<string, double>("A", attrList.at(1).toDouble()));
			pProduct->m_glassSize.insert(std::pair<string, double>("B", attrList.at(2).toDouble()));
			pProduct->m_glassSize.insert(std::pair<string, double>("C", attrList.at(3).toDouble()));
			pProduct->m_glassSize.insert(std::pair<string, double>("D", attrList.at(4).toDouble()));
			pProduct->m_glassSize.insert(std::pair<string, double>("E", attrList.at(5).toDouble()));
			pProduct->m_glassSize.insert(std::pair<string, double>("F", attrList.at(6).toDouble()));
			pProduct->m_glassSize.insert(std::pair<string, double>("G", attrList.at(7).toDouble()));
			pProduct->m_glassSize.insert(std::pair<string, double>("H", attrList.at(8).toDouble()));
			pProduct->m_glassSize.insert(std::pair<string, double>("I", attrList.at(9).toDouble()));
			pProduct->m_glassSize.insert(std::pair<string, double>("J", attrList.at(10).toDouble()));
			break;
		}
	}
	csvFile.close();
	return;
}

void CInvokeArithmetic::readDefectLevel(CImageProduct* pProduct)
{

	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile == NULL)
	{
		return;
	}

	string strPath = pFile->getValue(IMAGE_HISTORY_DATA_PATH).toCString();
	QDir dir2(QString::fromLocal8Bit(strPath.c_str()));
	if (!dir2.exists(QString::fromLocal8Bit(strPath.c_str())))
	{
		return;
	}
	dir2.cdUp();
	QString strDirName = dir2.dirName();
	// "2020.05.02_30_defectlist.csv"+
	QString csvFileName = QString::fromLocal8Bit(strPath.c_str())+ "/"+ strDirName + "_" + QString::number(pProduct->getProductSn()) + "_defectlist.csv";
	if (!QFile::exists(csvFileName))
	{
		return;
	}

	QFile csvFile;
	csvFile.setFileName(csvFileName);
	if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	QByteArray lineq = csvFile.readLine();
	QString str = codec->toUnicode(lineq);
	//lineq = csvFile.readLine();
	while (!csvFile.atEnd()) 
	{
		QByteArray QALine = csvFile.readLine();
		QString line = codec->toUnicode(QALine);
		QList<QString> attrList = line.split(",");
		if (attrList.size() <19)
		{
			return;
		}

		stDefectFeature* mark = new stDefectFeature();
		mark->strChannelName = attrList.at(0).toStdString();
		mark->iIndex = attrList.at(1).toInt();
		mark->iDefectLevel = attrList.at(2).toInt();
		mark->iH_DefectLevel = attrList.at(3).toInt();
		mark->idetect_flag = 0;
		for (int i = 0; i < DEFECT_SHAPE_SIZE;i++)
		{
			if (attrList.at(4) == QString::fromLocal8Bit(g_ShapeFlag[i]))//
			{
				mark->iShape = i;
				break;
			}			 		
		}
		
		QString strSize = attrList.at(5);
		strSize = strSize.remove(")");
		strSize = strSize.replace("(", "/");
		if (strSize.split("/").size() == 9)
		{
			mark->iSize = strSize.split("/").at(0).toInt();
			mark->dSizeArea = strSize.split("/").at(1).toDouble();
			mark->iSAreaSize[0] = strSize.split("/").at(2).toInt();
			mark->iSAreaSize[1] = strSize.split("/").at(3).toInt();
			mark->iSAreaSize[2] = strSize.split("/").at(4).toInt();
			mark->iSAreaSize[3] = strSize.split("/").at(5).toInt();
			mark->iSAreaSize[4] = strSize.split("/").at(6).toInt();
			mark->iSAreaSize[5] = strSize.split("/").at(7).toInt();
			mark->iSAreaSize[6] = strSize.split("/").at(8).toInt();
		}

		mark->iDefectType = 0;
        for (int i = 0; i < m_defectNameVec.size(); i++)
		{
			if (attrList.at(6) == QString::fromStdString(m_defectNameVec[i]))
			{
				mark->iDefectType = i;
				break;
			}
		}
		
		mark->iposition = 0;
		for (int i = 0; i < DEFECT_FLAG_SIZE; i++)
		{
			if (attrList.at(7) == QString::fromLocal8Bit(g_strFlag[i]))
			{
				mark->iposition = i;
				break;
			}
		}	

		QString strcontrast = attrList.at(8);
		strcontrast = strcontrast.remove(")");
		strcontrast = strcontrast.replace("(", "/");
		if (strcontrast.split("/").size() == 8)
		{
			mark->contrast = strcontrast.split("//").at(0).toInt();
			mark->iScontrast[0] = strcontrast.split("/").at(1).toInt();
			mark->iScontrast[1] = strcontrast.split("/").at(2).toInt();
			mark->iScontrast[2] = strcontrast.split("/").at(3).toInt();
			mark->iScontrast[3] = strcontrast.split("/").at(4).toInt();
			mark->iScontrast[4] = strcontrast.split("/").at(5).toInt();
			mark->iScontrast[5] = strcontrast.split("/").at(6).toInt();
			mark->iScontrast[6] = strcontrast.split("/").at(7).toInt();

		}
		mark->fdetect_score = attrList.at(9).toFloat();
		
		QString strSGray = attrList.at(10);
		if (strSGray.split("/").size() == 4)
		{
			mark->iSGray[0] = strSGray.split("/").at(0).toInt();
			mark->iSGray[1] = strSGray.split("/").at(1).toInt();
			mark->iSGray[2] = strSGray.split("/").at(2).toInt();
			mark->iSGray[3] = strSGray.split("/").at(3).toInt();
		}	
		mark->dWide = attrList.at(11).toDouble();
		mark->dHigh = attrList.at(12).toDouble();
		mark->posx = attrList.at(13).toDouble();
		mark->posy = attrList.at(14).toDouble();
		mark->strOriginate = attrList.at(15).toStdString();		
		mark->absposx = attrList.at(16).toDouble();
		mark->absposy = attrList.at(17).toDouble();
		mark->regionPos = attrList.at(18).toInt();
		if (attrList.size() == 20)
		{
			mark->iDvDefectType = attrList.at(19).toInt();
		}
		pProduct->pDefectStrutVec->push_back(mark);	
	}

	csvFile.close();
	return;
}

void CInvokeArithmetic::readProductSummary(CImageProduct* pProduct)
{
	CIniFile iniFile;
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile == NULL)
	{
		return;
	}

	string strPath = pFile->getValue(IMAGE_HISTORY_DATA_PATH).toCString();
	QDir dir2(QString::fromLocal8Bit(strPath.c_str()));
	if (!dir2.exists(QString::fromLocal8Bit(strPath.c_str())))
	{
		return;
	}
	dir2.cdUp();
	QString strDirName = dir2.dirName();
	QString csvFileName = QString::fromLocal8Bit(strPath.c_str()) +"/"+ strDirName + "_" + QString::number(pProduct->getProductSn()) + "_Summary.ini";

	if (!QFile::exists(csvFileName))
	{
		return;
	}
	iniFile.Initialize(csvFileName.toStdString());
	bool breadFlag =  iniFile.ReadFile();

    for (int i = 1; i < m_defectNameVec.size(); i++)
	{
		QString keyname = "defectSummary";
		QString valuename = "defectCnt" + QString::number(i);
		//iniFile.SetValueI(keyname.toStdString(), valuename.toStdString(), pProduct->m_stProductSummary.defectSummaryArr[i]);
		pProduct->m_stProductSummary.defectSummaryArr[i] = iniFile.GetValueI(keyname.toStdString(), valuename.toStdString());
	}

	for (int i = 1; i < DEFECT_LEVEL_SIZE; i++)
	{
		QString keyname = "defectLevelSummary";
		QString valuename = "LevelCnt" + QString::number(i);
		pProduct->m_stProductSummary.defectLevelSummaryArr[i] = iniFile.GetValueI(keyname.toStdString(), valuename.toStdString());
	}

    for (int i = 1; i < m_defectNameVec.size(); i++)
	{
		for (int j = 1; j < DEFECT_LEVEL_SIZE; j++)
		{
			QString keyname = "singleDefectLevelCnt" + QString::number(i);
			QString valuename = "LevelCnt" + QString::number(j);
			pProduct->m_stProductSummary.defectSingleLevelCntArr[i][j] = iniFile.GetValueI(keyname.toStdString(), valuename.toStdString());
		}

	}

    for (int i = 1; i < m_defectNameVec.size(); i++)
	{
		QString keyname = "dvdefectSummary";
		QString valuename = "dvdefectCnt" + QString::number(i);
		QString valuename2 = "dvdefectArea" + QString::number(i);
		pProduct->m_stProductSummary.dvdefectSummaryArr[i] = iniFile.GetValueI(keyname.toStdString(), valuename.toStdString());
		pProduct->m_stProductSummary.dvdefectSummaryAreaArr[i] = iniFile.GetValueF(keyname.toStdString(), valuename2.toStdString());
	}


	for (int i = 1; i < DEFECT_REGION_SIZE; i++)
	{
		QString keyname = "RegionSummary";
		QString valuename = "Region" + QString::number(i);
		pProduct->m_stProductSummary.dvdefectRegionCnt[i] = iniFile.GetValueI(keyname.toStdString(), valuename.toStdString());
	}
	return;
}

int CInvokeArithmetic::DefectPosToRegion(int iwidth, int height, int posX, int posY)
{
	if (posX > iwidth || posY > height)
	{
		return 0;
	}
	const int iRowCnt = 6;
	const int iColumnCnt = 4;
	const int regionMap[iRowCnt][iColumnCnt] = { { 1,1,2,2 },{ 3,13,14,4 },{ 5,15,16,6 },{ 7,17,18,8 },{ 9,19,20,10 },{ 11,11,12,12 } };

	int  iwidthBlockSize = iwidth / iColumnCnt;
	int  iheightBlockSize = height / iRowCnt;
	int iRegion = 0;
	int iRow = posX / iwidthBlockSize;
	int iCol = posY / iheightBlockSize;
	if (iRow >= iColumnCnt || iCol >= iRowCnt)
	{
		return 0;
	}
	iRegion = regionMap[iCol][iRow];

	return iRegion;
}

void CInvokeArithmetic::ImageForAi(CImageProduct * pProduct, int iStation)
{
	IFileVariable *pFile = getCameraSetCfgFileVariable();
	if (pFile)
	{
		bool bEnable = pFile->getValue(IMAGE_PARAM_RIGHT_IMAGE_ENABLE).toBool();
		if (!bEnable)
		{
			return;
		}
	}
	if (NULL == pProduct)
	{
		return;
	}
	uint64 RunTime = JTime::CurrentMillisecond();
	vector<string> vecEnableChannel;
	getEnabelChannle(vecEnableChannel, to_string(iStation));
	int num = vecEnableChannel.size();
	unsigned char *Right_Src_img_data = NULL;
	int iHOffset = 0;
	vector<JThreadTP<CInvokeArithmetic, ImageBufferAndChannel*, void>*> runningThreads;
	for (size_t i = 0; i < num; i++)
	{
		ImageBufferAndChannel *imgTpm = new ImageBufferAndChannel;
		ImageBuffer* pImagebuffer = pProduct->getImageBuffer(QString::fromStdString(vecEnableChannel.at(i)), iStation);
		imgTpm->imgBuff = pImagebuffer;
		imgTpm->iStation = iStation;
		imgTpm->iChannel = i;
		imgTpm->iHOffset = iHOffset;
		if (i == 0 || i == 3)//3是D下机相
		{
			int iGrabMax = 20;//跳动值
			IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), SString::IntToStr(iStation));
			if (pFileVariable)
			{
				iGrabMax=pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_AI_IMAGE_MOVE).toInt();
			}
			unsigned char *Src_img_data = pImagebuffer->rawData.img_data;
			int iLeftIndex = 8190;
			int iRightIdex = 8195;
			
			int iLeftOldVal = *(Src_img_data + 2 * IMAGE_8K_WIDTH * 10 + iLeftIndex);
			int iRightOldVal = *(Src_img_data + 2 * IMAGE_8K_WIDTH * 10 + iRightIdex);
			int iMakLeftH = 0;
			int iMakRightH = 0;
			for (size_t irh = 11; irh < pImagebuffer->rawData.img_height/4; irh++)
			{
				int iLeftCurVal = *(Src_img_data + 2 * IMAGE_8K_WIDTH*irh + iLeftIndex);
				int iRightCurVal = *(Src_img_data + 2 * IMAGE_8K_WIDTH*irh + iRightIdex);
				if (abs(iLeftCurVal - iLeftOldVal)>iGrabMax && 0 == iMakLeftH)
				{
					iMakLeftH = irh;
				}
				if (abs(iRightCurVal - iRightOldVal)>iGrabMax && 0 == iMakRightH)
				{
					iMakRightH = irh;
				}
				if (0 != iMakLeftH && 0 != iMakRightH)
				{
					imgTpm->iHOffset = iMakRightH - iMakLeftH;
					iHOffset         = iMakRightH - iMakLeftH;
					break;
				}
			}
		}
		
		JThreadTP<CInvokeArithmetic, ImageBufferAndChannel*, void>* pthread = NULL;
		if (m_mapAiImgeThreads.find(vecEnableChannel.at(i)) == m_mapAiImgeThreads.end())
		{
			pthread = new JThreadTP<CInvokeArithmetic, ImageBufferAndChannel*, void>;
			m_mapAiImgeThreads[vecEnableChannel.at(i)] = pthread;
		}

		pthread = m_mapAiImgeThreads[vecEnableChannel.at(i)];
		if (!pthread->IsRunning())
		{
			pthread->StartThread(this, &CInvokeArithmetic::ThreadDAiImgeProcess, imgTpm);
			runningThreads.push_back(pthread);
		}
	}

	do
	{
		int  icnt = 0;
		for (int i = 0; i <runningThreads.size(); i++)
		{
			if (!runningThreads.at(i)->IsRunning())
			{
				icnt++;
			}
		}
		if (icnt == runningThreads.size())
		{
			break;
		}
		else
		{
			Sleep(10);
		}
	} while (true);
	CSendMessage::GetInstance()->Send(QString::fromLocal8Bit("Produc:%1;ImageForAi结束time :%2ms").arg(pProduct->getProductSn()).arg(JTime::CurrentMillisecond() - RunTime));
}
void CInvokeArithmetic::ThreadDAiImgeProcess(ImageBufferAndChannel* imgBuffer)
{
	//
	unsigned char *Right_Src_img_data = NULL;
	int iChal = imgBuffer->iChannel;
	int istation = imgBuffer->iStation;
	ImageBuffer* pImagebuffer = imgBuffer->imgBuff;
	if (pImagebuffer != NULL)
	{
		IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), SString::IntToStr(istation));

		bool bm_imge_Enable = false;
		bool bm_grab_Enable = false;
		bool bm_jz_Enable = false;

		int iRightHMoveOffset = imgBuffer->iHOffset;//正数产品右边在整图中上移--负数产品下移-----配置参数
		int iimagemove = 0;
		int igrab_val = 0;//灰度值 正为加 负数 为减
		int ijz_val = 0;
		if (pFileVariable)
		{
			QString KeyNameCfg = QString("Config_%1").arg(iChal);
			bm_grab_Enable = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_GRAB_IMAGE_ENABLE).toBool();
			bm_jz_Enable = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_AI_JZ_ENABLE).toBool();
			igrab_val = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_GRAB_IMAGE_VAL).toInt();
			ijz_val = pFileVariable->getValue(KeyNameCfg.toStdString(), ACQUISITION_AI_JZ_VAL).toInt();
			bm_imge_Enable = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_AI_IMAGE_ENABLE).toBool();
			
		}
		
		//add 产品右边上移或下移操作
		bool OnLine = true;
		if (OnLine)
		{
			unsigned char *Src_img_data = pImagebuffer->rawData.img_data;
			Right_Src_img_data = new unsigned char[IMAGE_8K_WIDTH * pImagebuffer->rawData.img_height];

			for (size_t ih = 0; ih < pImagebuffer->rawData.img_height; ih++)
			{
				//数据右边的先拷贝出来 一行
				if (bm_imge_Enable&&0!= iRightHMoveOffset)
				{
					if (iRightHMoveOffset>0)
					{
						//产品上移这个时间移动的高度右边不需要加数据
						if (ih + iRightHMoveOffset<pImagebuffer->rawData.img_height)
						{
							memcpy(Right_Src_img_data + ih*IMAGE_8K_WIDTH, Src_img_data + 2 * IMAGE_8K_WIDTH*(ih + iRightHMoveOffset) + IMAGE_8K_WIDTH, IMAGE_8K_WIDTH);//右8192
							memcpy(Src_img_data + 2 * IMAGE_8K_WIDTH*ih + IMAGE_8K_WIDTH, Right_Src_img_data + IMAGE_8K_WIDTH*ih, IMAGE_8K_WIDTH);//右8192
						}
					}
					else
					{
						//产品下移//
						memcpy(Right_Src_img_data + ih*IMAGE_8K_WIDTH, Src_img_data + 2 * IMAGE_8K_WIDTH*ih + IMAGE_8K_WIDTH, IMAGE_8K_WIDTH);//右8192
						if (ih>abs(iRightHMoveOffset))
						{
							memcpy(Src_img_data + 2 * IMAGE_8K_WIDTH*(ih)+IMAGE_8K_WIDTH, Right_Src_img_data + IMAGE_8K_WIDTH*(ih - abs(iRightHMoveOffset)), IMAGE_8K_WIDTH);//右8192
						}

					}
				}

				//灰度值操作 
				if (0 != igrab_val&&bm_grab_Enable )
				{
					for (size_t k = 0; k < IMAGE_8K_WIDTH; k++)
					{
						unsigned char* pValue = Src_img_data + 2 * IMAGE_8K_WIDTH*ih + IMAGE_8K_WIDTH + k;
						int ival = *(pValue);
						if (ival>4)
						{
							ival = ival + igrab_val;
							ival = (ival >= 0) ? ival : 0;
							*(pValue) = ival;
						}

					}
				}

			}
		}

	}
	
	if (Right_Src_img_data)
	{
		delete[]Right_Src_img_data;
		Right_Src_img_data = NULL;
	}
	//
}
//void CInvokeArithmetic::repairGigeImage(CImageProduct* pProduct)
//{
//	// 2K 相机采图出现丢一个触发，导致图片错误。人为修复 begin
//
//	for (int iMageIndex = 0; iMageIndex < CImageProduct::SEQUE_NUMBER; iMageIndex++)  // 前端，后端
//	{
//		string strSceneName = iMageIndex ==CImageProduct::Image_first ? SCENE_NAME_FIRST  : SCENE_NAME_SECOND;
//		CImageProduct::int iImageIndex = (CImageProduct::int)iMageIndex;
//		for (int iCamindex = 0; iCamindex < 2; iCamindex++)  // 圖象1 ，圖象2
//		{
//			bool bEnablerestore = false;
//			int iBlockLines = 0;
//			int iBlockCnt = 0;
//			string strCam1Sn;
//			IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), strSceneName);
//			QString KeyName = QString("ImageConfig_%1").arg(iCamindex);
//			if (pFileVariable != NULL)
//			{
//				bEnablerestore = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_ENABLE_RESTORE_IMAGE).toBool();
//				iBlockLines = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_BLOCKLINES).toInt();;
//				iBlockCnt = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_BLOCKCNT).toInt();
//				strCam1Sn = pFileVariable->getValue(KeyName.toStdString(), ACQUISITION_UPCAM1_SN).toCString();
//			}
//			if (!bEnablerestore)
//			{
//				continue;
//			}
//			ILineCameraFactoryManage *pCameraManager = (ILineCameraFactoryManage*)GetSDKInstance("cameramanager", ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE);
//			LINE_CAM_PARAM info;
//			if (pCameraManager->GetCameraInParam(strCam1Sn, info))
//			{
//				iBlockLines = info.blockSize;
//				if (info.blockSize > 0)
//				{
//					iBlockCnt = pProduct->getProductHeight() / info.blockSize;
//				}
//			}
//			bool bGigeCam = info.strCameraSerialNumber == info.strCameraID;
//			if (bEnablerestore && bGigeCam)
//			{
//				vector<string> vec;
//				getChannleByCamIndex(vec, iCamindex, strSceneName);
//				if (vec.size() == 2)
//				{
//					ImageBuffer* pImageBufferA = pProduct->getImageBuffer(QString::fromStdString(vec.at(0)), (CImageProduct::int)iMageIndex);
//					ImageBuffer* pImageBufferB = pProduct->getImageBuffer(QString::fromStdString(vec.at(1)), (CImageProduct::int)iMageIndex);
//					if (pImageBufferA == NULL || pImageBufferA->rawData.img_data == NULL
//						|| pImageBufferB == NULL || pImageBufferA->rawData.img_data == NULL)
//					{
//						break;
//					}
//					unsigned char *pImageA = pImageBufferA->rawData.img_data;
//					unsigned char *pImageB = pImageBufferB->rawData.img_data;
//					unsigned char *pTmp = new unsigned char[pImageBufferA->rawData.img_width*iBlockLines / 2];
//					for (size_t i = 0; i < iBlockCnt; i++)
//					{
//						memcpy(pTmp, pImageA + (pImageBufferA->rawData.img_width*iBlockLines)*i, pImageBufferA->rawData.img_width*iBlockLines / 2);
//						memcpy(pImageA + (pImageBufferA->rawData.img_width*iBlockLines)*i, pImageB + (pImageBufferA->rawData.img_width*iBlockLines)*i, pImageBufferA->rawData.img_width*iBlockLines / 2);
//						memcpy(pImageB + (pImageBufferA->rawData.img_width*iBlockLines)*i, pTmp, pImageBufferA->rawData.img_width*iBlockLines / 2);
//					}
//					delete pTmp;
//				}
//			}
//		}
//	}
//	
//
//	// 2K 相机采图出现丢一个触发，导致图片错误。人为修复 end
//}

void CInvokeArithmetic::SetUnInitDeepLearn()
{
	return;//test lu
	if (m_pIDlArithmetic)
	{
		m_pIDlArithmetic->SetUnInitDeepLearn();
	}

}

InitDeepLearnState CInvokeArithmetic::GetDeepLearnEnvState()
{
	InitDeepLearnState str1 = DeepLearnNoNeed;
	InitDeepLearnState str2 = DeepLearnNoNeed;;
	if (m_pIDlArithmetic)
	{
		str1 = m_pIDlArithmetic->GetDeepLearnEnvState();
	}

	if (str1 == DeepLearnNoNeed && str2 == DeepLearnNoNeed)
	{
		return DeepLearnNoNeed;
	}
	else if (str1 == DeepLearnInitOK && str2 == DeepLearnInitOK)
	{
		return DeepLearnInitOK;
	}
	else if (str1 == DeepLearnIintError || str2 == DeepLearnIintError)
	{
		return DeepLearnIintError;
	}


	return DeepLearnIintError;
}


