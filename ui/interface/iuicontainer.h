#ifndef IUICONTAINER_H
#define IUICONTAINER_H

#include <QWidget>
class QPluginLoader;
class PluginInfo
{
public:
    PluginInfo()
    {
        strCLSID = "";
        strName = "";
        strVersion = "";
        strURL = "";
        strType ="";
		strlicense = "";
        bLoaded = false;
        bInit = false;
		bstatic = false;
        pPlugin = NULL;
        pPluginLoader= NULL;
    };

    QString strCLSID;
    QString strName;
    QString strVersion;
    QString strURL;
    QString strType;
	QString strlicense;
    bool bLoaded;
    bool bInit;
	bool bstatic;
    QObject *pPlugin;
    QPluginLoader *pPluginLoader;
};

class iuicontainer
{
public:
    virtual ~iuicontainer(){}
    virtual QObject* getPlugin(QString strCLSID) = 0;
    virtual QString  getPluginName(QString strCLSID) = 0;
    virtual QString  getPluginVersion(QString strCLSID) = 0;
    virtual QString  getPluginType(QString strCLSID) = 0;
	virtual QString  getPluginLicense(QString strCLSID) = 0;
    virtual PluginInfo* getPluginInfo(QString strCLSID) = 0;
    virtual QList<QString> getLoadedPluginCLSID() = 0;
    virtual bool isPluginLoad(QString strPluginClsid) = 0;
	virtual bool isPluginInit(QString strPluginClsid) = 0;
    virtual QList<QString> getPluginClsidsByType(QString type) = 0;

    virtual bool init() = 0;
};

#endif // IUIPLUGINCONTAINER_H
