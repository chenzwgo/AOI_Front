#include "usermanagercfg.h"
#include "QSettings"
#include <QStringList>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>

#define USER_LIST_PATH "./plugins/usermanagementui/userlist.dat"
#define WIDGET_ID_LIST_PATH   "./plugins/usermanagementui/widgetid.xml"

UserManagerCfg* UserManagerCfg::m_pSelfInstance = NULL;

UserManagerCfg* UserManagerCfg::GetInstance()
{
    if (NULL == m_pSelfInstance)
    {
        m_pSelfInstance = new UserManagerCfg;
    }
    return m_pSelfInstance;
}

void UserManagerCfg::ReleaseInstance()
{
    if (NULL != m_pSelfInstance)
    {
        delete m_pSelfInstance;
        m_pSelfInstance = NULL;
    }
}

UserManagerCfg::UserManagerCfg()
{
    ReadUserListCfg();
    ReadWidgetInfo();
}

bool UserManagerCfg::ReadUserListCfg()
{
    QSettings qSettings(USER_LIST_PATH, QSettings::IniFormat);

    m_mapUserInfo.clear();
    QStringList modelsList = qSettings.childGroups();
    for (int i = 0; i < modelsList.size(); ++i)
    {
        qSettings.beginGroup(modelsList[i]);
        QStringList userList = qSettings.allKeys();
        QMap<QString, QString> mapUserInfo;
        for (int j = 0; j < userList.size(); ++j)
        {
            QString strValue = qSettings.value(userList[j]).toString();
            mapUserInfo.insert(userList[j], strValue);
        }
        qSettings.endGroup();
        m_mapUserInfo.insert(modelsList[i], mapUserInfo);
    }
    return true;
}

bool UserManagerCfg::ReadWidgetInfo()
{
    QFile localfile(WIDGET_ID_LIST_PATH);
    if(!localfile.open(QFile::ReadOnly))
    {
        return false;
    }

    QDomDocument dom;
    if(!dom.setContent(&localfile))
    {
        localfile.close();
        return false;
    }

    m_mapWidgetInfo.clear();

    QDomElement rootNode = dom.documentElement();
    QDomNodeList widgetList = rootNode.childNodes();

	for (int i = 0; i < widgetList.count(); ++i)
	{
		STWidgetInfo stWidgetInfo;
		ReadWidgetElement(widgetList.at(i), &stWidgetInfo);
		m_mapWidgetInfo.insert(stWidgetInfo.stWidgetItem.strObjectName, stWidgetInfo);
	}
	localfile.close();
    return true;
}

bool UserManagerCfg::ReadWidgetElement( QDomNode nodelist, STWidgetInfo* pInfo)
{
	pInfo->stWidgetItem.strObjectName     = nodelist.toElement().attribute("OBJ");
	pInfo->stWidgetItem.strDesc = nodelist.toElement().attribute("DESC");
	pInfo->stWidgetItem.strPluginName = nodelist.toElement().attribute("PLUGIN");
	pInfo->stWidgetItem.bDisable[MODE_LOGOUT] = nodelist.toElement().attribute("LOGOUT").toInt();
	pInfo->stWidgetItem.bDisable[MODE_PRODUCTION] = nodelist.toElement().attribute("PRODUCTION").toInt();
	pInfo->stWidgetItem.bDisable[MODE_DEBUG] = nodelist.toElement().attribute("DEBUG").toInt();
	pInfo->stWidgetItem.bDisable[MODE_ENGINEER] = nodelist.toElement().attribute("ENGINEER").toInt();

	QDomNodeList widgetItemList = nodelist.childNodes();
	for (int i=0; i<widgetItemList.count(); i++)
	{
		STWidgetInfo* pChild = new STWidgetInfo;
		ReadWidgetElement(widgetItemList.at(i), pChild);
		pInfo->mapItem.insert(pChild->stWidgetItem.strObjectName, pChild);
	}
	return true;
}

bool UserManagerCfg::SetUserInfo( QString strModel, const QMap<QString, QString>& mapUser )
{
    m_mapUserInfo[strModel] = mapUser;

    QSettings qSettings(USER_LIST_PATH, QSettings::IniFormat);
    qSettings.clear();

    QMap<QString, QMap<QString, QString> >::Iterator tempIter = m_mapUserInfo.begin();
    for (; tempIter != m_mapUserInfo.end(); ++tempIter)
    {
        qSettings.beginGroup(tempIter.key());

        QMap<QString, QString>::Iterator tempChildIter = tempIter.value().begin();
        for (; tempChildIter != tempIter.value().end(); ++tempChildIter)
        {
            qSettings.setValue(tempChildIter.key(), tempChildIter.value());
        }

        qSettings.endGroup();
    }
    return true;
}

bool UserManagerCfg::SaveWidgetInfo()
{
	QFile localfile1(WIDGET_ID_LIST_PATH);
	if(!localfile1.open(QFile::WriteOnly))
	{
		return false;
	}

	QDomDocument doc;  
	QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");  
	doc.appendChild(instruction);  

	QDomElement root = doc.createElement("config");//创建根节点  
	doc.appendChild(root);//添加根节点  

	QMap<QString, STWidgetInfo>::Iterator itInfo = m_mapWidgetInfo.begin();
	for (; itInfo != m_mapWidgetInfo.end(); itInfo++)
	{
		CreateElement(&doc, &root, itInfo.value());
	}

	QTextStream out1(&localfile1);
	doc.save(out1,4);
	localfile1.close();
	return true;
}

bool UserManagerCfg::CreateElement(QDomDocument* pDoc, QDomElement* pDomElement, STWidgetInfo& stInfo )
{
	QDomElement strObject = pDoc->createElement("Obj");//创建元素节点  
	strObject.setAttribute("OBJ",stInfo.stWidgetItem.strObjectName);
	strObject.setAttribute("DESC",stInfo.stWidgetItem.strDesc);
	strObject.setAttribute("PLUGIN",stInfo.stWidgetItem.strPluginName);
	
	strObject.setAttribute("LOGOUT",stInfo.stWidgetItem.bDisable[MODE_LOGOUT]); 
	strObject.setAttribute("PRODUCTION",stInfo.stWidgetItem.bDisable[MODE_PRODUCTION]); 
	strObject.setAttribute("DEBUG",stInfo.stWidgetItem.bDisable[MODE_DEBUG]); 
	strObject.setAttribute("ENGINEER",stInfo.stWidgetItem.bDisable[MODE_ENGINEER]);

	if (stInfo.mapItem.size() > 0)
	{
		QMap<QString, STWidgetInfo*>::Iterator itWidgetInfo = stInfo.mapItem.begin();
		for (; itWidgetInfo != stInfo.mapItem.end(); itWidgetInfo++)
		{
			CreateElement(pDoc, &strObject, *itWidgetInfo.value());
		}
	}
	pDomElement->appendChild(strObject);//添加元素节点到根节点
	return true;
}
