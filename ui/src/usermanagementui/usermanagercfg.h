#ifndef USERMANAGERCFG_H
#define USERMANAGERCFG_H
#include "QMap"
#include <QString>
#include <QVector>
#include <QStringList>
#include <QList>
#include <QDomNodeList>
#include <QMap>
#include "uibusinesscomm.h"

class UserManagerCfg
{
public: 
    static UserManagerCfg* GetInstance();
    static void            ReleaseInstance();
public:
    QMap<QString, QMap<QString, QString> >* GetUserInfo(){return &m_mapUserInfo;}
    QMap<QString, STWidgetInfo>* GetWidgetID(){return &m_mapWidgetInfo;};

    bool SetUserInfo(QString strModel, const QMap<QString, QString>& mapUser);
	bool SaveWidgetInfo();
private:
    UserManagerCfg();
    bool ReadUserListCfg();
	bool ReadWidgetInfo();
	bool ReadWidgetElement(QDomNode nodelist, STWidgetInfo* pInfo);
	bool CreateElement(QDomDocument* pDoc, QDomElement* pDomElement, STWidgetInfo& stInfo);
private:
    static UserManagerCfg* m_pSelfInstance;

    QMap<QString, QMap<QString, QString> > m_mapUserInfo;
    QMap<QString, STWidgetInfo>  m_mapWidgetInfo;
};

#endif // USERMANAGERCFG_H
