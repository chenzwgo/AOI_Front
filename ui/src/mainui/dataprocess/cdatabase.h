#ifndef CDATABASE_H
#define CDATABASE_H

#include <QtSql>
#include <QMap>

class CDataBase
{
public:
    CDataBase(QString strDataBaseName, QString strPath);
    ~CDataBase();
    static CDataBase *getInstance(QString strDataBaseName = "", QString strPath = "");
    static void releaseInstance(QString strDataBaseName = "");
public:
    //void setTableNameList();
    bool createConnection();  //����һ������
    bool createTable(QString strTableName, QStringList list);       //�������ݿ��
    bool insert(QString strTableName,QStringList strValueList, QByteArrayList byteArrayList = QByteArrayList());            //��������
    //��������
    bool insert(QString strTableName, QList<QStringList> strValueList, QList<QByteArrayList> byteArrayList);            //��������
    bool queryAll(QString strTableName,QList<QByteArrayList> &itemList, QStringList name = QStringList(), QStringList value = QStringList(), bool bIsJoin = true);          //��ѯ������Ϣ
    bool updateById(int id);  //����
    bool deleteById(QString strTableName,int id);  //ɾ��
    bool sortById();          //����

private:
    static QMap<QString, CDataBase *>m_pMapInstance;
    QStringList m_TableNamelist;
    QString m_strDataBaseName;
    QString m_strDataBasePath;
};

#endif // CDATABASE_H