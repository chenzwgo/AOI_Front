#ifndef CDATAPROCESS_H
#define CDATAPROCESS_H
#include <jthreadt.h>
#include <QList>

struct stProductDefectInfo
{
    stProductDefectInfo()
    {
        strSN = "0";
        iResult = 0;
        listDefectInfo.clear();
        listArrayInfo.clear();
    }
    stProductDefectInfo(const stProductDefectInfo &other)
    {
        strSN = other.strSN;
        iResult = other.iResult;
        listDefectInfo = other.listDefectInfo;
        listArrayInfo = other.listArrayInfo;
    }
    stProductDefectInfo operator=(const stProductDefectInfo &other)
    {
        strSN = other.strSN;
        iResult = other.iResult;
        listDefectInfo = other.listDefectInfo;
        listArrayInfo = other.listArrayInfo;
        return *this;
    }
    QString strSN;
    int     iResult;
    QList<QStringList> listDefectInfo;
    QList<QByteArrayList> listArrayInfo;
};
class CDataBase;
class CDataProcess
{
public:
    CDataProcess();
    ~CDataProcess();
    void SetDataBasePath(QString strPath);
    void InsertDataInfo(QList<QStringList> &productInfo, QList<QByteArrayList>, QString strSN, int iResult);
private:
    void ThreadSaveDataProcess(void* param);       // 保存各种处理后的效果图
private:
    QList<stProductDefectInfo> m_DefectListInfo;
    QList<QByteArrayList>      m_DefectArrayListInfo;
    JThreadTP<CDataProcess, void*, void>* m_pSaveDataThread;
    JMutex  m_dataLock;
    bool    m_bIsRunning;
    QString m_strDataBasePath;
};

#endif // CDATAPROCESS_H