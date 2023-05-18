
#ifndef UI_DEFINE_H
#define UI_DEFINE_H

#include <QtGui>
#include <guid.h>
#include <QString>


#define _TIME_ qPrintable (QTime::currentTime ().toString ("hh:mm:ss:zzz"))
#define _DATETIME_ qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz"))


class QFunEnterTrace
{
public:
    QFunEnterTrace(QString msgFile,int msgLine,QString msgFun)
    {
        m_msgFile = msgFile;
        m_msgLine = msgLine;
        m_msgFun = msgFun;

        qDebug("%s: %s:%d %s enter",_TIME_,m_msgFile.toStdString().c_str(),m_msgLine,m_msgFun.toStdString().c_str());
    }
    ~QFunEnterTrace()
    {
        qDebug("%s: %s:%d %s Leave",_TIME_,m_msgFile.toStdString().c_str(),m_msgLine,m_msgFun.toStdString().c_str());
    }
private:
    QString m_msgFile;
    int m_msgLine;
    QString m_msgFun;
};



#if defined(RT_DBG_BUILD)

    #ifndef UI_SAFE_DELETE
        #define UI_SAFE_DELETE(_X_)   if (_X_) { delete (_X_); (_X_) = NULL;}
    #endif    

    #define FUNC_ENTER qDebug("%s:%s:%d %s enter",_TIME_,__FILE__,__LINE__,__FUNCTION__);
    #define FUNC_LEAVE qDebug("%s:%s:%d %s leave",_TIME_,__FILE__,__LINE__,__FUNCTION__);
    #define FUNC_AUTO_ENTER  QFunEnterTrace QFunEnterTraceTmp(__FILE__,__LINE__,__FUNCTION__);
    #define	qVerbose			   qDebug
#else
    #define UI_SAFE_DELETE(_X_)   if (_X_) delete (_X_)
    #define UI_SAFE_DELETE(_X_) if (_X_) { delete (_X_); (_X_) = NULL; }
    #define FUNC_ENTER
    #define FUNC_LEAVE
    #define FUNC_AUTO_ENTER
    #ifdef _WINDOWS
        #pragma warning(disable:4002)
        #define	qVerbose(_X_)
    #else
        #define	qVerbose(_X_...)
    #endif
#endif

#define CHECK_NULL(_X_) \
    if (NULL == _X_) \
    { \
        qDebug ("Arg is NULL: %s: %s: %s %d", _TIME_, __FILE__, __FUNCTION__, __LINE__); \
        return; \
    }

#define CHECK_NULL_RETURN_FALSE(_X_) \
    if (NULL == _X_) \
    { \
        qDebug ("Arg is NULL: %s: %s: %s %d", _TIME_, __FILE__, __FUNCTION__, __LINE__); \
        return false; \
    }



// If in mac, set all dialog as modal dialog
#ifdef Q_WS_MAC
    #define SET_MODALITY(x) x->setModal (true)
#else
    #define SET_MODALITY(x) x->setWindowModality (Qt::WindowModal)
#endif
#define SET_WINDOW_MODALITY(x)	x->setWindowModality (Qt::WindowModal)


#define LANG_POSTFIX                      ".qm"
#define SKIN_POSTFIX                      ".qss"

enum UILogLevel   // UI打印log级别
{
    UILOG_INFO = 0, //提示(黑色)
    UILOG_WARN,     //警告(蓝色)
    UILOG_ERR,      //错误(红色)
    UILOG_POP,      //弹框(黑色,弹出对话框内容也黑色)
    UILOG_POPRED,   //弹框(红色,弹出对话框内容也红色)
};

#endif // DEFINE_H
