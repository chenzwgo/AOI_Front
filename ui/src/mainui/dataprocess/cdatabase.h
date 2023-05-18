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
    bool createConnection();  //创建一个连接
    bool createTable(QString strTableName, QStringList list);       //创建数据库表
    bool insert(QString strTableName,QStringList strValueList, QByteArrayList byteArrayList = QByteArrayList());            //出入数据
    //批量插入
    bool insert(QString strTableName, QList<QStringList> strValueList, QList<QByteArrayList> byteArrayList);            //出入数据
    bool queryAll(QString strTableName,QList<QByteArrayList> &itemList, QStringList name = QStringList(), QStringList value = QStringList(), bool bIsJoin = true);          //查询所有信息
    bool updateById(int id);  //更新
    bool deleteById(QString strTableName,int id);  //删除
    bool sortById();          //排序

private:
    static QMap<QString, CDataBase *>m_pMapInstance;
    QStringList m_TableNamelist;
    QString m_strDataBaseName;
    QString m_strDataBasePath;
};

#endif // CDATABASE_H