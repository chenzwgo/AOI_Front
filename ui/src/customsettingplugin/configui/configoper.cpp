#include "configoper.h"
#include <QSettings>
#include <QTextCodec>
#include <QVector>

#define CFG_PATH			"./plugins/customsettingplugin/usersetting.ini"

static QMutex g_CfgMutex;
ConfigOper* ConfigOper::m_pInstance = NULL;
ConfigOper::ConfigOper()
{
	ReadFileCfg();
	InitConfig();
}

ConfigOper::~ConfigOper()
{
}

ConfigOper* ConfigOper::GetInstance()
{
	QMutexLocker locker(&g_CfgMutex);
	if (m_pInstance == NULL)
	{
		m_pInstance = new ConfigOper;
	}
	return m_pInstance;
}

void ConfigOper::ReleaseInstance()
{
	QMutexLocker locker(&g_CfgMutex);
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

SettingInfo* ConfigOper::GetSetttingInfo( QString beginGroup )
{
	if (m_mapSettingInfo.end() == m_mapSettingInfo.find(beginGroup))
	{
		return NULL;
	}
	return &m_mapSettingInfo[beginGroup];
}

bool ConfigOper::IsSameSetting(const SettingInfo* pSetting)
{
	if (pSetting == NULL)
		return false;
	
	bool bSame = true;
	QMap<QString, SettingInfo>::iterator mapSettingInfo;
	mapSettingInfo = m_mapSettingInfo.find(pSetting->strBeginGroup);
	if (mapSettingInfo != m_mapSettingInfo.end())
	{
		if (mapSettingInfo->listSetting.size() != pSetting->listSetting.size())
		{
			bSame = false;
		}
		else
		{
			QList<SettingValue>::iterator itValue;
			for (itValue = mapSettingInfo->listSetting.begin(); itValue != mapSettingInfo->listSetting.end(); itValue++)
			{
				bool bFind = false;
				QList<SettingValue>::const_iterator itSetting;
				for (itSetting = pSetting->listSetting.begin(); itSetting != pSetting->listSetting.end(); itSetting++)
				{
					if (0 == itSetting->strKey.compare(itValue->strKey))
					{
						bFind = true;
						break;
					}
				}

				if (!bFind)
				{
					bSame = false;
					break;
				}
			}
		}
	}
	return bSame;
}

bool ConfigOper::SetSetttingInfo( const SettingInfo* pSetting )
{
	if (pSetting == NULL)
		return false;

	QMap<QString, SettingInfo>::iterator mapSettingInfo;
	mapSettingInfo = m_mapSettingInfo.find(pSetting->strBeginGroup);
	if (mapSettingInfo == m_mapSettingInfo.end())
	{
		m_mapSettingInfo.insert(pSetting->strBeginGroup, *pSetting);
		mapSettingInfo = m_mapSettingInfo.find(pSetting->strBeginGroup);
		if (mapSettingInfo == m_mapSettingInfo.end())
		{
			return false;
		}
	}
	else
	{
		if (!IsSameSetting(pSetting))
		{
			mapSettingInfo->listSetting = pSetting->listSetting;
		}
	}

	for (int i=0; i<pSetting->listSetting.size(); i++)
	{
		if (0 == pSetting->listSetting[i].strKey.compare(mapSettingInfo->listSetting[i].strKey))
		{
			if (0 != mapSettingInfo->listSetting[i].strDisable.compare("0"))
			{
				mapSettingInfo->listSetting[i].strValue = pSetting->listSetting[i].strValue;
			}
		}
	}

	QSettings setting(CFG_PATH, QSettings::IniFormat);
	setting.setIniCodec("GB2312");
	setting.beginGroup(mapSettingInfo->strBeginGroup);

	for (int i=0; i<mapSettingInfo->listSetting.size(); i++)
	{
		QStringList strList;
		strList.append(mapSettingInfo->listSetting[i].strValue);
		if (mapSettingInfo->listSetting[i].strCtrlType.size() != 0)
		{
			strList.append(mapSettingInfo->listSetting[i].strCtrlType);
		}
		if (mapSettingInfo->listSetting[i].strDes.size() != 0)
		{
			strList.append(mapSettingInfo->listSetting[i].strDes);
		}
		if (mapSettingInfo->listSetting[i].strDisable.size() != 0)
		{
			strList.append(mapSettingInfo->listSetting[i].strDisable);
		} 
		setting.setValue(mapSettingInfo->listSetting[i].strKey, strList);
	}

	setting.endGroup();
	InitConfig();
	return true;
}

void ConfigOper::ReadFileCfg()
{
	QSettings setting(CFG_PATH, QSettings::IniFormat);
	setting.setIniCodec("GB2312");
	QStringList beginGroupList = setting.childGroups();

	foreach(QString key1,beginGroupList)
	{
		SettingInfo stSettingInfo(key1);
		setting.beginGroup(key1);
		QStringList keyValuelist =setting.allKeys();
		QMap<QString, QStringList> keyValueMap;
		foreach(QString key2,keyValuelist)
		{
			SettingValue stSettingValue(key2);
			QStringList keyValue = setting.value(key2).toStringList();
			if (keyValue.size() > 0)
			{
				stSettingValue.strValue = keyValue.at(0);
			}
			if (keyValue.size() > 1)
			{
				stSettingValue.strCtrlType = keyValue.at(1);
			}
			if (keyValue.size() > 2)
			{
				stSettingValue.strDes = keyValue.at(2);
			}
			if (keyValue.size() > 3)
			{
				stSettingValue.strDisable = keyValue.at(3);
			}

			stSettingInfo.listSetting.push_back(stSettingValue);
		}
		setting.endGroup();

		InsertMap(key1, stSettingInfo);
	}
}

void ConfigOper::InsertMap( QString key, const SettingInfo& stSetting )
{
	if (m_mapSettingInfo.end() == m_mapSettingInfo.find(key))
	{
		m_mapSettingInfo.insert(key, stSetting);
	}
}

QVariant ConfigOper::getValue( QString beginGroup, QString key)
{
	QMap<QString, SettingInfo>::iterator itBeginGroup;

	itBeginGroup = m_mapSettingInfo.find(beginGroup);
	if (itBeginGroup != m_mapSettingInfo.end())
	{
		QList<SettingValue>::iterator itKeyValue;
		for (itKeyValue = itBeginGroup->listSetting.begin(); itKeyValue != itBeginGroup->listSetting.end(); itKeyValue++)
		{
			if (0 == itKeyValue->strKey.compare(key))
			{
				return itKeyValue->strValue;
			}
		}
	}
	return "";
}

bool ConfigOper::setValue( QString beginGroup, QString key, QString value )
{
	QMap<QString, SettingInfo>::iterator itBeginGroup;

	itBeginGroup = m_mapSettingInfo.find(beginGroup);
	if (itBeginGroup != m_mapSettingInfo.end())
	{
		QList<SettingValue>::iterator itKeyValue;
		for (itKeyValue = itBeginGroup->listSetting.begin(); itKeyValue != itBeginGroup->listSetting.end(); itKeyValue++)
		{
			if (0 == itKeyValue->strKey.compare(key))
			{
				itKeyValue->strValue = value;
				return true;
			}
		}
	}
	return false;
}

bool ConfigOper::saveGroup( QString beginGroup )
{
	QMap<QString, SettingInfo>::iterator mapSettingInfo;
	mapSettingInfo = m_mapSettingInfo.find(beginGroup);
	if (mapSettingInfo == m_mapSettingInfo.end())
		return false;

	QSettings setting(CFG_PATH, QSettings::IniFormat);
	setting.setIniCodec("GB2312");
	setting.beginGroup(mapSettingInfo->strBeginGroup);

	for (int i=0; i<mapSettingInfo->listSetting.size(); i++)
	{
		QStringList strList;
		strList.append(mapSettingInfo->listSetting[i].strValue);
		if (mapSettingInfo->listSetting[i].strCtrlType.size() != 0)
		{
			strList.append(mapSettingInfo->listSetting[i].strCtrlType);
		}
		if (mapSettingInfo->listSetting[i].strDes.size() != 0)
		{
			strList.append(mapSettingInfo->listSetting[i].strDes);
		}
		if (mapSettingInfo->listSetting[i].strDisable.size() != 0)
		{
			strList.append(mapSettingInfo->listSetting[i].strDisable);
		} 
		setting.setValue(mapSettingInfo->listSetting[i].strKey, strList);
	}
	setting.endGroup();
	InitConfig();
	return true;
}


ComConfig* ConfigOper::GetLightCom()
{
	return &m_stLightCom;
}

void ConfigOper::InitConfig()
{
    string strListComPort[] = {"COM1","COM2","COM3","COM4","COM5","COM6","COM7","COM8","COM9","COM10"};
    int listBaudRate[] = {9600,19200,38400,56000,57600,115200,128000,256000};
    int listByteSize[] = {4,5,6,7,8};
	int listStopBit[] = {1,3,2};
	m_stLightCom.name = strListComPort[getValue("LightCom", "name").toInt()];
	m_stLightCom.baudRate = listBaudRate[getValue("LightCom", "baudrate").toInt()];
	m_stLightCom.parity = getValue("LightCom", "parity").toInt();
	m_stLightCom.byteSize = listByteSize[getValue("LightCom", "bytesize").toInt()];
	m_stLightCom.stopBits = listStopBit[getValue("LightCom", "stopbits").toInt()];

    m_stLightDownCom.name = strListComPort[getValue("LightCom_2", "name_2").toInt()];
    m_stLightDownCom.baudRate = listBaudRate[getValue("LightCom_2", "baudrate_2").toInt()];
    m_stLightDownCom.parity = getValue("LightCom_2", "parity_2").toInt();
    m_stLightDownCom.byteSize = listByteSize[getValue("LightCom_2", "bytesize_2").toInt()];
    m_stLightDownCom.stopBits = listStopBit[getValue("LightCom_2", "stopbits_2").toInt()];

	/*m_CameraCfg.Fivscount = getValue("CameraParam", "Fivscount").toInt();
	m_CameraCfg.Rivscount = getValue("CameraParam", "Rivscount").toInt();
	m_CameraCfg.dMoveUnit = getValue("CameraParam", "MoveUnit").toDouble();
	m_CameraCfg.dPluseWidgh = getValue("CameraParam", "PluseWidgh").toDouble();
	m_CameraCfg.dFstartpos = getValue("CameraParam", "Fstartpos").toDouble();
	m_CameraCfg.dRstartpos = getValue("CameraParam", "Rstartpos").toDouble();
	m_CameraCfg.MoveLenth = getValue("CameraParam", "MoveLenth").toDouble();
	m_CameraCfg.dTriggerBegin = getValue("CameraParam", "dTriggerBegin").toDouble();
	m_CameraCfg.dSpeed = getValue("CameraParam", "dSpeed").toDouble();*/
	
}

ComConfig* ConfigOper::GetLightDownCom()
{
    return &m_stLightDownCom;
}

//CameraParam* ConfigOper::GetCmeraPara()
//{
//	return &m_CameraCfg;
//}
