#ifndef Q_HARDWARE_INFO_H
#define Q_HARDWARE_INFO_H
#include "dllexport.h"
#include "uidefine.h"
#include <QString>
#include <QStringList>

class RT_API QHardwareInfo
{
public:
    static QString GetMacAddress(const QString& interfacename);
    static QStringList GetIPAddress(const QString& interfacename);
    static QStringList GetAllInterface();

};

#endif
