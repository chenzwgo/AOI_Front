#ifndef HTTPCLIENTIMP_H
#define HTTPCLIENTIMP_H

#include "dllexport.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QList>
#include <QSslError>
 #include <QTimer>

class RT_API HttpClientImp:public QObject
{
    Q_OBJECT
public:
    HttpClientImp();
    ~HttpClientImp();
    bool startRequest(QUrl url);
    bool startRequest(QUrl url,QString strVerb ,const QByteArray & data );
    bool IsError();
    bool IsFinish();
    QString getErrorString(); 
    QString getReplyContent();
    QList<QByteArray> getRequestHeader();
    QList<QByteArray> getResponseHeader();
    void SetUserAndPwd(QString struser,QString strpwd);
    void SetProxyInfo(QString strhostname,QString strport,QString strUser,QString strpwd);
    void SetTimeOut(int timeoutms);
signals:
    void Requestfinish();
private:
    void resetStatus();
    void combRequestHeader(QNetworkRequest* pRequest);
    void combResponsHeader( QNetworkReply* pReply );
    void FinishEnd();
    QString m_strSettingPath;
    QNetworkAccessManager* m_pNetAccess;
    QNetworkRequest*       m_pNetRequest;
    QNetworkReply*         m_pNetReply;
    QNetworkProxy*         m_pNetProxy;
    QUrl                   m_qurl;

    QString m_strUrl;
    QString m_strError;
    QString m_strReplyContent;
    QString m_strUser;
    QString m_strPwd;
    QString m_strRequestType;
    QList<QByteArray> m_strRequestHeader;
    QList<QByteArray> m_strResponseHeader;
    QTimer        *m_ptimeOut;
    bool m_bError;
    bool m_bFinish;
    bool m_bTimeout;
    int m_iTimeoutms;
    int m_AuthCount;
       

private slots:
    void slotAuthenticationRequired( QNetworkReply * reply, QAuthenticator * authenticator );
    //void slotfinished ( QNetworkReply * reply );
    void slotnetworkAccessibleChanged ( QNetworkAccessManager::NetworkAccessibility accessible );
    void slotproxyAuthenticationRequired ( const QNetworkProxy & proxy, QAuthenticator * authenticator );
    void slotsslErrors ( QNetworkReply * reply, const QList<QSslError> & errors );

    void slotReplyfinished ();
    void slotReplyReadyRead();
    void slotReplydownloadProgress ( qint64 bytesReceived, qint64 bytesTotal )    ; 
    void slotReplyuploadProgress ( qint64 bytesSent, qint64 bytesTotal );
    void handleTimeOut();
};

#endif // HTTPCLIENTIMP_H
