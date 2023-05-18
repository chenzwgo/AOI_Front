
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QString>
#include <QLibrary>
#include <QDesktopWidget>
#include "commonutils.h"
#include "OS_DEFINE.h"
#include "inotifyservice.h"
#include "iuicontainer.h"
#include "ilayoutservice.h"
#include "iuiplugin.h"
//#include "uicontainer.h"
#include "QWidget"
#include "imainframeuiservice.h"
#include "change.h"
//#include "proxymanager.h"
#include "istatusbar.h"
#ifdef Q_OS_WIN
    #include <windows.h>
#else
    #include <sys/time.h>
    #include <sys/select.h>
#endif
 
static QMutex shutDownMutex;
static QMutex threadMutex;
static QWaitCondition ThreadFinished;
static QWidget* m_pMainWidget = NULL;
static int threadCount = 0;

static QStringList m_AccessPlugins;

iuicontainer* CommonUtils::m_pPluginContainer = NULL;
ILayoutService* CommonUtils::m_pLayoutService = NULL;
INotifyService* CommonUtils::m_pNotifyService = NULL;



iuicontainer *CommonUtils::getPluginContainer()
{
    if (NULL == m_pPluginContainer)
    {
        QString strpath = getPluginLibraryPath();
        strpath += "/config/uiplugins.xml";

        typedef iuicontainer* (*CreateUIContainer)(const char* strPluginsFile);
        CreateUIContainer pCreateUIContainer =(CreateUIContainer) QLibrary::resolve("uicontainer", "CreateUIContainer");
         if (pCreateUIContainer != NULL)
         {

             m_pPluginContainer =  pCreateUIContainer(strpath.toStdString().c_str());
         }
    }
    return m_pPluginContainer;
}

IUIPlugin *CommonUtils::getIUIPlugin(QString strCLSID)
{
    if(m_pPluginContainer == NULL )
    {
       m_pPluginContainer =  getPluginContainer();
    }
    if(m_pPluginContainer == NULL )
    {
        return NULL;
    }

    QObject *pPlugin = m_pPluginContainer->getPlugin(strCLSID);
    IUIPlugin *pIPlugin = qobject_cast<IUIPlugin *>(pPlugin);
    return pIPlugin;
}

INotifyService *CommonUtils::getNotifyService()
{
    if (NULL == m_pNotifyService)
    {
        if (NULL != m_pPluginContainer)
        {
            QObject *pPlugin = m_pPluginContainer->getPlugin("notify");
            IPlugin *pNotifyPlugin = qobject_cast<IPlugin *>(pPlugin);
            if (NULL != pNotifyPlugin)
            {
                QObject *pInterface = NULL;
                pNotifyPlugin->queryInterface(INotifyService::IID_NOTIFYSERVICE, (void**)&pInterface);
                m_pNotifyService = qobject_cast<INotifyService *>(pInterface);
            }
        }
    }
    return m_pNotifyService;
}

ILayoutService* CommonUtils::getLayoutService()
{
    if (NULL == m_pLayoutService)
    {
        if (NULL != m_pPluginContainer)
        {
            QObject *pPlugin = m_pPluginContainer->getPlugin("layoutplugin");
            IUIPlugin *pLayoutPlugin = qobject_cast<IUIPlugin *>(pPlugin);
            if (NULL != pLayoutPlugin)
            {
                QObject *pInterface = NULL;
                pLayoutPlugin->queryInterface(ILayoutService::IID_LAYOUTSERVICE, (void**)&pInterface);
                m_pLayoutService = qobject_cast<ILayoutService *>(pInterface);
            }

        }
    }
    return m_pLayoutService;
}

IStatusBar* CommonUtils::getStatusBarServer()
{
    IStatusBar *pStatusbar = NULL;
    if (NULL != m_pPluginContainer)
    {
        QObject *pPlugin = m_pPluginContainer->getPlugin("statusbarplugin");
        IUIPlugin *pLayoutPlugin = qobject_cast<IUIPlugin *>(pPlugin);
        if (NULL != pLayoutPlugin)
        {
            QObject *pInterface = NULL;
            pLayoutPlugin->queryInterface(IStatusBar::ISTATUSBAR_ID, (void**)&pInterface);
            pStatusbar = qobject_cast<IStatusBar *>(pInterface);
        }
    }
    return pStatusbar;
}

QPoint CommonUtils::getCentrePoint()
{
    QRect rect2;
    QPoint pointCentre(0,0);
    QWidget *pWidgetMainFrame = NULL;

    qDebug ("%s: %s: %s Enter", _TIME_, __FILE__, __FUNCTION__);

    pWidgetMainFrame = getMainWindow();

    // Get the centre point of the root widget
    if (NULL == pWidgetMainFrame)
    {
        qDebug ("%s: %s: m_pwidgetParent is NULL", __FILE__, __FUNCTION__);
        rect2 = QApplication::desktop()->availableGeometry();
        qDebug ("%s: %s: rect x = %d, y = %d, w = %d, y = %d ", __FILE__, __FUNCTION__,
                rect2.x(),
                rect2.y(),
                rect2.width(),
                rect2.height());
        pointCentre = rect2.center();
        qDebug ("%s: %s: pointCentre x = %d, y = %d", __FILE__, __FUNCTION__,
                pointCentre.x(), pointCentre.y());
    }
    else
    {
        rect2 = pWidgetMainFrame->geometry();
        qDebug ("%s: %s: rect x = %d, y = %d, w = %d, y = %d ", __FILE__, __FUNCTION__,
                rect2.x(),
                rect2.y(),
                rect2.width(),
                rect2.height());
        pointCentre = rect2.center();
        qDebug ("%s: %s: pointCentre x = %d, y = %d", __FILE__, __FUNCTION__,
                pointCentre.x(), pointCentre.y());
    }

    qDebug ("%s: %s: %s Exit", _TIME_, __FILE__, __FUNCTION__);
    return pointCentre;
}


// This function is used to show the NewContact window in the middle
void CommonUtils::moveCentre (QDialog *pDialog)
{
    qDebug ("%s: %s: Enter", __FILE__, __FUNCTION__);
        /* pc lint fix */
    if (NULL == pDialog)
    {
            return;
    }
    QPoint point (getCentrePoint() - QPoint (pDialog->width() / 2,
                                             pDialog->height() / 2));
//    qDebug ("%s: %s: centre point x = %d, y = %d", __FILE__, __FUNCTION__,
//            point.x(), point.y());
    pDialog->move(point);

    qDebug ("%s: %s: Exit", __FILE__, __FUNCTION__);
}


void CommonUtils::moveRelatedCentre (QDialog *pDialog)
{
    QRect rectMainWindow;
    QWidget *pWidgetMainFrame = NULL;

   // qDebug ("%s: %s: %s Enter", _TIME_, __FILE__, __FUNCTION__);

    pWidgetMainFrame = getMainWindow();

    // 2. Get the centre point of the root widget
//    pWidgetMainFrame = NULL;
    if (NULL == pWidgetMainFrame)
    {
        qDebug ("%s: %s: m_pwidgetParent is NULL", __FILE__, __FUNCTION__);
        return;
    }
    else
    {
        rectMainWindow = pWidgetMainFrame->rect();
    }

    pDialog->move((rectMainWindow.width() - pDialog->width())/2,
                    (rectMainWindow.height() - pDialog->height())/2);

    //qDebug ("%s: %s: Exit", __FILE__, __FUNCTION__);
}

//  This function is used to get main frame window pointer
QWidget* CommonUtils::getMainWindow()
{
    FUNC_ENTER;
    IMainFrameUIService *pMainFrame = NULL;
    if (NULL != m_pPluginContainer)
    {
        IPlugin *pPlugin = qobject_cast<IPlugin *>(m_pPluginContainer->getPlugin("xframeplugin"));
        if (NULL != pPlugin)
        {
            pPlugin->queryInterface(IMainFrameUIService::IID_IMAINFRAMEUISERVICE, (void**)&pMainFrame);
            //pMainFrame = qobject_cast<IMainFrameUIService *>(pInterface);
        }
        if (NULL != pMainFrame)
        {
            return pMainFrame->getMainFrameWindow();;
        }
    }
    FUNC_LEAVE;
    return NULL;
}

bool CommonUtils::ChangeMainFrameSize()
{
    QWidget*pMainWidget = getMainWindow();
    if (pMainWidget == NULL)
    {
        return false;
    }

    QString strAppDirPath = getPluginLibraryPath ();
    strAppDirPath  = strAppDirPath + "/" + "config/runinfo.ini";
    QSettings setting(strAppDirPath, QSettings::IniFormat);
    setting.beginGroup("mainframesize");
    int height = setting.value("height","960").toInt();       //!<  warn 等级
    int width = setting.value("width","960").toInt();       //!<  erro 等级
    setting.endGroup();

    pMainWidget->resize(width, height);
    pMainWidget->setFixedSize(width, height);
    return true;
}


void CommonUtils::threadCountInc(QString file, QString func)
{
   QMutexLocker locker(&shutDownMutex);
   ++threadCount;
   qWarning ("%s: %s: %s:threadCount is %d; thread: %s: %s", _TIME_, __FILE__, __FUNCTION__, threadCount, qPrintable (file), qPrintable (func));
}

//Returns weather the Plugin is Available or not
bool CommonUtils::isPluginAvailable(QString stringPluginClsid)
{
    if (NULL != m_pPluginContainer)
    {
        return m_pPluginContainer->isPluginLoad(stringPluginClsid);
    }

    return false;
}


int CommonUtils::threadCountDec(QString file, QString func)
{
    QMutexLocker locker(&shutDownMutex);
    --threadCount;
    qWarning ("%s: %s: %s: threadCount is %d; thread: %s: %s", _TIME_, __FILE__, __FUNCTION__, threadCount, qPrintable (file), qPrintable (func));
    if (threadCount <= 0)
    {
        qWarning ("%s: %s: %s:ThreadFinished.wakeAll();", _TIME_, __FILE__, __FUNCTION__);
        ThreadFinished.wakeAll();
    }
    return threadCount;
}


void CommonUtils::waitThreadsStop()
{
    //Begin  Shutdown crash - Untill app threads quits we need to wait,  Max wait is 10Sec as statusbar thread can sleep up to 10 sec Begin
    static int i =0;
    while (threadCount > 0 && i < 20)
    {
        QMutexLocker locker(&threadMutex);
        qWarning ("%s: %s: %s: start ThreadFinished.wait", _TIME_, __FILE__, __FUNCTION__);
        ThreadFinished.wait(&threadMutex, 500); //app Close is taking time
        i++;
     }
     //End  Shutdown crash - Untill app threads quits we need to wait,Max wait is 10Sec as statusbar thread can sleep up to 10 sec  End
    qWarning ("%s: %s: %s: end ThreadFinished.wait", _TIME_, __FILE__, __FUNCTION__);
}

QString CommonUtils::getPluginLibraryPath()
{
    QString strAppPath = QApplication::applicationDirPath();
	QDir dir(strAppPath);
	strAppPath = dir.absolutePath();

#if defined(Q_WS_MAC) && defined(MP_REL_BUILD)
    QString strLibraryPath = strAppPath.replace("MacOS", "Resources");;
    //qDebug ("%s: %s: %s: mac release version plugin foler is %s returned", _TIME_, __FILE__, __FUNCTION__, qPrintable (strLibraryPath));
    return strLibraryPath;
#endif
    //qDebug ("%s: %s: %s: other os plugin folder is %s", _TIME_, __FILE__, __FUNCTION__, qPrintable (strAppPath));
    return strAppPath;
}

void CommonUtils::waitForThreadToStop(QThread* pThread, unsigned int unMaxWait)
{
	qDebug ("%s: %s: %s: Enter. unMaxWait: %u", _TIME_, __FILE__, __FUNCTION__, unMaxWait);
	if (NULL == pThread)
	{
		return;
	}

	//Set exit flag
	pThread->quit();

	//wait for thread to exit until time out
	if (pThread->isRunning())
	{
		pThread->wait(unMaxWait * 1000);
	}

	//if thread is still running, terminate
	if (pThread->isRunning())
	{
		pThread->terminate();
	}
	qDebug ("%s: %s: %s: Leave ", _TIME_, __FILE__, __FUNCTION__);
}

QWidget* CommonUtils::GetObjectByName( QString strName )
{
    qDebug ("%s: %s: %s: Object Name is %s ", _TIME_, __FILE__, __FUNCTION__,qPrintable(strName));
    if (m_pMainWidget == NULL )
    {
        m_pMainWidget = getMainWindow();
    }
    if (m_pMainWidget == NULL || strName=="")
    {
        return NULL;
    }
    QWidget *tmp = m_pMainWidget->findChild<QWidget *>(strName);
    if (tmp == NULL)
    {
        qWarning ("Don't find out Object Name is %s ",qPrintable(strName));
    }
    return tmp;
}

bool CommonUtils::ShowWidget( QString strWidgetName )
{
    //FUNC_AUTO_ENTER
    if (m_pMainWidget == NULL )
    {
        m_pMainWidget = getMainWindow();
    }
    if (m_pMainWidget == NULL|| strWidgetName == "")
    {
        return false;
    }
    QWidget* pWidget =  m_pMainWidget->findChild<QWidget *>(strWidgetName);
    if (pWidget == NULL)
    {
        qWarning ("Don't find out Widget  Name is %s ",qPrintable(strWidgetName));
        return false;
    }

    pWidget->show();
    return true;
}

bool CommonUtils::ShowPlugin(QString strPluginName)
{
    //FUNC_AUTO_ENTER;
    if (m_pPluginContainer == NULL)
    {
        return false;
    }

    IUIPlugin *pPlugin = qobject_cast<IUIPlugin *>(m_pPluginContainer->getPlugin(strPluginName));

    if (NULL == pPlugin)
    {
        qWarning ("Don't find out Plugin  Name is %s ",qPrintable(strPluginName));
        return false;
    }
    
    QRect tmpRect;
    tmpRect.setSize(QSize(590,440));

    QRect tmpRect2 = getMainWindow()->geometry();
    tmpRect.setX(tmpRect2.x()+ (tmpRect2.center().x() - tmpRect2.x())/2);
    tmpRect.setY(tmpRect2.y()+ (tmpRect2.center().y() - tmpRect2.y())/2);
    tmpRect.setWidth(tmpRect2.width() /2);
    tmpRect.setHeight(tmpRect2.height() /2);

    pPlugin->showWindow(getMainWindow(),tmpRect,true);
 
    return true;
}

bool CommonUtils::ShowPluginInCustomFrm(QString strPluginName, QWidget *pParent, QRect rect)
{
    if (m_pPluginContainer == NULL)
    {
        return false;
    }

    IUIPlugin *pPlugin = qobject_cast<IUIPlugin *>(m_pPluginContainer->getPlugin(strPluginName));

    if (NULL == pPlugin)
    {
        qWarning ("Don't find out Plugin  Name is %s ",qPrintable(strPluginName));
        return false;
    }
    pPlugin->showWindow(pParent,rect,false);
    return true;
}

QString CommonUtils::GetCurrentTimeString()
{
    return QTime::currentTime ().toString("hh:mm:ss:zzz");
}

void CommonUtils::millisleep(int ms)
{
#ifdef Q_OS_WIN
    Sleep(ms);
#else
   struct timeval tv;
   tv.tv_sec=0;
   tv.tv_usec=ms*1000;
   select(0, 0, 0, 0, &tv);
#endif
    return;
}

bool CommonUtils::CreateDir(const QString &path)
{
    if(IsDirExist(path))
    {
        return true;
    }
    else
    {
        QDir temp ;
        if(temp.mkdir(path))
        {
            return true;
        }
    }
    return false;
}

bool CommonUtils::IsDirExist(const QString &path)
{
    QDir temp ;
    return temp.exists(path);
}

bool CommonUtils::RemoveDir(const QString &path)
{
    if (path.isEmpty())
        return false;

    QDir dir(path);
    if(!dir.exists())
        return true;

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        if (fi.isFile())
            fi.dir().remove(fi.fileName());
        else
            RemoveDir(fi.absoluteFilePath());
    }
    return dir.rmdir(dir.absolutePath());
}

bool CommonUtils::CopyToFile( const QString & srcfileName, const QString & desfileName )
{
	return QFile::copy(srcfileName,desfileName);
}



QWidget *CommonUtils::GetPluginChilrdWidget(QString strPluginName, QString strWidgetName)
{
    //FUNC_AUTO_ENTER;

    QWidget* pchildMain = GetPluginMainWidget(strPluginName);
    if (pchildMain == NULL)
    {
        return NULL;
    }

    QWidget* pchild = pchildMain->findChild<QWidget *>(strWidgetName);
    if(pchild == NULL)
    {
        qWarning ("Don't find out Widget,  Widget Name is %s ",qPrintable(strWidgetName));
    }
    
    return pchild;
}

QWidget* CommonUtils::GetPluginMainWidget( QString strPluginName )
{
    if (m_pPluginContainer == NULL)
    {
        return NULL;
    }
    IUIPlugin *pPlugin = qobject_cast<IUIPlugin *>(m_pPluginContainer->getPlugin(strPluginName));

    if (NULL == pPlugin)
    {
        qWarning ("Don't find out Plugin  Name is %s ",qPrintable(strPluginName));
        return NULL;
    }

    QWidget* pMainWidget = NULL;
    if (!pPlugin->queryInterface(IUIPlugin::GET_MAIN_WIDGET_OBJECT, (void**)&pMainWidget))
    {
        return NULL;
    }
    return pMainWidget;
}



QString CommonUtils::getFileCodec(const QString& filePath)
{
    QString code = "";
    if(!QFile::exists(filePath))
    {
        return code;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return code;

    unsigned char s2[3] = {0};
    if (file.read((char*)&s2, 2) <0 )
    {
        file.close();
        return code;
    }
    file.close();
    ;//读取第一个字节，然后左移8位  
    int p = s2[0] << 8;
    p += s2[1];

  
    switch (p)//判断文本前两个字节  
    {
    case 0xfffe:  //65534  
        code = "Unicode";
        break;
    case 0xfeff://65279  
        code = "Unicode big endian";
        break;
    case 0xefbb://61371  
        code = "UTF-8";
        break;
    default:
        code = "ANSI";
    }
    return code;

}



