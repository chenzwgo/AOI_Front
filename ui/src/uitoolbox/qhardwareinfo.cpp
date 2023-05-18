#include "uitoolbox/qhardwareinfo.h"
#include <QNetworkInterface>
#include <QHostAddress>
#include <QDebug>

QString QHardwareInfo::GetMacAddress(const QString &interfacename)
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface inter,list)
    {
        qDebug() <<"Device:"<<inter.name();
        qDebug() << "HardwareAddress:"<<inter.hardwareAddress();
        if(interfacename == inter.name())
        {
            return inter.hardwareAddress();
        }
    }
    return "";
}

QStringList QHardwareInfo::GetIPAddress(const QString &interfacename)
{
    QStringList addlist;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface inter,list)
    {
        qDebug() <<"Device:"<<inter.name();
        qDebug() << "HardwareAddress:"<<inter.hardwareAddress();
        if(interfacename == inter.name())
        {
            QList<QHostAddress> Addresslist = inter.allAddresses();
            foreach(QHostAddress address,Addresslist)
            {
                addlist.append(address.toString());
            }
        }
    }
    return addlist;
}

QStringList QHardwareInfo::GetAllInterface()
{
    QStringList InterfaceList;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface inter,list)
    {
        qDebug() <<"Device:"<<inter.name();
        qDebug() << "HardwareAddress:"<<inter.hardwareAddress();
        InterfaceList <<inter.name();
    }
    return InterfaceList;
}
