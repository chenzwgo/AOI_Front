#ifndef CONFIGOPER_H
#define CONFIGOPER_H

#include <QStringList>
#include "businesscommon.h"
#include <QMap>
#include <QVariant>
#include <QMutex>
#include "settingdefine.h"

#define CONFIG_INSTANCE ConfigOper::GetInstance()

struct VisionInfo
{
    QString	visionIP;		// vision ip
    int		visionPort;		// vision port
    QString	localIP;
    int		localPort;
    VisionInfo()
    {
        visionIP = "192.168.1.10";
        visionPort = 8080;
        localIP = "192.168.1.12";
        localPort = 8000;
    }

    VisionInfo& operator=(const VisionInfo &other)
    {
        if (this == &other)
        {
            return *this;
        }
        visionIP = other.visionIP;
        visionPort = other.visionPort;
        localIP = other.localIP;
        localPort = other.localPort;

        return *this;
    }
};
class ConfigOper
{
public:
	static ConfigOper* m_pInstance;
	static ConfigOper* GetInstance();
	static void ReleaseInstance();

	
	ComConfig* GetLightCom();
    ComConfig* GetLightDownCom();
	//CameraParam* GetCmeraPara();

    SettingInfo* GetSetttingInfo(QString beginGroup);
    bool SetSetttingInfo(const SettingInfo* pSetting);

private:
    QVariant getValue(QString beginGroup, QString key);
	bool setValue(QString beginGroup, QString key, QString value);

private:
	ConfigOper();
	~ConfigOper();
	void ReadFileCfg();
	void InsertMap(QString key, const SettingInfo& stSetting);
	void InitConfig();
	bool IsSameSetting(const SettingInfo* pSetting);
	bool saveGroup(QString beginGroup);

private:
    QMutex		m_workLocker;	
    QMap<QString, SettingInfo> m_mapSettingInfo;
    QMap<QString,QMap<QString, QStringList>> m_mapSettingCfg;

	 ComConfig	m_stLightCom;
     ComConfig	m_stLightDownCom;
	CameraParam m_CameraCfg;
};

#endif // CONFIGOPER_H
