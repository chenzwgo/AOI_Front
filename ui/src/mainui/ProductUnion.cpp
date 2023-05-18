#include "ProductUnion.h"
#include "g12globalenv.h"
#include "maindefine.h"
#include "workflow/workflowfun.h"
#include "showdefect/diagramscene.h"
#include "sendmessage.h"
#include "filevariable/ifilevariable.h"
#include "commonapi.h"
#include "stringcommon.h"

CProductUnion *CProductUnion::m_pInstance = NULL;
unsigned long long CProductUnion::Sn = 0;
#define  IMAGE_LENGTH       14500
#define  BLOCK_LENGTH       512



 CProductUnion::CProductUnion(void)
{
    m_mutexProductList.Init();
}


CProductUnion::~CProductUnion(void)
{
    CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
    foreach(CImageProduct* pstProduct,m_listProduct)
    {
        if (pstProduct == NULL)
        {
            continue;
        }
        delete pstProduct;
    }
    m_listProduct.clear();
}

CProductUnion * CProductUnion::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CProductUnion();
    }
    return m_pInstance;
}

void CProductUnion::releaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}


void CProductUnion::initProductBuffer(unsigned int isize)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
	if (isize >10 || isize < m_listProduct.size())
	{
		return;
	}
	for (int i = 0; i < isize - m_listProduct.size();i++)
	{
		CImageProduct*pProduct = new CImageProduct(0);
		pProduct->setProductImageSize(8192, 12800);
		m_listProduct.push_back(pProduct);
	}
}

CImageProduct* CProductUnion::createProduct()	
{
    CISAPUGuard<JMutex>  autoLock(m_mutexProductList);

	CImageProduct* pProduct = NULL;
	foreach(CImageProduct* pstProduct, m_listProduct)
	{
		if (pstProduct != NULL && pstProduct->isFree())
		{
			pProduct = pstProduct;
			pProduct->resetStatus();
			break;
		}
	}
	if (pProduct == NULL)
	{
		pProduct = new CImageProduct(0);
		m_listProduct.push_back(pProduct);
	}
	
    Sn = readCurrentDayPrSn();
	pProduct->setProductSn(++Sn);
    writeCurrentDayPrSn(Sn);
	pProduct->setFree(false);

	int iProductfreeSize = 0;
	foreach(CImageProduct* pstProduct, m_listProduct)
	{
		if (pstProduct != NULL && !pstProduct->isFree())
		{
			iProductfreeSize++;
		}
	}

    CSendMessage::GetInstance()->Send(QString("createProduct(%1)").arg(Sn));
    CSendMessage::GetInstance()->sendUpdateUiStus(QString::number(iProductfreeSize),update_number);
    return pProduct;
}

void CProductUnion::releaseProduct( CImageProduct* pProduct )
{
    CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
    if (!m_listProduct.contains(pProduct))
    {
        return;
    }
	unsigned int uisn = pProduct->getProductSn();
	pProduct->resetFlagStatus();
	pProduct->setFree(true);

	int iProductfreeSize = 0;
	int iProductNofreeSize = 0;
	for (int i = 0; i < m_listProduct.size();i++)
	{	
		CImageProduct* pstProduct = m_listProduct[i];
		if (pstProduct != NULL)
		{
			if (!pstProduct->isFree())
			{
				iProductNofreeSize++;
			}
			else if(m_listProduct.size() >5)
			{
				delete pstProduct;
				m_listProduct[i] = NULL;
			}

		}
	}

	for (int i = 0; i < m_listProduct.size(); i++)
	{
		CImageProduct* pstProduct = m_listProduct[i];
		if (pstProduct == NULL )
		{
			m_listProduct.removeAt(i);
			i = 0;
		}
	}


    CSendMessage::GetInstance()->Send(QString("releaseProduct(%1)").arg(uisn));
    CSendMessage::GetInstance()->sendUpdateUiStus(QString::number(iProductNofreeSize),update_number);
    return ; 
}

int CProductUnion::getProductCnt()
{
    CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
    return m_listProduct.size();
}

bool CProductUnion::isProductExist(CImageProduct* pProduct)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
	foreach(CImageProduct* pstProduct, m_listProduct)
	{
		if (pstProduct == NULL)
		{
			continue;
		}
		if (pstProduct == pProduct)
		{
			return true;
		}
	}
	return false;
}

CImageProduct* CProductUnion::getProduct( int index )
{
    CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
    if (index < m_listProduct.size())
    {
        return m_listProduct.at(index);
    }
    return NULL; 
}

CImageProduct* CProductUnion::getProductBySn( unsigned long long proSn )
{
    CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
    foreach(CImageProduct* pstProduct,m_listProduct)
    {
        if (pstProduct == NULL)
        {
            continue;
        }
        if (pstProduct->getProductSn() == proSn)
        {
            return pstProduct;
        }
    }

    return NULL; 
}

CImageProduct* CProductUnion::getChannelOneOnStatus(QString strChannel, enProStatus status)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
	int iCnt = m_listProduct.size();
	CImageProduct* pProduct = NULL;
	for (int i = 0; i < iCnt; i++)
	{
		CImageProduct* pProducttmp = m_listProduct.at(i);
		if (pProducttmp->isChannelOneOnStatus(strChannel,status, IMAGE_STATION_MAX_CNT))
		{
			pProduct = pProducttmp;
			break;;
		}
	}
	return pProduct;
}

CImageProduct* CProductUnion::getChannelAllOnStatus(QString strChannel, enProStatus status)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
	int iCnt = m_listProduct.size();
	CImageProduct* pProduct = NULL;
	for (int i = 0; i < iCnt; i++)
	{
		CImageProduct* pProducttmp = m_listProduct.at(i);
        if (pProducttmp->isChannelAllOnStatus(strChannel,status))
		{
			pProduct = pProducttmp;
			break;;
		}
	}
	return pProduct;
}

CImageProduct* CProductUnion::getProductOneOnStatus( enProStatus status ,int index)
{
    CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
    int iCnt = m_listProduct.size();
    CImageProduct* pProduct  = NULL;
    for (int i=0;i < iCnt;i++)
    {
        CImageProduct* pProducttmp=  m_listProduct.at(i);
        if (pProducttmp->isProductOneOnStatus(status,index))
        {
            pProduct = pProducttmp;
            break;;
        }
    }
    return pProduct;
}

CImageProduct* CProductUnion::getProductAllOnStatus( enProStatus status ,int index)
{
    CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
    int iCnt = m_listProduct.size();
    CImageProduct* pProduct  = NULL;
    for (int i=0;i < iCnt;i++)
    {
        CImageProduct* pProducttmp=  m_listProduct.at(i);
        if (pProducttmp->isProductAllOnStatus(status, index))
        {
            pProduct = pProducttmp;
            break;
        }
    }
    return pProduct;
}

CImageProduct* CProductUnion::getProductGrapFinish(int index)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
	int iCnt = m_listProduct.size();
	CImageProduct* pProduct = NULL;
	for (int i = 0; i < iCnt; i++)
	{
		CImageProduct* pProducttmp = m_listProduct.at(i);
		if (pProducttmp->isProductAllOnStatus(en_grapfinish,index))
		{
			pProduct = pProducttmp;
			break;;
		}
	}
	return pProduct;
}

unsigned int  CProductUnion::readCurrentDayPrSn()
{
    IFileVariable *pFileVar = getGlobalSNFileVariable();
    //QString dateTime = QDateTime::currentDateTime().toString("yyyyMMdd");
	//m_strMarkDay = QDateTime::currentDateTime().toString("yyyyMMdd");
	QString strDate = getCurrentProducsTimeStrPath();
	m_strMarkDay = strDate.remove(QChar('-'), Qt::CaseInsensitive);
    return pFileVar->getValue("ProductSN", m_strMarkDay.toStdString(),0).toUInt();
}

void CProductUnion::writeCurrentDayPrSn(unsigned int ullSn)
{
    IFileVariable *pFileVar = getGlobalSNFileVariable();
    //QString dateTime = QDateTime::currentDateTime().toString("yyyyMMdd");
	QString strDate = getCurrentProducsTimeStrPath();
	QString dateTime  = strDate.remove(QChar('-'), Qt::CaseInsensitive);
    pFileVar->setValue("ProductSN",dateTime.toStdString(),ullSn,true);
}

void CProductUnion::releaseAllProduct()
{
    CISAPUGuard<JMutex>  autoLock(m_mutexProductList);
    for (int index = 0;index < m_listProduct.size();index++)
    {
        if (m_listProduct.at(index) != NULL )
        {
            CImageProduct *product = m_listProduct.at(index);
			product->resetStatus();
            delete product;
			m_listProduct[index] = NULL;
        }     
    }
	m_listProduct.clear();
}

CImageProduct::CImageProduct(unsigned long long  ullSn):
	m_proSn(ullSn),
	m_zipPath("")
{
	memset(m_iWidth, 0, sizeof(m_iWidth));
	memset(m_iHeight, 0, sizeof(m_iHeight));
	memset(m_bSetProductImageSize, false, sizeof(m_bSetProductImageSize));

    m_bSetProcessStartTimeflag = false;
    m_bSetProcessEndTimeflag = false;
	m_bSetGrapImageStartTimeflag = false;
	m_bSetGrapImageEndTimeflag = false;
	m_bUpdateUI = false;
	m_bIsFree = true;
	m_MarkFrontRes.init();
	m_ProcessErrorCode = er_no_error;
	pDefectStrutVec = new vector<stDefectFeature *>();  
}

CImageProduct::~CImageProduct()
{
    CISAPUGuard<JMutex>  autoLock(m_mutexStatus);


	for (int index = 0; index < getImageStationCnt();index++)
	{
		QMap<QString, ImageBuffer*>::iterator it = m_mapImageBuffer[index].begin();
		while (it != m_mapImageBuffer[index].end())
		{
			ImageBuffer* pImageBuffer = it.value();
			if (pImageBuffer == NULL)
			{
				it++;
				continue;;
			}
			delete pImageBuffer;
			it++;
		}
		m_mapImageBuffer[index].clear();
	}

	if (pDefectStrutVec != NULL)
	{
		for (int i = 0; i < pDefectStrutVec->size();i++)
		{
			if (pDefectStrutVec->at(i) != NULL)
			{
				delete pDefectStrutVec->at(i);
			}
		}
		pDefectStrutVec->clear();
		delete pDefectStrutVec;
		pDefectStrutVec = NULL;
	}

}

bool CImageProduct::isProductOneOnStatus( enProStatus status,int index)
{
	if (index == IMAGE_STATION_MAX_CNT)
	{
		bool bflag = true;
		for (int i = 0; i < getImageStationCnt(); i++)
		{
			if (isImageStationEnable(i))
			{
				bflag = bflag && isProductOneOnStatus(status, (int)i);
			}
		}
		return bflag;
        //return isProductOneOnStatus(status, Image_first) && isProductOneOnStatus(status, Image_second)&& isProductOneOnStatus(status, Image_third);
	}

    CISAPUGuard<JMutex>  autoLock(m_mutexStatus);

    string sense = SString::IntToStr((int)index);

    vector<string> vect;
    getEnabelChannle(vect,sense);
	for (int i = 0; i < vect.size(); i++)
	{
		QString Channel = QString::fromStdString(vect.at(i));
		if (status != m_mapProStatus[index][Channel])
		{
			return false;
		}
	}
    return true;
}

bool CImageProduct::isProductAllOnStatus( enProStatus status, int index)
{
	if (index == IMAGE_STATION_MAX_CNT)
	{
		bool bflag = true;
		for (int i = 0; i < getImageStationCnt(); i++)
		{
			if (isImageStationEnable(i))
			{
				bflag = bflag && isProductAllOnStatus(status, (int)i);
			}
		}
		return bflag;
       // return isProductAllOnStatus(status, Image_first) && isProductAllOnStatus(status, Image_second) && isProductAllOnStatus(status, Image_third);
	}
    CISAPUGuard<JMutex>  autoLock(m_mutexStatus);
    string sense = SString::IntToStr((int)index);

	vector<string> vect;
    getEnabelChannle(vect,sense);
	for (int i = 0; i < vect.size(); i++)
	{
		QString Channel = QString::fromStdString(vect.at(i));
		if (status != m_mapProStatus[index][Channel])
		{
			return false;
		}
	}
    return true;
}

bool CImageProduct::isChannelOneOnStatus(QString strChannel, enProStatus status, int index)
{
	if (index == IMAGE_STATION_MAX_CNT)
	{
		bool bflag = true;
		for (int i = 0; i < getImageStationCnt(); i++)
		{
			if (isImageStationEnable(i))
			{
				bflag = bflag && isChannelOneOnStatus(strChannel, status, (int)i);
			}
		}
		return bflag;
        //return isChannelOneOnStatus(strChannel, status, Image_first) && isChannelOneOnStatus(strChannel, status, Image_second)&& isChannelOneOnStatus(strChannel, status, Image_third);
	}
	CISAPUGuard<JMutex>  autoLock(m_mutexStatus);
    string sense = SString::IntToStr((int)index);

    vector<string> vect;
    getEnabelChannle(vect,sense);
	for (int i = 0; i < vect.size(); i++)
	{
		QString Channel = QString::fromStdString(vect.at(i));
		if (Channel == strChannel)
		{
			return status == m_mapProStatus[index][Channel];
		}
	}

	return true;
}

bool CImageProduct::isChannelAllOnStatus(QString strChannel, enProStatus status, int index)
{
	if (index == IMAGE_STATION_MAX_CNT)
	{
		bool bflag = true;
		for (int i = 0; i < getImageStationCnt();i++)
		{
			if (isImageStationEnable(i))
			{
				bflag = bflag && isChannelAllOnStatus(strChannel, status, (int) i);
			}
		}
		return bflag;
        //return isChannelAllOnStatus(strChannel, status, Image_first) && isChannelAllOnStatus(strChannel, status, Image_second)&& isChannelAllOnStatus(strChannel, status, Image_third);
	}
	CISAPUGuard<JMutex>  autoLock(m_mutexStatus);
    string sense = SString::IntToStr((int)index);

    vector<string> vect;
    getEnabelChannle(vect,sense);
	for (int i = 0; i < vect.size(); i++)
	{
		QString Channel = QString::fromStdString(vect.at(i));
		if (Channel == strChannel)
		{
			return  status == m_mapProStatus[index][Channel];
		}
	}
	return true;
}

void CImageProduct::setChannelAllStatus(QString strChannel, enProStatus status, int index)
{
	if (index == IMAGE_STATION_MAX_CNT)
	{
		for (int i = 0; i < getImageStationCnt(); i++)
		{
			setChannelAllStatus(strChannel, status, i);
		}
		return;
	}

	CISAPUGuard<JMutex>  autoLock(m_mutexStatus);
	m_mapProStatus[index][strChannel] = status;
	return;
}

void CImageProduct::setProductAllStatus( enProStatus status, int index)
{

	if (index == IMAGE_STATION_MAX_CNT)
	{
		for (int i = 0; i < getImageStationCnt(); i++)
		{
			setProductAllStatus( status, i);
		}
		return;
	}
    CISAPUGuard<JMutex>  autoLock(m_mutexStatus);
    string sense = SString::IntToStr((int)index);

    vector<string> vect;
    getEnabelChannle(vect,sense);
	for (int i = 0; i < vect.size(); i++)
	{
		QString Channel = QString::fromStdString(vect.at(i));
		m_mapProStatus[index][Channel] = status;
	}
    return ;
}

unsigned long long CImageProduct::getProductSn()
{
    return m_proSn;
}

void CImageProduct::setProductSn(unsigned long long ullSn)
{
	m_proSn = ullSn;
}

ImageBuffer* CImageProduct::getImageBuffer(QString strChannelName, int index)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexStatus);

	if (!m_mapImageBuffer[index].contains(strChannelName))
	{
		return NULL;
	}
	return m_mapImageBuffer[index][strChannelName];
}

bool CImageProduct::isFree()
{
    CISAPUGuard<JMutex>  autoLock(m_mutexStatus);
	return m_bIsFree;

}

void CImageProduct::setFree(bool bFree)
{
    CISAPUGuard<JMutex>  autoLock(m_mutexStatus);
	m_bIsFree = bFree;
}

void CImageProduct::resetStatus()
{
	resetFlagStatus();
	resetMemStatus();
}


void CImageProduct::resetFlagStatus()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexStatus);

	m_bSetProcessStartTimeflag = false;
	m_bSetProcessEndTimeflag = false;
	m_bSetGrapImageStartTimeflag = false;
	m_bSetGrapImageEndTimeflag = false;
	m_bUpdateUI = false;
	m_bIsFree = false;
	memset(m_bSetProductImageSize, false, sizeof(m_bSetProductImageSize));
	m_MarkFrontRes.init();
	m_ProcessErrorCode = er_no_error;

	if (pDefectStrutVec != NULL)
	{
		for (int i = 0; i < pDefectStrutVec->size(); i++)
		{
			if (pDefectStrutVec->at(i) != NULL)
			{
				delete pDefectStrutVec->at(i);
			}
		}
		pDefectStrutVec->clear();
	}
    for (int j = 0; j < getImageStationCnt();j++)
    {
        string sense = SString::IntToStr((int)j);

        vector<string> vect;
        getEnabelChannle(vect,sense);
        for (int i = 0; i < vect.size(); i++)
        {
            QString Channel = QString::fromStdString(vect.at(i));
            if (m_mapImageBuffer[j].contains(Channel))
            {
                m_mapImageBuffer[j][Channel]->rawData.img_blockIndex = 0;
            }
        }
    }
	m_stProductSummary.init();

	setProductAllStatus(en_idle);
}

void CImageProduct::resetMemStatus()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexStatus);

	for (int j = 0; j < getImageStationCnt();j++)
	{
        string sense = SString::IntToStr((int)j);

        vector<string> vect;
        getEnabelChannle(vect,sense);
		for (int i = 0; i < vect.size(); i++)
		{
			QString Channel = QString::fromStdString(vect.at(i));
			if (m_mapImageBuffer[j].contains(Channel))
			{
				m_mapImageBuffer[j][Channel]->clearMem();
			}
			m_mapProStatus[j][Channel] = en_idle;
		}	
	}
}

eProcessErroeCode CImageProduct::getProcessError()
{
	return m_ProcessErrorCode;
}

void CImageProduct::setProcessErrCode(eProcessErroeCode bflag)
{
	m_ProcessErrorCode = bflag;
}

void CImageProduct::SetZipPath(const QString & path)
{
	m_zipPath = path;
}

QString CImageProduct::GetZipPath()
{
	return m_zipPath;
}

void CImageProduct::setProductImageSize( unsigned int iWidth ,unsigned int iHeight, int index)
{
	CISAPUGuard<JMutex>  autoLock(m_mutexStatus);

	if (m_bSetProductImageSize[index])
	{
		return;
	}
    string sense = SString::IntToStr((int)index);

	m_bSetProductImageSize[index] = true;
	vector<string> vect;
    getEnabelChannle(vect, sense);
    int iOverCount = 0;
    int iBlockRow = 256;
    IFileVariable *pFileImage = getCurrentProgramFileVariable(CAM_ATTRIBUTE_FILE_NAME);
    if (NULL != pFileImage)
    {
        iOverCount = pFileImage->getValue(IMAGE_PARAM_DUL_LINES, 0).toInt();
    }
    IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), sense);
    if (pFileVariable != NULL)
    {
        iBlockRow = pFileVariable->getValue(ACQUISITION_IMAGECONFIG,ACQUISITION_BLOCKLINES).toInt(256);
        if (0 == iBlockRow)
        {
            iBlockRow = 256;
        }
    }

	for (int i = 0; i < vect.size(); i++)
	{
		QString Channel = QString::fromStdString(vect.at(i));


		if (!m_mapImageBuffer[index].contains(Channel))
		{
			m_mapImageBuffer[index][Channel] = new ImageBuffer(iWidth, iHeight);
		}

		if (m_mapImageBuffer[index][Channel]->rawData.img_height != iHeight || m_mapImageBuffer[index][Channel]->rawData.img_width != iWidth)
		{
			m_mapImageBuffer[index][Channel]->rawData.resetMem();
			m_mapImageBuffer[index][Channel]->rawData.img_data = new unsigned char[iWidth*iHeight];
			m_mapImageBuffer[index][Channel]->rawData.img_width = iWidth;
			m_mapImageBuffer[index][Channel]->rawData.img_height = iHeight;
            m_mapImageBuffer[index][Channel]->rawData.img_blockCnt = iHeight/iBlockRow;
            m_mapImageBuffer[index][Channel]->rawData.img_blockRow = iBlockRow;
            m_mapImageBuffer[index][Channel]->rawData.img_blockIndex = 0;
		}
		m_mapProStatus[index][Channel] = en_idle;
	}
	m_iWidth[index] = iWidth;
    m_iHeight[index] = iHeight;
}


unsigned int CImageProduct::getProductWidth(int index)
{
    return m_iWidth[index];
}

unsigned int CImageProduct::getProductHeight(int index)
{
    return m_iHeight[index];
}


void CImageProduct::setStartGrapImageTime()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexTime);
	
	if (!m_bSetGrapImageStartTimeflag)
	{
		m_startTimeGrapImage = QTime::currentTime();
		m_bSetGrapImageStartTimeflag = true;
	}
}

void CImageProduct::setEndGrapImageTime()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexTime);
	if (!m_bSetGrapImageEndTimeflag)
	{
		m_endTimeGrapImage = QTime::currentTime();
		m_bSetGrapImageEndTimeflag = true;
	}
}

unsigned int CImageProduct::grapImageElapsedTime()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexTime);
	return m_startTimeGrapImage.msecsTo(m_endTimeGrapImage);
}


void CImageProduct::setStartProcessTime( )
{
	CISAPUGuard<JMutex>  autoLock(m_mutexTime);
    if (!m_bSetProcessStartTimeflag)
    {
        m_startTimeProcess = QTime::currentTime();
        m_bSetProcessStartTimeflag =true;
    }   
}

void CImageProduct::setEndProcessTime( )
{
	CISAPUGuard<JMutex>  autoLock(m_mutexTime);
    if (!m_bSetProcessEndTimeflag)
    {
        m_endTimeProcess = QTime::currentTime();
        m_bSetProcessEndTimeflag =true;
    }
    
}

unsigned int CImageProduct::processElapsedTime()
{
	CISAPUGuard<JMutex>  autoLock(m_mutexTime);
    return m_startTimeProcess.msecsTo(m_endTimeProcess);
}

void CImageProduct::setUpdateUIFinish(bool bFlag)
{
	m_bUpdateUI = bFlag;
}

bool CImageProduct::getUpdateUIFlag()
{
	return m_bUpdateUI;
}
