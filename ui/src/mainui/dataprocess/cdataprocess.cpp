#include "cdataprocess.h"
#include "dataprocess/cdatabase.h"
#include <QDateTime>

CDataProcess::CDataProcess()
{
    m_dataLock.Init();
    m_bIsRunning = true;
    m_pSaveDataThread = new JThreadTP<CDataProcess, void*, void>;
}

CDataProcess::~CDataProcess()
{
    m_bIsRunning = false;
    while (m_pSaveDataThread->IsRunning())
    {
        Sleep(100);
    }
	if (m_pSaveDataThread != NULL)
	{
		delete m_pSaveDataThread;
		m_pSaveDataThread = NULL;
	}
}

void CDataProcess::SetDataBasePath(QString strPath)
{
    if (m_strDataBasePath != strPath)
    {
        m_strDataBasePath = strPath;
    }
}

void CDataProcess::ThreadSaveDataProcess(void* param)
{
    QTime startTime, endTime;
    QString strDataFormat = "yy_MM_dd";
    QDateTime time = QDateTime::currentDateTime();
    QString strTime = time.toString(strDataFormat);
    QString strProductTableName = "classify_result_product";
    QString strDataTableName = "classify_result_data";
    CDataBase  *dataBase = CDataBase::getInstance("classify_result" + strTime, m_strDataBasePath);
    QStringList proTitleList;
    QStringList dataTitleList;
    proTitleList << "sn varchar(30) primary key" << "result varchar(30)" << "insert_time varchar(30)" << "count varchar(30)";
    dataTitleList << "snid varchar(30) primary key" << "product varchar(16)" << "name varchar(30)" << "level varchar(30)" << "size varchar(30)"
        << "type varchar(30)" << "contrast varchar(30)" << "fdetect_score varchar(30)" << "score varchar(30)"<< "gray varchar(30)" << "defectwidth varchar(30)" << "defectheight varchar(30)"
		<< "width varchar(30)" << "height varchar(30)" << "x varchar(30)" << "y varchar(30)" << "ori varchar(30)" << "position varchar(30)"
        << "imageA blob" << "imageB blob" << "imageC blob" << "imageD blob" << "imageE blob" << "imageF blob" << "imageG blob" << "imageH blob";
    dataBase->createTable(strProductTableName, proTitleList);
    dataBase->createTable(strDataTableName, dataTitleList);
    bool bFound = false;
    while(m_bIsRunning)
    {
        stProductDefectInfo info;
        QList<QStringList> productInfo;
        m_dataLock.Lock();
        if (m_DefectListInfo.size() > 0)
        {
            info = m_DefectListInfo.front();
            m_DefectListInfo.pop_front();
            bFound = true;
        }
        m_dataLock.Unlock();
        if (!bFound)
        {
            Sleep(100);
            continue;
        }
        startTime = QTime::currentTime();
        productInfo = info.listDefectInfo;
        QList<QByteArrayList> byteArrayList = info.listArrayInfo;
        if (productInfo.size() > 0)
        {
            QDateTime curtime = QDateTime::currentDateTime();
            QString strCurTime = curtime.toString(strDataFormat);
            if (strTime != strCurTime)
            {
                strTime = strCurTime;
                dataBase = CDataBase::getInstance("classify_result" + strTime, m_strDataBasePath);
                dataBase->createTable(strProductTableName, proTitleList);
                dataBase->createTable(strDataTableName, dataTitleList);
            }
            startTime = QTime::currentTime();
            QList<QStringList> strListProductInfo;
            QList<QByteArrayList> strListImageInfo;
            for (int i = 0; i < productInfo.size() && i < byteArrayList.size(); ++i)
            {
                QStringList list = productInfo[i];
                QByteArrayList arraylist = byteArrayList[i];
                //dataBase->insert(strDataTableName, list, arraylist);
                strListProductInfo.push_back(list);
                strListImageInfo.push_back(arraylist);
            }
            dataBase->insert(strDataTableName, strListProductInfo, strListImageInfo);
            endTime = QTime::currentTime();
            int mTime = startTime.msecsTo(endTime);
            QStringList listProduct;
            listProduct << info.strSN << QString::number(info.iResult) << QString::number(mTime) << QString::number(productInfo.size());
            dataBase->insert(strProductTableName, listProduct);
            bFound = false;
        }
        else
        {
            Sleep(50);
        }
    }
}

void CDataProcess::InsertDataInfo(QList<QStringList> &productInfo, QList<QByteArrayList> arrayList, QString strSN, int iResult)
{
    CISAPUGuard<JMutex> autoLock(m_dataLock);
    stProductDefectInfo info;
    info.strSN = strSN;
    info.iResult = iResult;
    info.listDefectInfo = productInfo;
    info.listArrayInfo = arrayList;
    m_DefectListInfo.push_back(info);
    if (!m_pSaveDataThread->IsRunning())
    {
        m_pSaveDataThread->StartThread(this, &CDataProcess::ThreadSaveDataProcess, this);
    }
}
