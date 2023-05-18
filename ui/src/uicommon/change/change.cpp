
#include "change.h"
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QIODevice>
#include <QXmlStreamReader>
#include <QChar>
#include <QHashIterator>
#include <QApplication>
#include <QStyleFactory>
#include "commonutils.h"
#include "iuicontainer.h"
#include "iuiplugin.h"
#include <QTranslator>
#include <QMutex>


static QString m_qstrLangName = "";
static QString m_qstrSkin = "";
LanguageHash Change::m_languageHash;

QMutex  g_mutex;
void Change::ChangLang(const QString& qstrLangName)
{
    g_mutex.lock();
    m_qstrLangName = qstrLangName;
    QHashIterator <QString, QHash <QString, QString> > i(m_languageHash);
    while (i.hasNext()) 
    {
        i.next();
        LoadLangFile(i.key ());
    }
	g_mutex.unlock();
    iuicontainer* pPluginContainer = CommonUtils::getPluginContainer();
    if(pPluginContainer == NULL)
    {
        return ;
    }

    QList<QString> pluginlist = pPluginContainer->getLoadedPluginCLSID();
    QString strplugin = "";
    foreach (strplugin, pluginlist)
    {
        if(strplugin.length() >0)
        {
            QObject* pplugin = pPluginContainer->getPlugin(strplugin);
            IUIPlugin *pUIPlugin = qobject_cast<IUIPlugin *>(pplugin);
            if (pUIPlugin != NULL)
            {
                pUIPlugin->SetLanguage(qstrLangName);
            }
        }
    }
    WriteLangConfig(qstrLangName);
    
}

QString Change::GetUITranslatorPath(const QString& qstrPluginName)
{

    QString strModule = CommonUtils::getPluginLibraryPath();
    QString strLangFilePath = strModule + "/language/" + qstrPluginName + "/" + qstrPluginName + "_" + m_qstrLangName + LANG_POSTFIX;

    qDebug("%s:%d(%s) LangFile is \"%s\" ",__FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));

    if (!QFile::exists(strLangFilePath))
    {
        return "";
    }
    return strLangFilePath;
}


#ifdef Q_WS_MAC
#include <QMacStyle>
#endif

bool Change::ChangSkin( QString const &qstrSkin)
{
	
    QString qstrSkinPath = QApplication::applicationDirPath() + "/skin/" + qstrSkin + ".qss";
    QFile fileStyle (qstrSkinPath);
    if (QFile::exists(qstrSkinPath) && (fileStyle.open(QIODevice::ReadOnly)))
    {
#ifdef Q_WS_MAC
      QApplication::setStyle (new QMacStyle ());
#else

#endif

        QString strStyle;
        QTextStream in(&fileStyle);
		strStyle = in.readAll();
        //while (!in.atEnd())
        //{
        //    strStyle.append(in.readLine());
        //}
        fileStyle.close ();
        ((QApplication*)(QCoreApplication::instance()))->setStyleSheet (strStyle);
        m_qstrSkin = qstrSkin;
    }
    else
    {

#ifdef Q_WS_MAC
        qDebug ("%s: %s: File does not exist %s... setting mac style...", __FILE__, __FUNCTION__, qPrintable (qstrSkinPath));
        ((QApplication*)(QCoreApplication::instance()))->setStyle (new QMacStyle ());
        return true;
#endif
        QString styletmp = "* {font-family:Arial;}";
        ((QApplication*)(QCoreApplication::instance()))->setStyleSheet(styletmp);
        qDebug ("%s: %s: File does not exist or failed to open %s", __FILE__, __FUNCTION__, qPrintable (qstrSkinPath));
        return false;
    }
    WriteSkinConfig(qstrSkin);
    return true;
}



QString Change::GetCurrentLang()
{
    QString strAppDirPath = QApplication::applicationDirPath ();
    strAppDirPath  = strAppDirPath + "/" + "config/runinfo.ini";
    QSettings setting(strAppDirPath, QSettings::IniFormat);
    setting.beginGroup("language");
    m_qstrLangName = setting.value("current_language",LANGUAGE_EN_FILE).toString();
    setting.endGroup();
    return  m_qstrLangName;
}


QString Change::GetCurrentSkin()
{
    QString strAppDirPath = QApplication::applicationDirPath ();
    strAppDirPath  = strAppDirPath + "/" + "config/runinfo.ini";
    QSettings setting(strAppDirPath, QSettings::IniFormat);
    setting.beginGroup("skin");
    m_qstrSkin= setting.value("current_skin",SKIN_BLUE).toString();
    setting.endGroup();
    return m_qstrSkin;
}

int Change::LoadLangFile(const QString& qstrPluginName)
{
    QString qstrPathName = CommonUtils::getPluginLibraryPath() + "/language/" + qstrPluginName + "/" + qstrPluginName + "_" + m_qstrLangName + ".lang";

    qDebug ("%s: %s: loading %s", __FILE__, __FUNCTION__, qPrintable (qstrPathName));

    //Open the file
    QFile cfgFile(qstrPathName);
    if (!QFile::exists(qstrPathName))
    {
        qDebug ("File %s does not exist", qPrintable (qstrPathName));
        return false;
    }
    if (!cfgFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error open file:" << qstrPathName;
        return false;
    }
     //In read file, all content should be saved in memory
    int iRet = readXmlFile(&cfgFile,qstrPluginName);
    cfgFile.close();
    return iRet;
}

int Change::readXmlFile(QIODevice *pDevice,const QString qstrPluginName)
{
    if(!pDevice)
    {
        return 0;
    }
    QXmlStreamReader  xmlReader;
    xmlReader.setDevice(pDevice);

    while(!xmlReader.atEnd())
    {
        xmlReader.readNext();
        if(xmlReader.isStartElement())
        {
            if(xmlReader.name() == "item")
            {
                //Language id
                QString qstrID    = xmlReader.attributes().value("name").toString();
                QString qstrText  = xmlReader.readElementText();
                qstrText.remove ("\\n");
                qstrText.remove ("\\r");
                qstrText.replace ("\\t","            ");
                qstrText.remove ("\\v");
                qstrText.remove ("\\f");


                m_languageHash[qstrPluginName][qstrID] = qstrText;
            }
        }
    }
    return 1;
}


QString Change::tr(const QString& qstrLangID, const QString& qstrModuleName)
{
    g_mutex.lock();  
    QString strTmp = m_languageHash[qstrModuleName][qstrLangID];
    if(strTmp == "")
    {
        LoadLangFile(qstrModuleName);
        strTmp = m_languageHash[qstrModuleName][qstrLangID];
    }

    g_mutex.unlock();
    return strTmp;
}

bool Change::WriteSkinConfig( const QString strSkin )
{
    QString strAppDirPath = QApplication::applicationDirPath ();
    strAppDirPath  = strAppDirPath + "/" + "config/runinfo.ini";
    QSettings setting(strAppDirPath, QSettings::IniFormat);
    setting.beginGroup("skin");
    setting.setValue("current_skin",strSkin);
    m_qstrSkin = strSkin;
    setting.endGroup();
    return true;
}

bool Change::WriteLangConfig( const QString strLang )
{
    QString strAppDirPath = QApplication::applicationDirPath ();
    strAppDirPath  = strAppDirPath + "/" + "config/runinfo.ini";
    QSettings setting(strAppDirPath, QSettings::IniFormat);
    setting.beginGroup("language");
    setting.setValue("current_language",strLang);
    m_qstrLangName = strLang;
    setting.endGroup();
    return  true;
}



