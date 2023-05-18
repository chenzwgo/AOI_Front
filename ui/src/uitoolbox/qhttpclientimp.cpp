#include "uitoolbox/qhttpclientimp.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

HttpClientImp::HttpClientImp():m_pNetAccess(NULL),
                               m_pNetReply(NULL),
                               m_strUrl(""),
                               m_strError(""),
                               m_strReplyContent(""),
                               m_strUser(""),
                               m_strPwd(""),
                               m_strRequestType("get"),
                               m_bError(false),
                               m_bFinish(false),
                               m_bTimeout(false),
                               m_iTimeoutms(-1)
{
    m_pNetAccess = new QNetworkAccessManager();
    m_ptimeOut = new QTimer();
    connect(m_pNetAccess, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
        this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(m_pNetAccess, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
        this, SLOT(slotsslErrors(QNetworkReply*,QList<QSslError>)));
    connect(m_pNetAccess, SIGNAL(networkAccessibleChanged ( QNetworkAccessManager::NetworkAccessibility )),
        this, SLOT(slotnetworkAccessibleChanged  ( QNetworkAccessManager::NetworkAccessibility)));
    connect(m_pNetAccess, SIGNAL(proxyAuthenticationRequired ( const QNetworkProxy & , QAuthenticator *  )),
        this, SLOT(slotproxyAuthenticationRequired(const QNetworkProxy & , QAuthenticator *  )));
    connect(m_ptimeOut, SIGNAL(timeout()), this, SLOT(handleTimeOut()));

}


HttpClientImp::~HttpClientImp()
{
    if (m_pNetAccess != NULL)
    {
        delete m_pNetAccess;
        m_pNetAccess = NULL;
    }
    
}

bool HttpClientImp::startRequest( QUrl url )
{
    resetStatus();
    m_qurl = url; 
    m_strUrl = url.toString();
    QNetworkRequest requesttmp = QNetworkRequest(m_qurl);
    combRequestHeader(&requesttmp); 
    m_pNetReply = m_pNetAccess->get(requesttmp);
    
    connect(m_pNetReply, SIGNAL(finished()),
        this, SLOT(slotReplyfinished()));
    connect(m_pNetReply, SIGNAL(readyRead()),
        this, SLOT(slotReplyReadyRead()));
    connect(m_pNetReply, SIGNAL(downloadProgress(qint64,qint64)),
        this, SLOT(slotReplydownloadProgress(qint64,qint64)));
    connect(m_pNetReply, SIGNAL(uploadProgress(qint64,qint64)),
        this, SLOT(slotReplyuploadProgress(qint64,qint64)));
    return true;
}

bool HttpClientImp::startRequest( QUrl url,QString strVerb /*= "get"*/,const QByteArray & data )
{

    m_qurl = url; 
    QNetworkRequest requesttmp = QNetworkRequest(m_qurl);
    resetStatus();
    if(strVerb.compare("get",Qt::CaseInsensitive) == 0)
    {
        m_pNetReply = m_pNetAccess->get(requesttmp);
    }
    else if (strVerb.compare("head",Qt::CaseInsensitive) == 0)
    {
        m_pNetReply = m_pNetAccess->head(requesttmp);
    }
    else if (strVerb.compare("post",Qt::CaseInsensitive) == 0)
    {
        m_pNetReply = m_pNetAccess->post(requesttmp,data);
    }
    else if (strVerb.compare("put",Qt::CaseInsensitive) == 0)
    {
        m_pNetReply = m_pNetAccess->post(requesttmp,data);
    }
    else 
    {
        return false;
    }
    if(m_iTimeoutms != -1 && !m_ptimeOut->isActive())
    {
        m_ptimeOut->setInterval(m_iTimeoutms);
        m_bTimeout = false;
        m_ptimeOut->start();
    }

     combRequestHeader(&requesttmp); 
    connect(m_pNetReply, SIGNAL(finished()),
        this, SLOT(slotReplyfinished()));
    connect(m_pNetReply, SIGNAL(readyRead()),
        this, SLOT(slotReplyReadyRead()));
    connect(m_pNetReply, SIGNAL(downloadProgress(qint64,qint64)),
        this, SLOT(slotReplydownloadProgress(qint64,qint64)));
    connect(m_pNetReply, SIGNAL(uploadProgress(qint64,qint64)),
        this, SLOT(slotReplyuploadProgress(qint64,qint64)));
    return true; 
}


void HttpClientImp::slotAuthenticationRequired( QNetworkReply * reply, QAuthenticator * authenticator )
{
    if (m_AuthCount >= 10)
    {
        return;
    }  
    authenticator->setUser(m_strUser);
    authenticator->setPassword(m_strPwd);
    m_AuthCount++;
}

void HttpClientImp::slotnetworkAccessibleChanged( QNetworkAccessManager::NetworkAccessibility accessible )
{
    
}

void HttpClientImp::slotproxyAuthenticationRequired( const QNetworkProxy & proxy, QAuthenticator * authenticator )
{
   
}

void HttpClientImp::slotsslErrors( QNetworkReply * reply, const QList<QSslError> & errors )
{

}

void HttpClientImp::slotReplyfinished()
{
    if(m_ptimeOut->isActive())
    {
        m_ptimeOut->stop();
    }
    if(m_bTimeout)
    {
        return;
    }
    if (m_pNetReply == NULL)
    {
        FinishEnd();
        return;
    }
    if (m_pNetReply->error() != QNetworkReply::NoError)
    {
        m_bError = true;   
        m_strError = m_pNetReply->errorString();
    }
    else
    {
        m_bError = false;
        m_strError = "";
        QVariant redirectionTarget = m_pNetReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        if (!redirectionTarget.isNull())
        {
            QUrl newUrl = m_qurl.resolved(redirectionTarget.toUrl());
            m_qurl = newUrl;
            m_pNetReply->deleteLater();
            startRequest(newUrl);        
            return;
        }       
    }
    FinishEnd();
    
}

void HttpClientImp::slotReplyReadyRead()
{
    if (m_pNetReply == NULL)
    {
        return;
    }
    m_strReplyContent.append(m_pNetReply->readAll());
    
}

void HttpClientImp::slotReplydownloadProgress( qint64 bytesReceived, qint64 bytesTotal )
{

}

void HttpClientImp::slotReplyuploadProgress( qint64 bytesSent, qint64 bytesTotal )
{

}

void HttpClientImp::handleTimeOut()
{
    m_ptimeOut->stop();
    m_bTimeout= true;

    m_bError = true;
    m_strError = "TimeOut :" +QString::number(m_iTimeoutms);
    FinishEnd();

}

void HttpClientImp::resetStatus()
{
     m_strUrl = "";
     m_strError = "";
     m_strReplyContent = "";

     m_bError = false;
     m_bFinish = false;
     m_bTimeout= false;
     m_AuthCount = 0;
}

bool HttpClientImp::IsError()
{
    return m_bError;
}

bool HttpClientImp::IsFinish()
{
    return m_bFinish;
}

QString HttpClientImp::getErrorString()
{
    return m_strError;
}

QString HttpClientImp::getReplyContent()
{
    return m_strReplyContent;
}

void HttpClientImp::SetUserAndPwd( QString struser,QString strpwd )
{
    m_strUser =struser;
    m_strPwd =strpwd;
}

void HttpClientImp::SetProxyInfo( QString strhostname,QString strport,QString strUser,QString strpwd )
{
    QNetworkProxy prxoytmp ;
    prxoytmp.setHostName(strhostname);
    prxoytmp.setPort(strport.toInt());
    prxoytmp.setUser(strUser);
    prxoytmp.setPassword(strpwd);
    if (m_pNetAccess != NULL)
    {
        m_pNetAccess->setProxy(prxoytmp);
    }
    
}

void HttpClientImp::SetTimeOut(int timeoutms)
{
    m_iTimeoutms = timeoutms;
}

QList<QByteArray> HttpClientImp::getRequestHeader()
{
    return m_strRequestHeader;
}

QList<QByteArray> HttpClientImp::getResponseHeader()
{
    return m_strResponseHeader;
}

void HttpClientImp::combRequestHeader( QNetworkRequest* pRequest )
{
    if (pRequest == NULL)
    {
        return;
    }
    m_strRequestHeader.clear();
    m_strRequestHeader =pRequest->rawHeaderList();
    m_strRequestHeader.push_front(m_qurl.toString().toLatin1());


    //QList<QPair<QString, QString> > tmplist= m_qurl.queryItems () ;
    //QList<QPair<QByteArray, QByteArray> > tmplist2= m_qurl.encodedQueryItems();
    //QString tmp = m_qurl.authority();
    //tmp = m_qurl.host();
    //tmp = m_qurl.userName();
    //tmp = m_qurl.password();
    //tmp = m_qurl.scheme();
    //tmp = m_qurl.path();
    //tmp = m_qurl.toLocalFile();
    //tmp = m_qurl.userInfo();
    //tmp = m_qurl.topLevelDomain();
    //tmp = m_qurl.fragment();
    //tmp = m_qurl.errorString();
       
}

void HttpClientImp::FinishEnd()
{

    if (m_pNetReply != NULL)
    {
        combResponsHeader(m_pNetReply);
        m_pNetReply->deleteLater();
        m_pNetReply = NULL;

    }
    m_bFinish = true;
    m_AuthCount = 0;
    emit Requestfinish();
    
}

void HttpClientImp::combResponsHeader( QNetworkReply* pReply )
{
    m_strResponseHeader.clear();
    QList<QNetworkReply::RawHeaderPair> tmplist =  pReply->rawHeaderPairs();
    
    for (int index = 0 ;index < tmplist.size();index++)
    {
        QNetworkReply::RawHeaderPair paritemp = tmplist.at(index);
        QByteArray onerecord = paritemp.first;
        onerecord += " : ";
        onerecord += paritemp.second;
        m_strResponseHeader.append(onerecord);
    }
     
    return;
}























