
#ifndef _CHANGE_H_
#define _CHANGE_H_

#include <QString>
#include <QApplication>
#include <QHash>
#include <QIODevice>
#include <QKeySequence>
#include <QTranslator>
#include "dllexport.h"
#include "uidefine.h"

#define LANGUAGE_EN_FILE     "en_us"
#define LANGUAGE_CN_FILE     "cn_zh"

#define  SKIN_BLUE      "IDS_SKIN_BLUE"
#define  SKIN_DEFAULT   "IDS_SKIN_DEFAULT"
#define  SKIN_BLANK	 "IDS_SKIN_BLANK"
#define  SKIN_GRAY	     "IDS_SKIN_GRAY"

typedef QHash <QString, QHash <QString, QString> > LanguageHash;

class RT_API Change
{
public:
    Change();
    ~Change();

    static bool ChangSkin(QString const &qstrSkin);

    static void ChangLang(const QString& qstrLangName);

    static QString GetUITranslatorPath(const QString& qstrPluginName);

    static QString tr(const QString& qstrLangID, const QString& qstrModuleName = "");

    static QString GetCurrentLang();

    static QString GetCurrentSkin();
	

private:

     static int LoadLangFile(const QString& qstrPluginName);
     static int readXmlFile(QIODevice *pDevice,const QString qstrFilePath);
     static bool WriteSkinConfig(const QString strSkin);
     static bool WriteLangConfig(const QString strLang);

     static LanguageHash m_languageHash;
};
#endif
