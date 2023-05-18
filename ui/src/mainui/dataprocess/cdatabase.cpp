#include "cdatabase.h"
QMap<QString, CDataBase *> CDataBase::m_pMapInstance;

CDataBase::CDataBase(QString strDataBaseName, QString strPath)
{
    m_strDataBaseName = strDataBaseName;
	m_strDataBasePath = strPath;
    if (m_strDataBaseName.isEmpty())
    {
        m_strDataBaseName = "finance";
    }
    createConnection();
    //QStringList list;
    //list << "id" << "name" << "type" << "industry";
    //setTableNameList(list);
    //createTable();
}

CDataBase::~CDataBase()
{
    QSqlDatabase db = QSqlDatabase::database(m_strDataBaseName); //�������ݿ�����
    db.close();
}

CDataBase * CDataBase::getInstance(QString strTabName, QString strPath/* = ""*/)
{
    CDataBase *pInstance = NULL;
    if (NULL == m_pMapInstance[strTabName])
    {
        m_pMapInstance[strTabName] = new CDataBase(strTabName, strPath);
    }
    return m_pMapInstance[strTabName];
}

void CDataBase::releaseInstance(QString strTabName)
{
    if (NULL != m_pMapInstance[strTabName])
    {
        delete m_pMapInstance[strTabName];
        m_pMapInstance[strTabName] = NULL;
        m_pMapInstance.remove(strTabName);
    }
}

//void CDataBase::setTableNameList(QStringList list)
//{
//    m_TableNamelist = list;
//}

//����һ�����ݿ�����
bool CDataBase::createConnection()
{
    //�Ժ�Ϳ�����"sqlite1"�����ݿ����������
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_strDataBaseName);
    QString strDataBase = m_strDataBaseName;
    if (!m_strDataBasePath.isEmpty())
    {
        strDataBase = m_strDataBasePath;
        if (m_strDataBasePath.right(1) != QDir::separator())
        {
            strDataBase += QDir::separator();
        }
        strDataBase += m_strDataBaseName;
    }
    db.setDatabaseName(strDataBase);
    if (!db.open())
    {
        qDebug() << QString::fromLocal8Bit("�޷��������ݿ�����");
        return false;
    }
    return true;
}

//�������ݿ��
bool CDataBase::createTable(QString strTableName, QStringList list)
{
    try
    {
        QSqlDatabase db = QSqlDatabase::database(m_strDataBaseName); //�������ݿ�����
        QSqlQuery query(db);
        QString strType;
        strType = list[0];
        for (int i = 1; i < list.size(); i++)
        {
            strType.append(QString(",%1").arg(list[i]));
        }
        QString strCreate = QString("create table if not exists %1(%2)").arg(strTableName).arg(strType);
        bool success = query.exec(strCreate);
        if (success)
        {
            qDebug() << QString::fromLocal8Bit("���ݿ�����ɹ���\n");
            return true;
        }
        else
        {
            QSqlError lastError = query.lastError();
            QString strErr = lastError.driverText();
            qDebug() << lastError.driverText() << QString::fromLocal8Bit("���ݿ����ʧ�ܣ�\n");
            return false;
        }
    }
    catch (...)
    {
    	
    }
    return true;
}

//�����ݿ��в����¼
bool CDataBase::insert(QString strTableName, QStringList strValueList, QByteArrayList byteArrayList)
{
    QSqlDatabase db = QSqlDatabase::database(m_strDataBaseName); //�������ݿ�����
    QSqlQuery query(db);
    //query.exec("select * from automobil");
    //QSqlRecord rec = query.record();
    //int index = rec.count();
    QString strInsertSql = "insert into %1 values(%2)";
    int iCountValue = strValueList.size();
    int iCountByteArray = byteArrayList.size();
    int iCount = iCountValue + iCountByteArray;
    QString strValues = "?";
    for (int i = 1; i < iCount; i++)
    {
        strValues += ", ?";
    }
    strInsertSql = strInsertSql.arg(strTableName).arg(strValues);
    QString strID, strType, strName, strIndustry;
    query.prepare(strInsertSql);
    //query.bindValue(0, index);
    for (int j = 0; j < iCountValue; j++)
    {
        query.addBindValue(strValueList[j]);
    }
    for (int k = 0; k < iCountByteArray; k++)
    {
        query.addBindValue(byteArrayList[k], QSql::Binary);
    }
    bool success = query.exec();
    if (!success)
    {
        QSqlError lastError = query.lastError();
        QString strErr = lastError.driverText();
        qDebug() << lastError.driverText() << QString::fromLocal8Bit("����ʧ��");
        return false;
    }
    return true;
}


bool CDataBase::insert(QString strTableName, QList<QStringList> strValueList, QList<QByteArrayList> byteArrayList)
{
    QSqlDatabase db = QSqlDatabase::database(m_strDataBaseName); //�������ݿ�����
    QSqlQuery query(db);
    //query.exec("select * from automobil");
    //QSqlRecord rec = query.record();
    //int index = rec.count();
    QString strInsertSql = "insert into %1 values %2";
    int iListSize = strValueList.size();
    QString strInsertData;
    db.transaction();
    for (int i = 0; i < iListSize; i++)
    {
        QStringList rowData = strValueList[i];
        QByteArrayList imageData = byteArrayList[i];
        QString strInsertSql = "insert into %1 values(%2)";
        int iCountValue = rowData.size();
        int iCountByteArray = imageData.size();
        int iCount = iCountValue + iCountByteArray;
        QString strValues = "?";
        for (int i = 1; i < iCount; i++)
        {
            strValues += ", ?";
        }
        strInsertSql = strInsertSql.arg(strTableName).arg(strValues);
        QString strID, strType, strName, strIndustry;
        query.prepare(strInsertSql);
        //query.bindValue(0, index);
        for (int j = 0; j < iCountValue; j++)
        {
            query.addBindValue(rowData[j]);
        }
        for (int k = 0; k < iCountByteArray; k++)
        {
            query.addBindValue(imageData[k], QSql::Binary);
        }
        bool success = query.exec();
        if (!success)
        {
            QSqlError lastError = query.lastError();
            QString strErr = lastError.driverText();
            qDebug() << lastError.driverText() << QString::fromLocal8Bit("����ʧ��");
            return false;
        }

        //int iCountValue = rowData.size();
        //int iCountByteArray = imageData.size();
        //QString strRowInfo = rowData[0];
        //for (int j = 1; j < iCountValue; j++)
        //{
        //    strRowInfo.append(", ");
        //    strRowInfo.append(rowData[j]);
        //}
        //for (int k = 1; k < iCountByteArray; k++)
        //{
        //    strRowInfo.append(", ");
        //    strRowInfo.append(imageData[k]);
        //}
        //if (0 == i)
        //{
        //    strInsertData.append(QString("(%1)").arg(strRowInfo));
        //}
        //else
        //{
        //    strInsertData.append(QString(", (%1)").arg(strRowInfo));
        //}
    }
    bool commitsuccess = db.commit();
    //strInsertSql = strInsertSql.arg(strTableName).arg(strInsertData);
    //query.prepare(strInsertSql);
    //bool success = query.exec();
    if (!commitsuccess)
    {
        QSqlError lastError = query.lastError();
        QString strErr = lastError.driverText();
        qDebug() << lastError.driverText() << QString::fromLocal8Bit("����ʧ��");
        return false;
    }
    return true;
}

//��ѯ������Ϣ
bool CDataBase::queryAll(QString strTableName, QList<QByteArrayList> &itemList, QStringList name, QStringList value, bool bIsJoin)
{
    QSqlDatabase db = QSqlDatabase::database(m_strDataBaseName); //�������ݿ�����
    QSqlQuery query(db);
    QString strSelect = QString("select * from %1").arg(strTableName);
    if (name.size() > 0)
    {
        QString strType = " AND ";
        if (!bIsJoin)
        {
            strType = " OR ";
        }
        QString strCondition = QString(" where %1 = '%2'").arg(name[0]).arg(value[0]);
        for (int i = 1; i < name.size(); i++)
        {
            strCondition.append(strType);
            strCondition.append(QString("%1 = '%2'").arg(name[i]).arg(value[i]));
        }
        strSelect.append(strCondition);
    }
    bool bRet = query.exec(strSelect);
    if (!bRet)
    {
        QSqlError lastError = query.lastError();
        QString strErr = lastError.driverText();
        qDebug() << lastError.driverText() << QString::fromLocal8Bit("����ʧ��");
    }
    QSqlRecord rec = query.record();
    qDebug() << QString::fromLocal8Bit("%1���ֶ�����").arg(strTableName) << rec.count();

    while (query.next())
    {
        QByteArrayList strList;
        for (int index = 0; index < rec.count(); index++)
            strList.push_back(query.value(index).toByteArray());
        itemList.push_back(strList);
    }
    return true;
}

//����IDɾ����¼
bool CDataBase::deleteById(QString strTableName, int id)
{
    QSqlDatabase db = QSqlDatabase::database(m_strDataBaseName); //�������ݿ�����
    QSqlQuery query(db);
    query.prepare(QString("delete from %1 where id = %2").arg(strTableName).arg(id));
    if (!query.exec())
    {
        qDebug() << QString::fromLocal8Bit("ɾ����¼ʧ�ܣ�");
        return false;
    }
    return true;
}

////����ID���¼�¼
//bool CDataBase::updateById(int id)
//{
//    QSqlDatabase db = QSqlDatabase::database("sqlite_finance"); //�������ݿ�����
//    QSqlQuery query(db);
//    query.prepare(QString("update finance set attribute=?,type=?,"
//        "kind=?, nation=?,"
//        "carnumber=?, elevaltor=?,"
//        "distance=?, oil=?,"
//        "temperature=? where id=%1").arg(id));
//
//    query.bindValue(0, "����");
//    query.bindValue(1, "�γ�");
//    query.bindValue(2, "����");
//    query.bindValue(3, rand() % 100);
//    query.bindValue(4, rand() % 10000);
//    query.bindValue(5, rand() % 300);
//    query.bindValue(6, rand() % 200000);
//    query.bindValue(7, rand() % 52);
//    query.bindValue(8, rand() % 100);
//
//    bool success = query.exec();
//    if (!success)
//    {
//        QSqlError lastError = query.lastError();
//        qDebug() << lastError.driverText() << QString::fromLocal8Bit("����ʧ��");
//    }
//    return true;
//}
//
////����
//bool CDataBase::sortById()
//{
//    QSqlDatabase db = QSqlDatabase::database("sqlite_finance"); //�������ݿ�����
//    QSqlQuery query(db);
//    bool success = query.exec("select * from finance order by id desc");
//    if (success)
//    {
//        qDebug() << QString::fromLocal8Bit("����ɹ�");
//        return true;
//    }
//    else
//    {
//        qDebug() << QString::fromLocal8Bit("����ʧ�ܣ�");
//        return false;
//    }
//}